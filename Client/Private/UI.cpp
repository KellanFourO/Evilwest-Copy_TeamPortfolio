#include "..\Public\UI.h"
#include "GameInstance.h"
#include "SMath.h"
#include "Data_Manager.h"
#include "UI_Manager.h"


CUI::CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CGameObject(pDevice, pContext, strPrototypeTag)
	, m_pData_Manager(CData_Manager::GetInstance())
	, m_pUIManager(CUI_Manager::GetInstance())
{
	Safe_AddRef(m_pData_Manager);
	Safe_AddRef(m_pUIManager);
}

CUI::CUI(const CUI& rhs)
	: CGameObject(rhs)
	, m_ProjMatrix(rhs.m_ProjMatrix)
	, m_pData_Manager(rhs.m_pData_Manager)
	, m_pUIManager(rhs.m_pUIManager)
{
	Safe_AddRef(m_pData_Manager);
	Safe_AddRef(m_pUIManager);
}

HRESULT CUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI::Initialize(void* pArg)
{
	/* Info */
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

#pragma region Transform
	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext, 1.f, 1.f);

	if (nullptr == m_pTransformCom)
		return E_FAIL;

	if (nullptr != Find_Component(g_pTransformTag))
		return E_FAIL;

	m_Components.emplace(g_pTransformTag, m_pTransformCom);
	Safe_AddRef(m_pTransformCom);
#pragma endregion End

#pragma region 2D
	//if (m_tUIInfo.bWorld == false)
	{
		if (m_tUIInfo.fPositionZ == 0.f)
			m_tUIInfo.fPositionZ = 0.1f;

		m_pTransformCom->Set_Scaling(m_tUIInfo.fScaleX, m_tUIInfo.fScaleY, m_fScaleZ);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tUIInfo.fPositionX - (_float)g_iWinSizeX * 0.5f, -m_tUIInfo.fPositionY + (_float)g_iWinSizeY * 0.5f, m_tUIInfo.fPositionZ, 1.f));

		if (m_tUIInfo.bWorld == true)
		{
			// View ���� ī�޶� ���� �����̴� : ī�޶� ��ġ�� ��, ī�޶� �� ��, ī�޶��� UP vector
			m_ViewMatrix = XMMatrixLookAtLH({ 0.f, 0.f, -10.f, 0.f }, { 0.f, 0.f, 10.f, 0.f }, { 0.f, 1.f, 0.f, 0.f });

			// ���� Proj�� �����ش�.
			XMStoreFloat4x4(&m_ProjMatrix, m_pGameInstance->Get_TransformFloat4x4Inverse(CPipeLine::D3DTS_PROJ));

			// �� Proj�� �÷��ش�.
			XMStoreFloat4x4(&m_ProjMatrix, m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ));
		}
		else
		{
			XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
			XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.1f, 1.f));
		}

		m_fPositionX = m_tUIInfo.fPositionX;
		m_fPositionY = m_tUIInfo.fPositionY;
		m_fScaleX = m_tUIInfo.fScaleX;
		m_fScaleY = m_tUIInfo.fScaleY;

		SetUp_UIRect(m_fPositionX, m_fPositionY, m_fScaleX, m_fScaleY);
	}
	/*else
	{
		m_pTransformCom->Set_Scaling(m_tUIInfo.fScaleX, m_tUIInfo.fScaleY, m_fScaleZ);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tUIInfo.fPositionX, m_tUIInfo.fPositionY, m_tUIInfo.fPositionZ, 1.f));

		m_fPositionX = m_tUIInfo.fPositionX;
		m_fPositionY = m_tUIInfo.fPositionY;
		m_fScaleX = m_tUIInfo.fScaleX;
		m_fScaleY = m_tUIInfo.fScaleY;

		SetUp_UIRect(m_fPositionX, m_fPositionY, m_fScaleX, m_fScaleY);
	}*/
#pragma endregion End

	m_Origin_WorldMatrix = m_pTransformCom->Get_WorldMatrix();
	//FAILED_CHECK(Ready_UI("../Bin/DataFiles/Data_UI/UI_Info"));

	return S_OK;
}

void CUI::Priority_Tick(_float fTimeDelta)
{

}

void CUI::Tick(_float fTimeDelta)
{
	//if (m_bTool == true && m_pGameInstance->Get_CurrentLevel() == (_uint)LEVEL::LEVEL_TOOL)
	//	return;
	
	/* World or Orthogonal */
	//Check_Change_WorldUI(fTimeDelta);

	/* Check_MouseInput */
	if(m_pData_Manager->Get_GameState() == GAME_STATE::UI)
		Check_MouseInput(fTimeDelta);

	if (m_pGameInstance->Key_Pressing(DIK_LCONTROL))
	{
		if (m_pGameInstance->Key_Down(DIK_Q))
			m_pUIManager->Set_Active(UITYPE::QUESTBOX);
		if (m_pGameInstance->Key_Down(DIK_W))
			m_pUIManager->Set_Active(UITYPE::REWARD);
		if (m_pGameInstance->Key_Down(DIK_E))
			m_pUIManager->Set_Active(UITYPE::TUTORIALBOX);
	}

	switch (m_eState) // Tick���ٺ��� m_eState�� ��� STATE_ENDŸ������ �ٲ�� ������ �Ͼ... �ذ� : (���ڱ� ��)
	{
		case Client::UISTATE::READY:
			UI_Ready(fTimeDelta);
			break;
		case Client::UISTATE::ENTER:
			UI_Enter(fTimeDelta);
			break;
		case Client::UISTATE::LOOP:
			UI_Loop(fTimeDelta);
			break;
		case Client::UISTATE::EXIT:
			UI_Exit(fTimeDelta);
			break;
		case Client::UISTATE::PLAYER_HUD:
			Player_HUD(fTimeDelta);
			break;
		case Client::UISTATE::TUTORIAL_BOX:

			break;
		case Client::UISTATE::STATE_END:
			break;
		default:
			break;
	}

	if (m_bActive == false)	// ==================== Active ====================
		return;

	Play_Animation(fTimeDelta);
	Update_Child_Transform();
	//if(m_tUIInfo.bWorld == false)
	Check_RectPos();

#ifdef _DEBUG
	m_bButtonUI = true;
#endif // _DEBUG
	Picking_UI();
}

void CUI::UI_Ready(_float fTimeDelta)
{
}

void CUI::UI_Enter(_float fTimeDelta)
{
}

void CUI::UI_Loop(_float fTimeDelta)
{
}

void CUI::UI_Exit(_float fTimeDelta)
{
}

void CUI::UI_AppearTick(_float fTimeDelta)
{
}

void CUI::UI_DisappearTick(_float fTimeDelta)
{
}

void CUI::Late_Tick(_float fTimeDelta)
{

}

HRESULT CUI::Render()
{
	return S_OK;
}

void CUI::Picking_UI()
{
	if (m_bButtonUI == false)
		return;

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);  // Ŭ���̾�Ʈ ���� ���콺 ������ �������� 

	_uint ViewPortIndex = 1;
	CD3D11_VIEWPORT ViewPort;

	m_pContext->RSGetViewports(&ViewPortIndex, &ViewPort); // ����Ʈ �������� 

	if (PtInRect(&m_rcUI, pt))   //  PtInRect(��Ʈ�ּ�, ���콺 ����Ʈ)
	{
		m_bPick = true;
		//if (m_pGameInstance->Mouse_Down(DIM_LB)) // �ٸ������� �Է��� ��ĥ�Ű����� �������� �� �۷ι� ������ ���콺 Ŭ���� �ϳ� �δ°� ��������
		//	m_bSelect = true;
		//if (m_pGameInstance->Mouse_Pressing(DIM_LB))
		//	m_bSelectPressing = true;
	}
	else
	{
		m_bPick = false;
		//m_bSelect = false;
		//m_bSelectPressing = false;
	}
}

void CUI::Check_RectPos()
{
	m_fPositionX = m_pTransformCom->Get_Position().x + g_iWinSizeX * 0.5f;
	m_fPositionY = -m_pTransformCom->Get_Position().y + g_iWinSizeY * 0.5f;

	m_fScaleX = m_pTransformCom->Get_Scaled().x;
	m_fScaleY = m_pTransformCom->Get_Scaled().y;

	m_rcUI.left = LONG(m_fPositionX - (m_fScaleX / 2));
	m_rcUI.top = LONG(m_fPositionY - (m_fScaleY / 2));
	m_rcUI.right = LONG(m_fPositionX + (m_fScaleX / 2));
	m_rcUI.bottom = LONG(m_fPositionY + (m_fScaleY / 2));
}

HRESULT CUI::Ready_Components()
{
	if (m_tUIInfo.bDistortionUI == false) // Distortion�� ��� ���ϴ� UI�� ���
		return S_OK;

	//! For.Com_Distortion_Mask
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Mask"),
		TEXT("Com_Distortion_Mask"), reinterpret_cast<CComponent**>(&m_pDistortionCom[MASK]))))
		return E_FAIL;

	//! For.Com_Distortion_Noise
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Noise"),
		TEXT("Com_Distortion_Noise"), reinterpret_cast<CComponent**>(&m_pDistortionCom[NOISE]))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI::Bind_ShaderResources()
{
	if (m_tUIInfo.bDistortionUI == false) // Distortion�� ��� ���ϴ� UI�� ���
		return S_OK;

	_float4 vColor = m_tUIInfo.vColor;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor_Mul", &vColor, sizeof(_float4))))
		return E_FAIL;

	_int iColorMode = (_int)m_tUIInfo.eColorMode;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_iColorMode", &iColorMode, sizeof(_int))))
		return E_FAIL;

	if (FAILED(m_pDistortionCom[MASK]->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture", m_iMaskNum)))
		return E_FAIL;

	if (FAILED(m_pDistortionCom[NOISE]->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", m_iNoiseNum)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI::Setting_Owner()
{
	return S_OK;
}

HRESULT CUI::SetUp_UIRect(_float fPosX, _float fPosY, _float fSizeX, _float fSizeY)
{
	/* ��Ʈ ũ�⸦ ǥ���Ҹ��� ����� ��ҷ� ���� ������ (Collider, Texture, Color..) */
	m_rcUI.left = static_cast<LONG>(fPosX - (fSizeX * 0.5f));
	m_rcUI.top = static_cast<LONG>(fPosY - (fSizeY * 0.5f));
	m_rcUI.right = static_cast<LONG>(fPosX + (fSizeX * 0.5f));
	m_rcUI.bottom = static_cast<LONG>(fPosY + (fSizeY * 0.5f));

	return S_OK;
}

void CUI::Set_Size(_float fSizeX, _float fSizeY)
{
	m_fScaleX = fSizeX;
	m_fScaleY = fSizeY;

	//m_pTransformCom->Set_Scaling(m_fScaleX, m_fScaleY, m_fScaleZ);
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION,
	//	_float3(m_fPositionX - g_iWinSizeX * 0.5f, -m_fPositionY + g_iWinSizeY * 0.5f, m_tUIInfo.fPositionZ));
}

void CUI::Change_SizeX(_float MMX)
{
	m_fScaleX += MMX;
	m_pTransformCom->Set_Scaling(m_fScaleX, m_fScaleY, m_fScaleZ);
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION,
	//	_float3(m_fPositionX - g_iWinSizeX * 0.5f, -m_fPositionY + g_iWinSizeY * 0.5f, m_tUIInfo.fPositionZ));
}

void CUI::Change_SizeY(_float MMY)
{
	m_fScaleY += MMY;
	m_pTransformCom->Set_Scaling(m_fScaleX, m_fScaleY, m_fScaleZ);
	//m_fPositionY = m_fPositionY;
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION,
	//	_float3(m_fPositionX - g_iWinSizeX * 0.5f, -m_fPositionY + g_iWinSizeY * 0.5f, m_tUIInfo.fPositionZ));
}

void CUI::Change_SizeRight(_float MMX)
{
	m_fScaleX += MMX;
	m_fPositionX += MMX * 0.5f;

	m_pTransformCom->Set_Scaling(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(m_fPositionX - g_iWinSizeX * 0.5f, -m_fPositionY + g_iWinSizeY * 0.5f, m_tUIInfo.fPositionZ));
}

void CUI::Change_SizeLeft(_float MMX)
{
	m_fScaleX += -1.f * MMX;
	m_fPositionX += MMX * 0.5f;

	m_pTransformCom->Set_Scaling(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(m_fPositionX - g_iWinSizeX * 0.5f, -m_fPositionY + g_iWinSizeY * 0.5f, m_tUIInfo.fPositionZ));
}

void CUI::Change_SizeTop(_float MMY)
{
	m_fScaleY += MMY;
	m_fPositionY -= MMY * 0.5f;

	m_pTransformCom->Set_Scaling(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(m_fPositionX - g_iWinSizeX * 0.5f, -m_fPositionY + g_iWinSizeY * 0.5f, m_tUIInfo.fPositionZ));
}

void CUI::Change_SizeBottom(_float MMY)
{
	m_fScaleY += MMY;
	m_fPositionY += MMY * 0.5f;

	m_pTransformCom->Set_Scaling(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(m_fPositionX - g_iWinSizeX * 0.5f, -m_fPositionY + g_iWinSizeY * 0.5f, m_tUIInfo.fPositionZ));
}

HRESULT CUI::Set_ParentTransform(CTransform* pParentTransformCom)
{
	m_tUIInfo.pParentTransformCom = pParentTransformCom;

	return S_OK;
}

void CUI::Check_Change_WorldUI(_float fTimeDelta)
{
	if (m_bChange_Proj == true)
	{
		if (m_tUIInfo.bWorld == true)
		{
			// View ���� ī�޶� ���� �����̴� : ī�޶� ��ġ�� ��, ī�޶� �� ��, ī�޶��� UP vector
			XMMatrixLookAtLH({ 0.f, 0.f, -10.f, 0.f }, { 0.f, 0.f, 10.f, 0.f }, { 0.f, 1.f, 0.f, 0.f });

			// ���� Proj�� �����ش�.
			XMStoreFloat4x4(&m_ProjMatrix, m_pGameInstance->Get_TransformFloat4x4Inverse(CPipeLine::D3DTS_PROJ));

			// �� Proj�� �÷��ش�.
			XMStoreFloat4x4(&m_ProjMatrix, m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ));
		}
		else
		{
			// View ����
			XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

			// ���� Proj�� �����ش�.
			XMStoreFloat4x4(&m_ProjMatrix, m_pGameInstance->Get_TransformFloat4x4Inverse(CPipeLine::D3DTS_PROJ));

			// �� Proj�� �÷��ش�.
			XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, m_tUIInfo.fPositionZ, 1.f));
		}

		m_bChange_Proj = false;
	}
}

void CUI::Check_MouseInput(_float fTimeDelta)
{
	/*if (m_pGameInstance->Mouse_Down(DIM_LB))
		g_UIMouseDownLB = true;

	if (m_pGameInstance->Mouse_Down(DIM_RB))
		m_bMouseDown_RB = true;
	else
		m_bMouseDown_RB = false;
	if (m_pGameInstance->Mouse_Down(DIM_MB))
		m_bMouseDown_MB = true;
	else
		m_bMouseDown_MB = false;


	if (m_pGameInstance->Mouse_Pressing(DIM_LB))
		m_bMousePressing_LB = true;
	else
		m_bMousePressing_LB = false;
	if (m_pGameInstance->Mouse_Pressing(DIM_RB))
		m_bMousePressing_RB = true;
	else
		m_bMousePressing_RB = false;
	if (m_pGameInstance->Mouse_Pressing(DIM_MB))
		m_bMousePressing_MB = true;
	else
		m_bMousePressing_MB = false;


	if (m_pGameInstance->Mouse_Up(DIM_LB))
	{
		m_bMouseUp_LB = true;
		g_UIMouseDownLB = false;
	}
	else
		m_bMouseUp_LB = false;
	if (m_pGameInstance->Mouse_Up(DIM_RB))
		m_bMouseUp_RB = true;
	else
		m_bMouseUp_RB = false;
	if (m_pGameInstance->Mouse_Up(DIM_MB))
		m_bMouseUp_MB = true;
	else
		m_bMouseUp_MB = false;*/

}

void CUI::Add_Create_Parts(void* pArg)
{
	if (pArg == nullptr)
		return;

	CUI::UI_DESC* pUIDesc = (CUI::UI_DESC*)pArg;

	wstring wstrLayerTag = TEXT("");
	m_pGameInstance->String_To_WString(pUIDesc->strLayerTag, wstrLayerTag);
	wstring wstrPartsTag = TEXT("");
	m_pGameInstance->String_To_WString(pUIDesc->strCloneTag, wstrPartsTag);
	CUI* pUI = dynamic_cast<CUI*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_STATIC, wstrLayerTag, wstrPartsTag, &pArg));
	m_vecUIParts.push_back(pUI);
}

