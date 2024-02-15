#include "stdafx.h"
#include "Level_Tool.h"

#include "Imgui_Manager.h"
#include "GameInstance.h"

#include "Camera_Dynamic.h"


#include "Camera_Dynamic.h"
#include "Particle_Custom.h"


CLevel_Tool::CLevel_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Tool::Initialize()
{
	//Level_Tool 레벨 조정 
	m_pGameInstance->Set_CurrentLevel(3);
	if (FAILED(Ready_Imgui()))
	{
		Safe_Release(m_pDevice);
		Safe_Release(m_pContext);
		return E_FAIL;
	}
	FAILED_CHECK(Ready_LightDesc());
	FAILED_CHECK(Ready_Layer_Camera(TEXT("Layer_Camera")));

	return S_OK;

}

void CLevel_Tool::Tick(_float fTimeDelta)
{
	m_pImguiManager->Tick(fTimeDelta);
}

HRESULT CLevel_Tool::Render()
{
	SetWindowText(g_hWnd, TEXT("툴 레벨입니다."));

	m_pImguiManager->Render();

	return S_OK;
}

HRESULT CLevel_Tool::Ready_Imgui()
{
	m_pImguiManager = CImgui_Manager::GetInstance();

	m_pImguiManager->AddRef();

	if(nullptr == m_pImguiManager)
		return E_FAIL;

	if(FAILED(m_pImguiManager->Initialize(m_pDevice,m_pContext)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_Tool::Ready_Layer_Camera(const wstring& strLayerTag)
{

	CCamera_Dynamic::DYNAMIC_CAMERA_DESC		tDesc = {};
	tDesc.fMouseSensor = 0.05f;
	tDesc.vEye = _float4(0.f, 20.f, -15.f, 1.f);
	tDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	tDesc.fFovy = XMConvertToRadians(60.0f);
	tDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	tDesc.fNear = 0.1f;
	tDesc.fFar = 1000.f;
	tDesc.fSpeedPerSec = 5.f;
	tDesc.fRotationPerSec = XMConvertToRadians(180.0f);

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_TOOL, strLayerTag, TEXT("Prototype_GameObject_Camera_Dynamic"), &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Tool::Ready_LightDesc()
{
	LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(0.6f, 0.6f, 0.6f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc, TempLightNumber)))
		return E_FAIL;
}


CLevel_Tool* CLevel_Tool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Tool* pInstance = new CLevel_Tool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Tool");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Tool::Free()
{
	__super::Free();
	Safe_Release(m_pImguiManager);
	CImgui_Manager::GetInstance()->DestroyInstance();
}
