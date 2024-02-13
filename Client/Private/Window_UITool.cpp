#include "stdafx.h"
#include "Window_UITool.h"

CWindow_UITool::CWindow_UITool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CImgui_Window(pDevice, pContext)
{
}


HRESULT CWindow_UITool::Initialize()
{
	//! ����� Ư���� ��ɾ���. ���� �ʿ��� �� ���Ƽ� ����.
	if (FAILED(__super::Initialize()))
		return E_FAIL;


	return S_OK;
}

void CWindow_UITool::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	__super::Begin();

	UI_List(fTimeDelta);
	UI_ToolTip(fTimeDelta);

	ImGui::Text("UI_Test");


	__super::End();
}

void CWindow_UITool::Render()
{

}

void CWindow_UITool::UI_List(_float fTimeDelta)
{
	/* Test Value */
	tUI_Info.strName = "Test UI List";
	tUI_Info.iNum = 1;
	tUI_Info.fNum = 1.5f;

	// ������ ������� ǥ��
	if (ImGui::TreeNode(tUI_Info.strName.c_str())) {
		ImGui::Text(u8"��1 : %d", tUI_Info.iNum);
		ImGui::Text(u8"��2 : %.2f", tUI_Info.fNum);

		// ��ư
		if (ImGui::Button(u8"��ư")) 
		{

		}

		ImGui::TreePop();
	}
}

void CWindow_UITool::UI_ToolTip(_float fTimeDelta)
{
	/* �׽�Ʈ �� */
	_int	iTestNum = 1;
	_float	fTestValue1 = 33.f;
	_float	fTestValue2 = 44.f;

	/* ���콺 ������ ��ġ */
	_float	fTestX = 50.f;
	_float	fTestY = 500.f;
	
	/* ���콺 ������ ���� */
	_float	fHoverRangeX = 200.f;
	_float	fHoverRangeY = 200.f;

	/* ���콺 ������ ������ ����â�� ũ�� */
	_float	fScreenPosX = 20.f;
	_float	fScreenPosY = 40.f;

	// ���콺�� �ش� ��ġ ���� �ִ� ��쿡�� ���� ǥ�� (���콺 ����)
	if (ImGui::IsMouseHoveringRect(
		ImVec2(fTestX - fHoverRangeX, fTestY - fHoverRangeY),
		ImVec2(fTestX + fHoverRangeX, fTestY + fHoverRangeY)))
	{

		// ǥ���� ����
		ImGui::SetCursorScreenPos(ImVec2(fTestX + fScreenPosX, fTestY - fScreenPosY));
		ImGui::BeginTooltip();
		ImGui::Text(u8"UI_Num : %d", iTestNum);
		ImGui::Separator();
		ImGui::Text(u8"Test Value1 : %.2f", fTestValue1);
		ImGui::Text(u8"Test Value1 : %.2f", fTestValue2);

		ImGui::EndTooltip(); // ���� ����
	}
}


CWindow_UITool* CWindow_UITool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWindow_UITool* pInstance = new CWindow_UITool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CWindow_UITool");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CWindow_UITool::Free()
{
	__super::Free();
}