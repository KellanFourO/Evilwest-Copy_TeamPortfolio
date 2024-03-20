#include "Shader_Defines.hlsli"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D	g_DepthTexture;

Texture2D	g_DiffuseTexture;
Texture2D	g_MaskTexture;
Texture2D	g_NoiseTexture;

Texture2D	g_NormalTexture;

// Camera ====================
vector		g_vCamPosition;
float3		g_vCamDirection;
float		g_fCamFar;
// ===========================


// Sprite ====================
bool		g_bSprite;
float2		g_UVOffset;
float2		g_UVScale;
// ===========================


bool        g_bBillBoard;
float		g_fDegree;

float       g_fAlpha_Discard;
float3      g_vBlack_Discard;


/* Color */
int			g_iColorMode;
float4		g_vColor_Mul;




// �ҿ� Test ( + ���� �ּ� )  ================= 
// �ϴ� �� ����������. �����ص���. 
// �������Ѱ� �־��µ�, ������Ʈ �����Ϸ��� �븻���Ͱ� �ʿ���. 
// �׷��� BloomPower�� �����ؼ� ����� �󸶳� ���������� ������ ���� �� ����. 
// ����� BloomPower �� 0.3 0.3 0.3 �� �־����� Calculation_Brightness�Լ������� fPixelBrightness�� �Ӱ踦 �������ؼ� �ȳ����°�쵵 �־���.. 
// �����ϸ� �� fPixelBrightness �� �������� ������ �����ص���. �Լ��� ����� �۵��ϸ� �ȿ� ����̴� ������ �Լ��� �����ؼ� �ᵵ �� 
//  �� ? �븻�� �ȴٰ� ?? ??? ?? ???????????? ??
float4	g_vRimColor; /* = { 1.0f, 0.f, 0.f, 0.5f }; /* RimLight */
float	g_fRimPower;
float3	g_vBloomPower; /*= { 0.7f, 0.7f, 0.7f }; /* Bloom */


// Distortion ====================
float	g_fFrameTime;

int		g_iScrollType;

float3	g_vScrollSpeeds;
float3	g_vScales;

float2	g_vDistortion1;
float2	g_vDistortion2;
float2	g_vDistortion3;
float	g_fDistortionScale;
float	g_fDistortionBias;
// ================================


// ================================
struct EffectDesc
{
	float3	g_vDir;
	float	g_Padding;
};
EffectDesc g_EffectDesc[500];


// Custom Function ==============================================================================================================
float2 Rotate_Texcoord(float2 vTexcoord, float fDegree)
{
	float fDegree2Radian = 3.14159265358979323846 * 2 / 360.f;
	float fRotationRadian = fDegree * fDegree2Radian;
	float cosA = cos(fRotationRadian);
	float sinA = sin(fRotationRadian);

	float2x2 RotateMatrix = float2x2(cosA, -sinA, sinA, cosA);

	vTexcoord -= 0.5f;
	vTexcoord = mul(vTexcoord, RotateMatrix);
	vTexcoord += 0.5f;

	return vTexcoord;
}

float4 Calculation_RimColor(float4 In_Normal, float4 In_Pos)
{
    float fRimPower = 1.f - saturate(dot(In_Normal, normalize((-1.f * (In_Pos - g_vCamPosition)))));
	fRimPower = pow(fRimPower, 5.f) * g_fRimPower;
    float4 vRimColor = g_vRimColor * fRimPower;
    
    return vRimColor;
}

float4 Calculation_Brightness(float4 Out_Diffuse)
{
    float4 vBrightnessColor = float4(0.f, 0.f, 0.f, 0.f);

    float fPixelBrightness = dot(Out_Diffuse.rgb, g_vBloomPower.rgb);
    
    if (fPixelBrightness > 0.99f)
        vBrightnessColor = float4(Out_Diffuse.rgb, 1.0f);

    return vBrightnessColor;
}


float4 Calculation_ColorBlend(float4 vDiffuse, float4 vBlendColor)
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


// �� ���� ������ ���� ��� (����)
float Calculate_AngleBetweenVectors_Radian(float3 v1, float3 v2)
{
	float fDot = dot(normalize(v1), normalize(v2));
	float fAngle = acos(fDot);

	return fAngle;
}

// �� ���� ������ ���� ��� (�� Degree)
float Calculate_AngleBetweenVectors_Degree(float3 v1, float3 v2)
{
	float fRadians = Calculate_AngleBetweenVectors_Radian(v1, v2);
	float fDegree = fRadians * (180.0f / 3.14159265358979323846);

	return fDegree;
}
// Custom Function ==============================================================================================================



