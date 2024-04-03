#pragma once

#include "Client_Defines.h"
#include "Weapon_Player.h"

BEGIN(Client)

class CPlayer_Weapon_Revolver : public CWeapon_Player
{
private:
	CPlayer_Weapon_Revolver(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CPlayer_Weapon_Revolver(const CPlayer_Weapon_Revolver& rhs);
	virtual ~CPlayer_Weapon_Revolver() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Fire(_float3 vTargetPos = _float3(0.f, 0.f, 1.f), CCharacter* pTarget = nullptr);

protected:
	virtual HRESULT Ready_Components();
public:
	static CPlayer_Weapon_Revolver* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual CGameObject* Pool();
	virtual void Free() override;
};

END