#include "stdafx.h"
#include "Window_UITool.h"
#include "UI_Anything.h"
#include "GameInstance.h"

#include "Json_Utility.h"
#include "Target_Manager.h"
#include "RenderTarget.h"
#include "UI_Anything.h"

/* error �ܺ����� ��ȣ : define �ɾ������ */
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

CWindow_UITool::CWindow_UITool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CImgui_Window(pDevice, pContext)
{
}

HRESULT CWindow_UITool::Initialize()
{
	//! ����� Ư���� ��ɾ���. ���� �ʿ��� �� ���Ƽ� ����.
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("TOOL �ε���."));

	/* �ش� ��ξȿ� �ִ� ��� �̹������� �ҷ��´�. */
	LoadImgPath(ConverCtoWC(ConverWStringtoC(TEXT("../Bin/Resources/Textures/UI/Image/PlayerHUD"))));
	//LoadImgPath(ConverCtoWC(ConverWStringtoC(TEXT("../Bin/Resources/Textures/UI/Image"))));
	
	/* �� ���� �����ɸ��ϱ� ���� ��ư������ �ҷ�����.. */
	/* �����ص� �̹��� ��ε��� �ҷ��´�. */
	// Load_Paths();

	// �̹��� �ε�
	_int iSize = (_int)m_vecInitialPaths.size();
	for (_int i = 0; i < iSize; i++)
	{
		IMAGEINFO* tTexture = new IMAGEINFO;

		_bool bRet = LoadTextureFromFile(ConverWStringtoC(ConvertToWideString(m_vecInitialPaths[i]->strFilePath)), &tTexture->SRV_Texture, &tTexture->iImage_Width, &tTexture->iImage_Height);
		IM_ASSERT(bRet);

		/* Texture ũ�⸦ ���Ƿ� �����ϰ� �ʹٸ�, ���⼭ ������ �������. ���� ���ָ� ���� �ؽ�ó ũ���� ���´�. �� �� ���� ũ�⸦ �̿��� ������ �ٿ��� ����ص� ���� �� ����. */
		//tTexture->iImage_Width = 100;
		//tTexture->iImage_Height = 100;
		tTexture->iTextureNum = i;
		m_vecTexture.push_back(tTexture);

		tTexture = nullptr;
		delete[] tTexture;
	}

	_int iPathSize = (_int)m_vecInitialPaths.size();
	//for (auto& iter : m_vecInitialPaths)
	for(_int i = 0; i < iPathSize; i++)
	{
		string strFileNameWithoutExtension = GetFileName(m_vecInitialPaths[i]->strFilePath);
		string strFileName = RemoveExtension(strFileNameWithoutExtension);

		PATHINFO* pPathInfo = new PATHINFO;

		pPathInfo->strFileName = strFileName;
		pPathInfo->strFilePath = m_vecInitialPaths[i]->strFilePath;

		/* ��� �߶� �ֱ� */
		m_vecImagePaths.push_back(pPathInfo);

		pPathInfo = nullptr;
		delete[] pPathInfo;
	}

#pragma region �������
	//char filePath[MAX_PATH] = "../Bin/DataFiles/Data_UI/Texture_Info/Texture_Info";

	//json Out_Json;
	//_ushort iIndex = 0;

	//for (PATHINFO* iter : m_vecInitialPaths)
	//{
	//	json object;
	//	object["PathNum"] = iter->iPathNum;
	//	object["FileName"] = iter->strFileName;
	//	object["FilePath"] = iter->strFilePath;
	//	Out_Json.emplace(to_string(iIndex++), object);
	//}

	//CJson_Utility::Save_Json(filePath, Out_Json);

	//CJson_Utility::Load_Json(filePath, Out_Json);
#pragma endregion 


#pragma region Start Setting
	// �������ڸ��� �ƹ��͵� �ȴ����� �� �⺻ �ױ� �� �ְ� �����ϱ�.
	m_tChild_Desc.strLayerTag = "Layer_UI_Player";
	m_tChild_Desc.strCloneTag = "Prototype_GameObject_UI_Anything";
	m_tChild_Desc.strProtoTag = m_vecImagePaths[m_iSelectedPathIndex]->strFileName;
	m_tChild_Desc.strFilePath = m_vecImagePaths[m_iSelectedPathIndex]->strFilePath;
	m_tChild_Desc.fPositionX = g_iWinSizeX / 2;
	m_tChild_Desc.fPositionY = g_iWinSizeY / 2;
	m_tChild_Desc.fScaleX = 100;
	m_tChild_Desc.fScaleY = 100;

	m_tParent_Desc.strLayerTag = "Layer_UI_Player";
	m_tParent_Desc.strCloneTag = "Prototype_GameObject_UI_Player_Left_Interface";
	m_tParent_Desc.strProtoTag = m_vecImagePaths[m_iSelectedPathIndex]->strFileName;
	m_tParent_Desc.strFilePath = m_vecImagePaths[m_iSelectedPathIndex]->strFilePath;
	m_tParent_Desc.fPositionX = g_iWinSizeX / 2;
	m_tParent_Desc.fPositionY = g_iWinSizeY / 2;
	m_tParent_Desc.fScaleX = 100;
	m_tParent_Desc.fScaleY = 100;
#pragma endregion End

	SetWindowText(g_hWnd, TEXT("TOOL �ε��� �Ϸ�Ǿ����ϴ�."));

	return S_OK;
}

