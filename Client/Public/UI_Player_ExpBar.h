#pragma once
#include "Client_Defines.h"
#include "UI.h"

/* ü�� ������ */
class CUI_Player_ExpBar final : public CUI
{
public: /* �� UI�������� ��� �󸶳� ������ ����������. */
	enum TEXTUREKIND { EXPBAR, TEXTURE_END };

private:
	CUI_Player_ExpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CUI_Player_ExpBar(const CUI_Player_ExpBar& rhs);
	virtual ~CUI_Player_ExpBar() = default;

public:
	virtual HRESULT			Initialize_Prototype() override; //! ������ü�� �ʱ�ȭ�� ���� �Լ�.
	virtual HRESULT			Initialize(void* pArg) override; //! �纻��ü�� �ʱ�ȭ�� ���� �Լ�. ( void*�� �Ű����ڷ� �޾� Ư���� �纻��ü ó���� �����ϴ�. )
	virtual void			Priority_Tick(_float fTimeDelta) override;
	virtual void			Tick(_float fTimeDelta) override;
	virtual void			Late_Tick(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

private:
	virtual HRESULT			Ready_Components() override;
	virtual HRESULT			Bind_ShaderResources() override;

private:
	void					Compute_OwnerCamDistance();
	_bool					In_Frustum();

private:
	CTexture* m_pTextureCom[TEXTURE_END] = { nullptr };
	TEXTUREKIND				m_eTexture_Kind = TEXTURE_END;
	_float					m_fOrigin_Right = 0.f;

public:
	virtual json			Save_Desc(json& out_json);
	void					Load_Desc();

public:
	static CUI_Player_ExpBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag); //! ������ü ����
	virtual CGameObject* Clone(void* pArg) override; //! �纻��ü ����
	virtual CGameObject* Pool() override;
	virtual void			Free() override;
};

