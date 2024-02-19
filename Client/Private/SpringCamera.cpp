#include "stdafx.h"
#include "SpringCamera.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Character.h"

CSpringCamera::CSpringCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CCamera(pDevice, pContext, strPrototypeTag)
{
}

CSpringCamera::CSpringCamera(const CSpringCamera& rhs)
	: CCamera(rhs)
{
}

HRESULT CSpringCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSpringCamera::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	SPRING_CAMERA_DESC* pDesc = (SPRING_CAMERA_DESC*)pArg;

	m_fMouseSensor = pDesc->fMouseSensor;

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;
	{
		DampConstant = 2.f * sqrt(SpringConstant);

		hDist = 0.7f; //Z �� ī�޶�� �÷��̾� �Ÿ�
		vDist = 0.7f; //Y �� ī�޶�� �÷��̾� �Ÿ�
		m_CameraOffsetY = 3.5f;
		m_CameraOffsetZ = -7.0f;

		m_pPlayer = m_pGameInstance->Get_Player();
		m_ptarget = dynamic_cast<CTransform*>(m_pGameInstance->Get_Player()->Get_Transform());
		ActualPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	}
	ShowCursor(FALSE);
	return S_OK;
}

void CSpringCamera::Priority_Tick(_float fTimeDelta)
{

}

void CSpringCamera::Tick(_float fTimeDelta)
{


	//CCharacter* m_pTargetCharacter = m_pPlayer->Get_Target();
	//if (nullptr != m_pTargetCharacter && nullptr != m_pTargetCharacter->Get_TransformComp())
	//{
	//	hDist = 0.5f;
	//	vDist = 0.5f;

	//	_vector vTargetPos = XMLoadFloat4(&m_pTargetCharacter->Get_Pos_Float4());
	//	m_pTransformCom->Look_At(vTargetPos);

	//	//m_fAngle = m_ptarget->Calc_Radian(vTargetPos);

	//	Lock_On(fTimeDelta);

	//}
	//else 
	{

		/*if (0.7 - fTimeDelta > hDist)
		{
			hDist += fTimeDelta;
		}
		else
		{
			hDist = 0.7f;
		}

		if (0.7 - fTimeDelta > vDist)
		{
			vDist += fTimeDelta;
		}
		else
		{
			vDist = 0.7f;
		}*/

		m_pTransformCom->Look_At(m_ptarget->Get_State(CTransform::STATE::STATE_POSITION));
		CameraRotation(fTimeDelta);

		//Player�� ��Ű�� ������ ī�޶� ȸ���ߴ� �����ʿ��� ȸ������ �޾Ƽ� ī�޶� �ٶ󺸰� �ִ� �������� �÷��̾ �Ĵ� ���� ���� 
		if (true == m_pPlayer->Is_Rotate_In_CameraDir())
		{
			RotatePlayer();
		}
	}




	if (m_pGameInstance->Key_Down(DIK_TAB))
	{
		if (m_bFix)
		{
			m_bFix = false;
			m_bCheck = false;
		}
		else
		{
			m_bFix = true;
			m_bCheck = true;
		}
	}
	if (m_bCheck == false)
		ShowCursor(FALSE);
	else
		ShowCursor(TRUE);


	if (false == m_bFix)
		return;
	if (true == m_bFix)
	{
		Mouse_Fix();
	}

	__super::Tick(fTimeDelta);
}

void CSpringCamera::Late_Tick(_float fTimeDelta)
{
	// ī�޶� �÷��̾�� ���� Tick���� ��ġ ����ϰ� �����ӱ��� �־������ ȭ���� �����Ÿ�
	//�׷��� ������ �ڵ�� Late_Tick���ٰ� �־�����! 

	//CCharacter* m_pTargetCharacter = m_pPlayer->Get_Target();
	//if (m_pTargetCharacter) return;

	_long	MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMS_X);
	_long	MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMS_Y);

	if (MouseMoveX != 0)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fMouseSensor * MouseMoveX * fTimeDelta);
	}

	if (MouseMoveY != 0)
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), m_fMouseSensor * MouseMoveY * fTimeDelta);
	}

}

_bool CSpringCamera::Write_Json(json& Out_Json)
{
	//Out_Json["Name"] = m_sName;
	//__super::Write_Json(Out_Json);

	return false;
}


