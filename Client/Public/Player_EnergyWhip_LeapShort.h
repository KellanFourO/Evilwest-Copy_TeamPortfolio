#pragma once
#include "Player_State_EnergyWhip.h"


BEGIN(Client)

class CPlayer_EnergyWhip_LeapShort : public CPlayer_State_EnergyWhip
{
public:
	virtual void Initialize(CPlayer* pActor) override;
	virtual CState<CPlayer>* Update(CPlayer* pActor, _float fTimeDelta) override;
	virtual void Release(CPlayer* pActor) override;

public:
	static const _uint g_iAnimIndex = CPlayer::Player_State::Player_EnergyWhip_LeapShort;

};

END