#include "Shader_Defines.hlsli"

/* ------------ �� �� �� �� ----------------- */
// ���� ORM�ؽ��ĸ� Occulusuin, Roughness, Metalic �̶�� �ϴµ� 
// �츮�� ORM�ؽ��İ� �ֱ⶧���� R (Roughness), G (Metallic), B (Ambient Occlusion) ���� �����;��Ѵ�. 
/* ----------------------------------------- */

#define F_ZERO          float3(0.04f, 0.04f, 0.04f)
#define PI              3.14159265359f
#define TWO_PI          6.28318530718f
#define PI_OVER_TWO     1.5707963268f
#define EPSILON         0.000001f
/* ----------------- Variable ----------------- */
TextureCube g_IrradianceTexture;
TextureCube g_PreFilteredTexture;
Texture2D   g_BRDFTexture;
float4      g_LightDiffuse = { 1.f, 1.f, 1.f, 1.f };
/* -------------------------------------------- */

// Burley B. "Physically Based Shading at Disney"
// SIGGRAPH 2012 Course: Practical Physically Based Shading in Film and Game Production, 2012.
float3 Disney_Diffuse(in float roughnessPercent, in float3 diffuseColor, in float NdotL, in float NdotV, in float LdotH)
{
    float energyBias = lerp(0.0f, 0.5f, roughnessPercent);
    float energyFactor = lerp(1.0f, 1.0f / 1.51f, roughnessPercent);
   
    float fd90 = energyBias + 2.0f * roughnessPercent * LdotH * LdotH;
    
    float lightScatter = 1.0f + (fd90 - 1.0f) * pow(1.0f - NdotL, 5.0f);
    float viewScatter = 1.0f + (fd90 - 1.0f) * pow(1.0f - NdotV, 5.0f);
    
    return diffuseColor * lightScatter * viewScatter * energyFactor;
    
    //float albedoLuminosity = 0.3 * diffuseColor.r
    //            + 0.6 * diffuseColor.g
    //            + 0.1 * diffuseColor.b;
    //        // normalize luminosity to isolate hue and saturation
    //float3 albedoTint = albedoLuminosity > 0 ?
    //                            diffuseColor / albedoLuminosity :
    //                            float3(1, 1, 1);
    //
    //float fresnelL = pow(clamp(1 - NdotL, 0, 1), 5);
    //float fresnelV = pow(clamp(1 - NdotV, 0, 1), 5);
    //
    //float fresnelDiffuse = 0.5 + 2 * pow(LdotH, 2.f) * roughnessPercent;
    //
    //float diffuse = albedoTint
    //                        * lerp(1.0, fresnelDiffuse, fresnelL)
    //                        * lerp(1.0, fresnelDiffuse, fresnelV);
    //
    //float fresnelSubsurface90 = pow(LdotH, 2.f) * roughnessPercent;
    //
    //float fresnelSubsurface = lerp(1.0, fresnelSubsurface90, fresnelL)
    //                                * lerp(1.0, fresnelSubsurface90, fresnelV);
    //
    //float ss = 1.25 * (fresnelSubsurface * (1 / (NdotL + NdotV) - 0.5) + 0.5);
    //
    //return saturate(lerp(diffuse, ss, 0.5f /*_Subsurface*/) * (1 / PI) * diffuseColor);
}


float D_GGX(float roughness, float NoH, const float3 NxH)
{
    float a = NoH * roughness;
    float k = roughness / (dot(NxH, NxH) + a * a);
    float d = k * k * (1.0 / PI);
    return min(d, 65504.0);
}

// GGX Specular D (normal distribution)
// https://www.cs.cornell.edu/~srm/publications/EGSR07-btdf.pdf
float D_GGX(in float roughness2, in float NdotH)
{
    float alpha = roughness2 * roughness2;
    const float NdotH2 = NdotH * NdotH; // NdotH2 = NdotH^2
   
    //alpha = pow(alpha, 2.f); // �𸮾��� 4����

    const float lower = (NdotH2 * (alpha - 1.0f)) + 1.0f;
    //const float lower = NdotH2 * alpha + (1.0f - NdotH2);
    return alpha / (PI * lower * lower);   
}

