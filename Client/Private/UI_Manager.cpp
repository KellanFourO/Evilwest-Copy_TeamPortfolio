#include "stdafx.h"
#include "Clone_Manager.h"

#include "GameInstance.h"

#include "..\Public\UI_Manager.h"

#pragma region UI
#include "UI_EnemyHP_Bar.h"
#include "UI_EnemyHP_Shard.h"
#include "UI_Distortion.h"
#pragma endregion

IMPLEMENT_SINGLETON(CUI_Manager);

CUI_Manager::CUI_Manager()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);


}

HRESULT CUI_Manager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	if (nullptr == pDevice || nullptr == pContext)
		return E_FAIL;

	m_pDevice = pDevice;
	Safe_AddRef(m_pDevice);

	m_pContext = pContext;
	Safe_AddRef(m_pContext);

	return S_OK;
}

void CUI_Manager::Tick(_float fTimeDelta)
{ 
	Check_Active(fTimeDelta);
}

// 플레이 화면의 모든 UI생성
HRESULT CUI_Manager::Ready_Interface(_uint iLevelIndex)
{
	Add_LeftHUD(iLevelIndex, TEXT("Layer_LeftHUD"));
	Add_RightHUD(iLevelIndex, TEXT("Layer_RightHUD"));
	Add_TutorialBox(iLevelIndex, TEXT("Layer_TutorialBox"));
	//Add_LevelUp(iLevelIndex, TEXT("Layer_LevelUp"));
	Add_RewardBox(iLevelIndex, TEXT("Layer_RewardBox"));
	Add_QuestBox(iLevelIndex, TEXT("Layer_QuestBox"));

	return S_OK;
}

HRESULT CUI_Manager::Ready_Crosshair(_uint iLevelIndex)
{
	Add_Crosshair(iLevelIndex, TEXT("Layer_Crosshair"));

	return S_OK;
}

HRESULT CUI_Manager::Ready_Loading_Intro(_uint iLevelIndex)
{
	Add_Loading_Intro(iLevelIndex, TEXT("Layer_IntroLoading"));

	return S_OK;
}

HRESULT CUI_Manager::Ready_Loading_IntroBoss(_uint iLevelIndex)
{
	Add_Loading_IntroBoss(iLevelIndex, TEXT("Layer_IntroBossLoading"));

	return S_OK;
}

HRESULT CUI_Manager::Ready_BossHUD_Bar(_uint iLevelIndex, CGameObject* pOwner)
{
	Add_BossHUD_Bar(iLevelIndex, TEXT("Layer_BossHUDBar"), pOwner);

	return S_OK;
}

HRESULT CUI_Manager::Ready_EnemyHUD_Shard(_uint iLevelIndex, CGameObject* pOwner)
{
	Add_EnemyHUD_Shard(iLevelIndex, TEXT("Layer_EnemyHUDShard"), pOwner);
	return S_OK;
}

HRESULT CUI_Manager::Ready_DiedScreen(_uint iLevelIndex)
{
	Add_DiedScreen(iLevelIndex, TEXT("Layer_DiedScreen"));
	return S_OK;
}

HRESULT CUI_Manager::Ready_Option(_uint iLevelIndex)
{
	Add_Option(iLevelIndex, TEXT("Layer_Option"));
	return S_OK;
}

HRESULT CUI_Manager::Add_LeftHUD(_uint iLevelIndex, const wstring& strLayerTag)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/PlayerInterface/Left_Interface.json";

	CJson_Utility::Load_Json(strFile.c_str(), json_in);

	for (auto& item : json_in.items())
	{
		json object = item.value();

		CUI::UI_DESC tUI_Info;

		/* 저장순서랑 맞는지 확인하기 */
		if (object.contains("Parent"))
			tUI_Info.bParent = object["Parent"];					// 1. Parent
		if (object.contains("World"))
			tUI_Info.bWorld = object["World"];						// 2. World
		if (object.contains("Group"))
			tUI_Info.bGroup = object["Group"];						// 3. Group
		if (object.contains("Alpha"))
			tUI_Info.fAlpha = object["Alpha"];						// 4. Alpha
		if (object.contains("ObjectNum"))
			tUI_Info.iObjectNum = object["ObjectNum"];				// 5. ObjectNum
		if (object.contains("ShaderNum"))
			tUI_Info.iShaderNum = object["ShaderNum"];				// 6. ShaderPathNum
		if (object.contains("UINum"))								// "ObjectName" 키가 있으면
			tUI_Info.iUINum = object["UINum"];
		if (object.contains("UIName"))								// "ObjectName" 키가 있으면
			tUI_Info.strUIName = object["UIName"];
		if (object.contains("ObjectName"))							// "ObjectName" 키가 있으면
			tUI_Info.strObjectName = object["ObjectName"];			// 7. ObjectName
		if (object.contains("LayerTag"))
			tUI_Info.strLayerTag = object["LayerTag"];				// 8. LayerTag
		if (object.contains("CloneTag"))
			tUI_Info.strCloneTag = object["CloneTag"];				// 9. CloneTag
		if (object.contains("ProtoTag"))
			tUI_Info.strProtoTag = object["ProtoTag"];				// 10. ProtoTag
		if (object.contains("FilePath"))
			tUI_Info.strFilePath = object["FilePath"];				// 11. FilePath
		if (object.contains("MapTextureTag"))
			tUI_Info.strMapTextureTag = object["MapTextureTag"];	// 12. MapTexture
		if (object.contains("ColorR"))
			tUI_Info.vColor.m128_f32[0] = object["ColorR"];			// 13. R
		if (object.contains("ColorG"))
			tUI_Info.vColor.m128_f32[1] = object["ColorG"];			// 14. G
		if (object.contains("ColorB"))
			tUI_Info.vColor.m128_f32[2] = object["ColorB"];			// 15. B
		if (object.contains("ColorA"))
			tUI_Info.vColor.m128_f32[3] = object["ColorA"];			// 16. A


		wstring wstrClonetag;
		m_pGameInstance->String_To_WString(tUI_Info.strCloneTag, wstrClonetag);

		wstring wstrPrototag;
		m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

		wstring wstrFilePath;
		m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

		CGameObject* pGameObject = m_pGameInstance->Add_CloneObject_And_Get(iLevelIndex, strLayerTag, wstrClonetag, &tUI_Info);
		if (pGameObject == nullptr)
			return E_FAIL;

		CUI* pUI_Object = dynamic_cast<CUI*>(pGameObject);
		if (pUI_Object == nullptr)
			return E_FAIL;

		m_vecLeftHUD.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}

	Active_LeftHUD(); // 생성후 최초 한번 켜주기

	return S_OK;
}

void CUI_Manager::Active_LeftHUD()
{
	if (m_vecLeftHUD.empty())
		return;

	for (auto& iter : m_vecLeftHUD)
	{
		iter->Set_Alpha(0.f);		// UI 알파값 초기화
		iter->Set_Active(true);		// UI 활성화
		iter->Set_AnimPlay(true);	// UI Animation 재생
		iter->Set_Disappear(false); // UI 사라짐 Off
	}
}

void CUI_Manager::NonActive_LeftHUD()
{
	if (m_vecLeftHUD.empty())
		return;

	for (auto& iter : m_vecLeftHUD)
	{
		iter->Set_Alpha(1.f);		// UI 알파값 초기화
		iter->Set_Active(false);		// UI 활성화
		iter->Set_AnimPlay(false);	// UI Animation 재생
		iter->Set_Disappear(true); // UI 사라짐 Off
	}
}

void CUI_Manager::NonActive_RightHUD()
{
	if (m_vecRightHUD.empty())
		return;

	for (auto& iter : m_vecRightHUD)
	{
		iter->Set_Alpha(1.f);		// UI 알파값 초기화
		iter->Set_Active(false);		// UI 활성화
		iter->Set_AnimPlay(false);	// UI Animation 재생
		iter->Set_Disappear(true); // UI 사라짐 Off
	}
}

