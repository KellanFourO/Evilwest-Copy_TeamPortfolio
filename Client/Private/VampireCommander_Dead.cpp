#include "VampireCommander_Dead.h"

void CVampireCommander_Dead::Initialize(CVampireCommander* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);
}

CState<CVampireCommander>* CVampireCommander_Dead::Update(CVampireCommander* pActor, _float fTimeDelta)
{
	//�ӽ� ���� ���߿� ������ üũ�ϸ鼭 �ؾ� �� 
	if (pActor->Is_Animation_End())
	{
		pActor->m_bCntDead_Active = true;
	}

	return nullptr;
}

void CVampireCommander_Dead::Release(CVampireCommander* pActor)
{
	__super::Release(pActor);
}
