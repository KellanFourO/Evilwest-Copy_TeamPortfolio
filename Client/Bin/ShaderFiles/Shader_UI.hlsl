#include "Shader_Defines.hlsli"


/* float2, float3, float4 == vector */
/* vector.x == vector.r */
/* vector.xy = 1.f;*/
/* float4x4 == matrix */
/* matrix._11_12 = 1.f; */

/* ���̴��� �������� == ������̺�(Constant Table) */
matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D		g_DiffuseTexture;
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


struct VS_OUT_EFFECT
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};


VS_OUT_EFFECT VS_MAIN_EFFECT(VS_IN In)
{
	VS_OUT_EFFECT		Out = (VS_OUT_EFFECT)0;

	/* In.vPosition * ���� * �� * ���� */
	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexcoord = In.vTexcoord;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN_EFFECT
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};

/* �ȼ����̴� : �ȼ��� ��!!!! �� �����Ѵ�. */
PS_OUT PS_MAIN_EFFECT(PS_IN_EFFECT In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);

	float2	vDepthTexcoord;
	vDepthTexcoord.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	vDepthTexcoord.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	float4	vDepthDesc = g_DepthTexture.Sample(PointSampler, vDepthTexcoord);
	
	Out.vColor.a = Out.vColor.a * (vDepthDesc.y * 1000.f - In.vProjPos.w) * 2.f;

	return Out;
}


technique11 DefaultTechnique
{
	/* ���� ���ϴ� Ư�� ���̴����� �׸��� �𵨿� �����Ѵ�. */
	pass UI
	{
		/* ���̴�(������������) �׸������� �����Ұ͵� �������ְ� */
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 1.0f), 0xffffffff);
		
		/* ������������ */
		VertexShader = compile vs_5_0 VS_MAIN();	// �� �ް�
		GeometryShader = NULL;						// �ɼǵ� �����ϰ�
		HullShader = NULL;
		DomainShader = NULL;						
		PixelShader = compile ps_5_0 PS_MAIN();		// ���������� ����/���� �� �� ����Ѵ�. (ex : ����)
	}


}