HRESULT CUI_Manager::Add_RightHUD(_uint iLevelIndex, const wstring& strLayerTag)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/PlayerInterface/Right_Interface.json";

	CJson_Utility::Load_Json(strFile.c_str(), json_in);

	for (auto& item : json_in.items())
	{
		json object = item.value();

		CUI::UI_DESC tUI_Info;

		/* 저장순서랑 맞는지 확인하기 */
		if (object.contains("Parent"))
			tUI_Info.bParent = object["Parent"];					// 1. Parent
		if (object.contains("World"))
			tUI_Info.bWorld = object["World"];						// 2. World
		if (object.contains("Group"))
			tUI_Info.bGroup = object["Group"];						// 3. Group
		if (object.contains("Alpha"))
			tUI_Info.fAlpha = object["Alpha"];						// 4. Alpha
		if (object.contains("ObjectNum"))
			tUI_Info.iObjectNum = object["ObjectNum"];				// 5. ObjectNum
		if (object.contains("ShaderNum"))
			tUI_Info.iShaderNum = object["ShaderNum"];				// 6. ShaderPathNum
		if (object.contains("UINum"))								// "ObjectName" 키가 있으면
			tUI_Info.iUINum = object["UINum"];
		if (object.contains("UIName"))								// "ObjectName" 키가 있으면
			tUI_Info.strUIName = object["UIName"];
		if (object.contains("ObjectName"))							// "ObjectName" 키가 있으면
			tUI_Info.strObjectName = object["ObjectName"];			// 7. ObjectName
		if (object.contains("LayerTag"))
			tUI_Info.strLayerTag = object["LayerTag"];				// 8. LayerTag
		if (object.contains("CloneTag"))
			tUI_Info.strCloneTag = object["CloneTag"];				// 9. CloneTag
		if (object.contains("ProtoTag"))
			tUI_Info.strProtoTag = object["ProtoTag"];				// 10. ProtoTag
		if (object.contains("FilePath"))
			tUI_Info.strFilePath = object["FilePath"];				// 11. FilePath
		if (object.contains("MapTextureTag"))
			tUI_Info.strMapTextureTag = object["MapTextureTag"];	// 12. MapTexture
		if (object.contains("ColorR"))
			tUI_Info.vColor.m128_f32[0] = object["ColorR"];			// 13. R
		if (object.contains("ColorG"))
			tUI_Info.vColor.m128_f32[1] = object["ColorG"];			// 14. G
		if (object.contains("ColorB"))
			tUI_Info.vColor.m128_f32[2] = object["ColorB"];			// 15. B
		if (object.contains("ColorA"))
			tUI_Info.vColor.m128_f32[3] = object["ColorA"];			// 16. A


		wstring wstrClonetag;
		m_pGameInstance->String_To_WString(tUI_Info.strCloneTag, wstrClonetag);

		wstring wstrPrototag;
		m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

		wstring wstrFilePath;
		m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

		CGameObject* pGameObject = m_pGameInstance->Add_CloneObject_And_Get(iLevelIndex, strLayerTag, wstrClonetag, &tUI_Info);
		if (pGameObject == nullptr)
			return E_FAIL;

		CUI* pUI_Object = dynamic_cast<CUI*>(pGameObject);
		if (pUI_Object == nullptr)
			return E_FAIL;

		m_vecRightHUD.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}

	Active_RightHUD(); // 생성후 최초 한번 켜주기

	return S_OK;
}

void CUI_Manager::Active_RightHUD()
{
	if (m_vecRightHUD.empty())
		return;

	for (auto& iter : m_vecRightHUD)
	{
		iter->Set_Alpha(0.f);		// UI 알파값 초기화
		iter->Set_Active(true);		// UI 활성화
		iter->Set_AnimPlay(true);	// UI Animation 재생
		iter->Set_Disappear(false); // UI 사라짐 Off
	}
}

HRESULT CUI_Manager::Add_TutorialBox(_uint iLevelIndex, const wstring& strLayerTag)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/PlayerInterface/TutorialBox.json";

	CJson_Utility::Load_Json(strFile.c_str(), json_in);

	for (auto& item : json_in.items())
	{
		json object = item.value();

		CUI::UI_DESC tUI_Info;

		/* 저장순서랑 맞는지 확인하기 */
		if (object.contains("Parent"))
			tUI_Info.bParent = object["Parent"];					// 1. Parent
		if (object.contains("World"))
			tUI_Info.bWorld = object["World"];						// 2. World
		if (object.contains("Group"))
			tUI_Info.bGroup = object["Group"];						// 3. Group
		if (object.contains("Alpha"))
			tUI_Info.fAlpha = object["Alpha"];						// 4. Alpha
		if (object.contains("ObjectNum"))
			tUI_Info.iObjectNum = object["ObjectNum"];				// 5. ObjectNum
		if (object.contains("ShaderNum"))
			tUI_Info.iShaderNum = object["ShaderNum"];				// 6. ShaderPathNum
		if (object.contains("UINum"))								// "ObjectName" 키가 있으면
			tUI_Info.iUINum = object["UINum"];
		if (object.contains("UIName"))								// "ObjectName" 키가 있으면
			tUI_Info.strUIName = object["UIName"];
		if (object.contains("ObjectName"))							// "ObjectName" 키가 있으면
			tUI_Info.strObjectName = object["ObjectName"];			// 7. ObjectName
		if (object.contains("LayerTag"))
			tUI_Info.strLayerTag = object["LayerTag"];				// 8. LayerTag
		if (object.contains("CloneTag"))
			tUI_Info.strCloneTag = object["CloneTag"];				// 9. CloneTag
		if (object.contains("ProtoTag"))
			tUI_Info.strProtoTag = object["ProtoTag"];				// 10. ProtoTag
		if (object.contains("FilePath"))
			tUI_Info.strFilePath = object["FilePath"];				// 11. FilePath
		if (object.contains("MapTextureTag"))
			tUI_Info.strMapTextureTag = object["MapTextureTag"];	// 12. MapTexture
		if (object.contains("ColorR"))
			tUI_Info.vColor.m128_f32[0] = object["ColorR"];			// 13. R
		if (object.contains("ColorG"))
			tUI_Info.vColor.m128_f32[1] = object["ColorG"];			// 14. G
		if (object.contains("ColorB"))
			tUI_Info.vColor.m128_f32[2] = object["ColorB"];			// 15. B
		if (object.contains("ColorA"))
			tUI_Info.vColor.m128_f32[3] = object["ColorA"];			// 16. A


		wstring wstrClonetag;
		m_pGameInstance->String_To_WString(tUI_Info.strCloneTag, wstrClonetag);

		wstring wstrPrototag;
		m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

		wstring wstrFilePath;
		m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

		CGameObject* pGameObject = m_pGameInstance->Add_CloneObject_And_Get(iLevelIndex, strLayerTag, wstrClonetag, &tUI_Info);
		if (pGameObject == nullptr)
			return E_FAIL;

		CUI* pUI_Object = dynamic_cast<CUI*>(pGameObject);
		if (pUI_Object == nullptr)
			return E_FAIL;

		m_vecTutorialBox.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}

	return S_OK;
}

void CUI_Manager::Active_TutorialBox()
{
	if (m_vecTutorialBox.empty())
		return;

	for (auto& iter : m_vecTutorialBox)
	{
		iter->Set_Alpha(0.f);		// UI 알파값 초기화
		iter->Set_Active(true);		// UI 활성화
		iter->Set_AnimPlay(true);	// UI Animation 재생
		iter->Set_Disappear(false); // UI 사라짐 Off
	}
}

