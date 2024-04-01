#pragma once

#include "Client_Defines.h"

#include "AlphaObject.h"
#include "Easing_Utillity.h"

BEGIN(Client)

class CEffect_Void abstract : public CAlphaObject
{
public:
	enum TYPE_EFFECT { PARTICLE, RECT, MESH, TRAIL, TYPE_EFFECT_END };
	enum TEXTURE	 { TEXTURE_DIFFUSE, TEXTURE_NORAML, TEXTURE_MASK, TEXTURE_NOISE, TEXTURE_SPRITE, TEXTURE_END };
	enum TYPE_SCROLL { SCROLL_ROW, SCROLL_COL, SCROLL_BOTH, SCROLL_ROTATE, TYPE_SCROLL_END };


	typedef struct tagEffectVoidDesc : public CGameObject::GAMEOBJECT_DESC
	{
		/* �����ؾ� �ϴ� ���� ������ */
		TYPE_EFFECT	eType_Effect = { TYPE_EFFECT_END };
		EASING_TYPE eType_Easing = { EASING_TYPE::LINEAR };


		wstring		strProtoTag = { TEXT("") };
		wstring		strPartTag = { TEXT("") };

		// Texture
		wstring		strModelTag[CVIBuffer_Effect_Model_Instance::MORPH_END] = { TEXT(""), TEXT("") };
		wstring		strTextureTag[TEXTURE_END];
		_int		iTextureIndex[TEXTURE_END]	= { 0 };

		_uint		iCurNumInstance				= { 1000 };

		// Render Group
		_int		iRenderGroup = { 6 };	//! �ۿ��� �������� �����׷��� ��Ʈ�� ����ȯ�ؼ� �������� ����Ʈ�� 6��

		// Shader
		_uint		iShaderPassIndex = { 0 };
		// Shader Variables
		_bool		bBillBoard		= { TRUE };
		_bool		bDissolve		= { TRUE };
		_float2		vUV_Offset		= { 0.f, 0.f };
		_float2		vUV_Scale		= { 1.f, 1.f };

		_float		fUV_RotDegree	= { 0.f };

		_bool		bUV_Wave		= { FALSE };
		_float		fUV_WaveSpeed	= { 1.f };

		_float4		vColor_Offset	= { 0.f, 0.f, 0.f, 0.f };
		_float4		vColor_Clip		= { 0.f, 0.f, 0.f, 0.f };
		_float4		vColor_Mul		= { 1.f, 1.f, 1.f, 1.f };
		MODE_COLOR	eMode_Color		= { MODE_COLOR::MUL };

		// Rim & Bloom
		_float3		vBloomPower = { 0.f, 0.f, 0.f };		// ��� �Ŀ�(rgb ����)
		_float4		vRimColor	= { 1.f, 1.f, 1.f, 1.f };	// �� �÷�
		_float		fRimPower = { 0.f };

		// States
		_bool		bPlay			= { TRUE };
		_bool		bLoop			= { TRUE };
		_bool		bReverse		= { FALSE };
		_bool		bRender			= { FALSE };
		_bool		bUseSpriteAnim	= { FALSE };
		_bool		bUseRigidBody	= { FALSE };

		
		// Times
		_float		fWaitingTime	= { 0.f };	/* �� ���� �Ѿ�� m_fTimeAcc�� �����Ǳ� �����Ѵ�. */
		_float		fLifeTime		= { 5.f };
		_float		fRemainTime		= { 0.f };	/* ������Ÿ���� ������, �� �ð��� �Ѿ�� ����Ʈ ����. */
		_float		fSequenceTime	= { 0.f };	/* �� ������ �ð�(fWaitingTime + fLifeTime + fRemainTime) */


		// ����
		_bool		 bParentPivot	 = { TRUE };


		/* ������Ʈ �Ǹ鼭 �ٲ�� ������(����x) */
		_bool		bActive_Tool = { TRUE };

		// Times 
		_float		fWaitingAcc		= { 0.f };	/* ���� ������ �ð� ���� */
		_float		fTimeAcc		= { 0.f };	/* �ð� ���� */
		_float		fRemainAcc		= { 0.f };
		_float		fSequenceAcc	= { 0.f };	/* ������ �ð� ���� */
		_float		fSpriteTimeAcc	= { 0.f };	/* ��������Ʈ �ð� ���� */

		_float		fLifeTimeRatio	= { 0.f };	/* ������Ÿ���� 0~1�� ������ �� */

		// Values
		_float		fDissolveAmount = { 0.f };

		// ���� 
		// ���� ��ü�� ���ӿ�����Ʈ�� �ִ� �� ���
		_float4x4	 matPivot	= {}; /* XMStoreFloat4x4(&m_matPivot, XMMatrixIdentity()) */
		_float4x4	 matCombined = {};


		void Reset_Times()
		{
			fWaitingAcc		= { 0.f };
			fTimeAcc		= { 0.f };
			fRemainAcc		= { 0.f };
			fSequenceAcc	= { 0.f };
			fSpriteTimeAcc	= { 0.f };

			fLifeTimeRatio	= { 0.f };
		}
		
		void Reset_Desc()
		{
			ZeroMemory(this, sizeof(EFFECTVOID_DESC));
		}

	}EFFECTVOID_DESC;

