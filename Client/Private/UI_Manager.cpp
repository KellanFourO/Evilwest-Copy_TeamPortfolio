#include "stdafx.h"
#include "Clone_Manager.h"

#include "GameInstance.h"

#include "..\Public\UI_Manager.h"

#pragma region UI
//#include "UI.h"
#include "UI_EnemyHP_Bar.h"
#include "UI_EnemyHP_Shard.h"
#include "UI_EnemyHUD_Shard.h"
#include "UI_Distortion.h"
#include "UI_TutorialBox.h"
#include "UI_Player_Skill_Icon.h"
#include "UI_Player_Skill_Guige.h"
#include "UI_Interaction.h"
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
	//Check_Active(fTimeDelta);
	//Check_UIPicking(fTimeDelta);
}

// 플레이 화면의 모든 UI생성
HRESULT CUI_Manager::Ready_Interface(_uint iLevelIndex)
{
	Add_LeftHUD(iLevelIndex, TEXT("Layer_LeftHUD"));
	Add_RightHUD(iLevelIndex, TEXT("Layer_RightHUD"));
	Add_LeftSkill(iLevelIndex, TEXT("Layer_LeftSkill"));
	Add_RightSkill(iLevelIndex, TEXT("Layer_RightSkill"));
	Add_TutorialBox(iLevelIndex, TEXT("Layer_TutorialBox"));
	Add_LevelUp(iLevelIndex, TEXT("Layer_LevelUp"));
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

HRESULT CUI_Manager::Ready_Loading_SnowMountain(_uint iLevelIndex)
{
	Add_Loading_SnowMountain(iLevelIndex, TEXT("Layer_SnowMountain"));

	return S_OK;
}

HRESULT CUI_Manager::Ready_Loading_SnowMountainBoss(_uint iLevelIndex)
{
	Add_Loading_SnowMountainBoss(iLevelIndex, TEXT("Layer_SnowMountainBoss"));

	return S_OK;
}

HRESULT CUI_Manager::Ready_Loading_ToolLevel(_uint iLevelIndex)
{
	Add_Loading_ToolLevel(iLevelIndex, TEXT("Layer_ToolLevel"));

	return S_OK;
}

HRESULT CUI_Manager::Ready_Loading_TestLevel(_uint iLevelIndex)
{
	Add_Loading_TestLevel(iLevelIndex, TEXT("Layer_TestLevel"));

	return S_OK;
}

HRESULT CUI_Manager::Ready_BossHUD_Bar(_uint iLevelIndex, CGameObject* pOwner, const string& strBossName)
{
	Add_BossHUD_Bar(iLevelIndex, TEXT("Layer_BossHUDBar"), pOwner, strBossName);

	return S_OK;
}

CUI_EnemyHUD_Shard* CUI_Manager::Ready_EnemyHUD_Shard(_uint iLevelIndex, CGameObject* pOwner)
{
	return Add_EnemyHUD_Shard(iLevelIndex, TEXT("Layer_EnemyHUDShard"), pOwner);;
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

HRESULT CUI_Manager::Ready_MainMenu(_uint iLevelIndex)
{
	Add_MainMenu(iLevelIndex, TEXT("Layer_MainMenu"));
	Add_MainList(iLevelIndex, TEXT("Layer_MainList"));
	Add_LevelList(iLevelIndex, TEXT("Layer_LevelList"));
	Add_MainLogo(iLevelIndex, TEXT("Layer_MainLogo"));

	return S_OK;
}

HRESULT CUI_Manager::Ready_MouseCursor(_uint iLevelIndex)
{
	Add_MouseCursor(iLevelIndex, TEXT("Layer_MouseCursor"));

	return S_OK;
}

HRESULT CUI_Manager::Ready_HitUI(_uint iLevelIndex)
{
	Add_HitUI(iLevelIndex, TEXT("Layer_HitUI"));

	return S_OK;
}

void CUI_Manager::Active_PlayerHUD()
{
	if (m_vecLeftHUD.empty())
		return;

	for (auto& iter : m_vecLeftHUD)
	{
		iter->Set_Alpha(0.f);		// UI 알파값 초기화
		iter->Set_Active(true);		// UI 활성화
		iter->Set_AnimPlay(true);	// UI Animation 재생
		iter->Set_Disappear(false); // UI 사라짐 Off
		iter->ResetTime();			// ! (LifeTime UI일 경우) UI TimeReset
	}

	if (m_vecRightHUD.empty())
		return;

	for (auto& iter : m_vecRightHUD)
	{
		iter->Set_Alpha(0.f);		// UI 알파값 초기화
		iter->Set_Active(true);		// UI 활성화
		iter->Set_AnimPlay(true);	// UI Animation 재생
		iter->Set_Disappear(false); // UI 사라짐 Off
		iter->ResetTime();			// ! (LifeTime UI일 경우) UI TimeReset
	}

	if (m_vecLeftSkill.empty())
		return;

	for (auto& iter : m_vecLeftSkill)
	{
		iter->Set_Alpha(0.f);		// UI 알파값 초기화
		iter->Set_Active(true);		// UI 활성화
		iter->Set_AnimPlay(true);	// UI Animation 재생
		iter->Set_Disappear(false); // UI 사라짐 Off
		iter->ResetTime();			// ! (LifeTime UI일 경우) UI TimeReset
	}

	if (m_vecRightSkill.empty())
		return;

	for (auto& iter : m_vecRightSkill)
	{
		iter->Set_Alpha(0.f);		// UI 알파값 초기화
		iter->Set_Active(true);		// UI 활성화
		iter->Set_AnimPlay(true);	// UI Animation 재생
		iter->Set_Disappear(false); // UI 사라짐 Off
		iter->ResetTime();			// ! (LifeTime UI일 경우) UI TimeReset
	}
}

void CUI_Manager::NonActive_PlayerHUD()
{
	if (m_vecLeftHUD.empty())
		return;

	for (auto& iter : m_vecLeftHUD)
	{
		iter->Set_Alpha(1.f);		// UI 알파값 초기화
		iter->Set_Active(false);	// UI 활성화
		iter->Set_AnimPlay(false);	// UI Animation 재생
		iter->Set_Disappear(true);	// UI 사라짐 Off
	}

	if (m_vecRightHUD.empty())
		return;

	for (auto& iter : m_vecRightHUD)
	{
		iter->Set_Alpha(1.f);		// UI 알파값 초기화
		iter->Set_Active(false);	// UI 활성화
		iter->Set_AnimPlay(false);	// UI Animation 재생
		iter->Set_Disappear(true);	// UI 사라짐 Off
	}

	if (m_vecLeftSkill.empty())
		return;

	for (auto& iter : m_vecLeftSkill)
	{
		iter->Set_Alpha(1.f);		// UI 알파값 초기화
		iter->Set_Active(false);	// UI 활성화
		iter->Set_AnimPlay(false);	// UI Animation 재생
		iter->Set_Disappear(true);	// UI 사라짐 Off
	}

	if (m_vecRightSkill.empty())
		return;

	for (auto& iter : m_vecRightSkill)
	{
		iter->Set_Alpha(1.f);		// UI 알파값 초기화
		iter->Set_Active(false);	// UI 활성화
		iter->Set_AnimPlay(false);	// UI Animation 재생
		iter->Set_Disappear(true);	// UI 사라짐 Off
	}
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
		if (object.contains("AlphaTrue"))
			tUI_Info.fAlphaTrue = object["AlphaTrue"];				// 0. Alpha
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
		if (object.contains("ColorMode"))
			tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		if (object.contains("RenderGroup"))
			tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

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

		pUI_Object->Set_LifeTimeUI(true);					// LifeTime UI
		pUI_Object->Set_LifeTime(3000.f);					// UI LifeTime
		pUI_Object->Set_UIState(UISTATE::PLAYER_HUD);		// UI State

		m_vecLeftHUD.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}

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
		iter->ResetTime();			// ! (LifeTime UI일 경우) UI TimeReset
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

void CUI_Manager::LeftSkillState(const string& strUIName, SKILLSTATE eState)
{
	if (m_vecLeftHUD.empty())
		return;

	for (auto& iter : m_vecLeftHUD)
	{
		if (iter->Get_UIDesc().strUIName == strUIName)
		{
			// 아이콘 상태 변경
			dynamic_cast<CUI_Player_Skill_Icon*>(iter)->Set_SkillState(eState);
		}
	}
}

void CUI_Manager::LeftSkillUnlock(const string& strUIName, _bool bUnlock)
{
	if (m_vecLeftHUD.empty())
		return;

	for (auto& iter : m_vecLeftHUD)
	{
		if (iter->Get_UIDesc().strUIName == strUIName)
		{
			// 아이콘 상태 변경
			dynamic_cast<CUI_Player_Skill_Icon*>(iter)->Set_SkillUnlock(bUnlock);
		}
	}
}

void CUI_Manager::NonActive_RightHUD()
{
	if (m_vecRightHUD.empty())
		return;

	for (auto& iter : m_vecRightHUD)
	{
		iter->Set_Alpha(1.f);		// UI 알파값 초기화
		iter->Set_Active(false);	// UI 활성화
		iter->Set_AnimPlay(false);	// UI Animation 재생
		iter->Set_Disappear(true);	// UI 사라짐 Off
	}
}

void CUI_Manager::RightSkillState(const string& strUIName, SKILLSTATE eState)
{
	if (m_vecRightHUD.empty())
		return;

	for (auto& iter : m_vecRightHUD)
	{
		if (iter->Get_UIDesc().strUIName == strUIName)
		{
			// 아이콘 상태 변경
			dynamic_cast<CUI_Player_Skill_Icon*>(iter)->Set_SkillState(eState);
		}
	}
}

void CUI_Manager::RightSkillUnlock(const string& strUIName, _bool bUnlock)
{
	if (m_vecRightHUD.empty())
		return;

	for (auto& iter : m_vecRightHUD)
	{
		if (iter->Get_UIDesc().strUIName == strUIName)
		{
			// 아이콘 해금
			dynamic_cast<CUI_Player_Skill_Icon*>(iter)->Set_SkillUnlock(bUnlock);
		}
	}

	if (m_vecRightSkill.empty())
		return;

	for (auto& iter : m_vecRightSkill)
	{
		if (iter->Get_UIDesc().strUIName == strUIName)
		{
			// 스킬게이지도 같이 해금
			dynamic_cast<CUI_Player_Skill_Icon*>(iter)->Set_SkillUnlock(bUnlock);
		}
	}
}

HRESULT CUI_Manager::Add_LeftSkill(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pOwner)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/PlayerInterface/LeftSkillGuige.json";

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
		if (object.contains("AlphaTrue"))
			tUI_Info.fAlphaTrue = object["AlphaTrue"];				// 0. Alpha
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
		if (object.contains("ColorMode"))
			tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		if (object.contains("RenderGroup"))
			tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

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

		pUI_Object->Set_LifeTimeUI(true);					// LifeTime UI
		pUI_Object->Set_LifeTime(3000.f);					// UI LifeTime
		pUI_Object->Set_UIState(UISTATE::PLAYER_HUD);		// UI State

		m_vecLeftSkill.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}

	NonActive_LeftSkill();

	return S_OK;
}

