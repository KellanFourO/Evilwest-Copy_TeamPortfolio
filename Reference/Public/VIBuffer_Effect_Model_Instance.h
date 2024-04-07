#pragma once
#include "VIBuffer_Model_Instance.h"


BEGIN(Engine)
class CMesh;
class CModel;

class ENGINE_DLL CVIBuffer_Effect_Model_Instance : public CVIBuffer_Model_Instance
{
public:
	enum TYPE_MODE		{ MODE_STATIC, MODE_PARTICLE, MODE_END };
	enum TYPE_ACTION	{ SPARK, BLINK, FALL, RISE, TORNADO, TYPE_ACTION_END };
	//enum MODEL_MORPH	{ MORPH_01, MORPH_02, MORPH_END };

	enum TYPE_FADE			{ FADE_NONE, FADE_OUT, FADE_IN, TYPE_FADE_END };
	enum TYPE_FADE_TAKES	{ LIFE, DIST, HEIGHT, SCALE, TYPE_FADE_TAKES_END };
	enum TYPE_DIRAXIS		{ DIR_RIGHT, DIR_UP, DIR_LOOK, TYPE_DIRAXIS_END };

	typedef struct tagVIBuffer_EffectModelInstanceDesc
	{
		// �����ؾ� �ϴ� ���� ������
		_int			iCurNumInstance		= { 1000 };		// �ʱ�ȭ ���� �ִ� ������ ��	

		CModel* pModel		= { nullptr };
		//CModel*			pModel[MORPH_END]	= { nullptr };	// ���� X
		//MODEL_MORPH		eCurModelNum		= { MORPH_01 };	// ���� X


		/* States */
		TYPE_MODE	eType_Mode	 = { MODE_PARTICLE };	// ��ƼŬ�� ����Ұ���

		_bool		bRecycle = { TRUE };				// ���ڸ� ���� �Ұ���
		_bool		bReverse = { FALSE };				// ������� ������
		TYPE_ACTION eType_Action = { SPARK };			// �����̴� ���


		/* Times */
		_float2		vMinMaxLifeTime = { 0.1f, 3.f };


		/* Emitter */
		_float		fEmissionTime = { 0.f };		// ���� O
		_uint		iAddEmitCount = { 0 };			// ���� O	�ѹ� ���� �� �� ��� �߰��� �����Ұ���
		_float		fEmissionTimeAcc = { 0.f };		// ���� X
		_uint		iEmitCount = { 0 };				// ���� X
		_bool		bEmitFinished = { FALSE };		// ���� X


		/* Morph */
		_bool		bMorph			= { FALSE };
		_float		fMorphTimeTerm  = { 0.05f };


		/* RigidBody */
		_bool		bUseRigidBody	= { TRUE };
		_bool		bKinetic		= { TRUE };		// Ű��ƽ, �� TRUE�� �ӵ� ��� ��)
		_bool		bUseGravity		= { TRUE };
		FORCE_MODE	eForce_Mode		= { FORCE_MODE::IMPULSE };

		_float		fGravity		= { -9.8f };	// �߷� ���ӵ�

		_float2		vMinMaxFriction = { 0.1f, 0.1f };		// ���� ��� ����

		_float		fSleepThreshold = { 0.05f };			// ���� �Ѱ���
		_byte		byFreezeAxis	= { 0 };				// �� ���� Ȯ�ο� ����Ʈ

		_float2		vMinMaxPower = { 0.1f, 250.f };
		_float2		vMinMaxMass = { 10.f, 10.f };

		EASING_TYPE	eType_SpeedLerp		= { EASING_TYPE::LINEAR };
		_float2		vMinMaxSpeed		= { 1.f, 1.f };
		_float2		vMinMaxTornadoSpeed = { 1.f, 1.f };


		/* For.Position */
		_float3		vMinCenterOffsetPos = { 0.f, 0.f, 0.f };
		_float3		vMaxCenterOffsetPos = { 0.f, 0.f, 0.f };

		_float2		vMinMaxRange		= { 0.1f, 3.f };
		_float2		vMinMaxAddRange		= { 0.f, 0.f };

		_float2		vMinMaxPosY = { 0.1f, 3.f };	// ��ƼŬ�� �ö� �ְ� ����
		_float2		vMinMaxTheta = { 0.f, 360.f };	// 3.14f * 2.f


		/* For.Rotation */
		TYPE_DIRAXIS eType_Dir = { DIR_LOOK };

		_bool		bRotAcc = { FALSE };

		//_float3		vRadian = { 0.f, 0.f, 0.f };
		_float3		vMinRadian = { 0.f, 0.f, 0.f };
		_float3		vMaxRadian = { 0.f, 0.f, 0.f };

		_float2		vMinMaxRadianSpeed_X = { 0.f, 0.f };
		_float2		vMinMaxRadianSpeed_Y = { 0.f, 0.f };
		_float2		vMinMaxRadianSpeed_Z = { 0.f, 0.f };

		_float2		vMinMaxRotationOffsetX = { 0.0f, 360.f };
		_float2		vMinMaxRotationOffsetY = { 0.0f, 360.f };
		_float2		vMinMaxRotationOffsetZ = { 0.0f, 360.f };


		/* For.Scale */
		_bool		bScaleRatio = { TRUE };						// ũ�� ������
		_bool		bUseScaleLerp = { TRUE };
		EASING_TYPE	eType_ScaleLerp = { EASING_TYPE::LINEAR };
		_float2		vScaleLerp_Up_Pos = { 0.f, 0.3f };			// 0~1�� ������ ������ Ÿ�ӿ��� ��𼭺��� ������ �����ϰ�, ��������(Ŀ���� ��)
		_float2		vScaleLerp_Down_Pos = { 1.f, 1.f };			// 0~1�� ������ ������ Ÿ�ӿ��� ��𼭺��� ������ �����ϰ�, ��������(�۾����� ��)
		_float3		vStartScale			= { 1.f, 1.f, 1.f };
		_float3		vEndScale			= { 1.f, 1.f, 1.f };


		/* For.Color */
		EASING_TYPE	eType_ColorLerp = { EASING_TYPE::LINEAR };
		_bool		bDynamic_Color	= { TRUE };
		_float2     vMinMaxRed		= { 1.f, 1.f };
		_float2     vMinMaxGreen	= { 1.f, 1.f };
		_float2     vMinMaxBlue		= { 1.f, 1.f };
		_float2     vMinMaxAlpha	= { 1.f, 1.f };
		_float4     vCurrentColor = { 1.f, 1.f, 1.f, 1.f };	// �� (���� X)


		TYPE_FADE		eType_Fade = { FADE_NONE };
		TYPE_FADE_TAKES	eType_Fade_Takes = { TYPE_FADE_TAKES_END };


		// ������Ʈ ���鼭 ���ϴ� ������(����X)
		_float		fTimeAcc = { 0.f };			// �ð� ����
		_float		fLifeTimeRatio = { 0.f };	/* ������Ÿ���� 0~1�� ������ �� */

		_float		fMorphTimeAcc = { 0.f };


		void Reset_Times()
		{
			fTimeAcc = { 0.f };
			fLifeTimeRatio = { 0.f };
		}

		void Reset_Desc()
		{
			ZeroMemory(this, sizeof(EFFECT_MODEL_INSTANCE_DESC));
		}

	}EFFECT_MODEL_INSTANCE_DESC;


