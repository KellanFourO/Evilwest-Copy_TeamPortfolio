#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_SnowMountain final : public CLevel
{
private:
	CLevel_SnowMountain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_SnowMountain() = default;

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
	HRESULT Ready_Layer_UI_Player(const wstring& strLayerTag, void* pArg);

private:
	_int			TempLightNumber = 0;

public:
	static CLevel_SnowMountain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END