void CUI_Manager::Active_LeftSkill()
{
	if (m_vecLeftSkill.empty())
		return;

	for (auto& iter : m_vecLeftSkill)
	{
		iter->Set_Alpha(0.f);		// UI 알파값 초기화
		iter->Set_Active(true);		// UI 활성화
		iter->Set_AnimPlay(true);	// UI Animation 재생
		iter->Set_Disappear(false); // UI 사라짐 Off
	}
}

void CUI_Manager::NonActive_LeftSkill()
{
	if (m_vecLeftSkill.empty())
		return;

	for (auto& iter : m_vecLeftSkill)
	{
		iter->Set_Alpha(1.f);		// UI 알파값 초기화
		iter->Set_Active(false);	// UI 활성화
		iter->Set_AnimPlay(false);	// UI Animation 재생
		iter->Set_Disappear(true);	// UI 사라짐 Off
	}
}

CUI* CUI_Manager::Get_LeftHUD(const string& strUIName)
{
	if (m_vecLeftSkill.empty())
		return nullptr; // UI가 없음 (생성안됨)

	for (auto& iter : m_vecLeftSkill)
	{
		if (iter->Get_UIDesc().strUIName == strUIName)
			return iter;
	}

	return nullptr; // (못찾음)
}

// 현재 쿨타임을 변경합니다.
void CUI_Manager::Change_LeftHUD_CurrentCoolTime(const string& strUIName, _float fCoolTime)
{
	if (m_vecLeftSkill.empty())
		return; // UI가 없음 (생성안됨)

	for (auto& iter : m_vecLeftSkill)
	{
		if (iter->Get_UIDesc().strUIName == strUIName)
		{
			dynamic_cast<CUI_Player_Skill_Guige*>(iter)->Set_CurCoolTime(fCoolTime);
			return;
		}
	}
}

