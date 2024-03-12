#pragma once
#include "Player_State_Winchester.h"


BEGIN(Client)

class CPlayer_Winchester_WeaponUnholster : public CPlayer_State_Winchester
{
public:
	virtual void Initialize(CPlayer* pActor) override;
	virtual CState<CPlayer>* Update(CPlayer* pActor, _float fTimeDelta) override;
	virtual void Release(CPlayer* pActor) override;

public:
	static const _uint g_iAnimIndex = CPlayer::Player_State::Player_Winchester_WeaponUnholster;

};

END