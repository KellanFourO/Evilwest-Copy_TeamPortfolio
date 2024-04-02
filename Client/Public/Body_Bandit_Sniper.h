#pragma once

#include "Client_Defines.h"
#include "Body.h"

BEGIN(Engine)
END

BEGIN(Client)

class CBody_Bandit_Sniper final : public CBody
{
private:
	CBody_Bandit_Sniper(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CBody_Bandit_Sniper(const CBody_Bandit_Sniper& rhs);
	virtual ~CBody_Bandit_Sniper() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Tick(_float fTimeDelta) override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual void	Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Shadow() override;
	virtual HRESULT Render_CSM() override;

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

private:
	CTexture*	m_pDissolveTexture = { nullptr };
	_bool		m_bDissolve = { false };

	/* Dissolve */
	_float		m_fDissolveWeight = 0.f;
	_float		m_fDissolve_feather = 0.f;
	_float3		m_vDissolve_Color = { 0.f, 0.f, 0.f };
	_float		m_fDissolve_Discard = 0.f;

public:
	static CBody_Bandit_Sniper* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual CGameObject* Pool() override;
	virtual void Free() override;
};

END

/* 
Mesh �ε��� ����
0 : ������ ����
1 : ����
2 :
3 : ��� ���̺�
4 :
5 : ����
6 : ���ִ� �Ƕ�
7 : ��ִ� �Ƕ�
8 : ��� �����Ƕ�
9 : ��� ������ �Ƕ�
10 : ��

*/