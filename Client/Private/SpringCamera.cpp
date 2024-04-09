#include "stdafx.h"
#include "SpringCamera.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Data_Manager.h"
#include "Player.h"
#include "MasterCamera.h"
#include "Bone.h"
#include "SMath.h"

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
		//�ΰ��ӿ��� ���� ������ �����ǰ��ΰŰ��� 
		m_CameraOffset.x = 1.f;
		m_CameraOffset.y = 0.5f;
		m_CameraOffset.z = -3.0f;
		
// 		_uint iCurrentLevel = m_pGameInstance->Get_NextLevel();
// 		
// 		if (iCurrentLevel != (_uint)LEVEL_TOOL)
// 		{
// 			m_pPlayer = CData_Manager::GetInstance()->Get_Player();
// 			m_ptarget = m_pPlayer->Get_Transform();
// 		}

		ActualPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	}
	_float4x4 BoneMatrix = {};
	CPlayer* pPlayer = CData_Manager::GetInstance()->Get_Player();
	m_pPlayer = CData_Manager::GetInstance()->Get_Player();

	BoneMatrix = pPlayer->Get_Body()->Get_BonePtr("Spine2")->Get_CombinedTransformationMatrix();
	_float4x4 pPlayerPos = pPlayer->Get_Transform()->Get_WorldMatrix();
	_float4x4 temp = {};
	XMStoreFloat4x4(&temp, BoneMatrix * pPlayerPos);
	m_TargetPosition.x = temp._41;
	m_TargetPosition.y = temp._42;
	m_TargetPosition.z = temp._43;
	
	//m_pCharacter = m_pGameInstance->Get


	if (m_pGameInstance->Get_NextLevel() == (_uint)LEVEL_TOOL)
		ShowCursor(true);
	else 
		ShowCursor(false);

	return S_OK;
}

void CSpringCamera::Priority_Tick(_float fTimeDelta)
{
	
}

void CSpringCamera::Tick(_float fTimeDelta)
{
	PreActualPosition = m_TargetPosition;

	
	if(true == m_pPlayer->m_bPlayerCheck && m_pPlayer != nullptr)
	{// ���� ���� ī�޶� 
		_float4x4 BoneMatrix = {};
		CPlayer* pPlayer = CData_Manager::GetInstance()->Get_Player();

		BoneMatrix = pPlayer->Get_Body()->Get_BonePtr("Spine2")->Get_CombinedTransformationMatrix();
		_float4x4 pPlayerPos = pPlayer->Get_Transform()->Get_WorldMatrix();
		_float4x4 temp = {};
		XMStoreFloat4x4(&temp, BoneMatrix * pPlayerPos);
		m_TargetPosition.x = temp._41;
		m_TargetPosition.y = temp._42;
		m_TargetPosition.z = temp._43;
		NewTargetPosition = m_TargetPosition;
	}
	else
	{
		// ���� ���� ī�޶� TEST
		_float4x4 BoneMatrix = {};
		
		BoneMatrix = m_pCharacter->Get_Body()->Get_BonePtr("Spine2")->Get_CombinedTransformationMatrix();
		_float4x4 pCharacterPos = m_pCharacter->Get_Transform()->Get_WorldMatrix();
		_float4x4 temp = {};
		XMStoreFloat4x4(&temp, BoneMatrix * pCharacterPos);
		m_TargetPosition.x = temp._41;
		m_TargetPosition.y = temp._42;
		m_TargetPosition.z = temp._43;
		NewTargetPosition = m_TargetPosition;
	}
	

	m_pTransformCom->Look_At(m_ptarget->Get_State(CTransform::STATE::STATE_POSITION));
	
	Lerp_CameraPosition(fTimeDelta);
	CameraRotation(fTimeDelta);
	
	

	//Player�� ��Ű�� ������ ī�޶� ȸ���ߴ� �����ʿ��� ȸ������ �޾Ƽ� ī�޶� �ٶ󺸰� �ִ� �������� �÷��̾ �Ĵ� ���� ���� 
	if (true == m_pPlayer->Is_Rotate_In_CameraDir() && true == m_pPlayer->m_bPlayerCheck)
	{
		RotatePlayer();
	}
	else
	{
		int i = 0;
	}

	CData_Manager* pDataManager = CData_Manager::GetInstance();
	GAME_STATE eGameState = pDataManager->Get_GameState();

	if (GAME_STATE::GAMEPLAY != eGameState)
	{
		m_bFix = false;
		m_bCheck = false;

		m_bIsGamePlay = false;
	}
	else if (false == m_bIsGamePlay && GAME_STATE::GAMEPLAY == eGameState)
	{
		m_bFix = true;
		m_bCheck = true;

		m_bIsGamePlay = true;
	}
	else if (true == m_bIsGamePlay && true == m_pGameInstance->Key_Down(DIK_TAB))
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

	if (m_pGameInstance->Key_Pressing(DIK_LSHIFT))
	{
		if (m_pGameInstance->Key_Down(DIK_F2))
			CData_Manager::GetInstance()->Get_MasterCamera()->Set_CameraType(CMasterCamera::DynamicCamera);
	}

	_uint iCurrentLevel = m_pGameInstance->Get_NextLevel();
	if (iCurrentLevel != (_uint)LEVEL_TOOL)
	{
		if (m_bCheck == false)
			ShowCursor(FALSE);
		else
			ShowCursor(TRUE);
	}

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

	Shake_Camera(fTimeDelta);

	

}

