#include "stdafx.h"
#include "Level_Intro.h"
#include "GameInstance.h"
#include "Player.h"
#include "Camera_Dynamic.h"
#include "Environment_Instance.h"
#include "Effect_Instance.h"

#pragma region UI
#include "UI_Player_HPBar.h"
#include "UI_Anything.h"
#include "UI_MonsterHp.h"
#include "UI_MonsterHpFrame.h"
#include "UI_Manager.h"
#pragma endregion

#pragma region MAP
#include "Environment_Object.h"
#include "Environment_Instance.h"
#include "Environment_LightObject.h"
#include "Environment_SpecialObject.h"
#pragma endregion

#pragma region Test
#include "Monster.h"
#include "Screamer.h"  
#include "InstanceMonster.h"
#include "VampireCommander.h"
#pragma endregion

#pragma region Effect_Test
#include "Effect_Manager.h"
#include "Effect.h"
#pragma endregion

#include "Data_Manager.h"
#include "MasterCamera.h"
#include "SpringCamera.h"
#include "LandObject.h"
#include "Monster_Character.h"
#include "Light.h"

CLevel_Intro::CLevel_Intro(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Intro::Initialize()
{
    m_pGameInstance->Get_Renderer()->Render_UI_MRT(false);
    m_pGameInstance->Set_CurrentLevel(m_pGameInstance->Get_NextLevel());

    FAILED_CHECK(Ready_LightDesc());
    FAILED_CHECK(Ready_Layer_Player(TEXT("Layer_Player")));
    FAILED_CHECK(Ready_Layer_Camera(TEXT("Layer_Camera")));
    FAILED_CHECK(Ready_Layer_Monster(TEXT("Layer_Monster")));
    FAILED_CHECK(Ready_Layer_BackGround(TEXT("Layer_BackGround")));
    FAILED_CHECK(Ready_Layer_Effect(TEXT("Layer_Effect")));
    FAILED_CHECK(Ready_UI());

    FAILED_CHECK(Ready_Shader());


    return S_OK;
}

void CLevel_Intro::Tick(_float fTimeDelta)
{
}

HRESULT CLevel_Intro::Render()
{
    SetWindowText(g_hWnd, TEXT("Level Intro 입니다."));

    return S_OK;
}


HRESULT CLevel_Intro::Ready_Layer_Monster(const wstring& strLayerTag)
{
    CGameObject* pMonster = nullptr;

    json Stage1MapJson = {};

    if (FAILED(CJson_Utility::Load_Json(m_strStage1MapLoadPath.c_str(), Stage1MapJson)))
    {
        MSG_BOX("몬스터 불러오기 실패");
        return E_FAIL;
    }


    _bool bSpawnSniper = false;
    _bool bSpawnTanker = false;
    _bool bSpawnInfected = false;
    _bool bSpawnZenuGiant = true;
    

    json MonsterJson = Stage1MapJson["Monster_Json"];
    _int iMonsterJsonSize = (_int)MonsterJson.size();

    for (_int i = 0; i < iMonsterJsonSize; ++i)
    {
        CMonster_Character::MONSTER_DESC MonsterDesc = {};

        string LoadMonsterTag = (string(MonsterJson[i]["PrototypeTag"]));

        m_pGameInstance->String_To_WString(LoadMonsterTag, MonsterDesc.strProtoTypeTag);
        MonsterDesc.bPreview = false;
        MonsterDesc.eDescType = CGameObject::MONSTER_DESC;
        MonsterDesc.iStartNaviIndex = MonsterJson[i]["StartNaviIndex"];

        if (bSpawnSniper == false && MonsterDesc.strProtoTypeTag == L"Prototype_GameObject_Bandit_Sniper")
        {
            continue;
        }

        if (bSpawnTanker == false && MonsterDesc.strProtoTypeTag == L"Prototype_GameObject_Tank")
        {
            continue;
        }

        if (bSpawnZenuGiant == false && MonsterDesc.strProtoTypeTag == L"Prototype_GameObject_Heavy_Vampiric_Zombie")
        {
            continue;
        }

        if (bSpawnInfected == false)
        {
            if(MonsterDesc.strProtoTypeTag == L"Prototype_GameObject_Infected_A" || MonsterDesc.strProtoTypeTag == L"Prototype_GameObject_Infected_B" || MonsterDesc.strProtoTypeTag == L"Prototype_GameObject_Infected_C" || MonsterDesc.strProtoTypeTag == L"Prototype_GameObject_Infected_D")
                continue;
        }
        

        const json& TransformJson = MonsterJson[i]["Component"]["Transform"];
        _float4x4 WorldMatrix;

        for (_int TransformLoopIndex = 0; TransformLoopIndex < 4; ++TransformLoopIndex)
        {
            for (_int TransformSecondLoopIndex = 0; TransformSecondLoopIndex < 4; ++TransformSecondLoopIndex)
            {
                WorldMatrix.m[TransformLoopIndex][TransformSecondLoopIndex] = TransformJson[TransformLoopIndex][TransformSecondLoopIndex];
            }
        }

        MonsterDesc.WorldMatrix = WorldMatrix;

        CMonster_Character* pMonster = dynamic_cast<CMonster_Character*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_INTRO, L"Layer_Monster", MonsterDesc.strProtoTypeTag, &MonsterDesc));

        if (pMonster != nullptr)
        {
            pMonster->Set_InitPosition(pMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION));
        }
        else
            return E_FAIL;

        

        

    }

    //pMonster = m_pGameInstance->Add_CloneObject_And_Get(LEVEL_INTRO, strLayerTag, TEXT("Prototype_GameObject_Bandit_Sniper"));
    //NULL_CHECK_RETURN(pMonster, E_FAIL);
    ////pMonster->Set_Position(_float3(50.0f, 0.f, 35.f));
    //pMonster->Set_InitPosition(_float3(50.0f, 0.f, 35.f));
    //
    //pMonster = m_pGameInstance->Add_CloneObject_And_Get(LEVEL_INTRO, strLayerTag, TEXT("Prototype_GameObject_Infected_B"));
    //NULL_CHECK_RETURN(pMonster, E_FAIL);
    //pMonster->Set_InitPosition(_float3(61.f, 0.f, 37.f));
    
    //pMonster = m_pGameInstance->Add_CloneObject_And_Get(LEVEL_INTRO, strLayerTag, TEXT("Prototype_GameObject_Infected_C"));
    //NULL_CHECK_RETURN(pMonster, E_FAIL);
    //pMonster->Set_InitPosition(_float3(252.5f, 0.f, 9.f));	
    
    //pMonster = m_pGameInstance->Add_CloneObject_And_Get(LEVEL_INTRO, strLayerTag, TEXT("Prototype_GameObject_Bandit_Sniper"));
    //NULL_CHECK_RETURN(pMonster, E_FAIL);
    //pMonster->Set_InitPosition(_float3(161.5f, 14.65f, 215.5f));

    /* Shader Test Model */
    //CGameObject* pMonster = m_pGameInstance->Add_CloneObject_And_Get(LEVEL_INTRO, strLayerTag, TEXT("Prototype_GameObject_Screamer"));
    //NULL_CHECK_RETURN(pMonster, E_FAIL);
    //pMonster->Set_Position(_float3(250.5, 0.f, 20.f));

    return S_OK;
}

