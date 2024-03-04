#include "VampireCommander_BloodRange_Start.h"
#include "VampireCommander_BloodRange_Loop.h"

void CVampireCommander_BloodRange_Start::Initialize(CVampireCommander* pActor)
{
	__super::Initialize(pActor);
	//string Test = "Data_Animation/";
	//pActor->Set_EventNotify(Test, "Test2_AnimationData.json");
	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);
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
