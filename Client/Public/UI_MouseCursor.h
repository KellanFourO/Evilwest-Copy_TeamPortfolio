#pragma once
#include "Client_Defines.h"
#include "UI.h"

/* 체력 프레임 */
class CUI_MouseCursor final : public CUI
{
	enum TEXTUREKIND { PLAYGAME_CROSSHAIR, CURSOR,  CURSOR_LEFTCLICK, CURSOR_RIGHTCLICK, TEXTURE_END };

private:
	CUI_MouseCursor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CUI_MouseCursor(const CUI_MouseCursor& rhs);
	virtual ~CUI_MouseCursor() = default;

public:
	virtual HRESULT			Initialize_Prototype() override; //! 원형객체의 초기화를 위한 함수.
	virtual HRESULT			Initialize(void* pArg) override; //! 사본객체의 초기화를 위한 함수. ( void*를 매개인자로 받아 특수한 사본객체 처리가 가능하다. )
	virtual void			Priority_Tick(_float fTimeDelta) override;
	virtual void			Tick(_float fTimeDelta) override;
	virtual void			Late_Tick(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

	/* State */
	virtual void	UI_Ready(_float fTimeDelta);
	virtual void	UI_Enter(_float fTimeDelta);
	virtual void	UI_Loop(_float fTimeDelta);
	virtual void	UI_Exit(_float fTimeDelta);

private:
	virtual HRESULT			Ready_Components() override;
	virtual HRESULT			Bind_ShaderResources() override;

public:
	json				 Save_Desc(json& out_json);
	void				 Load_Desc();

private:
	CTexture* m_pTextureCom[TEXTURE_END] = { nullptr };
	_bool	m_bGamePlayMouse = false;
	POINT	m_ptMouse = {};
	POINT	m_ptOffset = {};

	//TEXTUREKIND m_eCurMouse = PLAYGAME_CURSOR;

public:
	static CUI_MouseCursor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag); //! 원형객체 생성
	virtual CGameObject* Clone(void* pArg) override; //! 사본객체 생성
	virtual CGameObject* Pool() override;
	virtual void			Free() override;
};

