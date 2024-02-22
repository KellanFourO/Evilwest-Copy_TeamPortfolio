#pragma once

#include "Client_Defines.h"
#include "Effect_Void.h"

BEGIN(Engine)
class CShader;
class CCollider;
class CModel;
class CVIBuffer_Effect_Model_Instance;
END

BEGIN(Client)

class CEffect_Instance final : public CEffect_Void
{
public:
	enum TYPE_MESH	{ FLAT, FIGURE, TYPE_MESH_END };

public:
	typedef struct tagEffect_Desc : public CEffect_Void::EFFECTVOID_DESC
	{
		TYPE_MESH		eType_Mesh = { FLAT };

		wstring			strModelTag = TEXT("");
		
	}EFFECT_INSTANCE_DESC;



	typedef struct tagSpriteUV
	{
		_float	fTimeAcc = { 0.f };
		_float	fAddTime = { 0.05f };

		_float	fAnimationSizeX = { 292.5f };
		_float	fAnimationSizeY = { 292.5f };

		_float	fSpriteSizeX = { 2048.f };
		_float	fSpriteSizeY = { 2048.f };

		_int	iCurrentVer = { 0 };
		_int	iCurrentHor = { 0 };

		_int	iMinVer = { 0 };
		_int	iMinHor = { 0 };

		_int	iMaxVer = { 7 };
		_int	iMaxHor = { 7 };

	}SPRITEUV_DESC;

private:
	CEffect_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CEffect_Instance(const CEffect_Instance& rhs);
	virtual ~CEffect_Instance() = default;

public:
	virtual HRESULT				Initialize_Prototype()				override;
	virtual HRESULT				Initialize(void* pArg)				override;
	virtual void				Priority_Tick(_float fTimeDelta)	override;
	virtual void				Tick(_float fTimeDelta)				override;
	virtual void				Late_Tick(_float fTimeDelta)		override;
	virtual HRESULT				Render()							override;


public:
	virtual _bool Write_Json(json& Out_Json)		 override;
	virtual void  Load_FromJson(const json& In_Json) override;

/* For.Desc */
public:
	EFFECT_INSTANCE_DESC* Get_Desc() { return &m_tInstanceDesc; }
	SPRITEUV_DESC*		  Get_Sprite_Desc() { return &m_tSpriteDesc; }

	void		Set_ShaderPassIndex(_uint iShaderPassIndex);

	_bool		bTest = { FALSE };

private:
	CShader*							m_pShaderCom				= { nullptr };	
	CModel*								m_pModelCom					= { nullptr };
	CTexture*							m_pTextureCom[TEXTURE_END]	= { nullptr };
	CVIBuffer_Effect_Model_Instance*	m_pVIBufferCom				= { nullptr };

	EFFECT_INSTANCE_DESC				m_tInstanceDesc = {};
	SPRITEUV_DESC						m_tSpriteDesc = {};

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	/* ������ü�� �����Ѵ�. */
	static CEffect_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);

	/* �纻��ü�� �����Ѵ�. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual CGameObject* Pool();

	virtual void Free() override;

};

END