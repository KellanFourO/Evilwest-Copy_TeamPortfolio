#pragma once
#include "Bandit_Heavy_State_Knock.h"


BEGIN(Client)

class CBanditHeavy_KnockUp_Down_End : public CBandit_Heavy_State_Knock
{
public:
	virtual void Initialize(CBandit_Heavy* pActor) override;
	virtual CState<CBandit_Heavy>* Update(CBandit_Heavy* pActor, _float fTimeDelta) override;
	virtual void Release(CBandit_Heavy* pActor) override;

public:
	static const _uint g_iAnimIndex = CBandit_Heavy::Bandit_Heavy_State::BanditHeavy_KnockUp_Down_End;

};

END