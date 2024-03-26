#include "Mother_SpittingLoop.h"
#include "Mother_SpittingEnd.h"
#include "MotherMouth.h"

void CMother_SpittingLoop::Initialize(CMother* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_LOOP, true);
}

CState<CMother>* CMother_SpittingLoop::Update(CMother* pActor, _float fTimeDelta)
{
	//MotherMouth�� ������ ���� �ð� �������� ������ ���� �ְ� ���� End�� ��ȯ ! 
	if (pActor->m_pMonster->m_bLoopFinished == true)
	{
		return new CMother_SpittingEnd;
	}
	return nullptr;
}

void CMother_SpittingLoop::Release(CMother* pActor)
{
	__super::Release(pActor);
}