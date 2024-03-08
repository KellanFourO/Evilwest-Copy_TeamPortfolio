#include "stdafx.h"
#include "Window_UITool.h"
#include "UI_Anything.h"
#include "GameInstance.h"
#include "imgui_internal.h"

#include "Json_Utility.h"
#include "Target_Manager.h"
#include "RenderTarget.h"
#include "UI_Anything.h"
#include "UI_Text.h"
#include "Easing_Utillity.h"

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
	//LoadImgPath(ConverCtoWC(ConverWStringtoC(TEXT("../Bin/Resources/Textures/UI/Image/Option"))));		// Option
	LoadImgPath(ConverCtoWC(ConverWStringtoC(TEXT("../Bin/Resources/Textures/UI/Image/PlayerHUD")))); // PlayerHUD
	//LoadImgPath(ConverCtoWC(ConverWStringtoC(TEXT("../Bin/Resources/Textures/UI/Image"))));			// Image
	//LoadImgPath(ConverCtoWC(ConverWStringtoC(TEXT("../Bin/Resources/Textures/UI/Image"))));			// All
	
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
	m_tChild_Desc.fScaleX = 100.f;
	m_tChild_Desc.fScaleY = 100.f;

	m_tParent_Desc.strLayerTag = "Layer_UI_Player";
	m_tParent_Desc.strCloneTag = "Prototype_GameObject_UI_Player_Left_Interface";
	m_tParent_Desc.strProtoTag = m_vecImagePaths[m_iSelectedPathIndex]->strFileName;
	m_tParent_Desc.strFilePath = m_vecImagePaths[m_iSelectedPathIndex]->strFilePath;
	m_tParent_Desc.fPositionX = g_iWinSizeX / 2;
	m_tParent_Desc.fPositionY = g_iWinSizeY / 2;
	m_tParent_Desc.fScaleX = 100.f;
	m_tParent_Desc.fScaleY = 100.f;
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

	switch (m_eToolType)
	{
	case Client::CWindow_UITool::TOOL_UI:
	{
		SetUp_ImGuiDESC("Parent", { 600.f, 300.f }, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | /*ImGuiWindowFlags_NoResize | /*ImGuiWindowFlags_NoMove |*/ ImGuiWindowFlags_NoBringToFrontOnFocus, ImVec4(0.f, 0.f, 0.f, 0.8f));
		__super::Begin();

		// Parent
		Parent_List(fTimeDelta);

		if (ImGui::Button("Create_Terrain"))
			m_pGameInstance->Add_CloneObject(LEVEL_TOOL, TEXT("Layer_BackGround"), TEXT("Prototype_GameObject_Terrain"));

		if(ImGui::Button("Create_Monster"))
			m_pGameInstance->Add_CloneObject(LEVEL_TOOL, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Screamer"));

		ImGui::Spacing();

		__super::End();


		SetUp_ImGuiDESC("Info", { 600.f, 300.f }, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | /*ImGuiWindowFlags_NoResize | /*ImGuiWindowFlags_NoMove |*/ ImGuiWindowFlags_NoBringToFrontOnFocus, ImVec4(0.f, 0.f, 0.f, 0.8f));
		__super::Begin();
		UI_Info();
		__super::End();

		//SetUp_ImGuiDESC("Text", { 600.f, 300.f }, 0, { 0.f, 0.f, 0.f, 0.f });
		//__super::Begin();
		//__super::End();


		SetUp_ImGuiDESC("Child", { 600.f, 300.f }, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | /*ImGuiWindowFlags_NoResize | /*ImGuiWindowFlags_NoMove |*/ ImGuiWindowFlags_NoBringToFrontOnFocus, ImVec4(0.f, 0.f, 0.f, 0.8f));
		__super::Begin();
		Menu_Info();
		if (ImGui::BeginTabBar("MyTabBar", m_Tab_bar_flags))
		{

			if (ImGui::BeginTabItem("Texture"))
			{

				ImGui::Dummy(ImVec2(0.f, 10.f));

				if (m_pGameInstance->Key_Down(DIK_HOME))
				{
					if (!m_bCreateOK)
					{
						m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Prototype_GameObject_Sky"));
						m_bCreateOK = true;
					}
				}
				/* List */
				Child_List(fTimeDelta);

				ImGui::Dummy(ImVec2(0.f, 10.f));

				/* UI_2D ���� */

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Text"))
			{
				ImGui::Text("Tab");

				Text_List();

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
		__super::End();
		break;
	}
	case Client::CWindow_UITool::TOOL_ANIMATION:
	{
		SetUp_ImGuiDESC("UI_Animation", { 600.f, 300.f }, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | /*ImGuiWindowFlags_NoResize | /*ImGuiWindowFlags_NoMove |*/ ImGuiWindowFlags_NoBringToFrontOnFocus, ImVec4(0.f, 0.f, 0.f, 0.8f));
		__super::Begin();
		if (ImGui::BeginTabBar("UI_AnimTab", m_Tab_bar_flags))
		{
			if (ImGui::BeginTabItem("UI_Anim"))
			{
				//PlayAnimation(fTimeDelta);
				//AnimationTimeLineBar();
				KeyframeAutomaticGeneration();
				KeyframeRender_ValueChange();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		__super::End();

		SetUp_ImGuiDESC("UI_TimeLine", { 1000.f, 300.f }, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | /*ImGuiWindowFlags_NoResize | /*ImGuiWindowFlags_NoMove |*/ ImGuiWindowFlags_NoBringToFrontOnFocus, ImVec4(0.f, 0.f, 0.f, 0.8f));
		__super::Begin();
		if (ImGui::BeginTabBar("m_vecTimelineab", m_Tab_bar_flags))
		{
			PlayAnimation(fTimeDelta);
			AnimationTimeLineBar();
			ImGui::EndTabBar();
		}
		__super::End();
		break;
	}
	case Client::CWindow_UITool::TOOL_END:
		break;
	default:
		break;
	}

	/* Default : LastNumber */
	UI_ToolTip(fTimeDelta); // Tip : ������ �������̽� ��� ����� ���� �������� ȣ��Ǿ���Ѵ�. (�ȱ׷��� �ٸ� �༮���� ������ �з��� ��µ�)
}

void CWindow_UITool::Render()
{

}

void CWindow_UITool::Shortcut_Key(_float fTimeDelta)
{
		// �����̽��ٰ� ������ �� ������ �ڵ�
	

	if (ImGui::IsKeyPressed(ImGuiKey_1))
	{
		m_eToolType = CWindow_UITool::TOOL_UI;
	}
	if (ImGui::IsKeyPressed(ImGuiKey_2))
	{
		m_eToolType = CWindow_UITool::TOOL_ANIMATION;
	}
	if (ImGui::IsKeyPressed(ImGuiKey_3))
	{
		CUI::UIKEYFRAME tUIKeyframe;
		tUIKeyframe.vPos.x = 1.f;
		tUIKeyframe.vPos.y = 33.f;

		m_vecTimeline.push_back(tUIKeyframe);
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
			m_tChild_Desc.fPositionX = (_float)m_pt.x;
			m_tChild_Desc.fPositionY = (_float)m_pt.y;
			Create_Child(m_tChild_Desc);
			
		}
	}

	/* Control_L */
	if (m_pGameInstance->Key_Pressing(DIK_LCONTROL))
	{
		if (m_pGameInstance->Mouse_Pressing(DIM_LB))
		{
			if (m_pCurrSelectUI != nullptr)
				m_pCurrSelectUI->Moving_Picking_Point(m_pt);
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

	/* Ű�Է��ϰ� Ŭ���ϴ� ��ɺ��� �Ʒ� �����ϰ� �ؾ��Ѵ�. */
	if (m_pGameInstance->Mouse_Down(DIM_RB))
	{
		if (!m_vecParentObject.empty())
		{
			_int iParentSize = m_vecParentObject.size();
			for (_int i = 0; i < iParentSize; ++i)
			{
				if (dynamic_cast<CUI*>(m_vecParentObject[i])->Get_Pick())
				{
					m_iSelected_ParentObjectIndex = i;
					m_pCurrParent = dynamic_cast<CUI*>(m_vecParentObject[i]);
					m_pCurrSelectUI = m_pCurrParent;
					m_eUIType = PARENT;
					return;
				}
			}
		}
		if (!m_vecChildObject.empty())
		{
			_int iChildSize = m_vecChildObject.size();
			for (_int i = 0; i < iChildSize; ++i)
			{
				if (dynamic_cast<CUI*>(m_vecChildObject[i])->Get_Pick())
				{
					if (dynamic_cast<CUI*>(m_vecChildObject[i])->Get_Kind() == CUI::TEXT)
					{
						m_iSelected_ChildObjectIndex = i;
						m_pCurrChild = dynamic_cast<CUI*>(m_vecChildObject[i]);
						m_pCurrSelectUI = dynamic_cast<CUI_Text*>(m_pCurrChild);
						m_eUIType = TEXT;
						return;
					}

					m_iSelected_ChildObjectIndex = i;
					m_pCurrChild = dynamic_cast<CUI*>(m_vecChildObject[i]);
					m_pCurrSelectUI = m_pCurrChild;
					m_eUIType = CHILD;
					return;
				}
			}
		}
		if (m_vecParentGroup != nullptr)
		{
			_int iGroupSize = (*m_vecParentGroup).size();
			for (_int i = 0; i < iGroupSize; ++i)
			{
				if (dynamic_cast<CUI*>((*m_vecParentGroup)[i])->Get_Pick())
				{
					m_iSelected_GroupObjectIndex = i;
					m_pCurrGroup = dynamic_cast<CUI*>((*m_vecParentGroup)[i]);
					m_pCurrSelectUI = m_pCurrGroup;
					m_eUIType = GROUP;
					return;
				}
			}
		}
		m_pCurrSelectUI = nullptr;
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

	///* Mod */
	//ImGui::SeparatorText(u8"���� ��� ����");
	//ImGui::RadioButton("Scale", &m_iChangeType, 1);
	//ImGui::RadioButton("Rotation", &m_iChangeType, 2);
	//ImGui::RadioButton("Position", &m_iChangeType, 3);

	///* Scale */
	//ImGui::SeparatorText(u8"ũ�� ����");
	//ImGui::InputFloat("ScaleX", &m_fParent_Scale.x);
	//ImGui::InputFloat("ScaleY", &m_fParent_Scale.y);
	//m_tParent_Desc.fScaleX = m_fParent_Scale.x;
	//m_tParent_Desc.fScaleY = m_fParent_Scale.y;

	///* Rotation */
	//ImGui::SeparatorText(u8"ȸ�� ����");
	//ImGui::InputFloat("RotationZ", &m_fParent_Rotation.z);
	//m_tParent_Desc.fRotationZ = m_fParent_Rotation.z;

	///* Position*/
	//ImGui::SeparatorText(u8"��ġ ����");
	//ImGui::InputFloat("PositionX", &m_fParent_Position.x);
	//ImGui::InputFloat("PositionY", &m_fParent_Position.y);
	//m_tParent_Desc.fPositionX = m_fParent_Position.x;
	//m_tParent_Desc.fPositionY = m_fParent_Position.y;


	ImGui::Separator();


	if (m_pCurrParent != nullptr)
	{
		_vector vPosition = dynamic_cast<CUI*>(m_pCurrParent)->Get_Transform()->Get_State(CTransform::STATE_POSITION);
		_vector vOrthoPos = m_pGameInstance->Convert_Orthogonal(vPosition);

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

	PATHINFO* tChild_Desc = new PATHINFO;

	// ���ڿ� �ߺ� ��
	m_tParent_Desc.strObjectName;
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

	if (m_pCurrSelectUI)
		m_pCurrSelectUI = nullptr; // ���� ���õ��ִ� �༮�� �׾����� �ּ� �������.
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
				m_pCurrSelectUI = m_pCurrParent;
#ifdef _DEBUG
				/* (�����̳��� �ּҸ� �޾ƿ��°� ������ ��忡�� ������ ���װ�����. ����) */
				m_vecParentGroup = m_pCurrParent->Get_vecUIParts();
#endif // DEBUG
				m_iSelected_GroupObjectIndex = 0; // ���� ���������ϱ� �ʱ�ȭ ������.
				m_eUIType = PARENT;
			}
			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}
	ImGui::Spacing();

	if (ImGui::Button("Group_Delete"))
	{
		Delete_Group();
	}
	
	if (m_vecParentGroup != nullptr)
	{
		/* �������� ���� */
		if (!m_vecParentGroup->empty())
		{
			_int		iGroupTagSize = (_int)m_vecParentGroup->size();

			if (ImGui::BeginListBox("Group Object"))
			{
				for (_int i = 0; i < iGroupTagSize; i++)
				{
					string Name = (*m_vecParentGroup)[i]->Get_ObjectNameTag();
					const bool is_selected = (m_iSelected_GroupObjectIndex == i);
					if (ImGui::Selectable(ConverWStringtoC(ConvertToWideString((*m_vecParentGroup)[i]->Get_ObjectNameTag())), is_selected))
					{
						//m_iSelected_GroupObjectIndex = is_selected; // bug : is_selected�� bool�ε� �ε����� �ְ��־���..
						m_iSelected_GroupObjectIndex = i;
						m_pCurrGroup = (*m_vecParentGroup)[m_iSelected_GroupObjectIndex];
						m_pCurrSelectUI = m_pCurrGroup;
						m_eUIType = GROUP;
					}
					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndListBox();
			}
		}
	}
	else
	{
		ImGui::Text("Not Group");
	}
	ImGui::Spacing();
}

void CWindow_UITool::Setting_Child()
{
	ImGui::CollapsingHeader("2D_Setting");

	///* Mod */
	//ImGui::SeparatorText(u8"���� ��� ����");
	//ImGui::RadioButton("Scale", &m_iChangeType, 1);
	//ImGui::RadioButton("Rotation", &m_iChangeType, 2);
	//ImGui::RadioButton("Position", &m_iChangeType, 3);

	///* Rotation */
	//ImGui::SeparatorText(u8"ȸ�� ����");
	//ImGui::InputFloat("ScaleX", &m_fParent_Scale.x);
	//ImGui::InputFloat("ScaleY", &m_fParent_Scale.y);

	///* Scale */
	//ImGui::SeparatorText(u8"ũ�� ����");
	//ImGui::InputFloat("ScaleX", &m_fChild_Scale.x);
	//ImGui::InputFloat("ScaleY", &m_fChild_Scale.y);

	///* Position*/
	//ImGui::SeparatorText(u8"��ġ ����");
	//ImGui::InputFloat("PositionX", &m_fChild_Possition.x);
	//ImGui::InputFloat("PositionY", &m_fChild_Possition.y);

	ImGui::Separator();

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

	ImGui::Separator();

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
	_int		ObjectTagSize = (_int)m_vecChildObject.size();
	if (ImGui::BeginListBox("Child Object"))
	{
		for (_int i = 0; i < ObjectTagSize; i++)
		{
			const bool is_selected = (m_iSelected_ChildObjectIndex == i);
			if (ImGui::Selectable(ConverWStringtoC(ConvertToWideString(dynamic_cast<CUI*>(m_vecChildObject[i])->Get_ObjectNameTag())), is_selected))
			{
				m_iSelected_ChildObjectIndex = i;
				m_pCurrChild = dynamic_cast<CUI*>(m_vecChildObject[m_iSelected_ChildObjectIndex]);
				if (m_pCurrChild->Get_Kind() == CUI::TEXT)
				{
					m_pCurrSelectUI = dynamic_cast<CUI_Text*>(m_pCurrChild);
					m_eUIType = TEXT;
				}
				else
				{
					m_pCurrSelectUI = m_pCurrChild;
					m_eUIType = CHILD;
				}
				
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
	//m_vecParentGroup.push_back(pChild);
	//if (!m_vecChildObjectName.empty())
	//	m_vecChildObjectName.erase(m_vecChildObjectName.begin() + m_iSelected_ChildObjectIndex); // ������Ʈ ��� �̸� ����
	if (!m_vecChildObject.empty())
		m_vecChildObject.erase(m_vecChildObject.begin() + m_iSelected_ChildObjectIndex); // ������Ʈ ����

	if (m_iSelected_ChildObjectIndex > 0 &&
		m_iSelected_ChildObjectIndex <= m_vecChildObject.size())
		--m_iSelected_ChildObjectIndex;
}

void CWindow_UITool::Delete_Group()
{
	if (m_vecParentGroup == nullptr)
		return;

	if (m_vecParentGroup->empty())
		return;

	//m_vecChildObjectName.push_back(tChild_Desc); // �̸� �ߺ� �˻� �� ó���� �ױװ����� �־�����.
	if((*m_vecParentGroup)[m_iSelected_GroupObjectIndex] != nullptr)
		m_vecChildObject.push_back((*m_vecParentGroup)[m_iSelected_GroupObjectIndex]); //


	if (!(*m_vecParentGroup).empty())
	{
		(*m_vecParentGroup).erase((*m_vecParentGroup).begin() + m_iSelected_GroupObjectIndex); // ������Ʈ �׷쿡�� ����

		if (m_iSelected_GroupObjectIndex > 0 &&
			m_iSelected_GroupObjectIndex <= m_vecParentGroup->size())
		{
			--m_iSelected_GroupObjectIndex;
		}
	}

	if (m_pCurrSelectUI)
		m_pCurrSelectUI = nullptr; // ���� ���õ��ִ� �༮�� �׾����� �ּ� �������.


	//(*m_vecParentGroup)[m_iSelected_GroupObjectIndex]->Set_Dead(true);
}
#pragma endregion

HRESULT CWindow_UITool::Create_Child(CUI::UI_DESC pUIDesc)
{
	CGameObject* pGameObject = nullptr;
	CUI* pUI = nullptr;
	wstring strCloneProto = TEXT("");
	m_pGameInstance->String_To_WString(m_tChild_Desc.strCloneTag, strCloneProto);

	Add_ChildList(pUIDesc); // ���� �ߺ��˻� �� �̸� �������ֱ�
	pUIDesc.iObjectNum = m_tChild_Desc.iObjectNum;
	pUIDesc.iShaderNum = m_tChild_Desc.iShaderNum;
	pUIDesc.strObjectName = m_tChild_Desc.strObjectName;
	pUIDesc.strCloneTag = m_tChild_Desc.strCloneTag;
	pUIDesc.strFilePath = m_tChild_Desc.strFilePath;
	pUIDesc.strLayerTag = m_strLayer[m_iCurrLayerNum];
	pUIDesc.strProtoTag = m_tChild_Desc.strProtoTag;
	pUIDesc.strMapTextureTag = m_tChild_Desc.strMapTextureTag;
	pUIDesc.bParent = false;
	pUIDesc.bWorld = m_tChild_Desc.bWorld;
	CUI* pCurrObject = dynamic_cast<CUI*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_STATIC, ConvertToWideString(m_strLayer[m_iCurrLayerNum]), strCloneProto, &pUIDesc));
	m_vecChildObject.push_back(pCurrObject);
	m_pCurrChild = pCurrObject;

	if(m_pCurrChild->Get_Kind() == CUI::TEXT)
		m_tTextInfo = dynamic_cast<CUI_Text*>(m_pCurrChild)->Get_TextInfo();

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

	//if(!m_vecChildObjectName.empty())
	//	m_vecChildObjectName.erase(m_vecChildObjectName.begin() + m_iSelected_ChildObjectIndex); // ������Ʈ ��� �̸� ����

	if(m_pCurrChild)
		m_pCurrChild = nullptr; // ���� ���õ��ִ� �༮�� �׾����� �ּ� �������.

	if (m_pCurrSelectUI)
		m_pCurrSelectUI = nullptr; // ���� ���õ��ִ� �༮�� �׾����� �ּ� �������.

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
	UI_Info.strObjectName = m_vecParent[m_iSelected_ParentClassIndex].c_str() + to_string(index);
	m_tParent_Desc.strObjectName = UI_Info.strObjectName;
	UI_Info.iPathNum = index;
}

//										���� ������ �༮�� ��ü ��θ� �޾ƿ´�.
void CWindow_UITool::Add_ChildIndexNumber(PATHINFO& UI_Info)
{
	int		index = 0;
	_bool	isPath = false;

	if (!m_vecChildObject.empty())
	{
		for (auto& iter : m_vecChildObject)
		{
			if (dynamic_cast<CUI*>(iter)->Get_FilePathTag() == UI_Info.strFilePath)
			{
				isPath = true;
				break;
			}
		}
		// ���ڿ��� �̹� �����ϴ� ���
		if (isPath)
		{
			for (auto& strFilePath : m_vecChildObject)
			{
				if (dynamic_cast<CUI*>(strFilePath)->Get_FilePathTag() == UI_Info.strFilePath)
				{
					/* ��� ��ȸ�ϰ� */
					if (dynamic_cast<CUI*>(strFilePath)->Get_ObjectNum() >= UI_Info.iPathNum) // ���� �����̳ʿ� �ִ� ���������� ���� �༮�� ���ں��� �۰ų� ���� ���
					{
						index = dynamic_cast<CUI*>(strFilePath)->Get_ObjectNum() + 1; // ������ �༮�� ��ȣ + 1�� ����
					}

				}
			}
		}
	}
	else 
	{
		// ���ο� ���ڿ��� ���
		index = 0;
		m_tChild_Desc.iObjectNum = 0;
	}

	{
		m_tChild_Desc.iObjectNum = index;
		UI_Info.strFileName = m_vecClass[m_iSelected_ChildClassIndex] + to_string(index);
		UI_Info.strObjectName = m_vecClass[m_iSelected_ChildClassIndex] + to_string(index);
		m_tChild_Desc.strObjectName = UI_Info.strObjectName;
		UI_Info.iPathNum = index;
	}
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

	//m_vecChildObjectName.push_back(tChild_Desc); // �̸� �ߺ� �˻� �� ó���� �ױװ����� �־�����.
	//m_pGameInstance->Get_CloneGameObjects(LEVEL_STATIC, &m_vecChildObject);

	tChild_Desc = nullptr;
	delete[] tChild_Desc;
}

void CWindow_UITool::Text_List()
{
	/* bug : Label �̸� �ߺ��Ǹ� �� ���������ϱ� Ȯ�����ֱ� */

	if (ImGui::InputTextWithHint(u8"Ű��", u8"Ű���� �Է��ϼ���.", ConverWStringtoC(ConvertToWideString(m_tTextInfo.strTextKey)), IM_ARRAYSIZE(ConverWStringtoC(ConvertToWideString(m_tTextInfo.strTextKey)))))
		m_tTextInfo.strTextKey = m_tTextInfo.strTextKey;

	//ImGui::Text(m_tTextInfo.strTextKey);

	if (ImGui::InputTextWithHint(u8"��Ʈ", u8"��Ʈ�� �Է��ϼ���.", ConverWStringtoC(ConvertToWideString(m_tTextInfo.strFontTag)), IM_ARRAYSIZE(ConverWStringtoC(ConvertToWideString(m_tTextInfo.strFontTag)))))
		m_tTextInfo.strFontTag = m_tTextInfo.strFontTag;
	//ImGui::Text(m_cTextFont);

	if(ImGui::InputTextWithHint(u8"�ؽ�Ʈ", u8"�ؽ�Ʈ�� �Է��ϼ���.", ConverWStringtoC(ConvertToWideString(m_tTextInfo.strText)), IM_ARRAYSIZE(ConverWStringtoC(ConvertToWideString(m_tTextInfo.strText)))))
		m_tTextInfo.strText = m_tTextInfo.strText;
	//ImGui::Text(m_cText);

	if (ImGui::Checkbox("Free Move", &m_bFreeMove))
	{
		dynamic_cast<CUI_Text*>(m_pCurrSelectUI)->Set_FreeMove(m_bFreeMove);
	}

	if (ImGui::DragFloat("Text PositionX", &m_tTextInfo.fPosX))
	{
		m_tTextInfo.fPosX = m_tTextInfo.fPosX;
	}

	if (ImGui::DragFloat("Text PositionY", &m_tTextInfo.fPosY))
	{
		m_tTextInfo.fPosY = m_tTextInfo.fPosY;
	}
	if (ImGui::DragFloat("Text Scale", &m_tTextInfo.fScale))
	{
		m_tTextInfo.fScale = m_tTextInfo.fScale;
	}
	if (ImGui::DragFloat("Text OriginX", &m_tTextInfo.vOrigin.x))
		m_tTextInfo.vOrigin.x = m_tTextInfo.vOrigin.x;
	if (ImGui::DragFloat("Text OriginY", &m_tTextInfo.vOrigin.y))
		m_tTextInfo.vOrigin.y = m_tTextInfo.vOrigin.y;
	if (ImGui::DragFloat("Text Rotation", &m_tTextInfo.fRotation))
		m_tTextInfo.fRotation = m_tTextInfo.fRotation;

	if (ImGui::DragFloat("Text ColorR", &m_tTextInfo.vColor.m128_f32[0]))
		m_tTextInfo.vColor.m128_f32[0] = m_tTextInfo.vColor.m128_f32[0];
	if (ImGui::DragFloat("Text ColorG", &m_tTextInfo.vColor.m128_f32[1]))
		m_tTextInfo.vColor.m128_f32[1] = m_tTextInfo.vColor.m128_f32[1];
	if (ImGui::DragFloat("Text ColorB", &m_tTextInfo.vColor.m128_f32[2]))
		m_tTextInfo.vColor.m128_f32[2] = m_tTextInfo.vColor.m128_f32[2];
	if (ImGui::DragFloat("Text ColorA", &m_tTextInfo.vColor.m128_f32[3]))
		m_tTextInfo.vColor.m128_f32[3] = m_tTextInfo.vColor.m128_f32[3];

	/* ���� */
	Change_Text(m_tTextInfo.strTextKey, m_tTextInfo.strFontTag, m_tTextInfo.strText, m_tTextInfo.fPosX, m_tTextInfo.fPosY, m_tTextInfo.vColor, m_tTextInfo.fScale, m_tTextInfo.vOrigin, m_tTextInfo.fRotation);

	if (ImGui::Button("Text Add"))
	{
		Text_Add();
	}

	ImGui::Spacing();

	if (!m_vecTextList.empty())
	{
		/* ������ �ε��� ���� */
		_int		iTextListSize = (_int)m_vecTextList.size();

		if (ImGui::BeginListBox("Text List"))
		{
			for (_int i = 0; i < iTextListSize; i++)
			{
				const bool is_selected = (m_iSelected_TextListIndex == i);
				if (ImGui::Selectable(ConverWStringtoC(ConvertToWideString(m_vecTextList[i].c_str())), is_selected))
				{
					m_iSelected_TextListIndex = i;
				}
				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}
		ImGui::Spacing();
	}

	if (ImGui::InputTextWithHint(u8"�ؽ�Ʈ �ױ� : ", u8"�����ϰ���� �ؽ�Ʈ �ױ� ���� �Է��ϼ���.", m_cChangeTextTag, IM_ARRAYSIZE(m_cChangeTextTag)))
		ImGui::Text(m_cChangeTextTag);

	if (ImGui::Button("Find Change"))
	{
		Find_Change(m_cChangeTextTag);
	}
}

void CWindow_UITool::Text_Add()
{
	if (m_pCurrSelectUI == nullptr)
		return;

	dynamic_cast<CUI_Text*>(m_pCurrSelectUI)->Add_Text(m_tTextInfo.strTextKey, m_tTextInfo.strFontTag, m_tTextInfo.strText,
														m_tTextInfo.fPosX, m_tTextInfo.fPosY, m_tTextInfo.vColor,
														m_tTextInfo.fScale, m_tTextInfo.vOrigin, m_tTextInfo.fRotation);
	m_vecTextList.push_back(m_tTextInfo.strTextKey); // �ױ� ���
}

void CWindow_UITool::Find_Change(string strTextTag)
{
	if (m_pCurrSelectUI == nullptr)
		return;

	dynamic_cast<CUI_Text*>(m_pCurrSelectUI)->Find_Change(strTextTag);
	m_tTextInfo = *dynamic_cast<CUI_Text*>(m_pCurrSelectUI)->Find_Text(strTextTag);

	*m_cTextFont = *ConverWStringtoC(ConvertToWideString(m_tTextInfo.strFontTag));
	*m_cText = *ConverWStringtoC(ConvertToWideString(m_tTextInfo.strText));
	*m_cTextKey = *ConverWStringtoC(ConvertToWideString(m_tTextInfo.strTextKey));

	m_fPosX = m_tTextInfo.fPosX;
	m_fPosY = m_tTextInfo.fPosY;
	m_fScale = m_tTextInfo.fScale;
	m_vOrigin.x = m_tTextInfo.vOrigin.x;
	m_vOrigin.y = m_tTextInfo.vOrigin.y;
	m_fRotation = m_tTextInfo.fRotation;

	m_vColor.m128_f32[0] = m_tTextInfo.vColor.m128_f32[0];
	m_vColor.m128_f32[1] = m_tTextInfo.vColor.m128_f32[1];
	m_vColor.m128_f32[2] = m_tTextInfo.vColor.m128_f32[2];
	m_vColor.m128_f32[3] = m_tTextInfo.vColor.m128_f32[3];
}

void CWindow_UITool::Add_TextList(CUI::UI_DESC tUIDesc)
{

	// ���ڿ� �ߺ� ��
	int		index = 0;
	_bool	isPath = false;

	if (!m_vecChildObject.empty())
	{
		for (auto& iter : m_vecChildObject)
		{
			if (dynamic_cast<CUI*>(iter)->Get_FilePathTag() == tUIDesc.strFilePath)
			{
				isPath = true;
				break;
			}
		}
		// ���ڿ��� �̹� �����ϴ� ���
		if (isPath)
		{
			for (auto& strFilePath : m_vecChildObject)
			{
				if (dynamic_cast<CUI*>(strFilePath)->Get_FilePathTag() == tUIDesc.strFilePath)
				{
					index = dynamic_cast<CUI*>(strFilePath)->Get_ObjectNum() + 1; // ������ �༮�� ��ȣ + 1�� ����
				}
			}
		}
	}
	else
	{
		// ���ο� ���ڿ��� ���
		index = 0;
		m_tChild_Desc.iObjectNum = 0;
	}

	{
		tUIDesc.iObjectNum = index;
		tUIDesc.strObjectName = m_vecClass[m_iSelected_ChildClassIndex] + to_string(index);
	}

}

void CWindow_UITool::Change_Text(string strTextKey, string strFontTag, string strText, _float fPosX, _float fPosY, _vector vColor, _float fScale, _float2 vOrigin, _float fRotation)
{
	if (!m_pCurrSelectUI)
		return;

	if (m_eUIType == CWindow_UITool::TEXT)
	{
		dynamic_cast<CUI_Text*>(m_pCurrSelectUI)->Change_Text(strTextKey, strFontTag, strText, fPosX, fPosY, vColor, fScale, vOrigin, fRotation);
	}
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
				m_strDialogPath = "../Bin/DataFiles/Data_UI/";
				m_eDialogType = CImgui_Window::SAVE_DIALOG;
				OpenDialog(CImgui_Window::IMGUI_UITOOL_WINDOW);
			}
			if (ImGui::MenuItem("Load"))
			{
				m_strDialogPath = "../Bin/DataFiles/Data_UI/";
				m_eDialogType = CImgui_Window::LOAD_DIALOG;
				OpenDialog(CImgui_Window::IMGUI_UITOOL_WINDOW);
			}
			if (ImGui::MenuItem("Target_Save"))
			{
				Create_TargetTexture();
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

	switch (m_eUIType)
	{
	case Client::CWindow_UITool::CHILD:
	{
		if (m_pCurrSelectUI != nullptr)
		{
			if (!m_vecChildObject.empty())
			{
				pTransformCom = dynamic_cast<CUI*>(m_pCurrSelectUI)->Get_Transform();
				ImGui::Text("Selected Child : ");
				ImGui::TextColored({ 1.f, 1.f, 1.f, 1.f }, dynamic_cast<CUI*>(m_vecChildObject[m_iSelected_ChildObjectIndex])->Get_ObjectNameTag().c_str());
				ImGui::SameLine();
				if (ImGui::Button("Parts_Add"))
				{
					Add_Parts(m_pCurrSelectUI); // �θ�  <- �ڽ�����
				}
			}
		}
		break;
	}
	case Client::CWindow_UITool::PARENT:
	{
		if (m_pCurrSelectUI != nullptr)
		{
			if (!m_vecParentObjectName.empty())
			{
				pTransformCom = dynamic_cast<CUI*>(m_pCurrSelectUI)->Get_Transform();
				ImGui::Text("Selected Parent : ");
				ImGui::TextColored({ 1.f, 1.f, 1.f, 1.f }, m_vecParentObjectName[m_iSelected_ParentObjectIndex]->strFileName.c_str());
			}
		}
		break;
	}
	case Client::CWindow_UITool::GROUP:
	{
		if (m_pCurrSelectUI != nullptr)
		{
			if (m_vecParentGroup == nullptr)
				break;

			if (!m_vecParentGroup->empty())
			{
				pTransformCom = dynamic_cast<CUI*>(m_pCurrSelectUI)->Get_Transform();
				ImGui::Text("Group Child : ");
				if (m_iSelected_GroupObjectIndex < (*m_vecParentGroup).size() &&
					m_iSelected_GroupObjectIndex >= 0)
				{
					if (m_iSelected_GroupObjectIndex >= (*m_vecParentGroup).size())
						m_iSelected_GroupObjectIndex = (*m_vecParentGroup).size() - 1;
					if (m_iSelected_GroupObjectIndex < 0)
						m_iSelected_GroupObjectIndex = 0;

					ImGui::TextColored({ 1.f, 1.f, 1.f, 1.f }, ConverWStringtoC(ConvertToWideString((*m_vecParentGroup)[m_iSelected_GroupObjectIndex]->Get_ObjectNameTag())));

				}
				else
				{
					ImGui::Text("Not Selected");
				}
				ImGui::SameLine();
			}
		}
		break;
	}
	case Client::CWindow_UITool::TEXT:
	{
		if (m_pCurrSelectUI != nullptr)
		{
			string strName = dynamic_cast<CUI_Text*>(m_pCurrSelectUI)->Get_ObjectNameTag();

			ImGui::Text(ConverWStringtoC(ConvertToWideString(strName)));

			Text_List();
		}
		else
		{
		ImGui::Text("Not Selected");
		}
		break;
	}
	case Client::CWindow_UITool::TYPE_END:
	{
		if (m_pCurrSelectUI == nullptr)
			ImGui::Text("Not Selected");
		break;
	}
	default:
		break;
	}

	if (pTransformCom != nullptr)
	{
		_vector vPosition = pTransformCom->Get_State(CTransform::STATE_POSITION);
		_vector vRotation = pTransformCom->Get_Rotated();
		_vector vScale = pTransformCom->Get_Scaled();

		_vector vOrthoPos = m_pGameInstance->Convert_Orthogonal(vPosition);

		ImGui::InputFloat("PositionX", &vPosition.m128_f32[0]);
		ImGui::InputFloat("PositionY", &vPosition.m128_f32[1]);
		ImGui::InputFloat("PositionZ", &vPosition.m128_f32[2]);

		ImGui::InputFloat("RotationX", &vRotation.m128_f32[0]);
		ImGui::InputFloat("RotationY", &vRotation.m128_f32[1]);
		ImGui::InputFloat("RotationZ", &vRotation.m128_f32[2]);

		ImGui::InputFloat("ScaleX", &vScale.m128_f32[0]);
		ImGui::InputFloat("ScaleY", &vScale.m128_f32[1]);
		ImGui::InputFloat("ScaleZ", &vScale.m128_f32[2]);
	}

	/* �̸����� */
	if(!m_vecTexture.empty())
		ImGui::Image((void*)m_vecTexture[m_iSelectedPathIndex]->SRV_Texture, ImVec2((_float)m_vecTexture[m_iSelectedPathIndex]->iImage_Width, (_float)m_vecTexture[m_iSelectedPathIndex]->iImage_Height));
}

void CWindow_UITool::Create_TargetTexture()
{
	/* error : Find�Լ��� ����Ÿ���� ã�ƿ� ��, �׳༮���� Create�Լ��� ȣ���ϸ� �ܺ�������ȣ ������ �߻���.. ���� �����ν��Ͻ��� Create���� ����Ǵ� �Լ��� ���� ���� ����ϸ� �ذ� */
	m_pGameInstance->Create_RenderTarget(TEXT("Target_UI_Diffuse"));
}

void CWindow_UITool::KeyframeList()
{

}

void CWindow_UITool::KeyframeChangeWindow()
{

}

void CWindow_UITool::KeyframeRender_ValueChange()
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList(); // �׸���

	if (m_vecTimeline.empty())
		return;

	// Ű������ ������ �� ����
	for (int i = 0; i < m_vecTimeline.size(); ++i)
	{
		//CUI::UIKEYFRAME& keyframe = timeline[m_eKeyType][i];

		// Ÿ�� ���� ���� ��ġ�� ���
		float xPos = timelinePos.x + (m_vecTimeline[i].fTime / 20.0f) * timelineSize.x;
		float yPos = timelinePos.y + timelineSize.y - timelineSize.y * m_vecTimeline[i].fValue;

		// Ű������ ��ġ ���� 0(�ʱⰪ)�� ���
		if (m_vecTimeline[i].vKeyFramePos.x <= 1 &&
			m_vecTimeline[i].vKeyFramePos.y <= 1)
		{
			// ���� ���õ� Ű������ ��ġ�� �־��ش�.
			m_vecTimeline[i].vKeyFramePos.x = xPos;
			m_vecTimeline[i].vKeyFramePos.y = yPos;
		}

		// ���� �ð��� �ش��ϴ� Ű������ ǥ�� (������ ��)
		if (m_vecTimeline[i].fTime >= currentTime - 0.1 &&
			m_vecTimeline[i].fTime <= currentTime + 0.1) {
			draw_list->AddCircleFilled(
				ImVec2(xPos, yPos),
				5.0f,
				IM_COL32(255, 0, 0, 255) // ���������� ǥ��
			);
		}
		else {
			// �ٸ� �ð��� Ű������ ǥ�� (����� ��)
			draw_list->AddCircleFilled(
				ImVec2(xPos, yPos),
				5.0f,
				IM_COL32(255, 255, 0, 255) // ��������� ǥ��
			);
		}

		// ���콺�� Ű������ ���� �ִ� ��쿡�� ���� ǥ�� (���콺 ����)
		if (ImGui::IsMouseHoveringRect(
			ImVec2(xPos - 5.0f, yPos - 5.0f),
			ImVec2(xPos + 5.0f, yPos + 5.0f)))
		{

			// Ű������ ���� ǥ��
			ImGui::SetCursorScreenPos(ImVec2(xPos + 10.0f, yPos - 20.0f));
			ImGui::BeginTooltip();
			ImGui::Text(u8"Ű������ %d", i);
			ImGui::Separator();
			ImGui::Text(u8"�ð� : %.2f", m_vecTimeline[i].fTime);
			ImGui::Text(u8"�� : %.2f", m_vecTimeline[i].fValue);
			ImGui::Text(u8"ũ�� X, Y : (%.2f, %.2f, %.2f)", m_vecTimeline[i].vScale.x, m_vecTimeline[i].vScale.y);
			ImGui::Text(u8"ȸ�� Z :(%.2f)", m_vecTimeline[i].fRot);
			ImGui::Text(u8"�̵� X, Y : (%.2f, %.2f, %.2f)", m_vecTimeline[i].vPos.x, m_vecTimeline[i].vPos.y);

			if (m_vecTimeline[i].isEaseIn)
				ImGui::Text(u8"��¡ In: ����");
			else
				ImGui::Text(u8"��¡ In: ����");
			if (m_vecTimeline[i].isEaseOut)
				ImGui::Text(u8"��¡ Out: ����");
			else
				ImGui::Text(u8"��¡ Out: ����");
			ImGui::EndTooltip();
		}

		// ���콺 ������ �� �巡�� ����
		if (ImGui::IsMouseReleased(0))
		{
			isDraggingTimeline = false;
		}

		if (!isDraggingTimeline) // �巡�׸� ���� ���� ��츸 ���� ����
		{
			// ������ ���콺 Ŭ������ Ű������ ����
			if (ImGui::IsMouseHoveringRect(
				ImVec2(xPos - 5.0f, yPos - 5.0f),
				ImVec2(xPos + 5.0f, yPos + 5.0f))
				&& ImGui::IsMouseClicked(1))
			{
				m_vecTimeline.erase(m_vecTimeline.begin() + i);
				break; // ������ �Ŀ� ������ ���������ϴ�.
			}
		}

	}
}

// �ּ�, �ִ� �� �� ũ��, ȸ��, �̵� ���� ������� ���� ������ Ű�������� �ڵ����� �����ϴ� �Լ�
void CWindow_UITool::CreateKeyframesWithLinearInterpolation(
	_float minTime, _float maxTime,
	_float minValue, _float maxValue,
	_float2 minScaleValue, _float2 maxScaleValue,
	_float minRotationValue, _float maxRotationValue,
	_float2 minTranslationValue, _float2 maxTranslationValue,
	_float _minTexture, _float _maxTexture,
	_int numKeyframes)
{
	if (numKeyframes < 2) {
		// �ּ� 2�� �̻��� Ű�������� �ʿ��մϴ�.
		return;
	}

	// Ű������ ���� �ð� ������ ���
	float timeStep = 1.0f / static_cast<float>(numKeyframes - 1);

	// ù ��°�� ������ Ű�������� �������� �߰�
	CUI::UIKEYFRAME firstKeyframe;

	// �ּ� �� ����
	firstKeyframe.fTime = minTime;
	firstKeyframe.fValue = minValue;
	firstKeyframe.vScale = minScaleValue; // Check CheckCheckCheckCheckCheckCheck
	firstKeyframe.fRot = minRotationValue;
	firstKeyframe.vPos = minTranslationValue;
	firstKeyframe.iTexureframe = _minTexture;

	m_vecTimeline.push_back(firstKeyframe);

	CUI::UIKEYFRAME lastKeyframe;

	// �ִ� �� ����
	lastKeyframe.fTime = maxTime;
	lastKeyframe.fValue = maxValue;
	lastKeyframe.vScale = maxScaleValue;
	lastKeyframe.fRot = maxRotationValue;
	lastKeyframe.vPos = maxTranslationValue;
	lastKeyframe.iTexureframe = _maxTexture;

	m_vecTimeline.push_back(lastKeyframe);

	int num_decimal_places = 2; // �Ҽ��� ���� ��

	// �Ҽ��� ���� ǥ��
	int multiplier = static_cast<int>(pow(10, num_decimal_places));


	// �߰� Ű������ ���� �� ����
	for (int i = 1; i < numKeyframes - 1; ++i)
	{
		float time = i * timeStep;
		float t = time;  // �ð��� ���� ����� ���

		int itextureRange = _maxTexture - _minTexture + 1; // �ؽ�ó ���� ���
		int iTextureNum = _minTexture + (i % itextureRange); // ���� �ε����� ���� �ؽ�ó �� ����

		// �� Ű�����Ӹ��� ���������� ���� �� ������ ���� �����Ͽ� �߰�
		float step = (maxValue - minValue) / (numKeyframes - 1); // ����/���� �ܰ�


		// ������ �Ӽ��� ���� �������� ���
		_float2 scale = Lerp_float2(minScaleValue, maxScaleValue, t);

		_float rotation = Lerp_float(minRotationValue, maxRotationValue, t);

		_float2 translation = Lerp_float2(minTranslationValue, maxTranslationValue, t);

		_float ftime = Lerp_float(minTime, maxTime, t);

		_float value = Lerp_float(minValue, maxValue, t);
		 
		_int iTexture = (_int)Lerp_float(_minTexture, _maxTexture, t);

		CUI::UIKEYFRAME keyframe;

		// �� ����
		keyframe.fTime = ftime;

		if (m_bRendomValue)
		{
			// ���������� ���� �� ������ �Ҽ��� �� ����
			if (i < numKeyframes / 2)
			{
				keyframe.fValue = minValue + i * step; // �ּҰ����� ����
			}
			else
			{
				keyframe.fValue = maxValue - (i - numKeyframes / 2) * step; // �ִ밪���� ����
			}
		}
		else
		{
			keyframe.fValue = value;
		}


		keyframe.vScale = scale;
		keyframe.fRot = rotation;
		keyframe.vPos = translation;

		//if (m_bIndividualTexture)
		//{
		//	keyframe.texureframe = iTextureNum;
		//}
		//else
		{
			keyframe.iTexureframe = iTexture;
		}

		// ���� timeline ���Ϳ� Ű�������� �̾ �߰�
		m_vecTimeline.emplace_back(keyframe);
	}

	// �ð��� ���� Ű������ ����
	std::sort(m_vecTimeline.begin(), m_vecTimeline.end(), [](const CUI::UIKEYFRAME& a, const CUI::UIKEYFRAME& b)
		{
			return a.fTime < b.fTime;
		});
}

void CWindow_UITool::KeyframeAutomaticGeneration()
{
	//ImGui::Begin(u8"Ű������ �ڵ� ����");

	ImGui::Text(u8"Ű������ �ڵ� ���� ����");

	// �Է°��� �޴� UI ��� ����
	ImGui::Dummy(ImVec2(0, 5)); // ����
	ImGui::SeparatorText(u8"�ð� ����");
	ImGui::InputFloat2(u8"����/�� �ð� �Է�", _v2Time);
	ImGui::Separator();

	//ImGui::Dummy(ImVec2(0, 5)); // ����
	ImGui::SeparatorText(u8"���� ����");
	ImGui::InputFloat2(u8"����/�� ���� �Է�", _v2Value);
	ImGui::Separator();

	ImGui::Dummy(ImVec2(0, 5)); // ����
	ImGui::SeparatorText(u8"ũ�� ����");
	ImGui::InputFloat2(u8"���� ������ ũ�� �Է�", minScale);

	//ImGui::Dummy(ImVec2(0, 5)); // ����
	ImGui::InputFloat2(u8"�� ������ ũ�� �Է�", maxScale);
	ImGui::Separator();

	ImGui::Dummy(ImVec2(0, 5)); // ����
	ImGui::SeparatorText(u8"ȸ�� ����");
	ImGui::InputFloat(u8"���� ������ ȸ�� �Է�", &minRot);

	//ImGui::Dummy(ImVec2(0, 5)); // ����
	ImGui::InputFloat(u8"�� ������ ȸ�� �Է�", &maxRot);
	ImGui::Separator();

	ImGui::Dummy(ImVec2(0, 5)); // ����
	ImGui::SeparatorText(u8"�̵� ����");
	ImGui::InputFloat2(u8"���� ������ �̵� �Է�", minPos);

	//ImGui::Dummy(ImVec2(0, 5)); // ����
	ImGui::InputFloat2(u8"�� ������ �̵� �Է�", maxPos);
	ImGui::Separator();

	ImGui::Dummy(ImVec2(0, 5)); // ����
	ImGui::SeparatorText(u8"�ؽ�ó ����");
	ImGui::InputFloat(u8"���� ������ �ؽ�ó �Է�", &fMin_Texture);

	//ImGui::Dummy(ImVec2(0, 5)); // ����
	ImGui::InputFloat(u8"�� ������ �ؽ�ó �Է�", &fMax_Texture);
	ImGui::Separator();

	ImGui::Dummy(ImVec2(0, 5)); // ����
	ImGui::SeparatorText(u8"���ϴ� Ű������ �� ����");
	ImGui::InputFloat(u8"Ű������ �� �Է�", &numKeyframes);
	ImGui::Separator();

	ImGui::Dummy(ImVec2(0, 5)); // ����
	ImGui::SeparatorText(u8"���� �ؽ�ó ����");
	ImGui::Checkbox(u8"���� �ؽ�ó", &m_bIndividualTexture);
	ImGui::Separator();

	ImGui::Dummy(ImVec2(0, 5)); // ����
	ImGui::SeparatorText(u8"���� ���� ����");
	ImGui::Checkbox(u8"���� ����", &m_bRendomValue);
	ImGui::Separator();
	
	if (ImGui::Button(u8"�ڵ� ����")) {
		// ��ư�� Ŭ���Ǹ� timeline ���͸� ���� CreateKeyframesWithLinearInterpolation �Լ� ȣ��
		//timeline[m_eKeyType].clear();

		/* float2�� ��ȯ�ؼ� �Ѱ��ֱ� */
		_float2 fminScale = { minScale[0], minScale[1] };
		_float2 fmaxScale = { maxScale[0], maxScale[1] };
		_float2 fminPos = { minPos[0], minPos[1] };
		_float2 fmaxPos = { maxPos[0], maxPos[1] };

		CreateKeyframesWithLinearInterpolation(
			_v2Time[0], _v2Time[1],
			_v2Value[0], _v2Value[1],
			fminScale, fmaxScale,
			minRot, maxRot,
			fminPos, fmaxPos,
			fMin_Texture, fMax_Texture,
			numKeyframes);
	}

	if (ImGui::Button("clear"))
	{
		// ��ư�� Ŭ���Ǹ� timeline ���͸� ���� CreateKeyframesWithLinearInterpolation �Լ� ȣ��
		m_vecTimeline.clear();
	}
}

void CWindow_UITool::PlayAnimation(_float fTimeDelta)
{
	// �ִϸ��̼� ���/�Ͻ� ���� ��ư
	if (m_isPlayAnim) 
	{
		if (ImGui::Button(u8"����"))
		{
			m_isPlayAnim = false;
		}
	}
	else 
	{
		if (ImGui::Button(u8"���"))
		{
			m_isPlayAnim = true;
		}
	}

	ImGui::SameLine();

	if (ImGui::Button(u8"�ʱ�ȭ"))
	{
		m_fPlayTime = 0.0f;
		currentTime = 0.0f;
	}

	// �ִϸ��̼� �ð� ����
	if (m_isPlayAnim)
	{
		m_fPlayTime += m_fPlaybackSpeed * ImGui::GetIO().DeltaTime;
		currentTime += m_fPlaybackSpeed * ImGui::GetIO().DeltaTime;
		if (m_fPlayTime > MaxTime) // ���� �ð��� �ִ� �ð� �� ���� Ŭ ���
		{
			m_fPlayTime = MaxTime; // ���� �ð��� �ִ� �ð� ������ ����
			currentTime = MaxTime; // ���� �ð��� �ִ� �ð� ������ ����
			m_isPlayAnim = false;  // �÷��� ����
		}
	}
}

void CWindow_UITool::AnimationTimeLineBar()
{
#pragma region �ִϸ��̼� Ÿ�� ����
	// Ÿ�� ���� ������
	// ImDrawList* draw_list = ImGui::GetWindowDrawList(); // �׸���
	timelinePos = ImGui::GetCursorScreenPos();
	ImDrawList* draw_list = ImGui::GetWindowDrawList(); // �׸���
	// �ִϸ��̼� Ÿ�� ���� ��ġ ����
	timelinePos.x += 0.f;
	timelinePos.y += 30.f;

	// �ִϸ��̼� Ÿ�� ���� �׸���
	draw_list->AddRectFilled(
		timelinePos,
		ImVec2(timelinePos.x + timelineSize.x, timelinePos.y + timelineSize.y),
		IM_COL32(50, 50, 50, 255) // Ÿ�Ӷ��� ��� ����
	);

	// �ð�
	ImGui::Text(u8"�ð� : %.2f", currentTime);
	ImGui::SameLine(80.f);

	ImGui::PushItemWidth(150);
	// �ð� �׷���
	for (int n = 0; n < 100; n++)
		m_fFrame[n] = sinf(n * 0.2f + (_float)ImGui::GetTime() * m_fPlaybackSpeed);
	ImGui::PlotLines(u8"�ð� �׷���", m_fFrame, 100);
	ImGui::PopItemWidth();
	ImGui::SameLine();

	// ���� �ð��� �ּҿ� �ִ� �ð� ������ ��ȯ
	float minTime = 0.0f; // �ּ� �ð�
	//float maxTime = 100.0f; // �ִ� �ð�

	// �ִϸ��̼� Ÿ�� ���� ���� �׸��� (1���� ǥ�� ���� : �ִ� �ð� �� ��ŭ)
	for (int i = 0; i <= MaxTime; ++i)
	{
		// ���� �ð��� ����                                �ִ� �ð������� ���� ���� ����
		float xPos = timelinePos.x + i * (timelineSize.x / MaxTime);

		float relativeStartTimeXPos = timelinePos.x;    //�ִϸ��̼� Ÿ�� ���� �׷����� ����
		float relativeEndTimeXPos = timelinePos.x + timelineSize.x; // �ִϸ��̼� Ÿ�� ���� �׷����� ��

		// �׷��� ������ �迭�� ���� �ð��� �°� ������Ʈ
		for (int n = 0; n < 100; n++) 
		{
			float time = n / 100.0f * MaxTime;
			m_fFrame[n] = EvaluateAnimationAtTime(time);
		}

		// ���� �ð��� �ִ� �ð� ���� ���� Ŭ����
		currentTime = ImClamp(currentTime, 0.0f, MaxTime);

		// ���� �ð��� �ּҿ� �ִ� �ð� ������ ��ȯ
		float relativeCurrentTime = (currentTime - minTime) / (MaxTime - minTime);

		// ���� �ð� ������ �ִϸ��̼� Ÿ�� ���ο� �´� ��ġ���� ���
		float relativeCurrentTimeXPos = ImLerp(relativeStartTimeXPos, relativeEndTimeXPos, relativeCurrentTime);

		// ���� ��(���� �ð� ��ġ) �׸���
		draw_list->AddLine(
			ImVec2(relativeCurrentTimeXPos, timelinePos.y), // ���� ������
			ImVec2(relativeCurrentTimeXPos, timelinePos.y + timelineSize.y), // ���� ����
			IM_COL32(255, 0, 0, 255) // ���� ������ ���� �ð� ǥ��
		);

		// ���� �Ʒ��� ���ڷ� �ð� ǥ��
		float timeLabel = i * fDisplayTime * (MaxTime / MaxTime); // 0.1 �������� 0���� MaxTime���� ǥ��
		char timeLabelStr[16];
		snprintf(timeLabelStr, sizeof(timeLabelStr), "%.1f", timeLabel);
		ImVec2 textPos(xPos - 10, timelinePos.y + timelineSize.y + 5.f);
		draw_list->AddText(textPos, IM_COL32(255, 255, 255, 255), timeLabelStr);
	}

	// graphPos : �ִϸ��̼� Ÿ�� ���� �׷����� �׸��� ���� ���� ��ġ�� ��Ÿ��
	//  graphPos = ImVec2(���� �� �𼭸� ȭ����� ��ġx, y, Ÿ�Ӷ����� ũ��)
	ImVec2 graphPos = ImVec2(timelinePos.x, timelinePos.y + timelineSize.y + 10);

#pragma endregion
}

_float CWindow_UITool::EvaluateAnimationAtTime(float time)
{

	if (m_vecTimeline.empty())
	{
		// ���� Ű�������� ���ٸ� �⺻�� 0.0�� ��ȯ�ϰų�,
		// �ٸ� ó���� ����.
		return 0.0f;
	}

	// Ű�������� �ð��� ���� ����.
	std::sort(m_vecTimeline.begin(), m_vecTimeline.end(), [](const CUI::UIKEYFRAME& tFirst_UI, const CUI::UIKEYFRAME& tSecond_UI)
		{
			return tFirst_UI.fTime < tSecond_UI.fTime;
		});

	// ���� �ð��� ���� ����� ������ ������ Ű�������� ã��
	CUI::UIKEYFRAME* prevKeyframe = nullptr;
	CUI::UIKEYFRAME* nextKeyframe = nullptr;

	for (int i = 0; i < m_vecTimeline.size(); ++i)
	{
		if (m_vecTimeline[i].fTime <= time)
		{
			prevKeyframe = &m_vecTimeline[i];
		}
		if (m_vecTimeline[i].fTime >= time)
		{
			nextKeyframe = &m_vecTimeline[i];
			break;
		}
	}

	// ������ ���� Ű�������� ã�� ���ϸ� ù ��° Ű�������� ���.
	if (!prevKeyframe) {
		prevKeyframe = &m_vecTimeline[0];
	}

	// ���� Ű�������� ã�� ���ϸ� ������ Ű�������� ���.
	if (!nextKeyframe) {
		nextKeyframe = &m_vecTimeline.back();
	}

	// ������ ���� Ű������ ���̿��� �ð��� �����Ͽ� ���� ���.
	float t = (time - prevKeyframe->fTime) / (nextKeyframe->fTime - prevKeyframe->fTime);
	float value = Lerp_float(prevKeyframe->fValue, nextKeyframe->fValue, t);

	// ��¡(Easing)�� �����մϴ�.
	if (t < 0.5f) {
		if (prevKeyframe->isEaseIn) 
		{
			float easedT = t / 0.5f;
			value = ImEaseInQuad(prevKeyframe->fValue, nextKeyframe->fValue, easedT);
		}
	}
	else {
		if (nextKeyframe->isEaseOut) 
		{
			float easedT = (t - 0.5f) / 0.5f;
			value = ImEaseOutQuad(prevKeyframe->fValue, nextKeyframe->fValue, easedT);
		}
	}

	return value;
}

void CWindow_UITool::KeyframeValueChange(_float fTimeDelta)
{
#pragma region Ÿ�Ӷ��� Ű������ ����

	// Ÿ�Ӷ��ο��� Ű�������� �巡���Ͽ� ���� ����
	for (int i = 0; i < m_vecTimeline.size(); ++i)
	{
		CUI::UIKEYFRAME& keyframe = m_vecTimeline[i];
		//float xPos = timelinePos.x + timelineSize.x * ImClamp(keyframe.time, 0.0f, MaxTime);

		// Ÿ�� ���� ���� ��ġ�� ���
		float xPos = timelinePos.x + (keyframe.fTime / 20.0f) * timelineSize.x;
		float yPos = timelinePos.y + timelineSize.y - timelineSize.y * keyframe.fValue;

		if (ImGui::IsMouseHoveringRect(
			ImVec2(xPos - 5.0f, yPos - 5.0f), // ��ǥ ����
			ImVec2(xPos + 5.0f, yPos + 5.0f)) // ��ǥ ����
			&& ImGui::IsMouseClicked(0))
		{
			isDraggingKeyframe = true;
			draggingKeyframeIndex = i;
			initialKeyframeX = xPos; // �巡�׸� �����ϴ� ��ġ ����
		}
	}

	if (isDraggingKeyframe)
	{
		// Ű�������� �ð��� �巡���� ��ġ�� ����
		CUI::UIKEYFRAME& keyframe = m_vecTimeline[draggingKeyframeIndex];
		float xPos = ImGui::GetMousePos().x - timelinePos.x;

		// ���콺 �巡�׷� Ű�������� �ð��� ����
		float newXPos = ImGui::GetMousePos().x;
		float deltaTime = (newXPos - initialKeyframeX) / timelineSize.x * MaxTime;

		// Ŭ���� ��ġ�� �ش��ϴ� value ��
		keyframe.fValue = 1.0f - (ImGui::GetMousePos().y - timelinePos.y) / timelineSize.y;

		// �ð� ��
		m_vecTimeline[draggingKeyframeIndex].fTime += deltaTime;

		// �ð��� ���� �����ϰ� Ŭ������ ���� ����.
		// �巡�� ���� �� �ð��� ���� �� ���� (Ű������ time�� value, �ּ� ��, �ִ� ��);
		keyframe.fTime = ImClamp(keyframe.fTime, 0.0f, MaxTime); // �ִ� ���� MaxTime���� ����
		keyframe.fValue = ImClamp(keyframe.fValue, 0.0f, MaxTime); // �ִ� ���� MaxTime���� ����

		// ���� ��ġ�� ����
		initialKeyframeX = newXPos;

		// Ű������ ���� ����
		if (draggingKeyframeIndex > 0 && keyframe.fTime < m_vecTimeline[draggingKeyframeIndex - 1].fTime) {
			std::swap(m_vecTimeline[draggingKeyframeIndex], m_vecTimeline[draggingKeyframeIndex - 1]);
			draggingKeyframeIndex--;
		}
		else if (draggingKeyframeIndex < m_vecTimeline.size() - 1 && keyframe.fTime > m_vecTimeline[draggingKeyframeIndex + 1].fTime) {
			std::swap(m_vecTimeline[draggingKeyframeIndex], m_vecTimeline[draggingKeyframeIndex + 1]);
			draggingKeyframeIndex++;
		}
	}

#pragma endregion
}

void CWindow_UITool::ImGuiKeyInput()
{
	// "G" Ű : ���� ���콺 ��ġ�� Ű�������� ����
	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_G)))
	{
		// ���ο� Ű�������� �߰��� �� ���� �ð��� ���� ����� Ű�������� ã�´�.
		float minDist = FLT_MAX;
		int insertIndex = 0;

		for (int i = 0; i < m_vecTimeline.size(); ++i)
		{
			float dist = fabsf(currentTime - m_vecTimeline[i].fTime);

			if (dist < minDist)
			{
				minDist = dist;
				insertIndex = i;
			}
		}

		// ���� ���콺 ��ġ
		ImVec2 mousePos = ImGui::GetMousePos();

		// �ִϸ��̼� Ÿ�� ���� ��ġ
		ImVec2 CurtimelinePos = ImGui::GetCursorScreenPos();
		CurtimelinePos.x += 0.f;
		CurtimelinePos.y += 30.f;

		// ���콺 ��ġX�� �ִϸ��̼� Ÿ�� ���� ���� ������� ��ġ�� ��ȯ
		float relativeMouseX = (mousePos.x - CurtimelinePos.x) / timelineSize.x;

		// ���콺 ��ġ�� �ִϸ��̼� Ÿ�� ���� ���� ������� ��ġ�� ��ȯ
		float relativeMouseY = (mousePos.y - CurtimelinePos.y) / timelineSize.y;

		// ���� ���� ����մϴ�. ���� ���, ������� ���콺 ��ġ�� ���� ������ ����� �� ����
		float newValue = 0.7f - ImClamp(relativeMouseY, 0.0f, 1.0f); // ���� �� ������ 0���� 1�� Ŭ����

		// �ð� ���� ����Ͽ� ���ο� Ű�������� ����
		float newTime = relativeMouseX * MaxTime; // MaxTime�� �ִ� �ð� ��

		// ���ο� Ű�������� �����ϰ� ����.
		CUI::UIKEYFRAME newKeyframe = { newTime, newValue, 0, false, false };
		newKeyframe.fTime = newTime;
		newKeyframe.fValue = newValue;
		newKeyframe.vKeyFramePos.x = relativeMouseX;
		newKeyframe.vKeyFramePos.y = relativeMouseY;
		m_vecTimeline.push_back(newKeyframe);

		// ���ο� Ű�������� �ð��� ���� ����� Ű�������� ã�´�. ( �� ��ȣ �ε����� ���õ� �ϴ� ����)
		float minDistNew = FLT_MAX;
		int insertIndexNew = 0;

		for (int i = 0; i < m_vecTimeline.size(); ++i)
		{
			float dist = fabsf(newTime - m_vecTimeline[i].fTime);

			if (dist < minDistNew)
			{
				minDistNew = dist;

				insertIndexNew = i;
				closestKeyframeIndex = insertIndexNew;
			}
		}

	}

	// "B" Ű : ���� �ð� ��ġ�� Ű�������� ����
	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_B)))
	{
		// �ִϸ��̼� Ÿ�� ���� ��ġ
		ImVec2 CurtimelinePos = ImGui::GetCursorScreenPos();
		CurtimelinePos.x += 0.f;
		CurtimelinePos.y += 30.f;

		// ���콺 ��ġX�� �ִϸ��̼� Ÿ�� ���� ���� ������� ��ġ�� ��ȯ
		float relativeMouseX = (CurtimelinePos.x) / timelineSize.x;

		// ���콺 ��ġ�� �ִϸ��̼� Ÿ�� ���� ���� ������� ��ġ�� ��ȯ
		float relativeMouseY = (CurtimelinePos.y) / timelineSize.y;

		// ���ο� Ű�������� �����ϰ� �����մϴ�.
		CUI::UIKEYFRAME newKeyframe = { currentTime, 0.f, 0, false, false };
		newKeyframe.fTime = currentTime;
		newKeyframe.fValue = 0.f;
		newKeyframe.vKeyFramePos.x = relativeMouseX;
		newKeyframe.vKeyFramePos.y = relativeMouseY;
		m_vecTimeline.push_back(newKeyframe);

		float minDist = FLT_MAX;
		int insertIndex = 0;

		for (int i = 0; i < m_vecTimeline.size(); ++i)
		{
			float dist = fabsf(currentTime - m_vecTimeline[i].fTime);

			if (dist < minDist)
			{
				minDist = dist;
				insertIndex = i;
				closestKeyframeIndex = insertIndex;
			}
		}
	}

	// "R" Ű : ���� �ð�(�ð���)�� 0���� �ʱ�ȭ [ó������ �̵�]
	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_R)))
	{
		currentTime = 0.f;
	}

	// "T" Ű : ���� �ð�(�ð� ��)�� ���콺 ��ġ�� ����
	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_T)))
	{
		// ���� ���콺 ��ġ
		ImVec2 mousePos = ImGui::GetMousePos();

		// ���콺 ��ġX�� �ִϸ��̼� Ÿ�� ���� ���� ������� ��ġ�� ��ȯ
		float relativeMouseX = (mousePos.x - timelinePos.x) / timelineSize.x;

		// �ð� ���� ����Ͽ� ���ο� Ű�������� ����
		float newTime = relativeMouseX * MaxTime; // MaxTime�� �ִ� �ð� ��

		// �ð��� ���� ���콺 ��ġ�� �°� �����մϴ�.
		currentTime = newTime;
	}

	// "Space" Ű : �ִϸ��̼� ���/����
	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Space)))
	{
		m_isPlayAnim = !m_isPlayAnim;
	}

	// "0" Ű : ��� �ӵ� �ʱ�ȭ
	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_0)))
	{
		m_fPlaybackSpeed = 1.f;
	}

	// "-" Ű : ��� �ӵ� -
	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Minus)))
	{
		m_fPlaybackSpeed -= 1.f;
	}

	// "+" Ű : ��� �ӵ� +
	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Equal)))
	{
		m_fPlaybackSpeed += 1.f;
	}

	//// " ` ~ " Ű : ���� ��� �ʱ�ȭ(����)
	//if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_GraveAccent)))
	//{
	//	eEditMode = EDITMODE_NONE;
	//}

	//// "F1" Ű : ũ�� ���� ���
	//if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_F1)))
	//{
	//	eEditMode = EDITMODE_SCALE;
	//}

	//// "F2" Ű : ȸ�� ���� ���
	//if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_F2)))
	//{
	//	eEditMode = EDITMODE_ROTATION;
	//}

	//// "F3" Ű : �̵� ���� ���
	//if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_F3)))
	//{
	//	eEditMode = EDITMODE_TRANSLATION;
	//}

	//// "F4" Ű : �ð� �� ���� ���� ���
	//if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_F4)))
	//{
	//	eEditMode = EDITMODE_TIME_VALUE;
	//}

	// "F5" Ű : Ű������ �ʱ�ȭ
	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_F5)))
	{
		m_vecTimeline.clear();
	}

	//// "F6" Ű : �Ӽ� �� ���� ������ ���� �ʱ�ȭ
	//if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_F6)))
	//{
	//	vValueSize = 1.f;
	//}

	//// "F7" Ű : �Ӽ� �� ���� ������ ���� Minus
	//if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_F7)))
	//{
	//	vValueSize -= 1.f;
	//}

	//// "F8" Ű : �Ӽ� �� ���� ������ ���� Plus
	//if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_F8)))
	//{
	//	vValueSize += 1.f;
	//}

	//// "P" Ű : ���� ��� �ʱ�ȭ(����)
	//if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_P)))
	//{
	//	eEditMode = EDITMODE_TEXTURE;
	//}

	//// "Z" Ű : ���� Ű������ ����
	//if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Z)))
	//{
	//	// ���� Ű�������� 0���� ���� ���� ��츸
	//	if (closestKeyframeIndex > 0)
	//	{
	//		closestKeyframeIndex -= (_int)1.f;
	//	}

	//	if (!timeline[m_iCurType].empty()) // �ִϸ��̼��� �ְ�
	//	{
	//		if (closestKeyframeIndex <= -1.f) // ���� �ƹ��͵� �������� �ʾ��� ���
	//		{
	//			closestKeyframeIndex = 0.f; // 0��° ����
	//		}
	//	}
	//}

	//// "X" Ű : ���� Ű������ ����
	//if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_X)))
	//{
	//	// ���� Ű�������� �ִ� Ű�������� ���� �ʾ��� ����
	//	if (closestKeyframeIndex < timeline[m_iCurType].size() - 1)
	//	{
	//		closestKeyframeIndex += 1.f;
	//	}

	//	if (!timeline[m_iCurType].empty()) // �ִϸ��̼��� �ְ�
	//	{
	//		if (closestKeyframeIndex <= -1.f) // ���� �ƹ��͵� �������� �ʾ��� ���
	//		{
	//			closestKeyframeIndex = 0.f; // 0��° ����
	//		}
	//	}
	//}
}

