#pragma once
#include "Player_State_Death.h"


BEGIN(Client)

class CPlayer_DeathLight_F_01 : public CPlayer_State_Death
{
public:
	virtual void Initialize(CPlayer* pActor) override;
	virtual CState<CPlayer>* Update(CPlayer* pActor, _float fTimeDelta) override;
	virtual void Release(CPlayer* pActor) override;

public:
	static const _uint g_iAnimIndex = CPlayer::Player_State::Player_DeathLight_F_01;

};

END