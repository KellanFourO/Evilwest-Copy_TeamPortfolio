#pragma once

#include "Client_Defines.h"
#include "Weapon_Player.h"

BEGIN(Client)

class CPlayer_Weapon_Revolver : public CWeapon_Player
{
private:
	CPlayer_Weapon_Revolver(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_Weapon_Revolver(const CPlayer_Weapon_Revolver& rhs);
	virtual ~CPlayer_Weapon_Revolver() = default;

public:
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components();
public:
	static CPlayer_Weapon_Revolver* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END