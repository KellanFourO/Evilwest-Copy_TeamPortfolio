#pragma once
#include "Client_Defines.h"
#include "UI.h"

/* ü�� ������ */
class CUI_EnemyHUD_Shard final : public CUI
{
	enum TEXTUREKIND { NONE, ATTACK, ELECTRIC, SKULL, SKULLRED, TEXTURE_END };

private:
	CUI_EnemyHUD_Shard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CUI_EnemyHUD_Shard(const CUI_EnemyHUD_Shard& rhs);
	virtual ~CUI_EnemyHUD_Shard() = default;

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

	void					UI_Setting();
	void					Set_Dead_Owner(_bool bDead) { m_bDeadOwner = bDead; }

public:
	/* Child */
	HRESULT					Ready_ChildHUD();
	void					Set_EnemyHUD_World(_matrix matWorld, _float3 vOffsetPos);
	void					ActiveEnemyHUD();
	void					NonActiveEnemyHUD();
	void					DeadEnemyHUD();

protected:
	_bool					m_bDeadOwner = false;

private:
	virtual HRESULT			Ready_Components() override;
	virtual HRESULT			Bind_ShaderResources() override;

private:
	CGameObject*			m_pTarget = nullptr;
	_vector					m_vTargetPosition = { 0.f, 0.f, 0.f, 0.f };
	_matrix					m_World = XMMatrixIdentity();
	_float					m_fChangeScale = 1.f;
	_bool					m_bAppear = false;
	TEXTUREKIND				m_eCurState = ATTACK;
	vector<CUI*>			m_vecEnemyHUD;

public:
	json					Save_Desc(json& out_json);
	void					Load_Desc();

private:
	CTexture* m_pTextureCom[TEXTURE_END] = { nullptr };

public:
	static CUI_EnemyHUD_Shard* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag); //! ������ü ����
	virtual CGameObject* Clone(void* pArg) override; //! �纻��ü ����
	virtual CGameObject* Pool() override;
	virtual void			Free() override;
};

