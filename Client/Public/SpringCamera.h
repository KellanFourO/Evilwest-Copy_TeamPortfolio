#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)

class CCharacter;

END

BEGIN(Client)

class CPlayer;
class CData_Manager;

class CSpringCamera final : public CCamera
{
public:
	typedef struct tagSpringCameraDesc : public tagCameraDesc
	{
		_float		fMouseSensor = 0.0f;
	}SPRING_CAMERA_DESC;

private:
	CSpringCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CSpringCamera(const CSpringCamera& rhs);
	virtual ~CSpringCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;

public:
	virtual HRESULT Ready_Components();

public:
	virtual _bool Write_Json(json& Out_Json) override;

public:
	void CameraRotation(_float fTimeDelta);
	void StartCameraRotation(_fmatrix StartRotationMatrix);
	void Lock_On(_float fTimeDelta);

public:
	void RotatePlayer();
	void Mouse_Fix();
	void Reset_Angle() { 
		//m_fAngle = 0.f; 
		m_fPitch = 0.f;
	};
	_float Get_Pitch() { return m_fPitch; };

public:
	void Set_CameraOffset(_float3 _CameraOffset); /*{ m_CameraOffset = _CameraOffset; }*/
	_float3 Get_CameraOffset() { return m_CameraOffset; }

	void Set_pPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer;} 
	void Set_pTarget(CTransform* _pTarget) { m_ptarget = _pTarget; }
	CTransform* Get_pTarget() { return m_ptarget; }

	void Set_pTargetCharacter(CCharacter* _pCharacter);
	CCharacter* Get_pTargetCharacter() { return m_pCharacter; }

	//ī�޶� Ÿ�� ������ ! 
	void Set_TargetPosition(_float3 _TargetPosition) { m_TargetPosition = _TargetPosition; }
	_float3 Get_TargetPosition() { return m_TargetPosition; }

	void Lerp_CameraPosition(_float fTimeDelta);
	//������ī�޶� ź�� ���� 
	void Set_SpringConstant(_float _SpringConstant) { SpringConstant = _SpringConstant; }
public:
	void Shake_Camera(_float fTimeDelta);
	void Set_ShakeCamera(_bool _bShake) { m_bShake = _bShake; }
	_bool Get_ShakeCamera() { return m_bShake; }

	void Set_ShakeCameraTime(_float _Shaketime) { m_fShakeTime = _Shaketime; }
	void Set_ShakeCameraMinMax(_float2 _fShakeMinMax) { m_fShakeMinMax = _fShakeMinMax; }

private:
	// 	���� , ���� ����Ÿ�
	_float hDist = 0.f;
	_float vDist = 0.f;
	_float SpringConstant = 1000.f; //���ö ���(spring constant)�� Ŀ���� �������� ź���� �پ���.	//���۰��� ī�޶� �����̰� ���� ������ ���� �޶�����.
	_float DampConstant = 0.f;//���ö ����� �������� �� ����(dampening)���
	_float3 Velocity = {};
	_float3 ActualPosition = {};//�ӵ����Ϳ� ī�޶��� ���� ��ġ�� ��Ÿ���� ����
	_float3 PreActualPosition = {};//ī�޶� ������ ���� ���� ������ �����ǰ�����
	_float3 NewTargetPosition = {};
	_float3 CameraTickPos = {};// tick ���� ���� �ڲ� �̻��ϰ� �ʱ�ȭ�Ǽ� �̰ɷ� �ٽ� ���� ��������� 
	CTransform* m_ptarget = { nullptr };//ī�޶� ����ٴ� Ÿ�� ������Ʈ //Ÿ�� ������Ʈ�� ��ġ, ���⺤�� , Ÿ���� ������ ����Ű�� ���͸� ���Ѵ�.
	CPlayer* m_pPlayer = { nullptr };
	CCharacter* m_pCharacter = { nullptr };
	_float	m_fAngle = 0.f;
	_float  m_fPitch = 0.f;
	_matrix cameraMatrix;//���� ī�޶� ���
	//_bool m_bCheck = false;//���콺 ��� ������ 
	//_bool m_bFix = true;//���콺 ��� ������
	_bool m_bPlayerCheck = false; // ī�޶� Ÿ���� �÷��̾� ���� �ƴ���  Ȯ�� �ϱ� �� �� �뵵  
	_bool m_bChangeOffset = false;

	//shake camera
	_bool m_bShake = false;// ī�޶� ��� ������ �ƴѰ����� 
	_float m_fShakeTime = 0.2f;
	_float2 m_fShakeMinMax = { 0.3f ,0.3f};
	//_float m_fTimeDelta = 0.f;

	// !SH Add
	CData_Manager* m_pDataManager = nullptr;
private:
	//ī�޶� X,Y,Z
	_float3 m_CameraOffset = {};
	_float3 m_TargetPosition = {};
	_float CameraMoveSpeed = 1.f;//ī�޶� ������ �̵��ӵ�
	_float CameraMoveSpeedtest = 0.1f;//ī�޶� ������ �̵��ӵ�
private:
	_float				m_fMouseSensor = { 0.0f };
public:
	static CSpringCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual CGameObject* Pool();
	virtual void Free() override;

};

END