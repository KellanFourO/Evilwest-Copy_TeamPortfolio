#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CCollider;
class CModel;
class CVIBuffer_Effect_Model_Instance;
END

BEGIN(Client)

class CEffect_Instance final : public CGameObject
{
public:
	typedef struct tagEffect_Desc : public CGameObject::GAMEOBJECT_DESC
	{
		wstring strModelTag = {TEXT("")};
		_uint	iNumInstance = { 0 };
		_uint	iShaderPassIndex = { 0 };
		
		//CRenderer::RENDERGROUP eRenderGroup = CRenderer::RENDER_END; /* �ȵ�. int�� ����ȯ�ؼ� �ޱ� */
	}EFFECT_INSTANCE_DESC;

private:
	CEffect_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Instance(const CEffect_Instance& rhs);
	virtual ~CEffect_Instance() = default;

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Tick(_float fTimeDelta) override;
	virtual void				Tick(_float fTimeDelta) override;
	virtual void				Late_Tick(_float fTimeDelta) override;
	virtual HRESULT				Render() override;

private:
	CShader*							m_pShaderCom = { nullptr };	
	CModel*								m_pModelCom = { nullptr };
	CVIBuffer_Effect_Model_Instance*	m_pInstanceModelCom = { nullptr };

	EFFECT_INSTANCE_DESC		m_tInstanceDesc = {};

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	/* ������ü�� �����Ѵ�. */
	static CEffect_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/* �纻��ü�� �����Ѵ�. */
	virtual CGameObject* Clone(void* pArg) override;

	virtual void Free() override;
};

END