#pragma once
#include "VIBuffer.h"


BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Particle final : public CVIBuffer
{
public:
	enum TYPE_ACTION { SPARK, BLINK, FALL, RISE, RECYCLE, TYPE_ACTION_END };
	enum TYPE_FADE { FADE_NONE, FADE_OUT, FADE_IN, TYPE_FADE_END };

	typedef struct tagParticleBufferDESC
	{
		// �����ؾ� �ϴ� ���� ������
		_int		iCurNumInstance = { 500 };	// �ʱ�ȭ ���� Max�ν��Ͻ� ������ ��

		TYPE_FADE	eType_Fade = { FADE_NONE };

		/* LifeTime */
		_float2		vMinMaxLifeTime = { 0.1f, 3.f };

		/* RigidBody */
		_bool		bUseRigidBody = { TRUE };
		_bool		bKinetic = { TRUE };	// Ű��ƽ, �� TRUE�� �ӵ� ��� ��)
		_bool		bUseGravity = { TRUE };
		FORCE_MODE	eForce_Mode = { FORCE_MODE::IMPULSE };

		_float		fGravity = { -9.8f };			// �߷� ���ӵ�
		_float		fFriction = { 0.1f };			// ���� ���
		_float		fSleepThreshold = { 0.05f };	// ���� �Ѱ���
		_byte		byFreezeAxis = { 0 };			// �� ���� Ȯ�ο� ����Ʈ

		_float2		vMinMaxPower = { 0.1f, 250.f };
		_float2		vMinMaxMass = { 10.f, 10.f };

		EASING_TYPE	eType_SpeedLerp = { EASING_TYPE::LINEAR };
		_float2		vMinMaxSpeed = { 1.f, 1.f };

		/* For.Position */
		_float2		vMinMaxCenterX = { 0.f, 0.f };
		_float2		vMinMaxCenterY = { 0.f, 0.f };
		_float2		vMinMaxCenterZ = { 0.f, 0.f };

		_float2		vMinMaxRange = { 0.1f, 3.f };


		/* For.Rotation */
		_float2		vMinMaxRotationOffsetX = { 0.0f, 360.f };
		_float2		vMinMaxRotationOffsetY = { 0.0f, 360.f };
		_float2		vMinMaxRotationOffsetZ = { 0.0f, 360.f };


		/* For.Scale */
		EASING_TYPE	eType_ScaleLerp = { EASING_TYPE::LINEAR };
		_float2		vLerpScale_Pos	= { 0.f, 0.5f };		// 0~1�� ������ ������ Ÿ�ӿ��� ��𼭺��� ������ �����Ұ���(Ŀ����, �۾�����)
		_float2		vScaleSpeed		= { 0.005f, 0.05f };	// ũ�� ���� ���ǵ�, ���� ���ǵ�
		_float2		vMinMaxWidth	= { 1.f, 1.f };
		_float2		vMinMaxHeight	= { 1.f, 1.f };


		/* For.Color */
		EASING_TYPE	eType_ColorLerp = { EASING_TYPE::LINEAR };
		_bool		bDynamic_Color	= { TRUE };
		_float2     vMinMaxRed		= { 1.f, 1.f };
		_float2     vMinMaxGreen	= { 1.f, 1.f };
		_float2     vMinMaxBlue		= { 1.f, 1.f };
		_float2     vMinMaxAlpha	= { 1.f, 1.f };


		// ������Ʈ ���鼭 ���ϴ� ������(����X)
		_float		fTimeAcc = { 0.f };			// �ð� ����
		_float		fLifeTimeRatio = { 0.f };	/* ������Ÿ���� 0~1�� ������ �� */

		_float4		vCenterPosition = { 0.f, 0.f, 0.f, 1.f };	// ���� ��ġ

		// ũ��
		_float		fUpScaleTimeAcc = { 0.f };
		_float		fDownScaleTimeAcc = { 0.f };
		_float2		vCurScale		= { 1.f, 1.f };				

		_float4     vCurrentColor	= { 1.f, 1.f, 1.f, 1.f };	// ��


		void Reset_Times()
		{
			fTimeAcc = { 0.f };
			fLifeTimeRatio = { 0.f };

			fUpScaleTimeAcc = { 0.f };
			fDownScaleTimeAcc = { 0.f };
		}

		void Reset_Desc()
		{
			ZeroMemory(this, sizeof(PARTICLE_BUFFER_DESC));
		}


	}PARTICLE_BUFFER_DESC;

	typedef struct tagParticleDesc
	{
		// ������Ʈ ���鼭 ���ϴ� ������(����X)

		// �ð�
		_float	fTimeAccs = { 0.f };
		_float	fLifeTime = { 1.f };
		_float  fLifeTimeRatios = { 0.f };	/* ������Ÿ���� 0~1�� ������ �� */

		// ũ��
		_float2	vCurScales	= { 1.f, 1.f };
		_float2	vMaxScales = { 1.f, 1.f };

		// ��
		_float4 vCurrentColors = { 1.f, 1.f, 1.f, 1.f };

	} PARTICLE_INFO_DESC;

	typedef struct tagParticleShaderDesc
	{
		// 16 ���������
		
		// ������Ʈ ���鼭 ���ϴ� ������(����X)
		_float3	vDir	= { 1.f, 0.f, 0.f };
		_float	Padding = { 0.f };


	} PARTICLE_SHADER_INFO_DESC;

	typedef struct tagParticleRigidbodyDesc
	{
		// ������Ʈ ���鼭 ���ϴ� ������(����X)
		_bool			bSleep = { FALSE };

		_float3			vAccel = {0.f, 0.f, 0.f};		// ���ӵ�
		_float3			vVelocity = { 0.f, 0.f, 0.f };	// �ӵ�

		_float			fMaxSpeed = { 1.f };
		_float			fCurSpeed = { 1.f };
		_float			fMass = { 10.f };				// ����

	} PARTICLE_RIGIDBODY_DESC;

private:
	CVIBuffer_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Particle(const CVIBuffer_Particle& rhs);
	virtual ~CVIBuffer_Particle() = default;

public:
	virtual _bool Write_Json(json & Out_Json)		override;
	virtual void Load_FromJson(const json & In_Json)	override;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual HRESULT		Bind_VIBuffers();
	void				Update(_float fTimeDelta);

	HRESULT				Init_Instance(_int iNumInstance);
	HRESULT				Render();

public:
	void ReSet();
	void ReSet_Info(_uint iNum);

	/* For.RigidBody */
public:
	_float3 Update_Kinetic(_uint iNum, _float fTimeDelta);
	void	Update_Kinematic(_uint iNum);


	void	Add_Force(_uint iNum, _fvector vForce, FORCE_MODE eMode);
	void	Clear_Force(_uint iNum, const FORCE_MODE & eMode);
	void	Clear_Power(_uint iNum);


	const _bool	Check_Sleep(_uint iNum);
	void		Sleep(_uint iNum) { Clear_Power(iNum); m_vecParticleRigidbodyDesc[iNum].bSleep = TRUE; }
	void		Wake(_uint iNum) { m_vecParticleRigidbodyDesc[iNum].bSleep = FALSE; }

	void		Set_FreezeAxis(AXIS eAxis) { m_tBufferDesc.byFreezeAxis ^= 1 << (_int)eAxis; }
	_bool		Is_FrozeAxis(AXIS eAxis) { return m_tBufferDesc.byFreezeAxis & 1 << (_int)eAxis; }


public:
	PARTICLE_BUFFER_DESC* Get_Desc() { return &m_tBufferDesc; }
	vector<PARTICLE_SHADER_INFO_DESC>& Get_ParticleShaderInfoDescs() { return m_vecParticleShaderInfoDesc; }

private:
	PARTICLE_BUFFER_DESC				m_tBufferDesc;
	vector<PARTICLE_INFO_DESC>			m_vecParticleInfoDesc;
	vector<PARTICLE_SHADER_INFO_DESC>	m_vecParticleShaderInfoDesc;
	vector<PARTICLE_RIGIDBODY_DESC>		m_vecParticleRigidbodyDesc;


	/* �ν��Ͻ� */
private:
	ID3D11Buffer* m_pVBInstance = { nullptr };
	_uint				m_iInstanceStride = { 0 };
	_uint				m_iNumInstance = { 0 };
	_uint				m_iIndexCountPerInstance = { 0 };

public:
	static CVIBuffer_Particle* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END