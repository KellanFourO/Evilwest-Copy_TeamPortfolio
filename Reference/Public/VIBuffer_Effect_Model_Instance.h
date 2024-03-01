#pragma once
#include "VIBuffer_Model_Instance.h"


BEGIN(Engine)
class CMesh;

class ENGINE_DLL CVIBuffer_Effect_Model_Instance : public CVIBuffer_Model_Instance
{
public:
	enum AXIS_TYPE { AXIS_X, AXIS_Y, AXIS_Z, AXIS_END };
	enum FORCE_MODE { FORCE, IMPULSE, ACCELERATION, VELOCITY_CHANGE, MODE_END };
	/*
		FORCE			:	���� ���, �������� ���� ���ϴ� ��� ���
							�ַ� �ٶ��̳� �ڱ�°� ���� ���������� �־����� ��

		IMPULSE			:	���� ���, Ÿ���̳� ���߰� ���� �������� ��

		ACCELERATION	:	���� ����, ���� �������� ���ӷ� �ο� (�ַ� ������ �߷� ǥ��)

		VELOCITY_CHANGE :	���� ����, ���� �������� ���ӷ� �ο�
							ACCELERATION�� �ð��� �귯���鼭 ��ȭ�� ����Ű�µ� ���� �� ���� ���������� ������ �ӵ��� ��ȭ�� ����Ų��.
	*/


	typedef struct tagVIBuffer_EffectModelInstanceDesc
	{
		class CModel*	  pModel	= { nullptr };

		_bool			bUseRigidBody	= { FALSE };
		FORCE_MODE		eForce_Mode		= { IMPULSE };

		_int			iCurNumInstance = { 1 };

		/* For.Position */
		_float4		vCenterPosition		= {0.f, 0.f, 0.f, 1.f};
		_float2		vMinMaxRange		= { 0.1f, 3.f };

		/* For.Rotation */
		_float2		vMinMaxRotationOffsetX	= { 0.0f, 360.f };
		_float2		vMinMaxRotationOffsetY	= { 0.0f, 360.f };
		_float2		vMinMaxRotationOffsetZ	= { 0.0f, 360.f };
		_float3		vRotationOffset			= { 0.f, 0.f, 0.f };

		/* Power */
		_float2		vMinMaxPower = { 0.1f, 250.f };


	}EFFECT_MODEL_INSTANCE_DESC;


	typedef struct tagParticleRigidbodyDesc
	{
		_bool			bKinetic	= { TRUE };  // Ű��ƽ, �� TRUE�� ������ٵ� ���)
		_bool			bUseGravity = { TRUE };

		_bool			bSleep = { FALSE };
		_float			fSleepThreshold = { 0.05f };	// ���� �Ѱ���

		_float			fGravity = { -9.8f };			// �߷� ���ӵ�
		_float3			vAccel;							// ���ӵ�
		_float3			vVelocity;						// �ӵ�

		_float			fMass = { 10.f };				// ����
		_float			fFriction = { 0.1f };			// ���� ���

		_byte			byFreezeAxis = { 0 };			// �� ���� Ȯ�ο� ����Ʈ

	} PARTICLE_RIGIDBODY_DESC;

private:
    CVIBuffer_Effect_Model_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CVIBuffer_Effect_Model_Instance(const CVIBuffer_Effect_Model_Instance& rhs);
    virtual ~CVIBuffer_Effect_Model_Instance() = default;

public:
	virtual _bool Write_Json(json& Out_Json)		override;
	virtual void Load_FromJson(const json& In_Json)	override;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual HRESULT		Bind_VIBuffers(_uint iMeshContainerIndex);
	virtual void		Update(_float fTimeDelta) override;

	void				Init_Instance(_int iNumInstance) override;
	virtual				HRESULT	Render(_int iMeshIndex) override;

public:
	void ReSet();

	/* For.RigidBody */
public:
	_float3 Update_Kinetic(_int iNum, _float fTimeDelta);
	void	Update_Kinematic(_int iNum, _float fTimeDelta);


	void	Add_Force(_int iNum, _fvector vForce, FORCE_MODE eMode);
	void	Clear_Force(_int iNum, const FORCE_MODE& eMode);
	void	Clear_Power(_int iNum);


	const _bool	Check_Sleep(_int iNum);
	void		Sleep(_int iNum) { Clear_Power(iNum); m_vecParticleRigidbodyDesc[iNum].bSleep = TRUE; }
	void		Wake(_int iNum) { m_vecParticleRigidbodyDesc[iNum].bSleep = FALSE; }

	void		Set_FreezeAxis(_int iNum, AXIS_TYPE eAxis) { m_vecParticleRigidbodyDesc[iNum].byFreezeAxis ^= 1 << (_int)eAxis; }
	_bool		Is_FrozeAxis(_int iNum, AXIS_TYPE eAxis) { return m_vecParticleRigidbodyDesc[iNum].byFreezeAxis & 1 << (_int)eAxis; }


public:
	EFFECT_MODEL_INSTANCE_DESC* Get_Desc() { return &m_tBufferDesc; }

private:
	EFFECT_MODEL_INSTANCE_DESC			m_tBufferDesc;
	vector<PARTICLE_RIGIDBODY_DESC>		m_vecParticleRigidbodyDesc;

public:
	static CVIBuffer_Effect_Model_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END