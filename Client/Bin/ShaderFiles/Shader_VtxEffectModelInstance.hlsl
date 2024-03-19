#include "Shader_Defines.hlsli"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float       g_fTimeDelta;

Texture2D	g_DiffuseTexture;
Texture2D	g_MaskTexture;
Texture2D	g_NoiseTexture;

Texture2D	g_NormalTexture;
Texture2D   g_SpecularTexture;

// Camera ======================
vector      g_vCamPosition;
vector      g_vCamDirection;
float		g_fCamFar;
// =============================

/* UV */
float2      g_UVOffset;
float2      g_UVScale;
float		g_fDegree;


/* Color */
float4		g_vColor_Mul;


/* Discard */
float		g_fAlpha_Discard;
float3		g_vBlack_Discard;


/* Dissolve */
float		g_fDissolveRatio;


/* RimLight - �ʿ��Ѱ� �ΰ�����(+1�� �߰�) */
float4		g_vRimColor;
float		g_fRimPower;
float3      g_vBloomPower;


// Distortion ====================
float	g_fFrameTime;

int     g_iScrollType;

float3	g_vScrollSpeeds;
float3	g_vScales;

float2	g_vDistortion1;
float2	g_vDistortion2;
float2	g_vDistortion3;
float	g_fDistortionScale;
float	g_fDistortionBias;
// ================================


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


float2 RotateTexture(float2 texCoord, float angle)
{
    float2 rotatedTexCoord;
    rotatedTexCoord.x = texCoord.x * cos(angle) - texCoord.y * sin(angle);
    rotatedTexCoord.y = texCoord.x * sin(angle) + texCoord.y * cos(angle);
    
    return rotatedTexCoord;    
}