HRESULT CLevel_Intro::Ready_Layer_Camera(const wstring& strLayerTag)
{
    FAILED_CHECK(m_pGameInstance->Add_CloneObject(LEVEL_INTRO, strLayerTag, TEXT("Prototype_GameObject_MasterCamera")));

    return S_OK;
}

HRESULT CLevel_Intro::Ready_Layer_Effect(const wstring& strLayerTag)
{
    //FAILED_CHECK(m_pGameInstance->Add_CloneObject(LEVEL_INTRO, strLayerTag, TEXT("Prototype_GameObject_Particle_Blue")));
    //FAILED_CHECK(m_pGameInstance->Add_CloneObject(LEVEL_INTRO, strLayerTag, TEXT("Prototype_GameObject_Particle_Red")));
    //FAILED_CHECK(m_pGameInstance->Add_CloneObject(LEVEL_INTRO, strLayerTag, TEXT("Prototype_GameObject_Effect_Explosion")));

    return S_OK;
}

HRESULT CLevel_Intro::Ready_Layer_Player(const wstring& strLayerTag)
{
    FAILED_CHECK(m_pGameInstance->Add_CloneObject(LEVEL_INTRO, strLayerTag, TEXT("Prototype_GameObject_Player")));

    CPlayer* pPlayer = CData_Manager::GetInstance()->Get_Player();
    pPlayer->Set_Position(_float3(3.0f, 0.f, 3.84f));



    CNavigation* pNavigation = pPlayer->Get_Navigation();
    pNavigation->Set_CurrentIndex(pNavigation->Get_SelectRangeCellIndex(pPlayer));

    return S_OK;
}

