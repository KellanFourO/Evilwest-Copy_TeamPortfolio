#include "Mother_VomitLoop.h"

void CMother_VomitLoop::Initialize(CMother* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_LOOP, true);
}

CState<CMother>* CMother_VomitLoop::Update(CMother* pActor, _float fTimeDelta)
{
	//���� �ð��� �������� End�� ��ȯ 
	return nullptr;
}

void CMother_VomitLoop::Release(CMother* pActor)
{
	__super::Release(pActor);
}
