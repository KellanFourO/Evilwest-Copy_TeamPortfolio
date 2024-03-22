#pragma once

#include "Imgui_Window.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CWindow_ShaderTool final : public CImgui_Window
{
public:
	typedef struct tagParsed_Data
	{

	}PARSED_DATA;

protected:
	CWindow_ShaderTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CWindow_ShaderTool() = default;

public:
	virtual	HRESULT Initialize() override;
	virtual	void	Tick(_float fTimeDelta) override;
	virtual void	Render() override;
	_bool			Check_ImGui_Rect();

private:
	/* Imgui Window Click */
	_bool Main_Window = {};
	_bool ObjectList_Window = {};

	/* Top Setting */
	_bool bRenderTarget_Active = true;
	_bool bRenderCom_Active = true;
	_bool m_bObjectCreateWindwo = { true };				/* â ������ bool���� */

	/* Level Load - string�� �ٲ���� */
	_bool		m_bCreate_Level_Button = false;
	_int		m_iCurrLevel_Index = {};
	LEVEL		m_eCurrLevel_Enum = {};
	wstring		m_wstrLayerTag = {};
	
	/* Load Path */
	string			m_strStage1MapLoadPath = {};

	/* Object Load - string���� �� Ű�� �޾ƿ� */
	vector<string>	m_vObjectModelTag;
	_bool			m_bObject_Layer_Button = false; /* ������Ʈ ������ ���� ����Ʈ ���� ��ư */
	string			m_strCurrentObjectTag = {};
	_bool			m_bCreateObject_Button = false; /* ������Ʈ ���� �����ϴ� ��ư */
	class ShaderParsed_Object* pObject = nullptr;

	/* Level Shader Struct */
	HBAO_PLUS_DESC		m_eHBAO_Desc			= {};
	FOG_DESC			m_eFog_Desc				= {};
	DEFERRED_DESC		m_eDeferred_Desc		= {};

	RADIAL_DESC			m_eRadial_Desc			= {};
	DOF_DESC			m_eDOF_Desc				= {};
	HDR_DESC			m_eHDR_Desc				= {};
	ANTI_DESC			m_eAnti_Desc			= {};
	HSV_DESC			m_eHSV_Desc				= {};
	LEVEL_SHADER_DESC	m_eTotalShader_Desc		= {};

	/* Level LightControl */
	LEVEL			m_eLoadLevel = {};
	string			strPath;
	string			strFileName;
	_bool			m_bSave = false;
	_bool			m_bLoad = false;


	virtual	HRESULT		Save_Function(string strPath, string strFileName) override;
	virtual	HRESULT		Load_Function(string strPath, string strFileName) override;
	virtual _bool		Write_Json(json& Out_Json);

private:
	/* �ֻ��� ���� */
	void Imgui_Setting();
	void Top_Setting();
	void Choice_Level_N_Object();

	/* ���� �ҷ����� */
	void Select_Level();
	HRESULT Load_Level(_int iLevel_Index);
	HRESULT Load_LevelShader(_int iLevel_Index);

	/* ������Ʈ �ҷ����� */
	void Create_Object();
	void Show_N_Create_ObjectList();
	void Create_DummyObject(string ObjectTag);

	/* Collapsing */
	void Layer_Light_Control();
	void Layer_Level_Shader_Control();
	void Layer_Object_Shader_Control();

	/* Compress */
	void Compress_HBAO_Plus_Setting();
	void Compress_Fog_Setting();
	void Compress_BloomRim_Setting();

	void Compress_Radial_Setting();
	void Compress_DOF_Setting();
	void Compress_HDR_Setting();
	void Compress_FXAA_Setting();
	void Compress_HSV_Setting();

	/* Level Light Control */
	void Save_Load_Light(); // ������ ��� �ش� ������ ���� �������� �������� 
	void Compress_Directional_Light();

public:
	static CWindow_ShaderTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

