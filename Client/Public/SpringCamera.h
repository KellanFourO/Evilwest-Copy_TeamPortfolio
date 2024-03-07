#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
END

BEGIN(Client)

class CPlayer;

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
	virtual _bool Write_Json(json& Out_Json) override;

public:
	void CameraRotation(_float fTimeDelta);
	void Lock_On(_float fTimeDelta);

public:
	void RotatePlayer();
	void Mouse_Fix();
public:
	void Set_CameraOffset(_float3 _CameraOffset) { m_CameraOffset = _CameraOffset; }
	_float3 Get_CameraOffset() { return m_CameraOffset; }

	void Set_pTarget(CTransform* _pTarget) { m_ptarget = _pTarget; }
	CTransform* Get_pTarget() { return m_ptarget; }

private:
	// 	���� , ���� ����Ÿ�
	_float hDist = 0.f;
	_float vDist = 0.f;
	_float SpringConstant = 100.f; //���ö ���(spring constant)�� Ŀ���� �������� ź���� �پ���.	//���۰��� ī�޶� �����̰� ���� ������ ���� �޶�����.
	_float DampConstant = 0.f;//���ö ����� �������� �� ����(dampening)���
	_float3 Velocity = {};
	_float3 ActualPosition = {};//�ӵ����Ϳ� ī�޶��� ���� ��ġ�� ��Ÿ���� ����
	_float3 PreActualPosition = {};//ī�޶� ������ ���� ���� ������ �����ǰ�����
	_float3 CameraTickPos = {};// tick ���� ���� �ڲ� �̻��ϰ� �ʱ�ȭ�Ǽ� �̰ɷ� �ٽ� ���� ��������� 
	CTransform* m_ptarget = { nullptr };//ī�޶� ����ٴ� Ÿ�� ������Ʈ //Ÿ�� ������Ʈ�� ��ġ, ���⺤�� , Ÿ���� ������ ����Ű�� ���͸� ���Ѵ�.
	CPlayer* m_pPlayer = { nullptr };
	_float	m_fAngle = 0.f;
	_float  m_fPitch = 0.f;
	_matrix cameraMatrix;//���� ī�޶� ���
	_bool m_bCheck = false;//���콺 ��� ������ 
	_bool m_bFix = true;//���콺 ��� ������ 
private:
	//ī�޶� X,Y,Z
// 	_float m_CameraOffsetX = 0.f;
// 	_float m_CameraOffsetY = 0.f;
// 	_float m_CameraOffsetZ = 0.f;
	_float3 m_CameraOffset = {};
	_float CameraMoveSpeed = 3.f;//ī�޶� ������ �̵��ӵ�
private:
	_float				m_fMouseSensor = { 0.0f };
public:
	static CSpringCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual CGameObject* Pool();
	virtual void Free() override;

};

END