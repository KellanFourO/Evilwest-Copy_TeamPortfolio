#pragma once

#include "Client_Defines.h"
#include "AlphaObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Particle_Point;
END

BEGIN(Client)

class CParticle_Custom final : public CAlphaObject
{
public:
	enum TEXTURE { TYPE_DIFFUSE, TYPE_MASK, TYPE_NOISE, TYPE_END };

	typedef struct tagParticleCustomDesc : public CGameObject::GAMEOBJECT_DESC
	{
		wstring		strTextureTag[TYPE_END];
		_int		iTextureIndex[TYPE_END] = { 0 };

		wstring		strShaderTag;
		_uint		iShaderPassIndex = { 0 };

		_int		iRenderGroup = { 7 };	//! �ۿ��� �������� �����׷��� ��Ʈ�� ����ȯ�ؼ� �������� ���� �ۼ����� CRENDERER::RENDERGROUP::RENDER_END�� 8��

		_uint		iNumInstance = { 0 };
		_uint		iMaxNumInstance;

		_float		fRotateUvDegree;

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
	virtual _bool Write_Json(json& Out_Json)		 override;
	virtual void  Load_FromJson(const json& In_Json) override;

	/* For.Desc */
public:
	PARTICLE_CUSTOM_DESC* Get_Desc() { return &m_tParticleDesc; }

	void		Set_Active(_bool bActive) { m_bActive = bActive; }

	void		Set_TextureIndex(TEXTURE eTexture, _int iIndex) { m_tParticleDesc.iTextureIndex[eTexture] = iIndex; }

	void		Set_RotateUvDegree(_float fDegree) { m_tParticleDesc.fRotateUvDegree = fDegree; }

public:
	CVIBuffer_Particle_Point* Get_VIBufferCom() { return m_pVIBufferCom; }

private:
	PARTICLE_CUSTOM_DESC		m_tParticleDesc = {};

private:
	_bool			m_bActive = { TRUE };


private:
	CShader*					m_pShaderCom = { nullptr };
	CTexture*					m_pTextureCom[TYPE_END] = { nullptr };
	CVIBuffer_Particle_Point*	m_pVIBufferCom = { nullptr };

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