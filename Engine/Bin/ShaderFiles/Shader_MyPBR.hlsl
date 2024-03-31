#ifndef SHADER_DEFINES_HLSLI
#include "Shader_Defines.hlsli"
#define SHADER_DEFINES_HLSLI
#endif

/* ----------------- Variable ----------------- */
float4      g_SunColor = { 1.f, 1.f, 1.f, 0.f };
TextureCube g_Irradiance;
TextureCube g_PreFiltered;
Texture2D   g_BRDF_Texture;
/*=============================================================
 
                            D, G, F
                                
==============================================================*/

// 1. Normal Distribution Function (NDF)
// half way vector h �� ��ġ�ϴ� microfacets�� ǥ�� ������ �ٻ�ġ�� ���Ѵ�. 
// roughness�� 0�� ������ �ſ� ���� �ݰ濡 ���ߵǾ� halfway vector�� ��ġ�ϴ� microfacets�� �ٻ�ġ�� ���Ѵ�. -> �ſ� ���� ������ ��Եȴ�. 
// 1�� ������ �� ���� halfway vector�� ��ġ�ϰ����� �ڱत �� �ݰ濡 ���ߵ����ʾƼ� ��� Ư���κγ��� ��û �������� ��°� �ƴ϶� ��ü������ ȸ�� ������ ��Եȴ�. 
float My_NormalDistributionGGXTR(float3 n, float3 h, float a)
{
    float a2 = a * a;
    float NdotH = saturate(dot(n, h));
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;

    return nom / denom;
}


// 2. Geometry Function 
// Roughness�� ���ڷ� �޾Ƽ� ǥ���� ���� ���ļ� ���� �����ϴ� ������ �ٻ�ġ�� ��������� ���Ѵ�. 
// k�� direct lighting�̳� IBL Lighting�̳Ŀ� ���� �޶��� �� �ִ�. 
float My_GeometrySchlickGGX(float NdotV, float roughness)  // k is a remapping of roughness based on direct lighting or IBL lighting
{
    // k : roughness���� remapping (direct light, IBL �� ��쿡 ���� �ٸ�)
    // �׸��� �ƿ�ǲ�� �󸶳� �̼���׸��ڰ� ���ִ����� ���´�. 
    float r = roughness + 1.0f;
    float k = (r * r) / 8.0f; // direct

    //float k = (roughness * roughness) / 2.0f; // Ibl

    float nom = NdotV;
    float denom = NdotV * (1.0f - k) + k;

    return nom / denom;
}
  
float My_GeometrySmith(float3 n, float3 v, float3 l, float k)
{
    //  Geometry Obstruction
    float NdotV = saturate(dot(n, v));
    //  Geometry Shadowing
    float NdotL = saturate(dot(n, l));
    
    //float k = (roughness * roughness) / 2.0f; // IBL 

    float ggx2 = My_GeometrySchlickGGX(NdotV, k);
    float ggx1 = My_GeometrySchlickGGX(NdotL, k);

    return ggx1 * ggx2;
}

// 3. Fresnel Equation - �����ڹ�����
// �츮�� ���� ������ ���� �ݻ����� �������� ����Ѵ�. 
// -> �ſ� ������ �������� ������ ������ ����(Schlick)�� �ٻ������ �����ϰ� ���� �� �ִ�.
float3 My_fresnelSchlick(float cosTheta, float3 F0)
{
    // ������ �ٴڱ��� �� ���̰� �� ���� �ſ�ó�� �ݻ�Ǿ� ���̴°�
    // ���� �ٸ� ������ �浹�� �� ���� ������ �����ǰ� ���� ������ �ݻ簡 �Ǵµ� 
    // �Ի簢�� ���� �ݻ�, ������ ������ �޶����� ������ ǥ���Ѱ��� ������ �������̴�. 
    // ���� ����ϱ⿡�� �ſ� �����ؼ� PBR������ �ٻ�ȭ�� ������ ����Ѵ� = ������ �ٻ��
   
    // costheta�� (half way ���� * view����)�� ���̴°��̴�. 
    return (F0 + (1.0f - F0) * pow(1.0 - cosTheta, 5.0f));
}