// MAIN_PARTICLE ================================================================================================================
struct VS_IN
{
	float3				vPosition		: POSITION;
	float2				vPSize			: PSIZE;

	row_major float4x4	TransformMatrix : WORLD;
	float4				vColor			: COLOR0;

	uint				iInstanceID		: SV_INSTANCEID;
};

struct VS_OUT
{
	float4		vPosition	: POSITION;
	float2		vPSize		: PSIZE;
	float4		vColor		: COLOR0;

	uint	    iInstanceID : SV_INSTANCEID;
};

VS_OUT VS_MAIN_PARTICLE(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	vector		vPosition = mul(float4(In.vPosition, 1.f), In.TransformMatrix);

	Out.vPosition = mul(vPosition, g_WorldMatrix);
	Out.vPSize = float2(In.vPSize.x * In.TransformMatrix._11, In.vPSize.y * In.TransformMatrix._22);
	Out.vColor = In.vColor;

	Out.iInstanceID = In.iInstanceID;

	return Out;
}

struct GS_IN
{
	float4		vPosition	: POSITION;
	float2		vPSize		: PSIZE;
	float4		vColor		: COLOR0;

	uint	iInstanceID		: SV_INSTANCEID;
};

struct GS_OUT
{
	float4		vPosition	: SV_POSITION;
	float2		vTexcoord	: TEXCOORD0;
	float4		vColor		: COLOR0;
};

/* ������Ʈ�� ���̴� : ���̴��ȿ��� ������ �߰������� ������ �ش�. */
[maxvertexcount(6)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> OutStream)
{
	GS_OUT		Out[4];

	float4		vLook;
	float3		vRight, vUp;
	
	if (g_bBillBoard)
	{
		vLook = g_vCamPosition - In[0].vPosition;
		vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook.xyz)) * In[0].vPSize.x * 0.5f;
		vUp = normalize(cross(vLook.xyz, vRight)) * In[0].vPSize.y * 0.5f;
	}
	else
	{
		// �̵� ���� ���⺤�͸� Up���� �� ���ο� Right, Look �����ֱ� ===================================
		vUp = normalize(g_EffectDesc[In[0].iInstanceID].g_vDir).rgb * In[0].vPSize.y * 0.5f;
		vLook.rgb = normalize(cross(float3(0.f, 1.f, 0.f), vUp));
		vRight = normalize(cross(vUp, vLook.rgb)) * In[0].vPSize.x * 0.5f;
		vLook.rgb = normalize(cross(vRight, vUp)); vLook.a = 0.f;
	}

	matrix		matVP = mul(g_ViewMatrix, g_ProjMatrix);

	// �߾� ������ �ƴ�! (�߾� ���� ���� �ٲ㼭 �����!!)
	//Out[0].vPosition = mul(float4(In[0].vPosition.xyz + vRight + vUp, 1.f), matVP);
	Out[0].vPosition = mul(float4(In[0].vPosition.xyz + vRight, 1.f), matVP);
	Out[0].vTexcoord = Rotate_Texcoord(float2(0.f, 0.f), g_fDegree);
	Out[0].vColor = In[0].vColor;

	//Out[1].vPosition = mul(float4(In[0].vPosition.xyz - vRight + vUp, 1.f), matVP);
	Out[1].vPosition = mul(float4(In[0].vPosition.xyz - vRight, 1.f), matVP);
	Out[1].vTexcoord = Rotate_Texcoord(float2(1.f, 0.f), g_fDegree);
	Out[1].vColor = In[0].vColor;

	//Out[2].vPosition = mul(float4(In[0].vPosition.xyz - vRight - vUp, 1.f), matVP);
	Out[2].vPosition = mul(float4(In[0].vPosition.xyz - vRight - (vUp * 2), 1.f), matVP);
	Out[2].vTexcoord = Rotate_Texcoord(float2(1.f, 1.f), g_fDegree);
	Out[2].vColor = In[0].vColor;

	//Out[3].vPosition = mul(float4(In[0].vPosition.xyz + vRight - vUp, 1.f), matVP);
	Out[3].vPosition = mul(float4(In[0].vPosition.xyz + vRight - (vUp * 2), 1.f), matVP);
	Out[3].vTexcoord = Rotate_Texcoord(float2(0.f, 1.f), g_fDegree);
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
	float4		vPosition	: SV_POSITION;
	float2		vTexcoord	: TEXCOORD0;
	float4		vColor		: COLOR0;
};