_float2 CWindow_UITool::Lerp_float2(const _float2& a, const _float2& b, float t)
{
	_float2 result;
	result.x = a.x + t * (b.x - a.x);
	result.y = a.y + t * (b.y - a.y);
	return result;
}

_float CWindow_UITool::Lerp_float(const _float& a, const _float& b, float t)
{
	_float result;
	result = a + t * (b - a);
	return result;
}

// �����ƽ ��¡(InQuad) �Լ� (����)
_float CWindow_UITool::ImEaseInQuad(float start, float end, float t)
{
	return start + (end - start) * t * t;
}

// �����ƽ ��¡(OutQuad) �Լ� (��)
_float CWindow_UITool::ImEaseOutQuad(float start, float end, float t)
{
	return start + (end - start) * (-t * (t - 2));
}

HRESULT CWindow_UITool::Save_Function(string strPath, string strFileName)
{
	_ushort iIndex = 0;

	strPath = strPath + "\\" + strFileName;

	json Out_Json;

	if (!m_vecParentObject.empty())
	{
		for (auto& Parent : m_vecParentObject)
		{
			json Out_Object;

			dynamic_cast<CUI*>(Parent)->Save_Desc(Out_Object);
			Out_Json.emplace(to_string(iIndex++), Out_Object);
		}
	}

	if (m_vecParentGroup != nullptr)
	{
		if (!m_vecParentGroup->empty())
		{
			for (auto& Group : m_vecParentObject)
			{
				json Out_Object;

				dynamic_cast<CUI*>(Group)->Save_Desc(Out_Object);
				Out_Json.emplace(to_string(iIndex++), Out_Object);
			}
		}
	}

	if (!m_vecChildObject.empty())
	{
		for (auto& Child : m_vecChildObject)
		{
			json Out_Object;

			dynamic_cast<CUI*>(Child)->Save_Desc(Out_Object);
			Out_Json.emplace(to_string(iIndex++), Out_Object);
		}
	}
	CJson_Utility::Save_Json(ConverWStringtoC(ConvertToWideString(strPath)), Out_Json);

	return S_OK;
}

