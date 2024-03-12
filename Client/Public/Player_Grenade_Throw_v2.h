#pragma once
#include "Player_State_FlameBelcher.h"


BEGIN(Client)

class CPlayer_Grenade_Throw_v2 : public CPlayer_State_FlameBelcher
{
public:
	virtual void Initialize(CPlayer* pActor) override;
	virtual CState<CPlayer>* Update(CPlayer* pActor, _float fTimeDelta) override;
	virtual void Release(CPlayer* pActor) override;

public:
	static const _uint g_iAnimIndex = CPlayer::Player_State::Player_Grenade_Throw_v2;

};

END