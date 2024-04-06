	#include "stdafx.h"
#include "Effect_Manager.h"

#include "GameInstance.h"

#include "Effect.h"
#include "Effect_Void.h"
#include "Effect_Trail.h"

IMPLEMENT_SINGLETON(CEffect_Manager);

CEffect_Manager::CEffect_Manager()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CEffect_Manager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	if (nullptr == pDevice || nullptr == pContext)
		return E_FAIL;

	m_pDevice = pDevice;
	Safe_AddRef(m_pDevice);

	m_pContext = pContext;
	Safe_AddRef(m_pContext);


	return S_OK;
}

// ���� �Ĺ��� ����Ʈ
CEffect* CEffect_Manager::Play_Effect(string strFileName, CGameObject* pOwner, _bool bUseSocket, string strBoneTag)
{
	queue<CEffect*>* EffectPool = Get_EffectPool(strFileName);

	if (EffectPool == nullptr) 
		return nullptr;
	
	CEffect* pEffect = EffectPool->front();

	Safe_AddRef(pEffect);

	if (nullptr != pOwner)
		pEffect->Set_Object_Owner(pOwner);	// �θ� ���� (�θ� �ְ�, ����Ʈ�� bParentPivot�� True�̸� ���ʰ�ü�� ����ٴ�)


	if (bUseSocket)	// ���� ����̸� ���� ����
	{
		pEffect->Get_Desc()->bUseSocket = bUseSocket; 
		pEffect->Get_Desc()->strBoneTag = strBoneTag;
	}

	pEffect->Get_Desc()->bPlay = TRUE;
	pEffect->Set_Enable(TRUE);

	EffectPool->pop();

	return pEffect;

}

CEffect* CEffect_Manager::Play_Effect_StaticPivot(string strFileName, CGameObject* pOwner, _float4x4 matPivot)
{
	queue<CEffect*>* EffectPool = Get_EffectPool(strFileName);

	if (EffectPool == nullptr)
		return nullptr;

	CEffect* pEffect = EffectPool->front();

	Safe_AddRef(pEffect);

	if (nullptr != pOwner)
		pEffect->Set_Object_Owner(pOwner);	// �θ� ���� (�θ� �ְ�, ����Ʈ�� bParentPivot�� True�̸� ���ʰ�ü�� ����ٴ�)


	pEffect->Get_Desc()->bAttachStatic = TRUE;
	pEffect->Get_Desc()->matPivot = matPivot;

	pEffect->Get_Desc()->bPlay = TRUE;
	pEffect->Set_Enable(TRUE);

	EffectPool->pop();

	return pEffect;
}

// ���� ���� ��ġ ���� ����Ʈ
CEffect* CEffect_Manager::Play_Effect(string strFileName, _float3 vPos, _bool bLookTarget, _float3 vTargetPos)
{
	queue<CEffect*>* EffectPool = Get_EffectPool(strFileName);

	if (EffectPool == nullptr)
		return nullptr;

	CEffect* pEffect = EffectPool->front();

	Safe_AddRef(pEffect);

	// ��ġ ����
	pEffect->Set_Position(vPos);

	if (TRUE == bLookTarget)
	{
		// Ÿ���� �ٶ󺼰Ÿ� Look_At ���ֱ�
		_float4 vTargetPos_float4 = _float4(vTargetPos.x, vTargetPos.y, vTargetPos.z, 1.f);
		pEffect->Get_Transform()->Look_At(vTargetPos_float4);
	}

	pEffect->Get_Desc()->bPlay = TRUE;
	pEffect->Set_Enable(TRUE);

	EffectPool->pop();

	return pEffect;
}

