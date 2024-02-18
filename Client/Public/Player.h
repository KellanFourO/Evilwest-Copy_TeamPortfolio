#pragma once

#include "Client_Defines.h"
#include "Character.h"
#include "Event.h"

BEGIN(Engine)
class CCollider;
class CNavigation;
END

BEGIN(Client)

class CPlayer final : public CCharacter, public IEvent
{
private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();


public:
	virtual void Activate() override;
	virtual _bool Activate_Condition() override;
	virtual _bool End_Condition() override;


public:
	/* ������ü�� �����Ѵ�. */
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/* �纻��ü�� �����Ѵ�. */
	virtual CGameObject* Clone(void* pArg) override;

	virtual void Free() override;


};

END