HRESULT CLevel_Intro::Ready_Layer_BackGround(const wstring& strLayerTag)
{
    FAILED_CHECK(m_pGameInstance->Add_CloneObject(LEVEL_INTRO, strLayerTag, TEXT("Prototype_GameObject_Sky")));

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

        pObject = dynamic_cast<CEnvironment_Object*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_INTRO, L"Layer_BackGround", L"Prototype_GameObject_Environment_Object", &Desc));
    }

    json InteractJson = Stage1MapJson["Interact_Json"];
    _int InteractJsonSize = (_int)InteractJson.size();

    for (_int i = 0; i < InteractJsonSize; ++i)
    {
        //TODO 추후 상호작용 오브젝트 클래스 작성  후 작업
        //! L"Layer_Event"
    }

    json InstanceJson = Stage1MapJson["Instance_Json"];
    _int InstanceJsonSize = (_int)InstanceJson.size();

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
        _uint InstanceInfoJsonSize = (_uint)InstanceInfoJson.size();

        for (_uint j = 0; j < InstanceInfoJsonSize; ++j)
        {
            INSTANCE_INFO_DESC InstanceInfoDesc = {};

            CJson_Utility::Load_Float3(InstanceInfoJson[j]["Instance_Scale"], InstanceInfoDesc.vScale);
            CJson_Utility::Load_Float4(InstanceInfoJson[j]["Instance_Rotation"], InstanceInfoDesc.vRotation);
            CJson_Utility::Load_Float3(InstanceInfoJson[j]["Instance_Translation"], InstanceInfoDesc.vTranslation);
            CJson_Utility::Load_Float3(InstanceInfoJson[j]["Instance_Center"], InstanceInfoDesc.vCenter);

            InstanceDesc.vecInstanceInfoDesc.push_back(InstanceInfoDesc);
        }


        CEnvironment_Instance* pInstanceObject = { nullptr };

        pInstanceObject = dynamic_cast<CEnvironment_Instance*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_INTRO, L"Layer_BackGround", L"Prototype_GameObject_Environment_Instance", &InstanceDesc));

    }

    

    //CEnvironment_LightObject::ENVIRONMENT_LIGHTOBJECT_DESC LightObjectDesc;
    //
    //LightObjectDesc.bAnimModel = false;
    //LightObjectDesc.bPreview = false;
    //LightObjectDesc.strModelTag = L"Prototype_Component_Model_SecretTempleStatue1";
    //XMStoreFloat4x4(&LightObjectDesc.WorldMatrix, XMMatrixIdentity());
    //
    //LightObjectDesc.iLightIndex = 4;

    //LIGHT_DESC LightDesc;
    //
    //
    //LightDesc.eType = LIGHT_DESC::TYPE_POINT;
    //XMStoreFloat4(&LightDesc.vPosition, XMLoadFloat4x4(&LightObjectDesc.WorldMatrix).r[3]);
    //LightDesc.fRange = 100.f;
    //LightDesc.vPosition = _float4(60.0f, 0.f, 55.f, 1.f);
    //LightDesc.vDiffuse = _float4(0.6f, 0.2f, 0.05f, 1.0f);
    //LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
    //LightDesc.vSpecular = LightDesc.vDiffuse;
    //
    //LightObjectDesc.LightDesc = LightDesc;
    //
    //FAILED_CHECK(m_pGameInstance->Add_CloneObject(LEVEL_INTRO, L"Layer_BackGround", L"Prototype_GameObject_Environment_LightObject", &LightObjectDesc));

    return S_OK;

}

HRESULT CLevel_Intro::Ready_UI()
{
    // FAILED_CHECK(Ready_Layer_UI_Monster(TEXT("Layer_UI_Monster"), nullptr));

    FAILED_CHECK(Ready_Layer_UI_Interface(TEXT("Layer_UI_Interface"), nullptr));
    //
    //FAILED_CHECK(Ready_Layer_UI(TEXT("Layer_UI"), nullptr));

    return S_OK;
}

HRESULT CLevel_Intro::Ready_Layer_UI_Monster(const wstring& strLayerTag, void* pArg)
{
    /* 추 후 파싱해서 정보 받아오기 */

    /* MRT로 묶지 않으면 출력이안나옴. */
    json json_in;

    char filePath[MAX_PATH] = "../Bin/DataFiles/Data_UI/UI_Info";

    _int      iPathNum = 0;
    string      strFileName;
    string      strFilePath;

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

        // Pos 잡아주기
    }


    //if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STATIC, strLayerTag, TEXT("Prototype_GameObject_UI_Anything"), &json_in)))
    //   return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Intro::Ready_Layer_UI_Interface(const wstring& strLayerTag, void* pArg)
{
    // Ready Interface
    FAILED_CHECK(CUI_Manager::GetInstance()->Ready_Interface(LEVEL_STATIC));
    // Ready Crosshair
    FAILED_CHECK(CUI_Manager::GetInstance()->Ready_Crosshair(LEVEL_STATIC));

    //// =>Left_Interface
    //Ready_LeftInterface(strLayerTag, pArg);
    //// =>Right_Interface
    //Ready_RightInterface(strLayerTag, pArg);
    //// =>Quest_Box
    //Ready_QuestBox(strLayerTag, pArg);
    //// =>Tutorial_Box
    //Ready_TutorialBox(strLayerTag, pArg);
    //// =>LevelUp
    //Ready_LevelUp(strLayerTag, pArg);
    //// =>Reward_Item
    //Ready_Reward_Item(strLayerTag, pArg);
    return S_OK;
}