void CUI::Add_Parts(CUI* pUI)
{
	if (pUI == nullptr)
		return;
	//pUI->Set_ParentTransform(m_pTransformCom);

	m_Origin_WorldMatrix = m_pTransformCom->Get_WorldFloat4x4();

	m_vecUIParts.push_back(pUI);
}

CUI* CUI::Get_UIPart(const wstring& strPartTag)
{
	for (auto& iter : m_vecUIParts)
	{
		//if (strPartTag == iter->Get_UITag())
		//	return	 iter;
	}

	return nullptr;
}

vector<CUI*> CUI::Get_UIParts()
{
	return m_vecUIParts;
}

void CUI::Set_PosZ(_float fZ)
{
	if (!m_vecAnimation.empty())
	{
		m_tUIInfo.fPositionZ = fZ;
	}
	else
	{
		m_tUIInfo.fPositionZ = fZ;
		m_pTransformCom->Set_Scaling(m_fScaleX, m_fScaleY, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			_float3(m_fPositionX - g_iWinSizeX * 0.5f, -m_fPositionY + g_iWinSizeY * 0.5f, m_tUIInfo.fPositionZ));
	}
}

void CUI::Set_Pos(_float fPosX, _float fPosY)
{
	m_fPositionX = fPosX;
	m_fPositionY = fPosY;

	m_pTransformCom->Set_Scaling(m_fScaleX, m_fScaleY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(m_fPositionX - g_iWinSizeX * 0.5f, -m_fPositionY + g_iWinSizeY * 0.5f, m_tUIInfo.fPositionZ));
}

void CUI::Moving_Picking_Point(POINT pt)
{
	m_fPositionX = (_float)pt.x;
	m_fPositionY = (_float)pt.y;

#ifdef _DEBUG
	if (!m_vecAnimation.empty())
	{

	}
#endif // _DEBUG


	m_pTransformCom->Set_Scaling(m_fScaleX, m_fScaleY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(m_fPositionX - g_iWinSizeX * 0.5f, -m_fPositionY + g_iWinSizeY * 0.5f, m_tUIInfo.fPositionZ));
}

void CUI::Parts_Delete()
{
	if (m_vecUIParts.empty())
		return;

	_int iSize = (_int)m_vecUIParts.size();

	for (_int i = 0; i < iSize; ++i)
	{
		if (m_vecUIParts[i]->m_bDead == true)
		{
			m_vecUIParts.erase(m_vecUIParts.begin() + i);
		}
	}
}

_float CUI::Check_CamToTarget_Distance(_vector vTargetPos)
{
	_vector		vCamPosition = XMLoadFloat4(&m_pGameInstance->Get_CamPosition());
	_float		fDistance = 0.0f;

	fDistance = XMVectorGetX(XMVector3Length(vTargetPos - vCamPosition));

	return fDistance;
}

void CUI::LifeOff(_float fTimeDelta)
{
	/* LifeTime�� �ִ� UI�� ��� */
	if (m_bLifeTimeUI == true)
	{
		if (m_fTime + m_fLifeTime < GetTickCount64())
		{
			m_bActive = Alpha_Plus(fTimeDelta); // �Ⱥ��̰� ���İ� �������� ���ش�.

			//m_fTime = GetTickCount64();
		}
	}
}

void CUI::LifeOn(_float fTimeDelta)
{
	Set_Alpha(0.f);			// UI ���İ� �ʱ�ȭ
	Set_Active(true);		// UI Ȱ��ȭ
	Set_AnimPlay(true);		// UI Animation ���
	Set_Disappear(false);	// UI ����� Off
	ResetTime();			// ! (LifeTime UI�� ���) UI TimeReset
}

void CUI::ResetTime()
{
	m_fTime = (_float)GetTickCount64(); // TimeReset
	m_bActive = true;
}

HRESULT CUI::SetUp_Transform(_float fPosX, _float fPosY, _float fSizeX, _float fSizeY)
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_fPositionX = fPosX;
	m_fPositionY = fPosY;
	m_fScaleX = fSizeX;
	m_fScaleY = fSizeY;

	m_pTransformCom->Set_Scaling(m_fScaleX, m_fScaleY, 1.f);

	// ��ġ �̵�
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_fPositionX - (_float)g_iWinSizeX * 0.5f, -m_fPositionY + (_float)g_iWinSizeY * 0.5f, m_tUIInfo.fPositionZ, 1.f));

	// View Matrix �� Projection Matrix ����
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, m_tUIInfo.fPositionZ, 1.f));

	return S_OK;
}

HRESULT CUI::Ready_UI(const char* cFilePath) // �����̳ʿ� ���� UI ������ �ҷ�����
{
	json json_in;

	_int		iPathNum = 0;


	CJson_Utility::Load_Json(cFilePath, json_in);

	for (auto& item : json_in.items())
	{
		json object = item.value();

		CUI::UI_DESC tUI_Info;

		tUI_Info.strLayerTag = object["LayerTag"];
		tUI_Info.strCloneTag = object["CloneTag"];
		tUI_Info.strProtoTag = object["ProtoTag"];
		tUI_Info.strFilePath = object["FilePath"];

		tUI_Info.strMapTextureTag = object["FilePath"];

		tUI_Info.iShaderNum = object["ShaderNum"];

		wstring wstrLayertag;
		m_pGameInstance->String_To_WString(tUI_Info.strLayerTag, wstrLayertag);

		wstring wstrClonetag;
		m_pGameInstance->String_To_WString(tUI_Info.strCloneTag, wstrClonetag);

		wstring wstrPrototag;
		m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

		wstring wstrFilePath;
		m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

		CUI* pUI = dynamic_cast<CUI*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_STATIC, wstrLayertag, wstrClonetag, &tUI_Info));

		pUI->Get_Transform()->Load_FromJson(object);

	}

	return S_OK;
}

HRESULT CUI::Create_UIParts(UI_DESC tUI_Desc)
{
	wstring wstrPrototag;
	m_pGameInstance->String_To_WString(tUI_Desc.strProtoTag, wstrPrototag);

	wstring wstrFilePath;
	m_pGameInstance->String_To_WString(tUI_Desc.strFilePath, wstrFilePath);

	CUI* pUI_Object = dynamic_cast<CUI*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_STATIC, TEXT("Layer_UI"), wstrPrototag, &tUI_Desc));
	m_vecUIParts.push_back(pUI_Object);

	return S_OK;
}

HRESULT CUI::Update_Child_Transform()
{
	if (m_vecUIParts.empty())
		return S_OK;

	if (m_tUIInfo.bParent)
	{
		if (m_Origin_WorldMatrix != m_pTransformCom->Get_WorldFloat4x4())
		{
			_vector Origin_scale;
			_vector Curr_scale;
			_float3 Origin_rotation;
			_float3 Curr_rotation;
			_vector Origin_translation;
			_vector Curr_translation;

			/* ȸ�� */
			Origin_rotation = SMath::Extract_PitchYawRollFromRotationMatrix(m_Origin_WorldMatrix);
			Curr_rotation = SMath::Extract_PitchYawRollFromRotationMatrix(m_pTransformCom->Get_WorldFloat4x4());

			_float RotX = Curr_rotation.x - Origin_rotation.x;
			_float RotY = Curr_rotation.y - Origin_rotation.y;
			_float RotZ = Curr_rotation.z - Origin_rotation.z;

			/* ũ�� */
			Origin_scale = SMath::Get_Scale(m_Origin_WorldMatrix);
			Curr_scale = SMath::Get_Scale(m_pTransformCom->Get_WorldFloat4x4());

			_vector vOrigin_rotation;
			_vector vCurr_rotation;

			XMMATRIX RotMat;
			RotMat = XMMatrixRotationRollPitchYaw(RotX, RotY, RotZ);

			XMMatrixDecompose(&Origin_scale, &vOrigin_rotation, &Origin_translation, m_Origin_WorldMatrix);
			XMMatrixDecompose(&Curr_scale, &vCurr_rotation, &Curr_translation, m_pTransformCom->Get_WorldFloat4x4());

			_float3 Result_Scale = Curr_scale / Origin_scale;
			_float3 Result_Rotation = vCurr_rotation - vOrigin_rotation;
			_float3 Result_Position = Curr_translation - Origin_translation;


			for (auto& iter : m_vecUIParts)
			{
				_vector Child_Scale = iter->Get_Transform()->Get_Scaled();
				_vector Child_Rot = iter->Get_Transform()->Get_Rotated();
				_vector Child_Pos = iter->Get_Transform()->Get_State(CTransform::STATE_POSITION);

				_float3 Finish_Scale;
				_float3 Finish_Rot;
				_float3 Finish_Pos;

				Finish_Scale.x = Result_Scale.x * Child_Scale.m128_f32[0];
				Finish_Scale.y = Result_Scale.y * Child_Scale.m128_f32[1];
				Finish_Scale.z = Result_Scale.z * Child_Scale.m128_f32[2];

				Finish_Rot.x = Result_Rotation.x + Child_Rot.m128_f32[0];
				Finish_Rot.y = Result_Rotation.y + Child_Rot.m128_f32[1];
				Finish_Rot.z = Result_Rotation.z + Child_Rot.m128_f32[2];

				Finish_Pos.x = Result_Position.x + Child_Pos.m128_f32[0];
				Finish_Pos.y = Result_Position.y + Child_Pos.m128_f32[1];
				Finish_Pos.z = Result_Position.z + Child_Pos.m128_f32[2];

				iter->Get_Transform()->Set_Scaling(Finish_Scale.x, Finish_Scale.y, Finish_Scale.z); // Scale
				iter->Get_Transform()->Set_State(CTransform::STATE_POSITION, Finish_Pos);			// Pos
				iter->Set_WorldMatrix(iter->Get_Transform()->Get_WorldMatrix() * RotMat);			// Rot
			}
			m_Origin_WorldMatrix = m_pTransformCom->Get_WorldFloat4x4();
		}
	}

	return S_OK;
}
//				TargetPosition => Screen
void CUI::SetUp_PositionToScreen(_fvector vWorldPos)
{
	_vector vTargetPos = {};
	_float4 vViewPort = {};

	vTargetPos = vWorldPos;
	//vTargetPos.m128_f32[1] += 2.f;
	vTargetPos = XMVector3Transform(vTargetPos, m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
	vTargetPos = XMVector3TransformCoord(vTargetPos, m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));

	XMStoreFloat4(&vViewPort, vTargetPos);

	m_fWorldToScreenX = (vViewPort.x) * (g_iWinSizeX >> 1);
	m_fWorldToScreenY = /*abs*/((vViewPort.y) * (g_iWinSizeY >> 1));

	if (g_iWinSizeX < -(g_iWinSizeX * 0.5f))
	{
		m_fWorldToScreenY = -(g_iWinSizeX * 0.5f);
		//m_fWorldToScreenX = -300.f;
		//m_fWorldToScreenY = -300.f;
	}
	if (m_fWorldToScreenX > (g_iWinSizeX * 0.5f))
	{
		m_fWorldToScreenY = (g_iWinSizeX * 0.5f);
		//m_fWorldToScreenX = -300.f;
		//m_fWorldToScreenY = -300.f;
	}
	if (m_fWorldToScreenY < -(g_iWinSizeY * 0.5f))
	{
		m_fWorldToScreenY = -(g_iWinSizeY * 0.5f);
		//m_fWorldToScreenX = -300.f;
		//m_fWorldToScreenY = -300.f;
	}
	if(m_fWorldToScreenY > (g_iWinSizeY * 0.5f))
	{
		m_fWorldToScreenY = (g_iWinSizeY * 0.5f);
		//m_fWorldToScreenX = -300.f;
		//m_fWorldToScreenY = -300.f;
	}

	m_pTransformCom->Set_Position({ m_fWorldToScreenX, m_fWorldToScreenY, 1.f });

	return;
}

