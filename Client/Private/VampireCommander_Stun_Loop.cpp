#include "VampireCommander_Stun_Loop.h"
#include "GameInstance.h"

void CVampireCommander_Stun_Loop::Initialize(CVampireCommander* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_LOOP, true);
	//1�� 7��
}

CState<CVampireCommander>* CVampireCommander_Stun_Loop::Update(CVampireCommander* pActor, _float fTimeDelta)
{
	PlaySound_Breath(fTimeDelta);
	//�������� ������ ������ ���� ���¿��� ������ Stun_Stop���� ���� ��ų ���� �÷��̾��ʿ��� �̰� ���̰ų� ���������� �����ų� �ϴ� �� �־�־� ��
	return nullptr;
}

void CVampireCommander_Stun_Loop::Release(CVampireCommander* pActor)
{
	__super::Release(pActor);

	m_fSoundTimeAcc = 0.f;
	m_bBreathChange = false; //! 1�� ���� 2�� ���� �����ư��鼭
}

void CVampireCommander_Stun_Loop::PlaySound_Breath(_float fTimeDelta)
{
	
	//!_float	m_fSoundTimeAcc = 0.f;
	//!_float  m_fBreathPlayTime = 1.5f;
	//!_bool   m_bBreathChange = false; //! 1�� ���� 2�� ���� �����ư��鼭
	
	m_fSoundTimeAcc += fTimeDelta;


	if (m_fSoundTimeAcc >= m_fBreathPlayTime)
	{
		if (m_bBreathChange == true)
		{
			m_pGameInstance->Play_Sound(L"VAMPIRE_IDLE", L"commander_lesser_vo_idle001.wav", SOUND_ENEMY_VOICE, 7.f);
			m_bBreathChange = false;
		}
		else
		{
			m_pGameInstance->Play_Sound(L"VAMPIRE_IDLE", L"commander_lesser_vo_idle007.wav", SOUND_ENEMY_VOICE2, 7.f);
			m_bBreathChange = true;
		}

		m_fSoundTimeAcc = 0.f;
	}

}