void CWindow_UITool::Tick(_float fTimeDelta)
{
	/* Load */
	// SetUp_Initialize();

	IndexCheck();
	GetCursorPos(&m_pt);
	ScreenToClient(g_hWnd, &m_pt);
	Shortcut_Key(fTimeDelta);
	ShowDialog();

	__super::Tick(fTimeDelta);


	SetUp_ImGuiDESC("Parent", { 600.f, 300.f }, 0, { 0.f, 0.f, 0.f, 0.f });
	__super::Begin();

	// Parent
	Parent_List(fTimeDelta);
	ImGui::Spacing();

	__super::End();


	SetUp_ImGuiDESC("Info", { 600.f, 300.f }, 0, { 0.f, 0.f, 0.f, 0.f });
	__super::Begin();
	UI_Info();
	__super::End();


	SetUp_ImGuiDESC("Child", { 600.f, 300.f }, 0, { 0.f, 0.f, 0.f, 0.f });
	__super::Begin();
	if (ImGui::BeginTabBar("MyTabBar", m_Tab_bar_flags))
	{

		if (ImGui::BeginTabItem("Texture", &m_bOpenTexture))
		{

			ImGui::Dummy(ImVec2(0.f, 10.f));

			/* List */
			Child_List(fTimeDelta);

			ImGui::Dummy(ImVec2(0.f, 10.f));

			/* UI_2D ���� */

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Setting", &m_bOpenSetting))
		{
			/* �̹��� ���� �� �̸����� */
			ImagePreview(fTimeDelta);

			ImGui::Dummy(ImVec2(0, 5.f)); // ����

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Text", &m_bOpenUI))
		{
			ImGui::Text("Tab");



			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	/* Default : LastNumber */
	UI_ToolTip(fTimeDelta); // Tip : ������ �������̽� ��� ����� ���� �������� ȣ��Ǿ���Ѵ�. (�ȱ׷��� �ٸ� �༮���� ������ �з��� ��µ�)
	__super::End();

}

void CWindow_UITool::Render()
{

}

void CWindow_UITool::Shortcut_Key(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_1))
	{
		m_bOpenTexture = true;
		m_bOpenSetting = false;
		m_bOpenUI = false;
	}
	if (m_pGameInstance->Key_Down(DIK_2))
	{
		m_bOpenTexture = false;
		m_bOpenSetting = true;
		m_bOpenUI = false;
	}
	if (m_pGameInstance->Key_Down(DIK_3))
	{
		m_bOpenTexture = false;
		m_bOpenSetting = false;
		m_bOpenUI = true;
	}

	if (m_pGameInstance->Key_Down(DIK_Q))
	{
		m_iChangeType = SCALE;
	}
	if (m_pGameInstance->Key_Down(DIK_W))
	{
		m_iChangeType = ROTATION;
	}
	if (m_pGameInstance->Key_Down(DIK_E))
	{
		m_iChangeType = POSITION;
	}
	if (m_pGameInstance->Key_Down(DIK_R))
	{
		m_iChangeType = NONE;
	}

	if (m_pGameInstance->Key_Down(DIK_DELETE))
	{
		++m_iSelected_ChildObjectIndex;

		if (!m_vecChildObject.empty())
		{
			_int iSize = (_int)m_vecChildObject.size();
			if (m_iSelected_ChildObjectIndex >= iSize)
				m_iSelected_ChildObjectIndex = iSize - 1;
		}
	}

	if (m_pGameInstance->Key_Down(DIK_X))
	{
		--m_iSelected_ChildObjectIndex;

		if (m_iSelected_ChildObjectIndex <= 0)
			m_iSelected_ChildObjectIndex = 0;
	}

	if (m_pGameInstance->Key_Pressing(DIK_UP))
	{
		if(m_iChangeType == (_int)SCALE)
			m_pCurrChild->Change_SizeTop(m_fChangeValue);
		if (m_iChangeType == (_int)ROTATION)
			m_pCurrChild->Get_Transform()->Turn(_vector({ 1.f, 0.f, 0.f, 0.f }), m_fChangeValue);
		if (m_iChangeType == (_int)POSITION)
		{
			_vector vPos = m_pCurrChild->Get_Transform()->Get_Position();
			vPos.m128_f32[1] += m_fChangeValue;
			m_pCurrChild->Set_Pos(vPos.m128_f32[0], vPos.m128_f32[1]);
		}
	}
	if (m_pGameInstance->Key_Pressing(DIK_DOWN))
	{
		if (m_iChangeType == (_int)SCALE)
			m_pCurrChild->Change_SizeBottom(m_fChangeValue);
		if (m_iChangeType == (_int)ROTATION)
			m_pCurrChild->Get_Transform()->Turn(_vector({ 1.f, 0.f, 0.f, 0.f }), -m_fChangeValue);
		if (m_iChangeType == (_int)POSITION)
		{
			_vector vPos = m_pCurrChild->Get_Transform()->Get_Position();
			vPos.m128_f32[1] += -m_fChangeValue;
			m_pCurrChild->Set_Pos(vPos.m128_f32[0], vPos.m128_f32[1]);
		}
	}
	if (m_pGameInstance->Key_Pressing(DIK_LEFT))
	{
		if (m_iChangeType == (_int)SCALE)
			m_pCurrChild->Change_SizeLeft(m_fChangeValue);
		if (m_iChangeType == (_int)ROTATION)
			m_pCurrChild->Get_Transform()->Turn(_vector({ 0.f, 0.f, 1.f, 0.f }), -m_fChangeValue);
		if (m_iChangeType == (_int)POSITION)
		{
			_vector vPos = m_pCurrChild->Get_Transform()->Get_Position();
			vPos.m128_f32[0] += -m_fChangeValue;
			m_pCurrChild->Set_Pos(vPos.m128_f32[0], vPos.m128_f32[1]);
		}
	}
	if (m_pGameInstance->Key_Pressing(DIK_RIGHT))
	{
		if (m_iChangeType == (_int)SCALE)
			m_pCurrChild->Change_SizeRight(m_fChangeValue);
		if (m_iChangeType == (_int)ROTATION)
			m_pCurrChild->Get_Transform()->Turn(_vector({ 0.f, 0.f, 1.f, 0.f }), m_fChangeValue);
		if (m_iChangeType == (_int)POSITION)
		{
			_vector vPos = m_pCurrChild->Get_Transform()->Get_Position();
			vPos.m128_f32[0] += m_fChangeValue;
			m_pCurrChild->Set_Pos(vPos.m128_f32[0], vPos.m128_f32[1]);
		}
	}

	if (m_pGameInstance->Key_Pressing(DIK_0))
	{
		m_fChangeValue = 0.1f;
	}
	if (m_pGameInstance->Key_Pressing(DIK_MINUS))
	{
		m_fChangeValue += 0.1f;
	}
	if (m_pGameInstance->Key_Pressing(DIK_EQUALS))
	{
		m_fChangeValue -= 0.1f;
		if (m_fChangeValue <= 0.f)
			m_fChangeValue = 0.f;
	}

	/* SHIFT_L */
	if (m_pGameInstance->Key_Pressing(DIK_LSHIFT))
	{
		if (m_pGameInstance->Mouse_Down(DIM_LB))
		{
			m_tChild_Desc.fPositionX = m_pt.x;
			m_tChild_Desc.fPositionY = m_pt.y;
			Create_Child(m_tChild_Desc);
			
		}
	}

	/* Control_L */
	if (m_pGameInstance->Key_Pressing(DIK_LCONTROL))
	{
		if (m_pGameInstance->Mouse_Pressing(DIM_LB))
		{
			if (m_pCurrChild != nullptr)
				m_pCurrChild->Moving_Picking_Point(m_pt);
		}

		if (m_pGameInstance->Key_Down(DIK_S))
		{
			m_eDialogType = CImgui_Window::SAVE_DIALOG;
			OpenDialog(CImgui_Window::IMGUI_UITOOL_WINDOW);
		}

		if (m_pGameInstance->Key_Down(DIK_D))
		{
			Create_Child(m_tChild_Desc);
		}

		if (m_pGameInstance->Key_Down(DIK_L))
		{
			m_eDialogType = CImgui_Window::LOAD_DIALOG;
			OpenDialog(CImgui_Window::IMGUI_UITOOL_WINDOW);
		}

		if (m_pGameInstance->Key_Down(DIK_M))
		{
			Create_TargetTexture();
		}
	}

}

void CWindow_UITool::Layer_List()
{

	if (ImGui::BeginListBox("Layer"))
	{
		for (_int i = 0; i < 3; i++)
		{
			const bool is_selected = (m_iCurrLayerNum == i);
			if (ImGui::Selectable(m_strLayer[i].c_str(), is_selected))
				m_iCurrLayerNum = i;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
				m_tChild_Desc.strLayerTag = m_strLayer[is_selected].c_str();
				m_tParent_Desc.strLayerTag = m_strLayer[is_selected].c_str();
			}
		}

		ImGui::EndListBox();
	}
}

void CWindow_UITool::Texture_List()
{
	static int	Texture_idx = 0;
	_int		iTextureTagSize = (_int)m_vecImagePaths.size();

	if (ImGui::BeginListBox("Texture"))
	{
		for (_int i = 0; i < iTextureTagSize; i++)
		{
			const bool is_selected = (m_iSelectedPathIndex == i);
			if (ImGui::Selectable(ConverWStringtoC(ConvertToWideString(m_vecImagePaths[i]->strFileName)), is_selected))
			{
				m_iSelectedPathIndex = i;

				//// ���ڿ� �ߺ� ��
				//AddIndexNumber(*m_vecImagePaths[m_iSelectedPathIndex]); // ������Ʈ �ױ� ����
				/* ������ �ױ� ���� */
				m_tChild_Desc.strProtoTag = m_vecImagePaths[m_iSelectedPathIndex]->strFileName;
				m_tParent_Desc.strProtoTag = m_vecImagePaths[m_iSelectedPathIndex]->strFileName;
			}
			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}
	///* �̸����� */
	//ImGui::Image((void*)m_vecTexture[m_iSelectedPathIndex]->SRV_Texture, ImVec2((_float)m_vecTexture[m_iSelectedPathIndex]->iImage_Width, (_float)m_vecTexture[m_iSelectedPathIndex]->iImage_Height));
	//m_tChild_Desc.strProtoTag = m_vecImagePaths[m_iSelectedPathIndex]->strFileName; // ����Ʈ �ڽ� ������ �� ���� : ����Ʈ���� ���þ����� ���� ���� �����ϵ��� ��� �����ϰ���
}

void CWindow_UITool::Setting_Parent()
{
	ImGui::CollapsingHeader("Setting_Parent");

	/* Mod */
	ImGui::SeparatorText(u8"���� ��� ����");
	ImGui::RadioButton("Scale", &m_iChangeType, 1);
	ImGui::RadioButton("Rotation", &m_iChangeType, 2);
	ImGui::RadioButton("Position", &m_iChangeType, 3);

	/* Scale */
	ImGui::SeparatorText(u8"ũ�� ����");
	ImGui::InputFloat("ScaleX", &m_fParent_Scale.x);
	ImGui::InputFloat("ScaleY", &m_fParent_Scale.y);

	/* Rotation */
	ImGui::SeparatorText(u8"ȸ�� ����");
	ImGui::InputFloat("ScaleX", &m_fParent_Scale.x);
	ImGui::InputFloat("ScaleY", &m_fParent_Scale.y);

	/* Position*/
	ImGui::SeparatorText(u8"��ġ ����");
	ImGui::InputFloat("PositionX", &m_fParent_Possition.x);
	ImGui::InputFloat("PositionY", &m_fParent_Possition.y);


	ImGui::Separator();
	ImGui::InputTextWithHint(u8"�Է� ", u8"�ؽ�Ʈ�� �Է��ϼ���.", m_cInputText, IM_ARRAYSIZE(m_cInputText));
	ImGui::Text(m_cInputText);

	if (m_pCurrParent != nullptr)
	{
		_vector vPosition = dynamic_cast<CUI*>(m_pCurrParent)->Get_Transform()->Get_State(CTransform::STATE_POSITION);
		_vector vOrthoPos = m_pGameInstance->Convert_Orthogonal(vPosition);

		ImGui::InputFloat("PositionX", &vOrthoPos.m128_f32[0]);
		ImGui::InputFloat("PositionY", &vOrthoPos.m128_f32[1]);
		ImGui::InputFloat("PositionZ", &vOrthoPos.m128_f32[2]);

		Set_GuizmoCamView();
		Set_GuizmoCamProj();
		Set_GuizmoUI(m_pCurrParent);
	}

	ImGui::Dummy(ImVec2(0, 5)); // ����

	ImGui::Separator();

	ImGui::Dummy(ImVec2(0, 5)); // ����
}

HRESULT CWindow_UITool::Create_Parent(CUI::UI_DESC pUIDesc)
{
	wstring strCloneProto = TEXT("");
	m_pGameInstance->String_To_WString(m_tParent_Desc.strCloneTag, strCloneProto);

	m_tParent_Desc.bParent = true;
	CUI* m_pCurrObject = dynamic_cast<CUI*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_STATIC, ConvertToWideString(m_strLayer[m_iCurrLayerNum]), strCloneProto, &m_tParent_Desc));
	Add_ParentList(m_tParent_Desc);
	m_vecParentObject.push_back(m_pCurrObject);
	m_pCurrParent = m_pCurrObject;

	return S_OK;
}

