#include "stdafx.h"
#include "Window_MapTool.h"
#include "Imgui_Manager.h"
#include "GameInstance.h"

#include "Environment_Instance.h"
#include "Environment_Object.h"

CWindow_MapTool::CWindow_MapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CImgui_Window(pDevice, pContext)
{
}


HRESULT CWindow_MapTool::Initialize()
{
	//! ����� Ư���� ��ɾ���. ���� �ʿ��� �� ���Ƽ� ����.

	FAILED_CHECK(__super::Initialize());


	//! Loader���� Ǫ�ù� �س��� Imgui_Manager�� ���±� ���͸� �޾ƿ���.
	FAILED_CHECK(Ready_ModelTags());

	return S_OK;
}

void CWindow_MapTool::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	__super::Begin();

	//TODO ImGuiTabBarFlags_
	//!	ImGuiTabBarFlags_None = 0,
	
	//!	ImGuiTabBarFlags_Reorderable = 1 << 0,   // �������� ���� ��� ������ ������ �� ���� + �� ���� ��� ���� �߰���
	//!	ImGuiTabBarFlags_AutoSelectNewTabs = 1 << 1,   // �� ���� ��Ÿ���� �ڵ����� ����
	//!	ImGuiTabBarFlags_TabListPopupButton = 1 << 2,   // �� ��� �˾��� ������ ��ư�� ��Ȱ��ȭ�ϼ���.
	//!	ImGuiTabBarFlags_NoCloseWithMiddleMouseButton = 1 << 3,   // ���콺 ��� ��ư�� ����Ͽ� �� �ݱ� ����(p_open != NULL�� �����)�� ��Ȱ��ȭ�մϴ�. if (IsItemHovered() && IsMouseClicked(2)) *p_open = false�� ����Ͽ� ����� ������ �� ������ ������ �� �ֽ��ϴ�.
	//!	ImGuiTabBarFlags_NoTabListScrollingButtons = 1 << 4,   //��ũ�� ��ư ��Ȱ��ȭ(���� ��å�� ImGuiTabBarFlags_FittingPolicyScroll�� ��� ����)
	//!	ImGuiTabBarFlags_NoTooltip = 1 << 5,   //���� ����ų �� ���� ���� ��Ȱ��ȭ
	//!	ImGuiTabBarFlags_FittingPolicyResizeDown = 1 << 6,   // ���� ���� �ʴ� ��� �� ũ�� ����
	//!	ImGuiTabBarFlags_FittingPolicyScroll = 1 << 7,   // ���� ���� ���� �� ��ũ�� ��ư �߰�
	//!	ImGuiTabBarFlags_FittingPolicyMask_ = ImGuiTabBarFlags_FittingPolicyResizeDown | ImGuiTabBarFlags_FittingPolicyScroll,
	//!	ImGuiTabBarFlags_FittingPolicyDefault_ = ImGuiTabBarFlags_FittingPolicyResizeDown,

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyMask_;
	
	if (ImGui::BeginTabBar(u8"������Ʈ Ÿ��", tab_bar_flags))
	{
		if (ImGui::BeginTabItem(u8"�׶���"))
		{
			GroundTab_Function();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(u8"��ȣ�ۿ�"))
		{
			InteractTab_Function();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(u8"ȯ��"))
		{
			EnvironmentTab_Function();

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}


	__super::End();
}

void CWindow_MapTool::Render()
{

}

HRESULT CWindow_MapTool::Ready_ModelTags()
{
	//! �ִ��±�
	vector<wstring> vecAnimTags = *CImgui_Manager::GetInstance()->Get_Anim_E_ModelTag();
	_int iAnimTagVectorSize = (_int)vecAnimTags.size();
	
	m_vecAnimModelTag.reserve(iAnimTagVectorSize);

	for (_int i = 0; i < iAnimTagVectorSize; ++i)
	{
		string strAnimTag = {};
		m_pGameInstance->WString_To_String(vecAnimTags[i], strAnimTag);
		
		m_vecAnimModelTag.push_back(strAnimTag);

	}

	//!��ִ� �±�
	vector<wstring> vecNonAnimTags = *CImgui_Manager::GetInstance()->Get_NonAnim_E_ModelTag();
	_int iNonAnimTagVectorSize = (_int)vecNonAnimTags.size();

	m_vecNonAnimModelTag.reserve(iNonAnimTagVectorSize);

	for (_int i = 0; i < iNonAnimTagVectorSize; ++i)
	{
		string strNonAnimTag = {};
		m_pGameInstance->WString_To_String(vecNonAnimTags[i], strNonAnimTag);

		m_vecNonAnimModelTag.push_back(strNonAnimTag);
	}

	
	return S_OK;
}

void CWindow_MapTool::GroundTab_Function()
{
}

void CWindow_MapTool::InteractTab_Function()
{
}

void CWindow_MapTool::EnvironmentTab_Function()
{
	
}




CWindow_MapTool* CWindow_MapTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWindow_MapTool* pInstance = new CWindow_MapTool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CWindow_MapTool");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CWindow_MapTool::Free()
{
	__super::Free();
}