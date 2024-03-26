#pragma once
#include "Bandit_Sniper_State_Hit.h"

BEGIN(Client)
class CSniper_HitHeavy_FL : public CBandit_Sniper_State_Hit
{
public:
	virtual void Initialize(CBandit_Sniper* pActor) override;
	virtual CState<CBandit_Sniper>* Update(CBandit_Sniper* pActor, _float fTimeDelta) override;
	virtual void Release(CBandit_Sniper* pActor) override;

public:
	static const _uint g_iAnimIndex = CBandit_Sniper::Bandit_Sniper_State::Bandit_HitHeavy_FL_01_NEW;

};

END