void CWindow_UITool::Delete_Parent()
{
	if (m_vecParentObject.empty())
		return;
	if (m_vecParentObjectName.empty())
		return;

	dynamic_cast<CUI*>(m_vecParentObject[m_iSelected_ParentObjectIndex])->Set_Dead(true);
	m_vecParentObject.erase(m_vecParentObject.begin() + m_iSelected_ParentObjectIndex);

	if (!m_vecParentObjectName.empty())
		m_vecParentObjectName.erase(m_vecParentObjectName.begin() + m_iSelected_ParentObjectIndex); // ������Ʈ ��� �̸� ����

	if (m_iSelected_ParentObjectIndex > 0 &&
		m_iSelected_ParentObjectIndex <= m_vecParentObject.size())
		--m_iSelected_ParentObjectIndex;

	if (m_vecParentObject.empty())
	{
		m_pCurrParent = nullptr;
	}
	else
	{
		m_pCurrParent = dynamic_cast<CUI*>(m_vecParentObject[m_iSelected_ParentObjectIndex]);
	}
}

void CWindow_UITool::Parent_Class(_float fTimeDelta)
{
	if (m_vecParent.empty())
		return;

	_int iParentClassTagSize = (_int)m_vecParent.size();

	if (ImGui::BeginListBox("Parent Class"))
	{
		for (_int i = 0; i < iParentClassTagSize; i++)
		{
			const bool is_selected = (m_iSelected_ParentClassIndex == i);
			if (ImGui::Selectable(ConverWStringtoC(ConvertToWideString(m_vecParent[i].c_str())), is_selected))
			{
				m_iSelected_ParentClassIndex = i;

				/* Ŭ���� �ױ� ���� */
				m_tParent_Desc.strCloneTag = "Prototype_GameObject_UI_";
				m_tParent_Desc.strCloneTag += m_vecParent[m_iSelected_ParentClassIndex].c_str();
			}
			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}
}

void CWindow_UITool::Parent_Object(_float fTimeDelta)
{

	if (ImGui::Button("Parent_Create"))
	{
		Create_Parent(m_tParent_Desc);	// �θ����
	}

	ImGui::SameLine();

	if (ImGui::Button("Parent_Delete"))
	{
		Delete_Parent();
	}

	/* �������� ���� */
	_int		iParentObjectTagSize = (_int)m_vecParentObjectName.size();

	if (ImGui::BeginListBox("Parent Object"))
	{
		for (_int i = 0; i < iParentObjectTagSize; i++)
		{
			const bool is_selected = (m_iSelected_ParentObjectIndex == i);
			if (ImGui::Selectable(ConverWStringtoC(ConvertToWideString(m_vecParentObjectName[i]->strFileName)), is_selected))
			{
				m_iSelected_ParentObjectIndex = i;
				m_pCurrParent = dynamic_cast<CUI*>(m_vecParentObject[m_iSelected_ParentObjectIndex]);
				m_bParent = true;
			}
			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}
	ImGui::Spacing();

}

void CWindow_UITool::Setting_Child()
{
	ImGui::CollapsingHeader("2D_Setting");

	/* Mod */
	ImGui::SeparatorText(u8"���� ��� ����");
	ImGui::RadioButton("Scale", &m_iChangeType, 1);
	ImGui::RadioButton("Rotation", &m_iChangeType, 2);
	ImGui::RadioButton("Position", &m_iChangeType, 3);

	/* Rotation */
	ImGui::SeparatorText(u8"ȸ�� ����");
	ImGui::InputFloat("ScaleX", &m_fParent_Scale.x);
	ImGui::InputFloat("ScaleY", &m_fParent_Scale.y);

	/* Scale */
	ImGui::SeparatorText(u8"ũ�� ����");
	ImGui::InputFloat("ScaleX", &m_fChild_Scale.x);
	ImGui::InputFloat("ScaleY", &m_fChild_Scale.y);

	/* Position*/
	ImGui::SeparatorText(u8"��ġ ����");
	ImGui::InputFloat("PositionX", &m_fChild_Possition.x);
	ImGui::InputFloat("PositionY", &m_fChild_Possition.y);

	ImGui::Separator();
	ImGui::InputTextWithHint(u8"�Է� ", u8"�ؽ�Ʈ�� �Է��ϼ���.", m_cInputText, IM_ARRAYSIZE(m_cInputText));
	ImGui::Text(m_cInputText);

	ImGui::Dummy(ImVec2(0, 5)); // ����

	if (m_pCurrChild != nullptr)
	{
		Set_GuizmoCamView();
		Set_GuizmoCamProj();
		Set_GuizmoUI(m_pCurrChild);
	}

	ImGui::Separator();

	ImGui::Dummy(ImVec2(0, 5)); // ����
}

void CWindow_UITool::Current_Info()
{
	ImGui::CollapsingHeader("2D_Setting");

	/* Mod */
	ImGui::SeparatorText(u8"���� ��� ����");
	ImGui::RadioButton("Scale", &m_iChangeType, 1);
	ImGui::RadioButton("Rotation", &m_iChangeType, 2);
	ImGui::RadioButton("Position", &m_iChangeType, 3);

	/* Scale */
	ImGui::SeparatorText(u8"ũ�� ����");
	ImGui::InputFloat("ScaleX", &m_tChild_Desc.fScaleX);
	ImGui::InputFloat("ScaleY", &m_tChild_Desc.fScaleY);

	/* Position*/
	ImGui::SeparatorText(u8"��ġ ����");
	ImGui::InputFloat("PositionX", &m_tChild_Desc.fPositionX);
	ImGui::InputFloat("PositionY", &m_tChild_Desc.fPositionY);

	ImGui::Separator();
	ImGui::InputTextWithHint(u8"�Է� ", u8"�ؽ�Ʈ�� �Է��ϼ���.", m_cInputText, IM_ARRAYSIZE(m_cInputText));
	ImGui::Text(m_cInputText);

	ImGui::Dummy(ImVec2(0, 5)); // ����

	ImGui::Separator();

	ImGui::Dummy(ImVec2(0, 5)); // ����
}

void CWindow_UITool::Parent_List(_float fTimeDelta)
{
	Parent_Class(fTimeDelta);
	Parent_Object(fTimeDelta);
	Setting_Parent();
}

void CWindow_UITool::Child_List(_float fTimeDelta)
{
	if (m_vecImagePaths.empty() ||
		m_vecTexture.empty())
		return;

	//ImGui::RadioButton("UI", &m_iRenderGroup, (_int)CRenderer::RENDERGROUP::RENDER_UI); ImGui::SameLine();

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyMask_;
	ImGuiWindowFlags WindowFlag = ImGuiWindowFlags_HorizontalScrollbar;

	// Layer
	Layer_List();
	ImGui::Spacing();

	// Textuer
	Texture_List();
	ImGui::Spacing();

	// Child
	Child_Class(fTimeDelta);
	Child_Object(fTimeDelta);
	Setting_Child();
}

void CWindow_UITool::Child_Class(_float fTimeDelta)
{
	_int		ClassTagSize = (_int)m_vecClass.size();

	if (ImGui::BeginListBox("Child Class"))
	{
		for (_int i = 0; i < ClassTagSize; i++)
		{
			const bool is_selected = (m_iSelected_ChildClassIndex == i);
			if (ImGui::Selectable(ConverWStringtoC(ConvertToWideString(m_vecClass[i].c_str())), is_selected))
			{
				m_iSelected_ChildClassIndex = i;
				/* Ŭ���� �ױ� ���� */
				m_tChild_Desc.strCloneTag = "Prototype_GameObject_UI_";
				m_tChild_Desc.strCloneTag += m_vecClass[m_iSelected_ChildClassIndex].c_str();
			}
			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}
}

void CWindow_UITool::Child_Object(_float fTimeDelta)
{

#pragma region Create/Delete
	if (ImGui::Button("Create_Child"))
	{
		Create_Child(m_tChild_Desc);
	}

	ImGui::SameLine();

	if (ImGui::Button("Delete_Child"))
	{
		Delete_Child(fTimeDelta);
	}
#pragma endregion End

	/* ������ �ε��� ���� */
	_int		ObjectTagSize = (_int)m_vecChildObjectName.size();
	if (ImGui::BeginListBox("Child Object"))
	{
		for (_int i = 0; i < ObjectTagSize; i++)
		{
			const bool is_selected = (m_iSelected_ChildObjectIndex == i);
			if (ImGui::Selectable(ConverWStringtoC(ConvertToWideString(m_vecChildObjectName[i]->strFileName)), is_selected))
			{
				m_iSelected_ChildObjectIndex = i;
				m_pCurrChild = dynamic_cast<CUI*>(m_vecChildObject[m_iSelected_ChildObjectIndex]);
				m_bParent = false;
			}
			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}
	ImGui::Spacing();
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

	//// ���콺�� �ش� ��ġ ���� �ִ� ��쿡�� ���� ǥ�� (���콺 ����)
	//if (ImGui::IsMouseHoveringRect(
	//	ImVec2(fTestX - fHoverRangeX, fTestY - fHoverRangeY),
	//	ImVec2(fTestX + fHoverRangeX, fTestY + fHoverRangeY)))
	//{

	//	// ǥ���� ����
	//	ImGui::SetCursorScreenPos(ImVec2(fTestX + fScreenPosX, fTestY - fScreenPosY));
	//	ImGui::BeginTooltip();

	//	ImGui::EndTooltip(); // ���� ����
	//}
}

// �̹����� �޸𸮿� �ε��ϴ� �Լ�
std::vector<unsigned char> CWindow_UITool::UI_LoadImage(const std::string& filename, int& width, int& height, int& channels)
{
	// ���� ���� �б� (�̸������ �ִ� 100����Ʈ������ ���)
	std::ifstream file(filename, std::ios::binary);
	if (file.is_open()) {
		char buffer[101]; // 100����Ʈ + �� ����
		if (file.read(buffer, 100)) {
			buffer[file.gcount()] = '\0'; // �� ���� �߰�
			std::cout << "Preview: " << buffer << std::endl;
		}
		file.close();
	}
	else {
		std::cerr << "Failed to open file: " << filename << std::endl;
	}

	// ���� ũ�� Ȯ��
	file.seekg(0, std::ios::end);
	std::streamsize fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	// ���� ������ ������ ���� ����
	std::vector<unsigned char> imageData(fileSize);
	if (!file.read(reinterpret_cast<char*>(imageData.data()), fileSize)) {
		std::cerr << "Failed to read file: " << filename << std::endl;
		return {};
	}

	// �̹��� �ʺ�, ����, ä�� �� ���� (���÷� ���� �� ���)
	width = 256;
	height = 256;
	channels = 4; // RGBA

	return imageData;
}

// Simple helper function to load an image into a DX11 texture with common settings
bool CWindow_UITool::LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	m_pDevice->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	m_pDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();

	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(image_data);

	return true;
}

// std::string ���͸� const char* �迭�� ��ȯ�ϴ� �Լ�
std::vector<const char*> CWindow_UITool::ConvertStringVectorToCharArray(const std::vector<std::string>& stringVector)
{
	std::vector<const char*> charArray;
	charArray.reserve(stringVector.size());
	for (const std::string& str : stringVector)
	{
		charArray.push_back(str.c_str());
	}
	return charArray;
}

std::wstring CWindow_UITool::ConvertToWideString(const std::string& ansiString)
{
	int wideStrLen = MultiByteToWideChar(CP_ACP, 0, ansiString.c_str(), -1, nullptr, 0);
	if (wideStrLen == 0)
	{
		// ��ȯ ���� ó��
		return L"Fail";
	}

	std::wstring wideString(wideStrLen, L'\0');
	MultiByteToWideChar(CP_ACP, 0, ansiString.c_str(), -1, &wideString[0], wideStrLen);

	return wideString;
}

std::string CWindow_UITool::WStringToString(const std::wstring& wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	return converter.to_bytes(wstr);
}

char* CWindow_UITool::ConverWStringtoC(const wstring& wstr)
{
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	char* result = new char[size_needed];
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, result, size_needed, NULL, NULL);

	char* newResult = result;

	result = nullptr;
	delete[] result;

	return newResult;
}

wchar_t* CWindow_UITool::ConverCtoWC(char* str)
{
	_tchar* pStr;
	int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);
	pStr = new WCHAR[strSize];
	MultiByteToWideChar(CP_ACP, 0, str, (_int)strlen(str) + (size_t)1, pStr, strSize);

	_tchar* newResult = pStr;

	pStr = nullptr;
	delete[] pStr;

	return newResult;
}

std::string CWindow_UITool::WideStringToString(const wchar_t* wideStr) 
{
	char ch[260];
	char DefChar = ' ';
	WideCharToMultiByte(CP_ACP, 0, wideStr, -1, ch, 260, &DefChar, NULL);

	string ss(ch);
	return ss;
	//// std::wstring���κ��� std::string���� ��ȯ
	//std::wstring wstr(wideStr);
	//// std::string���� ��ȯ
	//return std::string(wstr.begin(), wstr.end());

}

// ���� �̸��� �����ϴ� �Լ�
std::string CWindow_UITool::GetFileName(const std::string& filePath) 
{
	size_t lastSlashPos = filePath.find_last_of("/");
	if (lastSlashPos != std::string::npos) 
	{
		return filePath.substr(lastSlashPos + 1);
	}
	else 
	{
		// ��� �����ڰ� ���� ��� ��ü ��θ� ��ȯ
		return filePath;
	}
}

// _�������� �̸��� �����ϴ� �Լ�
std::string CWindow_UITool::GetUnderbarName(const std::string& filePath)
{
	size_t lastSlashPos = filePath.find_last_of("_");
	if (lastSlashPos != std::string::npos)
	{
		return filePath.substr(lastSlashPos + 1);
	}
	else
	{
		// ��� �����ڰ� ���� ��� ��ü ��θ� ��ȯ
		return filePath;
	}
}

std::string CWindow_UITool::RemoveExtension(const std::string& filePath)
{
	size_t lastDotPos = filePath.find_last_of(".");
	if (lastDotPos != std::string::npos) 
	{
		return filePath.substr(0, lastDotPos);
	}
	else 
	{
		// Ȯ���ڰ� ���� ��� �״�� ��ȯ
		return filePath;
	}
}

WCHAR* CWindow_UITool::StringTowchar(const std::string& str)
{
	// std::wstring���� ��ȯ
	std::wstring wstr(str.begin(), str.end());
	// c_str() �Լ��� ����Ͽ� WCHAR* �����ͷ� ��ȯ
	return const_cast<WCHAR*>(wstr.c_str());
}

void CWindow_UITool::LoadImgPath(const _tchar* folderPath)
{
	// ã�� �̹��� �����͸� ���� ����
	WIN32_FIND_DATA findData;

	// ���� ���
	wstring wfolderPath = (wstring)folderPath + L"/*.*";

	// ���� ��� ���� �� �ڵ� ��ȯ
	HANDLE hFind = FindFirstFile(wfolderPath.c_str(), &findData);

	// �ڵ��� ���� ���
	if (hFind != INVALID_HANDLE_VALUE)
	{
		// ���� ���丮
		vector<wstring> subDirectories;
		// �̹��� ����
		vector<wstring> imageFiles;
		do
		{
			//  ������ �Ӽ� ������ �Էµȴ�. (���丮���� �������� ��)
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (lstrcmp(findData.cFileName, L".") != 0 && lstrcmp(findData.cFileName, L"..") != 0)
				{
					// ���� ��� = ��� + / + ���ϸ�
					wstring subFolderPath = (wstring)folderPath + L"/" + findData.cFileName;
					subDirectories.push_back(subFolderPath);
					LoadImgPath(subFolderPath.c_str());
				}
			}
			else
			{
				// ������ ���, �̹��� �������� Ȯ���ϰ� �ε�
				wstring filePath = (wstring)folderPath + L"/" + findData.cFileName;

				// ���� Ȯ���� Ȯ��
				if (wcsstr(findData.cFileName, L".png") || wcsstr(findData.cFileName, L".jpg") ||
					/*wcsstr(findData.cFileName, L".bmp") || wcsstr(findData.cFileName, L".tga") ||*/
					wcsstr(findData.cFileName, L".dds"))
				{
					PATHINFO* pPathInfo = new PATHINFO;

					/* ���� Ȯ���ϱ� */
					pPathInfo->strFileName = RemoveExtension(WStringToString(findData.cFileName));
					pPathInfo->strFilePath = WStringToString(filePath);
					pPathInfo->iPathNum = m_iTextureNum;
					m_vecInitialPaths.push_back(pPathInfo);

					pPathInfo = nullptr;
					delete[] pPathInfo;

					m_iTextureNum++;
				}
			}
		} while (FindNextFile(hFind, &findData));

		FindClose(hFind);
	}
}