//				TargetWorld => Screen
void CUI::SetUp_WorldToScreen(_matrix matWorldPos, _float3 vOffsetPos)
{
	_vector vTargetPos = {};
	_float4 vViewPort = {};

	matTargetWorld = matWorldPos;

	vTargetPos = XMVectorSet(
		matTargetWorld.r[3].m128_f32[0] + vOffsetPos.x,
		matTargetWorld.r[3].m128_f32[1] + vOffsetPos.y,
		matTargetWorld.r[3].m128_f32[2] + vOffsetPos.z,
		1.0f);

	/* Distance Check */
	m_fTarget_Distance = Check_CamToTarget_Distance(vTargetPos);
	if (m_fTarget_Distance >= m_fActive_Distance)
	{
		m_bActive = false;
		return;
	}
	else
		m_bActive = true;


	//// z �� ���
	//float zDistance = matTargetWorld.r[3].m128_f32[2];

	//// ũ�� ����
	//float scaleFactor = 1.0f / (zDistance + m_pGameInstance->Get_CamPosition().z); // �Ÿ��� ���� ����
	//vTargetPos.m128_f32[0] *= scaleFactor;
	//vTargetPos.m128_f32[1] *= scaleFactor;

	//vTargetPos.m128_f32[1] += 2.f;
	vTargetPos = XMVector3Transform(vTargetPos, m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
	vTargetPos = XMVector3TransformCoord(vTargetPos, m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));

	XMStoreFloat4(&vViewPort, vTargetPos);


	m_fWorldToScreenX = (vViewPort.x) * (g_iWinSizeX >> 1);
	m_fWorldToScreenY = /*abs*/((vViewPort.y) * (g_iWinSizeY >> 1));

	_int iWinHalfX = (g_iWinSizeX >> 1);
	_int iWinHalfY = (g_iWinSizeY >> 1);

	//m_bActive = m_pGameInstance->isIn_WorldPlanes(vTargetPos, 5.f);

	//_vector vRight = XMVector3Cross(m_pGameInstance->Get_CamDirection(), XMVectorSet(0.f, 1.f, 0.f, 0.f));

	//m_bActive = Calculation_Direcion(vTargetPos, m_pGameInstance->Get_CamDirection());

	//if (m_fWorldToScreenX < (_float)iWinHalfX &&	// Right
	//	m_fWorldToScreenX > -(_float)iWinHalfX &&	// Left
	//	m_fWorldToScreenY < (_float)iWinHalfY &&	// Top
	//	m_fWorldToScreenY > -(_float)iWinHalfY)	// Bottom
	//{
	//	m_bActive = true;
	//}

	if (m_fWorldToScreenX < -(_float)iWinHalfX)
		//&& m_fWorldToScreenX > -(_float)2000.f)
	{
		if (m_fWorldToScreenX < -((_float)iWinHalfX + m_fScreenOffsetX))
			m_bActive = false;
		else
			m_bActive = true;

		m_fWorldToScreenX = -(_float)iWinHalfX;
		//m_fWorldToScreenY = m_fPreScreenY;

		//m_fWorldToScreenX = -300.f;
		//m_fWorldToScreenY = -300.f;
	}
	//else
	//{
	//	m_fPreScreenY = m_fWorldToScreenY;
	//}

	if (m_fWorldToScreenX > (_float)iWinHalfX)
		//&& m_fWorldToScreenX < (_float)2000.f)
	{
		if (m_fWorldToScreenX > ((_float)iWinHalfX + m_fScreenOffsetX))
			m_bActive = false;
		else
			m_bActive = true;

		m_fWorldToScreenX = (_float)iWinHalfX;
		//m_fWorldToScreenY = m_fPreScreenY;
		//m_fWorldToScreenX = -300.f;
		//m_fWorldToScreenY = -300.f;
	}
	//else
	//{
	//	m_fPreScreenY = m_fWorldToScreenY;
	//}

	if (m_fWorldToScreenY < -(_float)iWinHalfY)
	{
		if (m_fWorldToScreenY < -((_float)iWinHalfY + m_fScreenOffsetY))
			m_bActive = false;
		else
			m_bActive = true;
		m_fWorldToScreenY = -((_float)iWinHalfY);
		//m_fWorldToScreenX = -300.f;
		//m_fWorldToScreenY = -300.f;
	}

	if (m_fWorldToScreenY > (_float)iWinHalfY)
	{
		if (m_fWorldToScreenY > ((_float)iWinHalfY + m_fScreenOffsetY))
			m_bActive = false;
		else
			m_bActive = true;

		m_fWorldToScreenY = (_float)iWinHalfY;
		//m_fWorldToScreenX = -300.f;
		//m_fWorldToScreenY = -300.f;
	}

	m_bActive = m_pTransformCom->Calc_FrontCheck(vTargetPos);

	m_pTransformCom->Set_Position({ m_fWorldToScreenX, m_fWorldToScreenY, 1.f });

	return;
}

HRESULT CUI::SetUp_BillBoarding()
{
	_matrix CamWorldMatrix;

	CamWorldMatrix = m_pGameInstance->Get_TransformMatrixInverse(CPipeLine::D3DTS_VIEW);

	_float3 vScale = m_pTransformCom->Get_Scaled();

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, CamWorldMatrix.r[0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, CamWorldMatrix.r[1]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, CamWorldMatrix.r[2]);
	m_pTransformCom->Set_Scaling(vScale.x, vScale.y, vScale.z);


	return S_OK;
}

_bool CUI::Calculation_Direcion(_vector vTargetPos, _float4 vCurrentDir)
{
	_float fAngle = Target_Contained_Angle(vCurrentDir, vTargetPos);

	//cout << "Angle : " << fAngle << endl;

	if (0 <= fAngle && fAngle <= 90)
		return true;
	else if (-90 <= fAngle && fAngle < 0)
		return true;
	else if (fAngle > 90)
		return false;
	else if (fAngle < -90)
		return false;
	else
		return false;

}
	
_float CUI::Target_Contained_Angle(_vector vStandard, _float4 vTargetPos)
{
	 // �Լ����� : Look �������� ������ ������� +���̰� , ������ ������� - ���̰����� ���� ���ϵȴ�. 
	_vector vLook = XMVector3Normalize(vTargetPos - m_pTransformCom->Get_Pos());

	_vector vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));

	_float fAngle = acos(XMVectorGetX(XMVector3Dot(vStandard, vLook)));

	fAngle = XMConvertToDegrees(fAngle);

	_vector vJudge = XMVector3Cross(vStandard, vLook);

	_float fRotationDirection = XMVectorGetY(vJudge) < 0 ? -1.0f : 1.0f;

	return fAngle * fRotationDirection;
}

void CUI::Tick_LevelUp(_float fTimeDelta)
{
	LifeTime_LevelUp(fTimeDelta);
}

void CUI::Player_HUD(_float fTimeDelta)
{
	/* LifeTime UI */
	if (m_pData_Manager->Get_ShowInterface() == true)
	{// treu : LifeTime�� �ð�(m_fTime) ���� �ʱ�ȭ�ؼ� UI�� ��� ����д� (���̰��Ѵ�)
		m_pUIManager->Active_PlayerHUD();
	}
	else
	{// false : ���� �ð����� true���¿� �ʱ�ȭ�� ������ �ð�(m_fTime) ���� �Ѿ�� ������ �������� �Ѵ�. (�Ⱥ��̰��Ѵ�)
		LifeOff(fTimeDelta);
	}
}

void CUI::TutorialBox(_float fTimeDelta)
{
	/* LifeTime UI */
	if (m_pData_Manager->Get_ShowInterface() == true)
	{// treu : LifeTime�� �ð�(m_fTime) ���� �ʱ�ȭ�ؼ� UI�� ��� ����д� (���̰��Ѵ�)
		LifeOn(fTimeDelta);
	}
	else
	{// false : ���� �ð����� true���¿� �ʱ�ȭ�� ������ �ð�(m_fTime) ���� �Ѿ�� ������ �������� �Ѵ�. (�Ⱥ��̰��Ѵ�)
		LifeOff(fTimeDelta);
	}
}

void CUI::Check_Disappear(_float fTimeDelta)
{
	if (m_bDisappear == true)
	{
		m_bActive = Alpha_Plus(fTimeDelta);
	}
	else
	{
		m_bActive = Alpha_Minus(fTimeDelta);
	}
}

