#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END


BEGIN(Client)

class CImgui_Window;

class CImgui_Manager : public CBase
{
	DECLARE_SINGLETON(CImgui_Manager);

public:
	enum class IMGUI_WINDOW_TYPE
	{
		IMGUI_MAPTOOL_WINDOW,
		IMGUI_ANIMATIONTOOL_WINDOW,
		IMGUI_EFFECTTOOL_WINDOW,
		IMGUI_UITOOL_WINDOW,
		IMGUI_BINARY_WINDOW,
		IMGUI_WINDOW_END
	};

private:
			 CImgui_Manager();
	virtual ~CImgui_Manager() = default;

public:
	HRESULT			Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	void			Tick(_float fTimeDelta);
		void			MenuTick(_float fTimeDelta);

	void			Render();

private:
	//!Intialize
	void			 ImGuizmo_Initialize();
	void			 ImGuiFileDialog_Initialize();
	HRESULT			 Ready_Windows();
	//

private:
	CImgui_Window*	 Find_Window(IMGUI_WINDOW_TYPE eWindowType);
	_bool			 Check_ImGui_Rect(); //! ���콺�� ImGui â�ȿ� �ִ��� üũ

private:
	class CGameInstance*	m_pGameInstance = { nullptr };
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pContext;

	//! Render�� ���� ȣ��Ǽ� ù�����ӿ� ������ ������ �������� Tick�� �ѹ��̻� ȣ�� �� ���Ŀ� Render�� ȣ���ϱ����� ����
	_bool					m_bFirstTick = false;

private:
	map<IMGUI_WINDOW_TYPE, CImgui_Window*>	m_mapWindows;


public:
	virtual void	Free();
};

END

