#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Renderer.h"

class CUI abstract : public CGameObject
{
public:
	enum UI_KIND { NORMAL, TEXT, KIND_END };

	enum KEYTYPE
	{
		KEYTYPE_NONE,
		KEYTYPE_NORMAL,
		KEYTYPE_END
	};

	// Ű������ ����ü
	typedef struct tagUIKeyframe
	{
		_float	fTime = 0.f;					// Ű�������� �ð� (0.0f ~ MaxTime ����)
		_float	fValue = 0.f;					// �ִϸ��̼� �� (ũ��, ȸ��, �̵� ��)
		_float	fAnimSpeed = 0.f;				// �ִϸ��̼� ��� �ӵ�

		_int	iType = 0;						// �ִϸ��̼� Ÿ�� (0: ũ��, 1: ȸ��, 2: �̵�)

		_bool	isEaseIn = false;				// Ease In ���� (True �Ǵ� False)
		_bool	isEaseOut = false;				// Ease Out ���� (True �Ǵ� False)

		_int	iTexureframe = 0;				// �ؽ�ó ���� ��

		_float2	vScale = { 0.f, 0.f };			// ũ�⸦ ���� �׸�
		_float	fRot = 0.f;						// ȸ���� ���� �׸�
		_float2	vPos = { 0.f, 0.f };			// ��ġ�� ���� �׸�

		_float2	vKeyFramePos = { 0.00000000f, 0.00000000f };	// �������� �ش� Ű������ ��ġ

	}UIKEYFRAME;


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

		_int		iKeyframeNum = 0;

	}UI_ANIMKEYFRAME;

	typedef struct tagUIDesc /* ����/�ҷ����� �Ҷ� ���� �� ���߱� */
	{
		_bool		bParent = false;
		_bool		bWorld = false;
		_bool		bGroup = false;

		_float		fOrigin_ScaleX = 100.f;
		_float		fOrigin_ScaleY = 100.f;

		_float		fOrigin_fRotationZ = 0.f;

		_float		fOrigin_fPositionX = 0.f;
		_float		fOrigin_fPositionY = 0.f;


		/* ũ�� */
		_float		fScaleX = 100.f;
		_float		fScaleY = 100.f;

		/* ȸ�� */
		_float		fRotationZ = 0.0f;

		/* �̵� */
		_float		fPositionX = (_float)g_iWinSizeX / 2;
		_float		fPositionY = (_float)g_iWinSizeY / 2;
		_float		fPositionZ = 0.5f;

		/* 
			(���� ����)
			=�θ� ����=
			bParent
			=���� ����=
			bWorld
			=ũ�� ����=
			fScaleX = 1
			fScaleY = 1
			=ȸ�� ����=
			fRotationX
			fRotationY
			=��ġ ����=
			fPositionX
			fPositionY
			fPositionZ
			=���� ����=
			fAlpha
			=������Ʈ �ѹ�=
			iObjectNum
			=���̴� �н� ����=
			iShaderNum
			=�ױ� ��=
			strObjectName
			strLayerTag
			strCloneTag
			strProtoTag
			strFilePath
			strMapTextureTag
			=����=
			vColor
		*/

		/* ���� */
		_float		fAlpha = 1.f;

		_int		iObjectNum = 0;			// ���° �༮����
		_int		iShaderNum = 0;			// ������ ���̴� �ѹ�

		string		strObjectName = "";
		string		strLayerTag = "";
		string		strCloneTag = "";
		string		strProtoTag = "";
		string		strFilePath = "";
		string		strMapTextureTag = "";	// ������ �� �ؽ�ó

		/* ���� */
		_vector		vColor = { 1.f, 1.f, 1.f, 1.f };

		class CTransform* pParentTransformCom = nullptr;
	}UI_DESC;

	enum UI_BUTTON_STATE
	{
		BUTTON_NORMAL, BUTTON_PICKING, BUTTON_PICKED, BUTTON_END
	};

protected:
	CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CUI(const CUI& rhs);
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
	// =>Kind
	void			Set_Kind(UI_KIND eUI_King) { m_eKind = eUI_King; }
	UI_KIND			Get_Kind() { return m_eKind; }


//protected:
public:
	virtual HRESULT	Set_ParentTransform(CTransform* pParentTransformCom);

	
protected:
	void			SetUp_WorldToScreen(_fvector vWorldPos);

