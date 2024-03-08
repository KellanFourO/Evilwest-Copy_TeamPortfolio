#include "Shader_Defines.hlsli"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float       g_fTimeDelta;

Texture2D	g_DiffuseTexture;
Texture2D	g_MaskTexture;
Texture2D	g_NoiseTexture;

Texture2D	g_NormalTexture;
Texture2D   g_SpecularTexture;

Texture2D	g_DepthTexture;


/* Camera */
vector      g_vCamPosition;
vector      g_vCamDirection;
float		g_fCamFar;

vector      g_vPlayerPosition;

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
texture2D	g_DissolveDiffTexture;
float		g_fDissolveWeight;
float		g_fDissolveRatio;


/* Bloom */
float4      g_BloomColor = { 0.f, 0.f, 0.f, 0.f };
float3      g_vBloomPower;


/* RimLight */
float4		g_vRimColor;
float		g_fRimPower;



/* Custom Function */
float4 Caculation_Brightness(float4 vColor, float3 BloomPower)
{
	float4 vBrightnessColor = float4(0.f, 0.f, 0.f, 0.f);

	float fPixelBrightness = dot(vColor.rgb, BloomPower.rgb);

	if (fPixelBrightness > 0.99f)
		vBrightnessColor = float4(vColor.rgb, 1.0f);

	return vBrightnessColor;
}

float IsIn_Range(float fMin, float fMax, float fValue)
{
	return (fMin <= fValue) && (fMax >= fValue);
}

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

struct VS_IN
{
	float3		vPosition	: POSITION;
	float3		vNormal		: NORMAL;
	float2		vTexUV		: TEXCOORD0;
	float3		vTangent	: TANGENT;

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

	float3		vViewNormal		: NORMAL1;		/* ssao */
	float3		vPositionView	: POSITION;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	
	matrix			WorldMatrix= float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector			vPosition = mul(vector(In.vPosition, 1.f), WorldMatrix);

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;


	// SSAO
	Out.vViewNormal = mul(Out.vNormal.xyz, (float3x3) g_ViewMatrix);
	Out.vPositionView = mul(float4(In.vPosition, 1.0f), matWV);

	return Out;	
}


VS_OUT VS_MAIN_SPRITE(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	matrix WorldMatrix = g_WorldMatrix;

	float3 vLook = normalize((g_vCamDirection * -1.f).xyz);
	float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook));
	float3 vUp = normalize(cross(vLook, vRight));

	WorldMatrix[0] = float4(vRight, 0.f) * length(WorldMatrix[0]);
	WorldMatrix[1] = float4(vUp, 0.f) * length(WorldMatrix[1]);
	WorldMatrix[2] = float4(vLook, 0.f) * length(WorldMatrix[2]);

	/* In.vPosition * 월드 * 뷰 * 투영 */
	matrix matWV, matWVP;

	matWV = mul(WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}



// w나누기연산을 수행한다. (In 투영스페이스)
// 뷰포트 변환. (In 뷰포트(윈도우좌표))

// 래스터라이즈(픽셀정보를 생성한다. )

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;

	/* ssao */
	float3	vViewNormal		: NORMAL1;
	float3	vPositionView	: POSITION;
};

struct PS_OUT
{	
	float4	vDiffuse        : SV_TARGET0;
	float4	vNormal         : SV_TARGET1;
	float4  vDepth          : SV_TARGET2;
	float4  vORM            : SV_TARGET3;
	float4  vViewNormal     : SV_TARGET4;
	float4  vBloom          : SV_TARGET5;
  
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	////* g_UVScale + g_UVOffset
	//In.vTexUV = In.vTexUV * g_UVScale + g_UVOffset;

	//Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	//float4 vAlphaMask = g_MaskTexture.Sample(LinearSampler, In.vTexUV);

	////clip(Out.vDiffuse.a - g_fAlpha_Discard);
	//Out.vDiffuse.a *= vAlphaMask.a;
	//
	//Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	//Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);
	

	vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (vMtrlDiffuse.a < 0.1f)
		discard;

	Out.vDiffuse = vMtrlDiffuse * g_vColor_Mul;	// 색 곱하기
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f); /* -1 ~ 1 -> 0 ~ 1 */
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.0f, 0.0f);

	/* 림라이트 -> 프레넬 공식 사용 */
	/* 노말 (내적) 정점이 카메라를 바라보는방향 -> 카메라가 조명처럼 인식된다. */
	//float fRim = saturate(dot(In.vNormal, (g_vCamPosition - In.vWorldPos)));
	//int iRimPower = 5.f;
	///* 일정이상보다 작으면 Rim을 없앤다. */
	//if(fRim > 0.3)
	//    fRim = 1;
	//else
	//    fRim = -1;

	//Out.vEmissive = Out.vORM + float4(pow(1 - fRim, iRimPower) * g_vRimColor.xyz, 1.f);

	float fRimPower = 1.f - saturate(dot(In.vNormal, normalize((-1.f * (In.vWorldPos - g_vCamPosition)))));
	fRimPower = pow(fRimPower, 5.f) * g_fRimPower;
	vector vRimColor = g_vRimColor * fRimPower;

	Out.vBloom = Caculation_Brightness(Out.vDiffuse, g_vBloomPower) + vRimColor;
	Out.vDiffuse += vRimColor;
	Out.vORM = g_SpecularTexture.Sample(LinearSampler, In.vTexUV);
	Out.vViewNormal = float4(normalize(In.vViewNormal), In.vPositionView.z);


	// 검은색 잘라내기
	if (Out.vDiffuse.r < g_vBlack_Discard.r && Out.vDiffuse.g < g_vBlack_Discard.g && Out.vDiffuse.b < g_vBlack_Discard.b)
		discard;

	return Out;

}