HRESULT CUI_Manager::Add_LevelUp(_uint iLevelIndex, const wstring& strLayerTag)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/PlayerInterface/LevelUp.json";

	CJson_Utility::Load_Json(strFile.c_str(), json_in);

	for (auto& item : json_in.items())
	{
		json object = item.value();

		CUI::UI_DESC tUI_Info;

		/* 저장순서랑 맞는지 확인하기 */
		if (object.contains("Parent"))
			tUI_Info.bParent = object["Parent"];					// 1. Parent
		if (object.contains("World"))
			tUI_Info.bWorld = object["World"];						// 2. World
		if (object.contains("Group"))
			tUI_Info.bGroup = object["Group"];						// 3. Group
		if (object.contains("Alpha"))
			tUI_Info.fAlpha = object["Alpha"];						// 4. Alpha
		if (object.contains("ObjectNum"))
			tUI_Info.iObjectNum = object["ObjectNum"];				// 5. ObjectNum
		if (object.contains("ShaderNum"))
			tUI_Info.iShaderNum = object["ShaderNum"];				// 6. ShaderPathNum
		if (object.contains("UINum"))								// "ObjectName" 키가 있으면
			tUI_Info.iUINum = object["UINum"];
		if (object.contains("UIName"))								// "ObjectName" 키가 있으면
			tUI_Info.strUIName = object["UIName"];
		if (object.contains("ObjectName"))							// "ObjectName" 키가 있으면
			tUI_Info.strObjectName = object["ObjectName"];			// 7. ObjectName
		if (object.contains("LayerTag"))
			tUI_Info.strLayerTag = object["LayerTag"];				// 8. LayerTag
		if (object.contains("CloneTag"))
			tUI_Info.strCloneTag = object["CloneTag"];				// 9. CloneTag
		if (object.contains("ProtoTag"))
			tUI_Info.strProtoTag = object["ProtoTag"];				// 10. ProtoTag
		if (object.contains("FilePath"))
			tUI_Info.strFilePath = object["FilePath"];				// 11. FilePath
		if (object.contains("MapTextureTag"))
			tUI_Info.strMapTextureTag = object["MapTextureTag"];	// 12. MapTexture
		if (object.contains("ColorR"))
			tUI_Info.vColor.m128_f32[0] = object["ColorR"];			// 13. R
		if (object.contains("ColorG"))
			tUI_Info.vColor.m128_f32[1] = object["ColorG"];			// 14. G
		if (object.contains("ColorB"))
			tUI_Info.vColor.m128_f32[2] = object["ColorB"];			// 15. B
		if (object.contains("ColorA"))
			tUI_Info.vColor.m128_f32[3] = object["ColorA"];			// 16. A


		wstring wstrClonetag;
		m_pGameInstance->String_To_WString(tUI_Info.strCloneTag, wstrClonetag);

		wstring wstrPrototag;
		m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

		wstring wstrFilePath;
		m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

		CGameObject* pGameObject = m_pGameInstance->Add_CloneObject_And_Get(iLevelIndex, strLayerTag, wstrClonetag, &tUI_Info);
		if (pGameObject == nullptr)
			return E_FAIL;

		CUI* pUI_Object = dynamic_cast<CUI*>(pGameObject);
		if (pUI_Object == nullptr)
			return E_FAIL;

		m_vecLevelUP.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}

	return S_OK;
}

void CUI_Manager::Active_LevelUp()
{
	if (m_vecLevelUP.empty())
		return;

	for (auto& iter : m_vecLevelUP)
	{
		iter->Set_Alpha(0.f);		// UI 알파값 초기화
		iter->Set_Active(true);		// UI 활성화
		iter->Set_AnimPlay(true);	// UI Animation 재생
		iter->Set_Disappear(false); // UI 사라짐 Off
	}
}

HRESULT CUI_Manager::Add_RewardBox(_uint iLevelIndex, const wstring& strLayerTag)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/PlayerInterface/RewardItem.json";

	CJson_Utility::Load_Json(strFile.c_str(), json_in);

	for (auto& item : json_in.items())
	{
		json object = item.value();

		CUI::UI_DESC tUI_Info;

		/* 저장순서랑 맞는지 확인하기 */
		if (object.contains("Parent"))
			tUI_Info.bParent = object["Parent"];					// 1. Parent
		if (object.contains("World"))
			tUI_Info.bWorld = object["World"];						// 2. World
		if (object.contains("Group"))
			tUI_Info.bGroup = object["Group"];						// 3. Group
		if (object.contains("Alpha"))
			tUI_Info.fAlpha = object["Alpha"];						// 4. Alpha
		if (object.contains("ObjectNum"))
			tUI_Info.iObjectNum = object["ObjectNum"];				// 5. ObjectNum
		if (object.contains("ShaderNum"))
			tUI_Info.iShaderNum = object["ShaderNum"];				// 6. ShaderPathNum
		if (object.contains("UINum"))								// "ObjectName" 키가 있으면
			tUI_Info.iUINum = object["UINum"];
		if (object.contains("UIName"))								// "ObjectName" 키가 있으면
			tUI_Info.strUIName = object["UIName"];
		if (object.contains("ObjectName"))							// "ObjectName" 키가 있으면
			tUI_Info.strObjectName = object["ObjectName"];			// 7. ObjectName
		if (object.contains("LayerTag"))
			tUI_Info.strLayerTag = object["LayerTag"];				// 8. LayerTag
		if (object.contains("CloneTag"))
			tUI_Info.strCloneTag = object["CloneTag"];				// 9. CloneTag
		if (object.contains("ProtoTag"))
			tUI_Info.strProtoTag = object["ProtoTag"];				// 10. ProtoTag
		if (object.contains("FilePath"))
			tUI_Info.strFilePath = object["FilePath"];				// 11. FilePath
		if (object.contains("MapTextureTag"))
			tUI_Info.strMapTextureTag = object["MapTextureTag"];	// 12. MapTexture
		if (object.contains("ColorR"))
			tUI_Info.vColor.m128_f32[0] = object["ColorR"];			// 13. R
		if (object.contains("ColorG"))
			tUI_Info.vColor.m128_f32[1] = object["ColorG"];			// 14. G
		if (object.contains("ColorB"))
			tUI_Info.vColor.m128_f32[2] = object["ColorB"];			// 15. B
		if (object.contains("ColorA"))
			tUI_Info.vColor.m128_f32[3] = object["ColorA"];			// 16. A


		wstring wstrClonetag;
		m_pGameInstance->String_To_WString(tUI_Info.strCloneTag, wstrClonetag);

		wstring wstrPrototag;
		m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

		wstring wstrFilePath;
		m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

		CGameObject* pGameObject = m_pGameInstance->Add_CloneObject_And_Get(iLevelIndex, strLayerTag, wstrClonetag, &tUI_Info);
		if (pGameObject == nullptr)
			return E_FAIL;

		CUI* pUI_Object = dynamic_cast<CUI*>(pGameObject);
		if (pUI_Object == nullptr)
			return E_FAIL;

		m_vecRewardBox.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}

	return S_OK;
}

void CUI_Manager::Active_RewardBox()
{
	if (m_vecRewardBox.empty())
		return;

	for (auto& iter : m_vecRewardBox)
	{
		iter->Set_Alpha(0.f);		// UI 알파값 초기화
		iter->Set_Active(true);		// UI 활성화
		iter->Set_AnimPlay(true);	// UI Animation 재생
		iter->Set_Disappear(false); // UI 사라짐 Off
	}
}

HRESULT CUI_Manager::Add_QuestBox(_uint iLevelIndex, const wstring& strLayerTag)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/PlayerInterface/QuestBox.json";

	CJson_Utility::Load_Json(strFile.c_str(), json_in);

	for (auto& item : json_in.items())
	{
		json object = item.value();

		CUI::UI_DESC tUI_Info;

		/* 저장순서랑 맞는지 확인하기 */
		if (object.contains("Parent"))
			tUI_Info.bParent = object["Parent"];					// 1. Parent
		if (object.contains("World"))
			tUI_Info.bWorld = object["World"];						// 2. World
		if (object.contains("Group"))
			tUI_Info.bGroup = object["Group"];						// 3. Group
		if (object.contains("Alpha"))
			tUI_Info.fAlpha = object["Alpha"];						// 4. Alpha
		if (object.contains("ObjectNum"))
			tUI_Info.iObjectNum = object["ObjectNum"];				// 5. ObjectNum
		if (object.contains("ShaderNum"))
			tUI_Info.iShaderNum = object["ShaderNum"];				// 6. ShaderPathNum
		if (object.contains("UINum"))								// "ObjectName" 키가 있으면
			tUI_Info.iUINum = object["UINum"];
		if (object.contains("UIName"))								// "ObjectName" 키가 있으면
			tUI_Info.strUIName = object["UIName"];
		if (object.contains("ObjectName"))							// "ObjectName" 키가 있으면
			tUI_Info.strObjectName = object["ObjectName"];			// 7. ObjectName
		if (object.contains("LayerTag"))
			tUI_Info.strLayerTag = object["LayerTag"];				// 8. LayerTag
		if (object.contains("CloneTag"))
			tUI_Info.strCloneTag = object["CloneTag"];				// 9. CloneTag
		if (object.contains("ProtoTag"))
			tUI_Info.strProtoTag = object["ProtoTag"];				// 10. ProtoTag
		if (object.contains("FilePath"))
			tUI_Info.strFilePath = object["FilePath"];				// 11. FilePath
		if (object.contains("MapTextureTag"))
			tUI_Info.strMapTextureTag = object["MapTextureTag"];	// 12. MapTexture
		if (object.contains("ColorR"))
			tUI_Info.vColor.m128_f32[0] = object["ColorR"];			// 13. R
		if (object.contains("ColorG"))
			tUI_Info.vColor.m128_f32[1] = object["ColorG"];			// 14. G
		if (object.contains("ColorB"))
			tUI_Info.vColor.m128_f32[2] = object["ColorB"];			// 15. B
		if (object.contains("ColorA"))
			tUI_Info.vColor.m128_f32[3] = object["ColorA"];			// 16. A


		wstring wstrClonetag;
		m_pGameInstance->String_To_WString(tUI_Info.strCloneTag, wstrClonetag);

		wstring wstrPrototag;
		m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

		wstring wstrFilePath;
		m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

		CGameObject* pGameObject = m_pGameInstance->Add_CloneObject_And_Get(iLevelIndex, strLayerTag, wstrClonetag, &tUI_Info);
		if (pGameObject == nullptr)
			return E_FAIL;

		CUI* pUI_Object = dynamic_cast<CUI*>(pGameObject);
		if (pUI_Object == nullptr)
			return E_FAIL;

		m_vecQuestBox.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}

	return S_OK;
}

