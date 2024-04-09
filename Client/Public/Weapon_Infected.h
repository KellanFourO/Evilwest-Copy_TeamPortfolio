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


public:
	void	Play_Trail(_bool bPlay);

protected:
	class CEffect_Trail* m_pTrail = { nullptr };	//! ���� : Ʈ���� �߰�

protected:
	virtual HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	virtual void	OnCollisionEnter(CCollider* other)	override;
	virtual void	OnCollisionStay(CCollider* other)	override;
	virtual void	OnCollisionExit(CCollider* other)	override;



public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual CGameObject* Pool() PURE;
	virtual void Free() override;
};

END