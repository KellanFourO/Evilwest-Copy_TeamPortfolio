#pragma once

#include "Body_Infected.h"

BEGIN(Client)

class CBody_Infected_D : public CBody_Infected
{

private:
	CBody_Infected_D(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CBody_Infected_D(const CBody_Infected_D& rhs);
	virtual ~CBody_Infected_D() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Shadow() override;

private:
	void Update_DiscardMesh();
	HRESULT OptionSetting();
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CBody_Infected_D* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual CGameObject* Pool() override;
	virtual void Free() override;
};

END