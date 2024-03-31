#pragma once
#include "Client_Defines.h"
#include "UI.h"

/* ü�� ������ */
class CUI_MoveEffect final : public CUI
{
public: /* �� UI�������� ��� �󸶳� ������ ����������. */
	enum TEXTUREKIND { DIFFUSE, TEXTURE_END };

private:
	CUI_MoveEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CUI_MoveEffect(const CUI_MoveEffect& rhs);
	virtual ~CUI_MoveEffect() = default;

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

private:
	virtual HRESULT			Ready_Components() override;
	virtual HRESULT			Bind_ShaderResources() override;

private:
	void					Compute_OwnerCamDistance();
	_bool					In_Frustum();

private:
	void					Set_OwnerHp();

private:
	CTexture* m_pTextureCom[TEXTURE_END] = { nullptr };
	TEXTUREKIND				m_eTexture_Kind = TEXTURE_END;
	_float					m_fPreHP = 0.f;
	_float					m_fCurHP = 0.f;
	_float					m_fMaxHP = 0.f;
	_float					m_fTimeAcc = 0.f;
	_float					m_fVariationSpeed = 2.0f;

	_bool					m_bTopChange = false;
	_bool					m_bBottomChange = false;

	/* Decal */
	_bool					m_bChangeType = false;
	_float2					m_vDecal_Scale = { 1.f, 1.f };
	_float2					m_vDecal_Offset = { 0.f, 0.f };

	_bool					m_bLerp = false;
public:
	void	Set_Restore(_bool bRestore) { m_bRestore = bRestore; }

public:
	virtual json			Save_Desc(json& out_json);
	void					Load_Desc();

public:
	static CUI_MoveEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag); //! ������ü ����
	virtual CGameObject* Clone(void* pArg) override; //! �纻��ü ����
	virtual CGameObject* Pool() override;
	virtual void			Free() override;
};