// 최대 쿨타임을 변경합니다.
void CUI_Manager::Change_LeftHUD_MaxCoolTime(const string& strUIName, _float fCoolTime)
{
	if (m_vecLeftSkill.empty())
		return; // UI가 없음 (생성안됨)

	for (auto& iter : m_vecLeftSkill)
	{
		if (iter->Get_UIDesc().strUIName == strUIName)
		{
			dynamic_cast<CUI_Player_Skill_Guige*>(iter)->Set_MaxCoolTime(fCoolTime);
			return;
		}
	}
}

_float CUI_Manager::Get_LeftHUD_CurrentCoolTime(const string& strUIName)
{
	if (m_vecLeftSkill.empty())
		return  _float(); // UI가 없음 (생성안됨)

	for (auto& iter : m_vecLeftSkill)
	{
		if (iter->Get_UIDesc().strUIName == strUIName)
		{
			return  dynamic_cast<CUI_Player_Skill_Guige*>(iter)->Get_CurCoolTime();
		}
	}

	return _float();
}

_float CUI_Manager::Get_LeftHUD_MaxCoolTime(const string& strUIName)
{
	if (m_vecLeftSkill.empty())
		return  _float(); // UI가 없음 (생성안됨)

	for (auto& iter : m_vecLeftSkill)
	{
		if (iter->Get_UIDesc().strUIName == strUIName)
		{
			return  dynamic_cast<CUI_Player_Skill_Guige*>(iter)->Get_MaxCoolTime();
		}
	}

	return _float();
}

// 스킬 해금
void CUI_Manager::Change_LeftHUD_SkillUnlock(const string& strUIName, _bool bUnlock)
{
	if (m_vecLeftHUD.empty())
		return; // UI가 없음 (생성안됨)

	for (auto& iter : m_vecLeftHUD)
	{
		if (iter->Get_UIDesc().strUIName == strUIName)
		{
			dynamic_cast<CUI_Player_Skill_Icon*>(iter)->Set_SkillUnlock(bUnlock);
			return;
		}
	}
}

HRESULT CUI_Manager::Add_RightSkill(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pOwner)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/PlayerInterface/RightSkillGuige.json";

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
		if (object.contains("AlphaTrue"))
			tUI_Info.fAlphaTrue = object["AlphaTrue"];				// 0. Alpha
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
		if (object.contains("ColorMode"))
			tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		if (object.contains("RenderGroup"))
			tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

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

		pUI_Object->Set_LifeTimeUI(true);					// LifeTime UI
		pUI_Object->Set_LifeTime(3000.f);					// UI LifeTime
		pUI_Object->Set_UIState(UISTATE::PLAYER_HUD);		// UI State

		m_vecRightSkill.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}

	NonActive_RightSkill();

	return S_OK;
}

void CUI_Manager::Active_RightSkill()
{
	if (m_vecRightSkill.empty())
		return;

	for (auto& iter : m_vecRightSkill)
	{
		iter->Set_Alpha(0.f);		// UI 알파값 초기화
		iter->Set_Active(true);		// UI 활성화
		iter->Set_AnimPlay(true);	// UI Animation 재생
		iter->Set_Disappear(false); // UI 사라짐 Off
	}
}

void CUI_Manager::NonActive_RightSkill()
{
	if (m_vecRightSkill.empty())
		return;

	for (auto& iter : m_vecRightSkill)
	{
		iter->Set_Alpha(1.f);		// UI 알파값 초기화
		iter->Set_Active(false);	// UI 활성화
		iter->Set_AnimPlay(false);	// UI Animation 재생
		iter->Set_Disappear(true);	// UI 사라짐 Off
	}
}

CUI* CUI_Manager::Get_RightHUD(const string& strUIName)
{
	if (m_vecRightSkill.empty())
		return nullptr; // UI가 없음 (생성안됨)

	for (auto& iter : m_vecRightSkill)
	{
		if (iter->Get_UIDesc().strUIName == strUIName)
			return iter;
	}

	return nullptr; // (못찾음)
}

// 현재 쿨타임을 변경합니다.
void CUI_Manager::Change_RightHUD_CurrentCoolTime(const string& strUIName, _float fCoolTime)
{
	if (m_vecRightSkill.empty())
		return; // UI가 없음 (생성안됨)

	for (auto& iter : m_vecRightSkill)
	{
		if (iter->Get_UIDesc().strUIName == strUIName)
		{
			dynamic_cast<CUI_Player_Skill_Guige*>(iter)->Set_CurCoolTime(fCoolTime);
			return;
		}
	}
}

// 최대 쿨타임을 변경합니다.
void CUI_Manager::Change_RightHUD_MaxCoolTime(const string& strUIName, _float fCoolTime)
{
	if (m_vecRightSkill.empty())
		return; // UI가 없음 (생성안됨)

	for (auto& iter : m_vecRightSkill)
	{
		if (iter->Get_UIDesc().strUIName == strUIName)
		{
			dynamic_cast<CUI_Player_Skill_Guige*>(iter)->Set_MaxCoolTime(fCoolTime);
			return;
		}
	}
}

_float CUI_Manager::Get_RightHUD_CurrentCoolTime(const string& strUIName)
{
	if (m_vecRightSkill.empty())
		return  _float(); // UI가 없음 (생성안됨)

	for (auto& iter : m_vecRightSkill)
	{
		if (iter->Get_UIDesc().strUIName == strUIName)
		{
			return  dynamic_cast<CUI_Player_Skill_Guige*>(iter)->Get_MaxCoolTime();
		}
	}

	return _float();
}

_float CUI_Manager::Get_RightHUD_MaxCoolTime(const string& strUIName)
{
	if (m_vecRightSkill.empty())
		return  _float(); // UI가 없음 (생성안됨)

	for (auto& iter : m_vecRightSkill)
	{
		if (iter->Get_UIDesc().strUIName == strUIName)
		{
			return  dynamic_cast<CUI_Player_Skill_Guige*>(iter)->Get_MaxCoolTime();
		}
	}

	return _float();
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
		if (object.contains("AlphaTrue"))
			tUI_Info.fAlphaTrue = object["AlphaTrue"];				// 0. Alpha
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
		if (object.contains("ColorMode"))
			tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		if (object.contains("RenderGroup"))
			tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

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

		pUI_Object->Set_LifeTimeUI(true);					// LifeTime UI
		pUI_Object->Set_LifeTime(3000.f);					// UI LifeTime
		pUI_Object->Set_UIState(UISTATE::PLAYER_HUD);		// UI State

		m_vecRightHUD.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}

	//Active_RightHUD(); // 생성후 최초 한번 켜주기

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
		iter->ResetTime();			// ! (LifeTime UI일 경우) UI TimeReset
	}
}

