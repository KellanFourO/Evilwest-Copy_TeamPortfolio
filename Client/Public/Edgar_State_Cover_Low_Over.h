#pragma once
#include "Edgar_State_Cover_Low.h"

BEGIN(Client)

class CEdgar_State_Cover_Low_Over : public CEdgar_State_Cover_Low
{
public:
	virtual void Initialize(CEdgar* pActor) override;
	virtual CState<CEdgar>* Update(CEdgar* pActor, _float fTimeDelta) override;
	virtual void Release(CEdgar* pActor) override;

public:
	virtual CState<CEdgar>* Update_State(CEdgar* pActor, _float fTimeDelta, _uint _iAnimIndex) override;


};

END