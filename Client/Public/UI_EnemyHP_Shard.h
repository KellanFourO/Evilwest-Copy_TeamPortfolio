#pragma once
#include "Client_Defines.h"
#include "UI.h"

/* ü�� ������ */
class CUI_EnemyHP_Shard final : public CUI
{
private:
	CUI_EnemyHP_Shard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CUI_EnemyHP_Shard(const CUI_EnemyHP_Shard& rhs);
	virtual ~CUI_EnemyHP_Shard() = default;

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
	void					Set_TargetPosition(_vector vTargetPosition);
	void					Check_TargetWorld();

private:
	virtual HRESULT			Ready_Components() override;
	virtual HRESULT			Bind_ShaderResources() override;

	CGameObject*			m_pTarget = nullptr;
	_vector					m_vTargetPosition = { 0.f, 0.f, 0.f, 0.f };
	_matrix					m_World = XMMatrixIdentity();
	_float					m_fChangeScale = 1.f;

public:
	HRESULT					Setting_Owner();

private:
	CCharacter*				m_pCharacterOwner = { nullptr };
	_float					m_fPreHP = 40.f;
	_float					m_fCurHP = 40.f;
	_float					m_fMaxHP = 40.f;
	_float2					m_vStartPoint = { 0.f, 0.f };
	_float2					m_vEndPoint = { 0.f, 0.f };
	_float					m_fTimeAcc = 0.f;
	_float					m_fVariationSpeed = 1.7f;
	_bool					m_bLerp = false;

public:
	json					Save_Desc(json& out_json);
	void					Load_Desc();

private:
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pTexturePreCom = { nullptr };

public:
	static CUI_EnemyHP_Shard* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag); //! ������ü ����
	virtual CGameObject* Clone(void* pArg) override; //! �纻��ü ����
	virtual CGameObject* Pool() override;
	virtual void			Free() override;
};

