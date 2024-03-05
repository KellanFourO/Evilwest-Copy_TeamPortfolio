#pragma once
#include "VampireCommander_State_CutScene.h"

BEGIN(Client)

class CVampireCommander_Dead : public CVampireCommander_State_CutScene
{
public:
	virtual void Initialize(CVampireCommander* pActor) override;
	virtual CState<CVampireCommander>* Update(CVampireCommander* pActor, _float fTimeDelta) override;
	virtual void Release(CVampireCommander* pActor) override;

public:
	static const _uint g_iAnimIndex = CVampireCommander::VampireCommander_State::VampireCommander_StunDead_F_01;

};

END
