#pragma once

#include "Imgui_Window.h"
#include "UI_Anything.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CWindow_UITool final : public CImgui_Window
{
public: /* ==================== Struct ==================== */
	struct WINDOW_UITOOL_DESC : public ImGuiDESC
	{
	};

	enum CHANGETYPE { NONE, SCALE, ROTATION, POSITION, CHANGE_END };

	typedef struct tagImageInfo
	{
		_int						iImage_Width = 100;
		_int						iImage_Height = 100;

		_int						iTextureNum = -1;

		ID3D11ShaderResourceView* SRV_Texture = NULL;
	}IMAGEINFO;

public:
	// SH_Add
	typedef struct tagPathInfo
	{
		_int		iPathNum = 0;
		string		strFileName;
		string		strFilePath;
	}PATHINFO;

public:
	typedef struct tagUIBaseDesc
	{
		//			��ġ X,Y / ������ X,Y
		_float		fX, fY, fSizeX, fSizeY;

		_float		fTexSizeX = 0.f, fTexSizeY = 0.f;

		_float		fOffsetX = 0.f, fOffsetY = 0.f;

		_bool		bWorldUI = false;	// ������� UI����
		_bool		bFrame = false;		// Frame
		_bool		bEnable = true;		// Enable ����
	}UI_DESC;

protected: /* ============ Create/Extinction ============= */
	CWindow_UITool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CWindow_UITool() = default;


public: /* =================== Basic =====================*/
	virtual	HRESULT				Initialize() override;
	virtual	void				Tick(_float fTimeDelta) override;
	virtual void				Render() override;

public: /* ==================== UI ===================== */
	void						UI_List(_float fTimeDelta);
	void						UI_ToolTip(_float fTimeDelta);
	std::vector<unsigned char>	UI_LoadImage(const std::string& filename, int& width, int& height, int& channels);
	void						ShowImagePreview(const std::vector<unsigned char>& imageData, int width, int height);
	bool						LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
	//HRESULT						Update_Pos();

public: /* ==================== Shortcut_Key ===================== */
	void						Shortcut_Key(_float fTimeDelta);

public:
	void						UI_Set();

public: /* ==================== List List List List List List List List List List List List List List List List List List List List List List List List List List List List List List List ===================== */
	void						Layer_List();
	void						Texture_List();
	void						Parent_List();
	void						Class_List();
	void						Object_List();

#pragma region	Parent
public:							/* Setting */
	void						Setting_Parent();
public:						    /* Create/Delete */
	HRESULT						Create_Parent(CUI::UI_DESC pUIDesc);
	void						Delete_Parent();
#pragma endregion

	
#pragma region	Child
public:							/* Setting */
	void						Setting_Child();
public:							/* Create/Delete */
	HRESULT						Create_Child(CUI::UI_DESC pUIDesc);
	void						Delete_Child(_float fTimeDelta);
public:							/* Add */
	void						Add_ParentList(CUI::UI_DESC tIn_UI_Desc);
	void						Add_ParentIndexNumber(PATHINFO& UI_Info);
#pragma endregion

public:
	void						Current_Info();
	void						Create_TargetTexture();

public: /* Save/Load */
	virtual	HRESULT				Save_Function(string strPath, string strFileName) override;
	virtual HRESULT				Load_Function(string strPath, string strFileName) override;

public: /* ================= Function ================= */
	// stringŸ���� �޴� ���� �����̳ʸ� char*Ÿ���� �޴� ���� �����̳ʷ� ��ȯ ���ִ� �Լ�
	std::vector<const char*> ConvertStringVectorToCharArray(const std::vector<std::string>& stringVector);
	// string�� wstring���� ��ȯ ���ִ� �Լ�
	std::wstring ConvertToWideString(const std::string& ansiString);
	// string�� wchar�� ��ȯ ���ִ� �Լ�
	WCHAR* StringTowchar(const std::string& str);
	// wstring�� string���� ��ȯ ���ִ� �Լ�
	std::string WStringToString(const std::wstring& wstr);
	//	wstring�� char�� ��ȯ ���ִ� �Լ�
	char* ConverWStringtoC(const wstring& wstr);
	//	char�� wchar_t�� ��ȯ ���ִ� �Լ�
	wchar_t* ConverCtoWC(char* str);
	// WCHAR*�� string���� ��ȯ ���ִ� �Լ�
	std::string WideStringToString(const wchar_t* wideStr);
	// ��ο��� �����̸��� Ȯ���ڸ� �������ִ� �Լ�
	std::string GetFileName(const std::string& filePath);
	// _�������� ���� ������ �̸��� �������ִ� �Լ�
	std::string GetUnderbarName(const std::string& filePath);
	// Ȯ���ڸ� �������ִ� �Լ�
	std::string RemoveExtension(const std::string& filePath);


public:
	/* Child */



	void						UI2D_Setting(_float fTimeDelta);

	void						AddIndexNumber(PATHINFO& str);

	void						Add_ObjectList(CUI::UI_DESC tIn_UI_Desc);



	void						Load_Paths();

public: /* Image */
	// �̹��� �ε� �Լ�
	void						LoadImgPath(const _tchar* folderPath);
	void						ImagePreview(_float fTimeDelta);

public:
	void						IndexCheck();
	void						SetUp_Initialize(); // Load
	_bool						bSetUpComplete = true;

private: /* Member */
	CUI::UI_DESC				m_tUI_Info;

private: /* Parent */
	void						Create_Add_UIParts(CUI::UI_DESC tUIDesc);
	void						Add_UIParts(CUI* pUI);



private: /* ==================== Mouse ==================== */


private: /* Image_Member */
	_int						m_My_Image_Width = 100;
	_int						m_My_Image_Height = 100;
	_int						m_iTextureNum = 0;
	_int						m_iTestNum = 0;
	vector<IMAGEINFO*>			m_vecTexture;		// �̹��� �̸����⸦ ���� �ؽ�ó ������
	vector<PATHINFO*>			m_vecPaths;			// ���, �����̸� ��� �����̳� (������Ÿ�� �Ľ̰���)
	std::vector<PATHINFO*>		m_vecImagePaths;	// �̹��� �ε带 ���� ���
	std::vector<PATHINFO*>		m_vecObjectName;	// �߰��� ������Ʈ���� �̸� (����Ʈ �ڽ� ��¿�)
	std::vector<PATHINFO*>		m_vecParentObjectName;	// �߰��� Parent������Ʈ���� �̸� (����Ʈ �ڽ� ��¿�)

	IMAGEINFO*					m_tTexture;


	_int						m_iSelectedPathIndex = 0; // ���õ� �̹��� ��� �ε���
	_int						m_iSelectedObjectIndex = 0; // ���õ� UI������Ʈ
	_int						m_iSelectedClassIndex = 0; // ���õ� Class
	_int						m_iSelectedParentClassIndex = 0; // ���õ� Parent Class
	_int						m_iSelectedParentIndex = 0; // ���õ� Parent
	_int						m_iUINameNum = 0;
private: /* 2D */
	_float						m_fPosition[2] = { 0.f, 0.f };
	_float						m_fScale[2] = { 0.f, 0.f };
	vector<CGameObject*>		m_vecUIObject;
	vector<CGameObject*>		m_vecUIParentObject;
	string						m_strLayer[3] = { "Layer_UI_Player", "Layer_UI_Monster", "Layer_UI_Inventory" };

	_int						m_iLayerNum = 0;
	CUI::UI_DESC				m_tUI_Desc;
	CUI::UI_DESC				m_tUIParent_Desc;
	CUI*						m_CurrObject = nullptr;
	CUI*						m_pCurrParent = nullptr;
	
	POINT						m_pt;

private:
	char cInputText[MAX_PATH] = "";
private: /* Value */
	_float						m_fChangeValue = 0.1f;

private: /* enum */
	_int						m_iChangeType = (_int)CHANGETYPE::NONE;
	_int						m_iRenderGroup = (_int)CRenderer::RENDER_UI;

private: /* bool */
	_bool						m_bOpenTexture = true;
	_bool						m_bOpenSetting = false;
	_bool						m_bOpenUI = false;
	_bool						m_bScaleChange = false;
	_bool						m_bRotChange = false;
	_bool						m_bPosChange = false;
	_bool						m_bParent = false;

private:
	//// ==============���� ���==============
	//// �̹��� ��� ����� �����ϴ� ����
	//std::vector<std::string> m_vecImgPs =
	//{
	//	"../Bin/Resources/Textures/UI/Textures/Blood",
	//	"../Bin/Resources/Textures/UI/Textures/Buttons",
	//	"../Bin/Resources/Textures/UI/Textures/Crosshairs",
	//	"../Bin/Resources/Textures/UI/Textures/DeathScreen",
	//	"../Bin/Resources/Textures/UI/Textures/EnemyHUD",
	//	"../Bin/Resources/Textures/UI/Textures/EnemyIndicators",
	//	"../Bin/Resources/Textures/UI/Textures/Icons",
	//	"../Bin/Resources/Textures/UI/Textures/Interaction",
	//	"../Bin/Resources/Textures/UI/Textures/InText",
	//	"../Bin/Resources/Textures/UI/Textures/Inventory",
	//	"../Bin/Resources/Textures/UI/Textures/Journal",
	//};

	// ============== Ŭ���� ��� ==============
	// 	// Ŭ���� ����� �����ϴ� ����
	std::vector<std::string> m_vecParent =
	{
		"UI_Player_Left_Interface"
	};
	// Ŭ���� ����� �����ϴ� ����
	std::vector<std::string> m_vecClass =
	{
		"Anything",
		"Player_HPBar",
		"Player_HPFrame",
		"Blood_Lights",
		"Blood_Veins",
		"Blood_Vignette",
		"Player_Skill_Frame",
		"Player_Skill_Guige",
		"Player_Skill_Icon"
	};

private:
	ImGuiTabBarFlags m_Tab_bar_flags = ImGuiTabBarFlags_None;
public:
	static CWindow_UITool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