HRESULT CLevel_Intro::Ready_LeftInterface(const wstring& strLayerTag, void* pArg)
{
    json json_in;

    char filePath[MAX_PATH] = "../Bin/DataFiles/Data_UI/PlayerInterface/Left_Interface.json";

    _int      iPathNum = 0;
    string      strFileName;
    string      strFilePath;

    CJson_Utility::Load_Json(filePath, json_in);

    for (auto& item : json_in.items())
    {
        json object = item.value();

        CUI::UI_DESC tUI_Info;

        /* 저장순서랑 맞는지 확인하기 */
        tUI_Info.bParent = object["Parent"];               // 1. Parent
        tUI_Info.bWorld = object["World"];                  // 2. World
        tUI_Info.bGroup = object["Group"];                  // 3. Group
        tUI_Info.fAlpha = object["Alpha"];                  // 4. Alpha
        tUI_Info.iObjectNum = object["ObjectNum"];            // 5. ObjectNum
        tUI_Info.iShaderNum = object["ShaderNum"];            // 6. ShaderPathNum
        tUI_Info.strObjectName = object["ObjectName"];         // 7. ObjectName
        tUI_Info.strLayerTag = object["LayerTag"];            // 8. LayerTag
        tUI_Info.strCloneTag = object["CloneTag"];            // 9. CloneTag
        tUI_Info.strProtoTag = object["ProtoTag"];            // 10. ProtoTag
        tUI_Info.strFilePath = object["FilePath"];            // 11. FilePath
        tUI_Info.strMapTextureTag = object["MapTextureTag"];   // 12. MapTexture
        tUI_Info.vColor.m128_f32[0] = object["ColorR"];         // 13. R
        tUI_Info.vColor.m128_f32[1] = object["ColorG"];         // 14. G
        tUI_Info.vColor.m128_f32[2] = object["ColorB"];         // 15. B
        tUI_Info.vColor.m128_f32[3] = object["ColorA"];         // 16. A

        wstring wstrLayer;
        m_pGameInstance->String_To_WString(tUI_Info.strLayerTag, wstrLayer); //

        wstring wstrClonetag;
        m_pGameInstance->String_To_WString(tUI_Info.strCloneTag, wstrClonetag);

        wstring wstrPrototag;
        m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

        wstring wstrFilePath;
        m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

        CUI* pUI_Object = dynamic_cast<CUI*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_STATIC, strLayerTag, wstrClonetag, &tUI_Info));

        pUI_Object->Get_Transform()->Load_FromJson(object);      // 17. TransformCom
    }

    return S_OK;
}

HRESULT CLevel_Intro::Ready_RightInterface(const wstring& strLayerTag, void* pArg)
{
    json json_in;

    char filePath[MAX_PATH] = "../Bin/DataFiles/Data_UI/PlayerInterface/Right_Interface.json";

    _int      iPathNum = 0;
    string      strFileName;
    string      strFilePath;

    CJson_Utility::Load_Json(filePath, json_in);

    for (auto& item : json_in.items())
    {
        json object = item.value();

        CUI::UI_DESC tUI_Info;

        /* 저장순서랑 맞는지 확인하기 */
        tUI_Info.bParent = object["Parent"];               // 1. Parent
        tUI_Info.bWorld = object["World"];                  // 2. World
        tUI_Info.bGroup = object["Group"];                  // 3. Group
        tUI_Info.fAlpha = object["Alpha"];                  // 4. Alpha
        tUI_Info.iObjectNum = object["ObjectNum"];            // 5. ObjectNum
        tUI_Info.iShaderNum = object["ShaderNum"];            // 6. ShaderPathNum
        tUI_Info.strObjectName = object["ObjectName"];         // 7. ObjectName
        tUI_Info.strLayerTag = object["LayerTag"];            // 8. LayerTag
        tUI_Info.strCloneTag = object["CloneTag"];            // 9. CloneTag
        tUI_Info.strProtoTag = object["ProtoTag"];            // 10. ProtoTag
        tUI_Info.strFilePath = object["FilePath"];            // 11. FilePath
        tUI_Info.strMapTextureTag = object["MapTextureTag"];   // 12. MapTexture
        tUI_Info.vColor.m128_f32[0] = object["ColorR"];         // 13. R
        tUI_Info.vColor.m128_f32[1] = object["ColorG"];         // 14. G
        tUI_Info.vColor.m128_f32[2] = object["ColorB"];         // 15. B
        tUI_Info.vColor.m128_f32[3] = object["ColorA"];         // 16. A

        wstring wstrLayer;
        m_pGameInstance->String_To_WString(tUI_Info.strLayerTag, wstrLayer); //

        wstring wstrClonetag;
        m_pGameInstance->String_To_WString(tUI_Info.strCloneTag, wstrClonetag);

        wstring wstrPrototag;
        m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

        wstring wstrFilePath;
        m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

        CUI* pUI_Object = dynamic_cast<CUI*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_STATIC, strLayerTag, wstrClonetag, &tUI_Info));

        pUI_Object->Get_Transform()->Load_FromJson(object);      // 17. TransformCom
    }

    return S_OK;
}

