#pragma once

#include "Imgui_Window.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CEnvironment_Instance;
class CEnvironment_Object;


class CWindow_MapTool final : public CImgui_Window
{
private:
	enum class TAP_TYPE { TAB_GROUND, TAB_INTERACT, TAB_ENVIRONMENT, TAB_END };
	enum class MODE_TYPE { MODE_CREATE, MODE_SELECT, MODE_DELETE, MODE_END };
	
	enum class MAP_KEY_TYPE //! �������̳� Ű
	{
		MODEL_GROUND, MODEL_ENVIRONMENT, MODEL_INTERACT, MODEL_END
	};

public:
	struct WINDOW_MAPTOOL_DESC : public ImGuiDESC
	{
	};

protected:
	CWindow_MapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CWindow_MapTool() = default;


public:
	virtual	HRESULT Initialize() override;
	virtual	void	Tick(_float fTimeDelta) override;	
	virtual void	Render() override;

private:
	virtual	HRESULT Save_Function() override;
	virtual HRESULT Load_Function() override;

private:
	HRESULT			Ready_ModelTags();
	

private:
	void			GroundTab_Function();
		//void			Create_EnvironmentFunction();
	void			InteractTab_Function();
		//void			Create_EnvironmentFunction();

	void			EnvironmentTab_Function();
		//void			Create_EnvironmentFunction();

private:
	void			MouseInfo_Window(_float fTimeDelta);
	
private:
	void			Preview_Function();
	void			Change_PreViewObject(TAP_TYPE eTabType);

private:
	ImTextureID		m_pSelectedTexture = { nullptr };

	TAP_TYPE		m_eTabType = TAP_TYPE::TAB_END;
	MODE_TYPE		m_eModeType = MODE_TYPE::MODE_END;


private:
	map<string, MAP_KEY_TYPE> m_mapNonAnimModelTag;
	map<string, MAP_KEY_TYPE> m_mapAnimModelTag;
	
	vector<string>  m_vecGroundModelTag;

	vector<string>  m_vecEnviroModelTag;
	vector<string>	m_vecAnimEnviroModelTag;

	vector<string>  m_vecInteractModelTag;
	vector<string>	m_vecAnimInteractModelTag;

private:
	_uint			m_iSelectModelTag = 0;

	//! for.PriviewObject //�̸������ ������Ʈ
	CEnvironment_Object*			m_pPreviewObject = {}; //! �̸����⸦ ���� Ŭ�н�Ų ������Ʈ.
	_uint							m_iOriginSelectModelTag = 0; 
	_float							m_fDeadWaiting = 0.1f; //! ��ƽ ���°� ��ٸ�������
	_float							m_fDeadWaitingAcc = 0.f;
	_bool							m_bDeadComplete = true;
	_bool							m_bChange = false;

private: //! �ʵ�
	class CField*	m_pField = { nullptr };
	_float			m_fFieldSizeX = {};
	_float			m_fFieldSizeZ = {};

private: //! ����ĳ��Ʈ
	RAY				m_tWorldRay = {};
	_float3			m_fRayPos = {};
	
	_float			m_fRayUpdateTime = { 0.1f };
	_float			m_fRayUpdateTimeAcc = { 0.f };

	

	//CGameObject*	m_pTestObject = { nullptr }; // ����� �׽�Ʈ

private:
	vector<CEnvironment_Object*>	m_vecCreateObject = {}; //! ������ ������Ʈ

private:
	vector<CEnvironment_Instance*>	m_vecCreateInstance = {};
	vector<CEnvironment_Object*>	m_vecPreViewInstance = {}; //! �ν��Ͻ� ��ũ���� ����� ����.
	vector<INSTANCE_INFO_DESC>		m_vecInstanceInfo = {}; //! m_vecPreViewInstance�� ��ŷ�ؼ� �ν��Ͻ� ��ũ������ ä�� ���� m_vecCreateInstance�� ���������
	




public:
	static CWindow_MapTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

