#include "Shader_Defines.hlsli"


/* float2, float3, float4 == vector */
/* vector.x == vector.r */
/* vector.xy = 1.f;*/
/* float4x4 == matrix */
/* matrix._11_12 = 1.f; */

/* ���̴��� �������� == ������̺�(Constant Table) */
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D g_DiffuseTexture;
Texture2D g_DiffuseTexture_Second;
Texture2D g_DiffuseTexture_Third;
Texture2D g_DiffuseTexture_Fourth;

Texture2D g_MaskTexture;
Texture2D g_NoiseTexture;
Texture2D g_DepthTexture;

/* Alpha */
float g_Alpha;

/* Loading */
float g_LoadingProgress;

/* Hp */
Texture2D g_HpBarWhite_Texture;
Texture2D g_HpBarRed_Texture;
Texture2D g_HpBarDecal_Texture;
float g_MaxHP;
float g_CurrentHP;
float g_LerpHP;

/* Aim */
float2 g_Recoil;
float2 g_Offset;
Texture2D g_AimTop_Texture;
Texture2D g_AimBottom_Texture;
Texture2D g_AimLeft_Texture;
Texture2D g_AimRight_Texture;

/* CoolDown */
Texture2D g_CoolDownTexture;
float2    g_Center;
float     g_Radius;
float     g_CoolTime;
float     g_MaxCoolTime;
float     g_Ratio;

texture2D g_DissolveTexture;
texture2D g_AlphaTexture;


// ======= Distortion
float g_fFrameTime;
float3 g_vScrollSpeeds;
float3 g_vScales;
float4 g_vColor_Mul;
int g_iColorMode;

float2 g_vDistortion1;
float2 g_vDistortion2;
float2 g_vDistortion3;
float g_fDistortionScale;
float g_fDistortionBias;
// =======

float4 Calculation_ColorBlend(float4 vDiffuse, float4 vBlendColor, int g_iColorMode)
{
    float4 vResault = vDiffuse;
   
    if (0 == g_iColorMode)
    {
      // ���ϱ�
        vResault = vResault * vBlendColor;
    }
    else if (1 == g_iColorMode)
    {
      // ��ũ��
        vResault = 1.f - ((1.f - vResault) * (1.f - vBlendColor));
    }
    else if (2 == g_iColorMode)
    {
      // ��������
        vResault = max(vResault, vBlendColor);
    }
    else if (3 == g_iColorMode)
    {
      // ���ϱ�
        vResault = vResault + vBlendColor;
    }
    else if (4 == g_iColorMode)
    {
      // ��(Burn)
        vResault = vResault + vBlendColor - 1.f;
    }
 
    return vResault;
}

/* ������ ��ȯ(���庯ȯ, �亯ȯ, ������ȯ.)�� �����Ѵ�. */
/* ������ ���������� �߰�, �������� ������ �����Ѵ�. */

/* ���� �װ�. */
/* �ε��� ������ .*/

/* ��ü���� Render�� ����, VIBuffer���� IASetPrimitiveTopology�Լ��� ���� �ؽ�ó�� �������� �޾ƿ´�. */
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

/* Distortion */
struct VS_OUT_DISTORTION
{
    float4 vPosition : SV_POSITION;

    float2 vTexcoord : TEXCOORD0;
    float2 vTexcoord1 : TEXCOORD1;
    float2 vTexcoord2 : TEXCOORD2;
    float2 vTexcoord3 : TEXCOORD3;

    float4 vProjPos : TEXCOORD4;
};

struct PS_IN_DISTORTION
{
    float4 vPosition : SV_POSITION;

    float2 vTexcoord : TEXCOORD0;
    float2 vTexcoord1 : TEXCOORD1;
    float2 vTexcoord2 : TEXCOORD2;
    float2 vTexcoord3 : TEXCOORD3;

    float4 vProjPos : TEXCOORD4;
};
/* Distortion End */

