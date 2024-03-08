#include "..\Public\Assassin_State_Scared.h"

void CAssassin_State_Scared::Initialize(CAssassin* pActor)
{
	__super::Initialize(pActor);
}

CState<CAssassin>* CAssassin_State_Scared::Update(CAssassin* pActor, _float fTimeDelta)
{
	__super::Update(pActor, fTimeDelta);

	return nullptr;
}

void CAssassin_State_Scared::Release(CAssassin* pActor)
{
	__super::Release(pActor);
}

CState<CAssassin>* CAssassin_State_Scared::Update_State(CAssassin* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	return Scared_State(pActor, fTimeDelta, _iAnimIndex);
}