void CUI_Manager::Change_RightHUD_SkillUnlock(const string& strUIName, _bool bUnlock)
{
	if (m_vecRightHUD.empty())
		return; // UI가 없음 (생성안됨)

	for (auto& iter : m_vecRightHUD)
	{
		if (iter->Get_UIDesc().strUIName == strUIName)
		{
			dynamic_cast<CUI_Player_Skill_Icon*>(iter)->Set_SkillUnlock(bUnlock);
			return;
		}
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
		if (object.contains("AlphaTrue"))
			tUI_Info.fAlphaTrue = object["AlphaTrue"];				// 0. Alpha
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
		if (object.contains("ColorMode"))
			tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		if (object.contains("RenderGroup"))
			tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

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

		pUI_Object->Set_LifeTimeUI(true);					// LifeTime UI
		pUI_Object->Set_LifeTime(3000.f);					// UI LifeTime
		pUI_Object->Set_UIState(UISTATE::PLAYER_HUD);		// UI State

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
		iter->ResetTime();			// ! (LifeTime UI일 경우) UI TimeReset
	}
}

void CUI_Manager::NonActive_TutorialBox()
{
	if (m_vecTutorialBox.empty())
		return;

	for (auto& iter : m_vecTutorialBox)
	{
		iter->Set_Alpha(1.f);		// UI 알파값 초기화
		iter->Set_Active(false);	// UI 활성화
		iter->Set_AnimPlay(false);	// UI Animation 재생
		iter->Set_Disappear(true);	// UI 사라짐 Off
	}
}