/* �޾ƿ� �ؽ�ó�� ������ ��� ������ ���������� */
VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

	/* In.vPosition * ���� * �� * ���� */
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;

	/* �������� �÷Ȱ�, z������ �����̴�. z������� PS_IN�� vPosition���� ���� ��, SV_POSITION�ɼ����� �ްԵǸ� �˾Ƽ� ó�����ȴ�. */
	
    return Out;
}

/* ����� ������ ��� .*/

/* ������ȯ : /w */ /* -> -1, 1 ~ 1, -1 */ 
/* ����Ʈ��ȯ-> 0, 0 ~ WINSX, WINSY */ 
/* �����Ͷ����� : ���������� ����Ͽ� �ȼ��� ������ �����. */


struct PS_IN
{
    float4 vPosition : SV_POSITION; // z���� ó���� ���°� �ȴ�.
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

/* �ȼ����̴� : �ȼ��� ��!!!! �� �����Ѵ�. */
PS_OUT PS_MAIN(PS_IN In) // 0
{
    PS_OUT Out = (PS_OUT) 0;

	/* �� ���̴��� ����ϴ� ��ü�� ������ g_DiffuseTexture�� �������� �����Ű�ڴ�. */
    Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
   
    Out.vColor.a -= g_Alpha;
    
    if (Out.vColor.a < 0.1f)
        discard;
	
    return Out;
}

PS_OUT PS_HPBAR_GAUGE_LERP(PS_IN In) // 1
{
    PS_OUT Out = (PS_OUT) 0;

    float4 vLerpColor = g_HpBarWhite_Texture.Sample(LinearSampler, In.vTexcoord); // Hp Pre
    float4 vGaugeColor = g_HpBarRed_Texture.Sample(LinearSampler, In.vTexcoord); // Hp Cur

    if (g_LerpHP / g_MaxHP < In.vTexcoord.x) // ���� ü�°� �ִ� ü���� �� ���� UV��ǥ�� �߸���.
        discard;

    if (g_LerpHP / g_MaxHP >= In.vTexcoord.x && g_CurrentHP / g_MaxHP <= In.vTexcoord.x)
        Out.vColor = vLerpColor;

    if (g_CurrentHP / g_MaxHP > In.vTexcoord.x) 
        Out.vColor = vGaugeColor;


    //if (vGaugeColor.a < 0.1f)
    //    discard;
    //
    //Out.vColor = lerp(vLerpColor, vGaugeColor, vGaugeColor.a);
    
    return Out;
}

/* Loading */
PS_OUT PS_MAIN_LOADING(PS_IN In) // 2
{
    PS_OUT Out = (PS_OUT) 0;

    float fLoadingPer = 1.f;

    if (In.vTexcoord.x < g_LoadingProgress || In.vTexcoord.x > (g_LoadingProgress + fLoadingPer))
    {
        Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    }
    else
    {
        discard;
		//Out.vColor = float4(0.0, 0.0, 0.0, 0.0);
    }

    //Out.vColor.a = g_Alpha;
    
    return Out;
}

/* Loading */
PS_OUT PS_MAIN_OPTION_BACKGROUND(PS_IN In) // 3
{
    PS_OUT Out = (PS_OUT) 0;
    //
    //float fLoadingPer = 1.f;
    //
    //g_DiffuseTexture;   // Background Texture
    //g_DissolveTexture;  // Fog Texture
    //g_AlphaTexture;     // AlphaTexture
    //
    Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);

    //Out.vColor.a = g_Alpha;
    return Out;
}

