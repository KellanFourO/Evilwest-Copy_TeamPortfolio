#pragma once

#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagGraphicDesc
	{
		enum WINMODE { WINMODE_FULL, WINMODE_WIN, WINMODE_END };

		WINMODE			eWinMode;
		HWND			hWnd;
		unsigned int	iBackBufferSizeX;
		unsigned int	iBackBufferSizeY;

	}GRAPHIC_DESC;

	typedef struct tagMtrl_Texture
	{
		class CTexture* pMtrlTextures[AI_TEXTURE_TYPE_MAX];

	}MATERIAL_DESC;

	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_SPOTLIGHT, TYPE_END };

		_bool	bEnable = true;					/* 불 킬건지 끌건지 여부 */
		TYPE	eType = TYPE_END;

		/* Directional Light  */
		_float4 vDirection = { 0.f, 0.f, 0.f, 0.f };

		/* Point Light  */
		_float4 vPosition = { 0.f, 0.f, 0.f, 0.f };
		_float	 fRange = 0.f;

		/* Spot Light  */
		_float fCutOff = 0.f;
		_float fOuterCutOff = 0.f;

		/* Common */
		_float4 vDiffuse = { 0.f, 0.f, 0.f, 0.f }; /* 반사될때 출력되는 주된 색 */
		_float4 vAmbient = { 0.f, 0.f, 0.f, 0.f }; /* 광원의 확산위치와 무관하게 똑같은양으로 모든점에서 반사되는 색 */
		_float4 vSpecular = { 0.f, 0.f, 0.f, 0.f }; /* 특정방향으로만 반사되는 색 */

	public:
		unsigned int iLightIndex = unsigned int(-1);
		const unsigned int Get_LightIndex() const { return iLightIndex; }

	}LIGHT_DESC;

	typedef struct tagAnimation_Channel_Info
	{
		XMFLOAT3		vScale, vPosition;
		XMFLOAT4		vRotation;
		float			fTrackPosition;
	}KEYFRAME;