void CUI_Manager::Change_TutorialText(TUTORIAL_TEXT eChangeText)
{
	if (m_vecTutorialBox.empty())
		return;

	for (auto& iter : m_vecTutorialBox)
	{
		if (iter->Get_UIDesc().strCloneTag == "Prototype_GameObject_UI_TutorialBox")
			dynamic_cast<CUI_TutorialBox*>(iter)->Change_TutorialMassege((_int)eChangeText);
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
		if (object.contains("AlphaTrue"))
			tUI_Info.fAlphaTrue = object["AlphaTrue"];				// 0. Alpha
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
		if (object.contains("ColorMode"))
			tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		if (object.contains("RenderGroup"))
			tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

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

void CUI_Manager::NonActive_LevelUp()
{
	if (m_vecLevelUP.empty())
		return;

	for (auto& iter : m_vecLevelUP)
	{
		iter->Set_Alpha(1.f);		// UI 알파값 초기화
		iter->Set_Active(false);		// UI 활성화
		iter->Set_AnimPlay(false);	// UI Animation 재생
		iter->Set_Disappear(true); // UI 사라짐 Off
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
		if (object.contains("AlphaTrue"))
			tUI_Info.fAlphaTrue = object["AlphaTrue"];				// 0. Alpha
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
		if (object.contains("ColorMode"))
			tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		if (object.contains("RenderGroup"))
			tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

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

void CUI_Manager::NonActive_RewardBox()
{
	if (m_vecRewardBox.empty())
		return;

	for (auto& iter : m_vecRewardBox)
	{
		iter->Set_Alpha(1.f);		// UI 알파값 초기화
		iter->Set_Active(false);		// UI 활성화
		iter->Set_AnimPlay(false);	// UI Animation 재생
		iter->Set_Disappear(true); // UI 사라짐 Off
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
		if (object.contains("AlphaTrue"))
			tUI_Info.fAlphaTrue = object["AlphaTrue"];				// 0. Alpha
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
		if (object.contains("ColorMode"))
			tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		if (object.contains("RenderGroup"))
			tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

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
		if (object.contains("AlphaTrue"))
			tUI_Info.fAlphaTrue = object["AlphaTrue"];				// 0. Alpha
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
		if (object.contains("ColorMode"))
			tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		if (object.contains("RenderGroup"))
			tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

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

void CUI_Manager::NonActive_Distortion()
{
	if (m_vecDistortion.empty())
		return;

	for (auto& iter : m_vecDistortion)
	{
		iter->Set_Alpha(1.f);			// UI 알파값 초기화
		iter->Set_Active(false);		// UI 활성화
		iter->Set_AnimPlay(false);		// UI Animation 재생
		iter->Set_Disappear(true);		// UI 사라짐 Off
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
		if (object.contains("AlphaTrue"))
			tUI_Info.fAlphaTrue = object["AlphaTrue"];				// 0. Alpha
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
		if (object.contains("ColorMode"))
			tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		if (object.contains("RenderGroup"))
			tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

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

void CUI_Manager::NonActive_Loading_Intro()
{
	if (m_vecLoading.empty())
		return;

	for (auto& iter : m_vecLoading)
	{
		iter->Set_Alpha(1.f);			// UI 알파값 초기화
		iter->Set_Active(false);		// UI 활성화
		iter->Set_AnimPlay(false);		// UI Animation 재생
		iter->Set_Disappear(true);		// UI 사라짐 Off
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
		if (object.contains("AlphaTrue"))
			tUI_Info.fAlphaTrue = object["AlphaTrue"];				// 0. Alpha
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
		if (object.contains("ColorMode"))
			tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		if (object.contains("RenderGroup"))
			tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

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

void CUI_Manager::NonActive_Loading_IntroBoss()
{
	if (m_vecLoadingIntroBoss.empty())
		return;

	for (auto& iter : m_vecLoadingIntroBoss)
	{
		iter->Set_Alpha(1.f);			// UI 알파값 초기화
		iter->Set_Active(false);		// UI 활성화
		iter->Set_AnimPlay(false);		// UI Animation 재생
		iter->Set_Disappear(true);		// UI 사라짐 Off
	}
}

HRESULT CUI_Manager::Add_Loading_SnowMountain(_uint iLevelIndex, const wstring& strLayerTag)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/Loading/SnowMountainLoading.json";

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
		if (object.contains("AlphaTrue"))
			tUI_Info.fAlphaTrue = object["AlphaTrue"];				// 0. Alpha
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
		if (object.contains("ColorMode"))
			tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		if (object.contains("RenderGroup"))
			tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

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

		m_vecLoadingSnowMountain.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}
	return S_OK;
}

void CUI_Manager::Active_Loading_SnowMountain(_bool bActive)
{
	if (m_vecLoadingSnowMountain.empty())
		return;

	for (auto& iter : m_vecLoadingSnowMountain)
	{
		iter->Set_Alpha(0.f);			// UI 알파값 초기화
		iter->Set_Active(true);		// UI 활성화
		iter->Set_AnimPlay(true);	// UI Animation 재생
		iter->Set_Disappear(false);	// UI 사라짐 Off
	}
}

void CUI_Manager::NonActive_Loading_SnowMountain()
{
	if (m_vecLoadingSnowMountain.empty())
		return;

	for (auto& iter : m_vecLoadingSnowMountain)
	{
		iter->Set_Alpha(1.f);			// UI 알파값 초기화
		iter->Set_Active(false);		// UI 활성화
		iter->Set_AnimPlay(false);		// UI Animation 재생
		iter->Set_Disappear(true);		// UI 사라짐 Off
	}
}

HRESULT CUI_Manager::Add_Loading_SnowMountainBoss(_uint iLevelIndex, const wstring& strLayerTag)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/Loading/SnowMountainBossLoading.json";

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
		if (object.contains("AlphaTrue"))
			tUI_Info.fAlphaTrue = object["AlphaTrue"];				// 0. Alpha
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
		if (object.contains("ColorMode"))
			tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		if (object.contains("RenderGroup"))
			tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

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

		m_vecLoadingSnowMountainBoss.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}
	return S_OK;
}

void CUI_Manager::Active_Loading_SnowMountainBoss(_bool bActive)
{
	if (m_vecLoadingSnowMountainBoss.empty())
		return;

	for (auto& iter : m_vecLoadingSnowMountainBoss)
	{
		iter->Set_Alpha(0.f);			// UI 알파값 초기화
		iter->Set_Active(true);		// UI 활성화
		iter->Set_AnimPlay(true);	// UI Animation 재생
		iter->Set_Disappear(false);	// UI 사라짐 Off
	}
}

void CUI_Manager::NonActive_Loading_SnowMountainBoss()
{
	if (m_vecLoadingSnowMountainBoss.empty())
		return;

	for (auto& iter : m_vecLoadingSnowMountainBoss)
	{
		iter->Set_Alpha(1.f);			// UI 알파값 초기화
		iter->Set_Active(false);		// UI 활성화
		iter->Set_AnimPlay(false);		// UI Animation 재생
		iter->Set_Disappear(true);		// UI 사라짐 Off
	}
}

HRESULT CUI_Manager::Add_Loading_ToolLevel(_uint iLevelIndex, const wstring& strLayerTag)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/Loading/ToolLoading.json";

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
		if (object.contains("AlphaTrue"))
			tUI_Info.fAlphaTrue = object["AlphaTrue"];				// 0. Alpha
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
		if (object.contains("ColorMode"))
			tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		if (object.contains("RenderGroup"))
			tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

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

		m_vecLoadingTool.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}
	return S_OK;
}

void CUI_Manager::Active_Loading_ToolLevel(_bool bActive)
{
	if (m_vecLoadingTool.empty())
		return;

	for (auto& iter : m_vecLoadingTool)
	{
		iter->Set_Alpha(0.f);			// UI 알파값 초기화
		iter->Set_Active(true);		// UI 활성화
		iter->Set_AnimPlay(true);	// UI Animation 재생
		iter->Set_Disappear(false);	// UI 사라짐 Off
	}
}

void CUI_Manager::NonActive_Loading_ToolLevel()
{
	if (m_vecLoadingTool.empty())
		return;

	for (auto& iter : m_vecLoadingTool)
	{
		iter->Set_Alpha(1.f);			// UI 알파값 초기화
		iter->Set_Active(false);		// UI 활성화
		iter->Set_AnimPlay(false);		// UI Animation 재생
		iter->Set_Disappear(true);		// UI 사라짐 Off
	}
}

HRESULT CUI_Manager::Add_Loading_TestLevel(_uint iLevelIndex, const wstring& strLayerTag)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/Loading/TestLevelLoading.json";

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
		if (object.contains("AlphaTrue"))
			tUI_Info.fAlphaTrue = object["AlphaTrue"];				// 0. Alpha
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
		if (object.contains("ColorMode"))
			tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		if (object.contains("RenderGroup"))
			tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

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

		m_vecLoadingTest.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}
	return S_OK;
}

void CUI_Manager::Active_Loading_TestLevel(_bool bActive)
{
	if (m_vecLoadingTest.empty())
		return;

	for (auto& iter : m_vecLoadingTest)
	{
		iter->Set_Alpha(0.f);			// UI 알파값 초기화
		iter->Set_Active(true);		// UI 활성화
		iter->Set_AnimPlay(true);	// UI Animation 재생
		iter->Set_Disappear(false);	// UI 사라짐 Off
	}
}

void CUI_Manager::NonActive_Loading_TestLevel()
{
	if (m_vecLoadingTest.empty())
		return;

	for (auto& iter : m_vecLoadingTest)
	{
		iter->Set_Alpha(1.f);			// UI 알파값 초기화
		iter->Set_Active(false);		// UI 활성화
		iter->Set_AnimPlay(false);		// UI Animation 재생
		iter->Set_Disappear(true);		// UI 사라짐 Off
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
		if (object.contains("AlphaTrue"))
			tUI_Info.fAlphaTrue = object["AlphaTrue"];				// 0. Alpha
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
		if (object.contains("ColorMode"))
			tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		if (object.contains("RenderGroup"))
			tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

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

void CUI_Manager::NonActive_Crosshair()
{
	if (m_vecCrosshair.empty())
		return;

	for (auto& iter : m_vecCrosshair)
	{
		iter->Set_Alpha(1.f);			// UI 알파값 초기화
		iter->Set_Active(false);		// UI 활성화
		iter->Set_AnimPlay(false);		// UI Animation 재생
		iter->Set_Disappear(true);		// UI 사라짐 Off
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

HRESULT CUI_Manager::Add_BossHUD_Bar(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pOwner, const string& strBossName)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	if (strBossName == "") // MessageBox No Name
		MessageBox(g_hWnd, L"HUD를 가질 보스의 이름을 넣어주셔야 합니다.", L"보스HUD UI 생성 실패", MB_OK);

	if (strBossName == "VampireCommander")	// Stage1 Boss
		strFile = "../Bin/DataFiles/Data_UI/BossHUD/BossHUD_VampireCommander.json";
	else if (strBossName == "TheParasiter") // Stage2 Boss
		strFile = "../Bin/DataFiles/Data_UI/BossHUD/BossHUD_TheParasiter.json";
	
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
		if (object.contains("AlphaTrue"))
			tUI_Info.fAlphaTrue = object["AlphaTrue"];				// 0. Alpha
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
		if (object.contains("ColorMode"))
			tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		if (object.contains("RenderGroup"))
			tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

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

void CUI_Manager::NonActive_BossHUD_Bar()
{
	if (m_vecBossHUD_Bar.empty())
		return;

	for (auto& iter : m_vecBossHUD_Bar)
	{
		iter->Set_Alpha(1.f);			// UI 알파값 초기화
		iter->Set_Active(false);		// UI 활성화
		iter->Set_AnimPlay(false);		// UI Animation 재생
		iter->Set_Disappear(true);		// UI 사라짐 Off
	}
}

HRESULT CUI_Manager::Add_BossHUD_Shard(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pOwner)
{

	return S_OK;
}

void CUI_Manager::Active_BossHUD_Shard(_bool bActive)
{

}

void CUI_Manager::NonActive_BossHUD_Shard()
{
	if (m_vecBossHUD_Shard.empty())
		return;

	for (auto& iter : m_vecBossHUD_Shard)
	{
		iter->Set_Alpha(1.f);			// UI 알파값 초기화
		iter->Set_Active(false);		// UI 활성화
		iter->Set_AnimPlay(false);		// UI Animation 재생
		iter->Set_Disappear(true);		// UI 사라짐 Off
	}
}

void CUI_Manager::Dead_BossHUD_Shard(_bool bDeadOwner)
{
	//if (m_vecBossHUD_Shard.empty())
	//	return;

	//for (auto& iter : m_vecBossHUD_Shard)
	//{
	//	dynamic_cast<CUI_EnemyHP_Shard*>(iter)->Set_Dead_Owner(bDeadOwner);			// UI 알파값 초기화
	//}
}

CUI_EnemyHUD_Shard* CUI_Manager::Add_EnemyHUD_Shard(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pOwner)
{
	json json_in;

	//char filePath[MAX_PATH];

	if (pOwner == nullptr) // MessageBox No Name
		MessageBox(g_hWnd, L"pOwner가 nullptr입니다.", L"몬스터 HUD 생성 실패", MB_OK);

	// 부모 생성
	CGameObject* pGameObject = m_pGameInstance->Add_CloneObject_And_Get(iLevelIndex, strLayerTag, TEXT("Prototype_GameObject_UI_EnemyHUD_Shard"));
	
	if (pGameObject == nullptr)
		return nullptr;

	CUI* pUI_Object = dynamic_cast<CUI*>(pGameObject);
	
	if (pUI_Object == nullptr)
		return nullptr;

	pUI_Object->Set_Object_Owner(pOwner);

	///* HP Bar */
	//string strCloneTag_HPBar = "Prototype_GameObject_UI_EnemyHP_Shard";
	//if (pUI_Object->Get_UIDesc().strCloneTag == strCloneTag_HPBar)
	//{
	//	dynamic_cast<CUI_EnemyHP_Shard*>(pUI_Object)->Setting_Owner();
	//}

	CUI_EnemyHUD_Shard* pEnemyHUD = dynamic_cast<CUI_EnemyHUD_Shard*>(pUI_Object);

	/* 부품들 생성 */
	pEnemyHUD->Ready_ChildHUD();

	return pEnemyHUD;
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

void CUI_Manager::NonActive_EnemyHUD_Shard()
{
	if (m_vecEnemyHUD_Shard.empty())
		return;

	for (auto& iter : m_vecEnemyHUD_Shard)
	{
		iter->Set_Alpha(1.f);			// UI 알파값 초기화
		iter->Set_Active(false);		// UI 활성화
		iter->Set_AnimPlay(false);		// UI Animation 재생
		iter->Set_Disappear(true);		// UI 사라짐 Off
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
		if (object.contains("AlphaTrue"))
			tUI_Info.fAlphaTrue = object["AlphaTrue"];				// 0. Alpha
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
		if (object.contains("ColorMode"))
			tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		if (object.contains("RenderGroup"))
			tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

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

		m_vecDiedScreen.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data

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
		if (object.contains("AlphaTrue"))
			tUI_Info.fAlphaTrue = object["AlphaTrue"];				// 0. Alpha
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
		if (object.contains("ColorMode"))
			tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		if (object.contains("RenderGroup"))
			tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

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

		m_vecOption.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
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

HRESULT CUI_Manager::Add_MainMenu(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pOwner)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/MainMenu/MainMenu.json";

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
		if (object.contains("AlphaTrue"))
			tUI_Info.fAlphaTrue = object["AlphaTrue"];				// 0. Alpha
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
		if (object.contains("ColorMode"))
			tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		if (object.contains("RenderGroup"))
			tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

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

		m_vecMainMenu.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}

	return S_OK;
}

void CUI_Manager::Active_MainMenu()
{
	if (m_vecMainMenu.empty())
		return;

	for (auto& iter : m_vecMainMenu)
	{
		iter->Set_Alpha(0.f);		// UI 알파값 초기화
		iter->Set_Active(true);		// UI 활성화
		iter->Set_AnimPlay(true);	// UI Animation 재생
		iter->Set_CurrTime(0.f);	// UI Animation 시간 초기화
		iter->Set_Disappear(false);	// UI 사라짐 Off
	}
}

void CUI_Manager::NonActive_MainMenu()
{
	if (m_vecMainMenu.empty())
		return;

	for (auto& iter : m_vecMainMenu)
	{
		iter->Set_Alpha(1.f);		// UI 알파값 초기화
		iter->Set_Active(false);	// UI 활성화
		iter->Set_AnimPlay(false);	// UI Animation 재생
		iter->Set_CurrTime(0.f);	// UI Animation 시간 초기화
		iter->Set_Disappear(true);	// UI 사라짐 Off
	}
}

HRESULT CUI_Manager::Add_MainList(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pOwner)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/MainMenu/MainList.json";

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
		if (object.contains("AlphaTrue"))
			tUI_Info.fAlphaTrue = object["AlphaTrue"];				// 0. Alpha
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
		if (object.contains("ColorMode"))
			tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		if (object.contains("RenderGroup"))
			tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

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

		m_vecMainList.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}

	return S_OK;
}

void CUI_Manager::Active_MainList()
{
	if (m_vecMainList.empty())
		return;

	for (auto& iter : m_vecMainList)
	{
		iter->Set_Alpha(0.f);		// UI 알파값 초기화
		iter->Set_Active(true);		// UI 활성화
		iter->Set_AnimPlay(true);	// UI Animation 재생
		iter->Set_CurrTime(0.f);	// UI Animation 시간 초기화
		iter->Set_Disappear(false);	// UI 사라짐 Off
	}
}

void CUI_Manager::NonActive_MainList()
{
	if (m_vecMainList.empty())
		return;

	for (auto& iter : m_vecMainList)
	{
		iter->Set_Alpha(1.f);		// UI 알파값 초기화
		iter->Set_Active(false);	// UI 활성화
		iter->Set_AnimPlay(false);	// UI Animation 재생
		iter->Set_CurrTime(0.f);	// UI Animation 시간 초기화
		iter->Set_Disappear(true);	// UI 사라짐 Off
	}
}

HRESULT CUI_Manager::Add_LevelList(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pOwner)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/MainMenu/LevelList.json";

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
		if (object.contains("AlphaTrue"))
			tUI_Info.fAlphaTrue = object["AlphaTrue"];				// 0. Alpha
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
		if (object.contains("ColorMode"))
			tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		if (object.contains("RenderGroup"))
			tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

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

		m_vecLevelList.push_back(pUI_Object);

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}

	return S_OK;
}

void CUI_Manager::Active_LevelList()
{
	if (m_vecLevelList.empty())
		return;

	for (auto& iter : m_vecLevelList)
	{
		iter->Set_Alpha(0.f);		// UI 알파값 초기화
		iter->Set_Active(true);		// UI 활성화
		iter->Set_AnimPlay(true);	// UI Animation 재생
		iter->Set_CurrTime(0.f);	// UI Animation 시간 초기화
		iter->Set_Disappear(false);	// UI 사라짐 Off
	}
}

void CUI_Manager::NonActive_LevelList()
{
	if (m_vecLevelList.empty())
		return;

	for (auto& iter : m_vecLevelList)
	{
		iter->Set_Alpha(1.f);		// UI 알파값 초기화
		iter->Set_Active(false);	// UI 활성화
		iter->Set_AnimPlay(false);	// UI Animation 재생
		iter->Set_CurrTime(0.f);	// UI Animation 시간 초기화
		iter->Set_Disappear(true);	// UI 사라짐 Off
	}
}

HRESULT CUI_Manager::Add_MainLogo(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pOwner)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/MainMenu/MainLogo.json";

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
		if (object.contains("AlphaTrue"))
			tUI_Info.fAlphaTrue = object["AlphaTrue"];				// 0. Alpha
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
		if (object.contains("ColorMode"))
			tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		if (object.contains("RenderGroup"))
			tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

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

		m_pMainLogo = pUI_Object;

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}

	return S_OK;
}

void CUI_Manager::Active_MainLogo()
{
	if (m_pMainLogo == nullptr)
		return;

	m_pMainLogo->Set_Alpha(0.f);		// UI 알파값 초기화
	m_pMainLogo->Set_Active(true);		// UI 활성화
	m_pMainLogo->Set_AnimPlay(true);	// UI Animation 재생
	m_pMainLogo->Set_CurrTime(0.f);		// UI Animation 시간 초기화
	m_pMainLogo->Set_Disappear(false);	// UI 사라짐 Off
}

void CUI_Manager::NonActive_MainLogo()
{
	if (m_pMainLogo == nullptr)
		return;

	m_pMainLogo->Set_Alpha(1.f);		// UI 알파값 초기화
	m_pMainLogo->Set_Active(false);		// UI 활성화
	m_pMainLogo->Set_AnimPlay(false);	// UI Animation 재생
	m_pMainLogo->Set_CurrTime(0.f);		// UI Animation 시간 초기화
	m_pMainLogo->Set_Disappear(true);	// UI 사라짐 Off
}

HRESULT CUI_Manager::Add_MouseCursor(_uint iLevelIndex, const wstring& strLayerTag)
{
	CGameObject* pGameObject = m_pGameInstance->Add_CloneObject_And_Get(iLevelIndex, strLayerTag, TEXT("Prototype_GameObject_UI_MouseCursor"));
	if (pGameObject == nullptr)
		return E_FAIL;

	CUI* pUI_Object = dynamic_cast<CUI*>(pGameObject);
	if (pUI_Object == nullptr)
		return E_FAIL;

	m_pMouseCursor = pUI_Object;

	return S_OK;
}

void CUI_Manager::Active_MouseCursor()
{
	if (m_pMouseCursor == nullptr)
		return;

	m_pMouseCursor->Set_Alpha(0.f);			// UI 알파값 초기화
	m_pMouseCursor->Set_Active(true);		// UI 활성화
	m_pMouseCursor->Set_AnimPlay(true);		// UI Animation 재생
	m_pMouseCursor->Set_CurrTime(0.f);		// UI Animation 시간 초기화
	m_pMouseCursor->Set_Disappear(false);	// UI 사라짐 Off
}

void CUI_Manager::NonActive_MouseCursor()
{
	if (m_pMouseCursor == nullptr)
		return;

	m_pMouseCursor->Set_Alpha(1.f);			// UI 알파값 초기화
	m_pMouseCursor->Set_Active(false);		// UI 활성화
	m_pMouseCursor->Set_AnimPlay(false);	// UI Animation 재생
	m_pMouseCursor->Set_CurrTime(0.f);		// UI Animation 시간 초기화
	m_pMouseCursor->Set_Disappear(true);	// UI 사라짐 Off
}

HRESULT CUI_Manager::Add_HitUI(_uint iLevelIndex, const wstring& strLayerTag)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = "../Bin/DataFiles/Data_UI/PlayerInterface/Blood.json";

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
		if (object.contains("AlphaTrue"))
			tUI_Info.fAlphaTrue = object["AlphaTrue"];				// 0. Alpha
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
		if (object.contains("ColorMode"))
			tUI_Info.eColorMode = object["ColorMode"];				// 16. Mode
		if (object.contains("RenderGroup"))
			tUI_Info.iRenderGroup = object["RenderGroup"];			// 16. RenderGroup

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

		m_pHitUI = pUI_Object;

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom
		pUI_Object->Load_FromJson(object); // 18. Load Data
	}

	return S_OK;
}

