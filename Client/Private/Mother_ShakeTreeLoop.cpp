#include "Mother_ShakeTreeLoop.h"
#include "Mother_ShakeTreeEnd.h"
#include "MasterCamera.h"
#include "Data_Manager.h"
#include "SMath.h"
#include "GameInstance.h"
#include "Player.h"
#include "Effect_Manager.h"

void CMother_ShakeTreeLoop::Initialize(CMother* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_LOOP, true);
}

CState<CMother>* CMother_ShakeTreeLoop::Update(CMother* pActor, _float fTimeDelta)
{
	//������ �ð��� ������ ���� ���� End ��  ��ȯ ! 5������ �浹�ϸ� �ɰ� ����� �� �븻�� �س��� ī��Ʈ �ٿ����� �ѹ� �ִϸ��̼� �ٵ��� ���� �ѹ��� ��������
	//--����  �̷������� üũ�ؼ� �� ���ڰ� 0�� �Ǿ����� ������ ����� End�� ��ȯ ! 
	m_fTimeDelta += fTimeDelta;

	if (pActor->Is_Inputable_Front(35))
		m_bFlags[1] = false;

	if (pActor->Is_Inputable_Front(38))
		m_bFlags[1] = true;

	if (m_bFlags[0] == false && m_bFlags[1] == false && pActor->Is_Inputable_Front(37))
	{
		CSpringCamera* pSpringCam = CData_Manager::GetInstance()->Get_MasterCamera()->Get_SpringCamera();
		pSpringCam->Set_ShakeCameraMinMax(_float2(0.f, 0.2f));
		pSpringCam->Set_ShakeCameraTime(0.3f);
		pActor->Apply_Shake_And_Blur(Power::Medium);
		//�̋� ī�޶� ����ŷ �ϸ鼭 �ʿ� ��ü ���� ���� �߰��ϸ� �� �Ű��� 

		EFFECT_MANAGER->Play_Effect("Parasiter/", "SY_Falling_Leaves.json", nullptr, CData_Manager::GetInstance()->Get_Player()->Get_Position());


		CGameObject* pObjcet = { nullptr };
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				_float fRandom = SMath::fRandom(-5.f, 5.f);
				_float fRandom2 = SMath::fRandom(-5.f, 5.f);
				_float fRandom3 = SMath::fRandom(-5.f, 5.f);
				_float fRandom4 = SMath::fRandom(-5.f, 5.f);
				//++���� 1��и�
				pObjcet = m_pGameInstance->Add_CloneObject_And_Get(LEVEL_SNOWMOUNTAINBOSS, L"Layer_Monster", L"Prototype_GameObject_MotherShakeTreeProjectile");
				pObjcet->Set_Position(CData_Manager::GetInstance()->Get_Player()->Get_Position() + _float3((j + 1) * 10.f, (20.f + fRandom), (i + 1) * 10.f));
				//-+���� 2��и�
				pObjcet = m_pGameInstance->Add_CloneObject_And_Get(LEVEL_SNOWMOUNTAINBOSS, L"Layer_Monster", L"Prototype_GameObject_MotherShakeTreeProjectile");
				pObjcet->Set_Position(CData_Manager::GetInstance()->Get_Player()->Get_Position() + _float3(-j * 10.f, (20.f + fRandom2), i * 10.f));
				//--���� 3��и�
				pObjcet = m_pGameInstance->Add_CloneObject_And_Get(LEVEL_SNOWMOUNTAINBOSS, L"Layer_Monster", L"Prototype_GameObject_MotherShakeTreeProjectile");
				pObjcet->Set_Position(CData_Manager::GetInstance()->Get_Player()->Get_Position() + _float3((-j - 1) * 10.f, (20.f + fRandom3), (-i - 1) * 10.f));
				//+-���� 4��и�
				pObjcet = m_pGameInstance->Add_CloneObject_And_Get(LEVEL_SNOWMOUNTAINBOSS, L"Layer_Monster", L"Prototype_GameObject_MotherShakeTreeProjectile");
				pObjcet->Set_Position(CData_Manager::GetInstance()->Get_Player()->Get_Position() + _float3(j * 10.f, (20.f + fRandom4), -i * 10.f));
			}

		}

		m_bFlags[0] = true;
	}

	if(pActor->Is_Inputable_Front(41))
		m_bFlags[0] = false;

	if (m_fTimeDelta >= 10.f && pActor->Is_Inputable_Front(41))
	{
		m_fTimeDelta = 0.f;
		return new CMother_ShakeTreeEnd;
	}
	return nullptr;
}

void CMother_ShakeTreeLoop::Release(CMother* pActor)
{
	__super::Release(pActor);
	CSpringCamera* pSpringCam = CData_Manager::GetInstance()->Get_MasterCamera()->Get_SpringCamera();
	pSpringCam->Set_ShakeCameraMinMax(_float2(0.3f, 0.3f));

	pSpringCam->Set_ShakeCameraTime(0.2f);
}
