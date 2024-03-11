#pragma once
#include "Infected_State_Hit.h"

BEGIN(Client)

class CInfected_Scared_03 : public CInfected_State_Hit
{
public:
	virtual void Initialize(CInfected* pActor) override;
	virtual CState<CInfected>* Update(CInfected* pActor, _float fTimeDelta) override;
	virtual void Release(CInfected* pActor) override;

public:
	static const _uint g_iAnimIndex = CInfected::Infected_State::Infected_Scared_03;

};

END