HRESULT CWindow_UITool::Load_Function(string strPath, string strFileName)
{
	json json_in;

	//char filePath[MAX_PATH];

	string strFile;

	strFile = strPath + "\\" + strFileName;
	
	CJson_Utility::Load_Json(strFile.c_str(), json_in);

	for (auto& item : json_in.items())
	{
		json object = item.value();

		CUI::UI_DESC tUI_Info;

		/* ��������� �´��� Ȯ���ϱ� */
		tUI_Info.bParent = object["Parent"];					// 1. Parent
		tUI_Info.bWorld = object["World"];						// 2. World
		tUI_Info.bGroup = object["Group"];						// 3. Group
		tUI_Info.fAlpha = object["Alpha"];						// 4. Alpha
		tUI_Info.iObjectNum = object["ObjectNum"];				// 5. ObjectNum
		tUI_Info.iShaderNum = object["ShaderNum"];				// 6. ShaderPathNum
		tUI_Info.strObjectName = object["ObjectName"];			// 7. ObjectName
		tUI_Info.strLayerTag = object["LayerTag"];				// 8. LayerTag
		tUI_Info.strCloneTag = object["CloneTag"];				// 9. CloneTag
		tUI_Info.strProtoTag = object["ProtoTag"];				// 10. ProtoTag
		tUI_Info.strFilePath = object["FilePath"];				// 11. FilePath
		tUI_Info.strMapTextureTag = object["MapTextureTag"];	// 12. MapTexture
		tUI_Info.vColor.m128_f32[0] = object["ColorR"];			// 13. R
		tUI_Info.vColor.m128_f32[1] = object["ColorG"];			// 14. G
		tUI_Info.vColor.m128_f32[2] = object["ColorB"];			// 15. B
		tUI_Info.vColor.m128_f32[3] = object["ColorA"];			// 16. A

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

		pUI_Object->Get_Transform()->Load_FromJson(object); // 17. TransformCom

		if (tUI_Info.bParent == true)
		{
			m_vecParentObject.push_back(pUI_Object);
			Add_ParentList(tUI_Info);
		}
		else
		{
			m_vecChildObject.push_back(pUI_Object);
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

	//if (!m_vecChildObjectName.empty())
	//{
	//	for (auto& path : m_vecChildObjectName)
	//	{
	//		delete path;
	//	}
	//	m_vecChildObjectName.clear();
	//}
	if (m_vecParentGroup != nullptr)
	{
		//Safe_Delete(m_vecParentGroup);
		//m_vecParentGroup = nullptr;
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