#pragma once
#include "State.h"
#include "Infected.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CInfected_State abstract : public CState<CInfected>
{
public:
	virtual void Initialize(CInfected* pActor) override;
	virtual CState<CInfected>* Update(CInfected* pActor, _float fTimeDelta) override;
	virtual void Release(CInfected* pActor) override;

public:
	virtual CState<CInfected>* Update_State(CInfected* pActor, _float fTimeDelta, _uint _iAnimIndex) PURE;



public:		//���� �׷� �� ������Ʈ 
	CState<CInfected>* Normal_State(CInfected* pActor, _float fTimeDelta, _uint _iAnimIndex);




public:		//���� ���� ���� ����
	CState<CInfected>* Normal(CInfected* pActor, _float fTimeDelta, _uint _iAnimIndex);




public:
	_int iKeyUp = DIK_W;	//DIK_UP;
	_int iKeyDown = DIK_S;	//DIK_DOWN;
	_int iKeyLeft = DIK_A;	//DIK_LEFT;
	_int iKeyRight = DIK_D;	//DIK_RIGHT;


protected:
	CGameInstance* m_pGameInstance = { nullptr };
};

END