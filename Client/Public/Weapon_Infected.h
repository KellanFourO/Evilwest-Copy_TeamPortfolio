#pragma once

#include "Client_Defines.h"
#include "Weapon.h"

BEGIN(Engine)
END

/* --- Memo --- */
// ���� ���� �ִ°� �ƴ϶� ������ �ݶ��̴����̹Ƿ� Render_Shadow�� �ʿ����. 
// A, B, C, = ��ü ���� ���� 
// D = ����ü�� ���� 
// E = ����
/* ------------ */

BEGIN(Client)

class CWeapon_Infected abstract : public CWeapon
{
protected:
	CWeapon_Infected(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CWeapon_Infected(const CWeapon_Infected& rhs);
	virtual ~CWeapon_Infected() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Tick(_float fTimeDelta) override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual void	Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual CGameObject* Pool() PURE;
	virtual void Free() override;
};

END