void CUI::Load_FromJson(const json& In_Json)
{
	// "KeyframeNum" Ű�� ������ �⺻�� ���
	_bool bKeyframeNum = In_Json.contains("KeyframeNum");
	m_tUIInfo.iKeyframeNum = bKeyframeNum ? In_Json["KeyframeNum"] : 0;
	
	for (_int i = 0; i < m_tUIInfo.iKeyframeNum; ++i) // 19. Keyframe
	{
		/* Keyframe */
		m_tUIInfo.tKeyframe.fTime = In_Json["Keyframe"][i]["Time"];
		m_tUIInfo.tKeyframe.fValue = In_Json["Keyframe"][i]["Value"];
		m_tUIInfo.tKeyframe.fAnimSpeed = In_Json["Keyframe"][i]["AnimSpeed"];
		m_tUIInfo.tKeyframe.iType = In_Json["Keyframe"][i]["Type"];
		m_tUIInfo.tKeyframe.isEaseIn = In_Json["Keyframe"][i]["EaseIn"];
		m_tUIInfo.tKeyframe.isEaseOut = In_Json["Keyframe"][i]["EaseOut"];
		m_tUIInfo.tKeyframe.iTexureframe = In_Json["Keyframe"][i]["Texureframe"];
		m_tUIInfo.tKeyframe.vScale.x = In_Json["Keyframe"][i]["ScaleX"];
		m_tUIInfo.tKeyframe.vScale.y = In_Json["Keyframe"][i]["ScaleY"];
		m_tUIInfo.tKeyframe.vPos.x = In_Json["Keyframe"][i]["PosX"];
		m_tUIInfo.tKeyframe.vPos.y = In_Json["Keyframe"][i]["PosY"];
		m_tUIInfo.tKeyframe.fRot = In_Json["Keyframe"][i]["Rot"];
		m_tUIInfo.tKeyframe.vKeyFramePos.x = In_Json["Keyframe"][i]["KeyFramePosX"];
		m_tUIInfo.tKeyframe.vKeyFramePos.y = In_Json["Keyframe"][i]["KeyFramePosY"];

		m_tUIInfo.tKeyframe.fAlpha = In_Json["Keyframe"][i]["Alpha"];
		m_tUIInfo.tKeyframe.bActive = In_Json["Keyframe"][i]["Active"];
		m_tUIInfo.tKeyframe.bAppear = In_Json["Keyframe"][i]["Appear"];
		m_tUIInfo.tKeyframe.bTrigger = In_Json["Keyframe"][i]["Trigger"];

		if (In_Json["Keyframe"][i].contains("Disappear")) // "Disappear" Ű�� ������
			m_tUIInfo.tKeyframe.bDisappear = In_Json["Keyframe"][i]["Disappear"];

		if (In_Json["Keyframe"][i].contains("LoopSection"))// "LoopSection" Ű�� ������
			m_tUIInfo.tKeyframe.bLoopSection = In_Json["Keyframe"][i]["LoopSection"];

		if (In_Json["Keyframe"][i].contains("StopPlay"))// "LoopSection" Ű�� ������
			m_tUIInfo.tKeyframe.bStopPlay = In_Json["Keyframe"][i]["StopPlay"];

		if (In_Json["Keyframe"][i].contains("ReversePlay"))// "LoopSection" Ű�� ������
			m_tUIInfo.tKeyframe.bReversePlay = In_Json["Keyframe"][i]["ReversePlay"];

		if (In_Json["Keyframe"][i].contains("MaskChange"))// "LoopSection" Ű�� ������
			m_tUIInfo.tKeyframe.bMaskChange = In_Json["Keyframe"][i]["MaskChange"];

		if (In_Json["Keyframe"][i].contains("NoiseChange"))// "LoopSection" Ű�� ������
			m_tUIInfo.tKeyframe.bNoiseChange = In_Json["Keyframe"][i]["NoiseChange"];

		if (In_Json["Keyframe"][i].contains("TimeAcc")) // Ű�� ������
			m_tUIInfo.tKeyframe.fTimeAcc = In_Json["Keyframe"][i]["TimeAcc"];
		if (In_Json["Keyframe"][i].contains("ScrollSpeedsX")) // Ű�� ������
			m_tUIInfo.tKeyframe.vScrollSpeeds.x = In_Json["Keyframe"][i]["ScrollSpeedsX"];
		if (In_Json["Keyframe"][i].contains("ScrollSpeedsY")) // Ű�� ������
			m_tUIInfo.tKeyframe.vScrollSpeeds.y = In_Json["Keyframe"][i]["ScrollSpeedsY"];
		if (In_Json["Keyframe"][i].contains("ScrollSpeedsZ")) // Ű�� ������
			m_tUIInfo.tKeyframe.vScrollSpeeds.z = In_Json["Keyframe"][i]["ScrollSpeedsZ"];
		if (In_Json["Keyframe"][i].contains("ScalesX")) // Ű�� ������
			m_tUIInfo.tKeyframe.vScales.x = In_Json["Keyframe"][i]["ScalesX"];
		if (In_Json["Keyframe"][i].contains("ScalesY")) // Ű�� ������
			m_tUIInfo.tKeyframe.vScales.y = In_Json["Keyframe"][i]["ScalesY"];
		if (In_Json["Keyframe"][i].contains("ScalesZ")) // Ű�� ������
			m_tUIInfo.tKeyframe.vScales.z = In_Json["Keyframe"][i]["ScalesZ"];
		if (In_Json["Keyframe"][i].contains("Distortion1X")) // Ű�� ������
			m_tUIInfo.tKeyframe.vDistortion1.x = In_Json["Keyframe"][i]["Distortion1X"];
		if (In_Json["Keyframe"][i].contains("Distortion1Y")) // Ű�� ������
			m_tUIInfo.tKeyframe.vDistortion1.y = In_Json["Keyframe"][i]["Distortion1Y"];
		if (In_Json["Keyframe"][i].contains("Distortion2X")) // Ű�� ������
			m_tUIInfo.tKeyframe.vDistortion2.x = In_Json["Keyframe"][i]["Distortion2X"];
		if (In_Json["Keyframe"][i].contains("Distortion2Y")) // Ű�� ������
			m_tUIInfo.tKeyframe.vDistortion2.y = In_Json["Keyframe"][i]["Distortion2Y"];
		if (In_Json["Keyframe"][i].contains("Distortion3X")) // Ű�� ������
			m_tUIInfo.tKeyframe.vDistortion3.y = In_Json["Keyframe"][i]["Distortion3X"];
		if (In_Json["Keyframe"][i].contains("Distortion3Y")) // Ű�� ������
			m_tUIInfo.tKeyframe.vDistortion3.y = In_Json["Keyframe"][i]["Distortion3Y"];
		if (In_Json["Keyframe"][i].contains("DistortionScale")) // Ű�� ������
			m_tUIInfo.tKeyframe.fDistortionScale = In_Json["Keyframe"][i]["DistortionScale"];

		if (In_Json["Keyframe"].contains("DistortionUI")) // Ű�� ������
			m_tUIInfo.tKeyframe.bDistortionUI = In_Json["Distortion"][i]["DistortionUI"];
		if (In_Json["Keyframe"].contains("MaskNum")) // Ű�� ������
			m_tUIInfo.tKeyframe.iMaskNum = In_Json["Distortion"][i]["MaskNum"];
		if (In_Json["Keyframe"].contains("NoiseNum")) // Ű�� ������
			m_tUIInfo.tKeyframe.iNoiseNum = In_Json["Distortion"][i]["NoiseNum"];

		m_vecAnimation.push_back(m_tUIInfo.tKeyframe);
	}

	if (In_Json.contains("Distortion")) // Ű�� ������
	{
		if (In_Json["Distortion"].contains("TimeAcc")) // Ű�� ������
			m_tUIInfo.fTimeAcc = In_Json["Distortion"]["TimeAcc"];
		if (In_Json["Distortion"].contains("ScrollSpeedsX")) // Ű�� ������
			m_tUIInfo.vScrollSpeeds.x = In_Json["Distortion"]["ScrollSpeedsX"];
		if (In_Json["Distortion"].contains("ScrollSpeedsY")) // Ű�� ������
			m_tUIInfo.vScrollSpeeds.y = In_Json["Distortion"]["ScrollSpeedsY"];
		if (In_Json["Distortion"].contains("ScrollSpeedsZ")) // Ű�� ������
			m_tUIInfo.vScrollSpeeds.z = In_Json["Distortion"]["ScrollSpeedsZ"];
		if (In_Json["Distortion"].contains("ScalesX")) // Ű�� ������
			m_tUIInfo.vScales.x = In_Json["Distortion"]["ScalesX"];
		if (In_Json["Distortion"].contains("ScalesY")) // Ű�� ������
			m_tUIInfo.vScales.y = In_Json["Distortion"]["ScalesY"];
		if (In_Json["Distortion"].contains("ScalesZ")) // Ű�� ������
			m_tUIInfo.vScales.z = In_Json["Distortion"]["ScalesZ"];
		if (In_Json["Distortion"].contains("Distortion1X")) // Ű�� ������
			m_tUIInfo.vDistortion1.x = In_Json["Distortion"]["Distortion1X"];
		if (In_Json["Distortion"].contains("Distortion1Y")) // Ű�� ������
			m_tUIInfo.vDistortion1.y = In_Json["Distortion"]["Distortion1Y"];
		if (In_Json["Distortion"].contains("Distortion2X")) // Ű�� ������
			m_tUIInfo.vDistortion2.x = In_Json["Distortion"]["Distortion2X"];
		if (In_Json["Distortion"].contains("Distortion2Y")) // Ű�� ������
			m_tUIInfo.vDistortion2.y = In_Json["Distortion"]["Distortion2Y"];
		if (In_Json["Distortion"].contains("Distortion3X")) // Ű�� ������
			m_tUIInfo.vDistortion3.x = In_Json["Distortion"]["Distortion3X"];
		if (In_Json["Distortion"].contains("Distortion3Y")) // Ű�� ������
			m_tUIInfo.vDistortion3.y = In_Json["Distortion"]["Distortion3Y"];
		if (In_Json["Distortion"].contains("DistortionScale")) // Ű�� ������
			m_tUIInfo.fDistortionScale = In_Json["Distortion"]["DistortionScale"];

		if (In_Json["Distortion"].contains("DistortionUI")) // Ű�� ������
			m_tUIInfo.bDistortionUI = In_Json["Distortion"]["DistortionUI"];
		if (In_Json["Distortion"].contains("MaskNum")) // Ű�� ������
			m_tUIInfo.iMaskNum = In_Json["Distortion"]["MaskNum"];
		if (In_Json["Distortion"].contains("NoiseNum")) // Ű�� ������
			m_tUIInfo.iNoiseNum = In_Json["Distortion"]["NoiseNum"];
	}

}

