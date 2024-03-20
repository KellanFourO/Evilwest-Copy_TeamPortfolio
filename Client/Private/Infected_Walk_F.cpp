#include "stdafx.h"
#include "Transform.h"
#include "GameInstance.h"
#include "Infected_Walk_F.h"

void CInfected_Walk_F::Initialize(CInfected* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_LOOP, true, false);

	pActor->Get_Transform()->Set_Speed(1.f);	/* �����̴� �ӵ� ���� */
	pActor->Set_StiffnessRate_Upper(1.f);		/* �ִϸ��̼� ����ӵ� ���� */
}

CState<CInfected>* CInfected_Walk_F::Update(CInfected* pActor, _float fTimeDelta)
{
	pActor->Look_At_Target();
	pActor->Go_Straight(fTimeDelta);

	_float fDist = pActor->Calc_Distance(m_pGameInstance->Get_Player());
	_float WalkDistance = pActor->Get_Info().fWalk_Distance;	 // 10.f
	_float AttackDistance = pActor->Get_Info().fAttack_Distance; // 3.5f

	if (fDist > WalkDistance) // �Ÿ��� �־����� Walk -> Run 
	{
		return __super::PreRun_State(pActor, fTimeDelta, g_iAnimIndex);
	}
	else if ( fDist < AttackDistance) // ������ Walk -> Attack 
	{
		return __super::PreAttack_State(pActor, fTimeDelta, g_iAnimIndex);
	}

	return nullptr;
}

void CInfected_Walk_F::Release(CInfected* pActor)
{
	__super::Release(pActor);
}
