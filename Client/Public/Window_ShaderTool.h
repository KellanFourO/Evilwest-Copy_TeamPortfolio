#pragma once

#include "Imgui_Window.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CWindow_ShaderTool final : public CImgui_Window
{
protected:
	CWindow_ShaderTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CWindow_ShaderTool() = default;

public:
	virtual	HRESULT Initialize() override;
	virtual	void	Tick(_float fTimeDelta) override;
	virtual void	Render() override;

private:
	/* Top Setting */
	_bool bRenderTarget_Active = true;

	/* Level Load */
	_int		m_iCurrLevel_Index = {};
	LEVEL		m_eCurrLevel_Enum = {};
	wstring		m_wstrLayerTag = {};
	string		m_strStage1MapLoadPath = "../Bin/DataFiles/Data_Map/Test1_MapData.json";

	/* Level Shader Struct */
	HBAO_PLUS_DESC	m_eHBAO_Desc = {};
	FOG_DESC		m_eFog_Desc = {};
	HDR_DESC		m_eHDR_Desc = {};
	SCREEN_DESC		m_eScreen_Desc = {};

	LEVEL_SHADER_DESC m_eTotalShader_Desc = {};


private:
	/* �ֻ��� ���� */
	void Imgui_Setting();
	void Top_Setting();

	/* ���� �ҷ����� */
	void Select_Level();
	HRESULT Load_Level(_int iLevel_Index);

	/* Collapsing */
	void Layer_Level_Shader_Control();
	void Layer_Object_Shader_Control();

	/* Compress */
	void Compress_HBAO_Plus_Setting();
	void Compress_Fog_Setting();
	void Compress_HDR_Setting();
	void Compress_FXAA_Setting();
	void Compress_Screen_Setting();


public:
	static CWindow_ShaderTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END