json CUI::Save_Desc(json& out_json)
{
	//if (out_json.contains("Parent")) // Ű�� ������
		out_json["Parent"] = m_tUIInfo.bParent;
	//if (out_json.contains("World")) // Ű�� ������
		out_json["World"] = m_tUIInfo.bWorld;
	//if (out_json.contains("Group")) // Ű�� ������
		out_json["Group"] = m_tUIInfo.bGroup;

	//out_json["ScaleX"] = m_tUIInfo.fScaleX;

	//out_json["ScaleY"] = m_tUIInfo.fScaleY;

	//out_json["PositionX"] = m_tUIInfo.fPositionX;

	//out_json["PositionY"] = m_tUIInfo.fPositionY;

	//out_json["PositionZ"] = m_tUIInfo.fPositionZ;
	//if (out_json.contains("Alpha")) // Ű�� ������
		out_json["Alpha"] = m_tUIInfo.fAlpha;
		out_json["AlphaTrue"] = m_tUIInfo.fAlphaTrue;

	//if (out_json.contains("ObjectNum")) // Ű�� ������
		out_json["ObjectNum"] = m_tUIInfo.iObjectNum;
	//if (out_json.contains("ShaderNum")) // Ű�� ������
		out_json["ShaderNum"] = m_tUIInfo.iShaderNum;
	//if (out_json.contains("ObjectName")) // Ű�� ������
		out_json["UINum"] = m_tUIInfo.iUINum;
		out_json["UIName"] = m_tUIInfo.strUIName;

		out_json["ObjectName"] = m_tUIInfo.strObjectName;
	//if (out_json.contains("LayerTag")) // Ű�� ������
		out_json["LayerTag"] = m_tUIInfo.strLayerTag;
	//if (out_json.contains("CloneTag")) // Ű�� ������
		out_json["CloneTag"] = m_tUIInfo.strCloneTag;
	//if (out_json.contains("ProtoTag")) // Ű�� ������
		out_json["ProtoTag"] = m_tUIInfo.strProtoTag;
	//if (out_json.contains("FilePath")) // Ű�� ������
		out_json["FilePath"] = m_tUIInfo.strFilePath;
	//if (out_json.contains("MapTextureTag")) // Ű�� ������
		out_json["MapTextureTag"] = m_tUIInfo.strMapTextureTag;
	//if (out_json.contains("ColorR")) // Ű�� ������
		out_json["ColorR"] = m_tUIInfo.vColor.m128_f32[0];
	//if (out_json.contains("ColorG")) // Ű�� ������
		out_json["ColorG"] = m_tUIInfo.vColor.m128_f32[1];
	//if (out_json.contains("ColorB")) // Ű�� ������
		out_json["ColorB"] = m_tUIInfo.vColor.m128_f32[2];
	//if (out_json.contains("ColorA")) // Ű�� ������
		out_json["ColorA"] = m_tUIInfo.vColor.m128_f32[3];

	//if (out_json.contains("ColorMode")) // Ű�� ������
		out_json["ColorMode"] = m_tUIInfo.eColorMode;

		out_json["RenderGroup"] = m_tUIInfo.iRenderGroup;

		out_json["Level"] = m_tUIInfo.iLevel;
		out_json["MaxLevel"] = m_tUIInfo.iMaxLevel;

	/* TransformCom */
	m_pTransformCom->Write_Json(out_json);


	/* Keyframe*/
	if (!m_vecAnimation.empty())
	{
		_int iSize = (_int)m_vecAnimation.size();
		out_json["KeyframeNum"] = iSize;

		for (_int i = 0; i < iSize; ++i)
		{
			// Ű������ ���̺� �۾���
			out_json["Keyframe"][i]["Time"] = m_vecAnimation[i].fTime;
			out_json["Keyframe"][i]["Value"] = m_vecAnimation[i].fValue;
			out_json["Keyframe"][i]["AnimSpeed"] = m_vecAnimation[i].fAnimSpeed;
			out_json["Keyframe"][i]["Type"] = m_vecAnimation[i].iType;
			out_json["Keyframe"][i]["EaseIn"] = m_vecAnimation[i].isEaseIn;
			out_json["Keyframe"][i]["EaseOut"] = m_vecAnimation[i].isEaseOut;
			out_json["Keyframe"][i]["Texureframe"] = m_vecAnimation[i].iTexureframe;

			/* 2D */
			out_json["Keyframe"][i]["ScaleX"] = m_vecAnimation[i].vScale.x;
			out_json["Keyframe"][i]["ScaleY"] = m_vecAnimation[i].vScale.y;
			out_json["Keyframe"][i]["PosX"] = m_vecAnimation[i].vPos.x;
			out_json["Keyframe"][i]["PosY"] = m_vecAnimation[i].vPos.y;
			out_json["Keyframe"][i]["Rot"] = m_vecAnimation[i].fRot;

			/* 3D */
			out_json["Keyframe"][i]["World_ScaleX"] = m_vecAnimation[i].vWorld_Scale.x;
			out_json["Keyframe"][i]["World_ScaleY"] = m_vecAnimation[i].vWorld_Scale.y;
			out_json["Keyframe"][i]["World_ScaleZ"] = m_vecAnimation[i].vWorld_Scale.z;
			out_json["Keyframe"][i]["World_PosX"] = m_vecAnimation[i].vWorld_Pos.x;
			out_json["Keyframe"][i]["World_PosY"] = m_vecAnimation[i].vWorld_Pos.y;
			out_json["Keyframe"][i]["World_PosZ"] = m_vecAnimation[i].vWorld_Pos.z;
			out_json["Keyframe"][i]["World_RotX"] = m_vecAnimation[i].vWorld_Rot.x;
			out_json["Keyframe"][i]["World_RotY"] = m_vecAnimation[i].vWorld_Rot.y;
			out_json["Keyframe"][i]["World_RotZ"] = m_vecAnimation[i].vWorld_Rot.z;

			out_json["Keyframe"][i]["KeyFramePosX"] = m_vecAnimation[i].vKeyFramePos.x;
			out_json["Keyframe"][i]["KeyFramePosY"] = m_vecAnimation[i].vKeyFramePos.y;

			out_json["Keyframe"][i]["Alpha"] = m_vecAnimation[i].fAlpha;
			out_json["Keyframe"][i]["Active"] = m_vecAnimation[i].bActive;
			out_json["Keyframe"][i]["Appear"] = m_vecAnimation[i].bAppear;
			out_json["Keyframe"][i]["Trigger"] = m_vecAnimation[i].bTrigger;
			out_json["Keyframe"][i]["Disappear"] = m_vecAnimation[i].bDisappear;
			out_json["Keyframe"][i]["LoopSection"] = m_vecAnimation[i].bLoopSection;

			//if (out_json["Keyframe"][i].contains("StopPlay"))// "LoopSection" Ű�� ������
				out_json["Keyframe"][i]["StopPlay"] = m_vecAnimation[i].bStopPlay;

			//if (out_json["Keyframe"][i].contains("ReversePlay"))// "LoopSection" Ű�� ������
				out_json["Keyframe"][i]["ReversePlay"] = m_vecAnimation[i].bReversePlay;

			//if (out_json["Keyframe"][i].contains("MaskChange"))// "LoopSection" Ű�� ������
				out_json["Keyframe"][i]["MaskChange"] = m_vecAnimation[i].bMaskChange;

			//if (out_json["Keyframe"][i].contains("NoiseChange"))// "LoopSection" Ű�� ������
				out_json["Keyframe"][i]["NoiseChange"] = m_vecAnimation[i].bNoiseChange;

				out_json["Keyframe"][i]["TimeAcc"] = m_vecAnimation[i].fTimeAcc;
			//if (out_json.contains("Distortion")) // Ű�� ������
				out_json["Keyframe"][i]["ScrollSpeedsX"] = m_vecAnimation[i].vScrollSpeeds.x;
			//if (out_json.contains("Distortion")) // Ű�� ������
				out_json["Keyframe"][i]["ScrollSpeedsY"] = m_vecAnimation[i].vScrollSpeeds.y;
			//if (out_json.contains("Distortion")) // Ű�� ������
				out_json["Keyframe"][i]["ScrollSpeedsZ"] = m_vecAnimation[i].vScrollSpeeds.z;
			//if (out_json.contains("Distortion")) // Ű�� ������
				out_json["Keyframe"][i]["ScalesX"] = m_vecAnimation[i].vScales.x;
			//if (out_json.contains("Distortion")) // Ű�� ������
				out_json["Keyframe"][i]["ScalesY"] = m_vecAnimation[i].vScales.y;
			//if (out_json.contains("Distortion")) // Ű�� ������
				out_json["Keyframe"][i]["ScalesZ"] = m_vecAnimation[i].vScales.z;
			//if (out_json.contains("Distortion")) // Ű�� ������
				out_json["Keyframe"][i]["Distortion1X"] = m_vecAnimation[i].vDistortion1.x;
			//if (out_json.contains("Distortion")) // Ű�� ������
				out_json["Keyframe"][i]["Distortion1Y"] = m_vecAnimation[i].vDistortion1.y;
			//if (out_json.contains("Distortion")) // Ű�� ������
				out_json["Keyframe"][i]["Distortion2X"] = m_vecAnimation[i].vDistortion2.x;
			//if (out_json.contains("Distortion")) // Ű�� ������
				out_json["Keyframe"][i]["Distortion2Y"] = m_vecAnimation[i].vDistortion2.y;
			//if (out_json.contains("Distortion")) // Ű�� ������
				out_json["Keyframe"][i]["Distortion3X"] = m_vecAnimation[i].vDistortion3.y;
			//if (out_json.contains("Distortion")) // Ű�� ������
				out_json["Keyframe"][i]["Distortion3Y"] = m_vecAnimation[i].vDistortion3.y;
			//if (out_json.contains("Distortion")) // Ű�� ������
				out_json["Keyframe"][i]["DistortionScale"] = m_vecAnimation[i].fDistortionScale;

			//if (out_json["Distortion"].contains("DistortionUI")) // Ű�� ������
				out_json["Keyframe"][i]["DistortionUI"] = m_vecAnimation[i].bDistortionUI;
			//if (out_json["Distortion"].contains("MaskNum")) // Ű�� ������
				out_json["Keyframe"][i]["MaskNum"] = m_vecAnimation[i].iMaskNum;
			//if (out_json["Distortion"].contains("NoiseNum")) // Ű�� ������
				out_json["Keyframe"][i]["NoiseNum"] = m_vecAnimation[i].iNoiseNum;
		}
	}

	if (m_tUIInfo.bDistortionUI)
	{
			out_json["Distortion"]["TimeAcc"] = m_tUIInfo.fTimeAcc;
		//if (out_json["Distortion"].contains("ScrollSpeedsX")) // Ű�� ������
			out_json["Distortion"]["ScrollSpeedsX"] = m_tUIInfo.vScrollSpeeds.x;
		//if (out_json["Distortion"].contains("ScrollSpeedsY")) // Ű�� ������
			out_json["Distortion"]["ScrollSpeedsY"] = m_tUIInfo.vScrollSpeeds.y;
		//if (out_json["Distortion"].contains("ScrollSpeedsZ")) // Ű�� ������
			out_json["Distortion"]["ScrollSpeedsZ"] = m_tUIInfo.vScrollSpeeds.z;
		//if (out_json["Distortion"].contains("ScalesX")) // Ű�� ������
			out_json["Distortion"]["ScalesX"] = m_tUIInfo.vScales.x;
		//if (out_json["Distortion"].contains("ScalesY")) // Ű�� ������
			out_json["Distortion"]["ScalesY"] = m_tUIInfo.vScales.y;
		//if (out_json["Distortion"].contains("ScalesZ")) // Ű�� ������
			out_json["Distortion"]["ScalesZ"] = m_tUIInfo.vScales.z;
		//if (out_json["Distortion"].contains("Distortion1X")) // Ű�� ������
			out_json["Distortion"]["Distortion1X"] = m_tUIInfo.vDistortion1.x;
		//if (out_json["Distortion"].contains("Distortion1Y")) // Ű�� ������
			out_json["Distortion"]["Distortion1Y"] = m_tUIInfo.vDistortion1.y;
		//if (out_json["Distortion"].contains("Distortion2X")) // Ű�� ������
			out_json["Distortion"]["Distortion2X"] = m_tUIInfo.vDistortion2.x;
		//if (out_json["Distortion"].contains("Distortion2Y")) // Ű�� ������
			out_json["Distortion"]["Distortion2Y"] = m_tUIInfo.vDistortion2.y;
		//if (out_json["Distortion"].contains("Distortion3X")) // Ű�� ������
			out_json["Distortion"]["Distortion3X"] = m_tUIInfo.vDistortion3.x;
		//if (out_json["Distortion"].contains("Distortion3Y")) // Ű�� ������
			out_json["Distortion"]["Distortion3Y"] = m_tUIInfo.vDistortion3.y;
		//if (out_json["Distortion"].contains("DistortionScale")) // Ű�� ������
			out_json["Distortion"]["DistortionScale"] = m_tUIInfo.fDistortionScale;
			out_json["Distortion"]["DistortionBias"] = m_tUIInfo.fDistortionBias;
		//if (out_json["Distortion"].contains("DistortionUI")) // Ű�� ������
			out_json["Distortion"]["DistortionUI"] = m_tUIInfo.bDistortionUI;
		//if (out_json["Distortion"].contains("MaskNum")) // Ű�� ������
			out_json["Distortion"]["MaskNum"] = m_tUIInfo.iMaskNum;
		//if (out_json["Distortion"].contains("NoiseNum")) // Ű�� ������
			out_json["Distortion"]["NoiseNum"] = m_tUIInfo.iNoiseNum;
	}

	///* Group Save */
	//if (!m_vecUIParts.empty())
	//{
	//	_int iSize = m_vecUIParts.size();

	//	for (_int i = 0; i < iSize; ++i)
	//	{

	//	}
	//}

	return out_json;
}

