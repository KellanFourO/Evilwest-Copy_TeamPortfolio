#include "VampireCommander_BloodRange_Loop.h"
#include "VampireCommander_BloodRange_Stop.h"
#include "VampireCommander_BloodRange_Stun_Start.h"

#include "Effect.h"
#include "Effect_Manager.h"

void CVampireCommander_BloodRange_Loop::Initialize(CVampireCommander* pActor)
{
	__super::Initialize(pActor);
	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_LOOP, true);
	m_iLoopescape = 0;
	m_fPreHP = pActor->Get_Hp();
	
	m_pEffect = EFFECT_MANAGER->Create_Effect(LEVEL_INTRO_BOSS,L"Layer_Effect", "blood Loop_03.json", pActor);

}

CState<CVampireCommander>* CVampireCommander_BloodRange_Loop::Update(CVampireCommander* pActor, _float fTimeDelta)
{
	if (m_fHealHP>=200.f) //  && false == pActor->Is_Revealed_Weakness()
	{
		return new CVampireCommander_BloodRange_Stop();
	}

	//else if (true == pActor->Is_Revealed_Weakness())//������ ���� ���� ����������
	//{
	//	pActor->Set_Hp(m_fPreHP);
	//	return new CVampireCommander_BloodRange_Stun_Start();
	//}


	//ü��ȸ�� 
	m_fHealHP += 0.3f;
	
	pActor->Set_Hp(m_fPreHP + m_fHealHP);

	++m_iLoopescape;

	return nullptr;
}

void CVampireCommander_BloodRange_Loop::Release(CVampireCommander* pActor)
{
	__super::Release(pActor);
	m_pEffect->Set_Dead(true);
}
