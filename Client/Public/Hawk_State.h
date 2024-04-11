#pragma once
#include "State.h"
#include "Edgar.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CHawk_State abstract : public CState<CEdgar>
{
public:
	virtual void Initialize(CEdgar* pActor) override;
	virtual CState<CEdgar>* Update(CEdgar* pActor, _float fTimeDelta) override;
	virtual void Release(CEdgar* pActor) override;

public:
	virtual CState<CEdgar>* Update_State(CEdgar* pActor, _float fTimeDelta, _uint _iAnimIndex) PURE;


public:		//���� �׷� �� ������Ʈ 
	CState<CEdgar>* Normal_State(CEdgar* pActor, _float fTimeDelta, _uint _iAnimIndex);


public:		//���� ���� ���� ����
	CState<CEdgar>* Normal(CEdgar* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CEdgar>* Walk(CEdgar* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CEdgar>* Run(CEdgar* pActor, _float fTimeDelta, _uint _iAnimIndex);


protected:
	CGameInstance* m_pGameInstance = { nullptr };
};

END