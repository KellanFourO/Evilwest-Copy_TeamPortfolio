#pragma once
#include "Tank_State_Spawn.h"


BEGIN(Client)

class CTank_Spawn_04_Charge : public CTank_State_Spawn
{
public:
	virtual void Initialize(CTank* pActor) override;
	virtual CState<CTank>* Update(CTank* pActor, _float fTimeDelta) override;
	virtual void Release(CTank* pActor) override;

public:
	static const _uint g_iAnimIndex = CTank::Tank_State::Tank_Spawn_04_Charge;

};

END