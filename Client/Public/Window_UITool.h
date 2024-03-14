#pragma once

#include "Imgui_Window.h"
#include "UI_Anything.h"
#include "UI_TutorialBox.h"

BEGIN(Engine)
class CGameObject;
END

//class CUI_TutorialBox;


BEGIN(Client)
class CWindow_UITool final : public CImgui_Window
{
public: /* ==================== Struct ==================== */
	struct WINDOW_UITOOL_DESC : public ImGuiDESC
	{
	};

	enum CHANGETYPE { NONE, SCALE, ROTATION, POSITION, CHANGE_END };

	enum UITYPE { CHILD, PARENT, GROUP, TEXT, TYPE_END };

	enum TOOLSTATE { TOOL_UI, TOOL_ANIMATION, TOOL_END };

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
		string		strObjectName = "";
		string		strFileName = "";
		string		strFilePath = "";
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

protected: /* ============= Create/Extinction ============== */
	CWindow_UITool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CWindow_UITool() = default;


public: /* ===================== Basic =======================*/
	virtual	HRESULT				Initialize() override;
	virtual	void				Tick(_float fTimeDelta) override;
	virtual void				Render() override;

public: /* ====================== UI ========================= */



	void						UI_ToolTip(_float fTimeDelta);
	std::vector<unsigned char>	UI_LoadImage(const std::string& filename, int& width, int& height, int& channels);
	void						ShowImagePreview(const std::vector<unsigned char>& imageData, int width, int height);
	bool						LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
	//HRESULT						Update_Pos();

public: /* ==================== Shortcut_Key ================= */
	void						Shortcut_Key(_float fTimeDelta);

public: /* ==================== List List List List List List List List List List List List List List List List List List List List List List List List List List List List List List List ===================== */
	void						Layer_List();
	void						Texture_List();
	void						Parent_List(_float fTimeDelta);
	void						Child_List(_float fTimeDelta);

#pragma region	Parent
public:							/* Setting */
	void						Setting_Parent();
public:						    /* Create/Delete */
	HRESULT						Create_Parent(CUI::UI_DESC pUIDesc);
	void						Delete_Parent();
public:							/* List */
	void						Parent_Class(_float fTimeDelta);
	void						Parent_Object(_float fTimeDelta);
public:							/* Add */
	void						Add_ParentList(CUI::UI_DESC tIn_UI_Desc);
	void						Add_ParentIndexNumber(PATHINFO& UI_Info);
	void						Add_Create_Parts(CUI::UI_DESC tUIDesc);
public:							/* Group */
	void						Add_Parts(CUI* pUI);
	void						Delete_Group();

private:						/* Member */
	_float3						m_fParent_Scale = { 100.f, 100.f, 100.f };
	_float3						m_fParent_Rotation = { 0.f, 0.f, 0.f };
	_float3						m_fParent_Position = { 0.f, 0.f, 0.5f };

	_int						m_iSelected_ParentClassIndex = 0; // ���õ� Parent Class
	_int						m_iSelected_ParentObjectIndex = 0; // ���õ� Parent Object
	_int						m_iSelected_GroupObjectIndex = 0; // ���õ� Parent Object

	CUI::UI_DESC				m_tParent_Desc;
	CUI*						m_pCurrParent = nullptr;
	CUI*						m_pCurrGroup = nullptr;

	vector<CGameObject*>		m_vecParentObject;
	vector<CUI*>*				m_vecParentGroup = nullptr;
	std::vector<PATHINFO*>		m_vecParentObjectName;	// �߰��� Parent������Ʈ���� �̸� (����Ʈ �ڽ� ��¿�)
#pragma endregion Parent End


#pragma region	Child
public:							/* Setting */
	void						Setting_Child();
public:							/* Create/Delete */
	HRESULT						Create_Child(CUI::UI_DESC pUIDesc);
	void						Delete_Child(_float fTimeDelta);
	/* List */
	void						Child_Class(_float fTimeDelta);
	void						Child_Object(_float fTimeDelta);
public:							/* Add */
	void						Add_ChildIndexNumber(PATHINFO& str);
	void						Add_ChildList(CUI::UI_DESC tIn_UI_Desc);

private:						/* Member */
	_float3						m_fChild_Scale = { 0.f, 0.f, 0.f };
	_float3						m_fChild_Rotation = { 0.f, 0.f, 0.f };
	_float3						m_fChild_Possition = { 0.f, 0.f, 0.f };

	_int						m_iSelected_ChildClassIndex = 0;	// ���õ� Child Class
	_int						m_iSelected_ChildObjectIndex = 0; // ���õ� Child Object

	CUI::UI_DESC				m_tChild_Desc;
	CUI*						m_pCurrChild = nullptr;

