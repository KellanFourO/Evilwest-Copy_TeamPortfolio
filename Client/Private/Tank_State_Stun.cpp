#include "..\Public\Tank_State_Stun.h"

void CTank_State_Stun::Initialize(CTank* pActor)
{
	__super::Initialize(pActor);
}

CState<CTank>* CTank_State_Stun::Update(CTank* pActor, _float fTimeDelta)
{
	__super::Update(pActor, fTimeDelta);

	return nullptr;
}

void CTank_State_Stun::Release(CTank* pActor)
{
	__super::Release(pActor);
}

CState<CTank>* CTank_State_Stun::Update_State(CTank* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	return Stun_State(pActor, fTimeDelta, _iAnimIndex);
}
