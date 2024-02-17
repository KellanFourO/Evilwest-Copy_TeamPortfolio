#include "Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

/* ssao */
vector  g_OffsetVector[26];
vector  FrustumCorner[4];
Texture2D NormalDepthMap;
Texture2D RandomVecMap;

matrix  ViewToTexSpcace;

/* godray */


/* ------------------- Vertex Shader(0) -------------------*/

struct VS_SSAO_IN
{
    float3 vPosition : POSITION; /* ���°� */ 
    float2 ToFarPlaneIndex : NORMAL;
    float3 vTexcoord : TEXCOORD;
};

struct VS_SSAO_OUT
{
    float4 vPosition : SV_POSITION;
    float3 ToFarPlane : TEXCOORD0;
    float2 vTexcoord : TEXCOORD1;
};


VS_SSAO_OUT VS_SSAO(VS_SSAO_IN In)
{
    VS_SSAO_OUT Out = (VS_SSAO_OUT) 0;
    
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(float4(In.vPosition, 1.0f), matWVP);
    
    Out.ToFarPlane = FrustumCorner[In.ToFarPlaneIndex.x].xyz;
    
    Out.vTexcoord = In.vTexcoord;
    
    return Out;
}

/* ------------------- Pixel Shader (0) -------------------*/

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float3 ToFarPlane : TEXCOORD0;
    float2 vTexcoord : TEXCOORD1;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

// ���հ��� ã�� ���
float OcclusionFunction(float distZ)
{
    float gSurfaceEpsilon = 0.05f;
    float gOcclusionFadeStart = 0.2f;
    float gOcclusionFadeEnd = 2.0f;
    float occlusion = 0.0f;
    
    if (distZ > gSurfaceEpsilon)
    {
        float fadeLength = gOcclusionFadeEnd - gOcclusionFadeStart;
        
        // DistZ�� �����Կ� ���� ����� ���� 1���� 0���� ���������� �����Ѵ�.
        occlusion = saturate((gOcclusionFadeEnd - distZ) / fadeLength);
    }
    
    return occlusion;
}

