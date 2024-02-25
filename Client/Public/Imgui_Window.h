#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_impl_win32.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameObject;
class CGameInstance;
END

BEGIN(Client)

class CImgui_Window abstract : public CBase
{
public:
	enum DIALOG_TYPE { SAVE_DIALOG, LOAD_DIALOG, DIALOG_END };
	
	enum WINDOW_TYPE 
	{
		IMGUI_MAPTOOL_WINDOW,
		IMGUI_ANIMATIONTOOL_WINDOW,
		IMGUI_EFFECTTOOL_WINDOW,
		IMGUI_UITOOL_WINDOW,
		IMGUI_BINARY_WINDOW,
		IMGUI_WINDOW_END
	};

	struct ImGuiDESC
	{
		string				strName;
		ImGuiWindowFlags	eWindowFlags = ImGuiWindowFlags_None;
		ImVec2				vWindowSize = { 0.f, 0.f };
		ImVec4				vBackgroundColor = ImVec4(0.f, 0.f, 0.f, 1.f);
	};

protected:
	CImgui_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CImgui_Window() = default;

public:
	_bool			Is_Enable() { return m_bEnable;}
	void			Set_Enable(_bool bEnable) { m_bEnable = bEnable; }
	HRESULT			SetUp_ImGuiDESC(string _strName, const ImVec2& _vWindowSize, ImGuiWindowFlags _eWindowFlags, const ImVec4& _vBackGroundColor);
	HRESULT			Begin();
	HRESULT			End();


public:
	void HelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::TextUnformatted(desc);
			ImGui::EndTooltip();
		}
	}

public:
	virtual	HRESULT Initialize();
	virtual	void	Tick(_float fTimeDelta);
	virtual void	Render() PURE;

public:	//TODO ImguiDialog #���̾�α�
	void				OpenDialog(WINDOW_TYPE eWindowType);
	void				ShowDialog();

	virtual	HRESULT		Save_Function(string strPath, string strFileName); //! ���� ������ Ư�������� override ���Ѽ� ���� �Լ� ����
	virtual HRESULT		Load_Function(string strPath, string strFileName); //! ���� ������ Ư�������� override ���Ѽ� �ҷ����� �Լ� ����

protected://TODO Guizmo #����� ������ �����̼ǰ����� �޾� ���� ���ؼ� �������
	_float3				Get_GuizmoTranslation() { return m_fmatrixTranslation; }
	_float3				Get_GuizmoRotation() { return m_fmatrixRotation; }
	_float3				Get_GuizmoScale() { return m_fmatrixScale; }

protected: //TODO Guizmo #�����
	void				Set_Guizmo(CGameObject* pGameObject);
	void				Set_GuizmoUI(CGameObject* pGameObject);
	void				Set_GuizmoCamView();
	void				Set_GuizmoCamProj();
	void				Set_GuizmoOrthographicLH();


protected: //TODO  ��ƿ
	_bool				ImGui_MouseInCheck();

protected:
	_bool				m_bEnable = false;
	_bool				m_bRenderTargetOnOff = true;
	_bool				m_bdialogCheck = false;

	ImGuiDESC			m_tImGuiDESC;
	_float				m_fWindowAlpha = 1.f;
	_float				m_fTimeDelta = 0.f;

	ID3D11Device*		m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	CGameInstance*		m_pGameInstance = { nullptr };

protected: //TODO ImguiDialog #���̾�α�
	DIALOG_TYPE			m_eDialogType = DIALOG_END; //! ���� �� ����, �ҷ� �ð���
	string				m_strDialogPath = {}; //! ���̾�α� �θ����� ���� ��� // ���� ../Bin/DataFiles/Map/
	string				m_strDialogKey = {};

protected: //TODO Guizmo #�����
	
	_float*				m_arrView = { nullptr };
	_float*				m_arrProj = { nullptr };
	_float*				m_arrOrthoProj = { nullptr };
	_float				snap[3] = { 1.f, 1.f, 1.f };
	_float				snap2D[3] = { 1.f, 1.f, 1.f };
private:
	_float3				m_fmatrixTranslation = {};
	_float3				m_fmatrixRotation = {};
	_float3				m_fmatrixScale = {};
	
public:
	virtual void Free() override;
};

END