void CUI_Manager::Active_QuestBox()
{
	if (m_vecQuestBox.empty())
		return;

	for (auto& iter : m_vecQuestBox)
	{
		iter->Set_Alpha(0.f);		// UI 알파값 초기화
		iter->Set_Active(true);		// UI 활성화
		iter->Set_AnimPlay(true);	// UI Animation 재생
		iter->Set_Disappear(false); // UI 사라짐 Off
	}
}

HRESULT CUI_Manager::Add_Distortion(_uint iLevelIndex, const wstring& strLayerTag)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/PlayerInterface/DistortionTest.json";

	CJson_Utility::Load_Json(strFile.c_str(), json_in);

	for (auto& item : json_in.items())
	{
		json object = item.value();

		CUI::UI_DESC tUI_Info;

		/* 저장순서랑 맞는지 확인하기 */
		if (object.contains("Parent"))
			tUI_Info.bParent = object["Parent"];					// 1. Parent
		if (object.contains("World"))
			tUI_Info.bWorld = object["World"];						// 2. World
		if (object.contains("Group"))
			tUI_Info.bGroup = object["Group"];						// 3. Group
		if (object.contains("Alpha"))
			tUI_Info.fAlpha = object["Alpha"];						// 4. Alpha
		if (object.contains("ObjectNum"))
			tUI_Info.iObjectNum = object["ObjectNum"];				// 5. ObjectNum
		if (object.contains("ShaderNum"))
			tUI_Info.iShaderNum = object["ShaderNum"];				// 6. ShaderPathNum
		if (object.contains("UINum"))								// "ObjectName" 키가 있으면
			tUI_Info.iUINum = object["UINum"];
		if (object.contains("UIName"))								// "ObjectName" 키가 있으면
			tUI_Info.strUIName = object["UIName"];
		if (object.contains("ObjectName"))							// "ObjectName" 키가 있으면
			tUI_Info.strObjectName = object["ObjectName"];			// 7. ObjectName
		if (object.contains("LayerTag"))
			tUI_Info.strLayerTag = object["LayerTag"];				// 8. LayerTag
		if (object.contains("CloneTag"))
			tUI_Info.strCloneTag = object["CloneTag"];				// 9. CloneTag
		if (object.contains("ProtoTag"))
			tUI_Info.strProtoTag = object["ProtoTag"];				// 10. ProtoTag
		if (object.contains("FilePath"))
			tUI_Info.strFilePath = object["FilePath"];				// 11. FilePath
		if (object.contains("MapTextureTag"))
			tUI_Info.strMapTextureTag = object["MapTextureTag"];	// 12. MapTexture
		if (object.contains("ColorR"))
			tUI_Info.vColor.m128_f32[0] = object["ColorR"];			// 13. R
		if (object.contains("ColorG"))
			tUI_Info.vColor.m128_f32[1] = object["ColorG"];			// 14. G
		if (object.contains("ColorB"))
			tUI_Info.vColor.m128_f32[2] = object["ColorB"];			// 15. B
		if (object.contains("ColorA"))
			tUI_Info.vColor.m128_f32[3] = object["ColorA"];			// 16. A


		wstring wstrClonetag;
		m_pGameInstance->String_To_WString(tUI_Info.strCloneTag, wstrClonetag);

		wstring wstrPrototag;
		m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

		wstring wstrFilePath;
		m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

		CGameObject* pGameObject = m_pGameInstance->Add_CloneObject_And_Get(iLevelIndex, strLayerTag, wstrClonetag, &tUI_Info);
		if (pGameObject == nullptr)
			return E_FAIL;

		CUI* pUI_Object = dynamic_cast<CUI*>(pGameObject);
		if (pUI_Object == nullptr)
			return E_FAIL;

		m_vecDistortion.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}

	return S_OK;
}

void CUI_Manager::Active_Distortion()
{
	if (m_vecDistortion.empty())
		return;

	for (auto& iter : m_vecDistortion)
	{
		iter->Set_Alpha(0.f);		// UI 알파값 초기화
		iter->Set_Active(true);		// UI 활성화
		iter->Set_AnimPlay(true);	// UI Animation 재생
		iter->Set_Disappear(false); // UI 사라짐 Off
	}
}

HRESULT CUI_Manager::Add_Loading_Intro(_uint iLevelIndex, const wstring& strLayerTag)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/Loading/IntroLoading.json";

	CJson_Utility::Load_Json(strFile.c_str(), json_in);

	for (auto& item : json_in.items())
	{
		json object = item.value();

		CUI::UI_DESC tUI_Info;

		/* 저장순서랑 맞는지 확인하기 */
		if (object.contains("Parent"))
			tUI_Info.bParent = object["Parent"];					// 1. Parent
		if (object.contains("World"))
			tUI_Info.bWorld = object["World"];						// 2. World
		if (object.contains("Group"))
			tUI_Info.bGroup = object["Group"];						// 3. Group
		if (object.contains("Alpha"))
			tUI_Info.fAlpha = object["Alpha"];						// 4. Alpha
		if (object.contains("ObjectNum"))
			tUI_Info.iObjectNum = object["ObjectNum"];				// 5. ObjectNum
		if (object.contains("ShaderNum"))
			tUI_Info.iShaderNum = object["ShaderNum"];				// 6. ShaderPathNum
		if (object.contains("UINum"))								// "ObjectName" 키가 있으면
			tUI_Info.iUINum = object["UINum"];
		if (object.contains("UIName"))								// "ObjectName" 키가 있으면
			tUI_Info.strUIName = object["UIName"];
		if (object.contains("ObjectName"))							// "ObjectName" 키가 있으면
			tUI_Info.strObjectName = object["ObjectName"];			// 7. ObjectName
		if (object.contains("LayerTag"))
			tUI_Info.strLayerTag = object["LayerTag"];				// 8. LayerTag
		if (object.contains("CloneTag"))
			tUI_Info.strCloneTag = object["CloneTag"];				// 9. CloneTag
		if (object.contains("ProtoTag"))
			tUI_Info.strProtoTag = object["ProtoTag"];				// 10. ProtoTag
		if (object.contains("FilePath"))
			tUI_Info.strFilePath = object["FilePath"];				// 11. FilePath
		if (object.contains("MapTextureTag"))
			tUI_Info.strMapTextureTag = object["MapTextureTag"];	// 12. MapTexture
		if (object.contains("ColorR"))
			tUI_Info.vColor.m128_f32[0] = object["ColorR"];			// 13. R
		if (object.contains("ColorG"))
			tUI_Info.vColor.m128_f32[1] = object["ColorG"];			// 14. G
		if (object.contains("ColorB"))
			tUI_Info.vColor.m128_f32[2] = object["ColorB"];			// 15. B
		if (object.contains("ColorA"))
			tUI_Info.vColor.m128_f32[3] = object["ColorA"];			// 16. A


		wstring wstrClonetag;
		m_pGameInstance->String_To_WString(tUI_Info.strCloneTag, wstrClonetag);

		wstring wstrPrototag;
		m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

		wstring wstrFilePath;
		m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

		CGameObject* pGameObject = m_pGameInstance->Add_CloneObject_And_Get(iLevelIndex, strLayerTag, wstrClonetag, &tUI_Info);
		if (pGameObject == nullptr)
			return E_FAIL;

		CUI* pUI_Object = dynamic_cast<CUI*>(pGameObject);
		if (pUI_Object == nullptr)
			return E_FAIL;

		m_vecLoading.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}

	return S_OK;
}