// Tick ���鼭 �εε� �����Ǵ� ����Ʈ
HRESULT CEffect_Manager::Generate_Effect(_float* fTimeAcc, _float fGenerateTimeTerm, _float fTimeDelta, string strFileName
	, _float3 vPos
	, _bool bLookTarget, _float3 vTargetPos)
{
	*fTimeAcc += fTimeDelta; // �ð� ����

	if (*fTimeAcc >= fGenerateTimeTerm) // ���� �ð��� ���� �ð� �Һ��� Ŀ���� ����Ʈ ���� & ���� �ð� �ʱ�ȭ
	{
		*fTimeAcc = 0.f;

		CEffect* pEffect = Play_Effect(strFileName, vPos, bLookTarget, vTargetPos);
	}

	return S_OK;
}


CEffect* CEffect_Manager::Load_Effect(_uint iLevelIndex, string strAddPath, string strFileName, _bool bHasTrail, string strTrailFileName)
{
	CEffect::EFFECT_DESC	tEffectDesc = {};
	CEffect* pEffect = dynamic_cast<CEffect*>(m_pGameInstance->Add_CloneObject_And_Get(iLevelIndex, LAYER_EFFECT, TEXT("Prototype_GameObject_Effect"), &tEffectDesc));

	string strPath = "../Bin/DataFiles/Data_Effect";
	string strLoadPath = strPath + "/" + strAddPath + strFileName;

	json In_Json;
	CJson_Utility::Load_Json(strLoadPath.c_str(), In_Json);

	pEffect->Load_FromJson(In_Json);

	pEffect->Get_Desc()->strFileName = strFileName;	// ���� �̸� �����ְ� ����


	if (bHasTrail)
	{
		// Ʈ������ �����ִ� ����Ʈ�� ����Ÿ� Ʈ���� �غ�
		pEffect->Ready_Trail(iLevelIndex, strTrailFileName);
	}
	else
	{
		pEffect->Set_Nullptr_Trail();
	}

	return	pEffect;
}


CEffect_Trail* CEffect_Manager::Ready_Trail(_uint iLevelIndex, const wstring& strLayerTag, string strFileName, CGameObject* pOwner)
{
	CEffect_Void::EFFECTVOID_DESC	tVoidDesc = {};

	tVoidDesc.eType_Effect = { CEffect_Void::TRAIL };

	tVoidDesc.strTextureTag[CEffect_Void::TEXTURE_DIFFUSE] = TEXT("Prototype_Component_Texture_Effect_Diffuse");
	tVoidDesc.iTextureIndex[CEffect_Void::TEXTURE_DIFFUSE] = { 0 };

	tVoidDesc.strTextureTag[CEffect_Void::TEXTURE_MASK] = TEXT("Prototype_Component_Texture_Effect_Mask");
	tVoidDesc.iTextureIndex[CEffect_Void::TEXTURE_MASK] = { 0 };

	tVoidDesc.strTextureTag[CEffect_Void::TEXTURE_NOISE] = TEXT("Prototype_Component_Texture_Effect_Noise");
	tVoidDesc.iTextureIndex[CEffect_Void::TEXTURE_NOISE] = { 0 };

	CEffect_Trail* pTrail = dynamic_cast<CEffect_Trail*>(m_pGameInstance->Add_CloneObject_And_Get(iLevelIndex, strLayerTag, TEXT("Prototype_GameObject_Effect_Trail"), &tVoidDesc));

	Safe_AddRef(pTrail);

	string strPath = "../Bin/DataFiles/Data_Effect/Data_Trail";
	string strLoadPath = strPath + "/" + strFileName;

	json In_Json;
	CJson_Utility::Load_Json(strLoadPath.c_str(), In_Json);

	if (nullptr != pOwner)
		pTrail->Set_Object_Owner(pOwner);

	pTrail->Load_FromJson(In_Json);

	return pTrail;


	/* ��� ���� */
	// m_pTrail = EFFECT_MANAGER->Ready_Trail(LEVEL_TOOL, LAYER_EFFECT, "Test_Trail.json"); // �Ǵ� Part_PreviewŬ���� ����
}