float IsIn_Range(float fMin, float fMax, float fValue)
{
	return (fMin <= fValue) && (fMax >= fValue);
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


float4 Calculation_Distortion(float2 In_TexUV, float2 In_vTexcoord1, float2 In_vTexcoord2, float2 In_vTexcoord3)
{
    float4 vDistortionTex_1;
    float4 vDistortionTex_2;
    float4 vDistortionTex_3;
    
    float4 vDistortionTex_Final;
   
	
	// ����� �ؽ����� �ؽ���带 ù��° ũ�� �� ������ ��ũ�� �ӵ� ���� �̿��Ͽ� ��� x 3
    // �ؽ���忡 ���ϱ�� ũ�� ��ȭ
    In_vTexcoord1 = (In_TexUV * g_vScales.x);
    In_vTexcoord2 = (In_TexUV * g_vScales.y);
    In_vTexcoord3 = (In_TexUV * g_vScales.z);
    
    
    if (0 == g_iScrollType)
    {
        // SCROLL_ROW : ���� ��ũ�� 
        In_vTexcoord1.x = In_vTexcoord1.x + (g_fFrameTime * g_vScrollSpeeds.x);
        In_vTexcoord2.x = In_vTexcoord2.x + (g_fFrameTime * g_vScrollSpeeds.y);
        In_vTexcoord3.x = In_vTexcoord3.x + (g_fFrameTime * g_vScrollSpeeds.z);     
    }
    else if (1 == g_iScrollType)
    {
        // SCROLL_COL : ���� ��ũ��      
        In_vTexcoord1.y = In_vTexcoord1.y + (g_fFrameTime * g_vScrollSpeeds.x);
        In_vTexcoord2.y = In_vTexcoord2.y + (g_fFrameTime * g_vScrollSpeeds.y);
        In_vTexcoord3.y = In_vTexcoord3.y + (g_fFrameTime * g_vScrollSpeeds.z);
    }
    else if (2 == g_iScrollType)
    {
        // SCROLL_BOTH : ���� + ���� ��ũ��
        In_vTexcoord1 = In_vTexcoord1 + (g_fFrameTime * g_vScrollSpeeds.x);
        In_vTexcoord2 = In_vTexcoord2 + (g_fFrameTime * g_vScrollSpeeds.y);
        In_vTexcoord3 = In_vTexcoord3 + (g_fFrameTime * g_vScrollSpeeds.z);
    }
	else if (3 == g_iScrollType)
    {
        In_vTexcoord1 = RotateTexture(In_vTexcoord1, g_fFrameTime * g_vScrollSpeeds.x);
        In_vTexcoord2 = RotateTexture(In_vTexcoord2, g_fFrameTime * g_vScrollSpeeds.y);
        In_vTexcoord3 = RotateTexture(In_vTexcoord3, g_fFrameTime * g_vScrollSpeeds.z);
    }


    // ������ �ؽ�ó�� �����(�ְ�ȿ��)�� �����.
	// �� �� ������ ������ �ؽ���������, ���� �ٸ� �ؽ���带 ����Ͽ��⿡ ���� �ٸ� ������� ���ø��ȴ�.  
    vDistortionTex_1 = g_NoiseTexture.Sample(LinearSampler, In_vTexcoord1);
    vDistortionTex_2 = g_NoiseTexture.Sample(LinearSampler, In_vTexcoord2);
    vDistortionTex_3 = g_NoiseTexture.Sample(LinearSampler, In_vTexcoord3);

	
	// ����� ���� ������ (0, 1)���� (-1, +1)�� �ǵ����Ѵ�.
    vDistortionTex_1 = (vDistortionTex_1 - 0.5f) * 2.0f;
    vDistortionTex_2 = (vDistortionTex_2 - 0.5f) * 2.0f;
    vDistortionTex_3 = (vDistortionTex_3 - 0.5f) * 2.0f;

	
	// ������� xy���� �� ���� ���� �ٸ� �ְ�xy��ǥ�� ��߸���.
    vDistortionTex_1.xy = vDistortionTex_1.xy * g_vDistortion1.xy;
    vDistortionTex_2.xy = vDistortionTex_2.xy * g_vDistortion2.xy;
    vDistortionTex_3.xy = vDistortionTex_3.xy * g_vDistortion3.xy;

	
	// �ְ�� �� ����� ������ �ϳ��� ��������� �ռ��Ѵ�.
    vDistortionTex_Final = vDistortionTex_1 + vDistortionTex_2 + vDistortionTex_3;

    
    return vDistortionTex_Final;
}
// Custom Function ==============================================================================================================



// MAIN =========================================================================================================================
struct VS_IN
{
	float3		vPosition	 : POSITION;
	float3		vNormal		 : NORMAL;
	float2		vTexUV		 : TEXCOORD0;
	float3		vTangent	 : TANGENT;

	float4		vRight		 : TEXCOORD1;
	float4		vUp			 : TEXCOORD2;
	float4		vLook		 : TEXCOORD3;
	float4		vTranslation : TEXCOORD4;

};

struct VS_OUT
{
	float4		vPosition	: SV_POSITION;
	float4		vNormal		: NORMAL;
	float2		vTexUV		: TEXCOORD0;
	float4		vWorldPos	: TEXCOORD1;
	float4		vProjPos	: TEXCOORD2;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT) 0;

	
	matrix WorldMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector vPosition = mul(vector(In.vPosition, 1.f), WorldMatrix);

	matrix matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}


struct PS_IN
{
	float4		vPosition	: SV_POSITION;
	float4		vNormal		: NORMAL;
	float2		vTexUV		: TEXCOORD0;
	float4		vWorldPos	: TEXCOORD1;
	float4		vProjPos	: TEXCOORD2;
};


struct PS_OUT
{	
	float4 vDiffuse			: SV_TARGET0;
    float4 vSolid			: SV_TARGET1;
	float4 vNormal			: SV_TARGET2;
	float4 vDepth			: SV_TARGET3;
	float4 vRimBloom		: SV_TARGET4;
    float4 vDistortion		: SV_TARGET5;
};


PS_OUT PS_MAIN(PS_IN In, uniform bool bSolid)
{
	PS_OUT Out = (PS_OUT) 0;
	
    In.vTexUV = In.vTexUV * g_UVScale + g_UVOffset;
    In.vTexUV = Rotate_Texcoord(In.vTexUV, g_fDegree);
	
	vector vDiffuseColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	float4 vAlphaColor = g_MaskTexture.Sample(LinearSampler, In.vTexUV);

	vDiffuseColor.a *= vAlphaColor;
	
	if (vDiffuseColor.a <= g_fAlpha_Discard)	// ���� �ڸ���
		discard;

	Out.vDiffuse = vDiffuseColor * g_vColor_Mul; // �� ���ϱ�

	
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f); /* -1 ~ 1 -> 0 ~ 1 */
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.0f, 0.0f);
   

    /* ---------------- New ---------------- :  */
	float4 vRimColor = Calculation_RimColor(In.vNormal, In.vWorldPos);
	Out.vDiffuse += vRimColor;
	//Out.vRimBloom = Calculation_Brightness(Out.vDiffuse) /*+ vRimColor*/;
	Out.vRimBloom = float4(g_vBloomPower, 1.0f);


    if (bSolid)
        Out.vSolid = Out.vDiffuse;
	

	return Out;

}
// MAIN =========================================================================================================================



