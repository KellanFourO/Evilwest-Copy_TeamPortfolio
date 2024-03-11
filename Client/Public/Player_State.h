#pragma once
#include "State.h"
#include "Player.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CPlayer_State abstract : public CState<CPlayer>
{
public:
	virtual void Initialize(CPlayer* pActor) override;
	virtual CState<CPlayer>* Update(CPlayer* pActor, _float fTimeDelta) override;
	virtual void Release(CPlayer* pActor) override;

public:
	virtual CState<CPlayer>* Update_State(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex) PURE;



public:		//���� �׷� �� ������Ʈ 
	CState<CPlayer>* Normal_State(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Attack_State(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Walk_State(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Run_State(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Dodge_State(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Roll_State(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Hit_State(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);

	CState<CPlayer>* Aim_State(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Rifle_State(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Winchester_State(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Crossbow_State(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Revolver_State(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Shotgun_State(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Gatling_State(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* FlameBelcher_State(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Grenade_State(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);

	
		
	
	CState<CPlayer>* Interaction_State(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Death_State(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Finisher_State(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	

	


public:		//���� ���� ���� ����
	CState<CPlayer>* Normal(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Walk(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Run(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Attack(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* MeleeCombo(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Dodge(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Roll(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);

	CState<CPlayer>* Rifle(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Winchester(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Crossbow(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Revolver(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Shotgun(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Gatilng(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* FlameBelcher(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CPlayer>* Grenade(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);




	CState<CPlayer>* TeleportPunch(CPlayer* pActor, _float fTimeDelta, _uint _iAnimIndex);



public:
	_int iKeyUp		= DIK_W;	//DIK_UP;
	_int iKeyDown	= DIK_S;	//DIK_DOWN;
	_int iKeyLeft	= DIK_A;	//DIK_LEFT;
	_int iKeyRight	= DIK_D;	//DIK_RIGHT;


protected:
	CGameInstance* m_pGameInstance = { nullptr };
};

END