	vector<CGameObject*>		m_vecChildObject;
	//std::vector<PATHINFO*>		m_vecChildObjectName;	// �߰��� ������Ʈ���� �̸� (����Ʈ �ڽ� ��¿�)
#pragma endregion Child End


#pragma region	Info
public:
	void						UI_Info();
	HRESULT						Menu_Info();
	void						Curr_Info();
	CUI* m_pCurrSelectUI = nullptr;
#pragma endregion Info End

#pragma region	Common
public:
	void						IndexCheck();
	void						Current_Info();
	void						Create_TargetTexture();
	void						Load_Paths();
#pragma endregion Common End

#pragma region	Text
public:
	void						Text_List();
	void						Text_Add();
	void						Find_Change(string strTextTag);
	void						Add_TextList(CUI::UI_DESC tUIDesc);
	void						Change_Text(string strTextKey, string strFontTag, string strText, _float fPosX, _float fPosY, _vector vColor, _float fScale, _float2 vOrigin, _float fRotation);
	_int						m_iSelected_TextClassIndex = 0; // ���õ� Text Class
	_int						m_iSelected_TextObjectIndex = 0; // ���õ� Text Object
	_int						m_iSelected_TextListIndex = 0; // ���õ� Text List

	char						m_cTextKey[MAX_PATH] = "";
	char						m_cTextFont[MAX_PATH] = "";
	char						m_cText[MAX_PATH] = "";
	char						m_cChangeTextTag[MAX_PATH] = "";
	_bool						m_bText = false;
	_bool						m_bFreeMove = false;
	_float						m_fPosX = 0.f;
	_float						m_fPosY = 0.f;
	_float						m_fScale = 5.f;
	_float						m_fRotation = 0.f;
	_float2						m_vOrigin = { 0.1f, 0.1f };
	_vector						m_vColor = { 1.f, 1.f, 1.f, 1.f };
	CUI::UI_DESC				m_tText_Desc;
	CUI*						m_pCurrText = nullptr;
	struct CUI_Text::TEXTINFO	m_tTextInfo;
	vector<CUI*>				m_vecTextObject;
	vector<string>				m_vecTextList =
	{
		"Defaul"
	};
#pragma endregion											Text End

#pragma region												Animation
	// Ű������ ���
	void						KeyframeList();
	// Ű������ ���� â
	void						KeyframeChangeWindow();
	// Ű������ ������ �� ���� (���� UI�� ���� ������)
	void						KeyframeRender_ValueChange();
	// Ű������ �ڵ� ���� �Լ�(���� ����)
	void CreateKeyframesWithLinearInterpolation(
												_float minTime, _float maxTime,
												_float minValue, _float maxValue,
												_float2 minScaleValue, _float2 maxScaleValue,
												_float minRotationValue, _float maxRotationValue,
												_float2 minTranslationValue, _float2 maxTranslationValue,
												_float _minTexture, _float _maxTexture,
												_int numKeyframes);
	// Ű������ �ڵ����� ���� �Լ�
	void	KeyframeAutomaticGeneration();
#pragma region												PlayAnim
	void	PlayAnimation(_float fTimeDelta);
	_bool	m_isPlayAnim = false;
	_float  m_fPlayTime = 0.f;
#pragma region												TimeLineBar
	void	AnimationTimeLineBar(_float fTimeDelta);// �ִϸ��̼� Ÿ�Ӷ��� ��
	_float	EvaluateAnimationAtTime(float time);	// �ִϸ��̼ǿ� ���� ���� �ð� ���
	void	KeyframeValueChange(_float fTimeDelta); // �ִϸ��̼� Ű������ ����
	void	ImGuiKeyInput();
	void	CurKeyframe_ValueChange();
	// ������ Ű������ ���� �����ϴ� �Լ�
	void	DrawSelectedKeyframeEditor(CUI::UIKEYFRAME& selectedKeyframe);
	void	SelectKeyframeMouse();
#pragma region Rect_Option
	// UV ����
	void	Setting_Distortion(_float fTimeDelta);
	_int	m_vUV_MaxTileCount[2] = { 7, 7 };
	_float	m_fSequenceTerm_RectSprite = { 0.05f };

	/* Distortion */
	_float m_fSequenceTerm_Distortion = { 1.f };

	_float	m_vScrollSpeeds[3] = { 1.f, 1.f, 0.f };
	_float	m_vScales_Distortion[3] = { 1.f, 1.f, 1.f };

	_float	m_vDistortion1[2] = { 0.1f, 0.1f };
	_float	m_vDistortion2[2] = { 0.0f, 0.0f };
	_float	m_vDistortion3[2] = { 0.0f, 0.1f };

