#include "..\Public\UI.h"
#include "GameInstance.h"

CUI::CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CUI::CUI(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI::Initialize(void* pArg)
{
	/* Info */
	m_tUIInfo = *(UI_DESC*)pArg;

#pragma region Transform
	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext, 0.f, 0.f);
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	if (nullptr != Find_Component(g_pTransformTag))
		return E_FAIL;

	m_Components.emplace(g_pTransformTag, m_pTransformCom);
	Safe_AddRef(m_pTransformCom);
#pragma endregion End

#pragma region 2D
	m_pTransformCom->Set_Scaling(m_tUIInfo.fScaleX, m_tUIInfo.fScaleY, 1.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tUIInfo.fPositionX - (_float)g_iWinSizeX * 0.5f, -m_tUIInfo.fPositionY + (_float)g_iWinSizeY * 0.5f, m_tUIInfo.fPositionZ, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f));

	SetUp_ScreenPosRect(m_tUIInfo.fPositionX, m_tUIInfo.fPositionY, m_tUIInfo.fScaleX, m_tUIInfo.fScaleY);
#pragma endregion End

	return S_OK;
}

void CUI::Priority_Tick(_float fTimeDelta)
{

}

void CUI::Tick(_float fTimeDelta)
{
	switch (m_eState)
	{
	case UISTATE::APPEAR:
		UI_AppearTick(fTimeDelta);
		break;
	case UISTATE::DISAPPEAR:
		UI_DisappearTick(fTimeDelta);
		break;
	}
}

void CUI::UI_AppearTick(_float fTimeDelta)
{
	m_eState = UISTATE::TICK;
}

void CUI::UI_DisappearTick(_float fTimeDelta)
{
	m_eState = UISTATE::READY;
}

void CUI::Late_Tick(_float fTimeDelta)
{
}

HRESULT CUI::Render()
{
	return S_OK;
}

HRESULT CUI::Ready_Components()
{
	return S_OK;
}

HRESULT CUI::Bind_ShaderResources()
{
	return S_OK;
}

HRESULT CUI::SetUp_ScreenPosRect(_float fPosX, _float fPosY, _float fSizeX, _float fSizeY)
{
	m_ScreenPosRect.left = static_cast<LONG>(m_tUIInfo.fPositionX - (m_tUIInfo.fScaleX * 0.5f));
	m_ScreenPosRect.top = static_cast<LONG>(m_tUIInfo.fPositionY - (m_tUIInfo.fScaleY * 0.5f));
	m_ScreenPosRect.right = static_cast<LONG>(m_tUIInfo.fPositionX + (m_tUIInfo.fScaleX * 0.5f));
	m_ScreenPosRect.bottom = static_cast<LONG>(m_tUIInfo.fPositionY + (m_tUIInfo.fScaleY * 0.5f));

	return S_OK;
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

HRESULT CUI::SetUp_Transform(_float fPosX, _float fPosY, _float fSizeX, _float fSizeY)
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_tUIInfo.fPositionX = fPosX;
	m_tUIInfo.fPositionY = fPosY;
	m_tUIInfo.fScaleX = fSizeX;
	m_tUIInfo.fScaleY = fSizeY;

	m_pTransformCom->Set_Scaling(m_tUIInfo.fScaleX, m_tUIInfo.fScaleY, 1.f);

	// 위치 이동
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tUIInfo.fPositionX - (_float)g_iWinSizeX * 0.5f, -m_tUIInfo.fPositionY + (_float)g_iWinSizeY * 0.5f, m_tUIInfo.fPositionZ, 1.f));

	// View Matrix 및 Projection Matrix 설정
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

/* @@@보류@@@ */
void CUI::Load_UIData(const char* _FilePath)
{
	_int		iPathNum = 0;
	string		strFileName;
	string		strFilePath;

	json json_in;

	CJson_Utility::Load_Json(_FilePath, json_in);

	for (auto& item : json_in.items())
	{
		json object = item.value();

		CUI::UI_DESC tUI_Info;

		tUI_Info.fPositionX = object["PostionX"];
		tUI_Info.fPositionY = object["PostionY"];
		tUI_Info.fScaleX = object["SizeX"];
		tUI_Info.fScaleY = object["SizeY"];
		tUI_Info.strProtoTag = object["ProtoTag"];
		tUI_Info.strFilePath = object["FilePath"];
	}
}

void CUI::Free()
{
}
