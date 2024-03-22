#include "Mother_Hit.h"
#include "Mother_StunLoop.h"
#include "Mother_StunEnd.h"

void CMother_Hit::Initialize(CMother* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);
}

CState<CMother>* CMother_Hit::Update(CMother* pActor, _float fTimeDelta)
{
	//int�� ���� Ȯ���Ҽ� �ִ� ���� 2���� �ּ� �װ��� 1����� StunLoop�� 2����� StunEnd�� 

	return nullptr;
}

void CMother_Hit::Release(CMother* pActor)
{
	__super::Release(pActor);
}
