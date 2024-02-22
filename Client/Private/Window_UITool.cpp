#include "stdafx.h"
#include "Window_UITool.h"
#include "UI_Anything.h"
#include "GameInstance.h"

#include "Json_Utility.h"
#include "Target_Manager.h"
#include "RenderTarget.h"

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
	_int iSize = (_int)m_vecPaths.size();
	for (_int i = 0; i < iSize; i++)
	{
		IMAGEINFO* tTexture = new IMAGEINFO;

		_bool bRet = LoadTextureFromFile(ConverWStringtoC(ConvertToWideString(m_vecPaths[i]->strFilePath)), &tTexture->SRV_Texture, &tTexture->iImage_Width, &tTexture->iImage_Height);
		IM_ASSERT(bRet);

		/* Texture ũ�⸦ ���Ƿ� �����ϰ� �ʹٸ�, ���⼭ ������ �������. ���� ���ָ� ���� �ؽ�ó ũ���� ���´�. �� �� ���� ũ�⸦ �̿��� ������ �ٿ��� ����ص� ���� �� ����. */
		//tTexture->iImage_Width = 100;
		//tTexture->iImage_Height = 100;
		tTexture->iTextureNum = i;
		m_vecTexture.push_back(tTexture);

		tTexture = nullptr;
		delete[] tTexture;
	}

	_int iPathSize = (_int)m_vecPaths.size();
	//for (auto& iter : m_vecPaths)
	for(_int i = 0; i < iPathSize; i++)
	{
		string strFileNameWithoutExtension = GetFileName(m_vecPaths[i]->strFilePath);
		string strFileName = RemoveExtension(strFileNameWithoutExtension);

		PATHINFO* pPathInfo = new PATHINFO;

		pPathInfo->strFileName = strFileName;
		pPathInfo->strFilePath = m_vecPaths[i]->strFilePath;

		/* ��� �߶� �ֱ� */
		m_vecImagePaths.push_back(pPathInfo);

		pPathInfo = nullptr;
		delete[] pPathInfo;
	}

