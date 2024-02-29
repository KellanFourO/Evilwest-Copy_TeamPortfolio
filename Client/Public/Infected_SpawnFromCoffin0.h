#pragma once
#include "Infected_State_Spawn.h"


BEGIN(Client)

class CInfected_SpawnFromCoffin0 : public CInfected_State_Spawn
{
public:
	virtual void Initialize(CInfected* pActor) override;
	virtual CState<CInfected>* Update(CInfected* pActor, _float fTimeDelta) override;
	virtual void Release(CInfected* pActor) override;

public:
	static const _uint g_iAnimIndex = CInfected::Infected_State::Infected_SpawnFromCoffin0;

};

END