#pragma once
#include "State.h"
#include "MotherMouth.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CMotherMouth_State abstract : public CState<CMotherMouth>
{
public:
	virtual void Initialize(CMotherMouth* pActor) override;
	virtual CState<CMotherMouth>* Update(CMotherMouth* pActor, _float fTimeDelta) override;
	virtual void Release(CMotherMouth* pActor) override;

public:
	virtual CState<CMotherMouth>* Update_State(CMotherMouth* pActor, _float fTimeDelta, _uint _iAnimIndex) PURE;

public:		//���� �׷� �� ������Ʈ 
	CState<CMotherMouth>* Normal_State(CMotherMouth* pActor, _float fTimeDelta, _uint _iAnimIndex);
	CState<CMotherMouth>* Attack_State(CMotherMouth* pActor, _float fTimeDelta, _uint _iAnimIndex);

public:		//���� ���� ���� ����
	CState<CMotherMouth>* Normal(CMotherMouth* pActor, _float fTimeDelta, _uint _iAnimIndex);

protected:
	CGameInstance* m_pGameInstance = { nullptr };
};

END