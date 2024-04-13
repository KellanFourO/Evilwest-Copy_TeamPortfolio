#include "Mother_Hit.h"
#include "Mother_StunLoop.h"
#include "Mother_StunEnd.h"
#include "GameInstance.h"

void CMother_Hit::Initialize(CMother* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);
	m_pGameInstance->Play_Sound(L"MOTHER_HIT", L"grand_parasiter_vo_mind_hit004.wav", SOUND_ENEMY_VOICE, 10.f);
	
}

CState<CMother>* CMother_Hit::Update(CMother* pActor, _float fTimeDelta)
{
	//int�� ���� Ȯ���Ҽ� �ִ� ���� 2���� �ּ� �װ��� 1����� StunLoop�� 2����� StunEnd�� 
	if (pActor->Is_Animation_End())
	{
		return new CMother_StunLoop;
	}

	return nullptr;
}

void CMother_Hit::Release(CMother* pActor)
{
	__super::Release(pActor);
}
