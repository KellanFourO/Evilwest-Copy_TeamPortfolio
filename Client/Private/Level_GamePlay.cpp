#include "stdafx.h"
#include "Level_GamePlay.h"
#include "GameInstance.h"
#include "Player.h"
#include "Camera_Dynamic.h"
#include "Environment_Instance.h"
#include "Effect_Instance.h"

#pragma region UI
#include "UI_Anything.h"
#include "UI_MonsterHp.h"
#include "UI_MonsterHpFrame.h"
#pragma endregion

#include "LandObject.h"

#pragma region MAP
#include "Environment_Object.h"
#include "Environment_Instance.h"

#pragma endregion


#pragma region Test
#include "Screamer.h"  
#include "InstanceMonster.h"
#pragma endregion


CLevel_GamePlay::CLevel_GamePlay(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	FAILED_CHECK(Ready_LightDesc());
	FAILED_CHECK(Ready_Layer_Camera(TEXT("Layer_Camera")));
	FAILED_CHECK(Ready_Layer_Effect(TEXT("Layer_Effect")));
	FAILED_CHECK(Ready_Layer_BackGround(TEXT("Layer_BackGround")));
	FAILED_CHECK(Ready_LandObjects());
	FAILED_CHECK(Ready_Layer_Test(TEXT("Layer_Test")));

	if (FAILED(Ready_UI()))
		return E_FAIL;

	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{

}

HRESULT CLevel_GamePlay::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));
	
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_LightDesc()
{
	LIGHT_DESC			LightDesc{};
	{
		LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
		LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
		LightDesc.vDiffuse = _float4(0.6f, 0.6f, 0.6f, 1.f);
		LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
		LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

		FAILED_CHECK(m_pGameInstance->Add_Light(LightDesc, TempLightNumber));
	}
	{
		ZeroMemory(&LightDesc, sizeof LightDesc);

		LightDesc.eType = LIGHT_DESC::TYPE_POINT;
		LightDesc.vPosition = _float4(30.f, 3.f, 30.f, 1.f);
		LightDesc.fRange = 20.f;
		LightDesc.vDiffuse = _float4(1.f, 0.0f, 0.0f, 1.f);
		LightDesc.vAmbient = _float4(0.4f, 0.1f, 0.1f, 1.f);
		LightDesc.vSpecular = LightDesc.vDiffuse;
		FAILED_CHECK(m_pGameInstance->Add_Light(LightDesc, TempLightNumber));

		LightDesc.eType = LIGHT_DESC::TYPE_POINT;
		LightDesc.vPosition = _float4(50.f, 3.f, 30.f, 1.f);
		LightDesc.fRange = 20.f;
		LightDesc.vDiffuse = _float4(0.0f, 1.f, 0.0f, 1.f);
		LightDesc.vAmbient = _float4(0.1f, 0.4f, 0.1f, 1.f);
		LightDesc.vSpecular = LightDesc.vDiffuse;
		FAILED_CHECK(m_pGameInstance->Add_Light(LightDesc, TempLightNumber));

		LightDesc.eType = LIGHT_DESC::TYPE_POINT;
		LightDesc.vPosition = _float4(70.f, 10.f, 30.f, 1.f);
		LightDesc.fRange = 20.f;
		LightDesc.vDiffuse = _float4(1.f, 0.0f, 1.f, 1.f);
		LightDesc.vAmbient = _float4(0.4f, 0.1f, 0.4f, 1.f);
		LightDesc.vSpecular = LightDesc.vDiffuse;
		FAILED_CHECK(m_pGameInstance->Add_Light(LightDesc, TempLightNumber));
	}
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const wstring & strLayerTag)
{
	CCamera_Dynamic::DYNAMIC_CAMERA_DESC		Desc = {};

	Desc.fMouseSensor = 0.05f;
	Desc.vEye = _float4(0.f, 20.f, -15.f, 1.f);
	Desc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	Desc.fFovy = XMConvertToRadians(60.0f);
	Desc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	Desc.fNear = 0.1f;
	Desc.fFar = m_pGameInstance->Get_CamFar();
	Desc.fSpeedPerSec = 15.f;
	Desc.fRotationPerSec = XMConvertToRadians(180.0f);

	FAILED_CHECK(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Camera_Dynamic"), &Desc));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const wstring & strLayerTag, void* pArg)
{
	FAILED_CHECK(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Player"), pArg));

	//CGameObject* pPlayer = m_pGameInstance->Add_CloneObject_And_Get(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Player"), pArg);
	//if (nullptr == pPlayer)
	//	return E_FAIL;

	//m_pGameInstance->Set_Player(pPlayer);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const wstring & strLayerTag)
{
	//FAILED_CHECK(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Particle_Blue")));
	//FAILED_CHECK(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Particle_Red")));
	//FAILED_CHECK(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Effect_Explosion")));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const wstring & strLayerTag, void* pArg)
{
	//FAILED_CHECK(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Monster"), pArg));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const wstring & strLayerTag)
{
	//FAILED_CHECK(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Terrain")));
	FAILED_CHECK(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Sky")));


	json Stage1MapJson = {};

	if (FAILED(CJson_Utility::Load_Json(m_strStage1MapLoadPath.c_str(), Stage1MapJson)))
	{
		MSG_BOX("맵 불러오기 실패");
		return E_FAIL;
	}

	json BasicJson = Stage1MapJson["Basic_Json"];
	_int iBasicJsonSize = (_int)BasicJson.size();

	for (_int i = 0; i < iBasicJsonSize; ++i)
	{
		CEnvironment_Object::ENVIRONMENT_OBJECT_DESC Desc;

		Desc.bAnimModel = BasicJson[i]["AnimType"];

		wstring strLoadModelTag;
		string strJsonModelTag = BasicJson[i]["ModelTag"];

		m_pGameInstance->String_To_WString(strJsonModelTag, strLoadModelTag);
		Desc.strModelTag = strLoadModelTag;

		Desc.iShaderPassIndex = BasicJson[i]["ShaderPassIndex"];
		Desc.iPlayAnimationIndex = BasicJson[i]["PlayAnimationIndex"];
		Desc.bPreview = false;

		const json& TransformJson = BasicJson[i]["Component"]["Transform"];
		_float4x4 WorldMatrix;

		for (_int TransformLoopIndex = 0; TransformLoopIndex < 4; ++TransformLoopIndex)
		{
			for (_int TransformSecondLoopIndex = 0; TransformSecondLoopIndex < 4; ++TransformSecondLoopIndex)
			{
				WorldMatrix.m[TransformLoopIndex][TransformSecondLoopIndex] = TransformJson[TransformLoopIndex][TransformSecondLoopIndex];
			}
		}

		XMStoreFloat4(&Desc.vPos, XMLoadFloat4x4(&WorldMatrix).r[3]);
		Desc.WorldMatrix = WorldMatrix;

		CEnvironment_Object* pObject = { nullptr };

		pObject = dynamic_cast<CEnvironment_Object*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_GAMEPLAY, L"Layer_BackGround", L"Prototype_GameObject_Environment_Object", &Desc));
	}


	json InteractJson = Stage1MapJson["Interact_Json"];
	_int InteractJsonSize = InteractJson.size();

	for (_int i = 0; i < InteractJsonSize; ++i)
	{

		//TODO 추후 상호작용 오브젝트 클래스 작성  후 작업
		//! L"Layer_Event"
	}

	json InstanceJson = Stage1MapJson["Instance_Json"];
	_int InstanceJsonSize = InstanceJson.size();

	for (_int i = 0; i < InstanceJsonSize; ++i)
	{
		MAPTOOL_INSTANCE_DESC InstanceDesc;

		InstanceDesc.iNumInstance = InstanceJson[i]["InstanceCount"];


		wstring strLoadModelTag;
		string strJsonModelTag = InstanceJson[i]["ModelTag"];

		m_pGameInstance->String_To_WString(strJsonModelTag, strLoadModelTag);
		InstanceDesc.strModelTag = strLoadModelTag;

		InstanceDesc.iShaderPassIndex = InstanceJson[i]["ShaderPassIndex"];

		json InstanceInfoJson = InstanceJson[i]["InstanceInfo_Json"];
		_uint InstanceInfoJsonSize = InstanceInfoJson.size();

		for (_int j = 0; j < InstanceInfoJsonSize; ++j)
		{
			INSTANCE_INFO_DESC InstanceInfoDesc = {};

			CJson_Utility::Load_Float3(InstanceInfoJson[j]["Instance_Scale"], InstanceInfoDesc.vScale);
			CJson_Utility::Load_Float3(InstanceInfoJson[j]["Instance_Rotation"], InstanceInfoDesc.vRotation);
			CJson_Utility::Load_Float3(InstanceInfoJson[j]["Instance_Translation"], InstanceInfoDesc.vTranslation);
			CJson_Utility::Load_Float3(InstanceInfoJson[j]["Instance_Center"], InstanceInfoDesc.vCenter);

			InstanceDesc.vecInstanceInfoDesc.push_back(InstanceInfoDesc);
		}


		CEnvironment_Instance* pInstanceObject = { nullptr };

		pInstanceObject = dynamic_cast<CEnvironment_Instance*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_GAMEPLAY, L"Layer_BackGround", L"Prototype_GameObject_Environment_Instance", &InstanceDesc));

	}

	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Skydome"))))
	//	return E_FAIL;

	return S_OK;

}

HRESULT CLevel_GamePlay::Ready_LandObjects()
{
	CLandObject::LANDOBJECT_DESC	LandObjectDesc{};

	LandObjectDesc.pTerrainBuffer = dynamic_cast<CVIBuffer_Terrain*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer")));
	LandObjectDesc.pTerrainTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_Transform")));

	FAILED_CHECK(Ready_Layer_Player(TEXT("Layer_Player"), &LandObjectDesc));
	FAILED_CHECK(Ready_Layer_Monster(TEXT("Layer_Monster"), &LandObjectDesc));
	FAILED_CHECK(Ready_Layer_Building(TEXT("Layer_Building"), &LandObjectDesc));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Building(const wstring & strLayerTag, void* pArg)
{
	//FAILED_CHECK(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_ForkLift"), pArg));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Test(const wstring& strLayerTag)
{
	//FAILED_CHECK(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Interact_Chain")));
	//FAILED_CHECK(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Screamer")));

	FAILED_CHECK(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_InstanceMonster")));



	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_UI()
{
	// FAILED_CHECK(Ready_Layer_UI_Monster(TEXT("Layer_UI_Monster"), nullptr));

	FAILED_CHECK(Ready_Layer_UI_Player(TEXT("Layer_UI_Player"), nullptr));

	FAILED_CHECK(Ready_Layer_UI(TEXT("Layer_UI"), nullptr));
	
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI_Monster(const wstring& strLayerTag, void* pArg)
{
	/* 추 후 파싱해서 정보 받아오기 */
	
	json json_in;

	char filePath[MAX_PATH] = "../Bin/DataFiles/Data_UI/UI_Info";

	_int		iPathNum = 0;
	string		strFileName;
	string		strFilePath;
	
	CJson_Utility::Load_Json(filePath, json_in);

	for (auto& item : json_in.items())
	{
		json object = item.value();

		CUI::UI_DESC tUI_Info;

		tUI_Info.strProtoTag = object["ProtoTag"];
		tUI_Info.strFilePath = object["FilePath"];

		wstring wstrPrototag;
		m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

		wstring wstrFilePath;
		m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

		CUI_Anything* pUI_Object = dynamic_cast<CUI_Anything*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_STATIC, strLayerTag, TEXT("Prototype_GameObject_UI_Anything"), &tUI_Info));

		pUI_Object->Get_Transform()->Load_FromJson(object);
	}


	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STATIC, strLayerTag, TEXT("Prototype_GameObject_UI_Anything"), &json_in)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI_Player(const wstring& strLayerTag, void* pArg)
{
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(const wstring& strLayerTag, void* pArg)
{
	json json_in;

	char filePath[MAX_PATH] = "../Bin/DataFiles/Data_UI/testtesttesttesttesttest.json";

	_int		iPathNum = 0;
	string		strFileName;
	string		strFilePath;

	CJson_Utility::Load_Json(filePath, json_in);

	for (auto& item : json_in.items())
	{
		json object = item.value();

		CUI::UI_DESC tUI_Info;

		/* 저장순서랑 맞는지 확인하기 */
		tUI_Info.bParent = object["Parent"];					// 1. Parent
		tUI_Info.bWorld = object["World"];						// 2. World
		tUI_Info.bGroup = object["Group"];						// 3. Group
		tUI_Info.fAlpha = object["Alpha"];						// 4. Alpha
		tUI_Info.iObjectNum = object["ObjectNum"];				// 5. ObjectNum
		tUI_Info.iShaderNum = object["ShaderNum"];				// 6. ShaderPathNum
		tUI_Info.strObjectName = object["ObjectName"];			// 7. ObjectName
		tUI_Info.strLayerTag = object["LayerTag"];				// 8. LayerTag
		tUI_Info.strCloneTag = object["CloneTag"];				// 9. CloneTag
		tUI_Info.strProtoTag = object["ProtoTag"];				// 10. ProtoTag
		tUI_Info.strFilePath = object["FilePath"];				// 11. FilePath
		tUI_Info.strMapTextureTag = object["MapTextureTag"];	// 12. MapTexture
		tUI_Info.vColor.m128_f32[0] = object["ColorR"];			// 13. R
		tUI_Info.vColor.m128_f32[1] = object["ColorG"];			// 14. G
		tUI_Info.vColor.m128_f32[2] = object["ColorB"];			// 15. B
		tUI_Info.vColor.m128_f32[3] = object["ColorA"];			// 16. A

		wstring wstrLayer;
		m_pGameInstance->String_To_WString(tUI_Info.strLayerTag, wstrLayer); //

		wstring wstrClonetag;
		m_pGameInstance->String_To_WString(tUI_Info.strCloneTag, wstrClonetag);

		wstring wstrPrototag;
		m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

		wstring wstrFilePath;
		m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

		CUI* pUI_Object = dynamic_cast<CUI*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_STATIC, strLayerTag, wstrClonetag, &tUI_Info));

		pUI_Object->Get_Transform()->Load_FromJson(object);		// 17. TransformCom
	}

	//m_pGameInstance->Add_CloneObject(LEVEL_STATIC, strLayerTag, TEXT("Prototype_GameObject_UI_Player_Left_Interface"));

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)

{
	CLevel_GamePlay*		pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}
	return pInstance; 
}

void CLevel_GamePlay::Free()
{
	__super::Free();

}
