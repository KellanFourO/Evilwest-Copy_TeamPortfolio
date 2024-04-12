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
CEffect* CEffect_Manager::Play_Effect(string strAddPath, string strFileName, CGameObject* pOwner, _bool bUseSocket, string strBoneTag)
{
	queue<CEffect*>* EffectPool = Get_EffectPool(strFileName);


	if (EffectPool == nullptr)
	{
//#ifdef _DEBUG
		//MSG_BOX("CEffect_Manager :: Please Check Ready_EffectPool()");
		//return Create_Effect_ForDebug(strAddPath, strFileName, pOwner, bUseSocket, strBoneTag);
		return nullptr;
//#endif // _DEBUG
	}

	if(0 >= EffectPool->size())
		return nullptr;

	CEffect* pEffect = EffectPool->front();

	if (nullptr == pEffect)
	{
//#ifdef _DEBUG
		//MSG_BOX("nullptr : CEffect_Manager::Play_Effect() / �غ��� ����Ʈ ������ �ʰ��߽��ϴ�.");
		//return Create_Effect_ForDebug(strAddPath, strFileName, pOwner, bUseSocket, strBoneTag);
		return nullptr;
//#endif // _DEBUG
	}


	//Safe_AddRef(pEffect);

	if (nullptr != pOwner)
		pEffect->Set_Object_Owner(pOwner);	// �θ� ���� (�θ� �ְ�, ����Ʈ�� bParentPivot�� true�̸� ���ʰ�ü�� ����ٴ�)


	if (bUseSocket)	// ���� ����̸� ���� ����
	{
		pEffect->Get_Desc()->bUseSocket = bUseSocket; 
		pEffect->Get_Desc()->strBoneTag = strBoneTag;
	}

	pEffect->Get_Desc()->bPlay = true;
	pEffect->Set_Enable(true);

	EffectPool->pop();

	return pEffect;

}


// ���� ���� ��ġ ���� ����Ʈ
CEffect* CEffect_Manager::Play_Effect(string strAddPath, string strFileName, CGameObject* pOwner, _float3 vPos, _bool bLookTarget, _float3 vTargetPos)
{
	queue<CEffect*>* EffectPool = Get_EffectPool(strFileName);

	if (EffectPool == nullptr)
	{
//#ifdef _DEBUG
		//MSG_BOX("CEffect_Manager :: Please Check Ready_EffectPool()");
		//return Create_Effect_ForDebug(strAddPath, strFileName, vPos, bLookTarget, vTargetPos);
		return nullptr;
//#endif // _DEBUG
	}

	if (0 >= EffectPool->size())
		return nullptr;

	CEffect* pEffect = EffectPool->front();

	if (nullptr == pEffect)
	{
//#ifdef _DEBUG
		//MSG_BOX("nullptr : CEffect_Manager::Play_Effect() / �غ��� ����Ʈ ������ �ʰ��߽��ϴ�.");
		//return Create_Effect_ForDebug(strAddPath, strFileName, vPos, bLookTarget, vTargetPos);
		return nullptr;
//#endif // _DEBUG
	}


	//Safe_AddRef(pEffect);

	if (nullptr != pOwner)	// ���� ����
	{
		pEffect->Get_Desc()->bParentPivot = false;
		pEffect->Set_Object_Owner(pOwner);
	}
	

	// ��ġ ����
	//pEffect->Set_Position(vPos);
	pEffect->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(vPos.x, vPos.y, vPos.z, 1.f));


	if (bLookTarget)
	{
		// Ÿ���� �ٶ󺼰Ÿ� Look_At ���ֱ�
		_float4 vTargetPos_float4 = _float4(vTargetPos.x, vTargetPos.y, vTargetPos.z, 1.f);
		pEffect->Get_Transform()->Look_At(vTargetPos_float4);
	}

	pEffect->Get_Desc()->bPlay = true;
	pEffect->Set_Enable(true);

	EffectPool->pop();

	return pEffect;
}



