#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CCollider;
class CShader;
class CModel;
class CBone;
END

BEGIN(Client)

class CWeapon_Player final : public CGameObject
{
public:
	typedef struct tagWeaponDesc
	{
		class CBone*			m_pSocketBone = { nullptr };
		shared_ptr<CTransform>		m_pParentTransform = { nullptr };
	}WEAPON_DESC;
private:
	CWeapon_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon_Player(const CWeapon_Player& rhs);
	virtual ~CWeapon_Player() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	shared_ptr<CShader> 		m_pShaderCom = { nullptr };
	shared_ptr<CModel>			m_pModelCom = { nullptr };
	shared_ptr<CCollider>		m_pColliderCom = { nullptr };

private:
	shared_ptr<CTransform>	m_pParentTransform = { nullptr };
	CBone*		m_pSocketBone = { nullptr };
	_float4x4				m_WorldMatrix = {};

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	/* 원형객체를 생성한다. */
	static CWeapon_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/* 사본객체를 생성한다. */
	virtual CGameObject* Clone(void* pArg) override;

	virtual void Free() override;
};

END