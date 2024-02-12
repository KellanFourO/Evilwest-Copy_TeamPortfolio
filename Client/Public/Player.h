#pragma once

#include "Client_Defines.h"
#include "LandObject.h"

BEGIN(Engine)
class CCollider;
class CNavigation;
END

BEGIN(Client)

class CPlayer final : public CLandObject
{
public:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual shared_ptr<CComponent> Find_Component(const wstring & strComTag, const wstring & strPartTag = TEXT("")) override;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	shared_ptr<CGameObject> Find_PartObject(const wstring& strPartTag);

private:
	shared_ptr<CNavigation>		m_pNavigationCom = { nullptr };
	shared_ptr<CCollider>		m_pColliderCom = { nullptr };

private:
	map<const wstring, shared_ptr<CGameObject>>		m_PartObjects;

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();
	HRESULT Add_PartObject(const wstring& strPrototypeTag, const wstring& strPartTag, void* pArg);



public:
	/* ������ü�� �����Ѵ�. */
	static shared_ptr<CPlayer> Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/* �纻��ü�� �����Ѵ�. */
	virtual shared_ptr<CGameObject> Clone(void* pArg) override;

	virtual void Free() override;
};

END