#pragma once
#include "Client_Defines.h"
#include "Engine_Macro.h"

#include "Base.h"
#include "GameInstance.h"


BEGIN(Client)
class CEffect;
class CEffect_Trail;

//! EFFECT_MANAGER ��ũ�� : GetInstance()���ص� ��. (EFFECT_MANAGER->Create_Effect(~~);)

class CEffect_Manager : public CBase
{
	DECLARE_SINGLETON(CEffect_Manager);

private:
	CEffect_Manager();
	virtual ~CEffect_Manager() = default;

public:
	HRESULT		Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

public:	
	//! ���� : strFileName�� json���� �̸�. ("../Bin/DataFiles/Data_Effect/" ��ο� ������ ���� json���� �� ���ϴ°� ��󾲱�!)
	CEffect* Create_Effect(_uint iLevelIndex, const wstring& strLayerTag, string strFileName, CGameObject* pOwner = nullptr);
	CEffect* Create_Effect_With_Trail(string strEffectFileName, string strTrailFileName, CGameObject* pOwner = nullptr);	// Ʈ������ �޸� ����Ʈ�� ������ �� ����ϱ�


	//! ��� : ���ϰ� ������ ���� �Լ��� Ȱ���� ����ȭ ��Ŵ
	CEffect* Create_Effect(string strFileName, CGameObject* pOwner = nullptr);


	CEffect_Trail*	Ready_Trail(_uint iLevelIndex, const wstring& strLayerTag, string strFileName, CGameObject* pOwner = nullptr);


private:
	class CGameInstance*	m_pGameInstance = { nullptr };
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };


public:
	virtual void	Free();
};

END

