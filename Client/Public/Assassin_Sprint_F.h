#pragma once
#include "Assassin_State_Run.h"


BEGIN(Client)

class CAssassin_Sprint_F : public CAssassin_State_Run
{
public:
	virtual void Initialize(CAssassin* pActor) override;
	virtual CState<CAssassin>* Update(CAssassin* pActor, _float fTimeDelta) override;
	virtual void Release(CAssassin* pActor) override;

public:
	static const _uint g_iAnimIndex = CAssassin::Assassin_State::Assassin_Sprint_F;

};

END