HRESULT CLevel_Intro::Ready_QuestBox(const wstring& strLayerTag, void* pArg)
{
    json json_in;

    char filePath[MAX_PATH] = "../Bin/DataFiles/Data_UI/PlayerInterface/QuestBox.json";

    _int      iPathNum = 0;
    string      strFileName;
    string      strFilePath;

    CJson_Utility::Load_Json(filePath, json_in);

    for (auto& item : json_in.items())
    {
        json object = item.value();

        CUI::UI_DESC tUI_Info;

        /* 저장순서랑 맞는지 확인하기 */
        tUI_Info.bParent = object["Parent"];               // 1. Parent
        tUI_Info.bWorld = object["World"];                  // 2. World
        tUI_Info.bGroup = object["Group"];                  // 3. Group
        tUI_Info.fAlpha = object["Alpha"];                  // 4. Alpha
        tUI_Info.iObjectNum = object["ObjectNum"];            // 5. ObjectNum
        tUI_Info.iShaderNum = object["ShaderNum"];            // 6. ShaderPathNum
        tUI_Info.strObjectName = object["ObjectName"];         // 7. ObjectName
        tUI_Info.strLayerTag = object["LayerTag"];            // 8. LayerTag
        tUI_Info.strCloneTag = object["CloneTag"];            // 9. CloneTag
        tUI_Info.strProtoTag = object["ProtoTag"];            // 10. ProtoTag
        tUI_Info.strFilePath = object["FilePath"];            // 11. FilePath
        tUI_Info.strMapTextureTag = object["MapTextureTag"];   // 12. MapTexture
        tUI_Info.vColor.m128_f32[0] = object["ColorR"];         // 13. R
        tUI_Info.vColor.m128_f32[1] = object["ColorG"];         // 14. G
        tUI_Info.vColor.m128_f32[2] = object["ColorB"];         // 15. B
        tUI_Info.vColor.m128_f32[3] = object["ColorA"];         // 16. A

        wstring wstrLayer;
        m_pGameInstance->String_To_WString(tUI_Info.strLayerTag, wstrLayer); //

        wstring wstrClonetag;
        m_pGameInstance->String_To_WString(tUI_Info.strCloneTag, wstrClonetag);

        wstring wstrPrototag;
        m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

        wstring wstrFilePath;
        m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

        CUI* pUI_Object = dynamic_cast<CUI*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_STATIC, strLayerTag, wstrClonetag, &tUI_Info));

        pUI_Object->Get_Transform()->Load_FromJson(object);      // 17. TransformCom
    }

    return S_OK;
}

HRESULT CLevel_Intro::Ready_TutorialBox(const wstring& strLayerTag, void* pArg)
{
    json json_in;

    char filePath[MAX_PATH] = "../Bin/DataFiles/Data_UI/PlayerInterface/TutorialBox.json";

    _int      iPathNum = 0;
    string      strFileName;
    string      strFilePath;

    CJson_Utility::Load_Json(filePath, json_in);

    for (auto& item : json_in.items())
    {
        json object = item.value();

        CUI::UI_DESC tUI_Info;

        /* 저장순서랑 맞는지 확인하기 */
        tUI_Info.bParent = object["Parent"];               // 1. Parent
        tUI_Info.bWorld = object["World"];                  // 2. World
        tUI_Info.bGroup = object["Group"];                  // 3. Group
        tUI_Info.fAlpha = object["Alpha"];                  // 4. Alpha
        tUI_Info.iObjectNum = object["ObjectNum"];            // 5. ObjectNum
        tUI_Info.iShaderNum = object["ShaderNum"];            // 6. ShaderPathNum
        tUI_Info.strObjectName = object["ObjectName"];         // 7. ObjectName
        tUI_Info.strLayerTag = object["LayerTag"];            // 8. LayerTag
        tUI_Info.strCloneTag = object["CloneTag"];            // 9. CloneTag
        tUI_Info.strProtoTag = object["ProtoTag"];            // 10. ProtoTag
        tUI_Info.strFilePath = object["FilePath"];            // 11. FilePath
        tUI_Info.strMapTextureTag = object["MapTextureTag"];   // 12. MapTexture
        tUI_Info.vColor.m128_f32[0] = object["ColorR"];         // 13. R
        tUI_Info.vColor.m128_f32[1] = object["ColorG"];         // 14. G
        tUI_Info.vColor.m128_f32[2] = object["ColorB"];         // 15. B
        tUI_Info.vColor.m128_f32[3] = object["ColorA"];         // 16. A

        wstring wstrLayer;
        m_pGameInstance->String_To_WString(tUI_Info.strLayerTag, wstrLayer); //

        wstring wstrClonetag;
        m_pGameInstance->String_To_WString(tUI_Info.strCloneTag, wstrClonetag);

        wstring wstrPrototag;
        m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

        wstring wstrFilePath;
        m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

        CUI* pUI_Object = dynamic_cast<CUI*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_STATIC, strLayerTag, wstrClonetag, &tUI_Info));

        pUI_Object->Get_Transform()->Load_FromJson(object);      // 17. TransformCom
    }

    return S_OK;
}