void CWindow_UITool::ImagePreview(_float fTimeDelta)
{

}

void CWindow_UITool::IndexCheck()
{
	if (m_iSelected_ChildObjectIndex <= 0)
		m_iSelected_ChildObjectIndex = 0;

	if (m_iSelectedPathIndex <= 0)
		m_iSelectedPathIndex = 0;
}

#pragma region Parent
void CWindow_UITool::Add_Create_Parts(CUI::UI_DESC tUIDesc)
{
	if (m_vecParentObject.empty())
		return;

		dynamic_cast<CUI*>(m_vecParentObject[m_iSelected_ParentObjectIndex])->Add_Create_Parts(&tUIDesc);
}

void CWindow_UITool::Add_Parts(CUI* pUI)
{
	if (m_vecParentObject.empty())
		return;

	CUI* pParent = dynamic_cast<CUI*>(m_vecParentObject[m_iSelected_ParentObjectIndex]);
	CUI* pChild = dynamic_cast<CUI*>(m_vecChildObject[m_iSelected_ChildObjectIndex]);

	if (pParent == nullptr ||
		pChild == nullptr)
		return;

	CTransform* pParentTransform = pParent->Get_Transform();
	pChild->Set_ParentTransform(pParentTransform);

	pParent->Add_Parts(pChild);
}
#pragma endregion