// Shlick's approximation of Fresnel By Unity Engine
float3 F_Shlick(in float3 specularColor, in float HdotV)
{
    float FC = pow(1.0f - HdotV, 5.0f);
    return specularColor + (float3(1.0f, 1.0f, 1.0f) - specularColor) * FC;
}

// Schlick-Smith specular G (visibility) By Unity Version
float G_Smith(float roughness2, float NdotV, float NdotL)
{
    float SmithV = NdotL * sqrt(NdotV * (NdotV - NdotV * roughness2) + roughness2);
    float SmithL = NdotV * sqrt(NdotL * (NdotL - NdotL * roughness2) + roughness2);
    
    return 0.5f / max(SmithV + SmithL, 1e-5f);
}

float GGX_Geometry(float cosThetaN, float roughness4)
{
    float cosTheta_sqr = saturate(cosThetaN * cosThetaN);
    float tan2 = (1.0f - cosTheta_sqr) / cosTheta_sqr;
    
    return 2.0f / (1.0f + sqrt(1.0f + roughness4 * tan2));
}

float G_GGX(in float roughness2, in float NdotV, in float NdotL)
{
    float alpha = roughness2 * roughness2;
    
    return GGX_Geometry(NdotV, alpha) * GGX_Geometry(NdotL, alpha);
}

float G_SmithShlick(in float roughness2, in float NdotV, in float NdotL)
{
    float r = sqrt(roughness2) + 1.0f;
    float k = (r * r) / 8.0f;
    
    float SmithV = NdotV / (NdotV * (1.0 - k) + k);
    float SmithL = NdotL / (NdotL * (1.0 - k) + k);
    
    return SmithV * SmithL;
}

/*=============================================================
 
                             �ٸ��� 
                                
==============================================================*/

// 3. Fresnel Equation - �����ڹ�����
// �츮�� ���� ������ ���� �ݻ����� �������� ����Ѵ�. 
// -> �ſ� ������ �������� ������ ������ ����(Schlick)�� �ٻ������ �����ϰ� ���� �� �ִ�.
float3 fresnelSchlick(float cosTheta, float3 F0)
{
    // ������ �ٴڱ��� �� ���̰� �� ���� �ſ�ó�� �ݻ�Ǿ� ���̴°�
    // ���� �ٸ� ������ �浹�� �� ���� ������ �����ǰ� ���� ������ �ݻ簡 �Ǵµ� 
    // �Ի簢�� ���� �ݻ�, ������ ������ �޶����� ������ ǥ���Ѱ��� ������ �������̴�. 
    // ���� ����ϱ⿡�� �ſ� �����ؼ� PBR������ �ٻ�ȭ�� ������ ����Ѵ� = ������ �ٻ��
   
    // costheta�� (half way ���� * view����)�� ���̴°��̴�. 
    return F0 + (1.0f - F0) * pow(1.0 - cosTheta, 5.0f);
    
}

