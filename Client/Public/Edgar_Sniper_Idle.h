#pragma once
#include "Edgar_State_Normal.h"


BEGIN(Client)

class CEdgar_Sniper_Idle : public CEdgar_State_Normal
{
public:
	virtual void Initialize(CEdgar* pActor) override;
	virtual CState<CEdgar>* Update(CEdgar* pActor, _float fTimeDelta) override;
	virtual void Release(CEdgar* pActor) override;

public:
	static const _uint g_iAnimIndex = CEdgar::Edgar_State::Edgar_Sniper_Idle;

};

END