json CUI::Save_Animation(json& out_json)
{
	/* Keyframe*/
	if (!m_vecAnimation.empty())
	{
		_int iSize = (_int)m_vecAnimation.size();
		out_json["KeyframeNum"] = iSize;

		for (_int i = 0; i < iSize; ++i)
		{
			// Ű������ ���̺� �۾���
			//if (out_json["Keyframe"][i].contains("Time")) // Ű�� ������
				out_json["Keyframe"][i]["Time"] = m_vecAnimation[i].fTime;
			//if (out_json["Keyframe"][i].contains("Value")) // Ű�� ������
				out_json["Keyframe"][i]["Value"] = m_vecAnimation[i].fValue;
			//if (out_json["Keyframe"][i].contains("AnimSpeed")) // Ű�� ������
				out_json["Keyframe"][i]["AnimSpeed"] = m_vecAnimation[i].fAnimSpeed;
			//if (out_json["Keyframe"][i].contains("Type")) // Ű�� ������
				out_json["Keyframe"][i]["Type"] = m_vecAnimation[i].iType;
			//if (out_json["Keyframe"][i].contains("EaseIn")) // Ű�� ������
				out_json["Keyframe"][i]["EaseIn"] = m_vecAnimation[i].isEaseIn;
			//if (out_json["Keyframe"][i].contains("EaseOut")) // Ű�� ������
				out_json["Keyframe"][i]["EaseOut"] = m_vecAnimation[i].isEaseOut;
			//if (out_json["Keyframe"][i].contains("Texureframe")) // Ű�� ������
				out_json["Keyframe"][i]["Texureframe"] = m_vecAnimation[i].iTexureframe;
			//if (out_json["Keyframe"][i].contains("ScaleX")) // Ű�� ������
				out_json["Keyframe"][i]["ScaleX"] = m_vecAnimation[i].vScale.x;
			//if (out_json["Keyframe"][i].contains("ScaleY")) // Ű�� ������
				out_json["Keyframe"][i]["ScaleY"] = m_vecAnimation[i].vScale.y;
			//if (out_json["Keyframe"][i].contains("PosX")) // Ű�� ������
				out_json["Keyframe"][i]["PosX"] = m_vecAnimation[i].vPos.x;
			//if (out_json["Keyframe"][i].contains("PosY")) // Ű�� ������
				out_json["Keyframe"][i]["PosY"] = m_vecAnimation[i].vPos.y;
			//if (out_json["Keyframe"][i].contains("Rot")) // Ű�� ������
				out_json["Keyframe"][i]["Rot"] = m_vecAnimation[i].fRot;
			//if (out_json["Keyframe"][i].contains("KeyFramePosX")) // Ű�� ������
				out_json["Keyframe"][i]["KeyFramePosX"] = m_vecAnimation[i].vKeyFramePos.x;
			//if (out_json["Keyframe"][i].contains("KeyFramePosY")) // Ű�� ������
				out_json["Keyframe"][i]["KeyFramePosY"] = m_vecAnimation[i].vKeyFramePos.y;

			//if (out_json["Keyframe"][i].contains("Alpha")) // Ű�� ������
				out_json["Keyframe"][i]["Alpha"] = m_vecAnimation[i].fAlpha;
			//if (out_json["Keyframe"][i].contains("Active")) // Ű�� ������
				out_json["Keyframe"][i]["Active"] = m_vecAnimation[i].bActive;
			//if (out_json["Keyframe"][i].contains("Appear")) // Ű�� ������
				out_json["Keyframe"][i]["Appear"] = m_vecAnimation[i].bAppear;
			//if (out_json["Keyframe"][i].contains("Trigger")) // Ű�� ������
				out_json["Keyframe"][i]["Trigger"] = m_vecAnimation[i].bTrigger;
			//if (out_json["Keyframe"][i].contains("Disappear")) // Ű�� ������
				out_json["Keyframe"][i]["Disappear"] = m_vecAnimation[i].bDisappear;
			//if (out_json["Keyframe"][i].contains("LoopSection")) // Ű�� ������
				out_json["Keyframe"][i]["LoopSection"] = m_vecAnimation[i].bLoopSection;

			//if (out_json["Keyframe"][i].contains("DistortionUI")) // Ű�� ������
				out_json["Keyframe"][i]["DistortionUI"] = m_vecAnimation[i].bDistortionUI;
			//if (out_json["Keyframe"][i].contains("Restore")) // Ű�� ������
				out_json["Keyframe"][i]["Restore"] = m_vecAnimation[i].bRestore;
			//if (out_json["Keyframe"][i].contains("TimeAcc")) // Ű�� ������
				out_json["Keyframe"][i]["TimeAcc"] = m_vecAnimation[i].fTimeAcc;
			//if (out_json["Keyframe"][i].contains("SequenceTerm")) // Ű�� ������
				out_json["Keyframe"][i]["SequenceTerm"] = m_vecAnimation[i].fSequenceTerm;
			//if (out_json["Keyframe"][i].contains("ScrollSpeedsX")) // Ű�� ������
				out_json["Keyframe"][i]["ScrollSpeedsX"] = m_vecAnimation[i].vScrollSpeeds.x;
			//if (out_json["Keyframe"][i].contains("ScrollSpeedsY")) // Ű�� ������
				out_json["Keyframe"][i]["ScrollSpeedsY"] = m_vecAnimation[i].vScrollSpeeds.y;
			//if (out_json["Keyframe"][i].contains("ScrollSpeedsZ")) // Ű�� ������
				out_json["Keyframe"][i]["ScrollSpeedsZ"] = m_vecAnimation[i].vScrollSpeeds.z;
			//if (out_json["Keyframe"][i].contains("ScalesX")) // Ű�� ������
				out_json["Keyframe"][i]["ScalesX"] = m_vecAnimation[i].vScales.x;
			//if (out_json["Keyframe"][i].contains("ScalesY")) // Ű�� ������
				out_json["Keyframe"][i]["ScalesY"] = m_vecAnimation[i].vScales.y;
			//if (out_json["Keyframe"][i].contains("ScalesZ")) // Ű�� ������
				out_json["Keyframe"][i]["ScalesZ"] = m_vecAnimation[i].vScales.z;
			//if (out_json["Keyframe"][i].contains("Distortion1X")) // Ű�� ������
				out_json["Keyframe"][i]["Distortion1X"] = m_vecAnimation[i].vDistortion1.x;
			//if (out_json["Keyframe"][i].contains("Distortion1Y")) // Ű�� ������
				out_json["Keyframe"][i]["Distortion1Y"] = m_vecAnimation[i].vDistortion1.y;
			//if (out_json["Keyframe"][i].contains("Distortion2X")) // Ű�� ������
				out_json["Keyframe"][i]["Distortion2X"] = m_vecAnimation[i].vDistortion2.x;
			//if (out_json["Keyframe"][i].contains("Distortion2Y")) // Ű�� ������
				out_json["Keyframe"][i]["Distortion2Y"] = m_vecAnimation[i].vDistortion2.y;
			//if (out_json["Keyframe"][i].contains("Distortion3X")) // Ű�� ������
				out_json["Keyframe"][i]["Distortion3X"] = m_vecAnimation[i].vDistortion3.x;
			//if (out_json["Keyframe"][i].contains("Distortion3Y")) // Ű�� ������
				out_json["Keyframe"][i]["Distortion3Y"] = m_vecAnimation[i].vDistortion3.y;
			//if (out_json["Keyframe"][i].contains("DistortionScale")) // Ű�� ������
				out_json["Keyframe"][i]["DistortionScale"] = m_vecAnimation[i].fDistortionScale;
			//if (out_json["Keyframe"][i].contains("DistortionBias")) // Ű�� ������
				out_json["Keyframe"][i]["DistortionBias"] = m_vecAnimation[i].fDistortionBias;

			//if (out_json["Keyframe"][i].contains("StopPlay")) // Ű�� ������
				out_json["Keyframe"][i]["StopPlay"] = m_vecAnimation[i].bStopPlay;
			//if (out_json["Keyframe"][i].contains("ReversePlay")) // Ű�� ������
				out_json["Keyframe"][i]["ReversePlay"] = m_vecAnimation[i].bReversePlay;
			//if (out_json["Keyframe"][i].contains("MaskChange")) // Ű�� ������
				out_json["Keyframe"][i]["MaskChange"] = m_vecAnimation[i].bMaskChange;
			//if (out_json["Keyframe"][i].contains("NoiseChange")) // Ű�� ������
				out_json["Keyframe"][i]["NoiseChange"] = m_vecAnimation[i].bNoiseChange;
			//if (out_json["Keyframe"][i].contains("MaskNum")) // Ű�� ������
				out_json["Keyframe"][i]["MaskNum"] = m_vecAnimation[i].iMaskNum;
			//if (out_json["Keyframe"][i].contains("NoiseNum")) // Ű�� ������
				out_json["Keyframe"][i]["NoiseNum"] = m_vecAnimation[i].iNoiseNum;

		}
	}

	return out_json;
}

void CUI::Change_Animation(const string& strAnimPath)
{
	if (!m_vecChangAnimation.empty()) // ���� �ִϸ��̼��� �ִ� ���
		m_vecChangAnimation.clear();

	json In_Json;
	string strFile;

	// FilePath
	strFile = strAnimPath;

	CJson_Utility::Load_Json(strFile.c_str(), In_Json);

	for (auto& item : In_Json.items())
	{
		json object = item.value();

		// "KeyframeNum" Ű�� ������ �⺻�� ���
		_bool bKeyframeNum = object.contains("KeyframeNum");
		m_tUIInfo.iKeyframeNum = bKeyframeNum ? object["KeyframeNum"] : 0;

		for (_int i = 0; i < m_tUIInfo.iKeyframeNum; ++i) // 19. Keyframe
		{
			/* Keyframe */
			m_tUIInfo.tKeyframe.fTime = object["Keyframe"][i]["Time"];
			m_tUIInfo.tKeyframe.fValue = object["Keyframe"][i]["Value"];
			m_tUIInfo.tKeyframe.fAnimSpeed = object["Keyframe"][i]["AnimSpeed"];
			m_tUIInfo.tKeyframe.iType = object["Keyframe"][i]["Type"];
			m_tUIInfo.tKeyframe.isEaseIn = object["Keyframe"][i]["EaseIn"];
			m_tUIInfo.tKeyframe.isEaseOut = object["Keyframe"][i]["EaseOut"];
			m_tUIInfo.tKeyframe.iTexureframe = object["Keyframe"][i]["Texureframe"];
			m_tUIInfo.tKeyframe.vScale.x = object["Keyframe"][i]["ScaleX"];
			m_tUIInfo.tKeyframe.vScale.y = object["Keyframe"][i]["ScaleY"];
			m_tUIInfo.tKeyframe.vPos.x = object["Keyframe"][i]["PosX"];
			m_tUIInfo.tKeyframe.vPos.y = object["Keyframe"][i]["PosY"];
			m_tUIInfo.tKeyframe.fRot = object["Keyframe"][i]["Rot"];
			m_tUIInfo.tKeyframe.vKeyFramePos.x = object["Keyframe"][i]["KeyFramePosX"];
			m_tUIInfo.tKeyframe.vKeyFramePos.y = object["Keyframe"][i]["KeyFramePosY"];

			m_tUIInfo.tKeyframe.fAlpha = object["Keyframe"][i]["Alpha"];
			m_tUIInfo.tKeyframe.bActive = object["Keyframe"][i]["Active"];
			m_tUIInfo.tKeyframe.bAppear = object["Keyframe"][i]["Appear"];
			m_tUIInfo.tKeyframe.bTrigger = object["Keyframe"][i]["Trigger"];

			if (object["Keyframe"][i].contains("Disappear")) // "Disappear" Ű�� ������
				m_tUIInfo.tKeyframe.bDisappear = object["Keyframe"][i]["Disappear"];

			if (object["Keyframe"][i].contains("LoopSection"))// "LoopSection" Ű�� ������
				m_tUIInfo.tKeyframe.bLoopSection = object["Keyframe"][i]["LoopSection"];

			if (object["Keyframe"][i].contains("StopPlay"))// "LoopSection" Ű�� ������
				m_tUIInfo.tKeyframe.bStopPlay = object["Keyframe"][i]["StopPlay"];

			if (object["Keyframe"][i].contains("ReversePlay"))// "LoopSection" Ű�� ������
				m_tUIInfo.tKeyframe.bReversePlay = object["Keyframe"][i]["ReversePlay"];

			if (object["Keyframe"][i].contains("MaskChange"))// "LoopSection" Ű�� ������
				m_tUIInfo.tKeyframe.bMaskChange = object["Keyframe"][i]["MaskChange"];

			if (object["Keyframe"][i].contains("NoiseChange"))// "LoopSection" Ű�� ������
				m_tUIInfo.tKeyframe.bNoiseChange = object["Keyframe"][i]["NoiseChange"];

			if (object["Keyframe"][i].contains("TimeAcc")) // Ű�� ������
				m_tUIInfo.tKeyframe.fTimeAcc = object["Keyframe"][i]["TimeAcc"];
			if (object["Keyframe"][i].contains("ScrollSpeedsX")) // Ű�� ������
				m_tUIInfo.tKeyframe.vScrollSpeeds.x = object["Keyframe"][i]["ScrollSpeedsX"];
			if (object["Keyframe"][i].contains("ScrollSpeedsY")) // Ű�� ������
				m_tUIInfo.tKeyframe.vScrollSpeeds.y = object["Keyframe"][i]["ScrollSpeedsY"];
			if (object["Keyframe"][i].contains("ScrollSpeedsZ")) // Ű�� ������
				m_tUIInfo.tKeyframe.vScrollSpeeds.z = object["Keyframe"][i]["ScrollSpeedsZ"];
			if (object["Keyframe"][i].contains("ScalesX")) // Ű�� ������
				m_tUIInfo.tKeyframe.vScales.x = object["Keyframe"][i]["ScalesX"];
			if (object["Keyframe"][i].contains("ScalesY")) // Ű�� ������
				m_tUIInfo.tKeyframe.vScales.y = object["Keyframe"][i]["ScalesY"];
			if (object["Keyframe"][i].contains("ScalesZ")) // Ű�� ������
				m_tUIInfo.tKeyframe.vScales.z = object["Keyframe"][i]["ScalesZ"];
			if (object["Keyframe"][i].contains("Distortion1X")) // Ű�� ������
				m_tUIInfo.tKeyframe.vDistortion1.x = object["Keyframe"][i]["Distortion1X"];
			if (object["Keyframe"][i].contains("Distortion1Y")) // Ű�� ������
				m_tUIInfo.tKeyframe.vDistortion1.y = object["Keyframe"][i]["Distortion1Y"];
			if (object["Keyframe"][i].contains("Distortion2X")) // Ű�� ������
				m_tUIInfo.tKeyframe.vDistortion2.x = object["Keyframe"][i]["Distortion2X"];
			if (object["Keyframe"][i].contains("Distortion2Y")) // Ű�� ������
				m_tUIInfo.tKeyframe.vDistortion2.y = object["Keyframe"][i]["Distortion2Y"];
			if (object["Keyframe"][i].contains("Distortion3X")) // Ű�� ������
				m_tUIInfo.tKeyframe.vDistortion3.y = object["Keyframe"][i]["Distortion3X"];
			if (object["Keyframe"][i].contains("Distortion3Y")) // Ű�� ������
				m_tUIInfo.tKeyframe.vDistortion3.y = object["Keyframe"][i]["Distortion3Y"];
			if (object["Keyframe"][i].contains("DistortionScale")) // Ű�� ������
				m_tUIInfo.tKeyframe.fDistortionScale = object["Keyframe"][i]["DistortionScale"];

			if (object["Keyframe"].contains("DistortionUI")) // Ű�� ������
				m_tUIInfo.tKeyframe.bDistortionUI = object["Distortion"][i]["DistortionUI"];
			if (object["Keyframe"].contains("MaskNum")) // Ű�� ������
				m_tUIInfo.tKeyframe.iMaskNum = object["Distortion"][i]["MaskNum"];
			if (object["Keyframe"].contains("NoiseNum")) // Ű�� ������
				m_tUIInfo.tKeyframe.iNoiseNum = object["Distortion"][i]["NoiseNum"];

			m_vecChangAnimation.push_back(m_tUIInfo.tKeyframe);
		}
	}
}

