#pragma once

#include "Client_Defines.h"
#include "Body.h"

BEGIN(Engine)
END

BEGIN(Client)

class CBody_Mother final : public CBody
{
public:
	enum class RENDER_STATE { ORIGIN, ATTACK, HITTED, NAKED, RENDER_STATE_END };

private:
	CBody_Mother(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CBody_Mother(const CBody_Mother& rhs);
	virtual ~CBody_Mother() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Shadow() override;

public:
	virtual void	OnCollisionEnter(CCollider* other)	override;
	virtual void	OnCollisionStay(CCollider* other)	override;
	virtual void	OnCollisionExit(CCollider* other)	override;

public:
	void SetUp_Animation(_uint iAnimIndex);
	void Set_Dissolve(_bool bOption) { m_bDissolve = bOption; }

public: /* For. RenderState */
	void	Set_RenderState(RENDER_STATE _state) { m_eRender_State = _state; }
private:
	map<CBody_Mother::RENDER_STATE, vector<_int>> m_vDiscardMesh = {};
	RENDER_STATE		m_eRender_State = RENDER_STATE::RENDER_STATE_END;

	_float				m_gCamFar = {};
	_float4				m_vCamPos = {};
	_float				m_fRimPower = {};
	_float4				m_vRimColor = {};
	_float3				m_vBloomPower = {};

private:
	CTexture*	m_pDissolveTexture = { nullptr };
	_bool		m_bDissolve = { false };
	_bool		m_bOutLine = { false };

	/* Dissolve */
	_float		m_fDissolveWeight = 0.f;
	_float		m_fDissolve_feather = 0.f;
	_float3		m_vDissolve_Color = { 0.f, 0.f, 0.f };
	_float		m_fDissolve_Discard = 0.f;
	_float		m_fTimeAcc = 0.f;

	_bool               m_bDeadState = { false };
	_int			iDiscardMeshNumber = 0;
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	virtual void Check_Frustum() override;



public:
	/* ������ü�� �����Ѵ�. */
	static CBody_Mother* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);

	/* �纻��ü�� �����Ѵ�. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual CGameObject* Pool() override;

	virtual void Free() override;

};

END