CEffect* CEffect_Manager::Play_Effect_StaticPivot(string strAddPath, string strFileName, CGameObject* pOwner, _float4x4 matPivot)
{
	queue<CEffect*>* EffectPool = Get_EffectPool(strFileName);

	if (EffectPool == nullptr)
	{
//#ifdef _DEBUG
		//MSG_BOX("CEffect_Manager :: Please Check Ready_EffectPool()");
		//return Create_Effect_ForDebug_StaticPivot(strAddPath, strFileName, pOwner, matPivot);
		return nullptr;
//#endif // _DEBUG
	}


	if (0 >= EffectPool->size())
		return nullptr;

	CEffect* pEffect = EffectPool->front();


	if (nullptr == pEffect)
	{
//#ifdef _DEBUG
		//MSG_BOX("nullptr : CEffect_Manager::Play_Effect() / �غ��� ����Ʈ ������ �ʰ��߽��ϴ�.");
		//return Create_Effect_ForDebug_StaticPivot(strAddPath, strFileName, pOwner, matPivot);
		return nullptr;
//#endif // _DEBUG
	}


	//Safe_AddRef(pEffect);

	if (nullptr != pOwner)
		pEffect->Set_Object_Owner(pOwner);	// �θ� ���� (�θ� �ְ�, ����Ʈ�� bParentPivot�� True�̸� ���ʰ�ü�� ����ٴ�)


	pEffect->Get_Desc()->bAttachStatic = true;
	pEffect->Get_Desc()->matPivot = matPivot;

	pEffect->Get_Desc()->bPlay = true;
	pEffect->Set_Enable(true);

	EffectPool->pop();

	return pEffect;
}



// Tick ���鼭 �εε� �����Ǵ� ����Ʈ
HRESULT CEffect_Manager::Generate_Effect(_float* fTimeAcc, _float fGenerateTimeTerm, _float fTimeDelta, string strAddPath, string strFileName
	, _float3 vPos
	, _bool bLookTarget, _float3 vTargetPos
	, _bool bScaleLerp, _float3* vScaleAcc)
{
	*fTimeAcc += fTimeDelta; // �ð� ����

	if (*fTimeAcc >= fGenerateTimeTerm) // ���� �ð��� ���� �ð� �Һ��� Ŀ���� ����Ʈ ���� & ���� �ð� �ʱ�ȭ
	{
		*fTimeAcc = 0.f;

		CEffect* pEffect = Play_Effect(strAddPath, strFileName, nullptr, vPos, bLookTarget, vTargetPos);

		if (nullptr == pEffect)
		{
//#ifdef _DEBUG
			//MSG_BOX("nullptr : CEffect_Manager::Play_Effect() / ��ο� ����Ʈ �����Ͱ� ���ų�, �غ��� ����Ʈ ������ �ʰ��߽��ϴ�.");
			//return S_OK;
			return S_OK;
//#endif // _DEBUG
		}


		// ũ�� ������ true��
		if (bScaleLerp)
		{
			*vScaleAcc += *vScaleAcc;	// ũ�� ����

			_float3 vScaled = pEffect->Get_Transform()->Get_Scaled();

			_float3 vNewScale;
			vNewScale.x = vScaled.x + vScaleAcc->x;
			vNewScale.y = vScaled.y + vScaleAcc->y;
			vNewScale.z = vScaled.z + vScaleAcc->z;

			pEffect->Get_Transform()->Set_Scaling(vNewScale.x, vNewScale.y, vNewScale.z);
		}
	}

	return S_OK;
}