HRESULT CWindow_UITool::Create_Child(CUI::UI_DESC pUIDesc)
{
	CGameObject* pGameObject = nullptr;
	CUI* pUI = nullptr;
	wstring strCloneProto = TEXT("");
	m_pGameInstance->String_To_WString(m_tChild_Desc.strCloneTag, strCloneProto);

	pUIDesc.bParent = false;
	CUI* pCurrObject = dynamic_cast<CUI*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_STATIC, ConvertToWideString(m_strLayer[m_iCurrLayerNum]), strCloneProto, &pUIDesc));
	Add_ChildList(pUIDesc);
	m_vecChildObject.push_back(pCurrObject);
	m_pCurrChild = pCurrObject;

	return S_OK;
}

void CWindow_UITool::Delete_Child(_float fTimeDelta)
{
	if (m_vecChildObject.empty())
		return;

	// ������Ʈ ����
	m_vecChildObject[m_iSelected_ChildObjectIndex]->Set_Dead(true); // Set_Dead�� �̳༮�� ������� Layer�� ����ִ� Object�� �״´�.

	/* Set_Dead�� ��û�ϸ� �Ŵ������� �˾Ƽ� �ش� ��ü�� �����޶�� �ѰŴϱ� �� ��ü�� ���� ����ó���� ���� ���� �� �ϸ� �ȵȴ�. (�ٵ� �� ����?) */
	//Safe_Release(m_vecChildObject[m_iSelected_ChildObjectIndex]);
	m_vecChildObject.erase(m_vecChildObject.begin() + m_iSelected_ChildObjectIndex);

	/* �θ��� �����̳ʿ� ����ִ� �༮�� Dead�� true�� �༮�� �����ش�. */
	//if (!m_vecParentObject.empty())
	//	dynamic_cast<CUI*>(m_vecParentObject[m_iSelected_ChildObjectIndex])->Parts_Delete();

	if(!m_vecChildObjectName.empty())
		m_vecChildObjectName.erase(m_vecChildObjectName.begin() + m_iSelected_ChildObjectIndex); // ������Ʈ ��� �̸� ����

	if(m_pCurrChild)
		m_pCurrChild = nullptr; // ���� ���õ��ִ� �༮�� �׾����� �ּ� �������.

	m_iSelected_ChildObjectIndex = m_iSelected_ChildObjectIndex - 1; // ���� ������ �ε�����°�� ������Ʈ�� ����������, ���õ� �ε����� �ڷ� ��ĭ ��������.
}

