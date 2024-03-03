#pragma once

#include "Imgui_Window.h"

BEGIN(Engine)
class CGameObject;
class CCamera;
END

BEGIN(Client)
class CEnvironment_Instance;
class CEnvironment_Object;
class CPlayer;
class CMonster;
//TODO ���� �߰� class CNPC;

class CWindow_MapTool final : public CImgui_Window
{
private:
	enum class TAP_TYPE { TAB_SINGLE, TAB_INTERACT, TAB_ENVIRONMENT, TAB_NORMALMONSTER, TAB_BOSSMONSTER, TAB_NPC, TAB_END };
	enum class MODE_TYPE { MODE_CREATE, MODE_SELECT, MODE_DELETE, MODE_END };
	enum class PICKING_TYPE { PICKING_FIELD, PICKING_MESH, PICKING_INSTANCE, PICKING_NONE, PICKING_END };
	enum class PICKING_MODE { MOUSE_PRESSING, MOUSE_DOWN, MOUSE_UP};
	enum class OBJECTMODE_TYPE { OBJECTMODE_ENVIRONMENT, OBJECTMODE_CHARACTER};
	enum class ANIM_TYPE { TYPE_NONANIM, TYPE_ANIM };
	
	enum class MAP_KEY_TYPE //! �������̳� Ű
	{
		MODEL_SINGLE, MODEL_INSTANCE, MODEL_INTERACT, MODEL_END
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
	virtual	HRESULT Save_Function(string strPath, string strFileName) override;
	virtual HRESULT Load_Function(string strPath, string strFileName) override;
				void	Reset_Function();
				void	ObjectMode_Change_For_Reset();
private:
	//!For. Environment
	HRESULT			Ready_ModelTags();
	//!For. Character
	HRESULT			Ready_PrototypeTags();
	
private:
	void			EnvironmentMode_Function();
	void			CharacterMode_Function();


	//!For. Environment
	void			GroundTab_Function();
	void			InteractTab_Function();
	void			EnvironmentTab_Function();

	//!For. Character
	void			MonsterTab_Function();
	void			NPC_Tab_Function();



private:
	#ifdef _DEBUG
void			MouseInfo_Window(_float fTimeDelta);
#endif // _DEBUG
	void			FieldWindowMenu();
	void			CameraWindow_Function();
	void			IsCreatePlayer_ReadyCamara();
	
	//!For.Environment
	void			Select_ModeType();
	void			Select_PickingType();

	//!For.Character
	void			Select_CharacterModeType();
	
	//!For. Public
	void			Create_Tab(TAP_TYPE eTabType);
	void			Delete_Tab(TAP_TYPE eTabType);

	
private: //! For. Create_Function

	void			Picking_Function();
	void			Preview_Function();
	void			Change_PreViewObject(TAP_TYPE eTabType);

	//!For. Environment
	void			Ground_CreateFunction();
	void			Interact_CreateFunction();
	void			Preview_Environment_CreateFunction();
	void			Create_Instance();
	void			Anim_Environment_CreateFunction();

	//!For. Character
	void			Character_CreateFunction();
		void			Monster_CreateFunction();
		void			Boss_CreateFunction();
		void			NPC_CreateFunction();
	

private: //!For. Select_Function

	//!For. Environment
	void			Basic_SelectFunction();
	void			Instance_SelectFunction();
	void			Guizmo_Tick(CGameObject* pPickingObject = nullptr);

	void			Instance_GuizmoTick(_int iIndex, INSTANCE_INFO_DESC* pInstance = nullptr);

	//!For. Character
	void			Character_SelectFunction();
		void			Monster_SelectFunction();
		void			Boss_SelectFunction();
		void			NPC_SelectFunction();

private:
	ImTextureID		m_pSelectedTexture = { nullptr };

	TAP_TYPE		m_eTabType = TAP_TYPE::TAB_END;
	MODE_TYPE		m_eModeType = MODE_TYPE::MODE_END;
	PICKING_TYPE	m_ePickingType = PICKING_TYPE::PICKING_END;
	PICKING_MODE	m_ePickingMode = PICKING_MODE::MOUSE_PRESSING;
	OBJECTMODE_TYPE m_eObjectMode = OBJECTMODE_TYPE::OBJECTMODE_ENVIRONMENT;
	ANIM_TYPE		m_eAnimType = ANIM_TYPE::TYPE_NONANIM;

	_bool			m_bChangeObjectMode = false;
	

	CPlayer*		m_pPlayer = nullptr;
	
	_bool			m_bOnTheInstance = false;
	_float4x4		m_matInstanceMatrix = {};
	_float3			m_vRotation = {};
	_bool			m_bRotateMode = { false};

