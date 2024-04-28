#include "Infected_DeathLight_F_01_NEW.h"
#include "GameInstance.h"

void CInfected_DeathLight_F_01_NEW::Initialize(CInfected* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);
	pActor->Get_Body()->Collider_Off();
	m_pGameInstance->Play_Sound(L"INFECTED_DEATH", L"death_1_turned01.wav", SOUND_ENEMY_DEAD, 7.f);
}

CState<CInfected>* CInfected_DeathLight_F_01_NEW::Update(CInfected* pActor, _float fTimeDelta)
{
	if (pActor->Is_Animation_End())
	{
		return __super::Update_State(pActor, fTimeDelta, g_iAnimIndex);
	}

	return nullptr;
}

void CInfected_DeathLight_F_01_NEW::Release(CInfected* pActor)
{
	__super::Release(pActor);
}
