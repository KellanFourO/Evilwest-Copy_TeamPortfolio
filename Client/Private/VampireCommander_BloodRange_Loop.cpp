#include "VampireCommander_BloodRange_Loop.h"
#include "VampireCommander_BloodRange_Stop.h"
#include "VampireCommander_BloodRange_Stun_Start.h"

#include "Effect.h"
#include "Effect_Manager.h"
#include "UI_Weakness.h"
#include "Bone.h"
#include "SMath.h"

void CVampireCommander_BloodRange_Loop::Initialize(CVampireCommander* pActor)
{
	__super::Initialize(pActor);
	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_LOOP, true);
	m_iLoopescape = 0;
	m_fPreHP = pActor->Get_Hp();
	
	m_pEffect = EFFECT_MANAGER->Create_Effect(LEVEL_INTRO_BOSS,L"Layer_Effect", "blood Loop_04.json", pActor);
}

CState<CVampireCommander>* CVampireCommander_BloodRange_Loop::Update(CVampireCommander* pActor, _float fTimeDelta)
{
	if (m_fHealHP>=200.f) //  && false == pActor->Is_Revealed_Weakness()
	{
		return new CVampireCommander_BloodRange_Stop();
	}
	else if (m_bSuccessShooting == true)//������ ���� ���� ����������
	{
		pActor->Set_Hp(m_fPreHP);
		return new CVampireCommander_BloodRange_Stun_Start();
	}
	//ü��ȸ�� 
	m_fHealHP += 0.2f;

	pActor->Set_Hp(m_fPreHP + m_fHealHP);

	++m_iLoopescape;
	
	//���Ϳ� ��Ƶ� ���� �������� �ҷ�����
	if (m_bFlags[0] == false&& m_fHealHP >= 10.f&&m_fHealHP <70.f)
	{
		
		CUI_Weakness* pWeakneesUI = dynamic_cast<CUI_Weakness*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_STATIC, TEXT("Layer_UI"), TEXT("Prototype_GameObject_UI_Weakness")));
		pActor->m_pWeakneesUIs.push_back(pWeakneesUI);
		
		m_iRandom[0] = SMath::Random(0, 12);
		m_bFlags[0] = true;
		//m_bFlags[1] = true;
	}
	else if (m_bFlags[1] == false && m_fHealHP >= 70.f && m_fHealHP < 130.f)
	{
		CUI_Weakness* pWeakneesUI = dynamic_cast<CUI_Weakness*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_STATIC, TEXT("Layer_UI"), TEXT("Prototype_GameObject_UI_Weakness")));
		pActor->m_pWeakneesUIs.push_back(pWeakneesUI);
		m_iRandom[1] = SMath::Random(0, 12);
		m_bFlags[1] = true;
	}
	else if (m_bFlags[2] == false && m_fHealHP >= 130.f)
	{
		CUI_Weakness* pWeakneesUI = dynamic_cast<CUI_Weakness*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_STATIC, TEXT("Layer_UI"), TEXT("Prototype_GameObject_UI_Weakness")));
		pActor->m_pWeakneesUIs.push_back(pWeakneesUI);
		m_iRandom[2] = SMath::Random(0, 12);
		m_bFlags[2] = true;
	}
	if (pActor->m_pWeakneesUIs.size() > 0)
	{
		if(m_fHealHP >= 10.f)
		{//���� ��ġ ����1
			_float4x4 BoneMatrix = {};
			_float4x4 temp = {};

			BoneMatrix = pActor->Get_Body()->Get_BonePtr(pActor->m_vRandomBones[m_iRandom[0]].c_str())->Get_CombinedTransformationMatrix();
			_float4x4 pCharacterPos = pActor->Get_Transform()->Get_WorldMatrix();

			XMStoreFloat4x4(&temp, BoneMatrix * pCharacterPos);

			pActor->m_pWeakneesUIs[0]->SetUp_WorldToScreen(temp);
		}
		if (m_fHealHP >= 70.f)
		{//���� ��ġ ����2
			_float4x4 BoneMatrix = {};
			_float4x4 temp = {};

			BoneMatrix = pActor->Get_Body()->Get_BonePtr(pActor->m_vRandomBones[m_iRandom[1]].c_str())->Get_CombinedTransformationMatrix();
			_float4x4 pCharacterPos = pActor->Get_Transform()->Get_WorldMatrix();

			XMStoreFloat4x4(&temp, BoneMatrix * pCharacterPos);

			pActor->m_pWeakneesUIs[1]->SetUp_WorldToScreen(temp);
		}
		if (m_fHealHP >= 130.f)
		{//���� ��ġ ����3
			_float4x4 BoneMatrix = {};
			_float4x4 temp = {};

			BoneMatrix = pActor->Get_Body()->Get_BonePtr(pActor->m_vRandomBones[m_iRandom[2]].c_str())->Get_CombinedTransformationMatrix();
			_float4x4 pCharacterPos = pActor->Get_Transform()->Get_WorldMatrix();

			XMStoreFloat4x4(&temp, BoneMatrix * pCharacterPos);

			pActor->m_pWeakneesUIs[2]->SetUp_WorldToScreen(temp);
		}
	}
	
	

	return nullptr;
}

void CVampireCommander_BloodRange_Loop::Release(CVampireCommander* pActor)
{
	__super::Release(pActor);
	m_pEffect->Set_Dead(true);
	for (int i = 0; i < pActor->m_pWeakneesUIs.size();++i)
	{
		pActor->m_pWeakneesUIs[i]->Set_Dead(true);
		//Safe_Release(pActor->m_pWeakneesUIs[i]);
	}
	pActor->m_pWeakneesUIs.clear();
}