//  Normal Mapping(MAIN_NORMAL) =================================================================================================
struct VS_OUT_NORMAL
{
	float4 vPosition	: SV_POSITION;
	float3 vNormal		: NORMAL;
	float2 vTexUV		: TEXCOORD0;
	float4 vWorldPos	: TEXCOORD1;
	float4 vProjPos		: TEXCOORD2;
	float3 vTangent		: TANGENT;
	float3 vBinormal	: BINORMAL;
};

VS_OUT_NORMAL VS_MAIN_NORMAL(VS_IN In)
{
	VS_OUT_NORMAL Out = (VS_OUT_NORMAL)0;


	matrix			WorldMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector			vPosition = mul(vector(In.vPosition, 1.f), WorldMatrix);

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix)).xyz;
	Out.vWorldPos = mul(vPosition, g_WorldMatrix);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix)).xyz;
	Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));

	return Out;
}


struct PS_IN_NORMAL
{
	float4 vPosition	: SV_POSITION;
	float3 vNormal		: NORMAL;
	float2 vTexUV		: TEXCOORD0;
	float4 vWorldPos	: TEXCOORD1;
	float4 vProjPos		: TEXCOORD2;
	float3 vTangent		: TANGENT;
	float3 vBinormal	: BINORMAL;
};


PS_OUT PS_MAIN_NORMAL(PS_IN_NORMAL In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
    clip(Out.vDiffuse.a - g_fAlpha_Discard);
    Out.vDiffuse.a = 1.f;
	/* 0 ~ 1 */
    float3 vPixelNormal = g_NormalTexture.Sample(LinearSampler, In.vTexUV).xyz;

	/* -1 ~ 1 */
	vPixelNormal = vPixelNormal * 2.f - 1.f;

	float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vPixelNormal = mul(vPixelNormal, WorldMatrix);

	Out.vNormal = vector(vPixelNormal * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, 0.f);
	
	return Out;
}

//  Normal Mapping(MAIN_NORMAL) =================================================================================================



//  Dissolve ====================================================================================================================
PS_OUT PS_MAIN_Dissolve(PS_IN_NORMAL In)
{
	PS_OUT Out = (PS_OUT)0;

	vector TexDissolve = g_NoiseTexture.Sample(LinearSampler, In.vTexUV);

    clip(TexDissolve - g_fDissolveRatio);

    In.vTexUV = In.vTexUV * g_UVScale + g_UVOffset;
    In.vTexUV = Rotate_Texcoord(In.vTexUV, g_fDegree);
	
    vector vTexDiff = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

    float fStepValue = IsIn_Range(0.f, 0.05f, TexDissolve.r - g_fDissolveRatio);
	
    Out.vDiffuse = (1.f - fStepValue) * vTexDiff + fStepValue ;
	
    clip(Out.vDiffuse.a - g_fAlpha_Discard);
    //Out.vDiffuse.a = 1.f;

	float4 vAlphaMask = g_MaskTexture.Sample(LinearSampler, In.vTexUV);
	Out.vDiffuse.a *= vAlphaMask.a;
	
    float3 vPixelNormal = g_NormalTexture.Sample(LinearSampler, In.vTexUV).xyz;
	vPixelNormal = vPixelNormal * 2.f - 1.f;

	float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vPixelNormal = mul(vPixelNormal, WorldMatrix);
	
	Out.vNormal    = vector(vPixelNormal * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, 0.f);

	
	
	 /* ---------------- New ---------------- :  */
    float4 vRimColor = Calculation_RimColor(float4(In.vNormal.r, In.vNormal.g, In.vNormal.b, 0.f), In.vWorldPos);
    Out.vDiffuse += vRimColor;
	//Out.vRimBloom = Calculation_Brightness(Out.vDiffuse) /*+ vRimColor*/;
    Out.vRimBloom = float4(g_vBloomPower, 1.0f);

    Out.vSolid = Out.vDiffuse;
	
	
	
	return Out;
}
//  Dissolve ====================================================================================================================


