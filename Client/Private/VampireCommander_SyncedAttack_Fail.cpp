#include "VampireCommander_SyncedAttack_Fail.h"
#include "VampireCommander_SyncedAttack.h"
#include "VampireCommander_Idle.h"

void CVampireCommander_SyncedAttack_Fail::Initialize(CVampireCommander* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);
	pActor->Get_Weapon(L"Weapon_hand_R")->Set_Synced(true);

	CWeapon* pWeapon = pActor->Get_Weapon(TEXT("Weapon_hand_R"));

	pWeapon
		->Set_Damage(0.f)
		->Set_Direction(Direction::Right)
		->Set_Power(Power::Medium)
		->Set_Force(0.f);

	pWeapon->Set_Enable(true);
}

CState<CVampireCommander>* CVampireCommander_SyncedAttack_Fail::Update(CVampireCommander* pActor, _float fTimeDelta)
{
	//���⼭ ���� �浹�� �˻縦 �ϰ� �浹���� ��� ���������̸� �б��� Trackposition�� �޾Ƽ� syncedAttack ���� ��ȯ! syncedAttack������ �б��� trackposition���� ��ŸƮ
	if (pActor->Is_Animation_End())
	{
		return new CVampireCommander_Idle();
	}
	//else
	//{
	//
	//}

	return nullptr;
}

void CVampireCommander_SyncedAttack_Fail::Release(CVampireCommander* pActor)
{
	__super::Release(pActor);

	CWeapon* pWeapon = pActor->Get_Weapon(TEXT("Weapon_hand_R"));
	pWeapon->Set_Enable(false);
}
