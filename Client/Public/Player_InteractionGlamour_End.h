#pragma once
#include "Player_State_Interaction.h"


BEGIN(Client)

class CPlayer_InteractionGlamour_End : public CPlayer_State_Interaction
{
public:
	virtual void Initialize(CPlayer* pActor) override;
	virtual CState<CPlayer>* Update(CPlayer* pActor, _float fTimeDelta) override;
	virtual void Release(CPlayer* pActor) override;

public:
	static const _uint g_iAnimIndex = CPlayer::Player_State::Player_InteractionGlamour_End;

};

END