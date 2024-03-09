#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CEffect final : public CGameObject
{
public:
	typedef struct tagEffectDesc : public CGameObject::GAMEOBJECT_DESC
	{
		/* �����ؾ� �ϴ� ���� ������ */
		_bool	bPlay				= { TRUE };
		_bool	bLoop				= { TRUE };
		_bool	bFinished			= { FALSE };
		_bool	bReverse			= { FALSE };
		_bool	bRender				= { FALSE };

		_int	iPartSize			= { 0 };

		// Times
		_float	fLifeTime			= { 5.f };
		_float	fWaitingTime		= { 0.f };	/* �� ���� �Ѿ�� m_fTimeAcc�� �����Ǳ� �����Ѵ�. */
		_float	fRemainTime			= { 0.f };	/* ������Ÿ���� ������, �� �ð��� �Ѿ�� ����Ʈ ����. */
		_float	fSequenceTime		= { 0.f };	/* �� ������ �ð�(fWaitingTime + fLifeTime + fRemainTime) */

		// ����
		_bool		 bParentPivot	= { FALSE };


		/* ������Ʈ �Ǹ鼭 �ٲ�� ������(����x) */
		_bool	bActive_Tool = { TRUE };

		// �ð� ����
		_float	fTimeAcc = { 0.f };			/* �ð� ���� */
		_float	fWaitingAcc = { 0.f };		/* ���� ������ �ð� ���� */
		_float  fRemainAcc = { 0.f };
		_float	fSequenceAcc = { 0.f };		/* ������ �ð� ���� */
		_float  fLerpTimeAcc = { 0.f };		/* ���� ���� �ð����� */

		_float  fLifeTimeRatio = { 0.f };	/* ������Ÿ���� 0~1�� ������ �� */

		// ����
		// ���� ��ü�� ���ӿ�����Ʈ�� �ִ� �� ���
		_float4x4	 matPivot		= {};	/* XMStoreFloat4x4(&m_tEffectDesc.matPivot, XMMatrixIdentity()); */
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
	virtual _bool Write_Json(json& Out_Json)		override;
	virtual void Load_FromJson(const json& In_Json)	override;

public:
	void	ReSet_Effect();
	void	End_Effect();

public:
	map<const wstring, class CGameObject*>* Get_PartObjects() { return &m_PartObjects; }
	CGameObject* Get_FirstPartObject() { return m_PartObjects.begin()->second; }


	CGameObject*	Find_PartObject(const wstring& strPartTag);
	HRESULT			Add_PartObject(const wstring& strPrototypeTag, const wstring& strPartTag, void* pArg);
	void			Delete_PartObject(const wstring& strPartTag);


public:
	EFFECT_DESC* Get_Desc() { return &m_tEffectDesc; }

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects(const wstring& strPrototypeTag, const wstring& strPartTag, void* pArg);

private:
	EFFECT_DESC		m_tEffectDesc = {};

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