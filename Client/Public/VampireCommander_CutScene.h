#pragma once
#include "VampireCommander_State_CutScene.h"

BEGIN(Client)

class CVampireCommander_CutScene : public CVampireCommander_State_CutScene
{
public:
	virtual void Initialize(CVampireCommander* pActor) override;
	virtual CState<CVampireCommander>* Update(CVampireCommander* pActor, _float fTimeDelta) override;
	virtual void Release(CVampireCommander* pActor) override;

public:
	static const _uint g_iAnimIndex = CVampireCommander::VampireCommander_State::Finisher_VampireCommander_VS;


private:
	void	PlaySound_Breath(_float fTimeDelta); //! �¿� �߰�

private: //!�¿� �߰�
	_float	m_fSoundTimeAcc = 0.f;
	_float  m_fBreathPlayTime = 2.25f;
	_bool   m_bBreathChange = false; //! 1�� ���� 2�� ���� �����ư��鼭
};

END
