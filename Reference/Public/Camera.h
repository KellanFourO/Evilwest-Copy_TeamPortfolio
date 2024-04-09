#pragma once

/* Ŭ���̾�Ʈ �����ڰ� ��������ϴ� ī�޶���� �������� �θ� �Ǳ����� Ŭ������. */
#include "GameObject.h"

BEGIN(Engine)
class CCollider;

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
	void	Set_Pos(float4x4 vPos);
	void	Update_Cascade();

public:
	void	Set_Enable(_bool _bEnable) { m_bEnable = _bEnable; }
	
public:
	void	Set_Fovy(_float _fFovy) { m_fFovy = _fFovy; }
	_float  Get_Fovy() { return m_fFovy; }
public:
	virtual HRESULT Ready_Components() PURE;

protected:
	_float	m_fFovy			= { 0.0f };
	_float	m_fAspect		= { 0.0f };
	_float	m_fNear			= { 0.0f };
	_float	m_fFar			= { 0.0f };

public:
	/* Cascade */
	HRESULT Ready_CSM();

	const _float3* GetEyePt() const;
	const _float3* GetWorldAhead() const;
	const _float3* GetWorldUp() const;
	const _float3* GetWorldRight() const;
	void ExtractFrustumBoundSphere(_float fNear, _float fFar, _float3& vBoundCenter, _float& fBoundRadius);

	static const int m_iTotalCascades = 3;
	_float4x4 m_arrCameraWorld;

	_float m_fShadowBoundRadius;
	_float m_fCascadeTotalRange;
	_float4x4 m_WorldToShadowSpace;
	_float	m_arrCascadeRanges[4];

	_float m_arrCascadeBoundRadius[m_iTotalCascades];
	_float3 m_arrCascadeBoundCenter[m_iTotalCascades];
	_float4x4 m_arrWorldToCascadeProj[m_iTotalCascades];

	_float				m_fCascadeEnd[4]; // cascade ����ü ���κ�
	_float4x4			m_shadowOrthoProj[3];
	BoundingOrientedBox	m_tCascadeShadowBox[3];

protected:
	CCollider* m_pColliderCom = { nullptr };


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END