void CSpringCamera::CameraRotation(_float fTimeDelta)
{
	//ī�޶� �������� Late_Tick�� �ִ�!
	_float3 currentCameraPosition = ActualPosition;
	_float3 idealPosition = m_ptarget->Get_State(CTransform::STATE_POSITION);
	_float3 displacement = ActualPosition - idealPosition;
	_float3 SpringAccel = (-SpringConstant * displacement) - (DampConstant * Velocity);
	Velocity += SpringAccel * fTimeDelta;
	ActualPosition += Velocity * fTimeDelta;
	_long	MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMS_X);
	_long	MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMS_Y);
	// ĳ���� ������ �߽����� �ϴ� ȸ���� ���
	m_fAngle += m_fMouseSensor * MouseMoveX * fTimeDelta;
	m_fPitch += m_fMouseSensor * MouseMoveY * fTimeDelta;

	//pitch ���� ����
	m_fPitch = max(-XM_PIDIV2, min(XM_PIDIV2, m_fPitch));

	// ȸ�� ��� ���
	_matrix rotationMatrix = XMMatrixRotationRollPitchYaw(m_fPitch, m_fAngle, 0.0f);

	// ī�޶� ��ġ ����
	currentCameraPosition.x = XMVectorGetX(XMVectorLerp(XMLoadFloat3(&currentCameraPosition), XMLoadFloat3(&idealPosition), 1.0f - expf(-CameraMoveSpeed * fTimeDelta)));
	currentCameraPosition.y = XMVectorGetY(XMVectorLerp(XMLoadFloat3(&currentCameraPosition), XMLoadFloat3(&idealPosition), 1.0f - expf(-CameraMoveSpeed * fTimeDelta)));
	currentCameraPosition.z = XMVectorGetZ(XMVectorLerp(XMLoadFloat3(&currentCameraPosition), XMLoadFloat3(&idealPosition), 1.0f - expf(-CameraMoveSpeed * fTimeDelta)));

	// ĳ���� ������ �߽����� �ϴ� ī�޶� ��ġ ���
	XMVECTOR cameraOffset = XMVectorSet(m_CameraOffsetX, m_CameraOffsetY, m_CameraOffsetZ, 0.0f);  // ī�޶��� �ʱ� ��ġ
	cameraOffset = XMVector3TransformCoord(cameraOffset, rotationMatrix);

	// ĳ������ ��ġ �� ȸ�� ����
	m_pTransformCom->Set_WorldMatrix(rotationMatrix * XMMatrixTranslationFromVector(ActualPosition));
	m_pTransformCom->Set_Position(currentCameraPosition + cameraOffset * hDist);
}

void CSpringCamera::Lock_On(_float fTimeDelta)
{
	//ī�޶� �������� Late_Tick�� �ִ�!
	_float3 currentCameraPosition = ActualPosition;
	_float3 idealPosition = m_ptarget->Get_State(CTransform::STATE_POSITION);
	_float3 displacement = ActualPosition - idealPosition;
	_float3 SpringAccel = (-SpringConstant * displacement) - (DampConstant * Velocity);
	Velocity += SpringAccel * fTimeDelta;
	ActualPosition += Velocity * fTimeDelta;

	//pitch ���� ����
	m_fPitch = max(-XM_PIDIV2, min(XM_PIDIV2, m_fPitch));

	// ȸ�� ��� ���
	_matrix rotationMatrix = XMMatrixRotationRollPitchYaw(m_fPitch, m_fAngle, 0.0f);

	// ī�޶� ��ġ ����
	currentCameraPosition.x = XMVectorGetX(XMVectorLerp(XMLoadFloat3(&currentCameraPosition), XMLoadFloat3(&idealPosition), 1.0f - expf(-CameraMoveSpeed * fTimeDelta)));
	currentCameraPosition.y = XMVectorGetY(XMVectorLerp(XMLoadFloat3(&currentCameraPosition), XMLoadFloat3(&idealPosition), 1.0f - expf(-CameraMoveSpeed * fTimeDelta)));
	currentCameraPosition.z = XMVectorGetZ(XMVectorLerp(XMLoadFloat3(&currentCameraPosition), XMLoadFloat3(&idealPosition), 1.0f - expf(-CameraMoveSpeed * fTimeDelta)));

	// ĳ���� ������ �߽����� �ϴ� ī�޶� ��ġ ���
	XMVECTOR cameraOffset = XMVectorSet(m_CameraOffsetX, m_CameraOffsetY, m_CameraOffsetZ, 0.0f);  // ī�޶��� �ʱ� ��ġ
	cameraOffset = XMVector3TransformCoord(cameraOffset, rotationMatrix);

	// ĳ������ ��ġ �� ȸ�� ����
	m_pTransformCom->Set_WorldMatrix(rotationMatrix * XMMatrixTranslationFromVector(ActualPosition));
	m_pTransformCom->Set_Position(currentCameraPosition + cameraOffset * hDist);

}

void CSpringCamera::RotatePlayer()
{
	//_matrix rotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, m_fAngle, 0.0f);

	m_pPlayer->Set_Rotate_In_CameraDir(false);

	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	m_ptarget->Rotation(vUp, m_fAngle);

	//m_ptarget->Set_WorldMatrix(rotationMatrix* XMMatrixTranslationFromVector(m_ptarget->Get_State(CTransform::STATE_POSITION)));
}

void CSpringCamera::Mouse_Fix()
{
	POINT   pt{ g_iWinSizeX >> 1, g_iWinSizeY >> 1 };

	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}


CSpringCamera* CSpringCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CSpringCamera* pInstance = new CSpringCamera(pDevice, pContext, strPrototypeTag);
	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSpringCamera");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CSpringCamera::Clone(void* pArg)
{
	CSpringCamera* pInstance = new CSpringCamera(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSpringCamera");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSpringCamera::Free()
{
	__super::Free();
}
