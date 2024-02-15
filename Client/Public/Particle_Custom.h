#pragma once

#include "Client_Defines.h"
#include "Effect_Object.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Particle_Point;
END

BEGIN(Client)

class CParticle_Custom final : public CEffect_Object
{
public:
	typedef struct tagParticleCustomDesc : public CGameObject::GAMEOBJECT_DESC
	{
		wstring		strTextureTag;

	}PARTICLE_CUSTOM_DESC;

private:
	CParticle_Custom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Custom(const CParticle_Custom& rhs);
	virtual ~CParticle_Custom() = default;

public:
	virtual HRESULT Initialize_Prototype()				override;
	virtual HRESULT Initialize(void* pArg)				override;
	virtual void	Priority_Tick(_float fTimeDelta)	override;
	virtual void	Tick(_float fTimeDelta)				override;
	virtual void	Late_Tick(_float fTimeDelta)		override;
	virtual HRESULT Render()							override;

public:
	void		Set_Active(_bool bActive) { m_bActive = bActive; }

public:
	CVIBuffer_Particle_Point*	Get_VIBufferCom() { return m_pVIBufferCom; }
	CTransform*					Get_TransformCom() { return m_pTransformCom; }


private:
	wstring			m_strTextureTag;


private:
	_bool			m_bActive = { TRUE };
	_float			m_fTimer = { 0.0f };

private:
	CShader*						m_pShaderCom = { nullptr };
	CTexture*						m_pTextureCom = { nullptr };
	CVIBuffer_Particle_Point*		m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	/* ������ü�� �����Ѵ�. */
	static CParticle_Custom* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/* �纻��ü�� �����Ѵ�. */
	virtual CGameObject* Clone(void* pArg) override;

	virtual void Free() override;
};

END