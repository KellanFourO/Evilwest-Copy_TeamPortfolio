#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Particle_Rect;
END

BEGIN(Client)

class CParticle_Blue final : public CGameObject
{
public:	
	CParticle_Blue(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Blue(const CParticle_Blue& rhs);
	virtual ~CParticle_Blue() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	shared_ptr<CShader>						m_pShaderCom = { nullptr };
	shared_ptr<CTexture>					m_pTextureCom = { nullptr };
	shared_ptr<CVIBuffer_Particle_Rect>		m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	/* ������ü�� �����Ѵ�. */
	static shared_ptr<CParticle_Blue> Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/* �纻��ü�� �����Ѵ�. */
	virtual shared_ptr<CGameObject> Clone(void* pArg) override;

	virtual void Free() override;
};

END