#pragma region �������
	//char filePath[MAX_PATH] = "../Bin/DataFiles/Data_UI/Texture_Info/Texture_Info";

	//json Out_Json;
	//_ushort iIndex = 0;

	//for (PATHINFO* iter : m_vecPaths)
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
	m_tUI_Desc.strCloneTag = "Prototype_GameObject_UI_Anything";
	m_tUI_Desc.strProtoTag = m_vecImagePaths[m_iSelectedPathIndex]->strFileName;
	m_tUI_Desc.strFilePath = m_vecImagePaths[m_iSelectedPathIndex]->strFilePath;
	m_tUI_Desc.fPositionX = g_iWinSizeX / 2;
	m_tUI_Desc.fPositionY = g_iWinSizeY / 2;
	m_tUI_Desc.fScaleX = 100;
	m_tUI_Desc.fScaleY = 100;
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

	SetUp_ImGuiDESC("Anim", { 600.f, 300.f }, 0, { 0.f, 0.f, 0.f, 0.f });

	__super::Begin();

	ImGui::Text("Mouse Pos : %d, %d", m_pt.x, m_pt.y);

	/* �̸����� */
	ImGui::Image((void*)m_vecTexture[m_iSelectedPathIndex]->SRV_Texture, ImVec2((_float)m_vecTexture[m_iSelectedPathIndex]->iImage_Width, (_float)m_vecTexture[m_iSelectedPathIndex]->iImage_Height));

	__super::End();

	SetUp_ImGuiDESC("Anim2", { 600.f, 300.f }, 0, { 0.f, 0.f, 0.f, 0.f });

	__super::Begin();

	if (ImGui::BeginTabBar("MyTabBar", m_Tab_bar_flags))
	{

		if (ImGui::BeginTabItem("Texture", &m_bOpenTexture))
		{

			ImGui::Dummy(ImVec2(0.f, 10.f));

			/* List */
			UI_List(fTimeDelta);

			ImGui::Dummy(ImVec2(0.f, 10.f));

			/* UI_2D ���� */
			UI2D_Setting(fTimeDelta);

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

void CWindow_UITool::UI_List(_float fTimeDelta)
{
	if (m_vecImagePaths.empty() ||
		m_vecTexture.empty())
		return;

	ImGui::RadioButton("UI", &m_iRenderGroup, (_int)CRenderer::RENDERGROUP::RENDER_UI); ImGui::SameLine();

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyMask_;
	ImGuiWindowFlags WindowFlag = ImGuiWindowFlags_HorizontalScrollbar;

	ImGui::BeginChild("Layer List", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 150), ImGuiChildFlags_Border, WindowFlag);
	// Layer
	Layer_List();
	ImGui::Spacing();
	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild("Texture List", ImVec2(ImGui::GetContentRegionAvail().x, 150), ImGuiChildFlags_Border, WindowFlag);
	// Textuer
	Texture_List();
	ImGui::Spacing();
	ImGui::EndChild();

	ImGui::BeginChild("Parent2 List", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 150), ImGuiChildFlags_Border, WindowFlag);
	// Parent
	Parent_List();
	ImGui::Spacing();
	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild("Class List", ImVec2(ImGui::GetContentRegionAvail().x, 150), ImGuiChildFlags_Border, WindowFlag);
	// Class
	Class_List();
	ImGui::Spacing();
	ImGui::EndChild();

	// Object
	Object_List();


#pragma region Create/Delete
	if (ImGui::Button("Create"))
	{
		UI2D_Create(fTimeDelta);
	}

	ImGui::SameLine(70.f);

	if (ImGui::Button("Delete"))
	{
		UI2D_Delete(fTimeDelta);
	}
#pragma endregion End

#pragma region Save/Load
	if (ImGui::Button("Save"))
	{
		m_eDialogType = CImgui_Window::SAVE_DIALOG;
		OpenDialog(CImgui_Window::IMGUI_UITOOL_WINDOW);
	}

	ImGui::SameLine(70.f);

	if (ImGui::Button("Load"))
	{
		m_eDialogType = CImgui_Window::LOAD_DIALOG;
		OpenDialog(CImgui_Window::IMGUI_UITOOL_WINDOW);
	}
#pragma endregion End
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
		++m_iSelectedObjectIndex;

		if (!m_vecUIObject.empty())
		{
			_int iSize = (_int)m_vecUIObject.size();
			if (m_iSelectedObjectIndex >= iSize)
				m_iSelectedObjectIndex = iSize - 1;
		}
	}

	if (m_pGameInstance->Key_Down(DIK_X))
	{
		--m_iSelectedObjectIndex;

		if (m_iSelectedObjectIndex <= 0)
			m_iSelectedObjectIndex = 0;
	}

	if (m_pGameInstance->Key_Pressing(DIK_UP))
	{
		if(m_iChangeType == (_int)SCALE)
			m_CurrObject->Change_SizeTop(m_fChangeValue);
		if (m_iChangeType == (_int)ROTATION)
			m_CurrObject->Get_Transform()->Turn(_vector({ 1.f, 0.f, 0.f, 0.f }), m_fChangeValue);
		if (m_iChangeType == (_int)POSITION)
		{
			_vector vPos = m_CurrObject->Get_Transform()->Get_Position();
			vPos.m128_f32[1] += m_fChangeValue;
			m_CurrObject->Set_Pos(vPos.m128_f32[0], vPos.m128_f32[1]);
		}
	}
	if (m_pGameInstance->Key_Pressing(DIK_DOWN))
	{
		if (m_iChangeType == (_int)SCALE)
			m_CurrObject->Change_SizeBottom(m_fChangeValue);
		if (m_iChangeType == (_int)ROTATION)
			m_CurrObject->Get_Transform()->Turn(_vector({ 1.f, 0.f, 0.f, 0.f }), -m_fChangeValue);
		if (m_iChangeType == (_int)POSITION)
		{
			_vector vPos = m_CurrObject->Get_Transform()->Get_Position();
			vPos.m128_f32[1] += -m_fChangeValue;
			m_CurrObject->Set_Pos(vPos.m128_f32[0], vPos.m128_f32[1]);
		}
	}
	if (m_pGameInstance->Key_Pressing(DIK_LEFT))
	{
		if (m_iChangeType == (_int)SCALE)
			m_CurrObject->Change_SizeLeft(m_fChangeValue);
		if (m_iChangeType == (_int)ROTATION)
			m_CurrObject->Get_Transform()->Turn(_vector({ 0.f, 0.f, 1.f, 0.f }), -m_fChangeValue);
		if (m_iChangeType == (_int)POSITION)
		{
			_vector vPos = m_CurrObject->Get_Transform()->Get_Position();
			vPos.m128_f32[0] += -m_fChangeValue;
			m_CurrObject->Set_Pos(vPos.m128_f32[0], vPos.m128_f32[1]);
		}
	}
	if (m_pGameInstance->Key_Pressing(DIK_RIGHT))
	{
		if (m_iChangeType == (_int)SCALE)
			m_CurrObject->Change_SizeRight(m_fChangeValue);
		if (m_iChangeType == (_int)ROTATION)
			m_CurrObject->Get_Transform()->Turn(_vector({ 0.f, 0.f, 1.f, 0.f }), m_fChangeValue);
		if (m_iChangeType == (_int)POSITION)
		{
			_vector vPos = m_CurrObject->Get_Transform()->Get_Position();
			vPos.m128_f32[0] += m_fChangeValue;
			m_CurrObject->Set_Pos(vPos.m128_f32[0], vPos.m128_f32[1]);
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
			m_tUI_Desc.fPositionX = m_pt.x;
			m_tUI_Desc.fPositionY = m_pt.y;
			UI2D_Create(fTimeDelta);
			
		}
	}

	/* Control_L */
	if (m_pGameInstance->Key_Pressing(DIK_LCONTROL))
	{
		if (m_pGameInstance->Mouse_Pressing(DIM_LB))
		{
			if (m_CurrObject != nullptr)
				m_CurrObject->Moving_Picking_Point(m_pt);
		}

		if (m_pGameInstance->Key_Down(DIK_S))
		{
			m_eDialogType = CImgui_Window::SAVE_DIALOG;
			OpenDialog(CImgui_Window::IMGUI_UITOOL_WINDOW);
		}

		if (m_pGameInstance->Key_Down(DIK_D))
		{
			UI2D_Create(fTimeDelta);
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

void CWindow_UITool::UI_Set()
{
	_int iSize = (_int)m_vecUIObject.size();
	for (_int i = 0; i < iSize; i++)
	{
		if (dynamic_cast<CUI*>(m_vecUIObject[i])->Get_Pick() == true)
		{
			if (m_pGameInstance->Mouse_Down(DIM_LB))
			{
				m_CurrObject = nullptr;
				m_CurrObject = dynamic_cast<CUI*>(m_vecUIObject[i]);
				m_iSelectedObjectIndex = i;
			}
		}
	}

	if (m_CurrObject != nullptr)
	{
		_vector vPosition = dynamic_cast<CUI*>(m_CurrObject)->Get_Transform()->Get_State(CTransform::STATE_POSITION);
		_vector vOrthoPos = m_pGameInstance->Convert_Orthogonal(vPosition);

		ImGui::InputFloat("PositionX", &vOrthoPos.m128_f32[0]);
		ImGui::InputFloat("PositionY", &vOrthoPos.m128_f32[1]);
		ImGui::InputFloat("PositionZ", &vOrthoPos.m128_f32[2]);

		Set_GuizmoCamView();
		Set_GuizmoCamProj();
		Set_GuizmoUI(m_CurrObject);
	}
}

void CWindow_UITool::Layer_List()
{

	if (ImGui::BeginListBox("Layer"))
	{
		for (_int i = 0; i < 3; i++)
		{
			const bool is_selected = (m_iLayerNum == i);
			if (ImGui::Selectable(m_strLayer[i].c_str(), is_selected))
				m_iLayerNum = i;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
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
				m_tUI_Desc.strProtoTag = m_vecImagePaths[m_iSelectedPathIndex]->strFileName;
			}
			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}
	///* �̸����� */
	//ImGui::Image((void*)m_vecTexture[m_iSelectedPathIndex]->SRV_Texture, ImVec2((_float)m_vecTexture[m_iSelectedPathIndex]->iImage_Width, (_float)m_vecTexture[m_iSelectedPathIndex]->iImage_Height));
	//m_tUI_Desc.strProtoTag = m_vecImagePaths[m_iSelectedPathIndex]->strFileName; // ����Ʈ �ڽ� ������ �� ���� : ����Ʈ���� ���þ����� ���� ���� �����ϵ��� ��� �����ϰ���
}

void CWindow_UITool::Parent_List()
{
	if (m_vecParent.empty())
		return;

	_int		ParentTagSize = (_int)m_vecParent.size();

	if (ImGui::BeginListBox("Parent"))
	{
		for (_int i = 0; i < ParentTagSize; i++)
		{
			const bool is_selected = (m_iSelectedParentIndex == i);
			if (ImGui::Selectable(ConverWStringtoC(ConvertToWideString(m_vecParent[i].c_str())), is_selected))
			{
				m_iSelectedParentIndex = i;

				/* Ŭ���� �ױ� ���� */
				m_tUI_Desc.strCloneTag = "Prototype_GameObject_";
				m_tUI_Desc.strCloneTag += m_vecParent[m_iSelectedParentIndex].c_str();
				m_tUI_Desc.bParent = true;
			}
			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}
}

void CWindow_UITool::Class_List()
{
	_int		ClassTagSize = (_int)m_vecClass.size();

	if (ImGui::BeginListBox("Class"))
	{
		for (_int i = 0; i < ClassTagSize; i++)
		{
			const bool is_selected = (m_iSelectedClassIndex == i);
			if (ImGui::Selectable(ConverWStringtoC(ConvertToWideString(m_vecClass[i].c_str())), is_selected))
			{
				m_iSelectedClassIndex = i;
				/* Ŭ���� �ױ� ���� */
				m_tUI_Desc.strCloneTag = "Prototype_GameObject_UI_";
				m_tUI_Desc.strCloneTag += m_vecClass[m_iSelectedClassIndex].c_str();
				m_tUI_Desc.bParent = false;
			}
			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}
}

void CWindow_UITool::Object_List()
{
	_int		ObjectTagSize = (_int)m_vecObjectName.size();

	if (ImGui::BeginListBox("Object"))
	{
		for (_int i = 0; i < ObjectTagSize; i++)
		{
			const bool is_selected = (m_iSelectedObjectIndex == i);
			if (ImGui::Selectable(ConverWStringtoC(ConvertToWideString(m_vecObjectName[i]->strFileName)), is_selected))
			{
				m_iSelectedObjectIndex = i;
				m_CurrObject = dynamic_cast<CUI*>(m_vecUIObject[m_iSelectedObjectIndex]);
			}
			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
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
					m_vecPaths.push_back(pPathInfo);

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
	if (m_iSelectedObjectIndex <= 0)
		m_iSelectedObjectIndex = 0;

	if (m_iSelectedPathIndex <= 0)
		m_iSelectedPathIndex = 0;
}

/* ���� �ε��ð��� ���̱� ���� ���� ���� �����Ű��. */
void CWindow_UITool::SetUp_Initialize()
{
	if (ImGui::IsKeyDown(ImGuiKey_F1))
	{
		bSetUpComplete = false;
	}

	if (!bSetUpComplete)
	{
		
		/* �����ص� �̹��� ��ε��� �ҷ��´�. */
		Load_Paths();

		// �̹��� �ε�
		_int iSize = (_int)m_vecPaths.size();

		for (_int i = 0; i < iSize; i++)
		{
			IMAGEINFO* tTexture = new IMAGEINFO;

			_bool bRet = LoadTextureFromFile(ConverWStringtoC(ConvertToWideString(m_vecPaths[i]->strFilePath)), &tTexture->SRV_Texture, &tTexture->iImage_Width, &tTexture->iImage_Height);
			IM_ASSERT(bRet);

			/* Texture ũ�⸦ ���Ƿ� �����ϰ� �ʹٸ�, ���⼭ ������ �������. ���� ���ָ� ���� �ؽ�ó ũ���� ���´�. �� �� ���� ũ�⸦ �̿��� ������ �ٿ��� ����ص� ���� �� ����. */
			//tTexture->iImage_Width = 100;
			//tTexture->iImage_Height = 100;
			tTexture->iTextureNum = i;
			m_vecTexture.push_back(tTexture);

			tTexture = nullptr;
			delete[] tTexture;
		}

		_int iPathSize = (_int)m_vecPaths.size();

		//for (auto& iter : m_vecPaths)
		for (_int i = 0; i < iPathSize; i++)
		{
			string strFileNameWithoutExtension = GetFileName(m_vecPaths[i]->strFilePath);
			string strFileName = RemoveExtension(strFileNameWithoutExtension);

			PATHINFO* pPathInfo = new PATHINFO;

			pPathInfo->strFileName = strFileName;
			pPathInfo->strFilePath = m_vecPaths[i]->strFilePath;

			/* ��� �߶� �ֱ� */
			m_vecImagePaths.push_back(pPathInfo);

			pPathInfo = nullptr;
			delete[] pPathInfo;
		}

#pragma region �������
		//char filePath[MAX_PATH] = "../Bin/DataFiles/Data_UI/Texture_Info/Texture_Info";

		//json Out_Json;
		//_ushort iIndex = 0;

		//for (PATHINFO* iter : m_vecPaths)
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
		m_tUI_Desc.strProtoTag = m_vecImagePaths[m_iSelectedPathIndex]->strFileName;
		m_tUI_Desc.strFilePath = m_vecImagePaths[m_iSelectedPathIndex]->strFilePath;
		m_tUI_Desc.fPositionX = g_iWinSizeX / 2;
		m_tUI_Desc.fPositionY = g_iWinSizeY / 2;
		m_tUI_Desc.fScaleX = 100;
		m_tUI_Desc.fScaleY = 100;

		/* Parent */
		m_tUIParent_Desc.strProtoTag = m_vecImagePaths[m_iSelectedPathIndex]->strFileName;
		m_tUIParent_Desc.strFilePath = m_vecImagePaths[m_iSelectedPathIndex]->strFilePath;
		m_tUIParent_Desc.fPositionX = g_iWinSizeX / 2;
		m_tUIParent_Desc.fPositionY = g_iWinSizeY / 2;
		m_tUIParent_Desc.fScaleX = 100;
		m_tUIParent_Desc.fScaleY = 100;
#pragma endregion End

		
		bSetUpComplete = true;
	}	
}

#pragma region Parent
void CWindow_UITool::Create_Add_UIParts(CUI::UI_DESC tUIDesc)
{
	dynamic_cast<CUI*>(m_vecUIParentObject[m_iSelectedParentIndex])->Create_Add_UIParts(&tUIDesc);
}

void CWindow_UITool::Add_UIParts(CUI* pUI)
{
	dynamic_cast<CUI*>(m_vecUIParentObject[m_iSelectedParentIndex])->Add_UIParts(pUI);
}

void CWindow_UITool::Delete_UIParts()
{
	if (m_vecUIParentObject.empty())
		return;

	dynamic_cast<CUI*>(m_vecUIParentObject[m_iSelectedParentIndex])->Is_Dead();

	if(m_iSelectedParentIndex > 0 &&
		m_iSelectedParentIndex <= m_vecUIParentObject.size())
		--m_iSelectedParentIndex;

	m_pCurrParent = nullptr;
}
#pragma endregion

void CWindow_UITool::UI2D_Setting(_float fTimeDelta)
{
	ImGui::CollapsingHeader("2D_Setting");

	/* Mod */
	ImGui::SeparatorText(u8"���� ��� ����");
	ImGui::RadioButton("Scale", &m_iChangeType, 1);
	ImGui::RadioButton("Rotation", &m_iChangeType, 2);
	ImGui::RadioButton("Position", &m_iChangeType, 3);

	/* Scale */
	ImGui::SeparatorText(u8"ũ�� ����");
	ImGui::InputFloat("ScaleX", &m_tUI_Desc.fScaleX);
	ImGui::InputFloat("ScaleY", &m_tUI_Desc.fScaleY);

	/* Position*/
	ImGui::SeparatorText(u8"��ġ ����");
	ImGui::InputFloat("PositionX", &m_tUI_Desc.fPositionX);
	ImGui::InputFloat("PositionY", &m_tUI_Desc.fPositionY);

	ImGui::Separator();
	ImGui::InputTextWithHint(u8"�Է� ", u8"�ؽ�Ʈ�� �Է��ϼ���.", cInputText, IM_ARRAYSIZE(cInputText));
	ImGui::Text(cInputText);

	ImGui::Dummy(ImVec2(0, 5)); // ����

	UI_Set();

	ImGui::Separator();

	ImGui::Dummy(ImVec2(0, 5)); // ����


}

HRESULT CWindow_UITool::UI2D_Create(_float fTimeDelta)
{
	CGameObject* pGameObject = nullptr;
	CUI* pUI = nullptr;
	wstring strCloneProto = TEXT("");
	m_pGameInstance->String_To_WString(m_tUI_Desc.strCloneTag, strCloneProto);

	if (m_tUI_Desc.bParent == true)
	{
		pGameObject = m_pGameInstance->Add_CloneObject_And_Get(LEVEL_STATIC, ConvertToWideString(m_strLayer[m_iLayerNum]), strCloneProto, &m_tUI_Desc);
		pUI = dynamic_cast<CUI*>(pGameObject);
		Add_ParentList(m_tUI_Desc);
		m_vecUIParentObject.push_back(pUI);
	}
	else
	{
		if (m_pCurrParent == nullptr)
			return E_FAIL;

		m_pGameInstance->Add_CloneObject_And_Get(LEVEL_STATIC, ConvertToWideString(m_strLayer[m_iLayerNum]), strCloneProto, &m_tUI_Desc);
		m_pCurrParent->Create_UIParts(m_tUI_Desc);
		Add_ObjectList(m_tUI_Desc);
	}

	return S_OK;
}

void CWindow_UITool::UI2D_Delete(_float fTimeDelta)
{
	if (m_vecUIObject.empty())
		return;

	// ������Ʈ ����
	m_vecUIObject[m_iSelectedObjectIndex]->Set_Dead(true); // Set_Dead�� �̳༮�� ������� Layer�� ����ִ� Object�� �״´�.

	/* Set_Dead�� ��û�ϸ� �Ŵ������� �˾Ƽ� �ش� ��ü�� �����޶�� �ѰŴϱ� �� ��ü�� ���� ����ó���� ���� ���� �� �ϸ� �ȵȴ�. (�ٵ� �� ����?) */
	//Safe_Release(m_vecUIObject[m_iSelectedObjectIndex]);
	m_vecUIObject.erase(m_vecUIObject.begin() + m_iSelectedObjectIndex);

	if(!m_vecObjectName.empty())
		m_vecObjectName.erase(m_vecObjectName.begin() + m_iSelectedObjectIndex); // ������Ʈ ��� �̸� ����

	if(m_CurrObject)
		m_CurrObject = nullptr; // ���� ���õ��ִ� �༮�� �׾����� �ּ� �������.

	m_iSelectedObjectIndex = m_iSelectedObjectIndex - 1; // ���� ������ �ε�����°�� ������Ʈ�� ����������, ���õ� �ε����� �ڷ� ��ĭ ��������.
}

//										���� ������ �༮�� ��ü ��θ� �޾ƿ´�.
void CWindow_UITool::AddIndexNumber(PATHINFO& UI_Info)
{
	int		index = 0;
	_bool	isPath = false;
	//size_t underscorePos = str.find_last_of('_');
	//auto it = std::find(m_vecObjectName.begin(), m_vecObjectName.end(), UI_Info.strFilePath); // ��θ� ���Ѵ�.
	if (m_vecObjectName.empty())
		return;

	for (auto& iter : m_vecObjectName)
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
		for (auto& strFilePath : m_vecObjectName)
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
		
		//std::cout << "Name: " << str << ", Index: " << index << std::endl;
	}
	else 
	{
		// ���ο� ���ڿ��� ���
		index = 0;
		//std::cout << "Name: " << str << ", Index: " << m_vecObjectName.size() - 1 << std::endl;
	}

	/* �ױ� */
	UI_Info.strFileName = UI_Info.strFileName + to_string(index);
	UI_Info.iPathNum = index;
}

void CWindow_UITool::Add_ParentList(CUI::UI_DESC tIn_UI_Desc)
{
	// error : �Ʒ� Get_CloneGameObjects�� ������Ʈ�� �����ö� ���� ������Ʈ���� ��� �ٽõ�� ���� ������, �Լ��� ���� ����ų� Ŭ�����ϰ� �������
	if (!m_vecUIObject.empty())
		m_vecUIObject.clear();

	PATHINFO* tUI_Desc = new PATHINFO;

	// ���ڿ� �ߺ� ��
	tUI_Desc->strFileName = tIn_UI_Desc.strProtoTag;
	tUI_Desc->strFilePath = tIn_UI_Desc.strFilePath;
	AddIndexNumber(*tUI_Desc); // ������Ʈ �ױ� ����

	m_vecParentObjectName.push_back(tUI_Desc); // �̸� �ߺ� �˻� �� ó���� �ױװ����� �־�����.
	m_pGameInstance->Get_CloneGameObjects(LEVEL_STATIC, &m_vecUIParentObject);

	tUI_Desc = nullptr;
	delete[] tUI_Desc;
}

void CWindow_UITool::Add_ObjectList(CUI::UI_DESC tIn_UI_Desc)
{
	// error : �Ʒ� Get_CloneGameObjects�� ������Ʈ�� �����ö� ���� ������Ʈ���� ��� �ٽõ�� ���� ������, �Լ��� ���� ����ų� Ŭ�����ϰ� �������
	if (!m_vecUIObject.empty())
		m_vecUIObject.clear();

	PATHINFO* tUI_Desc = new PATHINFO;

	// ���ڿ� �ߺ� ��
	tUI_Desc->strFileName = tIn_UI_Desc.strProtoTag;
	tUI_Desc->strFilePath = tIn_UI_Desc.strFilePath;
	AddIndexNumber(*tUI_Desc); // ������Ʈ �ױ� ����

	m_vecObjectName.push_back(tUI_Desc); // �̸� �ߺ� �˻� �� ó���� �ױװ����� �־�����.
	m_pGameInstance->Get_CloneGameObjects(LEVEL_STATIC, &m_vecUIObject);

	tUI_Desc = nullptr;
	delete[] tUI_Desc;
}

void CWindow_UITool::Create_TargetTexture()
{
	/* error : Find�Լ��� ����Ÿ���� ã�ƿ� ��, �׳༮���� Create�Լ��� ȣ���ϸ� �ܺ�������ȣ ������ �߻���.. ���� �����ν��Ͻ��� Create���� ����Ǵ� �Լ��� ���� ���� ����ϸ� �ذ� */
	m_pGameInstance->Create_RenderTarget(TEXT("Target_Diffuse_UI"));
}

/* ex : Save */
void CWindow_UITool::Save_Desc()
{

}

/* ex : Load */
HRESULT CWindow_UITool::Load_Desc()
{
	json json_in;

	char filePath[MAX_PATH] = "../Bin/DataFiles/Data_UI/UI_Info";

	_int		iPathNum = 0;
	string		strFileName;
	string		strFilePath;


	CJson_Utility::Load_Json(filePath, json_in);

	for (auto& item : json_in.items())
	{
		json object = item.value(); 

		CUI::UI_DESC tUI_Info;

		//tUI_Info.fPositionX = object["PostionX"];
		//tUI_Info.fPositionY = object["PostionY"];
		//tUI_Info.fScaleX = object["SizeX"];
		//tUI_Info.fScaleY = object["SizeY"];

		tUI_Info.strCloneTag = object["CloneTag"];
		tUI_Info.strProtoTag = object["ProtoTag"];
		tUI_Info.strFilePath = object["FilePath"];

		wstring wstrPrototag;
		m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

		wstring wstrFilePath;
		m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

		wstring wstrLayer;
		m_pGameInstance->String_To_WString(m_strLayer[m_iLayerNum], wstrLayer);

		CUI_Anything* pUI_Object = dynamic_cast<CUI_Anything*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_STATIC, wstrLayer, TEXT("Prototype_GameObject_UI_Anything"), &tUI_Info));
		
		pUI_Object->Get_Transform()->Load_FromJson(object);

		Add_ObjectList(tUI_Info);
	}
	return S_OK;
}

HRESULT CWindow_UITool::Save_Function(string strPath, string strFileName)
{
	if (m_vecUIObject.empty())
		return E_FAIL;

	_ushort iIndex = 0;

	strPath = strPath + "\\" + strFileName;

	json Out_Json;

	for (auto& UI : m_vecUIObject)
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

		tUI_Info.strCloneTag = object["CloneTag"];
		tUI_Info.strProtoTag = object["ProtoTag"];
		tUI_Info.strFilePath = object["FilePath"];

		wstring wstrPrototag;
		m_pGameInstance->String_To_WString(tUI_Info.strProtoTag, wstrPrototag);

		wstring wstrFilePath;
		m_pGameInstance->String_To_WString(tUI_Info.strFilePath, wstrFilePath);

		wstring wstrLayer;
		m_pGameInstance->String_To_WString(m_strLayer[m_iLayerNum], wstrLayer);

		CUI_Anything* pUI_Object = dynamic_cast<CUI_Anything*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_STATIC, wstrLayer, TEXT("Prototype_GameObject_UI_Anything"), &tUI_Info));

		pUI_Object->Get_Transform()->Load_FromJson(object);

		Add_ObjectList(tUI_Info);
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

		m_vecPaths.push_back(tPath);

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

	//if (!m_vecObjectName.empty())
	//	Safe_Delete(m_vecObjectName);
	//if (!m_vecImagePaths.empty())
	//	Safe_Delete(m_vecImagePaths);
	//if (!m_vecPaths.empty())
	//	Safe_Delete(m_vecPaths);
	//if (!m_vecTexture.empty())
	//	Safe_Delete(m_vecTexture);
	//if (!m_vecUIObject.empty())
	//	Safe_Delete(m_vecUIObject);
	
	m_CurrObject = nullptr;
	m_pCurrParent = nullptr;

	if (!m_vecObjectName.empty())
	{
		for (auto& path : m_vecObjectName)
		{
			delete path;
		}
		m_vecObjectName.clear();
	}
	if (!m_vecImagePaths.empty())
	{
		for (auto& path : m_vecImagePaths)
		{
			delete path;
		}
		m_vecImagePaths.clear();
	}
	if (!m_vecPaths.empty())
	{
		for (auto& path : m_vecPaths)
		{
			delete path;
		}
		m_vecPaths.clear();
	}
	if (!m_vecTexture.empty())
	{
		for (auto& path : m_vecTexture)
		{
			delete path;
		}
		m_vecTexture.clear();
	}
	//if (!m_vecUIObject.empty())
	//{
	//	for (auto& path : m_vecUIObject) {
	//		delete path;
	//	}
	//	m_vecUIObject.clear();
	//}


}