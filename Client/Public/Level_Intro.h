#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Intro final : public CLevel
{
private:
	CLevel_Intro(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Intro() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_LightDesc();
	HRESULT Ready_Layer_Camera(const wstring& strLayerTag);
	HRESULT Ready_Layer_Effect(const wstring& strLayerTag);
	HRESULT Ready_Layer_BackGround(const wstring& strLayerTag);
	HRESULT Ready_LandObjects();
	HRESULT Ready_Layer_Player(const wstring& strLayerTag, void* pArg);
	HRESULT Ready_Layer_Monster(const wstring& strLayerTag, void* pArg);
	HRESULT Ready_Layer_Building(const wstring& strLayerTag, void* pArg);
	HRESULT Ready_Layer_Test(const wstring& strLayerTag);

private: /* UI */
	HRESULT Ready_UI();
	HRESULT Ready_Layer_UI_Monster(const wstring& strLayerTag, void* pArg);
#pragma region Interface
	HRESULT Ready_Layer_UI_Interface(const wstring& strLayerTag, void* pArg);
	HRESULT Ready_LeftInterface(const wstring& strLayerTag, void* pArg);
	HRESULT Ready_RightInterface(const wstring& strLayerTag, void* pArg);
	HRESULT Ready_QuestBox(const wstring& strLayerTag, void* pArg);
	HRESULT Ready_TutorialBox(const wstring& strLayerTag, void* pArg);
	HRESULT Ready_LevelUp(const wstring& strLayerTag, void* pArg);
	HRESULT Ready_Reward_Item(const wstring& strLayerTag, void* pArg);
#pragma endregion End
	HRESULT Ready_Layer_UI(const wstring& strLayerTag, void* pArg);

private:
	_int			TempLightNumber = 0;

	string			m_strStage1MapLoadPath = "../Bin/DataFiles/Data_Map/Stage1Map_MapData_MapData.json";

public:
	static CLevel_Intro* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END