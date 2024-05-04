#include "stdafx.h"
#include "Camera_Dynamic.h"
#include "GameInstance.h"
#include "Data_Manager.h"
#include "MasterCamera.h"
#include "Character.h"

CCamera_Dynamic::CCamera_Dynamic(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring& strPrototypeTag)
	: CCamera(pDevice, pContext, strPrototypeTag)
{
}

CCamera_Dynamic::CCamera_Dynamic(const CCamera_Dynamic & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Dynamic::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Dynamic::Initialize(void * pArg)
{
	NULL_CHECK_RETURN(pArg, E_FAIL);

	DYNAMIC_CAMERA_DESC*	pDesc = (DYNAMIC_CAMERA_DESC*)pArg;

	m_fMouseSensor = pDesc->fMouseSensor;

	FAILED_CHECK(__super::Initialize(pDesc));

	//CData_Manager::GetInstance()->Set_MasterCamera();
	
	return S_OK;
}

void CCamera_Dynamic::Priority_Tick(_float fTimeDelta)
{
	//CData_Manager::GetInstance()->Reset_Camera_Dynamic((LEVEL)m_pGameInstance->Get_NextLevel());
}

void CCamera_Dynamic::Tick(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Pressing(DIK_LEFTARROW))
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}

	if (m_pGameInstance->Key_Pressing(DIK_RIGHTARROW))
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}

	if (m_pGameInstance->Key_Pressing(DIK_UPARROW))
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if (m_pGameInstance->Key_Pressing(DIK_DOWNARROW))
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}

	if (m_pGameInstance->Mouse_Pressing(DIM_RB))
	{
		_long	MouseMove = 0;

		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_X))
		{
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fMouseSensor * MouseMove * fTimeDelta);
		}

		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_Y))
		{
			m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), m_fMouseSensor * MouseMove * fTimeDelta);
		}
	}

	if (m_pGameInstance->Key_Pressing(DIK_LSHIFT))
	{
		if (m_pGameInstance->Key_Down(DIK_F2))
		{
			CCharacter* pCharacter = m_pGameInstance->Get_Player();

			if (pCharacter != nullptr)
			{
				CData_Manager::GetInstance()->Get_MasterCamera()->Set_CameraType(CMasterCamera::SpringCamera);
			}
			else
				MSG_BOX("�÷��̾����. �����ڵ� �־���. �� ���� ��ȴ� ��¥ ");
		}
	}

	Ready_CameraAction(fTimeDelta);
	//if (m_pGameInstance->Key_Down(DIK_NUMPAD9))
	//{
	//	CCharacter* pCharacter = m_pGameInstance->Get_Player();
	//
	//	if (pCharacter != nullptr)
	//	{
	//		//pCharacter->Get_Navigation()->SpawnCell_Setting(m_pTransformCom->Get_Position());
	//		pCharacter->Set_InitPosition(m_pTransformCom->Get_Position());
	//
	//	}
	//	else
	//		MSG_BOX("�÷��̾ ������");
	//}

	__super::Tick(fTimeDelta);
}

void CCamera_Dynamic::Late_Tick(_float fTimeDelta)
{
}

HRESULT CCamera_Dynamic::Ready_CameraAction(_float fTimeDelta)
{
	//if (m_pGameInstance->Key_Pressing(DIK_NUMPAD1))
	//{
	//	_matrix BookMark_Saved = {};
	//	
	//	// Intro
	//	BookMark_Saved = XMMatrixSet(0.373051584, 9.31322575e-10, 0.927539706, 0.00000000, -0.191060930, 0.978552282, 0.0768424720, 0.00000000, -0.907650650, -0.205963597, 0.365051597, 0.00000000, 152.950897, 34.1651726, -7.01468706, 1.00000000);
	//	
	//	// Intro Boss
	//	//BookMark_Saved = XMMatrixSet(0.758161783, -1.62981451e-09, 0.651973069, 0.00000000, 0.0328741893, 0.998697162, -0.0382298827, 0.00000000, -0.651142895, 0.0504116043, 0.757191420, 0.00000000, 58.6203728, 3.81628489, 45.6046448, 1.00000000);
	//	 
	//	//SnowMountain 
	//	//BookMark_Saved = XMMatrixSet(0.999947131, 3.85625754e-10, -0.00206708559, 0.00000000, 0.000310920528, 0.988554597, 0.150741339, 0.00000000, 0.00204049703, -0.150744557, 0.988521099, 0.00000000,24.9641399, 22.7107353, 16.9577255, 1.00000000);
	//
	//	// SnowMountain Boss
	//	// BookMark_Saved = XMMatrixSet();
	//
	//	m_pTransformCom->Set_WorldMatrix(BookMark_Saved);
	//}
	//if (m_pGameInstance->Key_Pressing(DIK_NUMPAD2))
	//{
	//	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fMouseSensor * fAction_X * fTimeDelta);
	//
	//}
	//if (m_pGameInstance->Key_Pressing(DIK_NUMPAD3))
	//{
	//	//�ʱ�ȭ 
	//	//fAction_X += fTimeDelta;
	//}

	// �ϸ�ũ 1 
	if (m_pGameInstance->Key_Pressing(DIK_LCONTROL) && m_pGameInstance->Key_Down(DIK_NUMPAD9))
	{
		m_vBookMark = m_pTransformCom->Get_WorldFloat4x4();
	}
	if (m_pGameInstance->Key_Pressing(DIK_LSHIFT) && m_pGameInstance->Key_Down(DIK_NUMPAD9))
	{
		if (true == IsMatrixEmpty(m_vBookMark)) // �����ڵ� 
			return S_OK;

		_matrix NewWorld = XMLoadFloat4x4(&m_vBookMark);
		m_pTransformCom->Set_WorldMatrix(NewWorld);
	}

	// �ϸ�ũ 2
	if (m_pGameInstance->Key_Pressing(DIK_LCONTROL) && m_pGameInstance->Key_Down(DIK_NUMPAD8))
	{
		m_vBookMark2 = m_pTransformCom->Get_WorldFloat4x4();
	}
	if (m_pGameInstance->Key_Pressing(DIK_LSHIFT) && m_pGameInstance->Key_Down(DIK_NUMPAD8))
	{
		if (true == IsMatrixEmpty(m_vBookMark2))
			return S_OK;

		_matrix NewWorld = XMLoadFloat4x4(&m_vBookMark2);
		m_pTransformCom->Set_WorldMatrix(NewWorld);
	}

	return S_OK;
}

_bool CCamera_Dynamic::IsMatrixEmpty(const float4x4& matrix)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (matrix.m[i][j] != 0.0f)
			{
				return false;
			}
		}
	}
	return true;
}

HRESULT CCamera_Dynamic::Ready_Components()
{
	return S_OK;
}

CCamera_Dynamic * CCamera_Dynamic::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring& strPrototypeTag)
{
	CCamera_Dynamic*		pInstance = new CCamera_Dynamic(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Dynamic");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCamera_Dynamic::Clone(void* pArg)
{
	CCamera_Dynamic*		pInstance = new CCamera_Dynamic(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Dynamic");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CCamera_Dynamic::Pool()
{
	return new CCamera_Dynamic(*this);
}


void CCamera_Dynamic::Free()
{
	__super::Free();
}

