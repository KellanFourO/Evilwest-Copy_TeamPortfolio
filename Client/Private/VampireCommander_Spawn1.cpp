#include "VampireCommander_Spawn1.h"
#include "VampireCommander_Idle.h"

void CVampireCommander_Spawn1::Initialize(CVampireCommander* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);
}

CState<CVampireCommander>* CVampireCommander_Spawn1::Update(CVampireCommander* pActor, _float fTimeDelta)
{
	if (pActor->Is_Animation_End())
	{
		return new CVampireCommander_Idle;
	}

	return nullptr;
}

void CVampireCommander_Spawn1::Release(CVampireCommander* pActor)
{
	__super::Release(pActor);
}