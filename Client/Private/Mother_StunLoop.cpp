#include "Mother_StunLoop.h"
#include "Mother_StunEnd.h"
#include "GameInstance.h"

void CMother_StunLoop::Initialize(CMother* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_LOOP, true);
}

CState<CMother>* CMother_StunLoop::Update(CMother* pActor, _float fTimeDelta)
{
	//���������� �������� �� �ı��ؾ����� �������� �Ѿ 
	if (m_pGameInstance->Key_Down(DIK_2))
	{
		return new CMother_StunEnd;
	}
	return nullptr;
}

void CMother_StunLoop::Release(CMother* pActor)
{
	__super::Release(pActor);
}	