struct PS_OUT
{
    float4 vColor		: SV_TARGET0;	// Diffuse
    float4 vSolid		: SV_TARGET1;			 
    float4 vNormal		: SV_TARGET2;	// Normal
    float4 vDepth		: SV_TARGET3;	// Depth
    float4 vRimBloom	: SV_TARGET4;	// RimBloom
};


/* �ȼ����̴� : �ȼ��� ��!!!! �� �����Ѵ�. */
PS_OUT PS_MAIN_PARTICLE(PS_IN In, uniform bool bSolid)
{
	PS_OUT		Out = (PS_OUT)0;

	if (g_bSprite)
	{
		float2 clippedTexCoord = In.vTexcoord * g_UVScale + g_UVOffset;
		float4 vDiffuseColor = g_DiffuseTexture.Sample(PointSampler, clippedTexCoord);

		vDiffuseColor.rgb *= In.vColor.rgb;

		if (vDiffuseColor.a < g_fAlpha_Discard	// ���� �߶󳻱�
		|| vDiffuseColor.r < g_vBlack_Discard.r && vDiffuseColor.g < g_vBlack_Discard.g && vDiffuseColor.b < g_vBlack_Discard.b)	// ������ �߶󳻱�
			discard;

        Out.vColor = vDiffuseColor * g_vColor_Mul;
		/* ============== �ҿ� / �����ص���! ���� �Ѱ� �����ڵ��� ! ==============  */ 
		// ���� ������ ���� ������Ʈ�ε�, �븻���� ��� ���� �ȵ�.. �� �ذ� ���̳� �̷��ֵ�ó�� �븻�ִ¾ֵ鸸 �����ҵ�..?
        //float4 vRimColor = Calculation_RimColor(In.vNormal, In.vPosition);
        //Out.vDiffuse += vRimColor;
		
		// Case1. ������ Diffuse�� ���� �Կ��� ȿ���� �ش�. 
        //Out.vRimBloom = Calculation_Brightness(Out.vColor);
		// Case2. ������ �ƿ� �־������ : �̰�� g_RimBloom_Color ����� �������� �޾Ƽ� �װɷ� �ص���
        //Out.vRimBloom = float4(0.f, 0.f, 1.f, 1.f);
    }
	else
	{
		/* ù��° ������ ������� �ι�° ������ ��ġ�� �ִ� �ȼ��� ���� ���´�. */
		float4 vDiffuseColor = g_DiffuseTexture.Sample(PointSampler, In.vTexcoord);
		float4 vAlphaColor = g_MaskTexture.Sample(PointSampler, In.vTexcoord);

		vDiffuseColor.rgb *= In.vColor.rgb;
		vDiffuseColor.a = In.vColor.a * vAlphaColor;

		if (vDiffuseColor.a < g_fAlpha_Discard	// ���� �߶󳻱�
			|| vDiffuseColor.r < g_vBlack_Discard.r && vDiffuseColor.g < g_vBlack_Discard.g && vDiffuseColor.b < g_vBlack_Discard.b)	// ������ �߶󳻱�
			discard;

        Out.vColor = vDiffuseColor /** g_vColor_Mul*/;
		
		/* ============== �ҿ� / �����ص���! ���� �Ѱ� �����ڵ��� ! ==============  */ 
       // Out.vRimBloom = Calculation_Brightness(Out.vColor);
		Out.vRimBloom = float4(g_vBloomPower, 1.0f);
    }

	
    if (bSolid)
        Out.vSolid = Out.vColor;
	
	
    return Out;
}
// MAIN_PARTICLE ================================================================================================================




technique11 DefaultTechnique
{
	/* ���� ���ϴ� Ư�� ���̴����� �׸��� �𵨿� �����Ѵ�. */
	pass Particle  // 0
	{
		SetRasterizerState(RS_Cull_None);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

		/* ������������ */
		VertexShader	= compile vs_5_0 VS_MAIN_PARTICLE();
		GeometryShader	= compile gs_5_0 GS_MAIN();
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_PARTICLE(false);
	}

    pass Particle_Solid // 1
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

		/* ������������ */
        VertexShader	= compile vs_5_0 VS_MAIN_PARTICLE();
        GeometryShader	= compile gs_5_0 GS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_PARTICLE(true);
    }

}