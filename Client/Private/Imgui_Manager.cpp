#include "stdafx.h"

//#define IMGUI_DEFINE_MATH_OPERATORS
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_dx11.h"
#include "../Imgui/imgui_impl_win32.h"

#include "GameInstance.h"
#include "Imgui_Manager.h"
#include "Imgui_Window.h"
#include "Include_Imgui_Windows.h"


ImGuiIO g_io;
IMPLEMENT_SINGLETON(CImgui_Manager);

CImgui_Manager::CImgui_Manager()
{
}

#pragma region �̴ϼȶ�����

HRESULT CImgui_Manager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{

	m_pDevice = pDevice;
	m_pContext = pContext;
	m_pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	g_io = ImGui::GetIO(); (void)g_io;
	g_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls


	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);

	if (false == ImGui_ImplWin32_Init(g_hWnd))
		return E_FAIL;

	if (false == ImGui_ImplDX11_Init(m_pDevice, m_pContext))
		return E_FAIL;

	//ImGui::StyleColorsDark();
	g_io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, g_io.Fonts->GetGlyphRangesKorean());

	if(FAILED(Ready_Windows()))
		return E_FAIL;


	_int iSize = _int(CImgui_Manager::IMGUI_WINDOW_TYPE::IMGUI_WINDOW_END);

	for (_int i = 0; i < iSize; ++i)
	{
		m_bEnableTool[i] = false;
	}


	return S_OK;

}

void CImgui_Manager::ImGuizmo_Initialize()
{
}

void CImgui_Manager::ImGuiFileDialog_Initialize()
{
}

HRESULT CImgui_Manager::Ready_Windows()
{
	//TODO ImGuiWindowFlags WindowFlag
		// ImGuiWindowFlags_MenuBar |
		// ImGuiWindowFlags_NoDocking |
		// ImGuiWindowFlags_NoTitleBar |
		// ImGuiWindowFlags_NoCollapse |
		// ImGuiWindowFlags_NoResize |
		// ImGuiWindowFlags_NoMove |
		// ImGuiWindowFlags_NoBringToFrontOnFocus |
		// ImGuiWindowFlags_NoNavFocus;


	//! ����ٰ� �ڱⰡ �ʿ��� �� ������ ��ü �߰��ؼ� ����. �Ʒ��� ����

	
#pragma region ����
	CImgui_Window* pWindow = CWindow_MapTool::Create(m_pDevice, m_pContext);

	if (pWindow == nullptr)
		return E_FAIL;

	pWindow->SetUp_ImGuiDESC(u8"����", ImVec2{ 200.f, 200.f }, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus, ImVec4(0.f, 0.f, 0.f, 1.f));

	m_mapWindows.emplace(IMGUI_WINDOW_TYPE::IMGUI_MAPTOOL_WINDOW, pWindow);
#pragma endregion ����

	
#pragma region �ִϸ��̼���

	pWindow = CWindow_AnimTool::Create(m_pDevice, m_pContext);

	if (pWindow == nullptr)
		return E_FAIL;

	pWindow->SetUp_ImGuiDESC(u8"�ִϸ��̼���", ImVec2{ 300.f,300.f }, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus, ImVec4(0.f, 0.f, 0.f, 1.f));

	m_mapWindows.emplace(IMGUI_WINDOW_TYPE::IMGUI_ANIMATIONTOOL_WINDOW, pWindow);
#pragma endregion �ִϸ��̼���

#pragma region ����Ʈ��
	pWindow = CWindow_EffectTool::Create(m_pDevice, m_pContext);
	if (pWindow == nullptr)
		return E_FAIL;

	pWindow->SetUp_ImGuiDESC(u8"����Ʈ ��", ImVec2{ 300.f, 680.f }, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | /*ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |*/ ImGuiWindowFlags_NoBringToFrontOnFocus, ImVec4(0.f, 0.f, 0.f, 1.f));
	m_mapWindows.emplace(IMGUI_WINDOW_TYPE::IMGUI_EFFECTTOOL_WINDOW, pWindow);
#pragma endregion ����Ʈ��

#pragma region UI_START
	/* ���� �� �����츦 ������ش�. */
	CImgui_Window* pWindowUI = CWindow_UITool::Create(m_pDevice, m_pContext);

	if (pWindowUI == nullptr)
		return E_FAIL;

	/* UI�����쿡���� �ɼ� ���� */
	pWindowUI->SetUp_ImGuiDESC(u8"UI��", ImVec2{ 400.f, 600.f }, ImGuiWindowFlags_MenuBar, ImVec4(0.f, 0.f, 0.f, 1.f) /*| ImGuiWindowFlags_NoCollapse*/ /*| ImGuiWindowFlags_NoBringToFrontOnFocus*/ /*| ImGuiWindowFlags_NoNavFocus*/);

	/* ���õ� �����츦 �߰������ش�. */
	m_mapWindows.emplace(IMGUI_WINDOW_TYPE::IMGUI_UITOOL_WINDOW, pWindowUI);
#pragma endregion UI_END

	return S_OK;
}



#pragma endregion �̴ϼȶ�����

