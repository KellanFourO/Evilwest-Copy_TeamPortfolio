#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)
//class CTransform;
class CShader;
class CTexture;
class CVIBuffer_Field;
END

BEGIN(Client)

class CField final : public CGameObject
{
public:
	enum TEXTURE { TYPE_DIFFUSE, TYPE_MASK, TYPE_BRUSH, TYPE_END }; // ����, ����ũ

	enum EDIT_MODE
	{
		HEIGHT_FLAT,
		HEIGHT_LERP,
		HEIGHT_SET,
		FILLTER,
		NON,
		EDIT_END
	};

	typedef struct tagVertex_Dynamic_Info
	{
		float		fX;
		float		fY;
		float		fZ;

		vector<VTXFIELD> vecVertexInfo;
	}DINFO;

private:
	CField(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CField(const CField& rhs);
	virtual ~CField() = default;

public:
	//void	Delete_Component(const wstring& strComTag);
	_vector Get_WorldMatrix() { return m_pTransformCom->Get_State(CTransform::STATE_POSITION); }

public:
	virtual HRESULT Initialize_Prototype(); //! ������ü�� �ʱ�ȭ�� ���� �Լ�.
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Tick(_float fTimeDelta) override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual void	Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	void				Picking_Terrain(RAY WorldRay, EDIT_MODE eMode);
	_float3				GetMousePos(RAY WorldRay);

	_bool				MouseOnTerrain();

	void				SetPower(_float fPower) { m_fPower = fPower; }
	void				SetRadious(_float fRadious) { m_fDrawRadious = fRadious; }

private:
	CShader*			m_pShaderCom = { nullptr };
	CTexture*			m_pTextureCom[TYPE_END] = { nullptr }; // ������ ����ũ�� �Ѱ��� ������� �迭�� ���� 0: ���� 2��, 1: ����ũ 1��
	CVIBuffer_Field*	m_pVIBufferCom = { nullptr };

private:
	_float						m_fDrawRadious = { 1.f };
	_float						m_fPower = { 2.f };
	_float3						m_fPickingPos = { 0.f, 0.f, 0.f };
	_bool						m_bOnMouse = false;

private:
	EDIT_MODE					m_eEditMode = { EDIT_MODE::NON };
	LEVEL						m_eCurrentLevelID = { LEVEL_END };
	DINFO						m_tDynamicInfo;

private:
	HRESULT						Ready_Components();
	HRESULT						Bind_ShaderResources();
	void						ReceiveInfo(DINFO pInfo);

public:
	static CField* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag); //! ������ü ����
	virtual CGameObject* Clone(void* pArg) override; //! �纻��ü ����
	virtual CGameObject* Pool() override;
	virtual void				Free() override;
	
};

END

