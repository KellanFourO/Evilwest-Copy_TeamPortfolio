#include "Mother_StunLoop.h"
#include "Mother_StunEnd.h"

void CMother_StunLoop::Initialize(CMother* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_LOOP, true);
}

CState<CMother>* CMother_StunLoop::Update(CMother* pActor, _float fTimeDelta)
{
	//���������� �������� �� �ı��ؾ����� �������� �Ѿ 
	return nullptr;
}

void CMother_StunLoop::Release(CMother* pActor)
{
	__super::Release(pActor);
}	
