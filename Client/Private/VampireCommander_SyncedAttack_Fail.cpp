#include "VampireCommander_SyncedAttack_Fail.h"
#include "VampireCommander_SyncedAttack.h"
#include "VampireCommander_Idle.h"
#include "Body_VampireCommander.h"

void CVampireCommander_SyncedAttack_Fail::Initialize(CVampireCommander* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);
	pActor->Get_Weapon(L"Weapon_hand_R")->Set_Synced(true);

	CWeapon* pWeapon = pActor->Get_Weapon(TEXT("Weapon_hand_R"));

	pWeapon
		->Set_Damage(0)
		->Set_Direction(Direction::Right)
		->Set_Power(Power::Medium)
		->Set_Force(0.f);

	CBody_VampireCommander* pBody = dynamic_cast<CBody_VampireCommander*>(pActor->Get_Body());
	pBody->Set_RenderState(CBody_VampireCommander::RENDER_STATE::ATTACK);

	pActor->m_bLookAt = false;

}

CState<CVampireCommander>* CVampireCommander_SyncedAttack_Fail::Update(CVampireCommander* pActor, _float fTimeDelta)
{
	CWeapon* pWeapon = pActor->Get_Weapon(TEXT("Weapon_hand_R"));
	if (pActor->Is_Inputable_Front(44))
	{
		pWeapon->Set_Enable(true);
	}
	else if (pActor->Is_Inputable_Front(49))
	{
		pWeapon->Set_Enable(false);
	}

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

	pActor->m_bLookAt = true;

}