CEffect* CEffect_Manager::Load_Effect(_uint iLevelIndex, string strAddPath, string strFileName, _bool bHasTrail, string strTrailFileName)
{
	CEffect::EFFECT_DESC	tEffectDesc = {};
	CEffect* pEffect = dynamic_cast<CEffect*>(m_pGameInstance->Add_CloneObject_And_Get(iLevelIndex, LAYER_EFFECT, TEXT("Prototype_GameObject_Effect"), &tEffectDesc));

	if (nullptr == pEffect)
	{
		MSG_BOX("nullptr : CEffect_Manager::Load_Effect()");
		return nullptr;
	}

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

CEffect* CEffect_Manager::Create_Effect_ForDebug(string strAddPath, string strFileName, CGameObject* pOwner, _bool bUseSocket, string strBoneTag)
{
	// �� �Լ��� ����Ʈ�� �������� �ʴ°� �������Դϴ�. Ready_EffectPool()�� ����Ʈ�� �߰��ϰ� ����ϴ°� �����Դϴ�.
	// ����Ʈ�� Ʈ���� �������. 

	CEffect* pEffect = Load_Effect(LEVEL_STATIC, strAddPath, strFileName);

	pEffect->End_Effect_ForPool();		// ���� & ���� ���·� �����

	if (nullptr != pOwner)
		pEffect->Set_Object_Owner(pOwner);	// �θ� ���� (�θ� �ְ�, ����Ʈ�� bParentPivot�� True�̸� ���ʰ�ü�� ����ٴ�)


	if (bUseSocket)	// ���� ����̸� ���� ����
	{
		pEffect->Get_Desc()->bUseSocket = bUseSocket;
		pEffect->Get_Desc()->strBoneTag = strBoneTag;
	}

	pEffect->Get_Desc()->bPlay = true;
	pEffect->Set_Enable(true);


	return pEffect;
}

CEffect* CEffect_Manager::Create_Effect_ForDebug(string strAddPath, string strFileName, _float3 vPos, _bool bLookTarget, _float3 vTargetPos)
{
	// �� �Լ��� ����Ʈ�� �������� �ʴ°� �������Դϴ�. Ready_EffectPool()�� ����Ʈ�� �߰��ϰ� ����ϴ°� �����Դϴ�.
	// ����Ʈ�� Ʈ���� �������. 

	CEffect* pEffect = Load_Effect(LEVEL_STATIC, strAddPath, strFileName);

	pEffect->End_Effect_ForPool();		// ���� & ���� ���·� �����

	// ��ġ ����
	pEffect->Set_Position(vPos);

	if (bLookTarget)
	{
		// Ÿ���� �ٶ󺼰Ÿ� Look_At ���ֱ�
		_float4 vTargetPos_float4 = _float4(vTargetPos.x, vTargetPos.y, vTargetPos.z, 1.f);
		pEffect->Get_Transform()->Look_At(vTargetPos_float4);
	}

	pEffect->Get_Desc()->bPlay = true;
	pEffect->Set_Enable(true);

	return pEffect;

}

CEffect* CEffect_Manager::Create_Effect_ForDebug_StaticPivot(string strAddPath, string strFileName, CGameObject* pOwner, _float4x4 matPivot)
{

	// �� �Լ��� ����Ʈ�� �������� �ʴ°� �������Դϴ�. Ready_EffectPool()�� ����Ʈ�� �߰��ϰ� ����ϴ°� �����Դϴ�.
	// ����Ʈ�� Ʈ���� �������. 

	CEffect* pEffect = Load_Effect(LEVEL_STATIC, strAddPath, strFileName);

	pEffect->End_Effect_ForPool();		// ���� & ���� ���·� �����

	if (nullptr != pOwner)
		pEffect->Set_Object_Owner(pOwner);	// �θ� ���� (�θ� �ְ�, ����Ʈ�� bParentPivot�� True�̸� ���ʰ�ü�� ����ٴ�)


	pEffect->Get_Desc()->bAttachStatic = true;
	pEffect->Get_Desc()->matPivot = matPivot;

	pEffect->Get_Desc()->bPlay = true;
	pEffect->Set_Enable(true);


	return pEffect;
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

	if(nullptr != pOwner)
		pTrail->Set_Object_Owner(pOwner); // ���� ����

	pTrail->Load_FromJson(In_Json);

	return pTrail;

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


#pragma region �׽�Ʈ ����Ʈ ����

	/* Circle_Floor */
	FAILED_CHECK(Add_ToPool(iLevel, "Parasiter/", "Circle_Floor_03_Solid.json"));

	/* Test Explosion */
	FAILED_CHECK(Add_ToPool(iLevel, "Explosion/", "Explosion_05.json"));
	FAILED_CHECK(Add_ToPool(iLevel, "Explosion/", "Explosion_05_Big.json"));

#pragma endregion �׽�Ʈ ����Ʈ ��

	/* Light */
	FAILED_CHECK(Add_ToPool(iLevel, "Fire/", "Fire_Torch_05.json", 50));


#pragma region Hit ����Ʈ ����
	/* Hit */
	FAILED_CHECK(Add_ToPool(iLevel, "Hit/", "Hit_Distortion.json", 200));
	FAILED_CHECK(Add_ToPool(iLevel, "Hit/", "Hit_Normal.json", 200));
#pragma endregion Hit ����Ʈ ��

	
#pragma region ����1 ����Ʈ ����
	/* Boos 1 */
	FAILED_CHECK(Add_ToPool(iLevel, "VampireCommander/Map_Blood/", "Map_Blood_09.json"));

	FAILED_CHECK(Add_ToPool(iLevel, "VampireCommander/", "VampireCommanderAura_02.json")); 
	FAILED_CHECK(Add_ToPool(iLevel, "VampireCommander/BloodRange_Loop/", "BloodRange_Loop_22_Smoke.json"));

	//FAILED_CHECK(Add_ToPool(iLevel, "VampireCommander/Projectile_Range1/", "Projectile_Range1_04.json", 50));
	FAILED_CHECK(Add_ToPool(iLevel, "VampireCommander/Projectile_Range1/", "Projectile_Range1_Re_02.json", 200));


	FAILED_CHECK(Add_ToPool(iLevel, "VampireCommander/Projectile_Range3/", "Projectile_Pillar_Tick_10.json", 50));


	FAILED_CHECK(Add_ToPool(iLevel, "VampireCommander/", "landing_Rock_01.json", 2));
#pragma endregion ����1 ����Ʈ ��


#pragma region ����2 ����Ʈ ����
	/* Boos 2 */
	FAILED_CHECK(Add_ToPool(iLevel, "Parasiter/", "SY_Falling_Leaves_Map_05.json", 2));

	/* SnowBoss Falling Leaves */
	FAILED_CHECK(Add_ToPool(iLevel, "Parasiter/", "SY_Falling_Leaves_02.json", 30));

	/* Boss2 Monster_Explosion */
	FAILED_CHECK(Add_ToPool(iLevel, "Parasiter/", "Monster_ExplosionNonLoop.json", 50));
	FAILED_CHECK(Add_ToPool(iLevel, "Parasiter/", "Monster_Blood3.json", 50));


	FAILED_CHECK(Add_ToPool(iLevel, "Parasiter/", "Son_Test_07.json", 200));
	FAILED_CHECK(Add_ToPool(iLevel, "Parasiter/", "Son_ProjectilcTail.json", 800));
	
	/* Boss2 MotherShakeTreeProjectile */
	FAILED_CHECK(Add_ToPool(iLevel, "Parasiter/MotherShakeTree/", "Circle_Floor_05.json", 200));
	FAILED_CHECK(Add_ToPool(iLevel, "Parasiter/MotherShakeTree/", "MotherProjectileDead_08.json", 200));
	FAILED_CHECK(Add_ToPool(iLevel, "Parasiter/MotherShakeTree/", "MotherShakeTreeProjectile1.json", 200));

	//Mother_Egg
	FAILED_CHECK(Add_ToPool(iLevel, "Parasiter/", "Egg_Dead3.json", 10));

	/* Mother Breath */
	FAILED_CHECK(Add_ToPool(iLevel, "Parasiter/", "Mother_breath4.json", 500));
	FAILED_CHECK(Add_ToPool(iLevel, "Parasiter/Mother_Breath/", "Mother_Breath_08.json", 400));
	FAILED_CHECK(Add_ToPool(iLevel, "Parasiter/Mother_Breath/", "Mother_Breath_08_Tick.json", 1000));
	//FAILED_CHECK(Add_ToPool(iLevel, "Parasiter/Mother_Breath/", "Mother_Breath_08_02.json", 400));
	FAILED_CHECK(Add_ToPool(iLevel, "Parasiter/Mother_Breath/", "Mother_Breath_Start_Circle_03.json", 2));

#pragma endregion ����2 ����Ʈ ��


#pragma region �÷��̾� ����Ʈ ����
	/* Breath */
	FAILED_CHECK(Add_ToPool(iLevel, "Player/Breath/", "SY_Player_Breath04.json", 2));

	/* Heal */
	FAILED_CHECK(Add_ToPool(iLevel, "Player/Heal/", "Heal_08.json", 2));
	FAILED_CHECK(Add_ToPool(iLevel, "Player/Heal/", "Heal_Particle_07_Reverse.json", 2));
	FAILED_CHECK(Add_ToPool(iLevel, "Player/Heal/", "Heal_07_Light_03.json", 2));
	FAILED_CHECK(Add_ToPool(iLevel, "Player/Heal/", "Heal_Particle_07.json", 2));

	/* Heal_Blue */
	FAILED_CHECK(Add_ToPool(iLevel, "Player/Heal_Blue/", "Heal_08_Blue.json", 2));
	FAILED_CHECK(Add_ToPool(iLevel, "Player/Heal_Blue/", "Heal_Particle_07_Reverse_Blue.json", 2));
	FAILED_CHECK(Add_ToPool(iLevel, "Player/Heal_Blue/", "Heal_07_Light_03_Blue.json", 2));
	FAILED_CHECK(Add_ToPool(iLevel, "Player/Heal_Blue/", "Heal_Particle_07_Blue.json", 2));

	/* EnergyWhip */
	FAILED_CHECK(Add_ToPool(iLevel, "Player/Zapper_Shield/", "Zapper_Shield_22_distortionTest.json", 2));
	FAILED_CHECK(Add_ToPool(iLevel, "Player/Zapper_Dash/", "Zapper_Dash_31.json", 2));

	/* SlamDown */
	FAILED_CHECK(Add_ToPool(iLevel, "Player/SlamDown/", "SlamDown_v1_03_Rock.json", 2));
	FAILED_CHECK(Add_ToPool(iLevel, "Player/SlamDown/", "SlamDown_v2_24_Rock.json", 2));
	FAILED_CHECK(Add_ToPool(iLevel, "Player/SlamDown/", "SlamDown_v2_26_Rock.json", 2));

	/* DodgeBlink */
	FAILED_CHECK(Add_ToPool(iLevel, "Player/DodgeBlink/", "DodgeBlink_L_18.json", 2));
	FAILED_CHECK(Add_ToPool(iLevel, "Player/DodgeBlink/", "DodgeBlink_R_18.json", 2));

	/* Roll */
	FAILED_CHECK(Add_ToPool(iLevel, "Player/Roll/", "Roll_R_04.json", 2));
	FAILED_CHECK(Add_ToPool(iLevel, "Player/Roll/", "Roll_R_04.json", 2));

	/* Revolver */
	FAILED_CHECK(Add_ToPool(iLevel, "Player/Revolver/", "Revolver_13.json", 10));
	FAILED_CHECK(Add_ToPool(iLevel, "Player/Revolver/", "Revolver_13_Tail_01.json", 10));

	/* Revolver_Fire */
	FAILED_CHECK(Add_ToPool(iLevel, "Player/Revolver_Fire/", "Revolver_Fire_03.json", 10));
	//FAILED_CHECK(Add_ToPool(iLevel, "Player/Revolver_Fire/", "Revolver_Fire_02_Tail.json", 10));
#pragma endregion �÷��̾� ����Ʈ ��

	

#pragma region �ʿ� �������� �򸮴� ����Ʈ

		FAILED_CHECK(Add_ToPool(iLevel, "Fog/", "SY_SnowMap.json", 1));

#pragma endregion // �ʿ� �������� �򸮴� ����Ʈ

	return S_OK;
}



HRESULT CEffect_Manager::Add_ToPool(_uint iLevelIndex, string strAddPath, string strFileName, _uint iCount, _bool bHasTrail, string strTrailFileName)
{
	for (_uint i = 0; i < iCount; ++i)
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

		Safe_AddRef(pEffect);
	}

	return S_OK;
}

HRESULT CEffect_Manager::Return_ToPool(CEffect* pEffect)
{
	if (pEffect == nullptr)
	{
//#ifdef _DEBUG
		MSG_BOX("nullptr : CEffect_Manager::Return_ToPool()");
		return S_OK;
//#endif // _DEBUG
	}

	pEffect->End_Effect_ForPool();

	string strTempFileName = pEffect->Get_Desc()->strFileName;
	auto iter = m_EffectPool.find(strTempFileName);
	if (iter != m_EffectPool.end())
	{
		iter->second.push(pEffect);		//m_EffectPool[strTempFileName].push(pEffect);
	}

	//Safe_Release(pEffect);

	return S_OK;
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
			//iter.second.front()->Set_Dead(true);
			iter.second.front()->Set_Enable(true); // ���̾��� Free���� ������ ���� true.(������ �� ������Ʈ �Ŵ����� Ǯ�� �ȳ־��� ������)
			Safe_Release(iter.second.front());
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