void CWindow_UITool::Add_ParentIndexNumber(PATHINFO& UI_Info)
{
	int		index = 0;
	_bool	isPath = false;

	if (!m_vecParentObjectName.empty())
	{
		for (auto& iter : m_vecParentObjectName)
		{
			if (iter->strFilePath == UI_Info.strFilePath)
			{
				isPath = true;
				break;
			}
		}

		// ���ڿ��� �̹� �����ϴ� ���
		if (isPath)
		{
			for (auto& strFilePath : m_vecParentObjectName)
			{
				if (strFilePath->strFilePath == UI_Info.strFilePath)
				{
					/* ��� ��ȸ�ϰ� */
					if (strFilePath->iPathNum >= UI_Info.iPathNum) // ���� �����̳ʿ� �ִ� ���������� ���� �༮�� ���ں��� �۰ų� ���� ���
					{
						index = strFilePath->iPathNum + 1; // ������ �༮�� ��ȣ + 1�� ����
					}

				}
			}
		}
	}
	else
	{
		// ���ο� ���ڿ��� ���
		index = 0;
	}


	/* �ױ� */
	UI_Info.strFileName = m_vecParent[m_iSelected_ParentClassIndex].c_str() + to_string(index);
	UI_Info.iPathNum = index;
}

//										���� ������ �༮�� ��ü ��θ� �޾ƿ´�.
void CWindow_UITool::Add_ChildIndexNumber(PATHINFO& UI_Info)
{
	int		index = 0;
	_bool	isPath = false;
	//size_t underscorePos = str.find_last_of('_');
	//auto it = std::find(m_vecChildObjectName.begin(), m_vecChildObjectName.end(), UI_Info.strFilePath); // ��θ� ���Ѵ�.
	if (m_vecChildObjectName.empty())
		return;

	if (!m_vecParentObjectName.empty())
	{
		for (auto& iter : m_vecChildObjectName)
		{
			if (iter->strFilePath == UI_Info.strFilePath)
			{
				isPath = true;
				break;
			}
		}
		// ���ڿ��� �̹� �����ϴ� ���
		if (isPath)
		{
			for (auto& strFilePath : m_vecChildObjectName)
			{
				if (strFilePath->strFilePath == UI_Info.strFilePath)
				{
					/* ��� ��ȸ�ϰ� */
					if (strFilePath->iPathNum >= UI_Info.iPathNum) // ���� �����̳ʿ� �ִ� ���������� ���� �༮�� ���ں��� �۰ų� ���� ���
					{
						index = strFilePath->iPathNum + 1; // ������ �༮�� ��ȣ + 1�� ����
					}

				}
			}
		}
	}
	else 
	{
		// ���ο� ���ڿ��� ���
		index = 0;
	}

	///* �ױ� */
	//if(m_vecClass[m_iSelected_ParentClassIndex].c_str() == "Anything")
	//	UI_Info.strFileName = UI_Info.strFileName + to_string(index);
	//else
	{
		UI_Info.strFileName = m_vecClass[m_iSelected_ChildClassIndex] + to_string(index);
	}

	UI_Info.iPathNum = index;
}

