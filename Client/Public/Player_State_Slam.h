#pragma once
#include "Player_State_Attack.h"

BEGIN(Client)

class CPlayer_State_Slam : public CPlayer_State_Attack
{
public:
	virtual void Initialize(CPlayer* pActor) override;
	virtual CState<CPlayer>* Update(CPlayer* pActor, _float fTimeDelta) override;
	virtual void Release(CPlayer* pActor) override;

public:
	virtual CState<CPlayer>* Update_State(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex) override;


};

END