HRESULT CLevel_Intro::Ready_LevelUp(const wstring& strLayerTag, void* pArg)
{
    json json_in;

    char filePath[MAX_PATH] = "../Bin/DataFiles/Data_UI/PlayerInterface/LevelUp.json";

    _int      iPathNum = 0;
    string      strFileName;
    string      strFilePath;

    CJson_Utility::Load_Json(filePath, json_in);

    for (auto& item : json_in.items())
    {
        json object = item.value();

        CUI::UI_DESC tUI_Info;

        /* 저장순서랑 맞는지 확인하기 */
        tUI_Info.bParent = object["Parent"];               // 1. Parent
        tUI_Info.bWorld = object["World"];                  // 2. World
        tUI_Info.bGroup = object["Group"];                  // 3. Group
        tUI_Info.fAlpha = object["Alpha"];                  // 4. Alpha
        tUI_Info.iObjectNum = object["ObjectNum"];            // 5. ObjectNum
        tUI_Info.iShaderNum = object["ShaderNum"];            // 6. ShaderPathNum
        tUI_Info.strObjectName = object["ObjectName"];         // 7. ObjectName
        tUI_Info.strLayerTag = object["LayerTag"];            // 8. LayerTag
        tUI_Info.strCloneTag = object["CloneTag"];            // 9. CloneTag
        tUI_Info.strProtoTag = object["ProtoTag"];            // 10. ProtoTag
        tUI_Info.strFilePath = object["FilePath"];            // 11. FilePath
        tUI_Info.strMapTextureTag = object["MapTextureTag"];   // 12. MapTexture
        tUI_Info.vColor.m128_f32[0] = object["ColorR"];         // 13. R
        tUI_Info.vColor.m128_f32[1] = object["ColorG"];         // 14. G
        tUI_Info.vColor.m128_f32[2] = object["ColorB"];         // 15. B
        tUI_Info.vColor.m128_f32[3] = object["ColorA"];         // 16. A

        wstring wstrLayer;
        m_pGameInstance->String_To_WString(tUI_Info.strLayerTag, wstrLayer); //

        wstring wstrClonetag;
        m_pGameInstance->String_To_WString(tUI_Info.strCloneTag, wstrClonetag);

        wstring wstrPrototag;
        m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

        wstring wstrFilePath;
        m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

        CUI* pUI_Object = dynamic_cast<CUI*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_STATIC, strLayerTag, wstrClonetag, &tUI_Info));

        pUI_Object->Get_Transform()->Load_FromJson(object);      // 17. TransformCom
    }

    return S_OK;
}

HRESULT CLevel_Intro::Ready_Reward_Item(const wstring& strLayerTag, void* pArg)
{
    json json_in;

    char filePath[MAX_PATH] = "../Bin/DataFiles/Data_UI/PlayerInterface/RewardItem.json";

    _int      iPathNum = 0;
    string      strFileName;
    string      strFilePath;

    CJson_Utility::Load_Json(filePath, json_in);

    for (auto& item : json_in.items())
    {
        json object = item.value();

        CUI::UI_DESC tUI_Info;

        /* 저장순서랑 맞는지 확인하기 */
        tUI_Info.bParent = object["Parent"];               // 1. Parent
        tUI_Info.bWorld = object["World"];                  // 2. World
        tUI_Info.bGroup = object["Group"];                  // 3. Group
        tUI_Info.fAlpha = object["Alpha"];                  // 4. Alpha
        tUI_Info.iObjectNum = object["ObjectNum"];            // 5. ObjectNum
        tUI_Info.iShaderNum = object["ShaderNum"];            // 6. ShaderPathNum
        tUI_Info.strObjectName = object["ObjectName"];         // 7. ObjectName
        tUI_Info.strLayerTag = object["LayerTag"];            // 8. LayerTag
        tUI_Info.strCloneTag = object["CloneTag"];            // 9. CloneTag
        tUI_Info.strProtoTag = object["ProtoTag"];            // 10. ProtoTag
        tUI_Info.strFilePath = object["FilePath"];            // 11. FilePath
        tUI_Info.strMapTextureTag = object["MapTextureTag"];   // 12. MapTexture
        tUI_Info.vColor.m128_f32[0] = object["ColorR"];         // 13. R
        tUI_Info.vColor.m128_f32[1] = object["ColorG"];         // 14. G
        tUI_Info.vColor.m128_f32[2] = object["ColorB"];         // 15. B
        tUI_Info.vColor.m128_f32[3] = object["ColorA"];         // 16. A

        wstring wstrLayer;
        m_pGameInstance->String_To_WString(tUI_Info.strLayerTag, wstrLayer); //

        wstring wstrClonetag;
        m_pGameInstance->String_To_WString(tUI_Info.strCloneTag, wstrClonetag);

        wstring wstrPrototag;
        m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

        wstring wstrFilePath;
        m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

        CUI* pUI_Object = dynamic_cast<CUI*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_STATIC, strLayerTag, wstrClonetag, &tUI_Info));

        pUI_Object->Get_Transform()->Load_FromJson(object);      // 17. TransformCom
    }

    return S_OK;
}

HRESULT CLevel_Intro::Ready_Layer_UI(const wstring& strLayerTag, void* pArg)
{
    return S_OK;
}