void CImgui_Manager::Tick(_float fTimeDelta)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//ImGuizmo::BeginFrame();

	/////* ---------------------- IMGUI ---------------------- */
	//ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_NoBackground;
	//
	//auto& style = ImGui::GetStyle();
	//ImVec4* colors = style.Colors;
	//
	///* �����ϰ� */
	//const ImVec4 bgColor = ImVec4(0.1f, 0.1f, 0.1f, 0.5f);
	//colors[ImGuiCol_WindowBg] = bgColor;
	//colors[ImGuiCol_ChildBg] = bgColor;
	//colors[ImGuiCol_TitleBg] = bgColor;
	//
	//ImGui::SetNextWindowSize(ImVec2(g_iWinSizeX, g_iWinSizeY), ImGuiCond_FirstUseEver);
	//ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);


	//! �ֻ�� �޴��ٿ� ���õ� �Լ�
	MenuTick(fTimeDelta);

 	for (auto& pWindowPair : m_mapWindows)
 	{
 		if(true == pWindowPair.second->Is_Enable()) //!���� �����찡 Ȱ��ȭ�������� üũ.
 			pWindowPair.second->Tick(fTimeDelta);
 	}

}

void CImgui_Manager::MenuTick(_float fTimeDelta)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Tools"))
		{
 			if (ImGui::MenuItem("MapTool", nullptr, m_bEnableTool[(_int)IMGUI_WINDOW_TYPE::IMGUI_MAPTOOL_WINDOW]))
 			{
 				CImgui_Window* pWindow = Find_Window(CImgui_Manager::IMGUI_WINDOW_TYPE::IMGUI_MAPTOOL_WINDOW);

 				if (nullptr == pWindow)
 				{
 					MSG_BOX("�� �����찡 ����. Ready_Window �Լ� Ȯ�� �ٶ�");
 					return;
 				}

 				pWindow->Set_Enable(!pWindow->Is_Enable()); //! ������ Ȱ��ȭ ���¸� ��������
 			}

 			if (ImGui::MenuItem("AnimationTool", nullptr, m_bEnableTool[(_int)IMGUI_WINDOW_TYPE::IMGUI_ANIMATIONTOOL_WINDOW]))
 			{
 				CImgui_Window* pWindow = Find_Window(CImgui_Manager::IMGUI_WINDOW_TYPE::IMGUI_ANIMATIONTOOL_WINDOW);
				
 				if (nullptr == pWindow)
 				{
 					MSG_BOX("�ִϸ��̼� �����찡 ����. Ready_Window �Լ� Ȯ�� �ٶ�");
 					return;
 				}

 				pWindow->Set_Enable(!pWindow->Is_Enable()); //! ������ Ȱ��ȭ ���¸� ��������o
 			}

 			if (ImGui::MenuItem("EffectTool", nullptr, m_bEnableTool[(_int)IMGUI_WINDOW_TYPE::IMGUI_EFFECTTOOL_WINDOW]))
 			{
 				CImgui_Window* pWindow = Find_Window(CImgui_Manager::IMGUI_WINDOW_TYPE::IMGUI_EFFECTTOOL_WINDOW);

 				if (nullptr == pWindow)
 				{
 					MSG_BOX("����Ʈ �����찡 ����. Ready_Window �Լ� Ȯ�� �ٶ�");
 					return;
 				}

 				pWindow->Set_Enable(!pWindow->Is_Enable()); //! ������ Ȱ��ȭ ���¸� ��������
 			}

 			if (ImGui::MenuItem("UITool", nullptr, m_bEnableTool[(_int)IMGUI_WINDOW_TYPE::IMGUI_UITOOL_WINDOW]))
 			{
 				CImgui_Window* pWindow = Find_Window(CImgui_Manager::IMGUI_WINDOW_TYPE::IMGUI_UITOOL_WINDOW);

 				if (nullptr == pWindow)
 				{
 					MSG_BOX("UI �����찡 ����. Ready_Window �Լ� Ȯ�� �ٶ�");
 					return;
 				}

 				pWindow->Set_Enable(!pWindow->Is_Enable()); //! ������ Ȱ��ȭ ���¸� ��������
 			}

 			ImGui::EndMenu();
		}
		
		
		ImGui::EndMainMenuBar();
		
	}

	
}

void CImgui_Manager::Render()
{
	//!���� �ѹ� Render ����. ������ Header���� ������� m_bFirstTick �ּ� Ȯ���غ���.
	if (false == m_bFirstTick)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		m_bFirstTick = true;
	}


	for (auto& pWindowPair : m_mapWindows)
	{
		if (true == pWindowPair.second->Is_Enable()) //!���� �����찡 Ȱ��ȭ�������� üũ.
			pWindowPair.second->Render();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGui::EndFrame();
}


CImgui_Window* CImgui_Manager::Find_Window(IMGUI_WINDOW_TYPE eWindowType)
{
	auto iter = m_mapWindows.find(eWindowType);

	if (iter == m_mapWindows.end())
		return nullptr;

	return iter->second;
}

_bool CImgui_Manager::Check_ImGui_Rect()
{
	POINT tMouse = {};
	GetCursorPos(&tMouse);
	ScreenToClient(m_pGameInstance->Get_GraphicDesc()->hWnd, &tMouse);

	ImVec2 windowPos = ImGui::GetWindowPos(); //���ʻ�ܸ𼭸���
	ImVec2 windowSize = ImGui::GetWindowSize();

	if (tMouse.x >= windowPos.x && tMouse.x <= windowPos.x + windowSize.x &&
		tMouse.y >= windowPos.y && tMouse.y <= windowPos.y + windowSize.y)
	{
		return false; //ImGui ���� ��
	}
	return true; //ImGui �����̶� �� ��ħ!
}

void CImgui_Manager::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	for(auto& pWindowPair : m_mapWindows)
		Safe_Release(pWindowPair.second);

	m_mapWindows.clear();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
