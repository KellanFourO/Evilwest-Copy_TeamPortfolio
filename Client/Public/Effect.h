#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CEffect final : public CGameObject
{
public:
	typedef struct tagEffectDesc : public CGameObject::GAMEOBJECT_DESC
	{
		_bool	bActive_Tool		= { TRUE };
		_bool	bPlay				= { TRUE };
		_bool	bLoop				= { TRUE };
		_bool	bFinished			= { FALSE };
		_bool	bReverse			= { FALSE };
		_bool	bRender				= { FALSE };

		_int	iPartSize			= { 0 };

		// Times
		_float	fTimeAcc			= { 0.f };	/* �ð� ���� */
		_float	fWaitingAcc			= { 0.f };	/* ���� ������ �ð� ���� */
		_float	fSequenceAcc		= { 0.f };	/* ������ �ð� ���� */

		_float	fLifeTime			= { 3.f };
		_float	fWaitingTime		= { 0.f };	/* �� ���� �Ѿ�� m_fTimeAcc�� �����Ǳ� �����Ѵ�. */
		_float	fRemainTime			= { 0.f };	/* ������Ÿ���� ������, �� �ð��� �Ѿ�� ����Ʈ ����. */
		_float	fSequenceTime		= { 0.f };	/* �� ������ �ð�(fWaitingTime + fLifeTime + fRemainTime) */

		_float  fLifeTimeRatio		= { 0.f };	/* ������Ÿ���� 0~1�� ������ �� */
		_float	fDissolveStartTime	= { 1.f };	/* ������ ���� �ð�(���� ����) */


		// ����
		CGameObject* pOwner			= { nullptr };
		_bool		 bParentPivot	= { FALSE };
		_float4x4	 matPivot		= {};		/* XMStoreFloat4x4(&m_matPivot, XMMatrixIdentity()) */
		_float4x4	 matOffset		= {};

	}EFFECT_DESC;


private:
	CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;

public:
	virtual HRESULT Initialize_Prototype()				override;
	virtual HRESULT Initialize(void* pArg)				override;
	virtual void	Priority_Tick(_float fTimeDelta)	override;
	virtual void	Tick(_float fTimeDelta)				override;
	virtual void	Late_Tick(_float fTimeDelta)		override;
	virtual HRESULT Render()							override;

public:
	void	ReSet_Effect();
	void	End_Effect();

public:
	map<const wstring, class CGameObject*>* Get_PartObjects() { return &m_PartObjects; }
	CGameObject*	Find_PartObject(const wstring& strPartTag);
	HRESULT			Add_PartObject(const wstring& strPrototypeTag, const wstring& strPartTag, void* pArg);


public:
	EFFECT_DESC* Get_Desc() { return &m_tEffectDesc; }

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

private:
	EFFECT_DESC	m_tEffectDesc = {};
	_float4x4	m_matCombined = {};
	map<const wstring, class CGameObject*>		m_PartObjects;


public:
	/* ������ü�� �����Ѵ�. */
	static CEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);

	/* �纻��ü�� �����Ѵ�. */
	virtual CGameObject* Clone(void* pArg)	override;
	virtual CGameObject* Pool()				override;

	virtual void Free() override;
};

END