public: /* ============================== Add ============================== */
	void			Add_Create_Parts(void* pArg);
	void			Add_Parts(CUI* pArg);

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
	virtual void	Parts_Delete();

public: /* ============================== SetUp ============================== */
	HRESULT			SetUp_UIRect(_float fPosX, _float fPosY, _float fSizeX = 1.f, _float fSizeY = 1.f);
	HRESULT			SetUp_Transform(_float fPosX, _float fPosY, _float fScaleX, _float fScaleY);
	HRESULT			SetUp_BillBoarding();
	HRESULT			Ready_UI(const char* cFilePath);
	HRESULT			Create_UIParts(UI_DESC tUI_Desc);
	HRESULT			Update_Child_Transform();

	// error : �� �ڵ� ��Ҹ� ��ȯ���� ���߽��ϴ�. ���� ������ �� �ֽ��ϴ�. -> �ذ��� : ������Ʈ ���� ���� vs���� ���� �� ����
	void			Tick_LevelUp(_float fTimeDelta);

public:
#ifdef _DEBUG
	/* (�����̳��� �ּҸ� �޾ƿ��°� ������ ��忡�� ������ ���װ�����. ����) */
	vector<CUI*>*	Get_vecUIParts() { return &m_vecUIParts; }
#endif // DEBUG
	string			Get_FilePathTag() { return m_tUIInfo.strFilePath; }
	string			Get_ObjectNameTag() { return m_tUIInfo.strObjectName; }
	_int			Get_ObjectNum() { return m_tUIInfo.iObjectNum; }

protected: /* =========================== Ready ============================= */
	virtual HRESULT Ready_Components();
	virtual HRESULT Bind_ShaderResources();

public: /* =========================== Save/Load ============================== */
	void			Load_UIData(const char* _FilePath);
	virtual json	Save_Desc(json& out_json);

public: /* =========================== Animation ============================== */
	void			Play_Animation();
	// �ִϸ��̼� ��
	std::vector<UIKEYFRAME>* m_vecAnimation[KEYTYPE_END] = {};
	KEYTYPE				m_eKeyframe = KEYTYPE_NORMAL;
	_int				m_iTextureNum[KEYTYPE_END];

	void				Set_AnimPlay(_bool bPlay) { m_bPlayAnim = bPlay; }
	_bool				Get_AnimPlay() { return m_bPlayAnim; }
	_bool				m_bPlayAnim = false;

	void				Set_CurrTime(_float fCurrTime) { m_fCurrTime = fCurrTime; }
	_float				Get_CurrTime() { return m_fCurrTime; }
	_float				m_fCurrTime = 0.f;

	void				Set_Repetition(_bool bRepetition) { m_bRepetition = bRepetition; }
	_bool				Get_Repetition() { return m_bRepetition; }
	_bool				m_bRepetition = false;
	// dt ��
	_float fFrameTimeDelta, fCurFrameTimeDelta;

	// ũ��
	_float fSizeX_Delta, fSizeY_Delta;

	// ȸ��
	_float fRotX_Delta, fRotY_Delta, fRotZ_Delta;

	// �̵�
	_float fPosX_Delta, fPosY_Delta;

protected: /* Data */
	class CData_Manager* m_pData_Manager = { nullptr };

protected: /* LifeTime */
	void				LifeTime_LevelUp(_float fTimeDelta);
	_bool				m_bEventOn = false;
	_float				m_fLifeTime = 5000.f;
	_float				m_fTime = (_float)GetTickCount64();

protected: /* ========================= Component =========================== */
	CShader*			m_pShaderCom = { nullptr };
	//CTexture*			m_pTextureCom;
	CTexture*			m_pMapTextureCom = { nullptr };	// ������ �� �ؽ�ó
	_int				m_iShaderNum;		// ������ ���̴� �ѹ�
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	_vector				m_vAxis = { 0.f, 0.f, 0.f, 0.f };

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
	_float4x4			m_Origin_WorldMatrix = {};
	_bool				m_bActive = false;
	// UI_Member
	_float				m_fPositionX = 0.f, m_fPositionY = 0.f;
	_float				m_fScaleX = 0.f, m_fScaleY = 0.f, m_fScaleZ = 0.1f;
	UI_KIND				m_eKind = NORMAL;

	// ����
	_float				m_fAlpha = 0.f;

protected: /* ============================ bool =============================== */
	_bool				m_bPick = false;
	_uint				m_iButtonState = {};

public:
	virtual void		 Free() override;

};