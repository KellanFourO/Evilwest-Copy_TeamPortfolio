#pragma once

/* Ŭ���̾�Ʈ �����ڰ� ��������ϴ� ī�޶���� �������� �θ� �Ǳ����� Ŭ������. */
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc : public tagGameObjectDesc
	{
		_float4		vEye, vAt;
		_float		fFovy, fAspect, fNear, fFar;
	}CAMERA_DESC;

protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;	

public:
	void	Update_Cascade();

public:
	void	Set_Enable(_bool _bEnable) { m_bEnable = _bEnable; }


protected:
	_float	m_fFovy			= { 0.f };
	_float	m_fAspect		= { 0.0f };
	_float	m_fNear			= { 0.0f };
	_float	m_fFar			= { 0.0f };

	_bool	m_bEnable = false; //! true�� ��쿡�� �� ���� �����ϰ�~ ���¿�


	_float						m_fCascadeEnd[4]; // ��ٸ��� ����ü ���κ� 
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END