/* AIM_CROSSHAIR */ // ũ������/��ġ���� ������
PS_OUT PS_MAIN_AIM_CROSSHAIR(PS_IN In) // 4
{
    PS_OUT Out = (PS_OUT) 0;
    
    // �̹��� ������ �����ϴ� ������ g_Recoil�� ���
    float2 vOffset = g_Offset * g_Recoil;

    // �� �̹����� ���ø� ��ġ�� ��¦ �̵��Ͽ� ��ġ�� �ʵ��� ����
    float2 vTopTexCoord = In.vTexcoord + float2(0, vOffset.y);
    float2 vBottomTexCoord = In.vTexcoord + float2(0, -vOffset.y);
    float2 vLeftTexCoord = In.vTexcoord + float2(-vOffset.x, 0);
    float2 vRightTexCoord = In.vTexcoord + float2(vOffset.x, 0);

    // �� �ؽ�ó ���ø�
    float4 vTopColor = g_AimTop_Texture.Sample(LinearSampler, vTopTexCoord);
    float4 vBottomColor = g_AimBottom_Texture.Sample(LinearSampler, vBottomTexCoord);
    float4 vLeftColor = g_AimLeft_Texture.Sample(LinearSampler, vLeftTexCoord);
    float4 vRightColor = g_AimRight_Texture.Sample(LinearSampler, vRightTexCoord);

    // �����¿� �̹����� ���� ���·� ����
    float4 vCombinedColor = vTopColor + vBottomColor + vLeftColor + vRightColor;

    // ���յ� ����� ���
    Out.vColor = vCombinedColor;

    //Out.vColor.a = g_Alpha;
    return Out;
}


// ���� �׸��� �Լ�
float Circle(float2 uv, float2 center, float radius)
{
    float2 diff = uv - center;
    return saturate(1 - length(diff) / radius);
}

PS_OUT PS_MAIN_COOLTIME(PS_IN In) // 5
{
    PS_OUT Out = (PS_OUT) 0; // �ʱ�ȭ

    float4 vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord); // Diffuse Tex Sampling
    float4 vMaskColor = g_MaskTexture.Sample(LinearSampler, In.vTexcoord); // Mask Tex Sampling

    if (vMaskColor.r > 0.9f && vMaskColor.g > 0.9f && vMaskColor.b > 0.9f)
    {
        Out.vColor = saturate(vColor);
        if (Out.vColor.a < 0.1f)
            discard;
    }
    else
        discard;

	// ������� ����ũ�� ���� ����

    float2 vDir = In.vTexcoord - float2(0.5f, 0.5f); // float2(0.5f, 0.5f)�� �����̴�.
    vDir = normalize(vDir); // ���⺤�� Normalize
    float2 vUpDir = float2(0.0f, sign(vDir.x));
    vUpDir = normalize(vUpDir);

    float fDot = dot(vUpDir, vDir); // �� ���͸� �����Ѵ�.
    float fDotRatio = g_Ratio;

	// ���⺤�Ͱ� ������ ���, ���� ���� ������ ������ ��
    if (vDir.x < 0.f)
    {
        fDotRatio -= 0.5f;
    }

    fDotRatio = fDotRatio * 4.f - 1.f;

    if (fDotRatio < fDot) // �ܿ� ��Ÿ���� ���������� �������� �ȼ��̴�
    {
		//Out.vColor.rgb = lerp(vColor.rgb, float3(0.0f, 0.0f, 0.0f), 0.5f);
        Out.vColor.a = 0.f;
    }

	// Ư�� ���������� ������ ���� �̹����� ǥ�õȴ�.
    return Out;
}