void CUI_Manager::Active_Loading_Intro(_bool bActive)
{
	if (m_vecLoading.empty())
		return;

	for (auto& iter : m_vecLoading)
	{
		iter->Set_Alpha(!bActive);			// UI 알파값 초기화
		iter->Set_Active(bActive);		// UI 활성화
		iter->Set_AnimPlay(bActive);	// UI Animation 재생
		iter->Set_Disappear(!bActive);	// UI 사라짐 Off
	}
}

HRESULT CUI_Manager::Add_Loading_IntroBoss(_uint iLevelIndex, const wstring& strLayerTag)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/Loading/IntroBossLoading.json";

	CJson_Utility::Load_Json(strFile.c_str(), json_in);

	for (auto& item : json_in.items())
	{
		json object = item.value();

		CUI::UI_DESC tUI_Info;

		/* 저장순서랑 맞는지 확인하기 */
		if (object.contains("Parent"))
			tUI_Info.bParent = object["Parent"];					// 1. Parent
		if (object.contains("World"))
			tUI_Info.bWorld = object["World"];						// 2. World
		if (object.contains("Group"))
			tUI_Info.bGroup = object["Group"];						// 3. Group
		if (object.contains("Alpha"))
			tUI_Info.fAlpha = object["Alpha"];						// 4. Alpha
		if (object.contains("ObjectNum"))
			tUI_Info.iObjectNum = object["ObjectNum"];				// 5. ObjectNum
		if (object.contains("ShaderNum"))
			tUI_Info.iShaderNum = object["ShaderNum"];				// 6. ShaderPathNum
		if (object.contains("UINum"))								// "ObjectName" 키가 있으면
			tUI_Info.iUINum = object["UINum"];
		if (object.contains("UIName"))								// "ObjectName" 키가 있으면
			tUI_Info.strUIName = object["UIName"];
		if (object.contains("ObjectName"))							// "ObjectName" 키가 있으면
			tUI_Info.strObjectName = object["ObjectName"];			// 7. ObjectName
		if (object.contains("LayerTag"))
			tUI_Info.strLayerTag = object["LayerTag"];				// 8. LayerTag
		if (object.contains("CloneTag"))
			tUI_Info.strCloneTag = object["CloneTag"];				// 9. CloneTag
		if (object.contains("ProtoTag"))
			tUI_Info.strProtoTag = object["ProtoTag"];				// 10. ProtoTag
		if (object.contains("FilePath"))
			tUI_Info.strFilePath = object["FilePath"];				// 11. FilePath
		if (object.contains("MapTextureTag"))
			tUI_Info.strMapTextureTag = object["MapTextureTag"];	// 12. MapTexture
		if (object.contains("ColorR"))
			tUI_Info.vColor.m128_f32[0] = object["ColorR"];			// 13. R
		if (object.contains("ColorG"))
			tUI_Info.vColor.m128_f32[1] = object["ColorG"];			// 14. G
		if (object.contains("ColorB"))
			tUI_Info.vColor.m128_f32[2] = object["ColorB"];			// 15. B
		if (object.contains("ColorA"))
			tUI_Info.vColor.m128_f32[3] = object["ColorA"];			// 16. A


		wstring wstrClonetag;
		m_pGameInstance->String_To_WString(tUI_Info.strCloneTag, wstrClonetag);

		wstring wstrPrototag;
		m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

		wstring wstrFilePath;
		m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

		CGameObject* pGameObject = m_pGameInstance->Add_CloneObject_And_Get(iLevelIndex, strLayerTag, wstrClonetag, &tUI_Info);
		if (pGameObject == nullptr)
			return E_FAIL;

		CUI* pUI_Object = dynamic_cast<CUI*>(pGameObject);
		if (pUI_Object == nullptr)
			return E_FAIL;

		m_vecLoadingIntroBoss.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}
	return S_OK;
}

void CUI_Manager::Active_Loading_IntroBoss(_bool bActive)
{
	if (m_vecLoadingIntroBoss.empty())
		return;

	for (auto& iter : m_vecLoadingIntroBoss)
	{
		iter->Set_Alpha(!bActive);			// UI 알파값 초기화
		iter->Set_Active(bActive);		// UI 활성화
		iter->Set_AnimPlay(bActive);	// UI Animation 재생
		iter->Set_Disappear(!bActive);	// UI 사라짐 Off
	}
}

HRESULT CUI_Manager::Add_Crosshair(_uint iLevelIndex, const wstring& strLayerTag)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/Crosshair/Crosshair.json";

	CJson_Utility::Load_Json(strFile.c_str(), json_in);

	for (auto& item : json_in.items())
	{
		json object = item.value();

		CUI::UI_DESC tUI_Info;

		/* 저장순서랑 맞는지 확인하기 */
		if (object.contains("Parent"))
			tUI_Info.bParent = object["Parent"];					// 1. Parent
		if (object.contains("World"))
			tUI_Info.bWorld = object["World"];						// 2. World
		if (object.contains("Group"))
			tUI_Info.bGroup = object["Group"];						// 3. Group
		if (object.contains("Alpha"))
			tUI_Info.fAlpha = object["Alpha"];						// 4. Alpha
		if (object.contains("ObjectNum"))
			tUI_Info.iObjectNum = object["ObjectNum"];				// 5. ObjectNum
		if (object.contains("ShaderNum"))
			tUI_Info.iShaderNum = object["ShaderNum"];				// 6. ShaderPathNum
		if (object.contains("UINum"))								// "ObjectName" 키가 있으면
			tUI_Info.iUINum = object["UINum"];
		if (object.contains("UIName"))								// "ObjectName" 키가 있으면
			tUI_Info.strUIName = object["UIName"];
		if (object.contains("ObjectName"))							// "ObjectName" 키가 있으면
			tUI_Info.strObjectName = object["ObjectName"];			// 7. ObjectName
		if (object.contains("LayerTag"))
			tUI_Info.strLayerTag = object["LayerTag"];				// 8. LayerTag
		if (object.contains("CloneTag"))
			tUI_Info.strCloneTag = object["CloneTag"];				// 9. CloneTag
		if (object.contains("ProtoTag"))
			tUI_Info.strProtoTag = object["ProtoTag"];				// 10. ProtoTag
		if (object.contains("FilePath"))
			tUI_Info.strFilePath = object["FilePath"];				// 11. FilePath
		if (object.contains("MapTextureTag"))
			tUI_Info.strMapTextureTag = object["MapTextureTag"];	// 12. MapTexture
		if (object.contains("ColorR"))
			tUI_Info.vColor.m128_f32[0] = object["ColorR"];			// 13. R
		if (object.contains("ColorG"))
			tUI_Info.vColor.m128_f32[1] = object["ColorG"];			// 14. G
		if (object.contains("ColorB"))
			tUI_Info.vColor.m128_f32[2] = object["ColorB"];			// 15. B
		if (object.contains("ColorA"))
			tUI_Info.vColor.m128_f32[3] = object["ColorA"];			// 16. A


		wstring wstrClonetag;
		m_pGameInstance->String_To_WString(tUI_Info.strCloneTag, wstrClonetag);

		wstring wstrPrototag;
		m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

		wstring wstrFilePath;
		m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

		CGameObject* pGameObject = m_pGameInstance->Add_CloneObject_And_Get(iLevelIndex, strLayerTag, wstrClonetag, &tUI_Info);
		if (pGameObject == nullptr)
			return E_FAIL;

		CUI* pUI_Object = dynamic_cast<CUI*>(pGameObject);
		if (pUI_Object == nullptr)
			return E_FAIL;

		m_vecCrosshair.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}

	return S_OK;
}