	typedef struct tagParticleDesc
	{
		// ������Ʈ ���鼭 ���ϴ� ������(����X)
		_bool bEmit = { FALSE };
		_bool bDie = { FALSE };

		// �ð�
		_float	fTimeAccs = { 0.f };
		_float	fLifeTime = { 1.f };
		_float  fLifeTimeRatios = { 0.f };	/* ������Ÿ���� 0~1�� ������ �� */


		// ��ġ
		_float4	vCenterPositions = { 0.f, 0.f, 0.f, 1.f };
		_float	fMaxRange = { 3.f };
		_float	fAddRange = { 0.f };

		_float	fMaxPosY = { 3.f };


		//  ���� 
		_float3	vDir = { 1.f, 0.f, 0.f };


		_float3  vOffsetTheta = { 0.f, 0.f, 0.f };

		// ���ǵ�
		_float	fCurSpeed			= { 1.f };
		_float  fCurTheta			= { 0.f };
		_float	fCurTornadoSpeed	= { 1.f };

		// ȸ��
		_float3	vRadian				= { 0.f, 0.f, 0.f };
		_float3	vCurRadian			= { 0.f, 0.f, 0.f };
		_float3	vAddRadianSpeed		= { 0.f, 0.f, 0.f };

		// ũ��
		_float	fUpScaleTimeAccs = { 0.f };
		_float	fDownScaleTimeAccs = { 0.f };

		_float3	vCurScales = { 1.f, 1.f, 1.f };
		_float3	vMaxScales = { 1.f, 1.f, 1.f };


		// �� (����)
		_float  fAddAlpha = { 1.f };

		void Reset_ParticleTimes()
		{
			fTimeAccs = { 0.f };
			fLifeTimeRatios = { 0.f };

			fUpScaleTimeAccs = { 0.f };
			fDownScaleTimeAccs = { 0.f };
		}

	} PARTICLE_INFO_DESC;