//  DISTORTION ==================================================================================================================
struct VS_OUT_DISTORTION
{
	float4 vPosition	: SV_POSITION;
	float3 vNormal		: NORMAL;
	float2 vTexUV		: TEXCOORD0;
	float4 vWorldPos	: TEXCOORD1;
	float4 vProjPos		: TEXCOORD2;
	float3 vTangent		: TANGENT;
	float3 vBinormal	: BINORMAL;

	float2 vTexcoord1	: TEXCOORD3;
	float2 vTexcoord2	: TEXCOORD4;
	float2 vTexcoord3	: TEXCOORD5;
};


VS_OUT_DISTORTION VS_MAIN_DISTORTION(VS_IN In)
{
	VS_OUT_DISTORTION Out = (VS_OUT_DISTORTION) 0;


	matrix WorldMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector vPosition = mul(vector(In.vPosition, 1.f), WorldMatrix);

	matrix matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN_DISTORTION
{
	float4 vPosition	: SV_POSITION;
	float3 vNormal		: NORMAL;
	float2 vTexUV		: TEXCOORD0;
	float4 vWorldPos	: TEXCOORD1;
	float4 vProjPos		: TEXCOORD2;
	float3 vTangent		: TANGENT;
	float3 vBinormal	: BINORMAL;
	
	float2 vTexcoord1	: TEXCOORD3;
	float2 vTexcoord2	: TEXCOORD4;
	float2 vTexcoord3	: TEXCOORD5;
};



PS_OUT PS_MAIN_DISTORTION(PS_IN_DISTORTION In, uniform bool bSolid)
{
	PS_OUT Out = (PS_OUT) 0;

	
	/* Distortion ============================================================ */	
    float4  vDistortion;
	float	fPerturb;
    float2  vDistortedCoord;
	
	float4	vFinalDiffuse;
	float4	vAlphaColor;

    
    In.vTexUV = In.vTexUV * g_UVScale + g_UVOffset;
    In.vTexUV = Rotate_Texcoord(In.vTexUV, g_fDegree);
	
    vDistortion = Calculation_Distortion(In.vTexUV, In.vTexcoord1, In.vTexcoord2, In.vTexcoord3);

	
	// �Է����� ���� �ؽ����� Y��ǥ�� �ְ� ũ��� ���̾ ������ ������Ų��. �� ������ �ؽ����� �������� ������ �������� �� ���ʿ��� �����̴� ȿ���� ������.
	fPerturb = ((1.0f - In.vTexUV.y) * g_fDistortionScale) + g_fDistortionBias;

	
	// �ؽ��ĸ� ���ø��ϴµ� ���� �ְ� �� ������ �ؽ��� ��ǥ��(UV) �����.
    vDistortedCoord = (vDistortion.xy * fPerturb) + In.vTexUV.xy;


	// ��ǻ�� �ؽ�ó (clamp ���÷� ���)
    vFinalDiffuse = g_DiffuseTexture.Sample(ClampSampler, vDistortedCoord.xy);


	// ����ũ �ؽ�ó�� ���ķ� ��� (clamp ���÷� ���)
    vAlphaColor = g_MaskTexture.Sample(ClampSampler, vDistortedCoord.xy);
    vFinalDiffuse.a = vAlphaColor;
	
    
	/* Discard & Color Mul ==================================================== */
    if (Out.vDiffuse.a < g_fAlpha_Discard) // ���� �߶󳻱�
        discard;
	
    Out.vDiffuse = vFinalDiffuse * g_vColor_Mul;
	
    
	/* Dissolve ============================================================== */
    vector vDissolveTex = g_NoiseTexture.Sample(LinearSampler, In.vTexUV);
    clip(vDissolveTex - g_fDissolveRatio);

    float fStepValue = IsIn_Range(0.f, 0.05f, vDissolveTex.r - g_fDissolveRatio);

    vFinalDiffuse = (1.f - fStepValue) * vFinalDiffuse + fStepValue;

		

	/* Normal & Depth ========================================================= */
	float3 vPixelNormal = g_NormalTexture.Sample(LinearSampler, In.vTexUV).xyz;
	vPixelNormal = vPixelNormal * 2.f - 1.f;

	float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vPixelNormal = mul(vPixelNormal, WorldMatrix);

	Out.vNormal = vector(vPixelNormal * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, 0.f);
	
	
	/* RimBloom ================================================================ */
    float4 vRimColor = Calculation_RimColor(float4(In.vNormal.r, In.vNormal.g, In.vNormal.b, 0.f), In.vWorldPos);
    Out.vDiffuse += vRimColor;
    Out.vRimBloom = float4(g_vBloomPower, 1.0f);	//Out.vRimBloom = Calculation_Brightness(Out.vDiffuse) /*+ vRimColor*/;
	
	
	
    if (bSolid)
        Out.vSolid = Out.vDiffuse;
	
	
	return Out;
}
//  DISTORTION ==================================================================================================================




//  PS_MAIN_DISTORTION_DEFERRED =======================================================================================================
PS_OUT PS_MAIN_DISTORTION_DEFERRED(PS_IN_DISTORTION In)
{
    PS_OUT Out = (PS_OUT) 0;	
    
    In.vTexUV = In.vTexUV * g_UVScale + g_UVOffset;
    In.vTexUV = Rotate_Texcoord(In.vTexUV, g_fDegree);
    

	/* Distortion ============================================================ */ 
    float4 vDistortion;
    
    vDistortion = Calculation_Distortion(In.vTexUV, In.vTexcoord1, In.vTexcoord2, In.vTexcoord3);
    Out.vDistortion = vDistortion;
    
    
	/* Depth ================================================================ */
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, 0.f);
	
	
	/* RimBloom ============================================================== */
    Out.vDiffuse = float4(0.f, 0.f, 0.f, 0.f);
    Out.vRimBloom = float4(g_vBloomPower, 1.0f); //Out.vRimBloom = Calculation_Brightness(Out.vDiffuse) /*+ vRimColor*/;
	
      
    return Out;
}
//  PS_MAIN_DISTORTION_DEFERRED =======================================================================================================