void CUI_Manager::Active_Crosshair(_bool bActive)
{
	if (m_vecCrosshair.empty())
		return;

	for (auto& iter : m_vecCrosshair)
	{
		iter->Set_Alpha(!bActive);		// UI 알파값 초기화
		iter->Set_Active(bActive);		// UI 활성화
		iter->Set_Disappear(!bActive);	// UI 사라짐
	}
}

void CUI_Manager::Trigger_Crosshair(_bool bPlayAnim)
{
	if (m_vecCrosshair.empty())
		return;

	for (auto& iter : m_vecCrosshair)
	{
		iter->Set_AnimPlay(bPlayAnim);	// UI Animation 재생
		iter->Set_CurrTime(0.f);
	}
}

HRESULT CUI_Manager::Add_BossHUD_Bar(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pOwner)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/BossHUD/BossHUDBar.json";

	CJson_Utility::Load_Json(strFile.c_str(), json_in);

	for (auto& item : json_in.items())
	{
		json object = item.value();

		CUI::UI_DESC tUI_Info;

		/* 저장순서랑 맞는지 확인하기 */
		if (object.contains("Parent"))
			tUI_Info.bParent = object["Parent"];					// 1. Parent
		if (object.contains("World"))
			tUI_Info.bWorld = object["World"];						// 2. World
		if (object.contains("Group"))
			tUI_Info.bGroup = object["Group"];						// 3. Group
		if (object.contains("Alpha"))
			tUI_Info.fAlpha = object["Alpha"];						// 4. Alpha
		if (object.contains("ObjectNum"))
			tUI_Info.iObjectNum = object["ObjectNum"];				// 5. ObjectNum
		if (object.contains("ShaderNum"))
			tUI_Info.iShaderNum = object["ShaderNum"];				// 6. ShaderPathNum
		if (object.contains("UINum"))								// "ObjectName" 키가 있으면
			tUI_Info.iUINum = object["UINum"];
		if (object.contains("UIName"))								// "ObjectName" 키가 있으면
			tUI_Info.strUIName = object["UIName"];
		if (object.contains("ObjectName"))							// "ObjectName" 키가 있으면
			tUI_Info.strObjectName = object["ObjectName"];			// 7. ObjectName
		if (object.contains("LayerTag"))
			tUI_Info.strLayerTag = object["LayerTag"];				// 8. LayerTag
		if (object.contains("CloneTag"))
			tUI_Info.strCloneTag = object["CloneTag"];				// 9. CloneTag
		if (object.contains("ProtoTag"))
			tUI_Info.strProtoTag = object["ProtoTag"];				// 10. ProtoTag
		if (object.contains("FilePath"))
			tUI_Info.strFilePath = object["FilePath"];				// 11. FilePath
		if (object.contains("MapTextureTag"))
			tUI_Info.strMapTextureTag = object["MapTextureTag"];	// 12. MapTexture
		if (object.contains("ColorR"))
			tUI_Info.vColor.m128_f32[0] = object["ColorR"];			// 13. R
		if (object.contains("ColorG"))
			tUI_Info.vColor.m128_f32[1] = object["ColorG"];			// 14. G
		if (object.contains("ColorB"))
			tUI_Info.vColor.m128_f32[2] = object["ColorB"];			// 15. B
		if (object.contains("ColorA"))
			tUI_Info.vColor.m128_f32[3] = object["ColorA"];			// 16. A

		wstring wstrClonetag;
		m_pGameInstance->String_To_WString(tUI_Info.strCloneTag, wstrClonetag);

		wstring wstrPrototag;
		m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

		wstring wstrFilePath;
		m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

		CGameObject* pGameObject = m_pGameInstance->Add_CloneObject_And_Get(iLevelIndex, strLayerTag, wstrClonetag, &tUI_Info);
		if (pGameObject == nullptr)
			return E_FAIL;

		CUI* pUI_Object = dynamic_cast<CUI*>(pGameObject);
		if (pUI_Object == nullptr)
			return E_FAIL;

		/* HP Bar */
		string strCloneTag_HPBar = "Prototype_GameObject_UI_EnemyHP_Bar";
		if (tUI_Info.strCloneTag == strCloneTag_HPBar)
		{
			pUI_Object->Set_Object_Owner(pOwner);
			dynamic_cast<CUI_EnemyHP_Bar*>(pUI_Object)->Setting_Owner();
			dynamic_cast<CUI_EnemyHP_Bar*>(pUI_Object)->Set_Active(true);
		}

		/* Distortion */
		string strCloneTag_Distortion = "Prototype_GameObject_UI_Distortion";
		if (tUI_Info.strCloneTag == strCloneTag_Distortion)
		{
			dynamic_cast<CUI_Distortion*>(pUI_Object)->Set_Active(true);
		}

		m_vecBossHUD_Bar.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}

	Active_BossHUD_Bar(true);

	return S_OK;
}

void CUI_Manager::Active_BossHUD_Bar(_bool bActive)
{
	if (m_vecBossHUD_Bar.empty())
		return;

	for (auto& iter : m_vecBossHUD_Bar)
	{
		iter->Set_Alpha(!bActive);		// UI 알파값 초기화
		iter->Set_Active(bActive);		// UI 활성화
		iter->Set_AnimPlay(bActive);	// UI Animation 재생
		iter->Set_Disappear(!bActive);	// UI 사라짐 Off
	}
}

HRESULT CUI_Manager::Add_BossHUD_Shard(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pOwner)
{

	return S_OK;
}

void CUI_Manager::Active_BossHUD_Shard(_bool bActive)
{

}

HRESULT CUI_Manager::Add_EnemyHUD_Shard(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pOwner)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/EnemyHUD/EnemyHUDShard.json";

	CJson_Utility::Load_Json(strFile.c_str(), json_in);

	for (auto& item : json_in.items())
	{
		json object = item.value();

		CUI::UI_DESC tUI_Info;

		/* 저장순서랑 맞는지 확인하기 */
		if (object.contains("Parent"))
			tUI_Info.bParent = object["Parent"];					// 1. Parent
		if (object.contains("World"))
			tUI_Info.bWorld = object["World"];						// 2. World
		if (object.contains("Group"))
			tUI_Info.bGroup = object["Group"];						// 3. Group
		if (object.contains("Alpha"))
			tUI_Info.fAlpha = object["Alpha"];						// 4. Alpha
		if (object.contains("ObjectNum"))
			tUI_Info.iObjectNum = object["ObjectNum"];				// 5. ObjectNum
		if (object.contains("ShaderNum"))
			tUI_Info.iShaderNum = object["ShaderNum"];				// 6. ShaderPathNum
		if (object.contains("UINum"))								// "ObjectName" 키가 있으면
			tUI_Info.iUINum = object["UINum"];
		if (object.contains("UIName"))								// "ObjectName" 키가 있으면
			tUI_Info.strUIName = object["UIName"];
		if (object.contains("ObjectName"))							// "ObjectName" 키가 있으면
			tUI_Info.strObjectName = object["ObjectName"];			// 7. ObjectName
		if (object.contains("LayerTag"))
			tUI_Info.strLayerTag = object["LayerTag"];				// 8. LayerTag
		if (object.contains("CloneTag"))
			tUI_Info.strCloneTag = object["CloneTag"];				// 9. CloneTag
		if (object.contains("ProtoTag"))
			tUI_Info.strProtoTag = object["ProtoTag"];				// 10. ProtoTag
		if (object.contains("FilePath"))
			tUI_Info.strFilePath = object["FilePath"];				// 11. FilePath
		if (object.contains("MapTextureTag"))
			tUI_Info.strMapTextureTag = object["MapTextureTag"];	// 12. MapTexture
		if (object.contains("ColorR"))
			tUI_Info.vColor.m128_f32[0] = object["ColorR"];			// 13. R
		if (object.contains("ColorG"))
			tUI_Info.vColor.m128_f32[1] = object["ColorG"];			// 14. G
		if (object.contains("ColorB"))
			tUI_Info.vColor.m128_f32[2] = object["ColorB"];			// 15. B
		if (object.contains("ColorA"))
			tUI_Info.vColor.m128_f32[3] = object["ColorA"];			// 16. A

		wstring wstrClonetag;
		m_pGameInstance->String_To_WString(tUI_Info.strCloneTag, wstrClonetag);

		wstring wstrPrototag;
		m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

		wstring wstrFilePath;
		m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

		CGameObject* pGameObject = m_pGameInstance->Add_CloneObject_And_Get(iLevelIndex, strLayerTag, wstrClonetag, &tUI_Info);
		if (pGameObject == nullptr)
			return E_FAIL;

		CUI* pUI_Object = dynamic_cast<CUI*>(pGameObject);
		if (pUI_Object == nullptr)
			return E_FAIL;

		pUI_Object->Set_Object_Owner(pOwner);
		pUI_Object->Setting_Owner();

		/* HP Shard */
		string strCloneTag_HPBar = "Prototype_GameObject_UI_EnemyHP_Shard";
		if (tUI_Info.strCloneTag == strCloneTag_HPBar)
		{
			//dynamic_cast<CUI_EnemyHP_Shard*>(pUI_Object)->Setting_Owner();
		}

		/* Distortion */
		string strCloneTag_Distortion = "Prototype_GameObject_UI_Distortion";
		if (tUI_Info.strCloneTag == strCloneTag_Distortion)
		{
			dynamic_cast<CUI_Distortion*>(pUI_Object)->Set_Active(true);
		}

		m_vecEnemyHUD_Shard.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}

	return S_OK;
}

