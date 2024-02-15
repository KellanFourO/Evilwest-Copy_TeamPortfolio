#pragma once

#include "Renderer.h"
#include "PipeLine.h"
#include "Component_Manager.h"

/* 클라이언트에서 엔진의 기능을 사용하기위해 반드시 거쳐야하는 객체. */

/* DX11을 통해 필요한 객체를 생성하고 렌더링하는 작업을 수행한다. */
/* 지금 생성한 레벨(씬)을 보관하고 필요에따라 업데이트 수행. */
/* 내가 생성한 객체들을 보관하고 업데이트하고 렌더한다. */
/* 내가 생성한 여러 컴포넌트들의 원형을 보관하고 복제하여 가져온다. */

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* For.Engine */
	/* 엔진라이브러리를 사용하기위한 준비를 모두 거친다. */
	HRESULT		Initialize_Engine(_uint iNumLevels, HINSTANCE hInstance, const GRAPHIC_DESC& GraphicDesc, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext);
	void		Tick_Engine(_float fTimeDelta);
	HRESULT		Render_Engine();
	void		Clear(_uint iLevelIndex);

public: /* For.Graphic_Device */
	HRESULT		Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT		Clear_DepthStencil_View();
	HRESULT		Present();
	ID3D11RenderTargetView* Get_BackBufferRTV() const;
	ID3D11DepthStencilView* Get_DSV() const;
	GRAPHIC_DESC*			Get_GraphicDesc();


public: /* For.Input_Device */
	_byte		Get_DIKeyState(_ubyte byKeyID);
	_byte		Get_DIMouseState(MOUSEKEYSTATE eMouse);
	_long		Get_DIMouseMove(MOUSEMOVESTATE eMouseState);

	_bool		Key_Pressing(_ubyte byKeyID);
	_bool		Key_Down(_ubyte byKeyID);
	_bool		Key_Up(_ubyte byKeyID);
	_bool		Mouse_Pressing(MOUSEKEYSTATE eMouse);
	_bool		Mouse_Down(MOUSEKEYSTATE eMouse);
	_bool		Mouse_Up(MOUSEKEYSTATE eMouse);

public: /* For.Timer_Manager */
	HRESULT		Add_Timer(const wstring& strTimeTag);
	_float		Compute_TimeDelta(const wstring& strTimeTag);

public: /* For.Level_Manager */
	HRESULT Open_Level(_uint iCurrentLevelIndex, class CLevel* pNewLevel);
	_uint	Get_NextLevel();
	void	Set_CurrentLevel(_uint CurrentLevel);

public: /* For.Object_Manager */
	HRESULT				Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	HRESULT				Add_CloneObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg = nullptr);
	CGameObject*		Clone_Prototype(const wstring& strPrototypeTag, void* pArg = nullptr);
	class CComponent*	Get_Component(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strComponentTag, _uint iIndex = 0, const wstring& strPartTag = TEXT(""));

	list<CGameObject*>* Get_GameObjects(_uint iLevelIndex, const wstring & strLayerTag);
	void Get_CloneGameObjects(_uint iLevelIndex, vector<CGameObject*>*clonevector);
	class CGameObject* Get_GameObect_Last(_uint iLevelIndex, const wstring & strLayerTag);
	class CGameObject* Add_CloneObject_And_Get(_uint iLevelIndex, const wstring & strLayerTag, const wstring & strPrototypeTag, void* pArg = nullptr);
	class CGameObject* Get_Player();
	void Set_Player(class CGameObject* _pPlayer);



	void Fill_PrototypeTags(vector<string>*_vector);

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg = nullptr);

public: /* For.Renderer */
	HRESULT Add_RenderGroup(CRenderer::RENDERGROUP eGroupID, class CGameObject* pGameObject);
	HRESULT Add_DebugRender(class CComponent* pDebugCom);
#ifdef _DEBUG
	void Set_RenderDebug(_bool _bRenderDebug);
#endif


public: /* For.PipeLine */
	void		Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix);
	void		Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _float4x4 TransformMatrix);
	_matrix		Get_TransformMatrix(CPipeLine::D3DTRANSFORMSTATE eState);
	_float4x4	Get_TransformFloat4x4(CPipeLine::D3DTRANSFORMSTATE eState);
	_matrix		Get_TransformMatrixInverse(CPipeLine::D3DTRANSFORMSTATE eState);
	_float4x4	Get_TransformFloat4x4Inverse(CPipeLine::D3DTRANSFORMSTATE eState);
	_float4		Get_CamPosition();
	_float4		Get_CamSetting();
	_float		Get_CamFar();

public: /* For.Font_Manager */
	HRESULT		Add_Font(const wstring& strFontTag, const wstring& strFontFilePath);
	HRESULT		Render_Font(const wstring& strFontTag, const wstring& strText, const _float2 & vPosition, _fvector vColor = XMVectorSet(1.f, 1.f, 1.f ,1.f), _float fScale = 1.f, _float2 vOrigin = _float2(0.f, 0.f), _float fRotation = 0.f);

public: /* For.Target_Manager */
	HRESULT		Add_RenderTarget(const wstring& strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	HRESULT		Add_MRT(const wstring& strMRTTag, const wstring& strTargetTag);
	HRESULT		Begin_MRT(const wstring& strMRTTag, ID3D11DepthStencilView* pDSV = nullptr);
	HRESULT		End_MRT();
	HRESULT		Bind_RenderTarget_ShaderResource(const wstring& strTargetTag, class CShader* pShader, const _char* pConstantName);
#ifdef _DEBUG
	HRESULT		Ready_RenderTarget_Debug(const wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT		Render_Debug_RTVs(const wstring& strMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif

public: /* For.Light_Manager */
	HRESULT		Add_Light(const LIGHT_DESC& LightDesc, _int & outLightIndex);
	HRESULT		Render_Lights(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

public: /* For.Frustum */
	void		Transform_Frustum_ToLocalSpace(_fmatrix WorldMatrix);
	_bool		isIn_WorldPlanes(_fvector vPoint, _float fRadius = 0.f);
	_bool		isIn_LocalPlanes(_fvector vPoint, _float fRadius);

public: /* Common */
	void		String_To_WString(string _string, wstring & _wstring);
	void		WString_To_String(wstring _wstring, string & _string);

private:
	class CGraphic_Device*			m_pGraphic_Device = { nullptr };
	class CInput_Device*			m_pInput_Device = { nullptr };
	class CTimer_Manager*			m_pTimer_Manager = { nullptr };
	class CLevel_Manager*			m_pLevel_Manager = { nullptr };
	class CObject_Manager*			m_pObject_Manager = { nullptr };
	class CComponent_Manager*		m_pComponent_Manager = { nullptr };
	class CRenderer*				m_pRenderer = { nullptr };
	class CPipeLine*				m_pPipeLine = { nullptr };
	class CFont_Manager*			m_pFont_Manager = { nullptr };
	class CTarget_Manager*			m_pTarget_Manager = { nullptr };
	class CLight_Manager*			m_pLight_Manager = { nullptr };
	class CFrustum*					m_pFrustum = { nullptr };

public:
	void Release_Manager();
	static void Release_Engine();
};

END