#pragma region Shader

	typedef struct ENGINE_DLL tagVertex_Position
	{
		XMFLOAT3		vPosition;

		tagVertex_Position()
		{
			ZeroMemory(this, sizeof(tagVertex_Position));
		}

		static const unsigned int					iNumElements = 1;
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXPOS;


	typedef struct ENGINE_DLL tagVertex_Position_Size
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vPSize;

		tagVertex_Position_Size()
		{
			ZeroMemory(this, sizeof(tagVertex_Position_Size));
		}
		static const unsigned int					iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXPOINT;

	typedef struct ENGINE_DLL tagVertex_Position_Texcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		tagVertex_Position_Texcoord()
		{
			ZeroMemory(this, sizeof(tagVertex_Position_Texcoord));
		}

		static const unsigned int					iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXPOSTEX;

	typedef struct ENGINE_DLL tagVertex_Cube
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexcoord;

		tagVertex_Cube()
		{
			ZeroMemory(this, sizeof(tagVertex_Cube));
		}

		static const unsigned int					iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXCUBE;

	typedef struct ENGINE_DLL tagVertex_Position_Normal_Texcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;

		tagVertex_Position_Normal_Texcoord()
		{
			ZeroMemory(this, sizeof(tagVertex_Position_Normal_Texcoord));
		}

		static const unsigned int					iNumElements = 3;
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertex_Position_Normal_Texcoord_Tangent
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;

		tagVertex_Position_Normal_Texcoord_Tangent()
		{
			ZeroMemory(this, sizeof(tagVertex_Position_Normal_Texcoord_Tangent));
		}

		static const unsigned int					iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXMESH;

	typedef struct ENGINE_DLL tagVertex_Position_Normal_Texcoord_Tangent_Blendindices_Blendweight
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;
		XMUINT4			vBlendIndices;
		XMFLOAT4		vBlendWeights;

		tagVertex_Position_Normal_Texcoord_Tangent_Blendindices_Blendweight()
		{
			ZeroMemory(this, sizeof(tagVertex_Position_Normal_Texcoord_Tangent_Blendindices_Blendweight));
		}

		static const unsigned int					iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXANIMMESH;

	/* 파티클 + 인스턴싱 */
	typedef struct ENGINE_DLL tagParticle_Rect /* 셰이더 그리는용도 */
	{
		static const unsigned int					iNumElements = 7;
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTX_PARTICLE_RECT;

	typedef struct ENGINE_DLL tagParticle_Point
	{
		static const unsigned int					iNumElements = 8;
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTX_PARTICLE_POINT;

	typedef struct
	{
		XMFLOAT4		vRight, vUp, vLook, vPosition;
		XMFLOAT4		vColor;
	}VTXINSTANCE;


	typedef struct ENGINE_DLL tagVertex_Model_Instance
	{
		XMFLOAT4			vRight;
		XMFLOAT4			vUp;
		XMFLOAT4			vLook;
		XMFLOAT4			vTranslation;
	}VTXMODELINSTANCE;

	typedef struct ENGINE_DLL tagVertex_AnimModel_Instance
	{
		_float3		vPosition;
		_float3		vNormal;
		_float2		vTexture;
		_float3		vTangent;
		XMUINT4		vBlendIndex; /* 이 정점에 영향을 주는 뼈의 인덱스 네개. */
		_float4		vBlendWeight; /* 영향르 주고 있는 각 뼈대의 영향 비율 */

		_uint		iID;
		_matrix		matWorld;

	}VTXANIMMODELINSTANCE;

	typedef struct ENGINE_DLL tagVertex_Model_Instance_Declaration
	{
		static const unsigned int				iNumElements = 8;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTXMODEL_INSTANCE_DECLARATION;

	typedef struct ENGINE_DLL tagVertex_AnimModel_Instance_Declaration
	{
		static const unsigned int				iNumElements = 11;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTXMODEL_ANIMMODEL_INSTANCE_DECLARATION;

	typedef struct ENGINE_DLL tagVertex_Dynamic_Field
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;

		static const unsigned int					iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXFIELD;

	template <typename T>
	constexpr const T& clamp(const T& value, const T& start, const T& end)
	{
		return (value < start) ? start : (value > end) ? end : value;
	}

	template<typename T, typename = enable_if_t<is_enum<T>::value>,
		typename Return = underlying_type_t<T>>
		constexpr Return ECast(T value)
	{
		return static_cast<Return>(value);
	}

	template<typename EnumType>
	EnumType ECast(int value) {
		return static_cast<EnumType>(value);
	}

#pragma endregion


#pragma region 구조체

	typedef struct ENGINE_DLL tag_InstanceDesc
	{
		_float3         vScale;
		_float4         vRotation;
		_float3			vTranslation;
		_float			fMaxRange;
		_float3			vCenter;

		_float3 Get_Position() { return vTranslation; }

		_matrix Get_Matrix() const
		{
			_matrix TransformationMatrix;
			_matrix RotationMatrix, ScaleMatrix;

			_vector vQuarternion;
			_vector vPosition;

			
			vQuarternion = XMLoadFloat4(&vRotation);
			vPosition = XMVectorSetW(XMLoadFloat3(&vTranslation), 1.f);

			//vPitchYawRoll = XMQuaternionRotationRollPitchYawFromVector(vPitchYawRoll);

			RotationMatrix = XMMatrixRotationQuaternion(vQuarternion);
//			RotationMatrix = XMMatrixRotationRollPitchYawFromVector(vPitchYawRoll);
			ScaleMatrix = XMMatrixScaling(vScale.x, vScale.y, vScale.z);
			TransformationMatrix = ScaleMatrix * RotationMatrix;
			TransformationMatrix.r[3] = vPosition;

			return TransformationMatrix;
		}


		void Set_Matrix(const _fmatrix& matrix)
		{

			_vector vTempScale, vTempRotation, vTempTranslation;

		
			XMMatrixDecompose(&vTempScale, &vTempRotation, &vTempTranslation, matrix);

			XMStoreFloat3(&vScale, vTempScale);
			XMStoreFloat4(&vRotation, vTempRotation);
			XMStoreFloat3(&vTranslation, vTempTranslation);
		}

		void	Bake_CenterWithMatrix()
		{
			_vector vCenterFromVector = XMLoadFloat3(&vCenter);
			XMStoreFloat3(&vCenter, XMVector3TransformCoord(vCenterFromVector, Get_Matrix()));
		}

	}INSTANCE_INFO_DESC;

	typedef struct ENGINE_DLL tagAnimInstanceDesc
	{
		const _uint			iMaxInstanceCount = 100;
		_uint				iNumInstance = { 0 };
		ID3D11Texture2D* pAnimInstanceTexture = { nullptr };
		//ID3D11ShaderResourceView* pAnimSRV = nullptr;
	}ANIM_INSTANCE_INFO_DESC;

	typedef struct ENGINE_DLL tagEnvironment_Desc
	{
		wstring strModelTag = {};
		_uint	iNumInstance = { 0 };
		_uint	iShaderPassIndex = { 1 };
		vector<INSTANCE_INFO_DESC> vecInstanceInfoDesc;
	}MAPTOOL_INSTANCE_DESC;

	typedef struct ENGINE_DLL tagRayDesc
	{
		XMFLOAT4 vPosition;
		XMFLOAT3 vDirection;
		float	fLength;
	}RAY;

	typedef struct ENGINE_DLL AnimInstanceDesc
	{
		_uint						iMaxInstanceCount = 0;
		_uint						iSizePerSecond = 0;
		//class CShader*				pInstanceShader =	{ nullptr };

		ID3D11ShaderResourceView* pInstanceSRV = { nullptr };
		ID3D11Texture2D* pInstanceTexture = { nullptr };
		BYTE* pByte = { nullptr };
		_float4x4* pMatrix = { nullptr };

		AnimInstanceDesc() {};
		~AnimInstanceDesc();

	}ANIMMODEL_INSTANCE_DESC;

	struct InstanceDataElement
	{
		_float4 world1;            // the world transform for this matrix row 1
		_float4 world2;            // the world transform for this matrix row 2
		_float4 world3;            // the world transform for this matrix row 3 (row 4 is implicit)

		// $ Technically this is bundled, but there is not class that makes a uint vector, so just keep flat
		UINT animationIndex;            // offset in vectors into the whole data stream for the start of the animation playing
		UINT frameOffset;            // offset in vectors into the animation stream for the start of the frame playing
		UINT attachmentSet;            // the id to determine which geo attachments get set
		UINT lerpValue;            // lerp between frames
	};



#pragma endregion 구조체

#pragma region Shader Control Struct - Screen 
	/* 각자 구분용 */

	typedef struct ENGINE_DLL tagHBAO_Plus_Desc
	{
		_bool  bHBAO_Active			= false;
		_float fRadius				= 1.f;
		_float fBias				= 0.1f;
		_float fPowerExponent		= 2.f;
		_float fBlur_Sharpness		= 16.f;

	}HBAO_PLUS_DESC;

	typedef struct ENGINE_DLL tagBloomRim_Desc
	{
		_bool	bRimBloom_Blur_Active		= { false };
		_bool	bShadow_Active				= { false };
		_float2 padding = {};
	}DEFERRED_DESC;

	typedef struct ENGINE_DLL tagFogDesc
	{
		_bool	bFog_Active				= false;
		_float	fFogStartDepth			= 55.f;
		_float	fFogStartDistance		= 0.1f;
		_float	fFogDistanceValue		= 30.f;
		_float	fFogHeightValue			= 50.f;
		_float	fFogDistanceDensity		= 0.05f;
		_float	fFogHeightDensity		= 0.05f;
		_float3 padding = {};
		_float4 vFogColor				= { 0.5f, 0.5f, 0.5f, 0.2f };

	} FOG_DESC;

	typedef struct ENGINE_DLL tagHDRDesc
	{
		_bool  bHDR_Active			= false;
		_float fmax_white			= 0.4f;
	}HDR_DESC;

	typedef struct ENGINE_DLL tagAnti_Aliasing
	{
		_bool  bFXAA_Active = false;
	}ANTI_DESC;

	typedef struct ENGINE_DLL tagScreenDesc
	{
		_bool bScreen_Active = false;
		_float fFinal_Saturation = 1.f;
		_float fFinal_Brightness = 1.f;

	}HSV_DESC;

	typedef struct ENGINE_DLL tagRadialBlurDesc
	{
		_bool	bRadial_Active	= false;
		_float	fRadial_Quality = 16.f;
		_float	fRadial_Power	= 0.1f;
	}RADIAL_DESC;

	typedef struct ENGINE_DLL tagDOF
	{
		_bool  bDOF_Active		= false;
		float  fFocusDistance = 10.f;
		float  fFocusRange = 3.f;

	}DOF_DESC;

	/* 전체 컨트롤 - 레벨시작할때 초기 컨트롤용도 */
	typedef struct ENGINE_DLL tagLevelShader
	{
		/* 활성여부 */
		_bool bHBAO_Plus_Active		= { false };
		_bool bFog_Active			= { false };
		_bool bHDR_Active			= { false };
		_bool bFXAA_Active			= { false };

		/* HBAO+ */
		_float fRadius				= 1.f;
		_float fBias				= 0.1f;
		_float fPowerExponent		= 2.f;
		_float fBlur_Sharpness		= 16.f;

		/* Fog */
		_float fFogStartDepth		= 100.f;
		_float fFogStartDistance	= 10.f;
		_float fFogDistanceValue	= 30.f;
		_float fFogHeightValue		= 50.f;
		_float fFogDistanceDensity	= 0.04f;
		_float fFogHeightDensity	= 0.04f;
		
		/* HDR */
		_float fmax_white			= 0.4f;

		/* Screen */
		_float fFinal_Saturation	= 1.f;
		_float fFinal_Brightness	= 1.f;

	}LEVEL_SHADER_DESC;


#pragma endregion

#pragma region Shader Control Struct - Object

	typedef struct ENGINE_DLL tagObject_Shader_Desc
	{
		_bool bRimLight; 
		_bool bBloom;
		
		_float4 vBloom_Color = {};
		_float3 vBloomPoser = {};
		_float4 vRimColor = {};

	}OBJECT_SHADER_DESC;

#pragma endregion


}

#endif // Engine_Struct_h__
