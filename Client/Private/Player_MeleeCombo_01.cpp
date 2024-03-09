#include "Player_MeleeCombo_01.h"
#include "Weapon.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "Bone.h"

void CPlayer_MeleeCombo_01::Initialize(CPlayer* pActor)
{
	__super::Initialize(pActor);
	string Test = "Data_Animation/";
	pActor->Set_EventNotify(Test, "Test2_AnimationData.json");
	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);

	CWeapon* pWeapon = pActor->Get_Weapon(TEXT("Weapon_Punch_R"));
	
	pWeapon
		->Set_Damage(0.f)
		->Set_Direction(Direction::Right)
		->Set_Power(Power::Medium)
		->Set_Force(0.3f);

	pWeapon->Set_Enable(true);

}

CState<CPlayer>* CPlayer_MeleeCombo_01::Update(CPlayer* pActor, _float fTimeDelta)
{
	__super::Update(pActor, fTimeDelta);
	
		if (pActor->Get_TrackPosition() <= pActor->Get_CharcterDesc().EffectOnTrackPosition)
			pActor->bTest = true;

		if (pActor->bTest == true)
		{
			if (pActor->Get_TrackPosition() >= pActor->Get_CharcterDesc().EffectOnTrackPosition)
			{
				//�޼� ������ ����ȭ ���Ѽ� �� ������ ��ǥ�� ���⿡ �ִ� �� �����Ǿȿ� ���� ������ ����ġ���� �������� ���� ���� ������ �Ǿ� �־ 0�� ��ó����������
				CEffect* pEffect = EFFECT_MANAGER->Create_Effect(m_pGameInstance->Get_NextLevel(), LAYER_EFFECT, pActor->Get_CharcterDesc().EffectFileName + ".json");
				//_float3 EffectBonePosition = {};
				//EffectBonePosition.x = pActor->Get_Body()->Get_BonePtr("RightHandIK")->Get_CombinedTransformationFloat4x4()._41;
				//EffectBonePosition.y = pActor->Get_Body()->Get_BonePtr("RightHandIK")->Get_CombinedTransformationFloat4x4()._42;
				//EffectBonePosition.z = pActor->Get_Body()->Get_BonePtr("RightHandIK")->Get_CombinedTransformationFloat4x4()._43;
				//pEffect->Set_Position(EffectBonePosition);

				CWeapon* pWeapon = pActor->Get_Weapon(TEXT("Weapon_Punch_R"));
				_float3 vPos = pWeapon->Get_WorldPosition();

				pEffect->Set_Position(vPos);
				pActor->bTest = false;
			}
		}


	return __super::Update_State(pActor, fTimeDelta, g_iAnimIndex);
	//�ݶ��̴� ���� ����
// 	if (m_fCurrentTrackPosition >= m_iColliderOnTrackPosition && m_fCurrentTrackPosition < m_iColliderOffTrackPosition)
// 	{
// 		m_pBoneCollider[m_iSelectColliderIndex]->Set_Enable(true);
// 	}
// 	else
// 	{
// 		m_pBoneCollider[m_iSelectColliderIndex]->Set_Enable(false);
// 	}
}

void CPlayer_MeleeCombo_01::Release(CPlayer* pActor)
{
	__super::Release(pActor);

	CWeapon* pWeapon = pActor->Get_Weapon(TEXT("Weapon_Punch_R"));
	pWeapon->Set_Enable(false);
}