	typedef struct tagParticleShaderDesc
	{
		// 16 ��������� (64)

		// ������Ʈ ���鼭 ���ϴ� ������(����X)
		_float4 vCurrentColors = { 1.f, 1.f, 1.f, 1.f }; // 16

		_float3 vRight = { 1.f, 0.f, 0.f };			// 12
		_float  fCurAddAlpha = { 1.f };				// 4	

		_float3 vUp = { 0.f, 1.f, 0.f };			// 12
		_float  fPadding2 = { 0.f };				// 4	

		_float3 vLook = { 0.f, 0.f, 1.f };			//12
		_float  fPadding3 = { 0.f };				// 4	

	} PARTICLE_SHADER_INFO_DESC;

	typedef struct tagParticleRigidbodyDesc
	{
		// ������Ʈ ���鼭 ���ϴ� ������(����X)
		_bool			bForced = { FALSE };
		_bool			bSleep = { FALSE };

		_float3			vAccel = { 0.f, 0.f, 0.f };		// ���ӵ�
		_float3			vVelocity = { 0.f, 0.f, 0.f };	// �ӵ�

		_float			fMass	  = { 10.f };				// ����
		_float			fFriction = { 0.1f };				// ���� ���

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
	virtual void		Update_Particle(_float fTimeDelta);

	void				Init_Instance(_int iNumInstance) override;
	virtual	HRESULT		Render(_int iMeshIndex) override;


public:
	HRESULT			Change_Model(CModel* pChangeModel);	// ����

public:
	void ReSet();
	void ReSet_ParticleInfo(_uint iNum);


public:
	_float4 Make_Dir(_uint iNum);
	void	Rotation_Instance(_uint iNum);

	void	Update_DirToAxis(_uint iNum);
	void	Make_DirToRight(_uint iNum);
	void	Make_DirToUp(_uint iNum);
	void	Make_DirToLook(_uint iNum);
	//void	Update_Spark_Rotation(_uint iNum);


	/* For.RigidBody */
public:
	_float3 Update_Kinetic(_uint iNum, _float fTimeDelta);
	void	Update_Kinematic(_uint iNum);


	void	Add_Force(_uint iNum, _fvector vForce, FORCE_MODE eMode);
	void	Clear_Force(_uint iNum, const FORCE_MODE& eMode);
	void	Clear_Power(_uint iNum);


	const _bool	Check_Sleep(_uint iNum, const FORCE_MODE& eMode);
	void		Sleep(_uint iNum) { Clear_Power(iNum); m_vecParticleRigidbodyDesc[iNum].bSleep = TRUE; }
	void		Wake(_uint iNum) { m_vecParticleRigidbodyDesc[iNum].bSleep = FALSE; }

	void		Set_FreezeAxis(_uint iNum, AXIS eAxis) { m_tBufferDesc.byFreezeAxis ^= 1 << (_int)eAxis; }
	_bool		Is_FrozeAxis(_uint iNum, AXIS eAxis) { return m_tBufferDesc.byFreezeAxis & 1 << (_int)eAxis; }


public:
	EFFECT_MODEL_INSTANCE_DESC* Get_Desc() { return &m_tBufferDesc; }
	vector<PARTICLE_SHADER_INFO_DESC>& Get_ParticleShaderInfoDescs() { return m_vecParticleShaderInfoDesc; }

private:
	EFFECT_MODEL_INSTANCE_DESC			m_tBufferDesc;
	vector<PARTICLE_INFO_DESC>			m_vecParticleInfoDesc;
	vector<PARTICLE_SHADER_INFO_DESC>	m_vecParticleShaderInfoDesc;
	vector<PARTICLE_RIGIDBODY_DESC>		m_vecParticleRigidbodyDesc;



	//vector<CMesh*>		m_vecInstanceMeshs[MORPH_END] = {};

public:
	static CVIBuffer_Effect_Model_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END