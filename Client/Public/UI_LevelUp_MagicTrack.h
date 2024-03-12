#pragma once
#include "Client_Defines.h"
#include "UI.h"

/* ü�� ������ */
class CUI_LevelUp_MagicTrack final : public CUI
{
	enum TEXTUREKIND { MAGIC_TRACK, TEXTURE_END };

private:
	CUI_LevelUp_MagicTrack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CUI_LevelUp_MagicTrack(const CUI_LevelUp_MagicTrack& rhs);
	virtual ~CUI_LevelUp_MagicTrack() = default;

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
	void					Check_Disappear(_float fTimeDelta);

public:
	json				 Save_Desc(json& out_json);
	void				 Load_Desc();

private:
	CTexture* m_pTextureCom[TEXTURE_END] = { nullptr };

public:
	static CUI_LevelUp_MagicTrack* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag); //! ������ü ����
	virtual CGameObject* Clone(void* pArg) override; //! �纻��ü ����
	virtual CGameObject* Pool() override;
	virtual void			Free() override;
};

