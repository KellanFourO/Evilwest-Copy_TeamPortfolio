#pragma once
#include "VampireCommander_Stun.h"

BEGIN(Client)

class CVampireCommander_Stun_Loop : public CVampireCommander_Stun
{
public:
	virtual void Initialize(CVampireCommander* pActor) override;
	virtual CState<CVampireCommander>* Update(CVampireCommander* pActor, _float fTimeDelta) override;
	virtual void Release(CVampireCommander* pActor) override;

public:
	static const _uint g_iAnimIndex = CVampireCommander::VampireCommander_State::VampireCommander_Stun_Loop;

private:
	void	PlaySound_Breath(_float fTimeDelta); //! �¿� �߰�
	
private: //!�¿� �߰�
	_float	m_fSoundTimeAcc = 0.f;
	_float  m_fBreathPlayTime = 1.5f;
	_bool   m_bBreathChange = false; //! 1�� ���� 2�� ���� �����ư��鼭
};

END