/*=============================================================
 
                             IBL 
                                
==============================================================*/
float3 FresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
    return F0 + (max(float3(1.0 - roughness, 1.0 - roughness, 1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

/*=============================================================
 
                             BRDF 
                                
==============================================================*/

// Result = BRDF / ����� �ݻ�����Լ� 
// ���� �ΰ��� �ް� �� ���迡 ���� �ݻ��� (�ݻ籤 / �Ի籤)�� ����ϴ� �Լ�. 
// Mtrl�� �ݻ�� ���� �Ӽ��� �ٻ�ġ�� micofacet�̷��� ��Ÿ���� ���Ѵ�. 
// ��κ� �ǽð� ���������������ο����� Cook-Torrance BRDF�� ����Ѵ�. 

float3 MY_BRDF(in float fRoughness, in float fMetallic, in float3 vAlbedo, in float3 F0, in float3 N, in float3 V, in float3 L, in float3 H)
{
    //Result 
    float3 vColor = { 1.f, 1.f, 1.f };
    
    //  calculate light radiance
    //float3 lightDir = -LightDirection;                    // Deferred���� L���Ҷ� �̹� �����
    //float3 halfwayVec = normalize(viewDir + lightDir);    // Deferred���� H ���ҋ� �̹� �����
    float3 radiance = g_SunColor.rgb;

    //const float NdotL = saturate(dot(N, L));
    //const float NdotV = saturate(dot(N, V));
    //const float NdotH = saturate(dot(N, H));
    const float HdotV = saturate(dot(H, V));
    
    //  Cook-Torrance specular BRDF
    // EpicGame's Unreal Engine4 �� ä������ PBR : Trowbridge-Reitz GGX(D), Fresnel-Schlick(F), �׸��� Smith's Schlick-GGX(G)
    // pbr���� ���Ǵ� D, F, G �� �������� ������ ���� �� �ִ�. ���ӵ��� �߽��Ѵٴ��� ���������� ������ΰ� ���� ����ϱ� �����̴�. 
    float   D = My_NormalDistributionGGXTR(N, H, fRoughness); // roughness���� ���� �ݻ����
    float   G = My_GeometrySmith(N, V, L, fRoughness); // �̼��� �׸���
    float3  F = My_fresnelSchlick(HdotV, F0); // �ٸ� ������ ���� �� �޶����� �ݻ����� - �ݼ����� ������ ũ��.  costheta�� (half way ���� * view����)�� ���̴°��̴�. 
    
    float3 nominator = D * G * F;

    //  Wo : View Direction
    //  Wi : Light Direction
    float WoDotN = saturate(dot(V, N));
    float WiDotN = saturate(dot(L, N));
    float denominator = (4 * WoDotN * WiDotN);

    // 0.001f just in case product is 0
    //float3 specular_factor = nominator / (denominator + 0.001f);
    float3 specular_factor = nominator / (denominator + 0.001f); //max(denominator, 0.001);

    //  Energy Conservation
    float3 kS = F; //  reflection energy
    float3 kD = 1.0f - kS; //  refraction energy
    kD *= 1.0 - fMetallic; //  if metallic nullify kD

    //  Calculate Cook-Torrance Reflectance Equation 
    vColor = (((kD * vAlbedo / PI) + specular_factor) * radiance * WiDotN);
    // (kD * vDiffuseColor / PI) = Cook-Torrance Diffuse
    // specular_factor = BRDF�����ϰ� �߰������� ��� 
    // radiance = ī�޶� SOLID ANGLE�� ���� ���·� ������ ���� �� -> �������Ϳ� ���� �ݴ������ dot
    // WiDotN
    
    return vColor;
}


float3 MY_BRDF_Irradiance(in float fRoughness, in float fMetallic, in float3 vAlbedo,
in float3 F0, in float fAO,
in float3 N, in float3 V, in float3 L, in float3 H)
{
    //Result 
    float3 vColor = { 1.f, 1.f, 1.f };
    
    //  calculate light radiance
    //float3 lightDir = -LightDirection;                    // Deferred���� L���Ҷ� �̹� �����
    //float3 halfwayVec = normalize(viewDir + lightDir);    // Deferred���� H ���ҋ� �̹� �����
    float3 radiance = g_SunColor.rgb;

    //const float NdotL = saturate(dot(N, L));
    //const float NdotV = saturate(dot(N, V));
    //const float NdotH = saturate(dot(N, H));
    const float HdotV = saturate(dot(H, V));
    
    //  Cook-Torrance specular BRDF
    // EpicGame's Unreal Engine4 �� ä������ PBR : Trowbridge-Reitz GGX(D), Fresnel-Schlick(F), �׸��� Smith's Schlick-GGX(G)
    // pbr���� ���Ǵ� D, F, G �� �������� ������ ���� �� �ִ�. ���ӵ��� �߽��Ѵٴ��� ���������� ������ΰ� ���� ����ϱ� �����̴�. 
    float D = My_NormalDistributionGGXTR(N, H, fRoughness); // roughness���� ���� �ݻ����
    float G = My_GeometrySmith(N, V, L, fRoughness); // �̼��� �׸���
    float3 F = My_fresnelSchlick(HdotV, F0); // �ٸ� ������ ���� �� �޶����� �ݻ����� - �ݼ����� ������ ũ��.  costheta�� (half way ���� * view����)�� ���̴°��̴�. 
    
    float3 nominator = D * G * F;

    //  Wo : View Direction
    //  Wi : Light Direction
    float WoDotN = saturate(dot(V, N));
    float WiDotN = saturate(dot(L, N));
    float denominator = (4 * WoDotN * WiDotN);

    // 0.001f just in case product is 0
    //float3 specular_factor = nominator / (denominator + 0.001f);
    float3 specular_factor = nominator / (denominator + 0.001f); //max(denominator, 0.001);

    //  Energy Conservation
    float3 kS = F; //  reflection energy
    float3 kD = 1.0f - kS; //  refraction energy
    kD *= 1.0 - fMetallic; //  if metallic nullify kD

    //  Calculate Cook-Torrance Reflectance Equation 
    float3 Lo = (((kD * vAlbedo / PI) + specular_factor) * radiance * WiDotN);
    // (kD * vDiffuseColor / PI) = Cook-Torrance Diffuse
    // specular_factor = BRDF�����ϰ� �߰������� ��� 
    // radiance = ī�޶� SOLID ANGLE�� ���� ���·� ������ ���� �� -> �������Ϳ� ���� �ݴ������ dot
    // WiDotN
    
    /* ---- MY_BRDF_Irradiance Map �� PBR�� ���� ---- */
    kS = FresnelSchlickRoughness(max(dot(N, V), 0.8f), F0, fRoughness);
    kD = float3(1.f, 1.f, 1.f) -kS;
    kD *= 1.0 - fMetallic;
    
    float3 Irradiance = g_Irradiance.Sample(ClampSampler, N).rgb;
    
    float3 Diffuse = vAlbedo * Irradiance;
    //float3 ambient = (kD * Diffuse) * fAO;
    
    const float MAX_REF_LOD = 4.0f;
    
    float3 R = reflect(-V, N);
    
    float3 prefiltered = g_PreFiltered.SampleLevel(ClampSampler, R, fRoughness * MAX_REF_LOD).rgb;
    float2 brdf = g_BRDF_Texture.Sample(ClampSampler, float2(max(dot(N, V), 0.0f), fRoughness)).rg;
    float3 specular = prefiltered * (kS * brdf.x + brdf.y);

    float3 ambient = (kD * Diffuse + specular) * fAO;
    vColor = ambient + Lo;
    
    return vColor;
}
