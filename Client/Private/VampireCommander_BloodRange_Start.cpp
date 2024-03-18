#include "VampireCommander_BloodRange_Start.h"
#include "VampireCommander_BloodRange_Loop.h"

#include "SMath.h"
#include "GameInstance.h"

void CVampireCommander_BloodRange_Start::Initialize(CVampireCommander* pActor)
{
	__super::Initialize(pActor);
	//string Test = "Data_Animation/";
	//pActor->Set_EventNotify(Test, "Test2_AnimationData.json");
	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);

	_int iRandomInfected = SMath::Random(0, 9);//����Ƽ�� ���� ���� Ȯ�� 
	
	if (iRandomInfected <= 8)
	{

		for (int i = 0; i < 3; ++i)
		{
			_int randomX = SMath::Random(-4, 4);
			_int randomZ = SMath::Random(-4, 4);
			CGameObject* pMonster = nullptr;
			_float3 Temp = pActor->Get_Position() + _float3((_float)randomX, 0.f, (_float)randomZ);
			Temp.y = 0.f;
			if (Temp.x >= 20 && Temp.x <= 100 && Temp.z >= 30 && Temp.z <= 100)
			{
				pMonster = m_pGameInstance->Add_CloneObject_And_Get(LEVEL_INTRO_BOSS, L"Layer_Monster", TEXT("Prototype_GameObject_Infected_A"));
				pMonster->Set_InitPosition(Temp);
			}
			
		}
		
	}

	pActor->Set_Invincible(true);
}

CState<CVampireCommander>* CVampireCommander_BloodRange_Start::Update(CVampireCommander* pActor, _float fTimeDelta)
{
	if (pActor->Is_Animation_End())
	{
		return new CVampireCommander_BloodRange_Loop();
	}

	return nullptr;
	//__super::Update(pActor, fTimeDelta);
//  ����
// 	if (pActor->Get_TrackPosition() <= pActor->Get_CharcterDesc().EffectOnTrackPosition)
// 		pActor->bTest = true;
// 
// 	if (pActor->bTest == true)
// 	{
// 		if (pActor->Get_TrackPosition() >= pActor->Get_CharcterDesc().EffectOnTrackPosition)
// 		{
// 			//�޼� ������ ����ȭ ���Ѽ� �� ������ ��ǥ�� ���⿡ �ִ� �� �����Ǿȿ� ���� ������ ����ġ���� �������� ���� ���� ������ �Ǿ� �־ 0�� ��ó����������
// 			CEffect* pEffect = CClone_Manager::GetInstance()->Create_Effect(m_pGameInstance->Get_NextLevel(), LAYER_EFFECT, pActor->Get_CharcterDesc().EffectFileName + ".json");
// 			_float3 EffectBonePosition = {};
// 			EffectBonePosition.x = pActor->Get_Body()->Get_BonePtr("RightHandIK")->Get_CombinedTransformationFloat4x4()._41;
// 			EffectBonePosition.y = pActor->Get_Body()->Get_BonePtr("RightHandIK")->Get_CombinedTransformationFloat4x4()._42;
// 			EffectBonePosition.z = pActor->Get_Body()->Get_BonePtr("RightHandIK")->Get_CombinedTransformationFloat4x4()._43;
// 			pEffect->Set_Position(EffectBonePosition);
// 			pActor->bTest = false;
// 		}
// 	}

	//return __super::Update_State(pActor, fTimeDelta, g_iAnimIndex);
}

void CVampireCommander_BloodRange_Start::Release(CVampireCommander* pActor)
{
	__super::Release(pActor);
}
