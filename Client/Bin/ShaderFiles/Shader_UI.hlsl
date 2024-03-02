#include "Shader_Defines.hlsli"


/* float2, float3, float4 == vector */
/* vector.x == vector.r */
/* vector.xy = 1.f;*/
/* float4x4 == matrix */
/* matrix._11_12 = 1.f; */

/* ���̴��� �������� == ������̺�(Constant Table) */
matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D		g_DiffuseTexture;
texture2D		g_DiffuseTexture_Second;
texture2D		g_DiffuseTexture_Third;
texture2D		g_DiffuseTexture_Fourth;

texture2D		g_HpBarWhite_Texture;
texture2D		g_HpBarRed_Texture;
texture2D		g_HpBarDecal_Texture;

float			g_MaxHP;
float			g_CurrentHP;
float			g_LerpHP;

texture2D		g_DepthTexture;


/* ������ ��ȯ(���庯ȯ, �亯ȯ, ������ȯ.)�� �����Ѵ�. */
/* ������ ���������� �߰�, �������� ������ �����Ѵ�.*/

/* ���� �װ�. */
/* �ε��� ������ .*/

/* ��ü���� Render�� ����, VIBuffer���� IASetPrimitiveTopology�Լ��� ���� �ؽ�ó�� �������� �޾ƿ´�. */
struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexcoord : TEXCOORD0;
};


struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;	
};


/* �޾ƿ� �ؽ�ó�� ������ ��� ������ ���������� */
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	/* In.vPosition * ���� * �� * ���� */
	matrix		matWV, matWVP;

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
	float4		vPosition : SV_POSITION; // z���� ó���� ���°� �ȴ�.
	float2		vTexcoord : TEXCOORD0;
};

struct PS_OUT 
{
	float4		vColor : SV_TARGET0;
};

/* �ȼ����̴� : �ȼ��� ��!!!! �� �����Ѵ�. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	/* �� ���̴��� ����ϴ� ��ü�� ������ g_DiffuseTexture�� �������� �����Ű�ڴ�. */
    Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
		
    if (Out.vColor.a < 0.1f)
        discard;
	
     return Out;
}

PS_OUT PS_HPBAR_GAUGE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float4 vBackColor = g_HpBarRed_Texture.Sample(LinearSampler, In.vTexcoord);

    if (g_CurrentHP / g_MaxHP < In.vTexcoord.x) // ���� ü�°� �ִ� ü���� �� ���� UV��ǥ�� �߸���.
    {
        Out.vColor = vBackColor;
        return Out;
    }

    float4 vGaugeColor = g_HpBarWhite_Texture.Sample(LinearSampler, In.vTexcoord);
    if (vGaugeColor.a < 0.3f)
        discard;

    Out.vColor = lerp(vBackColor, vGaugeColor, vGaugeColor.a);

    return Out;
}

PS_OUT PS_HPBAR_GAUGE_LERP(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float4 vLerpColor = g_HpBarWhite_Texture.Sample(LinearSampler, In.vTexcoord);   // Hp Pre
    float4 vGaugeColor = g_HpBarRed_Texture.Sample(LinearSampler, In.vTexcoord);    // Hp Cur
    float4 vDecalColor = g_HpBarDecal_Texture.Sample(LinearSampler, In.vTexcoord);  // Hp Decal

    
    
    Out.vColor = vDecalColor;
    
    if (vDecalColor.a == 0.f)
    {
        if (g_LerpHP / g_MaxHP < In.vTexcoord.x) // ���� ü�°� �ִ� ü���� �� ���� UV��ǥ�� �߸���.
            discard;

        if (g_LerpHP / g_MaxHP >= In.vTexcoord.x && g_CurrentHP / g_MaxHP <= In.vTexcoord.x)
            Out.vColor = vLerpColor;

        if (g_CurrentHP / g_MaxHP > In.vTexcoord.x) 
            Out.vColor = vGaugeColor;
    }
    


	//if (vGaugeColor.a < 0.3f)
	//	discard;
	//Out.vColor = lerp(vLerpColor, vGaugeColor, vGaugeColor.a);

    return Out;
}

PS_OUT PS_INCREASING_PROGRESS(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float4 vGaugeColor = g_HpBarRed_Texture.Sample(LinearSampler, In.vTexcoord); // ������ �� Texture
    float4 vBackColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord); // Frame Texture

    if (vGaugeColor.a < 0.3f)
        discard;
    if (vBackColor.a < 0.3f)
        discard;

    if (g_LerpHP / g_MaxHP > In.vTexcoord.x) // ���� ü�°� �ִ� ü���� �� ���� UV��ǥ�� �߸���.
    {
        Out.vColor = vBackColor;
        return Out;
    }

    if (g_CurrentHP / g_MaxHP <= In.vTexcoord.x)
    {
        Out.vColor = vGaugeColor;
    }

    Out.vColor = lerp(vBackColor, vGaugeColor, vGaugeColor.a);

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
		VertexShader = compile vs_5_0 VS_MAIN();	// �� �ް�
		GeometryShader = NULL;						// �ɼǵ� �����ϰ�
		HullShader = NULL;
		DomainShader = NULL;						
		PixelShader = compile ps_5_0 PS_MAIN();		// ���������� ����/���� �� �� ����Ѵ�. (ex : ����)
	}
    
    pass HPBarGauge // 1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(1.f, 1.f, 1.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_HPBAR_GAUGE();
    }

    pass HPBarGauge_Lerp // 2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(1.f, 1.f, 1.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_HPBAR_GAUGE_LERP();
    }

    pass IncreasingProgress // 3
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INCREASING_PROGRESS();
    }

}
