#pragma once

#include "Client_Defines.h"
#include "Infected.h"
#include "Actor.h"

BEGIN(Client)

class CInfected_A  : public CInfected
{
protected:
	CInfected_A(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CInfected_A(const CInfected_A& rhs);
	virtual ~CInfected_A() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


protected:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();


private:
	CActor<CInfected>* m_pActor = { nullptr };

public:
	static CInfected_A* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual CGameObject* Pool() override;
	virtual void Free() override;

};

END



