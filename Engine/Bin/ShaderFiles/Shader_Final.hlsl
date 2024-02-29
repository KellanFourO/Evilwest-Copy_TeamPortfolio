#include "Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D g_Final_Deferred_Target;
Texture2D g_Final_Effect_Target;
Texture2D g_PriorityTarget;
Texture2D g_Final_UI_Target;

/* �� ä�� ���� */
float g_brightness = 1.f; // �� 
float g_saturation = 1.f; // ä�� 

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

VS_OUT VS_MAIN_FINAL(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;

    return Out;
}


struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

float3 AdjustHue(float3 hsv, float hueShift)
{
    // hueShift�� ������ ǥ���Ǹ�, �̸� [0, 360] ������ ����
    hueShift = frac(hueShift / 360.0);

    // �̹����� ������ ����
    hsv.x += hueShift;
    hsv.x = frac(hsv.x); // 0~1 ������ ����

    return hsv;
}

PS_OUT PS_MAIN_FINAL(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
   // float4 vUI = g_Diffuse_UITexture.Sample(LinearSampler, In.vTexcoord);
    float4 vEffect = g_Final_Effect_Target.Sample(LinearSampler, In.vTexcoord);
    float4 originalColor = g_Final_Deferred_Target.Sample(LinearSampler, In.vTexcoord);
    float4 vPriority = g_PriorityTarget.Sample(LinearSampler, In.vTexcoord);
    
    float4 vResult = vEffect + originalColor;
    
    if (vResult.a == 0)
    {
        Out.vColor = g_PriorityTarget.Sample(LinearSampler, In.vTexcoord);
    }
    else
    {
        Out.vColor = vResult;
    }
    
    if (Out.vColor.a == 0) // ����� blue ������ 
        discard;

    return Out;    
}

  
    
technique11 DefaultTechnique
{
    pass Final // 0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN_FINAL();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FINAL();
    }

}