#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Renderer.h"

class CUI abstract : public CGameObject
{
public:
	// �ִϸ��̼� ������ �ּ� �� �ִ� ���� �Է� �޴� ����ü (Ű������)
	typedef struct tagUI_AnimKeyframe 
	{
		_float		fminTime = 0.0f;
		_float		fmaxTime = 20.f;
		_float		fminValue = 0.0f;
		_float		fmaxValue = 1.0f;
		_float3		vminScale = { 0.01f, 0.01f, 0.f };
		_float3		vmaxScale = { 5.f, 5.f, 5.f };
		_float3		vminRotation = { 0.f, 0.f, 0.f };
		_float3		vmaxRotation = { 360.f, 360.f, 360.f };
		_float3		vminPosition = { 0.f, 0.f, 0.f };
		_float3		vmaxPosition = { 300.f, 300.f, 300.f };
	}UI_ANIMKEYFRAME;

	typedef struct tagUIDesc /* ����/�ҷ����� �Ҷ� ���� �� ���߱� */
	{
		_bool		bParent = false;
		_bool		bWorld = false;

		/* ũ�� */
		_float		fScaleX = 100.f;
		_float		fScaleY = 100.f;

		/* ȸ�� */
		_float		fRotationX = 0.0f;
		_float		fRotationY = 0.0f;

		/* �̵� */
		_float		fPositionX = (_float)g_iWinSizeX / 2;
		_float		fPositionY = (_float)g_iWinSizeY / 2;
		_float		fPositionZ = 0.0f;

		/* ���� */
		_float		fAlpha = 1.f;

		/* ���� */
		_vector		vColor = { 1.f, 1.f, 1.f, 1.f };

		/* 
			=�ؽ�ó �������� ���� ������=
			Texture�� ������ ���� �𸣴�, vector �����̳ʷ� ����, �Ѱ��� �����ŭ ������ �ؽ�ó�� ���������. 
			CTexture�� ���������̳ʷ� �����Ͽ� ������ �ؽ�ó �ױ� �� ��ŭ �ؽ�ó�� ���� push_back ������.
			mapTexture�� ��������. (�Ľ����� �ٲ�����) (Ŭ����/��ü ���� ��ҵ�)
			����ü �ϳ� �ļ� ���̴� �н��� ���� �����ϸ� �����Ű���.
			
			=UV��ǥ�� �ؽ�ó�� ��������=

			=UI�� ����Ÿ���� ���� �����, �����ϰ���� ȿ������ �����Ű��=

			=UI�� ����Ÿ�ٿ� �׷��� UI���� �ؽ�ó�� ������ �� �ְ� ��������� (Ʋ �����ϱ�)=

			=>���� : Ŭ�������� ��ɰ� � �༮���� ������������ �ؽ�ó�� ���� ������ ���� �ʿ���� �ϼ��� Ŭ������ �����, ������ �ϼ����� �����Ͽ� ��ġ����.

		*/

		string		strCloneTag; 
		string		strProtoTag;
		string		strFilePath;
		string		strMapTextureTag;	// ������ �� �ؽ�ó
		_int		iShaderNum;			// ������ ���̴� �ѹ�

	}UI_DESC;

	enum UI_BUTTON_STATE
	{
		BUTTON_NORMAL, BUTTON_PICKING, BUTTON_PICKED, BUTTON_END
	};

protected:
	CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI(const CGameObject& rhs);
	virtual ~CUI() = default;

public: /* ============================== Get / Set =============================== */
	// =>UIDesc
	UI_DESC			Get_UIDesc() { return m_tUIInfo; }
	void			Set_UIDesc(UI_DESC UIDesc) { m_tUIInfo = UIDesc; }
	// =>Pickup
	_bool			Get_Pick() { return m_bPick; }
	_bool			Set_Pick(_bool Pick) { m_bPick = Pick; }
	// =>Part
	CUI*			Get_UIPart(const wstring& strPartTag);
	// =>Parts
	vector<CUI*>	Get_UIParts();
	// =>Button
	_uint			Get_ButtonState() { return m_iButtonState; }
	_uint			Set_ButtonState(_uint iButtonState) { m_iButtonState = (UI_BUTTON_STATE)iButtonState; }
	// =>Position
	void			Set_Pos(_float fPosX, _float fPosY);
	void			Set_PosZ(_float fZ);

public: /* ========================== Change_Size ========================== */
	void			Set_Size(_float fSizeX, _float fSizeY);
	void			Change_SizeBottom(_float MMY);
	void			Change_SizeTop(_float MMY);
	void			Change_SizeLeft(_float MMX);
	void			Change_SizeRight(_float MMX);
	void			Change_SizeY(_float MMY);
	void			Change_SizeX(_float MMX);

public: /* ============================== Basic =============================== */
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Priority_Tick(_float fTimeDelta);
	virtual void	Tick(_float fTimeDelta);
	virtual void	UI_AppearTick(_float fTimeDelta);
	virtual void	UI_DisappearTick(_float fTimeDelta);
	virtual void	Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public: /* ============================= Function ============================= */
	virtual void	Picking_UI();	// Pick
	virtual void	Check_RectPos();	// Moving
	void			Moving_Picking_Point(POINT pt); // Picking Moving

public: /* ============================== SetUp ============================== */
	HRESULT			SetUp_UIRect(_float fPosX, _float fPosY, _float fSizeX = 1.f, _float fSizeY = 1.f);
	HRESULT			SetUp_BillBoarding();
	HRESULT			SetUp_Transform(_float fPosX, _float fPosY, _float fSizeX = 1.f, _float fSizeY = 1.f);
	HRESULT			Ready_UI(UI_DESC tUI_Desc);
	HRESULT			Create_UIParts(UI_DESC tUI_Desc);

protected: /* =========================== Ready ============================= */
	virtual HRESULT Ready_Components();
	virtual HRESULT Bind_ShaderResources();

protected: /* =========================== Load ============================== */
	void			Load_UIData(const char* _FilePath);

protected: /* ========================= Component =========================== */
	CShader*			m_pShaderCom = { nullptr };
	//CTexture*			m_pTextureCom;
	CTexture*			m_pMapTextureCom = { nullptr };	// ������ �� �ؽ�ó
	_int				iShaderNum;		// ������ ���̴� �ѹ�
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };

protected: /* =========================== Space ============================ */
	_float4x4			m_ViewMatrix, m_ProjMatrix;

protected: /* =========================== Screen ============================ */
	_float				m_fWorldToScreenX = 0.f;
	_float				m_fWorldToScreenY = 0.f;

protected: /* ============================= UI =============================== */
	vector<CUI*>		m_vecUIParts;
	UI_DESC				m_tUIInfo;
	RECT				m_rcUI = {};
	UISTATE				m_eState;
	
	// UI_Member
	_float				m_fPositionX = 0.f, m_fPositionY = 0.f;
	_float				m_fScaleX = 0.f, m_fScaleY = 0.f;

protected: /* ============================ bool =============================== */
	_bool				m_bPick = false;
	_uint				m_iButtonState = {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void		 Free() override;

};