	typedef struct tagUvSpriteDesc
	{
		_bool	bSpriteFinish   = { FALSE };

		_bool	bLoop = { TRUE };				// ����
		_float	fSequenceTerm	= { 0.05f };	// ����

		_float2 vTextureSize	 = { 1792.f, 1792.f };  // ����
		_float2 vTileSize		 = { 256.f, 256.f };	// ����

		_float2	vUV_CurTileIndex = { 0, 0 }; 
		_float2	vUV_MinTileCount = { 0, 0 }; // ����
		_float2	vUV_MaxTileCount = { 7, 7 }; // ����


		void Reset_Sprite()
		{
			bSpriteFinish = { FALSE };
			vUV_CurTileIndex.y = vUV_MinTileCount.y;
			vUV_CurTileIndex.x = vUV_MinTileCount.x;
		}


	}UVSPRITE_DESC;


	typedef struct tagDistortionDesc
	{
		//_float	fSequenceTerm	= { 0.05f };

		TYPE_SCROLL	eType_Scroll = { TYPE_SCROLL_END };

		_float3 vScrollSpeeds	= { 0.f, 0.f, 0.f };
		_float3 vScales			= { 0.f, 0.f, 0.f };

		_float2 vDistortion1	= { 0.f, 0.f };
		_float2 vDistortion2	= { 0.f, 0.f };
		_float2 vDistortion3	= { 0.f, 0.f };

		_float fDistortionScale = { 0.f };
		_float fDistortionBias	= { 0.f };

	}DISTORTION_DESC;


protected:
	CEffect_Void(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CEffect_Void(const CEffect_Void& rhs);
	virtual ~CEffect_Void() = default;

public:
	virtual HRESULT Initialize_Prototype()				override;
	virtual HRESULT Initialize(void* pArg)				override;
	virtual void	Priority_Tick(_float fTimeDelta)	override;
	virtual void	Tick(_float fTimeDelta)				override;
	virtual void	Late_Tick(_float fTimeDelta)		override;
	virtual HRESULT Render()							override;


public:
	virtual void	Update_PivotMat();


public:
	virtual void	ReSet_Effect();
	virtual void	Init_ReSet_Effect();
	virtual void	End_Effect();


public:
	virtual _bool Write_Json(json& Out_Json)		override;
	virtual void Load_FromJson(const json& In_Json)	override;

	void		 Write_VoidDesc(json& Out_Json);
	void		 Load_VoidDesc(const json& In_Json);


public:
	EFFECTVOID_DESC* Get_Desc() { return &m_tVoidDesc; }

	
public:
	void	ReNumber_PartTag();	// ����


protected:
	EFFECTVOID_DESC	m_tVoidDesc = {};


public:
	virtual CGameObject* Clone(void* pArg)	override = 0;
	virtual CGameObject* Pool()				override = 0;

	virtual void Free() override;
};

END