void CWindow_UITool::Add_ParentList(CUI::UI_DESC tIn_UI_Desc)
{
	PATHINFO* tUI_Desc = new PATHINFO;

	// ���ڿ� �ߺ� ��
	tUI_Desc->strFileName = m_vecParent[m_iSelected_ParentClassIndex];
	tUI_Desc->strFilePath = tIn_UI_Desc.strFilePath;
	Add_ParentIndexNumber(*tUI_Desc); // ������Ʈ �ױ� ����

	m_vecParentObjectName.push_back(tUI_Desc); // �̸� �ߺ� �˻� �� ó���� �ױװ����� �־�����.

	tUI_Desc = nullptr;
	delete[] tUI_Desc;
}

void CWindow_UITool::Add_ChildList(CUI::UI_DESC tIn_UI_Desc)
{
	// error : �Ʒ� Get_CloneGameObjects�� ������Ʈ�� �����ö� ���� ������Ʈ���� ��� �ٽõ�� ���� ������, �Լ��� ���� ����ų� Ŭ�����ϰ� �������
	//if (!m_vecChildObject.empty())
	//	m_vecChildObject.clear();

	PATHINFO* tChild_Desc = new PATHINFO;

	// ���ڿ� �ߺ� ��
	tChild_Desc->strFileName = m_vecClass[m_iSelected_ChildClassIndex];
	tChild_Desc->strFilePath = tIn_UI_Desc.strFilePath;
	Add_ChildIndexNumber(*tChild_Desc); // ������Ʈ �ױ� ����

	m_vecChildObjectName.push_back(tChild_Desc); // �̸� �ߺ� �˻� �� ó���� �ױװ����� �־�����.
	//m_pGameInstance->Get_CloneGameObjects(LEVEL_STATIC, &m_vecChildObject);

	tChild_Desc = nullptr;
	delete[] tChild_Desc;
}

void CWindow_UITool::UI_Info()
{
	Menu_Info();

	Curr_Info();

}

HRESULT CWindow_UITool::Menu_Info()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::MenuItem("Save"))
			{
				m_eDialogType = CImgui_Window::SAVE_DIALOG;
				OpenDialog(CImgui_Window::IMGUI_UITOOL_WINDOW);
			}
			if (ImGui::MenuItem("Load"))
			{
				m_eDialogType = CImgui_Window::LOAD_DIALOG;
				OpenDialog(CImgui_Window::IMGUI_UITOOL_WINDOW);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	return S_OK;
}

void CWindow_UITool::Curr_Info()
{
	ImGui::Text("Mouse Pos : %d, %d", m_pt.x, m_pt.y);

	CTransform* pTransformCom = nullptr;

	if (m_bParent)
	{
		if (m_pCurrParent != nullptr)
		{
			pTransformCom = dynamic_cast<CUI*>(m_pCurrParent)->Get_Transform();
			ImGui::Text("Selected Parent : ");
			ImGui::TextColored({ 1.f, 1.f, 1.f, 1.f }, m_vecParentObjectName[m_iSelected_ParentObjectIndex]->strFileName.c_str());
		}
	}
	else
	{
		if (m_pCurrChild != nullptr)
		{
			pTransformCom = dynamic_cast<CUI*>(m_pCurrChild)->Get_Transform();
			ImGui::Text("Selected Child : ");
			ImGui::TextColored({ 1.f, 1.f, 1.f, 1.f }, m_vecChildObjectName[m_iSelected_ChildObjectIndex]->strFileName.c_str());
			ImGui::SameLine();
			if (ImGui::Button("Parts_Add"))
			{
				Add_Parts(m_pCurrChild); // �θ�  <- �ڽ�����
			}
		}

	}

	if (pTransformCom != nullptr)
	{
		_vector vScale = pTransformCom->Get_State(CTransform::STATE_POSITION);
		_vector vRotation = pTransformCom->Get_Rotated();
		_vector vPosition = pTransformCom->Get_Scaled();

		_vector vOrthoPos = m_pGameInstance->Convert_Orthogonal(vPosition);

		ImGui::InputFloat4("PositionX", &vOrthoPos.m128_f32[0]);
		ImGui::InputFloat("PositionY", &vOrthoPos.m128_f32[1]);
		ImGui::InputFloat("PositionZ", &vOrthoPos.m128_f32[2]);

		ImGui::InputFloat("RotationX", &vRotation.m128_f32[0]);
		ImGui::InputFloat("RotationY", &vRotation.m128_f32[1]);
		ImGui::InputFloat("RotationZ", &vRotation.m128_f32[2]);

		ImGui::InputFloat("ScaleX", &vScale.m128_f32[0]);
		ImGui::InputFloat("ScaleY", &vScale.m128_f32[1]);
		ImGui::InputFloat("ScaleZ", &vScale.m128_f32[2]);
	}
	else
	{
		ImGui::Text("Not Selected");
	}

	/* �̸����� */
	if(!m_vecTexture.empty())
		ImGui::Image((void*)m_vecTexture[m_iSelectedPathIndex]->SRV_Texture, ImVec2((_float)m_vecTexture[m_iSelectedPathIndex]->iImage_Width, (_float)m_vecTexture[m_iSelectedPathIndex]->iImage_Height));
}