CEffect_Trail* CEffect_Manager::Ready_Trail(string strFileName, CGameObject* pOwner)
{
	return Ready_Trail(m_pGameInstance->Get_NextLevel(), LAYER_EFFECT, strFileName, pOwner);
}


HRESULT CEffect_Manager::Ready_EffectPool()
{
	// Json�ε���� ���� ����Ʈ�� ����Ʈ Ǯ�� ����Ѵ�. 
	// Level_Logo �̴ϼȶ������ ȣ���ϰ� �ִ�.(��ü ���� �� �ѹ��� ȣ��Ǿ�� ��)
	// Bin/DataFiles/Data_Effect + �߰� ���

	_uint iLevel = LEVEL_STATIC;	//_uint iCurLevel = m_pGameInstance->Get_NextLevel();

	// 1���� �־ �Ǵ� ����Ʈ
	{
		/* Circle_Floor */
		FAILED_CHECK(Add_ToPool(iLevel, "Parasiter/", "Circle_Floor_03.json"));
		FAILED_CHECK(Add_ToPool(iLevel, "Parasiter/", "Circle_Floor_03_Solid.json"));
		FAILED_CHECK(Add_ToPool(iLevel, "Parasiter/", "Circle_Floor_04.json"));

		/* Boos 1 */
		FAILED_CHECK(Add_ToPool(iLevel, "VampireCommander/Map_Blood/", "Map_Blood_04.json"));
		FAILED_CHECK(Add_ToPool(iLevel, "VampireCommander/BloodRange_Loop/", "BloodRange_Loop_22_Smoke.json"));
	}


	// �뷮���� �ʿ��� ����Ʈ (300��)
	for (_uint i = 0; i < iMaxManyEffect; ++i)
	{
		/* Light */
		FAILED_CHECK(Add_ToPool(iLevel, "Fire/", "Fire_Torch_05.json"));

		/* Hit */
		FAILED_CHECK(Add_ToPool(iLevel, "Hit/", "Hit_Distortion.json"));
		FAILED_CHECK(Add_ToPool(iLevel, "Hit/", "Hit_Normal.json"));
	}


	// �߰� �ʿ��� ����Ʈ (100��)
	for (_uint i = 0; i < iMaxEffect; ++i)
	{
		/* Boos 1 */
		FAILED_CHECK(Add_ToPool(iLevel, "VampireCommander/Projectile_Range1/", "Projectile_Range1_04.json"));
		FAILED_CHECK(Add_ToPool(iLevel, "VampireCommander/Projectile_Range3/", "Projectile_Range3_02.json"));
		FAILED_CHECK(Add_ToPool(iLevel, "VampireCommander/Projectile_Range3/", "Projectile_Range3_Tick_03.json"));

		/* Boos 2 */
		FAILED_CHECK(Add_ToPool(iLevel, "Parasiter/", "Yellow_Blood_Test_02.json"));
		FAILED_CHECK(Add_ToPool(iLevel, "Parasiter/", "Son_Test_06.json"));


#pragma region �÷��̾� ����Ʈ

		/* Revolver_Fire */
		FAILED_CHECK(Add_ToPool(iLevel, "Player/Revolver_Fire/", "Revolver_Fire_03.json"));
		//FAILED_CHECK(Add_ToPool(iLevel, "Player/Revolver_Fire/", "Revolver_Fire_02_Tail.json"));

#pragma endregion


	}


	// �ؼҷ� �ʿ��� ����Ʈ (10��)
	for (_uint i = 0; i < iMaxFewEffect; ++i)
	{
#pragma region �÷��̾� ����Ʈ

		/* Heal */
		FAILED_CHECK(Add_ToPool(iLevel, "Player/Heal/", "Heal_08.json"));
		FAILED_CHECK(Add_ToPool(iLevel, "Player/Heal/", "Heal_Particle_07_Reverse.json"));
		FAILED_CHECK(Add_ToPool(iLevel, "Player/Heal/", "Heal_07_Light_03.json"));
		FAILED_CHECK(Add_ToPool(iLevel, "Player/Heal/", "Heal_Particle_07.json"));

		/* Heal_Blue */
		FAILED_CHECK(Add_ToPool(iLevel, "Player/Heal_Blue/", "Heal_08_Blue.json"));
		FAILED_CHECK(Add_ToPool(iLevel, "Player/Heal_Blue/", "Heal_Particle_07_Reverse_Blue.json"));
		FAILED_CHECK(Add_ToPool(iLevel, "Player/Heal_Blue/", "Heal_07_Light_03_Blue.json"));
		FAILED_CHECK(Add_ToPool(iLevel, "Player/Heal_Blue/", "Heal_Particle_07_Blue.json"));


		/* EnergyWhip */
		FAILED_CHECK(Add_ToPool(iLevel, "Player/Zapper_Shield/", "Zapper_Shield_21_distortionTest.json"));
		FAILED_CHECK(Add_ToPool(iLevel, "Player/Zapper_Dash/", "Zapper_Dash_30.json"));

		/* SlamDown */
		FAILED_CHECK(Add_ToPool(iLevel, "Player/SlamDown/", "SlamDown_v1_03_Rock.json"));
		FAILED_CHECK(Add_ToPool(iLevel, "Player/SlamDown/", "SlamDown_v2_24_Rock.json"));
		FAILED_CHECK(Add_ToPool(iLevel, "Player/SlamDown/", "SlamDown_v2_25_Rock.json"));

		/* DodgeBlink */
		FAILED_CHECK(Add_ToPool(iLevel, "Player/DodgeBlink/", "DodgeBlink_L_18.json"));
		FAILED_CHECK(Add_ToPool(iLevel, "Player/DodgeBlink/", "DodgeBlink_R_18.json"));

		/* Roll */
		FAILED_CHECK(Add_ToPool(iLevel, "Player/Roll/", "Roll_R_04.json"));
		FAILED_CHECK(Add_ToPool(iLevel, "Player/Roll/", "Roll_R_04.json"));

		/* Revolver */
		FAILED_CHECK(Add_ToPool(iLevel, "Player/Revolver/", "Revolver_13.json"));
		FAILED_CHECK(Add_ToPool(iLevel, "Player/Revolver/", "Revolver_13_Tail_01.json"));

#pragma endregion


	}

	return S_OK;
}



