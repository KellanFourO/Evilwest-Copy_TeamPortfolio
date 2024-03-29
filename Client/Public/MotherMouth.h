#pragma once

#include "Client_Defines.h"
#include "Monster_Character.h"
#include "Actor.h"

BEGIN(Client)

class CMotherMouth final : public CMonster_Character
{
private:
	CMotherMouth(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);
	CMotherMouth(const CMotherMouth& rhs);
	virtual ~CMotherMouth() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();


public:
	CActor<CMotherMouth>* Get_Actor() { return m_pActor; }
	void Set_Actor(CActor<CMotherMouth>* _pActor) { m_pActor = _pActor; }
private:
	CActor<CMotherMouth>* m_pActor = { nullptr };
public:
	_bool	m_bLoopFinished = false;
public:
	/* ������ü�� �����Ѵ�. */
	static CMotherMouth* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag);

	/* �纻��ü�� �����Ѵ�. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual CGameObject* Pool() override;

	virtual void Free() override;

	enum MotherMouth_State
	{
		Grand_parasiter_mouth_Idle            ,
		Parasiter_SpittingMines_End_1800	  ,
		Parasiter_SpittingMines_Loop_1800	  ,
		Parasiter_SpittingMines_Start_1800
	};
};

END

