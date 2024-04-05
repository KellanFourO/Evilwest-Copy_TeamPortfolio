#pragma once
#include "Edgar_State_Throw.h"


BEGIN(Client)

class CEdgar_ThrowDynamite : public CEdgar_State_Throw
{
public:
	virtual void Initialize(CEdgar* pActor) override;
	virtual CState<CEdgar>* Update(CEdgar* pActor, _float fTimeDelta) override;
	virtual void Release(CEdgar* pActor) override;

public:
	static const _uint g_iAnimIndex = CEdgar::Edgar_State::Edgar_ThrowDynamite;

};

END