HRESULT CEffect_Manager::Add_ToPool(_uint iLevelIndex, string strAddPath, string strFileName, _bool bHasTrail, string strTrailFileName)
{
	CEffect* pEffect = Load_Effect(iLevelIndex, strAddPath, strFileName, bHasTrail, strTrailFileName);

	pEffect->End_Effect_ForPool();		// ���� & ���� ���·� �����

	if (0 == m_EffectPool.count(strFileName))
	{
		queue<CEffect*> TempQueue;
		TempQueue.push(pEffect);
		m_EffectPool.insert({ strFileName, TempQueue });
	}
	else
		m_EffectPool[strFileName].push(pEffect);

	return S_OK;
}

void CEffect_Manager::Return_ToPool(CEffect* pEffect)
{
	if (pEffect == nullptr)
		return;

	pEffect->End_Effect_ForPool();

	m_EffectPool[pEffect->Get_Desc()->strFileName].push(pEffect);

	Safe_Release(pEffect);
}


queue<CEffect*>* CEffect_Manager::Get_EffectPool(string strFileName)
{
	if (0 != m_EffectPool.count(strFileName))
	{
		return &m_EffectPool[strFileName];
	}

	return nullptr;
}


HRESULT CEffect_Manager::Clear_EffectPool()
{
	for (auto iter : m_EffectPool)
	{
		while (!iter.second.empty())
		{
			iter.second.front()->Set_Dead(TRUE);
			//Safe_Release(iter.second.front());
			iter.second.pop();
		}
	}

	m_EffectPool.clear();

	return S_OK;
}


void CEffect_Manager::Free()
{
	Clear_EffectPool();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

}