void CUI_Manager::Active_EnemyHUD_Shard(_bool bActive)
{
	if (m_vecEnemyHUD_Shard.empty())
		return;

	for (auto& iter : m_vecEnemyHUD_Shard)
	{
		iter->Set_Alpha(!bActive);			// UI 알파값 초기화
		iter->Set_Active(bActive);		// UI 활성화
		iter->Set_AnimPlay(bActive);	// UI Animation 재생
		iter->Set_Disappear(!bActive);	// UI 사라짐 Off
	}
}

void CUI_Manager::Set_EnemyHUD_World(_matrix matWorld, _float3 vOffsetPos)
{
	if (m_vecEnemyHUD_Shard.empty())
	return;

	for (auto& iter : m_vecEnemyHUD_Shard)
	{
		iter->SetUp_WorldToScreen(matWorld, vOffsetPos);
	}
}

void CUI_Manager::Set_Offset(_float fOffsetX, _float fOffsetY)
{
	if (m_vecEnemyHUD_Shard.empty())
		return;

	for (auto& iter : m_vecEnemyHUD_Shard)
	{
		iter->Set_OffsetXY(fOffsetX, fOffsetY);
	}
}

HRESULT CUI_Manager::Add_DiedScreen(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pOwner)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/DIED/DIED.json";

	CJson_Utility::Load_Json(strFile.c_str(), json_in);

	for (auto& item : json_in.items())
	{
		json object = item.value();

		//CUI::UI_DESC tUI_Info;
		CUI::UI_DESC* tUI_Info = new CUI::UI_DESC;
		Load_Json_BasicInfo(object, tUI_Info);

		///* 저장순서랑 맞는지 확인하기 */
		//if (object.contains("Parent"))
		//	tUI_Info.bParent = object["Parent"];						// 1. Parent
		//if (object.contains("World"))
		//	tUI_Info.bWorld = object["World"];						// 2. World
		//if (object.contains("Group"))
		//	tUI_Info.bGroup = object["Group"];						// 3. Group
		//if (object.contains("Alpha"))
		//	tUI_Info.fAlpha = object["Alpha"];						// 4. Alpha
		//if (object.contains("ObjectNum"))
		//	tUI_Info.iObjectNum = object["ObjectNum"];				// 5. objectNum
		//if (object.contains("ShaderNum"))
		//	tUI_Info.iShaderNum = object["ShaderNum"];				// 6. ShaderPathNum
		//if (object.contains("UINum"))									// "ObjectName" 키가 있으면
		//	tUI_Info.iUINum = object["UINum"];
		//if (object.contains("UIName"))								// "ObjectName" 키가 있으면
		//	tUI_Info.strUIName = object["UIName"];
		//if (object.contains("ObjectName"))							// "ObjectName" 키가 있으면
		//	tUI_Info.strObjectName = object["ObjectName"];			// 7. ObjectName
		//if (object.contains("LayerTag"))
		//	tUI_Info.strLayerTag = object["LayerTag"];				// 8. LayerTag
		//if (object.contains("CloneTag"))
		//	tUI_Info.strCloneTag = object["CloneTag"];				// 9. CloneTag
		//if (object.contains("ProtoTag"))
		//	tUI_Info.strProtoTag = object["ProtoTag"];				// 10. ProtoTag
		//if (object.contains("FilePath"))
		//	tUI_Info.strFilePath = object["FilePath"];				// 11. FilePath
		//if (object.contains("MapTextureTag"))
		//	tUI_Info.strMapTextureTag = object["MapTextureTag"];		// 12. MapTexture
		//if (object.contains("ColorR"))
		//	tUI_Info.vColor.m128_f32[0] = object["ColorR"];			// 13. R
		//if (object.contains("ColorG"))
		//	tUI_Info.vColor.m128_f32[1] = object["ColorG"];			// 14. G
		//if (object.contains("ColorB"))
		//	tUI_Info.vColor.m128_f32[2] = object["ColorB"];			// 15. B
		//if (object.contains("ColorA"))
		//	tUI_Info.vColor.m128_f32[3] = object["ColorA"];			// 16. A
		//if (object.contains("ColorMode"))
		//	tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		//if (object.contains("RenderGroup"))
		//	tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

		//wstring wstrClonetag;
		//m_pGameInstance->String_To_WString(tUI_Info.strCloneTag, wstrClonetag);

		//wstring wstrPrototag;
		//m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

		//wstring wstrFilePath;
		//m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

		wstring wstrClonetag;
		m_pGameInstance->String_To_WString(tUI_Info->strCloneTag, wstrClonetag);

		wstring wstrPrototag;
		m_pGameInstance->String_To_WString(tUI_Info->strProtoTag, wstrPrototag);

		wstring wstrFilePath;
		m_pGameInstance->String_To_WString(tUI_Info->strFilePath, wstrFilePath);

		CGameObject* pGameObject = m_pGameInstance->Add_CloneObject_And_Get(iLevelIndex, strLayerTag, wstrClonetag, tUI_Info);
		if (pGameObject == nullptr)
			return E_FAIL;

		CUI* pUI_Object = dynamic_cast<CUI*>(pGameObject);
		if (pUI_Object == nullptr)
			return E_FAIL;

		m_vecDiedScreen.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data

		delete tUI_Info;
		tUI_Info = nullptr;
	}

	return S_OK;
}

void CUI_Manager::Active_DiedScreen()
{
	if (m_vecDiedScreen.empty())
		return;

	for (auto& iter : m_vecDiedScreen)
	{
		iter->Set_Alpha(0.f);		// UI 알파값 초기화
		iter->Set_Active(true);		// UI 활성화
		iter->Set_AnimPlay(true);	// UI Animation 재생
		iter->Set_CurrTime(0.f);	// UI Animation 시간 초기화
		iter->Set_Disappear(false);	// UI 사라짐 Off
	}
}

void CUI_Manager::NonActive_DiedScreen()
{
	if (m_vecDiedScreen.empty())
		return;

	for (auto& iter : m_vecDiedScreen)
	{
		iter->Set_Alpha(1.f);		// UI 알파값 초기화
		iter->Set_Active(false);	// UI 활성화
		iter->Set_AnimPlay(false);	// UI Animation 재생
		iter->Set_CurrTime(0.f);	// UI Animation 시간 초기화
		iter->Set_Disappear(true);	// UI 사라짐 Off
	}
}

HRESULT CUI_Manager::Add_Option(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pOwner)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/Option/Option.json";

	CJson_Utility::Load_Json(strFile.c_str(), json_in);

	for (auto& item : json_in.items())
	{
		json object = item.value();

		CUI::UI_DESC* tUI_Info = new CUI::UI_DESC;
		Load_Json_BasicInfo(object, tUI_Info);

		wstring wstrClonetag;
		m_pGameInstance->String_To_WString(tUI_Info->strCloneTag, wstrClonetag);

		wstring wstrPrototag;
		m_pGameInstance->String_To_WString(tUI_Info->strProtoTag, wstrPrototag);

		wstring wstrFilePath;
		m_pGameInstance->String_To_WString(tUI_Info->strFilePath, wstrFilePath);
		
		CGameObject* pGameObject = m_pGameInstance->Add_CloneObject_And_Get(iLevelIndex, strLayerTag, wstrClonetag, tUI_Info);
		if (pGameObject == nullptr)
			return E_FAIL;

		CUI* pUI_Object = dynamic_cast<CUI*>(pGameObject);
		if (pUI_Object == nullptr)
			return E_FAIL;

		m_vecOption.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data

		delete tUI_Info;
		tUI_Info = nullptr;
	}

	return S_OK;
}

