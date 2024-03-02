#pragma once

#include "Client_Defines.h"
#include "Effect_Void.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Particle_Point;
END

BEGIN(Client)

class CEffect_Particle final : public CEffect_Void
{
public:
	typedef struct tagParticleDesc : public CEffect_Void::EFFECTVOID_DESC
	{
		CVIBuffer_Particle_Point::TYPE_ACTION	eType_Action = { CVIBuffer_Particle_Point::SPHERE };
		CVIBuffer_Particle_Point::TYPE_FADE		eType_Fade = { CVIBuffer_Particle_Point::FADE_NONE };



#pragma region ���� Desc�ʱ�ȭ�� ���� �� (����X)
		/* LifeTime */
		_float2		vMinMaxLifeTime = { 0.1f, 3.f };

		/* RigidBody */
		_bool		bUseRigidBody = { TRUE };
		_bool		bKinetic	  = { TRUE };	// Ű��ƽ, �� TRUE�� �ӵ� ��� ��)
		_bool		bUseGravity   = { TRUE };
		FORCE_MODE	eForce_Mode   = { FORCE_MODE::IMPULSE };

		_float		fGravity		= { -9.8f };	// �߷� ���ӵ�
		_float		fFriction		= { 0.1f };		// ���� ���
		_float		fSleepThreshold = { 0.05f };	// ���� �Ѱ���
		_byte		byFreezeAxis	= { 0 };		// �� ���� Ȯ�ο� ����Ʈ

		_float2		vMinMaxPower = { 0.1f, 250.f };


		/* For.Position */
		_float4		vCenterPosition = { 0.f, 0.f, 0.f, 1.f };
		_float2		vMinMaxRange	= { 0.1f, 3.f };


		/* For.Rotation */
		_float2		vMinMaxRotationOffsetX = { 0.0f, 360.f };
		_float2		vMinMaxRotationOffsetY = { 0.0f, 360.f };
		_float2		vMinMaxRotationOffsetZ = { 0.0f, 360.f };


		/* For.Color */
		EASING_TYPE	eType_ColorLerp = { EASING_TYPE::LINEAR };
		_bool		bDynamic_Color	= { TRUE };
		_float2     vMinMaxRed		= { 1.f, 1.f };
		_float2     vMinMaxGreen	= { 1.f, 1.f };
		_float2     vMinMaxBlue		= { 1.f, 1.f };
		_float2     vMinMaxAlpha	= { 1.f, 1.f };
#pragma endregion


	}PARTICLE_DESC;


private:
	CEffect_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CEffect_Particle(const CEffect_Particle& rhs);
	virtual ~CEffect_Particle() = default;

public:
	virtual HRESULT Initialize_Prototype()				override;
	virtual HRESULT Initialize(void* pArg)				override;
	virtual void	Priority_Tick(_float fTimeDelta)	override;
	virtual void	Tick(_float fTimeDelta)				override;
	virtual void	Late_Tick(_float fTimeDelta)		override;
	virtual HRESULT Render()							override;

public:
	virtual void	ReSet_Effect()	override;
	virtual void	End_Effect()	override;

public:
	virtual _bool Write_Json(json& Out_Json)		 override;
	virtual void  Load_FromJson(const json& In_Json) override;

	void			Set_SortZ(_bool bSortz) { m_bSortZ = bSortz; }
	_bool			Get_SortZ() { return m_bSortZ; }

	/* For.Desc */
public:
	PARTICLE_DESC*	Get_Desc() { return &m_tParticleDesc; }
	UVSPRITE_DESC*	Get_Sprite_Desc() { return &m_tSpriteDesc; }
	void* Get_BufferDesc();

public:
	//CVIBuffer_Particle_Point* Get_VIBufferCom() { return m_pVIBufferCom; }
	CVIBuffer_Particle* Get_VIBufferCom() { return m_pVIBufferCom; }
	CTexture* Get_TextureCom(TEXTURE eTexture) { return m_pTextureCom[eTexture]; }

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

private:
	PARTICLE_DESC				m_tParticleDesc = {};
	UVSPRITE_DESC				m_tSpriteDesc = {};
	//CVIBuffer_Particle_Point::PARTICLE_POINT_DESC 

	//CModel*							 m_pModelCom = { nullptr };
	//CVIBuffer_Effect_Model_Instance* m_pVIBufferCom_Model = { nullptr };


private:
	CShader*					m_pShaderCom = { nullptr };
	CTexture*					m_pTextureCom[TEXTURE_END] = { nullptr };
	//CVIBuffer_Particle_Point*	m_pVIBufferCom = { nullptr };
	CVIBuffer_Particle* m_pVIBufferCom = { nullptr };

	_bool						m_bSortZ = { FALSE };

public:
	/* ������ü�� �����Ѵ�. */
	static CEffect_Particle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);

	/* �纻��ü�� �����Ѵ�. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual CGameObject* Pool() override;

	virtual void Free() override;
};

END