HRESULT CLevel_Intro::Ready_LightDesc()
{
    /* For. Shadow */
    m_pGameInstance->Add_ShadowLight_View(ECast(LEVEL::LEVEL_INTRO), _float4(Engine::g_vLightPos), _float4(0.f, 0.f, 0.f, 1.f), _float4(0.f, 1.f, 0.f, 0.f));
    m_pGameInstance->Add_ShadowLight_Proj(ECast(LEVEL::LEVEL_INTRO), 60.f, (_float)g_iWinSizeX / (_float)g_iWinSizeY, Engine::g_fLightNear, Engine::g_fLightFar);

   //LIGHT_DESC         LightDesc{};
   //{
   //    LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
   //    LightDesc.vDirection = _float4(0.125f, -0.01f, -0.45f, 0.485f);
   //    LightDesc.vDiffuse = _float4(0.822f, 0.822f, 0.822f, 0.5f);
   //    LightDesc.vAmbient = _float4(0.243f, 0.386f, 0.253f, 0.604f);
   //    LightDesc.vSpecular = _float4(0.428f, 0.985f, 0.350f, 0.5f);
   //
   //    FAILED_CHECK(m_pGameInstance->Add_Light(LightDesc, TempLightNumber));
   //}

    json Stage1MapJson = {};

    if (FAILED(CJson_Utility::Load_Json(m_strStage1MapLoadPath.c_str(), Stage1MapJson)))
    {
        MSG_BOX("조명 불러오기 실패");
        return E_FAIL;
    }

    json LightJson = Stage1MapJson["Light_Json"];
    _int iLightJsonSize = (_int)LightJson.size();

    for (_int i = 0; i < iLightJsonSize; ++i)
    {
        LIGHT_DESC LightDesc = {};

        LightDesc.iLightIndex = LightJson[i]["LightIndex"];
        LightDesc.bEnable = LightJson[i]["LightEnable"];
        LightDesc.fCutOff = LightJson[i]["CutOff"];
        LightDesc.fOuterCutOff = LightJson[i]["OuterCutOff"];

        LightDesc.eType = LightJson[i]["Type"];
        CJson_Utility::Load_Float4(LightJson[i]["Direction"], LightDesc.vDirection);
        LightDesc.fRange = LightJson[i]["Range"];
        CJson_Utility::Load_Float4(LightJson[i]["Position"], LightDesc.vPosition);
        CJson_Utility::Load_Float4(LightJson[i]["Diffuse"], LightDesc.vDiffuse);
        CJson_Utility::Load_Float4(LightJson[i]["Specular"], LightDesc.vSpecular);
        CJson_Utility::Load_Float4(LightJson[i]["Ambient"], LightDesc.vAmbient);


        if (LightDesc.eType == tagLightDesc::TYPE_DIRECTIONAL)
        {
            CLight* pDirectionLight = m_pGameInstance->Get_DirectionLight();

            if (pDirectionLight != nullptr)
            {
                m_pGameInstance->Remove_Light(pDirectionLight->Get_LightIndex());

            }
        }

        CLight* pLight = m_pGameInstance->Add_Light_AndGet(LightDesc, LightDesc.iLightIndex);

        if (pLight == nullptr)
        {
            MSG_BOX("라이트 불러오기 실패");
            return E_FAIL;
        }

    }

    json LightObjectJson = Stage1MapJson["LightObject_Json"];
    _int iLightObjectJsonSize = (_int)LightObjectJson.size();

    for (_int i = 0; i < iLightObjectJsonSize; ++i)
    {
        CEnvironment_LightObject::ENVIRONMENT_LIGHTOBJECT_DESC LightObjectDesc = {};

        LightObjectDesc.bAnimModel = LightObjectJson[i]["AnimType"];
        LightObjectDesc.bEffect = LightObjectJson[i]["Effect"];
        LightObjectDesc.eLightEffect = LightObjectJson[i]["EffectType"];
        LightObjectDesc.iPlayAnimationIndex = LightObjectJson[i]["PlayAnimationIndex"];
        LightObjectDesc.iShaderPassIndex = LightObjectJson[i]["ShaderPassIndex"];
        LightObjectDesc.bPreview = false;

        m_pGameInstance->String_To_WString((string)LightObjectJson[i]["ModelTag"], LightObjectDesc.strModelTag);

        const json& TransformJson = LightObjectJson[i]["Component"]["Transform"];
        _float4x4 WorldMatrix;

        for (_int TransformLoopIndex = 0; TransformLoopIndex < 4; ++TransformLoopIndex)
        {
            for (_int TransformSecondLoopIndex = 0; TransformSecondLoopIndex < 4; ++TransformSecondLoopIndex)
            {
                WorldMatrix.m[TransformLoopIndex][TransformSecondLoopIndex] = TransformJson[TransformLoopIndex][TransformSecondLoopIndex];
            }
        }

        LightObjectDesc.WorldMatrix = WorldMatrix;



        LIGHT_DESC LightDesc = {};

        LightDesc.iLightIndex = LightObjectJson[i]["LightIndex"];
        LightDesc.bEnable = LightObjectJson[i]["LightEnable"];
        LightDesc.fCutOff = LightObjectJson[i]["CutOff"];
        LightDesc.fOuterCutOff = LightObjectJson[i]["OuterCutOff"];

        LightDesc.eType = LightObjectJson[i]["LightType"];
        CJson_Utility::Load_Float4(LightObjectJson[i]["Direction"], LightDesc.vDirection);
        LightDesc.fRange = LightObjectJson[i]["Range"];
        CJson_Utility::Load_Float4(LightObjectJson[i]["Position"], LightDesc.vPosition);
        CJson_Utility::Load_Float4(LightObjectJson[i]["Diffuse"], LightDesc.vDiffuse);
        CJson_Utility::Load_Float4(LightObjectJson[i]["Ambient"], LightDesc.vAmbient);
        CJson_Utility::Load_Float4(LightObjectJson[i]["Specular"], LightDesc.vSpecular);


        LightObjectDesc.LightDesc = LightDesc;

        CEnvironment_LightObject* pLightObject = dynamic_cast<CEnvironment_LightObject*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_TOOL, L"Layer_BackGround", L"Prototype_GameObject_Environment_LightObject", &LightObjectDesc));

        if (pLightObject == nullptr)
        {
            MSG_BOX("라이트오브젝트 생성실패");
            return E_FAIL;
        }
    }

    return S_OK;
}