void CWindow_UITool::Create_TargetTexture()
{
	/* error : Find�Լ��� ����Ÿ���� ã�ƿ� ��, �׳༮���� Create�Լ��� ȣ���ϸ� �ܺ�������ȣ ������ �߻���.. ���� �����ν��Ͻ��� Create���� ����Ǵ� �Լ��� ���� ���� ����ϸ� �ذ� */
	m_pGameInstance->Create_RenderTarget(TEXT("Target_Diffuse_UI"));
}

HRESULT CWindow_UITool::Save_Function(string strPath, string strFileName)
{
	if (m_vecChildObject.empty())
		return E_FAIL;

	_ushort iIndex = 0;

	strPath = strPath + "\\" + strFileName;

	json Out_Json;

	for (auto& UI : m_vecChildObject)
	{
		json Out_Object;

		dynamic_cast<CUI_Anything*>(UI)->Save_Desc(Out_Object);
		Out_Json.emplace(to_string(iIndex++), Out_Object);
	}

	CJson_Utility::Save_Json(ConverWStringtoC(ConvertToWideString(strPath)), Out_Json);

	return S_OK;
}

HRESULT CWindow_UITool::Load_Function(string strPath, string strFileName)
{
	json json_in;

	strPath = strPath + "\\" + strFileName;

	CJson_Utility::Load_Json(ConverWStringtoC(ConvertToWideString(strPath)), json_in);

	for (auto& item : json_in.items())
	{
		json object = item.value();

		CUI::UI_DESC tUI_Info;

		/* ��������� �´��� Ȯ���ϱ� */
		tUI_Info.fAlpha = object["Alpha"];					// 1. Alpha
		tUI_Info.iShaderNum = object["ShaderNum"];			// 2. ShaderPathNum
		tUI_Info.strLayerTag = object["LayerTag"];			// 3. LayerTag
		tUI_Info.strCloneTag = object["CloneTag"];			// 4. CloneTag
		tUI_Info.strProtoTag = object["ProtoTag"];			// 5. ProtoTag
		tUI_Info.strFilePath = object["FilePath"];			// 6. FilePath
		tUI_Info.strMapTextureTag = object["MapTextureTag"];// 7. MapTexture
		tUI_Info.vColor.m128_f32[0] = object["ColorR"];		// 8. R
		tUI_Info.vColor.m128_f32[1] = object["ColorG"];		// 9. G
		tUI_Info.vColor.m128_f32[2] = object["ColorB"];		// 10. B
		tUI_Info.vColor.m128_f32[3] = object["ColorA"];		// 11. A

		wstring wstrLayer;
		m_pGameInstance->String_To_WString(m_strLayer[m_iCurrLayerNum], wstrLayer);

		wstring wstrClonetag;
		m_pGameInstance->String_To_WString(tUI_Info.strCloneTag, wstrClonetag);

		wstring wstrPrototag;
		m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

		wstring wstrFilePath;
		m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

		CGameObject* pGameObject = m_pGameInstance->Add_CloneObject_And_Get(LEVEL_STATIC, wstrLayer, wstrClonetag, &tUI_Info);
		if (pGameObject == nullptr)
			return E_FAIL;

		CUI* pUI_Object = dynamic_cast<CUI*>(pGameObject);
		if (pUI_Object == nullptr)
			return E_FAIL;

		pUI_Object->Get_Transform()->Load_FromJson(object); // 12. TransformCom

		if (tUI_Info.bParent == true)
		{
			Add_ParentList(tUI_Info);
		}
		else
		{
			Add_ChildList(tUI_Info);
		}

	}

	return S_OK;
}

void CWindow_UITool::Load_Paths()
{
	json json_in;

	char filePath[MAX_PATH] = "../Bin/DataFiles/Data_UI/Texture_Info/Texture_Info";

	CJson_Utility::Load_Json(filePath, json_in);

	for (auto& item : json_in.items())
	{
		json object = item.value();

		PATHINFO* tPath = new PATHINFO;

		tPath->strFileName = object["FileName"];
		tPath->strFilePath = object["FilePath"];
		tPath->iPathNum = object["PathNum"];

		m_vecInitialPaths.push_back(tPath);

		tPath = nullptr;
		delete[] tPath;
	}
}

// ImGui�� ����Ͽ� �̹����� ǥ���ϴ� �Լ�
void CWindow_UITool::ShowImagePreview(const std::vector<unsigned char>& imageData, int width, int height)
{
	ImVec2 imageSize((_float)width, (_float)height);
	ImGui::Image(reinterpret_cast<void*>(const_cast<unsigned char*>(imageData.data())), imageSize);
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

	//if (!m_vecChildObjectName.empty())
	//	Safe_Delete(m_vecChildObjectName);
	//if (!m_vecImagePaths.empty())
	//	Safe_Delete(m_vecImagePaths);
	//if (!m_vecInitialPaths.empty())
	//	Safe_Delete(m_vecInitialPaths);
	//if (!m_vecTexture.empty())
	//	Safe_Delete(m_vecTexture);
	//if (!m_vecChildObject.empty())
	//	Safe_Delete(m_vecChildObject);
	
	// Safe_Delete(m_pCurrChild);
	// Safe_Delete(m_pCurrParent);

	m_pCurrChild = nullptr;
	m_pCurrParent = nullptr;

	if (!m_vecChildObjectName.empty())
	{
		for (auto& path : m_vecChildObjectName)
		{
			delete path;
		}
		m_vecChildObjectName.clear();
	}
	if (!m_vecImagePaths.empty())
	{
		for (auto& path : m_vecImagePaths)
		{
			delete path;
		}
		m_vecImagePaths.clear();
	}
	if (!m_vecInitialPaths.empty())
	{
		for (auto& path : m_vecInitialPaths)
		{
			delete path;
		}
		m_vecInitialPaths.clear();
	}
	if (!m_vecTexture.empty())
	{
		for (auto& path : m_vecTexture)
		{
			delete path;
		}
		m_vecTexture.clear();
	}
	//if (!m_vecChildObject.empty())
	//{
	//	for (auto& path : m_vecChildObject) {
	//		delete path;
	//	}
	//	m_vecChildObject.clear();
	//}


}