PS_OUT PS_SSAO(PS_IN In) : SV_Target
{
    PS_OUT Out = (PS_OUT) 0;
    
	// p -- the point we are computing the ambient occlusion for.
	// n -- normal vector at p.
	// q -- a random offset from p.
	// r -- a potential occluder that might occlude p.
    
    float gOcclusionRadius = 0.5f;
    
    float4 normalDepth = NormalDepthMap.Sample(SSAONormalDepth, In.vTexcoord, 0.0f);
    
    // ī�޶� ������ �ȼ���ǥ�� Z ��
    float3 n = normalDepth.xyz;
    float pz = normalDepth.w;
    
    // p = t * In.ToFarPlane.
	// p.z = t * In.ToFarPlane.z
	// t = p.z / pin.ToFarPlane.z
    
    float3 p = (pz / In.ToFarPlane.z) * In.ToFarPlane;
    
    // ���� ���� ���� ���� ���ϰ� ���� [0, 1]���� [-1, 1] ������ �����Ѵ�.
    // ���� ���� Ÿ�ϸ��� ����.
    float3 randVec = 2.0f * RandomVecMap.Sample(SSAORandVec, 4.0f * In.vTexcoord, 0.0f).rgb - 1.0f;
    
    // ��� ���� ��
    float occlusionSum = 0.0f;
    
    // ���� �� ����
    const int sampleCount = 14;
   
    // n ���� �������� �� p�� �ݱ� ������ �ִ� ���� ���� ���ø��ϴ� ���� �Ϲ������� 8/16/32�� ����������.
    // ���⼭�� 14���� ���� ����, �� 14���� ���ø� ���� ���� �ȴ�.
    [unroll]
    for (int i = 0; i < sampleCount; ++i)
    {
        // ���� ���ʹ� �����ǰ� �����ϰ� �л�ȴ�
        // �ֳ��ϸ� ������ ���� ���⿡�� �����ϱ� ������ ���� ���͸� ����Ͽ� �ݿ��ϸ� �������� ������ ���� �л� ���͸� ��´�.
        // ������ ���͸� ����Ͽ� �л��ϰ� �����Ѵ�.
        // ť�� ���ʹ� ���� ���ͷ� ���Ǿ� �ݻ� ���͸� ���� ���� ���ͷ� ã�´�.
        float3 offset = reflect(g_OffsetVector[i].xyz, randVec);
        
        // ���� ���Ͱ� ǥ�� �ڿ� �ִ� ��� Flip�� �� q�� �׻� ������ ����� p���鿡 �ֵ��� ���� ���͸� �����´�.
        // sign()�� ���� ��ȣ�� ã�´�.
        float flip = sign(dot(offset, n));
        
        // �� p�� ���չݰ濡�� �� q�� ã�´�.
        float3 q = p + flip * gOcclusionRadius * offset;
        
        // q�� �����ϰ� ������ �ؽ��� ��ǥ�� ����.
        // q ��ü�� ī�޶� ������ �����Ƿ� ������ �ؽ��� ��ǥ�� ����ȴ�.
        // ���� ������ �������� ����Ǿ�� �Ѵ�.
        float4 ProjQ = mul(float4(q, 1.0f), ViewToTexSpcace);
        ProjQ /= ProjQ.w;

        // �������� q������ ī�޶� �������� NormalDepthMap�� �ش��ϴ� �ּ� ���� ���� ã�´�.
        // �� �ּ� ���̴� ī�޶� ������ q ���̿� ���� �ʴ�.
        float rz = NormalDepthMap.Sample(SSAONormalDepth, ProjQ.xy, 0.0f).a;
        
        // �ٽ� ���� �ﰢ�� ������ ����Ѵ�.
        float3 r = (rz / q.z) * q;
        
        float distZ = p.z - r.z;
        float dp = max(dot(n, normalize(r - p)), 0.0f);
        float occlusion = dp * OcclusionFunction(distZ);
        
        occlusionSum += occlusion;
    }
    
    occlusionSum /= (float) sampleCount;
    
    float access = 1.0f - occlusionSum;
    
    // �ʹ� ��Ӵٴ� �ǵ���� �־ 0.5~1 ������ ������ �����
    Out.vColor = (saturate(pow(access, 4.0f)) + 1) * 0.4;
    
    return Out;
}

/* ------------------- Vertex Shader(1) -------------------*/

//struct VS_GOD_IN
//{
//    float3 vPosition : POSITION; /* ���°� */
//    float2 vTexcoord : TEXCOORD0;
//    float3 vFarPlaneIndex : NORMAL;
//};

//struct VS_GOD_OUT
//{
//    float4 vPosition : SV_POSITION;
//    float3 vTexcoord : TEXCOORD0;
//    float2 vTexcoord1 : TEXCOORD1;
//};

//VS_GOD_OUT VS_GODRAY(VS_GOD_IN In)
//{
//    VS_GOD_OUT Out = (VS_GOD_OUT) 0;
    
//    return Out;
//}

///* ------------------- Pixel Shader (1) -------------------*/
//struct PS_GOD_IN
//{
    
//};

//struct PS_GOD_Out
//{
    
//};

//PS_GOD_Out PS_GODRAY(PS_GOD_IN In)
//{
//    PS_GOD_Out Out = (PS_GOD_Out) 0;

//    return Out;
//}

/* ------------------- Technique -------------------*/ 

technique11 DefaultTechnique
{
    pass SSAO // 0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 1.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_SSAO();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SSAO();
    }
   // pass GodRay // 0
   // {
   //     /* �ɼ����� �������� */ 
   //     SetRasterizerState(RS_Default);
   //     SetDepthStencilState(DSS_None, 0);
   //     SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 1.f), 0xffffffff);
   //     VertexShader = compile vs_5_0 VS_GODRAY();
   //     GeometryShader = NULL;
   //     HullShader = NULL;
   //     DomainShader = NULL;
   //     PixelShader = compile ps_5_0 PS_GODRAY();
   // }

}