void CUI::Play_Animation(_float fTimeDelta)
{
	// ������� �˻�
	if (!m_vecAnimation.empty())
	{
		/* StopPlay */
		if (m_vecAnimation[iFrameIndex].bStopPlay == true &&
			m_bStopPlay == true)
		{
			//m_bStopPlay = true; // �ΰ��� bool������ �ΰ�, StopPlay ������ ���� �����Ͽ� stop���θ� �����Ѵ�.
		}

		// ������ ��� ����
		if (m_bPlayAnim)
		{
			// ���� �������� �ð�(������)���� ������Ű��
			m_fCurrTime += /*m_tUIInfo.tKeyframe.fAnimSpeed +*/ fTimeDelta;

			// ���� �������� �ִ� �����ӿ� ������ ���
			if (m_fCurrTime > (m_vecAnimation)[m_vecAnimation.size() - 1].fTime)
			{
				// ���� ������ �ʱ�ȭ
				//m_pAnimationTool->Get_currentTime() = 0.f;

				// @@@ Trigger �ʱ�ȭ @@@
				m_bTrigger = false;

				if (m_vecAnimation[m_iLoopAnimIndex].bLoopSection == true)
				{
					m_fCurrTime = m_vecAnimation[m_iLoopAnimIndex].fTime;
				}
				else
				{
					// �ݺ� On/Off
					if (!m_bRepetition)
						m_bPlayAnim = false;

					// �ð� �ʱ�ȭ
					m_fCurrTime = 0.f;
				}
			}
		}
	}

	if (!m_vecAnimation.empty()) // ������� üũ
	{
		//if (m_pAnimationTool->Get_FramePlaying()) // ��� ��ư�� ������ ��츸
		//{
		if (m_fCurrTime >= 0.f &&
			m_fCurrTime <= m_vecAnimation.back().fTime)
		{
			//m_eAnimationInfo = m_vecAnimation[(int)m_iFrameCount].front();
			_uint iSize = (_uint)m_vecAnimation.size() - (_uint)1;
			for (_uint i = iSize; i >= 0; i--)
			{
				if (m_vecAnimation[i].fTime <= m_fCurrTime) //	error : i�� �����Ⱚ�� �Ǹ鼭 iSize���� ���Ե��� �ʰ�, ������ ���� => �ð����� �̻��ϰ� ���� ������ Ÿ���ʾ���. break�� Ÿ�� ������ �ʾƼ� -���� �������� ����.
				{
					iFrameIndex = i;
					break;
				}
				// bug ���� ó�� ������� : (0��° Ű�������� �ð����� ���� �ð��� ���� ��� ���� �ε����� 0������ ����������. (ex : �ִϸ��̼��� ���� �ð� ���Ŀ� �����ϰ� �������� ���)
				if (m_vecAnimation[0].fTime > m_fCurrTime)
				{
					iFrameIndex = 0;
					break;
				}
			}

			// Constant
			//m_fSizeX = (*m_vecAnimation[*m_eKeyframe])[iFrameIndex].vScale.x;
			//m_fSizeY = (*m_vecAnimation[*m_eKeyframe])[iFrameIndex].vScale.y;

			//m_fX = m_fSizeX * 0.5f; // ������ġ 
			//m_fY = m_fSizeY * 0.5f;

			//m_pTransformComp->Set_Pos({ (*m_vecAnimation[*m_eKeyframe])[iFrameIndex].vPos.x,
			//							(*m_vecAnimation[*m_eKeyframe])[iFrameIndex].vPos.y,
			//							0.f });	// �̹��� ��ġ

			//m_pTransformComp->Set_Scale({ m_fSizeX, m_fSizeY, 1.f });	// �̹��� ũ��


			// Linear
			if (iFrameIndex + 1U < m_vecAnimation.size() &&
				m_fCurrTime != m_vecAnimation[iFrameIndex].fTime)
			{
				// Ű �����Ӱ� �ð� ��ȭ��
				fFrameTimeDelta = m_vecAnimation[iFrameIndex + 1U].fTime - m_vecAnimation[iFrameIndex].fTime;
				// ���� Ű �����ӽð����� ���� �ð� ��ȭ��
				fCurFrameTimeDelta = (m_fCurrTime - m_vecAnimation[iFrameIndex].fTime);

				if (m_tUIInfo.bWorld == true)
				{
					fSizeX_Delta = m_vecAnimation[iFrameIndex + 1U].vWorld_Scale.x - m_vecAnimation[iFrameIndex].vWorld_Scale.x;
					fSizeX_Delta *= fCurFrameTimeDelta / fFrameTimeDelta;
					fSizeY_Delta = m_vecAnimation[iFrameIndex + 1U].vWorld_Scale.y - m_vecAnimation[iFrameIndex].vWorld_Scale.y;
					fSizeY_Delta *= fCurFrameTimeDelta / fFrameTimeDelta;
					fSizeZ_Delta = m_vecAnimation[iFrameIndex + 1U].vWorld_Scale.z - m_vecAnimation[iFrameIndex].vWorld_Scale.z;
					fSizeZ_Delta *= fCurFrameTimeDelta / fFrameTimeDelta;

					fRotX_Delta = m_vecAnimation[iFrameIndex + 1U].vWorld_Rot.x - m_vecAnimation[iFrameIndex].vWorld_Rot.x;
					fRotX_Delta *= fCurFrameTimeDelta / fFrameTimeDelta;
					fRotY_Delta = m_vecAnimation[iFrameIndex + 1U].vWorld_Rot.y - m_vecAnimation[iFrameIndex].vWorld_Rot.y;
					fRotY_Delta *= fCurFrameTimeDelta / fFrameTimeDelta;
					fRotZ_Delta = m_vecAnimation[iFrameIndex + 1U].vWorld_Rot.z - m_vecAnimation[iFrameIndex].vWorld_Rot.z;
					fRotZ_Delta *= fCurFrameTimeDelta / fFrameTimeDelta;

					fPosX_Delta = m_vecAnimation[iFrameIndex + 1U].vWorld_Pos.x - m_vecAnimation[iFrameIndex].vWorld_Pos.x;
					fPosX_Delta *= fCurFrameTimeDelta / fFrameTimeDelta;
					fPosY_Delta = m_vecAnimation[iFrameIndex + 1U].vWorld_Pos.y - m_vecAnimation[iFrameIndex].vWorld_Pos.y;
					fPosY_Delta *= fCurFrameTimeDelta / fFrameTimeDelta;
					fPosZ_Delta = m_vecAnimation[iFrameIndex + 1U].vWorld_Pos.z - m_vecAnimation[iFrameIndex].vWorld_Pos.z;
					fPosZ_Delta *= fCurFrameTimeDelta / fFrameTimeDelta;
				}
				else
				{
					fSizeX_Delta = m_vecAnimation[iFrameIndex + 1U].vScale.x - m_vecAnimation[iFrameIndex].vScale.x;
					fSizeX_Delta *= fCurFrameTimeDelta / fFrameTimeDelta;
					fSizeY_Delta = m_vecAnimation[iFrameIndex + 1U].vScale.y - m_vecAnimation[iFrameIndex].vScale.y;
					fSizeY_Delta *= fCurFrameTimeDelta / fFrameTimeDelta;

					fRotZ_Delta = m_vecAnimation[iFrameIndex + 1U].fRot - m_vecAnimation[iFrameIndex].fRot;
					fRotZ_Delta *= fCurFrameTimeDelta / fFrameTimeDelta;

					fPosX_Delta = m_vecAnimation[iFrameIndex + 1U].vPos.x - m_vecAnimation[iFrameIndex].vPos.x;
					fPosX_Delta *= fCurFrameTimeDelta / fFrameTimeDelta;
					fPosY_Delta = m_vecAnimation[iFrameIndex + 1U].vPos.y - m_vecAnimation[iFrameIndex].vPos.y;
					fPosY_Delta *= fCurFrameTimeDelta / fFrameTimeDelta;
				}

				fAlpha_Delta = m_vecAnimation[iFrameIndex + 1U].fAlpha - m_vecAnimation[iFrameIndex].fAlpha;
				fAlpha_Delta *= fCurFrameTimeDelta / fFrameTimeDelta;

				/* Distortion */
				/* TimeAcc */
				fTimeAcc_Delta = m_vecAnimation[iFrameIndex + 1U].fTimeAcc - m_vecAnimation[iFrameIndex].fTimeAcc;
				fTimeAcc_Delta *= fCurFrameTimeDelta / fFrameTimeDelta;
				m_tUIInfo.tKeyframe.fTimeAcc = m_vecAnimation[iFrameIndex].fTimeAcc + fTimeAcc_Delta;
				m_tUIInfo.fTimeAcc = m_tUIInfo.tKeyframe.fTimeAcc;
				/* SequenceTerm */
				fSequenceTerm_Delta = m_vecAnimation[iFrameIndex + 1U].fSequenceTerm - m_vecAnimation[iFrameIndex].fSequenceTerm;
				fSequenceTerm_Delta *= fCurFrameTimeDelta / fFrameTimeDelta;
				m_tUIInfo.tKeyframe.fSequenceTerm = m_vecAnimation[iFrameIndex].fSequenceTerm + fSequenceTerm_Delta;
				m_tUIInfo.fSequenceTerm = m_tUIInfo.tKeyframe.fSequenceTerm;
				/* ScrollSpeeds */
				vScrollSpeeds_Delta = m_vecAnimation[iFrameIndex + 1U].vScrollSpeeds - m_vecAnimation[iFrameIndex].vScrollSpeeds;
				vScrollSpeeds_Delta *= fCurFrameTimeDelta / fFrameTimeDelta;
				m_tUIInfo.tKeyframe.vScrollSpeeds = m_vecAnimation[iFrameIndex].vScrollSpeeds + vScrollSpeeds_Delta;
				m_tUIInfo.vScrollSpeeds = m_tUIInfo.tKeyframe.vScrollSpeeds;
				/* Scales */
				vScales_Delta = m_vecAnimation[iFrameIndex + 1U].vScales - m_vecAnimation[iFrameIndex].vScales;
				vScales_Delta *= fCurFrameTimeDelta / fFrameTimeDelta;
				m_tUIInfo.tKeyframe.vScales = m_vecAnimation[iFrameIndex].vScales + vScales_Delta;
				m_tUIInfo.vScales = m_tUIInfo.tKeyframe.vScales;
				/* Distortion1X */
				vDistortion1_Delta.x = m_vecAnimation[iFrameIndex + 1U].vDistortion1.x - m_vecAnimation[iFrameIndex].vDistortion1.x;
				vDistortion1_Delta.x *= fCurFrameTimeDelta / fFrameTimeDelta;
				m_tUIInfo.tKeyframe.vDistortion1.x = m_vecAnimation[iFrameIndex].vDistortion1.x + vDistortion1_Delta.x;
				m_tUIInfo.vDistortion1.x = m_tUIInfo.tKeyframe.vDistortion1.x;
				/* Distortion1Y */
				vDistortion1_Delta.y = m_vecAnimation[iFrameIndex + 1U].vDistortion1.y - m_vecAnimation[iFrameIndex].vDistortion1.y;
				vDistortion1_Delta.y *= fCurFrameTimeDelta / fFrameTimeDelta;
				m_tUIInfo.tKeyframe.vDistortion1.y = m_vecAnimation[iFrameIndex].vDistortion1.y + vDistortion1_Delta.y;
				m_tUIInfo.vDistortion1.y = m_tUIInfo.tKeyframe.vDistortion1.y;
				/* Distortion2X */
				vDistortion2_Delta.x = m_vecAnimation[iFrameIndex + 1U].vDistortion2.x - m_vecAnimation[iFrameIndex].vDistortion2.x;
				vDistortion2_Delta.x *= fCurFrameTimeDelta / fFrameTimeDelta;
				m_tUIInfo.vDistortion2.x = m_vecAnimation[iFrameIndex].vDistortion2.x + vDistortion2_Delta.x;
				m_tUIInfo.tKeyframe.vDistortion2.x = m_tUIInfo.vDistortion2.x;
				/* Distortion2Y */
				vDistortion2_Delta.y = m_vecAnimation[iFrameIndex + 1U].vDistortion2.y - m_vecAnimation[iFrameIndex].vDistortion2.y;
				vDistortion2_Delta.y *= fCurFrameTimeDelta / fFrameTimeDelta;
				m_tUIInfo.tKeyframe.vDistortion2.y = m_vecAnimation[iFrameIndex].vDistortion2.y + vDistortion2_Delta.y;
				m_tUIInfo.vDistortion2.y = m_tUIInfo.tKeyframe.vDistortion2.y;
				/* Distortion3X */
				vDistortion3_Delta.x = m_vecAnimation[iFrameIndex + 1U].vDistortion3.x - m_vecAnimation[iFrameIndex].vDistortion3.x;
				vDistortion3_Delta.x *= fCurFrameTimeDelta / fFrameTimeDelta;
				m_tUIInfo.tKeyframe.vDistortion3.x = m_vecAnimation[iFrameIndex].vDistortion3.x + vDistortion3_Delta.x;
				m_tUIInfo.vDistortion3.x = m_tUIInfo.tKeyframe.vDistortion3.x;
				/* Distortion3Y */
				vDistortion3_Delta.y = m_vecAnimation[iFrameIndex + 1U].vDistortion3.y - m_vecAnimation[iFrameIndex].vDistortion3.y;
				vDistortion3_Delta.y *= fCurFrameTimeDelta / fFrameTimeDelta;
				m_tUIInfo.tKeyframe.vDistortion3.y = m_vecAnimation[iFrameIndex].vDistortion3.y + vDistortion3_Delta.y;
				m_tUIInfo.vDistortion3.y = m_tUIInfo.tKeyframe.vDistortion3.y;
				/* Scale */
				fDistortionScale_Delta = m_vecAnimation[iFrameIndex + 1U].fDistortionScale - m_vecAnimation[iFrameIndex].fDistortionScale;
				fDistortionScale_Delta *= fCurFrameTimeDelta / fFrameTimeDelta;
				m_tUIInfo.tKeyframe.fDistortionScale = m_vecAnimation[iFrameIndex].fDistortionScale + fDistortionScale_Delta;
				m_tUIInfo.fDistortionScale = m_tUIInfo.tKeyframe.fDistortionScale;
				/* Bias */
				fDistortionBias_Delta = m_vecAnimation[iFrameIndex + 1U].fDistortionBias - m_vecAnimation[iFrameIndex].fDistortionBias;
				fDistortionBias_Delta *= fCurFrameTimeDelta / fFrameTimeDelta;
				m_tUIInfo.tKeyframe.fDistortionBias = m_vecAnimation[iFrameIndex].fDistortionBias + fDistortionBias_Delta;
				m_tUIInfo.fDistortionBias = m_tUIInfo.tKeyframe.fDistortionBias;

				if (m_tUIInfo.bWorld == true)
				{
					/* ������ ���� */
					m_pTransformCom->Set_Position({ m_vecAnimation[iFrameIndex].vWorld_Pos.x + fPosX_Delta,
													m_vecAnimation[iFrameIndex].vWorld_Pos.y + fPosY_Delta,
													m_vecAnimation[iFrameIndex].vWorld_Pos.z + fPosZ_Delta });	// �̹��� ��ġ

					/* ������ ���� */
					m_pTransformCom->Set_Scaling(m_vecAnimation[iFrameIndex].vWorld_Scale.x + fSizeX_Delta, 	// �̹��� ũ��
												 m_vecAnimation[iFrameIndex].vWorld_Scale.y + fSizeY_Delta,
												 m_vecAnimation[iFrameIndex].vWorld_Scale.z + fSizeZ_Delta);

					/* �����̼� ���� */ // ������ �����̼����� ����ż� X, Y�� ������ �ȵ�
					//m_pTransformCom->Rotation({ 1.0f, 0.0f, 0.0f, 0.0f }, m_vecAnimation[iFrameIndex].vWorld_Rot.x + fRotX_Delta);// �̹��� ȸ��
					//m_pTransformCom->Rotation({ 0.0f, 1.0f, 0.0f, 0.0f }, m_vecAnimation[iFrameIndex].vWorld_Rot.y + fRotY_Delta);// �̹��� ȸ��
					//m_pTransformCom->Rotation({ 0.0f, 0.0f, 1.0f, 0.0f }, m_vecAnimation[iFrameIndex].vWorld_Rot.z + fRotZ_Delta);// �̹��� ȸ��
					
					/* �����̼� ���� */
					m_pTransformCom->Rotation_Quaternion({ 
														  m_vecAnimation[iFrameIndex].vWorld_Rot.x + fRotX_Delta,
														  m_vecAnimation[iFrameIndex].vWorld_Rot.y + fRotY_Delta,
														  m_vecAnimation[iFrameIndex].vWorld_Rot.z + fRotZ_Delta 
														});
				}										 
				else
				{
					/* ������ ���� */
					m_pTransformCom->Set_Position({ m_vecAnimation[iFrameIndex].vPos.x + fPosX_Delta,
												m_vecAnimation[iFrameIndex].vPos.y + fPosY_Delta,
												m_tUIInfo.fPositionZ });	// �̹��� ��ġ

					/* ������ ���� */
					m_pTransformCom->Set_Scaling(m_vecAnimation[iFrameIndex].vScale.x + fSizeX_Delta, 	// �̹��� ũ��
						m_vecAnimation[iFrameIndex].vScale.y + fSizeY_Delta,
						1.f);

					/* �����̼� ���� */
					m_pTransformCom->Rotation({ 0.0f, 0.0f, 1.0f, 0.0f }, m_vecAnimation[iFrameIndex].fRot + fRotZ_Delta);// �̹��� ȸ��
				}

				/* ���� ���� */
				//m_tUIInfo.fAlpha = fAlpha_Delta;

				/* Disappear */
				if (m_vecAnimation[iFrameIndex].bDisappear == true)
				{
					m_bDisappear = true;
				}
				/* m_bRenderOut */
				m_bRenderOut = m_vecAnimation[iFrameIndex].bDisappear;

				/* LoopSection */
				if (m_vecAnimation[iFrameIndex].bLoopSection == true)
				{
					m_iLoopAnimIndex = iFrameIndex;
				}
				/* Trigger */
				if (m_vecAnimation[iFrameIndex].bTrigger == true)
				{
					m_bTrigger = true;
				}
				/* Restore */
				if (m_vecAnimation[iFrameIndex].bRestore == true)
				{
					m_bRestore = true;
				}
				/* ReversePlay */
				if (m_vecAnimation[iFrameIndex].bReversePlay == true)
				{
					m_bReversePlay = true;
				}
				/* StopPlay */
				if (m_vecAnimation[iFrameIndex].bStopPlay == true)
				{
					//m_bStopPlay = true; // �ΰ��� bool������ �ΰ�, StopPlay ������ ���� �����Ͽ� stop���θ� �����Ѵ�.
				}

				/* MaskChange */
				if (m_vecAnimation[iFrameIndex].bMaskChange == true)
				{
					m_tUIInfo.iMaskNum = m_vecAnimation[iFrameIndex].iMaskNum;
				}
				
				/* NoiseChange */
				if (m_vecAnimation[iFrameIndex].bNoiseChange == true)
				{
					m_tUIInfo.iNoiseNum = m_vecAnimation[iFrameIndex].iNoiseNum;
				}

				/* �ؽ�ó */
				m_iTextureNum = m_vecAnimation[iFrameIndex].iTexureframe;

			}
			else
			{
			if (m_tUIInfo.bWorld == true)
			{
				/* ������ ���� */
				m_pTransformCom->Set_Position({ m_vecAnimation[iFrameIndex].vWorld_Pos.x,
												m_vecAnimation[iFrameIndex].vWorld_Pos.y,
												m_vecAnimation[iFrameIndex].vWorld_Pos.z });	// �̹��� ��ġ

				/* ������ ���� */
				m_pTransformCom->Set_Scaling(m_vecAnimation[iFrameIndex].vWorld_Scale.x, 	// �̹��� ũ��
											 m_vecAnimation[iFrameIndex].vWorld_Scale.y,
											 m_vecAnimation[iFrameIndex].vWorld_Scale.z);

				/* �����̼� ���� */
				m_pTransformCom->Rotation({ 1.0f, 0.0f, 0.0f, 0.0f }, m_vecAnimation[iFrameIndex].vWorld_Rot.x);// �̹��� ȸ��
				m_pTransformCom->Rotation({ 0.0f, 1.0f, 0.0f, 0.0f }, m_vecAnimation[iFrameIndex].vWorld_Rot.y);// �̹��� ȸ��
				m_pTransformCom->Rotation({ 0.0f, 0.0f, 1.0f, 0.0f }, m_vecAnimation[iFrameIndex].vWorld_Rot.z);// �̹��� ȸ��
			}
			else
			{
				m_pTransformCom->Set_Scaling(m_vecAnimation[iFrameIndex].vScale.x, 	// �̹��� ũ��
					m_vecAnimation[iFrameIndex].vScale.y,
					1.f);

				m_pTransformCom->Set_Position({ m_vecAnimation[iFrameIndex].vPos.x,
												m_vecAnimation[iFrameIndex].vPos.y,
												m_tUIInfo.fPositionZ });	// �̹��� ��ġ

				/* �����̼� ���� */
				m_pTransformCom->Rotation({ 0.0f, 0.0f, 1.0f, 0.0f }, m_vecAnimation[iFrameIndex].fRot);// �̹��� ȸ��
			}

				/* ���� ���� */
				//m_tUIInfo.fAlpha = m_vecAnimation[iFrameIndex].fAlpha;

								/* Distortion */
				m_tUIInfo.tKeyframe.fTimeAcc = m_vecAnimation[iFrameIndex].fTimeAcc;

				m_tUIInfo.tKeyframe.fSequenceTerm = m_vecAnimation[iFrameIndex].fSequenceTerm;

				m_tUIInfo.tKeyframe.vScrollSpeeds = m_vecAnimation[iFrameIndex].vScrollSpeeds;

				m_tUIInfo.tKeyframe.vScales = m_vecAnimation[iFrameIndex].vScales;

				m_tUIInfo.tKeyframe.vDistortion1.x = m_vecAnimation[iFrameIndex].vDistortion1.x;

				m_tUIInfo.tKeyframe.vDistortion1.y = m_vecAnimation[iFrameIndex].vDistortion1.y;

				m_tUIInfo.tKeyframe.vDistortion2.x = m_vecAnimation[iFrameIndex].vDistortion2.x;

				m_tUIInfo.tKeyframe.vDistortion2.y = m_vecAnimation[iFrameIndex].vDistortion2.y;

				m_tUIInfo.tKeyframe.vDistortion3.x = m_vecAnimation[iFrameIndex].vDistortion3.x;

				m_tUIInfo.tKeyframe.vDistortion3.y = m_vecAnimation[iFrameIndex].vDistortion3.y;

				m_tUIInfo.tKeyframe.fDistortionScale = m_vecAnimation[iFrameIndex].fDistortionScale;

				m_tUIInfo.tKeyframe.fDistortionBias = m_vecAnimation[iFrameIndex].fDistortionBias;


				/* Disappear */
				if (m_vecAnimation[iFrameIndex].bDisappear == true)
				{
					m_bDisappear = true;
				}

				/* LoopSection */
				if (m_vecAnimation[iFrameIndex].bLoopSection == true)
				{
					m_iLoopAnimIndex = iFrameIndex;
				}

				/* Trigger */
				if (m_vecAnimation[iFrameIndex].bTrigger == true)
				{
					m_bTrigger = true;
				}

				m_iTextureNum = m_vecAnimation[iFrameIndex].iTexureframe;
			}
		}
	}
}

