#pragma once
#include "Son_State_Normal.h"

BEGIN(Client)

class CSon_Dead : public CSon_State_Normal
{
public:
	virtual void Initialize(CSon* pActor) override;
	virtual CState<CSon>* Update(CSon* pActor, _float fTimeDelta) override;
	virtual void Release(CSon* pActor) override;

public:
	static const _uint g_iAnimIndex = CSon::Son_State::Parasiter_Tentacle_Death;

};

END