float3 FresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
    return F0 + (max(float3(1.0 - roughness, 1.0 - roughness, 1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0f);
    float NdotH2 = NdotH * NdotH;
	
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;
	
    return num / denom;
}

// 2. Geometry Function 
// Roughness�� ���ڷ� �޾Ƽ� ǥ���� ���� ���ļ� ���� �����ϴ� ������ �ٻ�ġ�� ��������� ���Ѵ�. 
// k�� direct lighting�̳� IBL Lighting�̳Ŀ� ���� �޶��� �� �ִ�. 
float GeometrySchlickGGX(float NdotV, float roughness) // k is a remapping of roughness based on direct lighting or IBL lighting
{
    // k : roughness���� remapping (direct light, IBL �� ��쿡 ���� �ٸ�)
    // �׸��� �ƿ�ǲ�� �󸶳� �̼���׸��ڰ� ���ִ����� ���´�. 
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0; // direct
    
    //float k = (roughness * roughness) / 2.0f; // IBL 

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
// 
float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

float GeometrySmith(float NdotV, float NdotL, float roughness)
{
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

// 1. Normal Distribution Function (NDF)
// half way vector h �� ��ġ�ϴ� microfacets�� ǥ�� ������ �ٻ�ġ�� ���Ѵ�. 
// roughness�� 0�� ������ �ſ� ���� �ݰ濡 ���ߵǾ� halfway vector�� ��ġ�ϴ� microfacets�� �ٻ�ġ�� ���Ѵ�. -> �ſ� ���� ������ ��Եȴ�. 
// 1�� ������ �� ���� halfway vector�� ��ġ�ϰ����� �ڱत �� �ݰ濡 ���ߵ����ʾƼ� ��� Ư���κγ��� ��û �������� ��°� �ƴ϶� ��ü������ ȸ�� ������ ��Եȴ�. 
float NormalDistributionGGXTR(float NdotH, float roughness)
{
    // roughness���� Ŭ���� highlight�Ǵ� ������ �о����鼭 ���������. 
    float roughness2 = roughness * roughness;
    float NdotH2 = NdotH * NdotH;

    float nom = roughness2;
    float denom = (NdotH2 * (roughness2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;

    return nom / denom;
}

float3 FresnelSchlickFrostbite(float3 F0, float F90, float u)
{
    return F0 + (F90 - F0) * pow(1 - u, 5);
}

float DisneyFrostbiteDiff(in float roughness, in float3 diffuseColor, in float NdotL, in float NdotV, in float LdotH)
{
    float energyBias = lerp(0.f, 0.5f, roughness);
    float energyFactor = lerp(1.0f, 1.0f / 1.51f, roughness);
    float Fd90 = energyBias + 2.0f * pow(LdotH, 2.f) * roughness;
    float3 F0 = float3(1.f, 1.f, 1.f);
    float lightScatter = FresnelSchlickFrostbite(F0, Fd90, NdotL).r;
    float viewScatter = FresnelSchlickFrostbite(F0, Fd90, NdotV).r;
    return (lightScatter * viewScatter * energyFactor * diffuseColor) / PI;
}

// GGX Specular D
float Specular_D_GGX(in float roughness2, in float NdotH)
{
    const float lower = (NdotH * (NdotH * roughness2 - NdotH)) + 1.0f;
    return roughness2 / (PI * lower * lower);
}


// Shlick's approximation of Fresnel By Unity Engine
float3 Specular_F_Fresnel_Shlick_Unity(in float3 specularColor, in float LdotH)
{
    float FC = pow(1.0f - LdotH, 5.0f);
    return specularColor + (1.0f - specularColor) * FC;
}

float3 Specular_BRDF(in float roughness2, in float3 specularColor, in float NdotH, in float NdotV, in float NdotL, in float LdotH)
{
    // Specular D
    float specular_D = Specular_D_GGX(roughness2, NdotH);
    
    // Specular G
    float specular_G = G_Smith(roughness2, NdotV, NdotL);
    
    // Specular F
    float3 specular_F = Specular_F_Fresnel_Shlick_Unity(specularColor, LdotH);

    return (specular_D * specular_G) * specular_F;
}

/*=============================================================
 
                             BRDF 
                                
==============================================================*/

// Result = BRDF / ����� �ݻ�����Լ� 
// ���� �ΰ��� �ް� �� ���迡 ���� �ݻ��� (�ݻ籤 / �Ի籤)�� ����ϴ� �Լ�. 
// Mtrl�� �ݻ�� ���� �Ӽ��� �ٻ�ġ�� micofacet�̷��� ��Ÿ���� ���Ѵ�. 
// ��κ� �ǽð� ���������������ο����� Cook-Torrance BRDF�� ����Ѵ�. 

float3 BRDF(in float roughness2, in float fMetallic, in float3 vDiffuseColor, in float3 vSpecularColor, in float3 N, in float3 V, in float3 L, in float3 H)
{
    const float NdotL = max(dot(N, L), EPSILON);
    const float NdotV = max(dot(N, V), EPSILON);
    const float NdotH = max(dot(N, H), EPSILON);
    const float HdotV = max(dot(H, V), EPSILON);
    
    // Distribution & Geometry & Fresnel
    float D = D_GGX(roughness2, NdotH);
    float G = G_GGX(roughness2, NdotV, NdotL);
    float3 F = F_Shlick(vSpecularColor, HdotV);
    
    float3 kS = F;
    float3 kD = float3(1.0, 1.0, 1.0) - kS;
    kD *= 1.0 - fMetallic;
    
    // Diffuse & Specular factors
    float denom = max(4.0f * NdotV * NdotL, 0.001f); // 0.001f just in case product is 0
    float3 specular_factor = saturate((D * F * G) / denom); // Cook-Torrance Specula
    float3 diffuse_factor = kD * vDiffuseColor / PI; // Lambertian diffuse - diffuse���� ��� ���� ��� BRDF ���Ŀ��� �� ���̰� ���� 
    // -> EPIC GAMES���� ��ǥ�� ����� ������ Lambertian diffuse�� ��κ��� ����Ÿ�� ���������� ����ϴٰ� ��.
    // -> Diffuse BRDF�� ������ ������Ģ�� �����ϰԵǸ� ���� ���Ŀ� ���� Diffuse���� Albedo(RBG ä�δ� ǥ�� �ݻ���)�� �������� ����ϰԵȴ�. 
    // �̰�� �˺����� ���밪�� ������ �ƴ϶� �����̱⶧���� "������"�� �ǹ̷� ���⋚���� �ٸ� �Ķ���͸� ���ؼ� ��⸦ �����ؾߵȴ�. 
    // ������ ���Ұ�� /PI �̱⶧���� Diffuse�� 1/3���� �پ��Եȴ�. 
    
    return (diffuse_factor + specular_factor) * NdotL;
    
}

//float3 PBR_DirectionalLight(in float3 V, in float3 N, in DirectionalLight light,
//    in float3 albedo, in float ambientOcclusion, in float roughness, in float metallic, in float shadow)
//{
//    // Output color
//    float3 acc_color = float3(0.0f, 0.0f, 0.0f);
    
//    // Burley roughness bias
//    const float roughness2 = max(roughness * roughness, 0.01f);
    
//    // Blend base colors
//    const float3 c_diff = lerp(albedo, float3(0, 0, 0), metallic) * ambientOcclusion;
//    const float3 c_spec = lerp(F_ZERO, albedo, metallic) * ambientOcclusion;

//    // Calculate Directional Light
//    const float3 L = normalize(-light.Direction);
//    const float3 H = normalize(V + L);
    
//    // products
//    const float NdotL = max(dot(N, L), EPSILON);
//    const float NdotV = abs(dot(N, V)) + EPSILON;
//    const float NdotH = max(dot(N, H), EPSILON);
//    const float HdotV = max(dot(H, V), EPSILON);

//    // BRDF
//    float3 brdf_factor = BRDF(roughness2, metallic, c_diff, c_spec, NdotH, NdotV, NdotL, HdotV);
    
//    // Directional light
//    acc_color += light.Diffuse.rgb * light.Power * shadow * brdf_factor;

//    return acc_color;
//}


float3 New_BRDF(in float fRoughness, in float fMetallic, in float3 vDiffuseColor, in float3 F0, in float3 N, in float3 V, in float3 L, in float3 H, in float fAO)
{
    const float NdotL = max(dot(N, L), EPSILON); // Normal vector [dot] Light vector  -> ����� costheta�̴�. N�� L�� ����ȭ�� �����̹Ƿ� �� ���̰� 1�̱� ������. 
    const float NdotV = max(dot(N, V), EPSILON); // 
    const float NdotH = max(dot(N, H), EPSILON); // Normal vector [dot] halfway vector  
    const float HdotV = max(dot(H, V), EPSILON); // halfway vector [dot] view vector
    
    float3 R = reflect(-V, N);
    
    // EpicGame's Unreal Engine4 �� ä������ PBR : Trowbridge-Reitz GGX(D), Fresnel-Schlick(F), �׸��� Smith's Schlick-GGX(G)
    // pbr���� ���Ǵ� D, F, G �� �������� ������ ���� �� �ִ�. ���ӵ��� �߽��Ѵٴ��� ���������� ������ΰ� ���� �����ϱ� �����̴�. 
    float D = NormalDistributionGGXTR(NdotH, fRoughness); // roughness���� ���� �ݻ����
    float3 F = fresnelSchlick(HdotV, F0); // �ٸ� ������ ���� �� �޶����� �ݻ����� - �ݼ����� ������ ũ��. 
    float G = GeometrySmith(NdotV, NdotL, fRoughness); // �̼��� �׸���
    
    float3 kS = F;
    float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
    kD *= (1.0f - fMetallic);
    
    // Diffuse & Specular factors
    //float denom = max(4.0f * NdotV * NdotL, 0.001f); // 0.001f just in case product is 0
    float3 numerator = D * G * F;
    float denom = 4.0f * NdotV * NdotL;
    float3 specular_factor = numerator / max(denom, 0.001);
    
    float3 vRadiance = g_LightDiffuse.xyz * EPSILON /*attenuation*/;
    float3 Lo = (kD * vDiffuseColor / PI + specular_factor) * vRadiance * NdotL;
    
    float3 F1 = FresnelSchlickRoughness(NdotV, F0, fRoughness);
    
    kS = F1;
    kD = 1.0 - kS;
    kD *= 1.0 - fMetallic;
    
    float3 vIrradiance = g_IrradianceTexture.Sample(ClampSampler, N).rgb;
    float3 vDiffuse = vIrradiance * vDiffuseColor.xyz;
    
    const float MAX_REFLECTION_LOD = 9.0f;
    
    float3 prefilteredColor = g_PreFilteredTexture.SampleLevel(ClampSampler, R, fRoughness * MAX_REFLECTION_LOD).rgb;
    float2 envBRDF = g_BRDFTexture.Sample(ClampSampler, float2(NdotV, fRoughness)).rg;
    float3 specular = prefilteredColor * (F1 * envBRDF.x + envBRDF.y);
    
    float3 vAmbient = (kD * vDiffuse + specular) * fAO;
    
    float3 vColor = vAmbient + Lo;
    
    return vColor;
}

float3 MY_BRDF(in float fRoughness, in float fMetallic, in float3 vDiffuseColor, in float3 F0, in float3 N, in float3 V, in float3 L, in float3 H, in float fAO)
{
    const float NdotL = saturate(dot(N, L));
    const float NdotV = saturate(dot(N, V));
    const float NdotH = saturate(dot(N, H));
    const float HdotV = saturate(dot(H, V));
    
    // EpicGame's Unreal Engine4 �� ä������ PBR : Trowbridge-Reitz GGX(D), Fresnel-Schlick(F), �׸��� Smith's Schlick-GGX(G)
    // pbr���� ���Ǵ� D, F, G �� �������� ������ ���� �� �ִ�. ���ӵ��� �߽��Ѵٴ��� ���������� ������ΰ� ���� �����ϱ� �����̴�. 
    float D = NormalDistributionGGXTR(NdotH, fRoughness); // roughness���� ���� �ݻ����
    float G = GeometrySmith(NdotV, NdotL, fRoughness); // �̼��� �׸���
    float3 F = fresnelSchlick(HdotV, F0); // �ٸ� ������ ���� �� �޶����� �ݻ����� - �ݼ����� ������ ũ��. 
    
    float3 vColor = F;
    
    return vColor;
}