void CUI_Manager::Active_HitUI()
{
	if (m_pHitUI == nullptr)
		return;

	m_pHitUI->Set_Alpha(0.f);			// UI 알파값 초기화
	m_pHitUI->Set_Active(true);		// UI 활성화
	m_pHitUI->Set_AnimPlay(true);		// UI Animation 재생
	m_pHitUI->Set_CurrTime(0.f);		// UI Animation 시간 초기화
	m_pHitUI->Set_Disappear(false);	// UI 사라짐 Off
}

void CUI_Manager::NonActive_HitUI()
{
	if (m_pHitUI == nullptr)
		return;

	m_pHitUI->Set_Alpha(1.f);			// UI 알파값 초기화
	m_pHitUI->Set_Active(false);		// UI 활성화
	m_pHitUI->Set_AnimPlay(false);		// UI Animation 재생
	m_pHitUI->Set_CurrTime(0.f);		// UI Animation 시간 초기화
	m_pHitUI->Set_Disappear(true);		// UI 사라짐 Off
}

CUI_Interaction* CUI_Manager::Add_Interaction(_uint iLevelIndex, const string& strUIName, const string& strProtoTag)
{
	CUI::UI_DESC tUI_Info;

	tUI_Info.strUIName = strUIName;
	tUI_Info.strProtoTag = strProtoTag;

	CGameObject* pGameObject = m_pGameInstance->Add_CloneObject_And_Get(iLevelIndex, TEXT("Layer_UI_Interaction"), TEXT("Prototype_GameObject_UI_Interaction"), &tUI_Info);

	CUI_Interaction* pInteraction = dynamic_cast<CUI_Interaction*>(pGameObject);

	return pInteraction;
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
	if (Out_Json.contains("AlphaTrue"))
		tUI_Info->fAlphaTrue = Out_Json["AlphaTrue"];				// 0. Alpha
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

void CUI_Manager::Active_UI()
{

}

void CUI_Manager::NonActive_UI()
{
	NonActive_LeftHUD();
	NonActive_RightHUD();
	NonActive_TutorialBox();
	NonActive_LevelUp();
	NonActive_RewardBox();
	NonActive_EnemyHUD_Shard();
	NonActive_DiedScreen();
	NonActive_Option();
	NonActive_BossHUD_Bar();
}

void CUI_Manager::Check_UIPicking(_float fTimeDelta)
{
	/* Option */ // 전체 다 꺼져있다가 마우스 클릭시 전부 같은녀석으로 켜짐 (확인 필요)
	if (!m_vecOption.empty())
	{
		if (m_vecOption.front()->Get_Active() == true)
		{
			for (auto& Option : m_vecOption)
			{
				m_bMouseOver = Option->Get_Pick();

				if (m_bMouseOver == true)
				{// 마우스 오버시에 모든 정보를 넘겨주고 리턴시켜야함 -> 계속 순회하며 값을 바꾸니까 오버한게 덮혀서 안나온것. (오버한 녀석이 있다면, 그 순간 선택했는지 안했는지 정보까지 주면된다.)
					m_strMouseOverUI = Option->Get_UIDesc().strUIName;
					m_strSelectUI = Option->Get_UIDesc().strUIName;
					m_bSelect = Option->Get_Select();
					m_bSelectPressing = Option->Get_SelectPressing();
					if (m_bSelect == true || m_bSelectPressing == true)
						m_pUI = Option; // 이때 선택했을 경우만 UI를 넘겨주면됨 (필요하다면 오버했을 때도 넘겨주는식으로 바꿔써도 된다.)
					else
					{
						m_pUI = nullptr;
						m_strSelectUI = "";
					}
					return;
				}
				else
				{
					m_strMouseOverUI = "";
				}
			}
		}
	}

	/*  */

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
		case Client::UITYPE::PLAYER_HUD:
			Active_PlayerHUD();
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

	if (!m_vecLeftHUD.empty())
	for (auto& iter : m_vecLeftHUD)
	{
		Safe_Release(iter);
	}
	if (!m_vecRightHUD.empty())
	for (auto& iter : m_vecRightHUD)
	{
		Safe_Release(iter);
	}
	if (!m_vecTutorialBox.empty())
	for (auto& iter : m_vecTutorialBox)
	{
		Safe_Release(iter);
	}
	if (!m_vecLevelUP.empty())
	for (auto& iter : m_vecLevelUP)
	{
		Safe_Release(iter);
	}
	if (!m_vecRewardBox.empty())
	for (auto& iter : m_vecRewardBox)
	{
		Safe_Release(iter);
	}
	if (!m_vecQuestBox.empty())
	for (auto& iter : m_vecQuestBox)
	{
		Safe_Release(iter);
	}
	if (!m_vecDistortion.empty())
	for (auto& iter : m_vecDistortion)
	{
		Safe_Release(iter);
	}
	if (!m_vecLoading.empty())
	for (auto& iter : m_vecLoading)
	{
		Safe_Release(iter);
	}
	if (!m_vecLoadingIntroBoss.empty())
	for (auto& iter : m_vecLoadingIntroBoss)
	{
		Safe_Release(iter);
	}
	if (!m_vecCrosshair.empty())
	for (auto& iter : m_vecCrosshair)
	{
		Safe_Release(iter);
	}
	if (!m_vecBossHUD_Bar.empty())
	for (auto& iter : m_vecBossHUD_Bar)
	{
		Safe_Release(iter);
	}
	if (!m_vecBossHUD_Shard.empty())
	for (auto& iter : m_vecBossHUD_Shard)
	{
		Safe_Release(iter);
	}
	if (!m_vecEnemyHUD_Shard.empty())
	for (auto& iter : m_vecEnemyHUD_Shard)
	{
		Safe_Release(iter);
	}
	if (!m_vecDiedScreen.empty())
	for (auto& iter : m_vecDiedScreen)
	{
		Safe_Release(iter);
	}
	if (!m_vecOption.empty())
	for (auto& iter : m_vecOption)
	{
		Safe_Release(iter);
	}
	if (!m_vecMainMenu.empty())
	for (auto& iter : m_vecMainMenu)
	{
		Safe_Release(iter);
	}
	if (!m_vecMainList.empty())
	for (auto& iter : m_vecMainList)
	{
		Safe_Release(iter);
	}
	if (!m_vecLevelList.empty())
	for (auto& iter : m_vecLevelList)
	{
		Safe_Release(iter);
	}
	if (!m_vecLeftSkill.empty())
	for (auto& iter : m_vecLeftSkill)
	{
		Safe_Release(iter);
	}
	if (!m_vecRightSkill.empty())
	for (auto& iter : m_vecRightSkill)
	{
		Safe_Release(iter);
	}
	if(!m_vecInterraction.empty())
	for (auto& iter : m_vecInterraction)
	{
		Safe_Release(iter);
	}
}
