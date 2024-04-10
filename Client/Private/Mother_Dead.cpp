#include "Mother_Dead.h"
#include "Data_Manager.h"

void CMother_Dead::Initialize(CMother* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);
}

CState<CMother>* CMother_Dead::Update(CMother* pActor, _float fTimeDelta)
{
	if (pActor->Is_Animation_End())
	{	//�׿� ������ ���⼭ 
		pActor->Set_Dead(true);
	}

	return nullptr;
}

void CMother_Dead::Release(CMother* pActor)
{
	__super::Release(pActor);
	CData_Manager::GetInstance()->Add_CurEXP(PARASITER_EXP);
}
