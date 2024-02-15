#pragma once

#include "Imgui_Window.h"

BEGIN(Engine)
class CEffect_Object;
//class CParticle_Custom;
END

BEGIN(Client)

class CWindow_EffectTool final : public CImgui_Window
{
public:
	struct Window_EffectTool_DESC : public ImGuiDESC
	{
	};

protected:
	CWindow_EffectTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CWindow_EffectTool() = default;

public:
	virtual	HRESULT Initialize()			override;
	virtual	void	Tick(_float fTimeDelta) override;
	virtual void	Render()				override;

public:
	void	Update_PlayArea_Particle();
	void	Update_ColorEditArea_Particle();


/* For.Imgui Util */
public:
	void HelpMarker(const char* desc);

/* For.string Util */
public:
	string	WstringToUTF8(const wstring& wstr);
	wstring CharToWstring(char* szChar);

/* For.Particle */
public:
	HRESULT Create_NewParticle();
	HRESULT Ready_Layer_Particle(const wstring& strLayerTag);
	
private:
	map<const wstring, class CParticle_Custom*>	m_pParticles;

private:
	char**  m_szParticleNames						= { nullptr };
	_int	m_iCurParticleIndex						= { 0 };
	_float	m_fLifeTimePositionEdit					= { 0.f };
	class CParticle_Custom* m_pCurParticle			= { nullptr };

	_int	m_iPlayType = { 0 };
	_int	m_iLoop = { 0 };

	_float m_vRotationX[2] = { 0.f, 0.f };
	_float m_vRotationY[2] = { 0.f, 0.f };
	_float m_vRotationZ[2] = { 0.f, 0.f };
	_float m_fParticleAcceleration	= { 1.f };
	_float m_fParticleAccPosition	= { 0.1f };


	_float m_fColor_Particle[3] = { 0.f, 0.f, 0.f };

private:
	class CGameInstance* m_pGameInstance = { nullptr };

public:
	static CWindow_EffectTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END
