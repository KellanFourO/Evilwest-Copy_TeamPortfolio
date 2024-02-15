#include "stdafx.h"
#include "Window_MapTool.h"

CWindow_MapTool::CWindow_MapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CImgui_Window(pDevice, pContext)
{
}


HRESULT CWindow_MapTool::Initialize()
{
	//! ����� Ư���� ��ɾ���. ���� �ʿ��� �� ���Ƽ� ����.
	if(FAILED(__super::Initialize()))
		return E_FAIL;


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
	
	if (ImGui::BeginTabBar(u8"�׸�", tab_bar_flags))
	{
		if (ImGui::BeginTabItem(u8"��Ʈ�θ�"))
		{
			Theme_IntroTabFunction();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(u8"ȭ���"))
		{
			Theme_VolcanoTabFunction();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(u8"�����"))
		{
			Theme_SnowMountainTabFunction();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(u8"������"))
		{
			Theme_BossTabFunction();

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
	
	

	__super::End();
}

void CWindow_MapTool::Render()
{

}

void CWindow_MapTool::Theme_IntroTabFunction()
{
	
}

void CWindow_MapTool::Theme_VolcanoTabFunction()
{
	
}

void CWindow_MapTool::Theme_SnowMountainTabFunction()
{

}

void CWindow_MapTool::Theme_BossTabFunction()
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