#include "Son_Dead.h"
#include "Transform.h"
#include "GameInstance.h"

void CSon_Dead::Initialize(CSon* pActor)
{
	__super::Initialize(pActor);
	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);
	_float3 Temp = pActor->Get_Position();
	_float3 Down = pActor->Get_Transform()->Get_State(CTransform::STATE_UP);
	Temp = Temp -2*Down;
	pActor->Set_Position(Temp);
	m_pGameInstance->Play_Sound(L"SON_DEAD", L"VampireSpitter_VO_Spit_01.wav", SOUND_ENEMY_HIT_GROUND, 7.f);
}

CState<CSon>* CSon_Dead::Update(CSon* pActor, _float fTimeDelta)
{
	//�ִϸ��̼��� ������ ���� 
	if (pActor->Is_Animation_End())
	{
		pActor->Set_Dead(true);
	}

	return nullptr;
}

void CSon_Dead::Release(CSon* pActor)
{
	__super::Release(pActor);
	_float3 Temp = pActor->Get_Position();
	_float3 Down = pActor->Get_Transform()->Get_State(CTransform::STATE_UP);
	Temp = Temp + 2 * Down;
	pActor->Set_Position(Temp);
}
