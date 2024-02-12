#pragma once

#include "Base.h"

/* ȭ�鿡 �׷������� ��ü���� �׷����� ������� ��Ƴ��´�. */
/* ��Ƴ��� ������� ��ü���� �����Լ��� ȣ���Ѵ�.(������) */

BEGIN(Engine)

class CGameInstance;
class CShader;
class CVIBuffer_Rect;
class CGameObject;
class CComponent;

class CRenderer final : public CBase
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_SHADOW, RENDER_NONLIGHT, RENDER_NONBLEND, RENDER_BLEND, RENDER_UI, RENDER_END };

private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	HRESULT Initialize();
	HRESULT Add_RenderGroup(RENDERGROUP eGroupID, CGameObject* pGameObject);

	HRESULT Add_DebugRender(shared_ptr<CComponent> pDebugCom);

	HRESULT Draw_RenderGroup();

	void Clear();

private:
	ID3D11Device*							m_pDevice = { nullptr };
	ID3D11DeviceContext*					m_pContext = { nullptr };
	CGameInstance*							m_pGameInstance = { nullptr };
	list<CGameObject*>						m_RenderObjects[RENDER_END];

#ifdef _DEBUG
	list<shared_ptr<CComponent>>					m_DebugComponent;
#endif

private:
	shared_ptr<CShader>							m_pShader = { nullptr };
	shared_ptr<CVIBuffer_Rect>					m_pVIBuffer = { nullptr };

	_float4x4								m_WorldMatrix;
	_float4x4								m_ViewMatrix, m_ProjMatrix;

	ID3D11DepthStencilView*					m_pLightDepthDSV = { nullptr };

private:
	HRESULT Render_Priority();
	HRESULT Render_Shadow();
	HRESULT Render_NonLight();
	HRESULT Render_NonBlend();
	HRESULT Render_Blend();
	HRESULT Render_UI();

	HRESULT Render_LightAcc();
	HRESULT Render_Deferred();

#ifdef _DEBUG
private:
	HRESULT Render_Debug();
#endif

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END