#pragma once
#include "Player_State_Roll.h"


BEGIN(Client)

class CPlayer_Roll_R : public CPlayer_State_Roll
{
public:
	virtual void Initialize(CPlayer* pActor) override;
	virtual CState<CPlayer>* Update(CPlayer* pActor, _float fTimeDelta) override;
	virtual void Release(CPlayer* pActor) override;

public:
	static const _uint g_iAnimIndex = CPlayer::Player_State::Player_Roll_R;

};

END