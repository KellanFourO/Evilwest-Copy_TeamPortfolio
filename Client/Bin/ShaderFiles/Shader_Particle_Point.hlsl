#include "Shader_Defines.hlsli"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_Texture;
vector			g_vCamPosition;

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vPSize : PSIZE;

	row_major float4x4	TransformMatrix : WORLD;
	float4		vColor : COLOR0;
};


struct VS_OUT
{
	float4		vPosition : POSITION;
	float2		vPSize : PSIZE;		
	float4		vColor : COLOR0;
};



VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;	

	vector		vPosition = mul(float4(In.vPosition, 1.f), In.TransformMatrix);
		
	Out.vPosition = mul(vPosition, g_WorldMatrix);
	Out.vPSize = float2(In.vPSize.x * In.TransformMatrix._11, In.vPSize.y * In.TransformMatrix._22);
	Out.vColor = In.vColor;

	return Out;
}

struct GS_IN
{
	float4		vPosition : POSITION;
	float2		vPSize : PSIZE;
	float4		vColor : COLOR0;
};

struct GS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
	float4		vColor : COLOR0;
};

/* ������Ʈ�� ���̴� : ���̴��ȿ��� ������ �߰������� ������ �ش�. */
[maxvertexcount(6)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> OutStream)
{
	GS_OUT		Out[4];

	float4		vLook = g_vCamPosition - In[0].vPosition;
	float3		vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook.xyz)) * In[0].vPSize.x * 0.5f;
	float3		vUp = normalize(cross(vLook.xyz, vRight)) * In[0].vPSize.y * 0.5f;

	matrix		matVP = mul(g_ViewMatrix, g_ProjMatrix);	

	Out[0].vPosition = mul(float4(In[0].vPosition.xyz + vRight + vUp, 1.f), matVP);
	Out[0].vTexcoord = float2(0.f, 0.f);
	Out[0].vColor = In[0].vColor;

	Out[1].vPosition = mul(float4(In[0].vPosition.xyz - vRight + vUp, 1.f), matVP);
	Out[1].vTexcoord = float2(1.f, 0.f);
	Out[1].vColor = In[0].vColor;

	Out[2].vPosition = mul(float4(In[0].vPosition.xyz - vRight - vUp, 1.f), matVP);
	Out[2].vTexcoord = float2(1.f, 1.f);
	Out[2].vColor = In[0].vColor;

	Out[3].vPosition = mul(float4(In[0].vPosition.xyz + vRight - vUp, 1.f), matVP);
	Out[3].vTexcoord = float2(0.f, 1.f);
	Out[3].vColor = In[0].vColor;

	OutStream.Append(Out[0]);
	OutStream.Append(Out[1]);
	OutStream.Append(Out[2]);
	OutStream.RestartStrip();

	OutStream.Append(Out[0]);
	OutStream.Append(Out[2]);
	OutStream.Append(Out[3]);
	OutStream.RestartStrip();
}

/* ����� ������ ��� .*/

/* ������ȯ : /w */ /* -> -1, 1 ~ 1, -1 */ 
/* ����Ʈ��ȯ-> 0, 0 ~ WINSX, WINSY */ 
/* �����Ͷ����� : ���������� ����Ͽ� �ȼ��� ������ �����. */


struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
	float4		vColor : COLOR0;
};

struct PS_OUT 
{
	float4		vColor : SV_TARGET0;
};

/* �ȼ����̴� : �ȼ��� ��!!!! �� �����Ѵ�. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	/* ù��° ������ ������� �ι�° ������ ��ġ�� �ִ� �ȼ��� ���� ���´�. */
	Out.vColor = g_Texture.Sample(PointSampler, In.vTexcoord);	

	if (Out.vColor.a < 0.8f)
		discard;

	Out.vColor.rgb = float3(1.f, 0.f, 0.f);

	Out.vColor.a = In.vColor.a;

	return Out;
}


technique11 DefaultTechnique
{
	/* ���� ���ϴ� Ư�� ���̴����� �׸��� �𵨿� �����Ѵ�. */
	pass Particle
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		/* ������������ */
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}
