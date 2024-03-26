#pragma once
#include "State.h"
#include "Tank.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CTank_State abstract : public CState<CTank>
{
public:
	virtual void Initialize(CTank* pActor) override;
	virtual CState<CTank>* Update(CTank* pActor, _float fTimeDelta) override;
	virtual void Release(CTank* pActor) override;

public:
	virtual CState<CTank>* Update_State(CTank* pActor, _float fTimeDelta, _uint _iAnimIndex) PURE;



public:		//���� �׷� �� ������Ʈ 
	CState<CTank>* Normal_State(CTank* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CTank>* Walk_State(CTank* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CTank>* Run_State(CTank* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CTank>* Attack_State(CTank* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CTank>* Shield_State(CTank* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CTank>* NoShield_State(CTank* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CTank>* Hit_State(CTank* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CTank>* Death_State(CTank* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CTank>* Spawn_State(CTank* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CTank>* Stun_State(CTank* pActor, _float fTimeDelta, _uint _iAnimIndex);


public:		//���� ���� ���� ����
	CState<CTank>* Normal(CTank* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CTank>* Idle(CTank* pActor, _float fTimeDelta, _uint _iAnimIndex);


protected:
	CGameInstance* m_pGameInstance = { nullptr };
};

END