//// �ȼ� ���̴� ���� �Լ�
//PS_OUT PS_MAIN_COOLTIME(PS_IN In) // 5
//{
//    PS_OUT Out = (PS_OUT) 0;
//    
//    float2 center = float2(0.5f, 0.5f); // �߽� ��ǥ
//    float radius = 0.4f; // ������
//    float thickness = 0.1f; // ������ �β�
//
//    // ���� ��Ÿ�� ���� �̿��Ͽ� �������� ���
//    float progress = saturate(1.0f - g_CoolTime / g_MaxCoolTime); // ���� ��Ÿ���� [0, 1] ������ ��ȯ
//    float angle = progress * 1 * 3.14159265359f; // 0���� 2���̱����� ���� ������ ��ȯ
//    //        (* ��� *) �̺κ��� ���� ���������� �� �̹����� �߸��� ����ũ��.
//    
//    // �������� ���۰� �� ���� ���
//    float startAngle = 0.0f;
//    float endAngle = angle;
//
//    // ������ ����
//    float4 gaugeColor = float4(0.0f, 1.0f, 0.0f, 1.0f); // ���
//
//    // �������� �׸��� ���� ���ǽ�
//    float2 uv = In.vTexcoord;
//    float2 diff = uv - center;
//    float distance = length(diff);
//    float angleUV = atan2(diff.y, diff.x);
//    float angleDeg = angleUV * (360.0f / 3.14159265359f);
//
//    // �������� �׸��� ���� ���ǽ�
//    float gaugeValue = saturate((angleDeg - startAngle) / (endAngle - startAngle));
//    float gauge = smoothstep(radius - thickness, radius, distance);
//
//    // �������� �׸��� �κ�
//    float4 finalColor = lerp(float4(0.0f, 0.0f, 0.0f, 0.0f), gaugeColor, gauge * gaugeValue);
//
//    Out.vColor = finalColor;
//    
//    return Out;
//    
//    //PS_OUT Out;
//    //
//    //// ��Ÿ�� �ؽ�ó���� ���ø��Ͽ� ������ ��������, ���� �̹����� ��ȯ
//    //float  fCircleValue = Circle(In.vTexcoord, g_Center, g_Radius);
//    //float4 vTexColor = g_CoolDownTexture.Sample(LinearSampler, In.vTexcoord);
//    //
//    //// ���õ� ������ ���� �̹����� �°� �����Ͽ� ���
//    //Out.vColor = vTexColor * fCircleValue;
//    //
//    ////Out.vColor.a = g_Alpha;
//    //return Out;
//}

VS_OUT_DISTORTION VS_MAIN_DISTORTION(VS_IN In)
{
    VS_OUT_DISTORTION Out = (VS_OUT_DISTORTION) 0;

	/* In.vPosition * ���� * �� * ���� */
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vProjPos = Out.vPosition;

    return Out;
}

