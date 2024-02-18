#pragma once

#include "Imgui_Window.h"


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
	void	Update_PlayBarArea_Particle();
	void	Update_ParticleTab();
	void	Update_MeshTab();


/* For.Particle_Update */
public:
	void	Update_Demo_Sequencer();

	void	Update_Demo_Particle();
	void	Update_ListArea_Particle();
	void	Update_PlayArea_Particle();
	void	Update_AppearArea_Particle();
	void	Update_ActionArea_Particle();

	void	Update_WorldPositionArea_Particle();

	void	Update_CurParameters(wstring strName = TEXT(""));


/* For.Effect_Instance_Update */
public:
	void	Update_Demo_Effect_Instance();
	void	Update_Trnasform_Effect_Instance();

/* For.Save&Load */
public:
	void	Update_SaveLoad_Particle();
	virtual	HRESULT		Save_Function() override;
	virtual HRESULT		Load_Function() override;


/* For.Tool Util */
public:
	wstring Make_NameWithPin(const wstring& strFrontName);


/* For.Create_Object */
public:
	HRESULT Ready_Layer_Greed(const wstring& strLayerTag);

	HRESULT Create_New_Particle();
	HRESULT Ready_Layer_Particle(const wstring& strLayerTag);

	HRESULT Create_New_EffectInstance(wstring strModelTag);
	HRESULT Ready_Layer_EffectInstance(const wstring& strLayerTag, wstring strModelTag);


#pragma region Sequencer
private:
	int32_t m_iCurrentFrame = { 0 };
#pragma endregion

private:
#pragma region Particle_List
	map<const wstring, class CParticle_Custom*>	m_pParticles;

	char**					m_szParticleNames		= { nullptr };

	char*					m_cCurParticleName		= { nullptr };	
	_int					m_iCurParticleIndex		= { 0 };
	class CParticle_Custom*	m_pCurParticle			= { nullptr };
#pragma endregion

#pragma region Particle_Option
	_int	m_iLoop = { 0 };

	CVIBuffer_Particle_Point::TYPE_ACTION	m_eParticleTypeAction = { CVIBuffer_Particle_Point::TYPE_ACTION::SPHERE };
	CVIBuffer_Particle_Point::TYPE_FADE		m_eParticleTypeFade = { CVIBuffer_Particle_Point::TYPE_FADE::FADE_NONE };

	_int	m_iTextureIndex = { 0 };

	_int	m_iMaxNumInstance = { 500 };
	_int	m_iNumInstance = { 200 };

	_float	m_fAddScale = { 0.f };
	_float	m_vAddScale[2] = { 0.f, 0.f };

	_float	m_vMinMaxLifeTime[2] = { 0.f, 0.f };

	_float	m_fRotateUvDegree = { 0.f };

	_float	m_vMinMaxRange[2] = { 0.f, 0.f };
	_float	m_fMaxLengthPosition = { 0.f };

	_float	m_vRotationOffsetX[2] = { 0.f, 0.f };
	_float	m_vRotationOffsetY[2] = { 0.f, 0.f };
	_float	m_vRotationOffsetZ[2] = { 0.f, 0.f };

	_float	m_fParticleAcceleration = { 1.f };
	_float	m_fParticleAccPosition = { 0.1f };

	_float	m_fColor_Particle[3] = { 1.f, 1.f, 1.f };

	_float	m_vWorldPosition[4] = { 0.f, 0.f, 0.f, 1.f };
#pragma endregion


#pragma region EffectInstance_List
	map<const wstring, class CEffect_Instance*>	m_pEffectInstances;

	char** m_szEffectInstanceNames = { nullptr };

	char* m_cCurEffectInstanceName = { nullptr };
	_int					m_iCurEffectInstanceIndex = { 0 };
	class CEffect_Instance* m_pCurEffectInstance = { nullptr };
#pragma endregion


#pragma region EffectInstance_Option
	_int	m_iShaderPassIndex = { 0 };

	_float	m_vWorldPosition_EffectInstance[4] = { 0.f, 0.f, 0.f, 1.f };
	_float	m_vScale_EffectInstance = { 0.f };

#pragma endregion


public:
	static CWindow_EffectTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