	_float	m_fDistortionScale = { 1.f };
	_float	m_fDistortionBias = { 1.f };

#pragma endregion

	// ������ �Ӽ� �� ���
	enum EDITMODE
	{
		EDITMODE_NONE,
		EDITMODE_SCALE,
		EDITMODE_ROTATION,
		EDITMODE_TRANSLATION,
		EDITMODE_TIME_VALUE,
		EDITMODE_TEXTURE
	};
	EDITMODE eEditMode = EDITMODE_NONE; // ������ ��� ����
	float vValueSize = 1.f;	// ������ �� ���� ũ��
	_float originalIndex;
	// Ű������ �巡��
	_bool	isDraggingKeyframe = false;
	_int	draggingKeyframeIndex = 0;
	_float  initialKeyframeX = 0.f;					// ���콺 �巡�� ���� ��ġ
	_float	m_fFrame[100];							// �ð�
	_float	m_fPlaybackSpeed = 1.0f;				// ��� �ӵ�
#pragma endregion
	// _vec3 ���� ���� �Լ�
	_float2						Lerp_float2(const _float2& a, const _float2& b, float t);

	// _vec2 ���� ���� �Լ�
	_float						Lerp_float(const _float& a, const _float& b, float t);

	// �����ƽ ��¡(InQuad) �Լ� (����)
	_float						ImEaseInQuad(float start, float end, float t);

	// �����ƽ ��¡(OutQuad) �Լ� (��)
	_float						ImEaseOutQuad(float start, float end, float t);

private:
	// �ִϸ��̼� Ÿ�� ����
	std::vector<CUI::UIKEYFRAME>*	m_vecTimeline = nullptr;
	_float currentTime = 0.0f; // ���� �ð� ��
	_float MaxTime = 20.f;
	_float fDisplayTime = 1.f;	// ���ݸ����� �ð� �� ǥ��
	_bool isDraggingTimeline = false;
	_bool m_bRendomValue = false; // ���� �� �������� �ֱ�
	_int  closestKeyframeIndex = -1; // # ���� ���콺�� ������ Ű������ �ε���
	// �ִϸ��̼��� Ÿ��
	ImVec2					timelinePos = { 0.f, 0.f };
	ImVec2					timelineSize = { 800.f, 85.f }; // �ִϸ��̼� Ÿ�� ���� ũ��

	_int m_iOldIndex = -1;
#pragma region											�ּ�, �ִ� ��
// ũ��
	_float	fMin_Scale = -2000.0001f;	// �ּ�
	_float	fMax_Scale = 2000.f;	// �ִ�

	// ȸ��
	_float	fMin_Rot = -360.0f;		// �ּ�
	_float	fMax_Rot = 360.f;	// �ִ�

	// �̵�
	_float	fMin_Pos = -5000.f;		// �ּ�
	_float	fMax_Pos = 5000.f;	// �ִ�

	// �ð�
	_float	fMin_Time = 0.f;	// �ּ�
	_float	fMax_Time = MaxTime;// �ִ�

	// ����
	_float	fMin_Value = 0.f;	// �ּ�
	_float	fMax_Value = 1.f;	// �ִ�
#pragma endregion

#pragma region											�ּ�, �ִ� �ڵ� ���� ��
	_bool  m_bIndividualTexture = false;

	_float _v2Time[2] = { 0.f, 1.f };
	_float _v2Value[2] = { 0.f, 0.7f };
	_float minScale[2] = { 50.f, 50.f };
	_float maxScale[2] = { 50.f, 50.f };
	_float minRot = { -360.f };
	_float maxRot = { 360.f };
	_float minPos[2] = { 0.f, 0.f };
	_float maxPos[2] = { 0.f, 0.f };

	// �ؽ�ó ������
	_float	fMin_Texture = 0.f;	// �ּ�
	_float	fMax_Texture = 0.f;	// �ִ�

	_float numKeyframes = 1.f;
#pragma endregion
	// Test
	_bool	m_bCreateOK = false;
	_bool	m_bRepetition = false;
#pragma endregion

public: /* Save/Load */
	virtual	HRESULT				Save_Function(string strPath, string strFileName) override;
	virtual HRESULT				Load_Function(string strPath, string strFileName) override;

public: /* ================= Function ================= */
	// stringŸ���� �޴� ���� �����̳ʸ� char*Ÿ���� �޴� ���� �����̳ʷ� ��ȯ ���ִ� �Լ�
	std::vector<const char*> ConvertStringVectorToCharArray(const std::vector<std::string>& stringVector);
	// string�� wstring���� ��ȯ ���ִ� �Լ�
	std::wstring ConvertToWideString(const std::string& ansiString);
	// string�� wchar�� ��ȯ ���ִ� �Լ�
	WCHAR*		StringTowchar(const std::string& str);
	// wstring�� string���� ��ȯ ���ִ� �Լ�
	std::string WStringToString(const std::wstring& wstr);
	//	wstring�� char�� ��ȯ ���ִ� �Լ�
	char*		ConverWStringtoC(const wstring& wstr);
	//	char�� wchar_t�� ��ȯ ���ִ� �Լ�
	wchar_t*	ConverCtoWC(char* str);
	// WCHAR*�� string���� ��ȯ ���ִ� �Լ�
	std::string WideStringToString(const wchar_t* wideStr);
	// ��ο��� �����̸��� Ȯ���ڸ� �������ִ� �Լ�
	std::string GetFileName(const std::string& filePath);
	// _�������� ���� ������ �̸��� �������ִ� �Լ�
	std::string GetUnderbarName(const std::string& filePath);
	// Ȯ���ڸ� �������ִ� �Լ�
	std::string RemoveExtension(const std::string& filePath);



public: /* Image */
	void						LoadImgPath(const _tchar* folderPath);
	void						ImagePreview(_float fTimeDelta);

private: /* ==================== Mouse ==================== */
	POINT						m_pt;

private: /* Image_Member */
	_int						m_My_Image_Width = 100;		// �̸����� �̹��� ���λ�����
	_int						m_My_Image_Height = 100;	// �̸����� �̹��� ���λ�����
	_int						m_iTextureNum = 0;			// �ؽ�ó ����
	vector<IMAGEINFO*>			m_vecTexture;				// �̹��� �̸����⸦ ���� �ؽ�ó ������

	vector<PATHINFO*>			m_vecInitialPaths;			// 1. ������ ��ȸ�Ͽ� ���, �����̸��� ó������ ��ƿ��� �༮ (������Ÿ�� �Ľ̰���)
	std::vector<PATHINFO*>		m_vecImagePaths;			// 2. ��ƿ� �༮���� ���� ��ο� �̸����� �̹��� �ε带 ���� �༮
	_int						m_iSelectedPathIndex = 0;	// ���õ� �̹��� ��� �ε���

private: /* 2D */
	string						m_strLayer[3] = { "Layer_UI_Player", "Layer_UI_Monster", "Layer_UI_Inventory" };
	_int						m_iCurrLayerNum = 0;


private: /* enum */
	UITYPE						m_eUIType;
	TOOLSTATE					m_eToolType = TOOL_UI;

private: /* Value */
	_float						m_fChangeValue = 0.1f;
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
	_bool						m_bGroupObject = false;

private:
	_bool						m_bCheckImguiRect_Child = false;
	_bool						m_bCheckImguiRect_Parent = false;
	_bool						m_bCheckImguiRect_UI_Animation = false;
	_bool						m_bCheckImguiRect_ValueChange = false;
	_bool						m_bCheckImguiRect_UI_TimeLine = false;
	_bool						m_bCheckImguiRect_Info = false;
	_bool						m_bCheckImguiRect_UI_Info = false;
	_bool						Check_ImGui_Rect();

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
		"Player_Left_Interface",
		"Player_Right_Interface",
		"QuestBox"
	};

	// Ŭ���� ����� �����ϴ� ����
	std::vector<std::string> m_vecClass =
	{
		"Anything",
		"BackGround",
		"Loading_Icon",
		"Player_HPBar",
		"Player_HPFrame",
		"Player_Skill_Frame",
		"Player_Skill_Guige",
		"Player_Skill_Icon",
		"Player_ExpBar",
		"Blood_Lights",
		"Blood_Veins",
		"Blood_Vignette",
		"TutorialBox",
		"QuestIcon",
		"ItemShowcase",
		"ItemIcon",
		"LevelUp_MagicFrame",
		"LevelUp_MagicTrack",
		"LevelUp_Shards",
		"LevelUp_MagicSide",
		"LevelUp_SunBeacon",
		"LevelUp_TextBox",
		"LevelUp_ShieldFrame",
		"MouseCursor",
		"Option_Window",
		"AimCrosshair",
		"Weakness",
		"Distortion",
		"EnemyHP_FrameBar",
		"EnemyHP_Bar"
	};

	// Ŭ���� ����� �����ϴ� ����
	std::vector<std::string> m_vecTextBoxClass =
	{
		"TutorialBox",
		"QuestBox"
	};

private:
	void	UI_Preset();
	class CUI_Manager*	m_pUI_Manager = nullptr;
	_bool				m_bDisappear = false;

private:
	ImGuiTabBarFlags m_Tab_bar_flags = ImGuiTabBarFlags_None;

public:
	static CWindow_UITool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