	_float			m_fCamaraSpeed = { 60.f };

//!  ����� ����� ����
	string			m_strLoadFilePath = {}; //! ���� �ҷ������ ���� �ҷ��Դٸ� �ҷ��� ���� �����θ� �����Ѵ�. �̻��¿��� Ctrl S�� ������ �ش� ��ο� ������ϴ� ������ ������ҰŰ���.

private: //!For. Character
	vector<string>			  m_vecMonsterTag;
	vector<string>			  m_vecBossTag;
	vector<string>			  m_vecNpcTag;
	 
	_uint					  m_iSelectCharacterTag = {};
	CGameObject*			  m_pPreviewCharacter = { nullptr };

private: //!For. Environment
	map<string, MAP_KEY_TYPE> m_mapNonAnimModelTag;
	map<string, MAP_KEY_TYPE> m_mapAnimModelTag;
	
	vector<string>  m_vecSingleModelTag;

	vector<string>  m_vecEnviroModelTag;
	vector<string>	m_vecAnimEnviroModelTag;

	vector<string>  m_vecInteractModelTag;
	vector<string>	m_vecAnimInteractModelTag;

	_bool			m_bShowCreateField = false;

private:
	_uint			m_iSelectModelTag = 0;

	//! for.PriviewObject //�̸������ ������Ʈ
	CEnvironment_Object*			m_pPreviewObject = {}; //! �̸����⸦ ���� Ŭ�н�Ų ������Ʈ.
	_uint							m_iOriginSelectModelTag = 0; 
	_float							m_fDeadWaiting = 0.1f; //! ��ƽ ���°� ��ٸ�������
	_float							m_fDeadWaitingAcc = 0.f;
	_bool							m_bDeadComplete = true;
	_bool							m_bChange = false;
	_bool							m_bAnimType = false;

	_int							m_iShaderPassIndex = {0};

private: //! �ʵ�
	class CField*	m_pField = { nullptr };
	_float			m_fFieldSizeX = { 20.f };
	_float			m_fFieldSizeZ = { 20.f };

private: //! ����ĳ��Ʈ
	RAY				m_tWorldRay = {};
	_float3			m_fRayPos = {};
	_float3			m_fMeshPos = {};
	_float3			m_fInstanceMeshPos = {};
	

	_float			m_fRayUpdateTime = { 0.1f };
	_float			m_fRayUpdateTimeAcc = { 0.f };


private:
	//!For. CreateObject
	vector<CEnvironment_Object*>	m_vecCreateObject = {}; //! ������ ������Ʈ
	vector<string>					m_vecCreateObjectTag = {};	
	_int							m_vecCreateObjectIndex = 0;
	_int							m_iCreateObjectIndex = 0;
	_int							m_iSelectObjectIndex = 0;

	CGameObject*					m_pPickingObject = { nullptr };
	INSTANCE_INFO_DESC*				m_pPickingInstanceInfo = { nullptr };

	//!For. CreateCharacter
	vector<CMonster*>				m_vecCreateMonster;
	vector<string>					m_vecCreateMonsterTag;

	//TODO ���� NPC�߰��Ǹ� �ۼ�
	//!vector<CNPC*>					m_vecCreateNPC;
	//!vector<string>					m_vecCreateNPCTag ;
	//!_int							m_iCreateNPCIndex = {};

	

	
	_int							m_iCreateMonsterIndex = {};
	_int							m_iSelectCharacterIndex = {};

private: //! For. CreateInstance
	_uint							m_iSelectInstanceIndex = 0;
	_uint							m_iSelectEnvironmentIndex = 0;

	_int							m_iCreatePreviewIndex = 0;
	_int							m_iSelectPreviewIndex = 0;
	vector<CEnvironment_Instance*>	m_vecCreateInstance = {};
	vector<CEnvironment_Object*>	m_vecPreViewInstance = {}; //! �ν��Ͻ� ��ũ���� ����� ����.
	vector<string>					m_vecPreViewInstanceTag = {};
	
	map<string, vector<CEnvironment_Object*>> m_mapPreviewInstance; //! ������ ���±׸��� ���� �ٸ� ���Ϳ� �����ؼ� �����ϰ�����.


	vector<string>					m_vecCreateInstanceTag = {};
	_int							m_iCreateInstanceIndex = 0;
	
	vector<string>					m_vecInstanceInfoTag = {}; //! m_vecPreViewInstance�� ��ŷ�ؼ� �ν��Ͻ� ��ũ������ ä�� ���� m_vecCreateInstance�� ���������
	_int							m_iInstanceInfoTagIndex = 0;

private:
	vector<CCamera*>				m_vecCameras;
	_bool							m_bCreateCamera = false;

public:
	static CWindow_MapTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