PS_OUT PS_MAIN_DISTORTION(PS_IN_DISTORTION In) // 6
{
    PS_OUT Out = (PS_OUT) 0;
    /*
    1. Mask�� ����� ��´�.
    2. Noise�� �Ϸ��Ÿ��� ȿ���� �ش�.
    3. Diffuse�� ������ �����ش�.
    
    => Mask + Noise + Diffuse�� ��� ��� + ȿ�� + ������ ������ ������� �÷��� ��������.
    */
    float4 vNoise1;
    float4 vNoise2;
    float4 vNoise3;

    float4 vFinalNoise;
    float  fPerturb;
    float2 vNoiseCoords;
    float4 vFireColor;
    float4 vAlphaColor;


	// ������ �ؽ����� ��ǥ�� ù��° ũ�� �� ������ ��ũ�� �ӵ� ���� �̿��Ͽ� ��� x 3
    In.vTexcoord1 = (In.vTexcoord * g_vScales.x);
    In.vTexcoord1.y = In.vTexcoord1.y + (g_fFrameTime * g_vScrollSpeeds.x);

    In.vTexcoord2 = (In.vTexcoord * g_vScales.y);
    In.vTexcoord2.y = In.vTexcoord2.y + (g_fFrameTime * g_vScrollSpeeds.y);

    In.vTexcoord3 = (In.vTexcoord * g_vScales.z);
    In.vTexcoord3.y = In.vTexcoord3.y + (g_fFrameTime * g_vScrollSpeeds.z);


	// ������ ������ �ؽ��ĸ� ���� �ٸ� �� �ؽ��� ��ǥ�� ����Ͽ� �� ���� �ٸ� ũ���� ����� ��´�.
    vNoise1 = g_NoiseTexture.Sample(LinearSampler, In.vTexcoord1);
    vNoise2 = g_NoiseTexture.Sample(LinearSampler, In.vTexcoord2);
    vNoise3 = g_NoiseTexture.Sample(LinearSampler, In.vTexcoord3);

	// ������ ���� ������ (0, 1)���� (-1, +1)�� �ǵ����Ѵ�.
    vNoise1 = (vNoise1 - 0.5f) * 2.0f;
    vNoise2 = (vNoise2 - 0.5f) * 2.0f;
    vNoise3 = (vNoise3 - 0.5f) * 2.0f;

	// �������� x�� y���� �� ���� �ٸ� �ְ� x�� y��ǥ�� ��߸���.
    vNoise1.xy = vNoise1.xy * g_vDistortion1.xy;
    vNoise2.xy = vNoise2.xy * g_vDistortion2.xy;
    vNoise3.xy = vNoise3.xy * g_vDistortion3.xy;

	// �ְ�� �� ������ ������ �ϳ��� ������� �Լ��Ѵ�.
    vFinalNoise = vNoise1 + vNoise2 + vNoise3;

	// �Է����� ���� �ؽ����� Y��ǥ�� �ְ� ũ��� ���̾ ������ ������Ų��.
	// �� ������ �ؽ����� �������� ������ �������� �� ���ʿ��� �����̴� ȿ���� ������.
    fPerturb = ((1.0f - In.vTexcoord.y) * g_fDistortionScale) + g_fDistortionBias;

	// �Ҳ� ���� �ؽ��ĸ� ���ø��ϴµ� ���� �ְ� �� ������ �ؽ��� ��ǥ�� �����.
    vNoiseCoords.xy = (vFinalNoise.xy * fPerturb) + In.vTexcoord.xy;


	// �ְ�ǰ� ������ �ؽ��� ��ǥ�� �̿��Ͽ� �Ҳ� �ؽ��Ŀ��� ������ ���ø��Ѵ�.
	// wrap�� ����ϴ� ������Ʈ ��� clamp�� ����ϴ� ������Ʈ�� ����Ͽ� �Ҳ� �ؽ��İ� ���εǴ� ���� �����Ѵ�.
    vFireColor = g_DiffuseTexture.Sample(ClampSampler, vNoiseCoords.xy);


	// �ְ�ǰ� ������ �ؽ��� ��ǥ�� �̿��Ͽ� ���� �ؽ��Ŀ��� ���İ��� ���ø��Ѵ�. (�Ҳ��� ������ �����ϴ� �� ���)
	// wrap�� ����ϴ� ������Ʈ ��� clamp�� ����ϴ� ������Ʈ�� ����Ͽ� �Ҳ� �ؽ��İ� ���εǴ� ���� �����Ѵ�.
    vAlphaColor = g_MaskTexture.Sample(ClampSampler, vNoiseCoords.xy);

    vFireColor.a = vAlphaColor;

       // �÷� ȥ��
    Out.vColor = Calculation_ColorBlend(vFireColor, g_vColor_Mul, g_iColorMode);
    
    return Out;
}

technique11 DefaultTechnique
{
	/* ���� ���ϴ� Ư�� ���̴����� �׸��� �𵨿� �����Ѵ�. */
    pass Default // 0
    {
		/* ���̴�(������������) �׸������� �����Ұ͵� �������ְ� */
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 1.0f), 0xffffffff);
		
		/* ������������ */
        VertexShader = compile vs_5_0 VS_MAIN(); // �� �ް�
        GeometryShader = NULL; // �ɼǵ� �����ϰ�
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN(); // ���������� ����/���� �� �� ����Ѵ�. (ex : ����)
    }
    
    pass HPBarGauge_Lerp // 1
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 1.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_HPBAR_GAUGE_LERP();
    }

    pass LoadingBar_Gauge // 2
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 1.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LOADING();
    }

    pass Option_Background // 3
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 1.0f), 0xffffffff);
   
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_OPTION_BACKGROUND();
    }

    pass Aim_Crosshair // 4
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 1.0f), 0xffffffff);
   
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_AIM_CROSSHAIR();
    }

    pass CoolDown // 5
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(1.f, 1.f, 1.f, 1.f), 0xffffffff);
   
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_COOLTIME();
    }

    pass Distortion // 6
    {
        SetBlendState(BS_AlphaBlend_Add, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthStencilEnable, 0);
        SetRasterizerState(RS_Cull_None);

        VertexShader = compile vs_5_0 VS_MAIN_DISTORTION();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DISTORTION();
    }
}
