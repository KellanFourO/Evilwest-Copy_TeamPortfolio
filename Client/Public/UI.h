#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Renderer.h"

BEGIN(Engine)
class CCharacter;
END

BEGIN(Client)

class CUI abstract : public CGameObject
{
public:
	enum DISTORTIONKIND { MASK, NOISE, DISTORTION_END };
	enum UI_KIND { NORMAL, TEXT, KIND_END };
	enum UI_STATE {};

	// Ű������ ����ü
	typedef struct tagUIKeyframe
	{
		_float	fTime = 0.f;					// Ű�������� �ð� (0.0f ~ MaxTime ����)
		_float	fValue = 0.f;					// �ִϸ��̼� �� (ũ��, ȸ��, �̵� ��)
		_float	fAnimSpeed = 1.f;				// �ִϸ��̼� ��� �ӵ�

		_int	iType = 0;						// �ִϸ��̼� Ÿ�� (0: ũ��, 1: ȸ��, 2: �̵�)

		_bool	isEaseIn = false;				// Ease In ���� (True �Ǵ� False)
		_bool	isEaseOut = false;				// Ease Out ���� (True �Ǵ� False)

		_int	iTexureframe = 0;				// �ؽ�ó ���� ��

		_float2	vScale = { 50.f, 50.f };			// ũ�⸦ ���� �׸�
		_float	fRot = 0.f;						// ȸ���� ���� �׸�
		_float2	vPos = { 10.f, 10.f };			// ��ġ�� ���� �׸�

		_float2	vKeyFramePos = { 0.00000000f, 0.00000000f };	// �������� �ش� Ű������ ��ġ

		_float	fAlpha = 1.f;

		/* ���� �߰� */
		_bool	bActive = false;
		_bool	bAppear = true;
		_bool	bDisappear = false; // Disappear
		_bool	bLoopSection = false; // Disappear
		_bool	bTrigger = false;

		/* Distortion */
		_bool		bDistortionUI = false;
		_bool		bRestore = false;
		_float		fTimeAcc = 0.f;
		_float		fSequenceTerm = 0.f;
		_float3		vScrollSpeeds = { 0.f, 0.f, 0.f };
		_float3		vScales = { 0.f, 0.f, 0.f };
		_float2		vDistortion1 = { 0.f, 0.f };
		_float2		vDistortion2 = { 0.f, 0.f };
		_float2		vDistortion3 = { 0.f, 0.f };
		_float		fDistortionScale = 0.f;
		_float		fDistortionBias = 0.f;

		_bool		bStopPlay = false;		// ����
		_bool		bReversePlay = false;	// �����
		_bool		bMaskChange = false;	// ����ũ �ؽ�ó ����
		_bool		bNoiseChange = false;	// ������ �ؽ�ó ����
		_int		iMaskNum = 0;			// ����ũ �ؽ�ó ��ȣ
		_int		iNoiseNum = 0;			// ������ �ؽ�ó ��ȣ

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

		_int		iUINum = 0;			// ���� UI����
		string		strUIName = "";			// ���� UI����

		string		strObjectName = "";
		string		strLayerTag = "";
		string		strCloneTag = "";
		string		strProtoTag = "";
		string		strFilePath = "";
		string		strMapTextureTag = "";	// ������ �� �ؽ�ó
		string		strButton_Name = "";	// ������ �� �ؽ�ó

		/* ���� */
		_vector		vColor = { 1.f, 1.f, 1.f, 1.f };

		class CTransform* pParentTransformCom = nullptr;

		_int		iKeyframeNum = 0;
		UIKEYFRAME	tKeyframe;

		/* Distortion */
		_float		fTimeAcc = 1.f;
		_float		fSequenceTerm = 0.f;
		_float3		vScrollSpeeds = { 0.f, 0.f, 0.f };
		_float3		vScales = { 0.f, 0.f, 0.f };
		_float2		vDistortion1 = { 0.f, 0.f };
		_float2		vDistortion2 = { 0.f, 0.f };
		_float2		vDistortion3 = { 0.f, 0.f };
		_float		fDistortionScale = 0.f;
		_float		fDistortionBias = 0.f;

		_bool		bDistortionUI = false;
		_int		iMaskNum = 0;
		_int		iNoiseNum = 0;

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
	// =>Distortion
	_bool			Get_DistortionUI() { return m_tUIInfo.bDistortionUI; }
	void			Set_ScrollSpeeds(_float3 vScrollSpeeds) { m_tUIInfo.vScrollSpeeds = vScrollSpeeds; }
	void			Set_SequenceTerm(_float fSequenceTerm) { m_tUIInfo.fSequenceTerm = fSequenceTerm; }
	void			Set_DistortionScales(_float3 vScales) { m_tUIInfo.vScales = vScales; }
	void			Set_Distortion1(_float2 vDistortion1) { m_tUIInfo.vDistortion1 = vDistortion1; }
	void			Set_Distortion2(_float2 vDistortion2) { m_tUIInfo.vDistortion2 = vDistortion2; }
	void			Set_Distortion3(_float2 vDistortion3) { m_tUIInfo.vDistortion3 = vDistortion3; }
	void			Set_DistortionScale(_float fDistortionScale) { m_tUIInfo.fDistortionScale = fDistortionScale; }
	void			Set_DistortionBias(_float fDistortionBias) { m_tUIInfo.fDistortionBias = fDistortionBias; }
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

	// =>Active
	void			Set_Active(_bool bActive) { m_bActive = bActive; }
	_bool			Get_Active() { return m_bActive; }

	/* Debug */
	void			Set_Tool(_bool bTool) { m_bTool = bTool; }
	_bool			m_bTool = true;

//protected:
public:
	virtual HRESULT	Set_ParentTransform(CTransform* pParentTransformCom);

public: /* ============================== Add ============================== */
	void			Add_Create_Parts(void* pArg);
	void			Add_Parts(CUI* pArg);

public: /* ========================== Change_Size ========================== */
	void			Set_Size(_float fSizeX, _float fSizeY);
	void			Change_SizeX(_float MMX);
	void			Change_SizeY(_float MMY);
	void			Change_SizeRight(_float MMX);
	void			Change_SizeBottom(_float MMY);
	void			Change_SizeTop(_float MMY);
	void			Change_SizeLeft(_float MMX);


public: /* ============================== Basic =============================== */
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Priority_Tick(_float fTimeDelta);
	virtual void	Tick(_float fTimeDelta);

	/* State */
	virtual void	UI_Ready(_float fTimeDelta) = 0;
	virtual void	UI_Enter(_float fTimeDelta) = 0;
	virtual void	UI_Loop(_float fTimeDelta) = 0;
	virtual void	UI_Exit(_float fTimeDelta) = 0;

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
	HRESULT			Ready_UI(const char* cFilePath);
	HRESULT			Create_UIParts(UI_DESC tUI_Desc);
	HRESULT			Update_Child_Transform();

	//				TargetPosition => Screen
	void			SetUp_PositionToScreen(_fvector vWorldPos);

	//				TargetWorld => Screen
	void			SetUp_WorldToScreen(_matrix vWorldPos);
	HRESULT			SetUp_BillBoarding();


	// error : �� �ڵ� ��Ҹ� ��ȯ���� ���߽��ϴ�. ���� ������ �� �ֽ��ϴ�. -> �ذ��� : ������Ʈ ���� ���� vs���� ���� �� ����
	void			Tick_LevelUp(_float fTimeDelta);
	void			Player_HUD(_float fTimeDelta);

public:
	void			Check_Disappear(_float fTimeDelta);

public:
#ifdef _DEBUG
	/* (�����̳��� �ּҸ� �޾ƿ��°� ������ ��忡�� ������ ���װ�����. ����) */
	vector<CUI*>*	Get_vecUIParts() { return &m_vecUIParts; }
	vector<UIKEYFRAME>*	Get_vecAnimation() { return &m_vecAnimation; }
#endif // DEBUG
	string			Get_FilePathTag() { return m_tUIInfo.strFilePath; }
	string			Get_ObjectNameTag() { return m_tUIInfo.strObjectName; }
	_int			Get_ObjectNum() { return m_tUIInfo.iObjectNum; }

protected: /* =========================== Ready ============================= */
	virtual HRESULT Ready_Components();
	virtual HRESULT Bind_ShaderResources();

public: /* =========================== Save/Load ============================== */
	virtual void	Load_FromJson(const json& In_Json);
	virtual json	Save_Desc(json& out_json);
	virtual json	Save_Animation(json& out_json);

public: /* =========================== Animation ============================== */
	void			Play_Animation(_float fTimeDelta);
	void			Add_Keyframe(UIKEYFRAME tKeyframe) { m_vecAnimation.push_back(tKeyframe); }
	void			Clear_Keyframe() { m_vecAnimation.clear(); }
	void			Emplaceback_Keyframe(UIKEYFRAME tKeyframe) 
	{ 
		if(!m_vecAnimation.empty())
			m_vecAnimation.emplace_back(tKeyframe);
	}
	void			Set_AnimationKeyframe(UIKEYFRAME tKeyframe);
	// �ִϸ��̼� ��
	std::vector<UIKEYFRAME> m_vecAnimation = {};
	_int			m_iTextureNum = 0;
	_int			m_iLoopAnimIndex = 0;

	void			Set_AnimPlay(_bool bPlay) { m_bPlayAnim = bPlay; }
	_bool			Get_AnimPlay() { return m_bPlayAnim; }
	_bool			m_bPlayAnim = false;

	void			Set_Disappear(_bool bDisappear) { m_bDisappear = bDisappear; }
	_bool			Get_Disappear() { return m_bDisappear; }
	_bool			m_bDisappear = false;

	void			Set_CurrTime(_float fCurrTime) { m_fCurrTime = fCurrTime; }
	_float			Get_CurrTime() { return m_fCurrTime; }
	_float			m_fCurrTime = 0.f;

	void			Set_Repetition(_bool bRepetition) { m_bRepetition = bRepetition; }
	_bool			Get_Repetition() { return m_bRepetition; }
	_bool			m_bRepetition = false;

	void			Set_Alpha(_float fAlpha) { m_fAlpha = fAlpha; }
	_float			Get_Alpha() { return m_fAlpha; }

	// dt ��
	_float			fFrameTimeDelta, fCurFrameTimeDelta;

	// ũ��
	_float			fSizeX_Delta, fSizeY_Delta;

	// ȸ��
	_float			fRotX_Delta, fRotY_Delta, fRotZ_Delta;

	// �̵�
	_float			fPosX_Delta, fPosY_Delta;

	// ����
	_float			fAlpha_Delta;

	// �ð�
	_float			fTimeAcc_Delta = 0.f;

	// ������
	_float			fSequenceTerm_Delta = 0.f;

	// ��ũ�� ���ǵ�
	_float3			vScrollSpeeds_Delta = { 0.f, 0.f, 0.f };

	// ũ��
	_float3			vScales_Delta = { 0.f, 0.f, 0.f };

	// �����1
	_float2			vDistortion1_Delta = { 0.f, 0.f };

	// �����2
	_float2			vDistortion2_Delta = { 0.f, 0.f };

	// �����3
	_float2			vDistortion3_Delta = { 0.f, 0.f };

	// ����� Scale
	_float			fDistortionScale_Delta = 0.f;

	// ����� Bias
	_float			fDistortionBias_Delta = 0.f;

public:
	void	Set_TimeAcc(_float fTimeAcc) { m_tUIInfo.fTimeAcc = fTimeAcc; }

protected: /* Data */
	class CData_Manager* m_pData_Manager = { nullptr };

protected:
	class CUI_Manager* m_pUI_Manager = { nullptr };
	//	class CCharacter*  m_pCharacter = { nullptr };
	//
	//public:
	//	void	Set_Character(CCharacter* pCharacter) { m_pCharacter = pCharacter; }

protected:
	_bool				Alpha_Minus(_float fTimeDelta);
	_bool				Alpha_Plus(_float fTimeDelta);
	_float				m_fAlphaSpeed = 1.f;

protected:
	void				Compute_CamDistance();
	_float				m_fCamDistance = { 0.0f };

protected: /* LifeTime */
	void				LifeTime_LevelUp(_float fTimeDelta);
	_bool				m_bEventOn = false;
	_float				m_fLifeTime = 5000.f;
	_float				m_fActiveTime = 1000.f;
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
	UITYPE				m_eType = UITYPE::NONE;
	_float4x4			m_Origin_WorldMatrix = {};
	_bool				m_bActive = false;
	_bool				m_bReset = false;
	_bool				m_bRestore = false;
	// UI_Member
	_float				m_fPositionX = 0.f, m_fPositionY = 0.f;
	_float				m_fScaleX = 0.f, m_fScaleY = 0.f, m_fScaleZ = 0.1f;
	UI_KIND				m_eKind = NORMAL;
	_float				m_fOffsetX = 0.f, m_fOffsetY = 0.f;

	CTexture*			m_pDistortionCom[DISTORTION_END] = { nullptr };
	_int				m_iMaskNum = 0;
	_int				m_iNoiseNum = 0;

	_bool				m_bReversePlay = false;
	_bool				m_bStopPlay = false;
	_bool				m_bButtonUI = false;
	UI_BUTTON			m_eUI_ButtonState = UI_BUTTON::NONE;

	/* Anim Index */
	_uint iFrameIndex = 0;

public:
	void				Set_MaskNum(_int iMaskNum) { m_tUIInfo.iMaskNum = iMaskNum; }
	void				Set_NoiseNum(_int iNoiseNum) { m_tUIInfo.iNoiseNum = iNoiseNum; }

public:
	void	Set_OffsetX(_float fOffsetX) { m_fOffsetX = fOffsetX; }
	void	Set_OffsetY(_float fOffsetY) { m_fOffsetY = fOffsetY; }
	void	Set_OffsetXY(_float fOffsetX, _float fOffsetY) 
	{ 
		m_fOffsetX = fOffsetX;
		m_fOffsetY = fOffsetY;
	}

	// ����
	_float				m_fAlpha = 0.f;
	_bool				m_bTrigger = false;

protected: /* ============================ bool =============================== */
	_bool				m_bPick = false;
	_uint				m_iButtonState = {};

public:
	virtual void		 Free() override;

};

END