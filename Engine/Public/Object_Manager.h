#pragma once

#include "Base.h"

/* ������ ���� ��ü�� ��Ƴ��´�. (������Ÿ��) */
/* ������ü�� �����Ͽ� ������ �纻��ü�� ���̾�� �����Ͽ� ��Ƴ��´�. */
/* �����ϰ� �ִ� �纻��ü���� Tick���� �Լ��� �ݺ������� ȣ�����ش�.  */



BEGIN(Engine)

class CObject_Manager final : public CBase
{
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	class CComponent* Get_Component(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strComponentTag, _uint iIndex, const wstring& strPartTag);

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_CloneObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg);
	CGameObject* Clone_Prototype(const wstring& strPrototypeTag, void* pArg);

	HRESULT Fill_PoolObject(class CGameObject* pGameObject);
	HRESULT Create_PoolObjects(const wstring& strPrototypeTag, _uint iSize = 10);


	void Priority_Tick(_float fTimeDelta);
	void Tick(_float fTimeDelta);
	void Late_Tick(_float fTimeDelta);
	void Clear(_uint iLevelIndex);

public:
	list<class CGameObject*>* Get_GameObjects(_uint iLevelIndex, const wstring& strLayerTag);
	void Get_CloneGameObjects(_uint iLevelIndex, vector<CGameObject*>* clonevector);
	void Fill_PrototypeTags(vector<string>* _vector);

public:
	class CCharacter* Get_Player();
	void Set_Player(CCharacter* _pPlayer);

public:
	void Set_Layer_Speed(_uint iLevelIndex, const wstring& strLayerTag, _float fSpeed);
	void Reset_Layers_Speed(_uint iLevelIndex);

	void Set_Layer_Stop(_uint iLevelIndex, const wstring& strLayerTag, _bool bStop);
	void Reset_Layer_Stop(_uint iLevelIndex);






private:
	_uint			m_iNumLevels = { 0 };

private:
	map<const wstring, class CGameObject*>			m_Prototypes;
	map<const wstring, class CLayer*>*				m_pLayers = { nullptr } ;
	typedef map<const wstring, class CLayer*>		LAYERS;

	map<const wstring, list<class CGameObject*>>	m_Pool;

private:
	class CGameInstance* m_pGameInstance = { nullptr };
	class CCharacter* m_pPlayer = { nullptr };

private:
	class CGameObject* Find_Prototype(const wstring& strPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const wstring& strLayerTag);
public:
	static CObject_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END