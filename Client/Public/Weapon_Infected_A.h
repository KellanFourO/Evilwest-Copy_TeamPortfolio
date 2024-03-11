#pragma once

#include "Weapon_Infected.h"

BEGIN(Engine)
END

/* --- Memo --- */
// ���� ���� �ִ°� �ƴ϶� ������ �ݶ��̴����̹Ƿ� Render_Shadow�� �ʿ����. 
/* ------------ */
BEGIN(Client)

class CWeapon_Infected_A : public CWeapon_Infected
{
protected:
	CWeapon_Infected_A(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CWeapon_Infected_A(const CWeapon_Infected_A& rhs);
	virtual ~CWeapon_Infected_A() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Tick(_float fTimeDelta) override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual void	Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual	HRESULT Render_Shadow() override { return S_OK; };

protected:
	virtual HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CWeapon_Infected_A* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual CGameObject* Pool();
	virtual void Free() override;
};

END