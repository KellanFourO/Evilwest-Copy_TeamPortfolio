#pragma once
#include "Client_Defines.h"
#include "UI.h"

/* ü�� ������ */
class CUI_Video final : public CUI
{
public: /* �� UI�������� ��� �󸶳� ������ ����������. */
	enum TEXTUREKIND { HPBAR_WHITE, HPBAR_RED, HP_DECAL, TEXTURE_END };

private:
	CUI_Video(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CUI_Video(const CUI_Video& rhs);
	virtual ~CUI_Video() = default;

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
	CTexture* m_pTextureCom[TEXTURE_END] = { nullptr };
	ID3D11Texture2D*		m_pTextureVideo = nullptr;
	TEXTUREKIND				m_eTexture_Kind = TEXTURE_END;

public:
	virtual json			Save_Desc(json& out_json);
	void					Load_Desc();

public:
	static CUI_Video* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag); //! ������ü ����
	virtual CGameObject* Clone(void* pArg) override; //! �纻��ü ����
	virtual CGameObject* Pool() override;
	virtual void			Free() override;
};