void CUI::Set_AnimationKeyframe(UIKEYFRAME tKeyframe)
{
	
}

_bool CUI::Alpha_Minus(_float fTimeDelta)
{
	if (m_fAlpha > 0.f)
	{
		m_fAlpha -= m_fAlphaSpeed * fTimeDelta;
	}
	else
	{
		m_fAlpha = 0.f;
		return true;
	}

	return false;
}

_bool CUI::Alpha_Plus(_float fTimeDelta)
{
	if (m_fAlpha < 1.f)
	{
		m_fAlpha += m_fAlphaSpeed * fTimeDelta;
	}
	else
	{
		m_fAlpha = 1.f; // ���� �ʱ�ȭ
		return false;	// UI Off
	}
	return true;

}

_bool CUI::Alpha_Plus_Control(_float fTimeDelta, _float fAlpha)
{
	if (m_fAlpha < fAlpha)
	{
		m_fAlpha += m_fAlphaSpeed * fTimeDelta;
	}
	else
	{
		m_fAlpha = fAlpha; // ���� �ʱ�ȭ
		return false;	// UI Off
	}
	return true;

}

void CUI::Compute_CamDistance()
{
	_vector		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector		vCamPosition = XMLoadFloat4(&m_pGameInstance->Get_CamPosition());

	m_fCamDistance = XMVectorGetX(XMVector3Length(vPosition - vCamPosition));
}

void CUI::LifeTime_LevelUp(_float fTimeDelta)
{

	/* �ִϸ��̼��� ���������� ���� �Ǵ����ش�. */
	if (true == m_pData_Manager->Get_ShowLevelBox() && m_bPlayAnim == false)
	{
		m_bActive = false;
		m_bEventOn = true;
		m_pData_Manager->Set_ShowLevelBox(false);
	}
	/* ���� ������ ���� ��� */
	else if (m_pData_Manager->Get_ShowLevelBox()/*m_pData_Manager->Limit_EXP()*/)
	{
		//m_fAlpha = 0.f;
		//m_fTime = GetTickCount64();
		//m_bReset = false;
		m_bPlayAnim = true;		// �ִϸ��̼� ���
		m_bRepetition = false;	// �ݺ� ���
		m_bActive = true;		// Ȱ��ȭ
	}

	//if (m_fTime + m_fLifeTime < GetTickCount64())
	//{
	//	m_bEventOn = true;
	//}

	if (m_bEventOn)
	{
		m_fAlpha += fTimeDelta;
	}



	if (m_fAlpha >= 1.f)
	{
		m_bEventOn = false;
		//m_pData_Manager->Set_ShowLevelBox(false);
		//m_bActive = false;
		//m_bEventOn = false;
		//m_bReset = true;
	}
}

void CUI::Free()
{
	__super::Free();

	if (m_pData_Manager)
		Safe_Release(m_pData_Manager);

	if (m_pUIManager)
		Safe_Release(m_pUIManager);

	if (m_pVIBufferCom)
		Safe_Release(m_pVIBufferCom);

	if (m_pShaderCom)
		Safe_Release(m_pShaderCom);

	//Safe_Release(m_pTextureCom);
	if (m_pMapTextureCom)
		Safe_Release(m_pMapTextureCom);
	
}
