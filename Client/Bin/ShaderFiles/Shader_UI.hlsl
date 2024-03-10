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
float2 g_Center;
float g_Radius;

Texture2D g_DepthTexture;
Texture2D g_DissolveTexture;
Texture2D g_AlphaTexture;


/* ������ ��ȯ(���庯ȯ, �亯ȯ, ������ȯ.)�� �����Ѵ�. */
/* ������ ���������� �߰�, �������� ������ �����Ѵ�.*/

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


	//if (vGaugeColor.a < 0.3f)
	//	discard;
	//Out.vColor = lerp(vLerpColor, vGaugeColor, vGaugeColor.a);
    //Out.vColor.a = g_Alpha;
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
// �ȼ� ���̴� ���� �Լ�
PS_OUT PS_MAIN_COOLTIME(PS_IN In) // 5
{
    PS_OUT Out;

    // ��Ÿ�� �ؽ�ó���� ���ø��Ͽ� ������ ��������, ���� �̹����� ��ȯ
    float  fCircleValue = Circle(In.vTexcoord, g_Center, g_Radius);
    float4 vTexColor = g_CoolDownTexture.Sample(LinearSampler, In.vTexcoord);

    // ���õ� ������ ���� �̹����� �°� �����Ͽ� ���
    Out.vColor = vTexColor * fCircleValue;

    //Out.vColor.a = g_Alpha;
    return Out;
}

technique11 DefaultTechnique
{
	/* ���� ���ϴ� Ư�� ���̴����� �׸��� �𵨿� �����Ѵ�. */
    pass Default // 0
    {
		/* ���̴�(������������) �׸������� �����Ұ͵� �������ְ� */
        SetRasterizerState(RS_Default);
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
        SetRasterizerState(RS_Default);
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
        SetRasterizerState(RS_Default);
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
        SetRasterizerState(RS_Default);
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
        SetRasterizerState(RS_Default);
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
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 1.0f), 0xffffffff);
   
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_COOLTIME();
    }
}