HRESULT CLevel_Intro::Ready_Shader()
{
    json Shader_Json;

    FAILED_CHECK(CJson_Utility::Load_Json(m_strShader_FilepPath.c_str(), Shader_Json));

    HBAO_PLUS_DESC Desc_Hbao = {};
    Desc_Hbao.bHBAO_Active      = Shader_Json["HBAO"]["bHBAO_Active"];
    Desc_Hbao.fRadius           = Shader_Json["HBAO"]["fBias"];
    Desc_Hbao.fBias             = Shader_Json["HBAO"]["fBlur_Sharpness"];
    Desc_Hbao.fBlur_Sharpness   = Shader_Json["HBAO"]["fPowerExponent"];
    Desc_Hbao.fPowerExponent    = Shader_Json["HBAO"]["fRadius"];
    
    DEFERRED_DESC Desc_Deferred = {};
    Desc_Deferred.bRimBloom_Blur_Active     = Shader_Json["Deferred"]["bRimBloom_Blur_Active"];
    Desc_Deferred.bShadow_Active            = Shader_Json["Deferred"]["bShadow_Active"];

    FOG_DESC Desc_Fog = {};
    Desc_Fog.bFog_Active            = Shader_Json["Fog"]["bFog_Active"];
    Desc_Fog.fFogStartDepth         = Shader_Json["Fog"]["fFogStartDepth"];
    Desc_Fog.fFogStartDistance      = Shader_Json["Fog"]["fFogStartDistance"];
    Desc_Fog.fFogDistanceValue      = Shader_Json["Fog"]["fFogDistanceValue"];
    Desc_Fog.fFogHeightValue        = Shader_Json["Fog"]["fFogHeightValue"];
    Desc_Fog.fFogDistanceDensity    = Shader_Json["Fog"]["fFogDistanceDensity"];
    Desc_Fog.fFogHeightDensity      = Shader_Json["Fog"]["fFogHeightDensity"];
    Desc_Fog.vFogColor.x            = Shader_Json["Fog"]["vFogColor_x"];
    Desc_Fog.vFogColor.y            = Shader_Json["Fog"]["vFogColor_y"];
    Desc_Fog.vFogColor.z            = Shader_Json["Fog"]["vFogColor_z"];
    Desc_Fog.vFogColor.w            = Shader_Json["Fog"]["vFogColor_w"];

    HDR_DESC Desc_HDR = {};
    Desc_HDR.bHDR_Active            = Shader_Json["HDR"]["bHDR_Active"];
    Desc_HDR.fmax_white             = Shader_Json["HDR"]["fmax_white"];

    ANTI_DESC Desc_Anti = {};
    Desc_Anti.bFXAA_Active          = Shader_Json["Anti"]["bFXAA_Active"];

    HSV_DESC Desc_HSV = {};
    Desc_HSV.bScreen_Active         = Shader_Json["HSV"]["bScreen_Active"];
    Desc_HSV.fFinal_Brightness      = Shader_Json["HSV"]["fFinal_Saturation"];
    Desc_HSV.fFinal_Saturation      = Shader_Json["HSV"]["fFinal_Brightness"];

    RADIAL_DESC Desc_Radial = {};
    Desc_Radial.bRadial_Active      = Shader_Json["Radial"]["bRadial_Active"];
    Desc_Radial.fRadial_Quality     = Shader_Json["Radial"]["fRadial_Quality"];
    Desc_Radial.fRadial_Power       = Shader_Json["Radial"]["fRadial_Power"];

    DOF_DESC Desc_Dof = {};
    Desc_Dof.bDOF_Active            = Shader_Json["DOF"]["bDOF_Active"];
    //Desc_Dof.fFocusDistance         = Shader_Json["DOF"]["fFocusDistance"];
    //Desc_Dof.fFocusRange            = Shader_Json["DOF"]["fFocusRange"];

    m_pGameInstance->Get_Renderer()->Set_HBAO_Option(Desc_Hbao);
    m_pGameInstance->Get_Renderer()->Set_Deferred_Option(Desc_Deferred);
    m_pGameInstance->Get_Renderer()->Set_Fog_Option(Desc_Fog);
    m_pGameInstance->Get_Renderer()->Set_HDR_Option(Desc_HDR);
    m_pGameInstance->Get_Renderer()->Set_FXAA_Option(Desc_Anti);
    m_pGameInstance->Get_Renderer()->Set_HSV_Option(Desc_HSV);
    m_pGameInstance->Get_Renderer()->Set_RadialBlur_Option(Desc_Radial);
    m_pGameInstance->Get_Renderer()->Set_DOF_Option(Desc_Dof);

    return S_OK;
}


CLevel_Intro* CLevel_Intro::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)

{
    CLevel_Intro* pInstance = new CLevel_Intro(pDevice, pContext);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : CLEVEL_INTRO");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CLevel_Intro::Free()
{
    __super::Free();

}