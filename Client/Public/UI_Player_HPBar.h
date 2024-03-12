#pragma once
#include "Client_Defines.h"
#include "UI.h"

/* 체력 프레임 */
class CUI_Player_HPBar final : public CUI
{
public: /* 각 UI파츠마다 어떤걸 얼마나 가질지 설정해주자. */
	enum TEXTUREKIND { HPBAR_WHITE, HPBAR_RED, HP_DECAL, TEXTURE_END };

private:
	CUI_Player_HPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CUI_Player_HPBar(const CUI_Player_HPBar& rhs);
	virtual ~CUI_Player_HPBar() = default;

public:
	virtual HRESULT			Initialize_Prototype() override; //! 원형객체의 초기화를 위한 함수.
	virtual HRESULT			Initialize(void* pArg) override; //! 사본객체의 초기화를 위한 함수. ( void*를 매개인자로 받아 특수한 사본객체 처리가 가능하다. )
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
	void					Set_OwnerHp();

private:
	CTexture*				m_pTextureCom[TEXTURE_END] = { nullptr };
	TEXTUREKIND				m_eTexture_Kind = TEXTURE_END;
	_float					m_fPreHP = 0.f;
	_float					m_fCurHP = 0.f;
	_float					m_fMaxHP = 0.f;
	_float					m_fTimeAcc = 0.f;
	_float					m_fVariationSpeed = 2.0f;

	/* Decal */
	_bool					m_bChangeType = false;
	_float2					m_vDecal_Scale = { 1.f, 1.f };
	_float2					m_vDecal_Offset = { 0.f, 0.f };

	_bool					m_bLerp = false;
public:
	virtual json			Save_Desc(json& out_json);
	void					Load_Desc();

public:
	static CUI_Player_HPBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag); //! 원형객체 생성
	virtual CGameObject* Clone(void* pArg) override; //! 사본객체 생성
	virtual CGameObject* Pool() override;
	virtual void			Free() override;
};

