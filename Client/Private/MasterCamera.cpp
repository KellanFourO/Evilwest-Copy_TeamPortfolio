#include "..\Public\MasterCamera.h"
 
#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "Character.h"
#include "Player.h"
#include "Data_Manager.h"
#include "Clone_Manager.h"

CMasterCamera::CMasterCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	: CGameObject(pDevice, pContext, strPrototypeTag)
{
}

CMasterCamera::CMasterCamera(const CMasterCamera& rhs)
	: CGameObject(rhs)
{
}

HRESULT CMasterCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMasterCamera::Initialize(void* pArg)
{
	m_pPlayer = CData_Manager::GetInstance()->Get_Player();

	CCamera_Dynamic::DYNAMIC_CAMERA_DESC		Desc = {};

	Desc.fMouseSensor = 0.05f;

	Desc.fFovy = XMConvertToRadians(60.0f);
	Desc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	Desc.fNear = 0.1f;
	Desc.fFar = 1000.f;
	Desc.fSpeedPerSec = 20.f;
	Desc.fRotationPerSec = XMConvertToRadians(180.0f);


	if (m_pPlayer != nullptr)
	{
		_float4 vPlayerPos = m_pPlayer->Get_Transform()->Get_Position_Float4();
		_float pPlayerPosW = vPlayerPos.w;

		_float4 targetPos = { 0.f, 2.f, -5.f, pPlayerPosW };

		XMStoreFloat4(&targetPos, XMVector3TransformCoord(XMLoadFloat4(&targetPos), m_pPlayer->Get_Transform()->Get_WorldMatrix()));

		Desc.vEye = targetPos;
		Desc.vAt = vPlayerPos;
	}
	else
	{
		Desc.vEye = _float4(0.f, 20.f, -15.f, 1.f);
		Desc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	}


	
	{
		CCamera* pCamera = CCamera_Dynamic::Create(m_pDevice,m_pContext, L"FakeCameraDynamic");
		pCamera->Initialize(&Desc);
		if (pCamera)	m_Cameras.push_back(pCamera);

		//FAILED_CHECK(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Dynamic"), CCamera_Dynamic::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Camera_Dynamic"))));
	//FAILED_CHECK(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Spring"), CSpringCamera::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Camera_Spring"))));

		//CCamera* pCamera = CClone_Manager::GetInstance()->Clone_Object<CSpringCamera>(iNextLevel, LAYER_CAMERA, TEXT("Prototype_GameObject_Camera_Spring"), &Desc);
		//if (pCamera)	m_Cameras.push_back(pCamera);
		
	}

	if (m_pPlayer != nullptr)
	{
		CCamera* pCamera = CSpringCamera::Create(m_pDevice, m_pContext, L"FakeCameraSpring");
		pCamera->Initialize(&Desc);
		if (pCamera)	m_Cameras.push_back(pCamera);
		//CCamera* pCamera = CClone_Manager::GetInstance()->Clone_Object<CCamera_Dynamic>(iNextLevel, LAYER_CAMERA, TEXT("Prototype_GameObject_Camera_Dynamic"), &Desc);
		//if (pCamera)	m_Cameras.push_back(pCamera);

	}

	{//�߰� ������ ���߿� �ƽ� ī�޶� ������ ���⿡ ���� �ϼ� 
		//CCamera* pCamera = CClone_Manager::GetInstance()->Clone_Object<CSpringCamera>(iNextLevel, LAYER_CAMERA, TEXT("Prototype_GameObject_Camera_Spring"), &Desc);
		//if (pCamera)	m_Cameras.push_back(pCamera);
		////m_eCameraType = CameraType::SpringCamera;
	}
		
	_uint iCurrentLevel = m_pGameInstance->Get_NextLevel();



	switch (iCurrentLevel)
	{
		case (_uint)LEVEL_INTRO:
		{
			m_eCameraType = CameraType::SpringCamera;
			break;
		}
		case (_uint)LEVEL_GAMEPLAY:
		{
			m_eCameraType = CameraType::SpringCamera;
			break;
		}
		case (_uint)LEVEL_TOOL:
		{
			m_eCameraType = CameraType::DynamicCamera;
			break;
		}
	}

	CData_Manager::GetInstance()->Set_MasterCamera(this);

	return S_OK;
}

void CMasterCamera::Priority_Tick(_float fTimeDelta)
{
	if(m_pPlayer == nullptr)
		m_pPlayer = CData_Manager::GetInstance()->Get_Player();

	if (m_pPlayer != nullptr && m_bfirstCheck == true)
	{
		_uint iNextLevel = m_pGameInstance->Get_NextLevel();

		CCamera_Dynamic::DYNAMIC_CAMERA_DESC		Desc = {};

		Desc.fMouseSensor = 0.05f;

		Desc.fFovy = XMConvertToRadians(60.0f);
		Desc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
		Desc.fNear = 0.1f;
		Desc.fFar = 1000.f;
		Desc.fSpeedPerSec = 20.f;
		Desc.fRotationPerSec = XMConvertToRadians(180.0f);


		if (m_pPlayer)
		{
			_float4 vPlayerPos = m_pPlayer->Get_Transform()->Get_Position_Float4();
			_float pPlayerPosW = vPlayerPos.w;

			_float4 targetPos = { 0.f, 2.f, -5.f, pPlayerPosW };

			XMStoreFloat4(&targetPos, XMVector3TransformCoord(XMLoadFloat4(&targetPos), m_pPlayer->Get_Transform()->Get_WorldMatrix()));

			Desc.vEye = targetPos;
			Desc.vAt = vPlayerPos;
		}


		{
			CCamera* pCamera = CClone_Manager::GetInstance()->Clone_Object<CSpringCamera>(iNextLevel, LAYER_CAMERA, TEXT("Prototype_GameObject_Camera_Spring"), &Desc);
			if (pCamera)	m_Cameras.push_back(pCamera);
			m_bfirstCheck = false;
		}
	}

	if (m_Cameras[m_eCameraType])
		m_Cameras[m_eCameraType]->Priority_Tick(fTimeDelta);
}

void CMasterCamera::Tick(_float fTimeDelta)
{
	if (m_Cameras[m_eCameraType])
		m_Cameras[m_eCameraType]->Tick(fTimeDelta);
}

void CMasterCamera::Late_Tick(_float fTimeDelta)
{
	if (m_Cameras[m_eCameraType])
		m_Cameras[m_eCameraType]->Late_Tick(fTimeDelta);
}

CMasterCamera* CMasterCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CMasterCamera* pInstance = new CMasterCamera(pDevice, pContext, strPrototypeTag);
	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMasterCamera");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMasterCamera::Clone(void* pArg)
{
	CMasterCamera* pInstance = new CMasterCamera(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMasterCamera");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMasterCamera::Free()
{
	__super::Free();

	for (_uint i = 0; i < (_uint)CameraType_End; ++i)
	{
		if (nullptr != m_Cameras[i])
			Safe_Release(m_Cameras[i]);
	}
}

CGameObject* CMasterCamera::Pool()
{
	return new CMasterCamera(*this);
}
