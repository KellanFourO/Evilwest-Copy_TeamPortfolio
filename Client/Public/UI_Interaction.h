#pragma once
#include "UI.h"

/* ü�� ������ */
class CUI_Interaction final : public CUI
{
	enum TEXTUREKIND { NONACTIVE, ACTIVE, TEXTURE_END };

private:
	CUI_Interaction(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CUI_Interaction(const CUI_Interaction& rhs);
	virtual ~CUI_Interaction() = default;

public:
	virtual HRESULT			Initialize_Prototype() override; //! ������ü�� �ʱ�ȭ�� ���� �Լ�.
	virtual HRESULT			Initialize(void* pArg) override; //! �纻��ü�� �ʱ�ȭ�� ���� �Լ�. ( void*�� �Ű����ڷ� �޾� Ư���� �纻��ü ó���� �����ϴ�. )
	virtual void			Priority_Tick(_float fTimeDelta) override;
	virtual void			Tick(_float fTimeDelta) override;
	virtual void			Late_Tick(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

	/* State */
	virtual void			UI_Ready(_float fTimeDelta);
	virtual void			UI_Enter(_float fTimeDelta);
	virtual void			UI_Loop(_float fTimeDelta);
	virtual void			UI_Exit(_float fTimeDelta);

public:
	void					Set_TargetPosition(_vector vTargetPosition);
	void					Check_TargetWorld();

	void					Set_OnInteraction(_bool bOnInteraction) { m_bOnInteraction = bOnInteraction; }
	_bool					Get_OnInteraction() { return m_bOnInteraction; }

	void					Set_Available(_bool bAvailable) { m_bAvailable = bAvailable; }
	_bool					Get_Available() { return m_bAvailable; }

	void					Set_OnCollision(_bool bOnCollision) { m_bOnCollision = bOnCollision; }
	_bool					Get_OnCollision() { return m_bOnCollision; }

	void					Reset_Interaction_UI();

private:
	virtual HRESULT			Ready_Components() override;
	virtual HRESULT			Bind_ShaderResources() override;
	CGameObject*			m_pTarget = nullptr;
	_vector					m_vTargetPosition = { 0.f, 0.f, 0.f, 0.f };
	_matrix					m_World = XMMatrixIdentity();
	_float					m_fChangeScale = 1.f;
	_float					m_fOriginScaleX, m_fOriginScaleY;
	_bool					m_bAvailable = false;
	_bool					m_bOnCollision = false;

public:
	json					Save_Desc(json& out_json);
	void					Load_Desc();

public:
	CCollider* m_pColliderCom = { nullptr };

private:
	CTexture* m_pTextureCom[TEXTURE_END] = { nullptr };
	_bool			m_bOnInteraction = false;

public:
	static CUI_Interaction* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag); //! ������ü ����
	virtual CGameObject* Clone(void* pArg) override; //! �纻��ü ����
	virtual CGameObject* Pool() override;
	virtual void			Free() override;
};