technique11 DefaultTechnique
{
    pass Effect // 0
    {	
		//SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff); //SetBlendState(BS_AlphaBlend_Add, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetBlendState(BS_AlphaBlend_Add, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthStencilEnable, 0);	
		SetRasterizerState(RS_Cull_None); //SetRasterizerState(RS_Default);

		VertexShader	= compile vs_5_0 VS_MAIN();
		HullShader		= NULL;
		DomainShader	= NULL;
		GeometryShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN(false);
	}

    pass Effect_Solid // 1
    {
		//SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff); //SetBlendState(BS_AlphaBlend_Add, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetBlendState(BS_AlphaBlend_Add, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthStencilEnable, 0);
        SetRasterizerState(RS_Cull_None); //SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN(true);
    }

	pass WireFrame // 2
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthStencilEnable, 0);
        SetRasterizerState(RS_NoneCull_Wireframe);

		VertexShader	= compile vs_5_0 VS_MAIN();
		HullShader		= NULL;
		DomainShader	= NULL;
		GeometryShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();
	}

    pass Dissolve // 3
    {
        SetBlendState(BS_AlphaBlend_Add, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthStencilEnable, 0);
        SetRasterizerState(RS_Cull_None);

        VertexShader	= compile vs_5_0 VS_MAIN_NORMAL();
        HullShader		= NULL;
        DomainShader	= NULL;
        GeometryShader	= NULL;
        PixelShader		= compile ps_5_0 PS_MAIN_Dissolve();
    }
	
	pass Distortion // 4
	{
		SetBlendState(BS_AlphaBlend_Add, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthStencilEnable, 0);
		SetRasterizerState(RS_Cull_None);

		VertexShader	= compile vs_5_0 VS_MAIN_DISTORTION();
		HullShader		= NULL;
		DomainShader	= NULL;
		GeometryShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_DISTORTION(false);
	}

    pass Distortion_Solid // 5
    {
        SetBlendState(BS_AlphaBlend_Add, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthStencilEnable, 0);
        SetRasterizerState(RS_Cull_None);

        VertexShader = compile vs_5_0 VS_MAIN_DISTORTION();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DISTORTION(true);
    }

    pass Distortion_Deferred // 6
    {
        //SetBlendState(BS_AlphaBlend_Add, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthStencilEnable, 0);
        SetRasterizerState(RS_Cull_None);

        VertexShader = compile vs_5_0 VS_MAIN_DISTORTION();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DISTORTION_DEFERRED();
    }

}