//// Normal Mapping ///////////
struct VS_OUT_NORMAL
{
	float4 vPosition : SV_POSITION;
	float3 vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
	float4 vWorldPos : TEXCOORD1;
	float4 vProjPos : TEXCOORD2;
	float3 vTangent : TANGENT;
	float3 vBinormal : BINORMAL;
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
	float4 vPosition : SV_POSITION;
	float3 vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
	float4 vWorldPos : TEXCOORD1;
	float4 vProjPos : TEXCOORD2;
	float3 vTangent : TANGENT;
	float3 vBinormal : BINORMAL;

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


struct VS_OUT_SHADOW
{
	float4 vPosition : SV_POSITION;
	float4 vProjPos : TEXCOORD0;
};

VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN In)
{
	VS_OUT_SHADOW		Out = (VS_OUT_SHADOW)0;


	matrix			WorldMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector			vPosition = mul(vector(In.vPosition, 1.f), WorldMatrix);

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN_SHADOW
{
	float4 vPosition : SV_POSITION;
	float4 vProjPos : TEXCOORD0;
};

struct PS_OUT_SHADOW
{
	vector vLightDepth : SV_TARGET0;
};

PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN_SHADOW In)
{
	PS_OUT_SHADOW Out = (PS_OUT_SHADOW)0;

	Out.vLightDepth.r = In.vProjPos.w / 300.f;

	Out.vLightDepth.a = 1.f;

	return Out;
}


PS_OUT PS_MAIN_Dissolve(PS_IN_NORMAL In)
{
	PS_OUT Out = (PS_OUT)0;

	vector TexDissolve = g_NoiseTexture.Sample(LinearSampler, In.vTexUV);

    clip(TexDissolve - g_fDissolveRatio);

	In.vTexUV = In.vTexUV * g_UVScale + g_UVOffset;
	In.vTexUV = Rotate_Texcoord(In.vTexUV, g_fDegree);

    vector vTexDiff = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

    float fStepValue = IsIn_Range(0.f, 0.05f, TexDissolve.r - g_fDissolveRatio);
	

    Out.vDiffuse = (1.f - fStepValue) * vTexDiff + fStepValue * g_DissolveDiffTexture.Sample(LinearSampler, In.vTexUV);
	
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

	return Out;
}

PS_OUT PS_MAIN_SPRITE_ANIMATION(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float2 clippedTexCoord = In.vTexUV * g_UVScale + g_UVOffset;

	// Set Color
	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, clippedTexCoord);

	float2 vDepthTexcoord;
	vDepthTexcoord.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	vDepthTexcoord.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	float4 vDepthDesc = g_DepthTexture.Sample(PointSampler, vDepthTexcoord);

	Out.vDiffuse.a = Out.vDiffuse.a * (vDepthDesc.y * 1000.f - In.vProjPos.w) * 2.f;
	// Alpha Test
	if (Out.vDiffuse.a < 0.1f)
	{
		discard;
	}

	return Out;
}

technique11 DefaultTechnique
{
	pass Default //0
	{
		SetBlendState(BS_AlphaBlend_Add, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthStencilEnable, 0);
		SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Pass1_NormalMapping //1
	{
        SetBlendState(BS_AlphaBlend_Add, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthStencilEnable, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
        HullShader = NULL;
        DomainShader = NULL;
		GeometryShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_NORMAL();
	}

	pass Pass2_ShadowDepth //2
	{
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_None, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN_SHADOW();
        HullShader = NULL;
        DomainShader = NULL;
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
	}

	pass Pass3_WireFrame //3
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthStencilEnable, 0);
        SetRasterizerState(RS_NoneCull_Wireframe);

		VertexShader = compile vs_5_0 VS_MAIN();
		HullShader = NULL;
		DomainShader = NULL;
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Pass4_NonCulling //4
	{
        SetBlendState			(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState    (DSS_DepthStencilEnable, 0);
        SetRasterizerState		(RS_Cull_None);

        VertexShader    = compile vs_5_0 VS_MAIN();
        HullShader      = NULL;
        DomainShader    = NULL;
		GeometryShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();
	}

	pass Pass5_NonCulling_Norm //5
	{
        SetBlendState			(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState	(DSS_DepthStencilEnable, 0);
        SetRasterizerState		(RS_Cull_None);

        VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
        HullShader = NULL;
        DomainShader = NULL;
		GeometryShader	= NULL;
		PixelShader		= compile ps_5_0	PS_MAIN_NORMAL();
	}

    pass Pass6_Dissolve //6
    {
        SetBlendState(BS_AlphaBlend_Add, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_DepthStencilEnable, 0);
        SetRasterizerState(RS_Cull_None);

        VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_Dissolve();
    }
	
	pass Pass7_Sprite_Animation //7
	{
		SetBlendState(BS_AlphaBlend_Add, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_DepthStencilEnable, 0);
		SetRasterizerState(RS_Cull_None);

		VertexShader = compile vs_5_0 VS_MAIN_SPRITE();
		HullShader = NULL;
		DomainShader = NULL;
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SPRITE_ANIMATION();
		
	}


}