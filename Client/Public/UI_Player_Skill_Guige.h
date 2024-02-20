#pragma once
#include "Client_Defines.h"
#include "UI.h"

/* ü�� ������ */
class CUI_Player_Skill_Guige final : public CUI
{
private:
	CUI_Player_Skill_Guige(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Player_Skill_Guige(const CUI_Player_Skill_Guige& rhs);
	virtual ~CUI_Player_Skill_Guige() = default;

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

public:
	json				 Save_Desc(json& out_json);
	void				 Load_Desc();

public:
	static CUI_Player_Skill_Guige* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); //! ������ü ����
	virtual CGameObject* Clone(void* pArg) override; //! �纻��ü ����
	virtual void			Free() override;
};