HRESULT CSpringCamera::Ready_Components()
{
	/* For.Com_Collider */
	CBounding_Sphere::BOUNDING_SPHERE_DESC		BoundingDesc = {};
	BoundingDesc.iLayer = ECast(COLLISION_LAYER::MONSTER);
	BoundingDesc.fRadius = 1.0f;
	BoundingDesc.vCenter = _float3(0.f, 1.f, 0.f);


	if (FAILED(__super::Add_Component(m_pGameInstance->Get_NextLevel(), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingDesc)))
		return E_FAIL;

	return S_OK;
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
	idealPosition = m_TargetPosition;
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
	XMVECTOR cameraOffset = XMVectorSet(m_CameraOffset.x, m_CameraOffset.y, m_CameraOffset.z, 0.0f);  // ī�޶��� �ʱ� ��ġ
	cameraOffset = XMVector3TransformCoord(cameraOffset, rotationMatrix);

	// ĳ������ ��ġ �� ȸ�� ����
	m_pTransformCom->Set_WorldMatrix(rotationMatrix * XMMatrixTranslationFromVector(ActualPosition));
	m_pTransformCom->Set_Position(currentCameraPosition + cameraOffset * hDist);
}

void CSpringCamera::StartCameraRotation(_fmatrix StartRotationMatrix)
{
	m_pTransformCom->Set_WorldMatrix(StartRotationMatrix * XMMatrixTranslationFromVector(ActualPosition));
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
	currentCameraPosition = XMVectorLerp(XMLoadFloat3(&currentCameraPosition), XMLoadFloat3(&idealPosition), 1.0f - expf(-CameraMoveSpeed * fTimeDelta));
	//currentCameraPosition.y = XMVectorGetY(XMVectorLerp(XMLoadFloat3(&currentCameraPosition), XMLoadFloat3(&idealPosition), 1.0f - expf(-CameraMoveSpeed * fTimeDelta)));
	//currentCameraPosition.z = XMVectorGetZ(XMVectorLerp(XMLoadFloat3(&currentCameraPosition), XMLoadFloat3(&idealPosition), 1.0f - expf(-CameraMoveSpeed * fTimeDelta)));

	// ĳ���� ������ �߽����� �ϴ� ī�޶� ��ġ ���
	XMVECTOR cameraOffset = XMVectorSet(m_CameraOffset.x, m_CameraOffset.y, m_CameraOffset.z, 0.0f);  // ī�޶��� �ʱ� ��ġ
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

void CSpringCamera::Set_CameraOffset(_float3 _CameraOffset)
{
	if (m_CameraOffset != _CameraOffset)
	{
		m_bChangeOffset = true;
		NewTargetPosition = m_TargetPosition;
	}
	
	m_CameraOffset = _CameraOffset;

}

void CSpringCamera::Set_pTargetCharacter(CCharacter* _pCharacter)
{
	m_pCharacter = _pCharacter;
}

void CSpringCamera::Lerp_CameraPosition(_float fTimeDelta)
{
	// ���� ī�޶� ��ġ�� ���� Ÿ�� ��ġ ������ �Ÿ� ���
	_float3 direction = NewTargetPosition - ActualPosition;
	_float distance = XMVectorGetX(XMVector3Length(direction));

	// ���� ������ ���� ī�޶� ��ġ�� ���� Ÿ�� ��ġ�� �̵�
	_float moveDistance = CameraMoveSpeedtest * fTimeDelta;
	if (moveDistance > distance)
	{
		// �̵� �Ÿ��� ���� �Ÿ����� ũ�� �ٷ� ���� Ÿ�� ��ġ�� �̵�
		ActualPosition = NewTargetPosition;
	}
	else
	{
		// ���� ������ �̵�
		_float3 moveVector = XMVector3Normalize(direction) * moveDistance;
		ActualPosition += moveVector;
	}
// 	// ������ ����� ���� (LerpTargetPosition) �߰�
// 	_float3 LerpTargetPosition = PreActualPosition;
// 
// 	// ī�޶� ������ ���� �� ����
// 	if (m_bChangeOffset)
// 	{
// 		// ���ο� Ÿ�� ��ġ ����
// 		LerpTargetPosition = NewTargetPosition;
// 		m_bChangeOffset = false; // ���� �Ϸ� ǥ��
// 	}
// 
// 	m_TargetPosition = XMVectorLerp(XMLoadFloat3(&m_TargetPosition), XMLoadFloat3(&LerpTargetPosition), 0.1f - expf(-CameraMoveSpeedtest * fTimeDelta));
}

void CSpringCamera::Shake_Camera(_float fTimeDelta)
{
	_float3 Temp = {};
	if (m_bShake)
	{
		_float fRandomX = SMath::fRandom(-m_fShakeMinMax.x, m_fShakeMinMax.y);
		_float fRandomY = SMath::fRandom(-m_fShakeMinMax.x, m_fShakeMinMax.y);
		_float fRandomZ = SMath::fRandom(-m_fShakeMinMax.x, m_fShakeMinMax.y);
		
		m_fShakeTime -= fTimeDelta;
		Temp = m_bShake ? _float3(fRandomX, fRandomY, fRandomZ): _float3(0.0f, 0.0f, 0.0f);
		if (m_fShakeTime <= 0.f)
		{
			m_bShake = false;
			m_fShakeTime = 0.f;
		}
	}
	ActualPosition += Temp;

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

Engine::CGameObject* CSpringCamera::Pool()
{
	return new CSpringCamera(*this);
}