void CUI_Manager::Active_Option()
{
	if (m_vecOption.empty())
		return;

	for (auto& iter : m_vecOption)
	{
		iter->Set_Alpha(0.f);		// UI 알파값 초기화
		iter->Set_Active(true);		// UI 활성화
		iter->Set_AnimPlay(true);	// UI Animation 재생
		iter->Set_CurrTime(0.f);	// UI Animation 시간 초기화
		iter->Set_Disappear(false);	// UI 사라짐 Off
	}
}

void CUI_Manager::NonActive_Option()
{
	if (m_vecOption.empty())
		return;

	for (auto& iter : m_vecOption)
	{
		iter->Set_Alpha(1.f);		// UI 알파값 초기화
		iter->Set_Active(false);	// UI 활성화
		iter->Set_AnimPlay(false);	// UI Animation 재생
		iter->Set_CurrTime(0.f);	// UI Animation 시간 초기화
		iter->Set_Disappear(true);	// UI 사라짐 Off
	}
}

void CUI_Manager::Load_Json_BasicInfo(const json& Out_Json, CUI::UI_DESC* tUI_Info)
{
	/* 저장순서랑 맞는지 확인하기 */
	if (Out_Json.contains("Parent"))
		tUI_Info->bParent = Out_Json["Parent"];						// 1. Parent
	if (Out_Json.contains("World"))
		tUI_Info->bWorld = Out_Json["World"];						// 2. World
	if (Out_Json.contains("Group"))
		tUI_Info->bGroup = Out_Json["Group"];						// 3. Group
	if (Out_Json.contains("Alpha"))
		tUI_Info->fAlpha = Out_Json["Alpha"];						// 4. Alpha
	if (Out_Json.contains("ObjectNum"))
		tUI_Info->iObjectNum = Out_Json["ObjectNum"];				// 5. Out_JsonNum
	if (Out_Json.contains("ShaderNum"))
		tUI_Info->iShaderNum = Out_Json["ShaderNum"];				// 6. ShaderPathNum
	if (Out_Json.contains("UINum"))									// "ObjectName" 키가 있으면
		tUI_Info->iUINum = Out_Json["UINum"];
	if (Out_Json.contains("UIName"))								// "ObjectName" 키가 있으면
		tUI_Info->strUIName = Out_Json["UIName"];
	if (Out_Json.contains("ObjectName"))							// "ObjectName" 키가 있으면
		tUI_Info->strObjectName = Out_Json["ObjectName"];			// 7. ObjectName
	if (Out_Json.contains("LayerTag"))
		tUI_Info->strLayerTag = Out_Json["LayerTag"];				// 8. LayerTag
	if (Out_Json.contains("CloneTag"))
		tUI_Info->strCloneTag = Out_Json["CloneTag"];				// 9. CloneTag
	if (Out_Json.contains("ProtoTag"))
		tUI_Info->strProtoTag = Out_Json["ProtoTag"];				// 10. ProtoTag
	if (Out_Json.contains("FilePath"))
		tUI_Info->strFilePath = Out_Json["FilePath"];				// 11. FilePath
	if (Out_Json.contains("MapTextureTag"))
		tUI_Info->strMapTextureTag = Out_Json["MapTextureTag"];		// 12. MapTexture
	if (Out_Json.contains("ColorR"))
		tUI_Info->vColor.m128_f32[0] = Out_Json["ColorR"];			// 13. R
	if (Out_Json.contains("ColorG"))
		tUI_Info->vColor.m128_f32[1] = Out_Json["ColorG"];			// 14. G
	if (Out_Json.contains("ColorB"))
		tUI_Info->vColor.m128_f32[2] = Out_Json["ColorB"];			// 15. B
	if (Out_Json.contains("ColorA"))
		tUI_Info->vColor.m128_f32[3] = Out_Json["ColorA"];			// 16. A
	if (Out_Json.contains("ColorMode"))
		tUI_Info->eColorMode = Out_Json["ColorMode"];				// 16. Mode
	if (Out_Json.contains("RenderGroup"))
		tUI_Info->iRenderGroup = Out_Json["RenderGroup"];			// 16. RenderGroup
}

//// Add_CloneUI == Add_CloneObject_And_Get
//CUI* CUI_Manager::Add_CloneUI(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg)
//{
//	/* UI 객체 생성 */
//	CUI* pUI = dynamic_cast<CUI*>(m_pGameInstance->Add_CloneObject_And_Get(iLevelIndex, strLayerTag, strPrototypeTag, &pArg));
//
//	/* Manager에 UI 추가*/
//	m_vecUI_Manager.push_back(pUI);
//
//	return	pUI;
//}

#ifdef _DEBUG
CUI* CUI_Manager::Add_Tool(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg)
{
	/* UI 객체 생성 */
	CUI* pUI = dynamic_cast<CUI*>(m_pGameInstance->Add_CloneObject_And_Get(iLevelIndex, strLayerTag, strPrototypeTag, &pArg));

	/* Manager에 UI 추가*/
	m_vecUI_Manager.push_back(pUI);

	return	pUI;
}

void CUI_Manager::Active_Tool()
{
	if (m_vecUI_Manager.empty())
		return;

	for (auto& iter : m_vecUI_Manager)
	{
		iter->Set_Alpha(0.f);		// UI 알파값 초기화
		iter->Set_Active(true);		// UI 활성화
		iter->Set_AnimPlay(true);	// UI Animation 재생
		iter->Set_Disappear(false); // UI 사라짐 Off
	}
}
#endif // DEBUG

void CUI_Manager::Check_Active(_float fTimeDelta)
{
	switch (m_eType)
	{
		case Client::UITYPE::NONE:
			break;
		case Client::UITYPE::LEFT_HUD:
			Active_LeftHUD();
			break;
		case Client::UITYPE::RIGHT_HUD:
			Active_RightHUD();
			break;
		case Client::UITYPE::LEVEL_UP:
			Active_LevelUp();
			break;
		case Client::UITYPE::REWARD:
			Active_RewardBox();
			break;
		case Client::UITYPE::QUESTBOX:
			Active_QuestBox();
			break;
		case Client::UITYPE::TUTORIALBOX:
			Active_TutorialBox();
			break;
		case Client::UITYPE::CROSSHAIR:
			Active_Crosshair(true); // Active
			break;
		case Client::UITYPE::WEAKNESS:

			break;
		case Client::UITYPE::TYPE_END:
			break;
		default:
			break;
	}

	m_eType = UITYPE::NONE;
}

HRESULT CUI_Manager::Load_Json(const string& strPath, const string& strFileName)
{
	return S_OK;
}

void CUI_Manager::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	for (auto& iter : m_vecLeftHUD)
	{
		Safe_Release(iter);
	}
	for (auto& iter : m_vecRightHUD)
	{
		Safe_Release(iter);
	}
	for (auto& iter : m_vecTutorialBox)
	{
		Safe_Release(iter);
	}
	for (auto& iter : m_vecLevelUP)
	{
		Safe_Release(iter);
	}
	for (auto& iter : m_vecRewardBox)
	{
		Safe_Release(iter);
	}
	for (auto& iter : m_vecQuestBox)
	{
		Safe_Release(iter);
	}
	for (auto& iter : m_vecDistortion)
	{
		Safe_Release(iter);
	}
	for (auto& iter : m_vecLoading)
	{
		Safe_Release(iter);
	}
	for (auto& iter : m_vecLoadingIntroBoss)
	{
		Safe_Release(iter);
	}
	for (auto& iter : m_vecCrosshair)
	{
		Safe_Release(iter);
	}
	for (auto& iter : m_vecBossHUD_Bar)
	{
		Safe_Release(iter);
	}
	for (auto& iter : m_vecBossHUD_Shard)
	{
		Safe_Release(iter);
	}
	for (auto& iter : m_vecEnemyHUD_Shard)
	{
		Safe_Release(iter);
	}

}
