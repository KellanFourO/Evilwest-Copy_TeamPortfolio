#include "Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D g_FinalTarget; /* ���� Deferred�� �Ѿ�»� */ 
Texture2D g_Final_Effect;
Texture2D g_Diffuse_UITarget;

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
    float4 vEffect = g_Final_Effect.Sample(LinearSampler, In.vTexcoord);
    
    if (vEffect.a == 0.0f)
    {
        float4 originalColor = g_FinalTarget.Sample(LinearSampler, In.vTexcoord);
            if (originalColor.a == 0.f)
               discard;
    
        // �� ����
        float3 BrightColor = originalColor.rgb * g_brightness;
    
        float value = BrightColor.r + (1.0 - g_brightness);
    
        // ä�� ����
        BrightColor = lerp(float3(value, value, value), BrightColor, g_saturation);
        Out.vColor = float4(BrightColor, originalColor.a);
    }

    return Out;
}

technique11 DefaultTechnique
{
    pass Final // 0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN_FINAL();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FINAL();
    }

}