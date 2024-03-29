#pragma once
#include "Client_Defines.h"
#include "UI.h"

/* ü�� ������ */
class CUI_MainMenuList final : public CUI
{
	enum TEXTUREKIND {
		GAMEPLAY, INTRO, INTRO_BOSS, SNOWMOUNTAIN, SNOWMOUNTAIN_BOSS, TOOL, EXIT, TEXTURE_END
	};

private:
	CUI_MainMenuList(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CUI_MainMenuList(const CUI_MainMenuList& rhs);
	virtual ~CUI_MainMenuList() = default;

public:
	virtual HRESULT			Initialize_Prototype() override; //! ������ü�� �ʱ�ȭ�� ���� �Լ�.
	virtual HRESULT			Initialize(void* pArg) override; //! �纻��ü�� �ʱ�ȭ�� ���� �Լ�. ( void*�� �Ű����ڷ� �޾� Ư���� �纻��ü ó���� �����ϴ�. )
	virtual void			Priority_Tick(_float fTimeDelta) override;
	virtual void			Tick(_float fTimeDelta) override;
	virtual void			Late_Tick(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

	/* State */
	virtual void	UI_Ready(_float fTimeDelta);
	virtual void	UI_Enter(_float fTimeDelta);
	virtual void	UI_Loop(_float fTimeDelta);
	virtual void	UI_Exit(_float fTimeDelta);

public:
	void			Check_Picking(_float fTimeDelta);

private:
	virtual HRESULT			Ready_Components() override;
	virtual HRESULT			Bind_ShaderResources() override;

public:
	json				 Save_Desc(json& out_json);
	void				 Load_Desc();

private:
	CTexture* m_pTextureCom[TEXTURE_END] = { nullptr };
	_bool		m_bHABO_Active = false;
	_bool		m_bFOG_Active = false;
	_bool		m_bRadial_Blur_Active = false;
	_bool		m_bDof_Active = false;
	_bool		m_bHDR_Active = false;
	_bool		m_bShadow_Active = false;
	_bool		m_bHSV_Active = false;

public:
	static CUI_MainMenuList* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag); //! ������ü ����
	virtual CGameObject* Clone(void* pArg) override; //! �纻��ü ����
	virtual CGameObject* Pool() override;
	virtual void			Free() override;
};
