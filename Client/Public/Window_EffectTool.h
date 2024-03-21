#pragma once

#include "Imgui_Window.h"

BEGIN(Client)
class CEffect;
class CEffect_Void;
class CEffect_Particle;
class CEffect_Rect;
class CEffect_Instance;
class CEffect_Trail;

class CSky;

class CWindow_EffectTool final : public CImgui_Window
{
public:
	enum TYPE_FILE { FILE_EFFECT, FILE_TRAIL, TYPE_FILE_END };

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


/* For.Save&Load */
public:
	void	Update_SaveLoad_Menu();	// ���� �ҷ����� �޴� ������Ʈ
	virtual	HRESULT		Save_Function(string strPath, string strFileName) override;
	virtual HRESULT		Load_Function(string strPath, string strFileName) override;


/* For.Level Setting (ȯ�� ����) */
public:
	void	Show_ImGui_WindowSize();	// ImGui ������ â ũ�� ǥ��
	void	Show_MousePos();			// ���콺 ��ġ ǥ��

	void	Show_CameraInfo();	// ī�޶� ���� ǥ��
	void	ReSet_CameraPos();	// ī�޶� ��ġ ����

	HRESULT Ready_Grid();		// �׸��� ����

	HRESULT Ready_Sky();		// ��ī�̹ڽ� �غ�
	void	Set_SkyTexture();	// ��ī�̹ڽ� �ؽ�ó ����

	HRESULT Ready_Model_Preview(wstring strModelTag);	// ũ��񱳿� �� ����


/* For.Window Update (â ������Ʈ) */
public:
	void	Update_LevelSetting_Window();	// ����(ȯ��) ���� â(ī�޶�, ��ī�̹ڽ�, ũ��񱳿� �� ��...)

	void	Update_EffectList_Window();		// ����Ʈ ����Ʈ�ڽ� â

	void	Update_Timeline_Window();		// Ÿ�Ӷ��� â
	void	Update_NeoSequencer_Window();	// ������ â

	void	Update_ImageList_Window();		// �ؽ�ó �̹��� �̸�����, ����Ʈ

	void	Update_ParticleTab();			// ��ƼŬ ��
	void	Update_RectTab();				// ��Ʈ ��
	void	Update_MeshTab();				// �޽� ��
	void	Update_TrailTab();				// Ʈ���� ��


/* For.Create & Add (����Ʈ ���� ����) */
	HRESULT Create_EffectObject(const wstring& strLayerTag, CGameObject* pOwner = nullptr);		// ��Ʈ ����Ʈ�� ���� ����Ʈ ������Ʈ ���� �Լ�
	HRESULT Add_Part_Particle();						// ��Ʈ:2D��ƼŬ �߰�
	HRESULT Add_Part_Rect();							// ��Ʈ:Rect �߰�
	HRESULT Add_Part_Mesh(wstring strModelTag);			// ��Ʈ:�޽� ����Ʈ �߰�
	HRESULT Add_Part_Mesh_Morph(wstring strModelTag1, wstring strModelTag2);	// ��Ʈ:�޽�(����) ����Ʈ �߰�
	
	HRESULT Create_Trail();								// Ʈ���� ����
	HRESULT Delete_Trail();								// Ʈ���� ����

	void	Delete_CurEffectObject();					// ���� ���õ� ����Ʈ ����
	void	Delete_CurPart();							// ���� ���õ� ��Ʈ ����Ʈ ����		

	void	Copy_CurPart();								// ���� ���õ� ��Ʈ ����Ʈ ����

/* For.CurObject (���� ���õ� ����Ʈ ���� ���� ������Ʈ) */
	void	Update_CurMembers(wstring strName);		// ���� ����Ʈ�� ���� ������Ʈ
	void	Update_CurParameters();

	void	Update_CurParameters_Parts();			// ���� ��Ʈ ����Ʈ�� ���� ������Ʈ

public:
	void	Select_EasingType(EASING_TYPE* eType);	// ��¡ Ÿ��(��������) ����

private:
	TYPE_FILE	m_eFile = { TYPE_FILE_END };


private:
	map<const wstring, class CEffect*>		m_pEffects;
	map<const wstring, class CGameObject*>	m_CurPartObjects;

	char**			m_szEffectNames = { nullptr };
	char*			m_cCurEffectName = { nullptr };
	_int			m_iCurEffectIndex = { 0 };
	class CEffect*	m_pCurEffect = { nullptr };


	char**		  m_szPartNames			= { nullptr };
	char*		  m_cCurPartName		= { nullptr };
	_int		  m_iCurPartIndex		= { 0 };
	CEffect_Void* m_pCurPartEffect	    = { nullptr };



/* Desc */
private:
	CEffect::EFFECT_DESC*							m_pCurEffectDesc = { nullptr };	// Effect Desc (����)

	CEffect_Void::EFFECTVOID_DESC*					m_pCurVoidDesc = {}; // Effect_Void Desc (����)

	/* Particle Desc */
	CVIBuffer_Particle::PARTICLE_BUFFER_DESC*		m_pParticleBufferDesc	= {};	// ��ƼŬ ���� Desc
	CEffect_Void::UVSPRITE_DESC*					m_pSpriteDesc_Particle	= {};	// ��ƼŬ�� ����� Void�� ��������Ʈ Desc

	/* Rect Desc */
	CEffect_Rect::EFFECT_RECT_DESC*					m_pRectDesc			= {};		// Rect���� Desc


	/* Instance(Mesh) Desc */
	CEffect_Instance::EFFECT_INSTANCE_DESC*							m_pInstanceDesc		= {};	// Instance(Mesh)���� Desc
	CVIBuffer_Effect_Model_Instance::EFFECT_MODEL_INSTANCE_DESC*	m_pMeshBufferDesc	= {};	// Instance(Mesh) ���� Desc


/* Trail */
private:
	CEffect_Trail* m_pTrail = { nullptr };

	/* Trail Desc */
	CEffect_Void::EFFECTVOID_DESC*		m_pVoidTrailDesc = {};		// Effect_Void Desc (Ʈ���� ����)
	CEffect_Trail::TRAIL_DESC*			m_pTrailDesc = {};			// Trail���� Desc
	CVIBuffer_Trail::TRAIL_BUFFER_DESC* m_pTrailBufferDesc = {};	// Trail ���� Desc


private:
	_int m_iRenderGroup_Particle							= { ECast(CRenderer::RENDER_EFFECT) };
	_int m_iShaderPassIndex_Particle						= { 0 };
	_int m_iMaxShaderPassIndex_Particle						= { 4 };
	_int m_iTexIndex_Particle[CEffect_Void::TEXTURE_END]	= {};
	_int m_iMaxTexIndex_Particle[CEffect_Void::TEXTURE_END] = { 12, 0, 44, 24, 20 };


	_int m_iRenderGroup_Rect								= { ECast(CRenderer::RENDER_EFFECT) };
	_int m_iShaderPassIndex_Rect							= { 0 };
	_int m_iMaxShaderPassIndex_Rect							= { 5 };
	_int m_iTexIndex_Rect[CEffect_Void::TEXTURE_END]		= {};
	_int m_iMaxTexIndex_Rect[CEffect_Void::TEXTURE_END]		= { 12, 0, 44, 24, 20 };


	_int m_iRenderGroup_Mesh								= { ECast(CRenderer::RENDER_EFFECT) };
	_int m_iShaderPassIndex_Mesh							= { 0 };
	_int m_iMaxShaderPassIndex_Mesh							= { 6 };
	_int m_iTexIndex_Mesh[CEffect_Void::TEXTURE_END]		= {};
	_int m_iMaxTexIndex_Mesh[CEffect_Void::TEXTURE_END]		= { 12, 0, 44, 24, 20 };


	_int m_iRenderGroup_Trail								= { ECast(CRenderer::RENDER_EFFECT) };
	_int m_iShaderPassIndex_Trail							= { 0 };
	_int m_iMaxShaderPassIndex_Trail						= { 5 };
	_int m_iTexIndex_Trail[CEffect_Void::TEXTURE_END]		= {};
	_int m_iMaxTexIndex_Trail[CEffect_Void::TEXTURE_END]	= { 12, 0, 44, 24, 20 };


private:
	_float	m_vTimeAccs_Effect[3]	= { 0.f, 0.f, 0.f };
	_float	m_vTimeAccs_Part[3]		= { 0.f, 0.f, 0.f };

private:
	_int	m_iLoop = { 0 };
	_int	m_iType_Dead = { 0 };

	_float	m_vTimes_Effect[3]	= { 0.f, 5.f, 0.f };	// Wait, LifeTime, Remain
	_float	m_vTimes_Part[3]	= { 0.f, 5.f, 0.f };	// Wait, LifeTime, Remain

	_float	m_vWorldPosition_Effect[3]	 = { 0.f, 0.f, 0.f };
	_float	m_vWorldPosition_Part[3]	 = { 0.f, 0.f, 0.f };

	_float	m_vScale_Effect[3] = { 0.f, 0.f, 0.f };
	_float	m_vScale_Part[3] = { 0.f, 0.f, 0.f };

	_float	m_vRotate_Effect[3] = { 0.f, 0.f, 0.f };
	_float	m_vRotate_Part[3] = { 0.f, 0.f, 0.f };

	_float m_vColor_Clip_Part[4] = { 0.f, 0.f, 0.f, 0.f };


#pragma region Particle �ɼ� ����
private:
	_int	m_iNumInstance_Particle		= { 200 };
	_int	m_iMaxNumInstance_Particle	= { 500 };

	_int	m_iBillBoard				= { 0 };

	/* ��ƼŬ���� �Ӽ� */
	_int	m_iRecycle_Particle			= { 0 };
	_int	m_iReverse_Particle			= { 0 };
	_int	m_iType_Emit_Particle		= { 0 };
	_int	m_iType_Action_Particle		= { 0 };
	_int	m_iType_Fade_Particle		= { 0 };


	_float	m_vMinMaxLifeTime_Particle[2] = { 0.f, 0.f };	// ������Ÿ��

	/* RigidBody ============================================== */
	_int	m_iUseRigidBody_Particle	= { 0 };
	_int	m_iKinetic_Particle			= { 0 };
	_int	m_iUseGravity_Particle		= { 0 };

	_float	m_fGravity_Particle = { -9.8f };		// �߷� ���ӵ�
	
	//_float	m_fFriction_Particle = { 0.1f };		// ���� ���
	_float		m_vFrictionLerp_Pos_Particle[2] = { 0.f, 0.f };		// ��𼭺��� ������ �����ϰ�, ��������
	_float		m_vStartEnd_Friction_Particle[2] = { 0.1f, 0.1f };


	_float	m_fSleepThreshold_Particle = { 0.05f };	// ���� �Ѱ���(1���Ϸ�)

	_float	m_vMinMaxPower_Particle[2]	= { 0.1f, 250.f };
	_float	m_vMinMaxMass_Particle[2]	= { 10.f, 10.f };
	_float	m_vMinMaxSpeed_Particle[2] = { 1.f, 1.f };
	/* RigidBody ============================================== */


	/* For.Position */
	_float	m_vMinCenterOffsetPos_Particle[3] = { 0.f, 0.f, 0.f };
	_float	m_vMaxCenterOffsetPos_Particle[3] = { 0.f, 0.f, 0.f };
	_float	m_vMinMaxRange_Particle[2] = { 0.f, 0.f };

	/* For.Rotation */
	_float	m_vRadian_Particle[3] = { 0.f, 0.f, 0.f };
	_float	m_vRotationOffsetX_Particle[2] = { 0.f, 0.f };
	_float	m_vRotationOffsetY_Particle[2] = { 0.f, 0.f };
	_float	m_vRotationOffsetZ_Particle[2] = { 0.f, 0.f };


	/* For.Scale */
	//_float	m_vLerpScale_Pos_Particle[2]	= { 0.f, 1.f };

	_int	m_iUseScaleLerp					= { 0 };
	_float	m_vScaleLerp_Up_Pos[2]			= { 0.f, 0.3f };
	_float	m_vScaleLerp_Down_Pos[2]		= { 1.f, 1.f };
	_float	m_vMinMaxWidth_Particle[2]		= { 0.f, 1.f };
	_float	m_vMinMaxHeight_Particle[2]		= { 0.f, 1.f };
	//_float m_vScaleSpeed_Particle[2]		= { 0.005f, 0.05f };

	/* For.Color */
	_int	m_iDynamic_Color_Particle = { 0 };
	_float	m_fColor_Min_Particle[4] = { 1.f, 1.f, 1.f, 1.f };
	_float	m_fColor_Max_Particle[4] = { 1.f, 1.f, 1.f, 1.f };
	_float	m_fColor_Cur_Particle[4] = { 1.f, 1.f, 1.f, 1.f };

	_int	m_iColor_Mode_Particle = { 0 };
	_float	m_fColor_Mul_Particle[4] = { 1.f, 1.f, 1.f, 1.f };


	/* UV Option_Particle */
	_float  m_vUV_Offset_Particle[2]	= { 0.f, 0.f };
	_float  m_vUV_Scale_Particle[2]		= { 1.f, 1.f };
	_float	m_fUV_RotDegree_Particle	= { 0.f };			// UVȸ��

	_float	m_fAddScale					= { 0.f };
	_float	m_vAddScale[2]				= { 0.f, 0.f };


	_int	m_iSortZ = { 0 };

	/* For.Sprite ======================================== */
	_int	m_iSprite_Particle				= { 0 };	// 1�� True
	_int	m_iSpriteLoop					= { 0 };
	_int	m_vUV_MaxTileCount_Particle[2]	= { 7, 7 };
	_float	m_fSequenceTerm_Particle		= { 0.05f };
	/* For.Sprite ======================================== */



	/* Distortion ============================================== */
	_float m_fSequenceTerm_Distortion_Particle = { 1.f };
	_int	m_iType_Scroll_Particle = { 0 };

	_float	m_vScrollSpeeds_Particle[3] = { 1.f, 1.f, 0.f };
	_float	m_vScales_Distortion_Particle[3] = { 1.f, 1.f, 1.f };

	_float	m_vDistortion1_Particle[2] = { 0.1f, 0.1f };
	_float	m_vDistortion2_Particle[2] = { 0.0f, 0.0f };
	_float	m_vDistortion3_Particle[2] = { 0.0f, 0.1f };

	_float	m_fDistortionScale_Particle = { 1.f };
	_float	m_fDistortionBias_Particle = { 1.f };
	/* Distortion ============================================== */



	/* ��ƼŬ Rim & Bloom */
	_float	m_fRimColor_Particle[4] = { 1.f, 1.f, 1.f, 1.f };
	_float	m_vBloomPower_Particle[3] = { 1.f, 1.f, 1.f };

#pragma endregion Particle �ɼ� ��



#pragma region Rect �ɼ� ����

	/* For.Sprite ======================================== */
	_int	m_iSprite_Rect = { 0 };					// 1�� True
	_int	m_iSpriteLoop_Rect = { 0 };
	_int	m_vUV_MaxTileCount_Rect[2] = { 7, 7 };
	_float	m_fSequenceTerm_Rect = { 0.05f };
	/* For.Sprite ======================================== */


	_int	m_iBillBoard_Rect = { 0 };

	_float	m_fColor_Mul_Rect[4] = { 1.f, 1.f, 1.f, 1.f };	// ���̴����� ������ ��
	_float	m_vColor_Clip_Rect[4] = { 0.f, 0.f, 0.f, 0.f };	// ���̴����� discard�� ��


	/* Distortion */
	_int	m_iType_Scroll_Rect = { 0 };

	_float	m_vScrollSpeeds[3]			= { 1.f, 1.f, 0.f };
	_float	m_vScales_Distortion[3]		= { 1.f, 1.f, 1.f };

	_float	m_vDistortion1[2] = { 0.1f, 0.1f };
	_float	m_vDistortion2[2] = { 0.0f, 0.0f };
	_float	m_vDistortion3[2] = { 0.0f, 0.1f };

	_float	m_fDistortionScale = { 1.f };
	_float	m_fDistortionBias = { 1.f };


	/* UV Option_Rect */
	_float  m_fUV_Offset_Rect[2] = { 0.f, 0.f };
	_float  m_vUV_Scale_Rect[2] = { 1.f, 1.f };
	_float	m_fUV_RotDegree_Rect = { 0.f };	// UVȸ��


	/* ��Ʈ Rim & Bloom */
	_float	m_fRimColor_Rect[4] = { 1.f, 1.f, 1.f, 1.f };
	_float	m_vBloomPower_Rect[3] = { 1.f, 1.f, 1.f };

#pragma endregion Rect �ɼ� ��



#pragma region Instance(Mesh) �ɼ� ����
private:
	_int m_iNumInstance_Mesh	= { 20 };
	_int m_iMaxNumInstance_Mesh = { 20 };

	_int	m_iRecycle_Mesh = { 0 };

	_float	m_vMinMaxLifeTime_Mesh[2] = { 0.f, 0.f };	// ������Ÿ��

	/* Morph */
	_float m_fMorphTimeTerm = { 0.05f };


	/* RigidBody ============================================== */
	_int	m_iUseRigidBody_Mesh	= { 0 };
	_int	m_iKinetic_Mesh			= { 0 };
	_int	m_iUseGravity_Mesh		= { 0 };

	_float	m_fGravity_Mesh			= { -9.8f };	// �߷� ���ӵ�
	_float	m_fFriction_Mesh		= { 0.1f };		// ���� ���
	_float	m_fSleepThreshold_Mesh	= { 0.05f };	// ���� �Ѱ���(1���Ϸ�)

	_float	m_vMinMaxPower_Mesh[2]	= { 0.1f, 250.f };
	/* RigidBody ============================================== */


	/* Color */
	_int	m_iColor_Mode_Mesh = { 0 };
	_float	m_fColor_Mul_Mesh[4] = { 1.f, 1.f, 1.f, 1.f };


	/* Position_Mesh Particle */
	_float	m_vMinCenterOffsetPos_Mesh[3] = { 0.f, 0.f, 0.f };
	_float	m_vMaxCenterOffsetPos_Mesh[3] = { 0.f, 0.f, 0.f };

	_float	m_vMinMaxRange_Mesh[2] = { 0.f, 0.f };


	_float	m_vRotationOffsetX_Mesh[2] = { 0.f, 0.f };
	_float	m_vRotationOffsetY_Mesh[2] = { 0.f, 0.f };
	_float	m_vRotationOffsetZ_Mesh[2] = { 0.f, 0.f };


	/* UV Option */
	_float  m_fUV_Offset_Mesh[2] = { 0.f, 0.f };
	_float  m_vUV_Scale_Mesh[2] = { 1.f, 1.f };
	_float	m_fUV_RotDegree_Mesh = { 0.f };


	/* Rim Bloom */
	_float	m_vBloomPower_Mesh[3] = { 1.f, 1.f, 1.f };
	_float	m_fRimColor_Mesh[4] = { 1.f, 1.f, 1.f, 1.f };
	_float	m_fRimPower_Mesh = { 5.f };


	/* Dissolve */
	_float	m_DissolveAmount = { 0.f };


	/* Distortion ============================================== */
	_float m_fSequenceTerm_Distortion_Mesh = { 1.f };
	_int	m_iType_Scroll_Mesh = { 0 };

	_float	m_vScrollSpeeds_Mesh[3] = { 1.f, 1.f, 0.f };
	_float	m_vScales_Distortion_Mesh[3] = { 1.f, 1.f, 1.f };

	_float	m_vDistortion1_Mesh[2] = { 0.1f, 0.1f };
	_float	m_vDistortion2_Mesh[2] = { 0.0f, 0.0f };
	_float	m_vDistortion3_Mesh[2] = { 0.0f, 0.1f };

	_float	m_fDistortionScale_Mesh = { 1.f };
	_float	m_fDistortionBias_Mesh = { 1.f };
	/* Distortion ============================================== */
#pragma endregion Instance(Mesh) �ɼ� ��


#pragma region Trail �ɼ� ����
private:
	_float	m_vColor_Clip_Trail[4] = { 0.f, 0.f, 0.f, 0.f };	// ���̴����� discard�� ��
	_float	m_fColor_Mul_Trail[4] = { 1.f, 1.f, 1.f, 1.f };		// ���̴����� �߰��� ������ �� ��
	
	_float m_vPos_0[3] = { 0.f, 0.f, 0.f };	
	_float m_vPos_1[3] = { 0.f, 0.f, 0.f };

	_int	m_iMaxCnt_Trail = { 32 };
	_int	m_iLerpPointNum = { 12 };


	_float	m_vBloomPower_Trail[3]	= { 1.f, 1.f, 1.f };
	_float	m_fRimColor_Trail[4]	= { 1.f, 1.f, 1.f, 1.f };
	_float	m_fRimPower_Trail		= { 5.f };
#pragma endregion Trail �ɼ� ��


#pragma region ������
private:
	int32_t m_iCurSequenceTimePos = { 0 };
#pragma endregion


#pragma region Level Setting
private:
	/* Camera */
	CCamera*	m_pCamera = { nullptr };
	_float3		m_Camera_ResetPos = { 0.f, 1.8f, -5.f };		// ī�޶� ���� ��ġ
	_float4		m_Camera_ResetLookAt = { 0.f, 0.f, 0.f, 1.f };	// ī�޶� ���� LookAt

	/* Sky */
	CSky* m_pSky = { nullptr };
	_int m_iSkyTextureIndex = { 3 };		// ��ī�̹ڽ� �ؽ�ó �ε���

	/* Grid */
	CGrid*	m_pGrid	= { nullptr };
	_float	m_fColor_Grid[4] = { 0.3f, 0.3f, 0.3f, 1.f };	// �׸���(���̾�������) �÷�
	_int	m_iShaderPass_Grid = { 2 };


	/* Test Floor */
	CModel_Preview* m_pFloor_Preview = { nullptr };				// ���ٴ�
	_float			m_vWorldPosition_Floor[3] = { 0.f, 0.f, 0.f };	// �ٴ� �� ���� �̵�
	_float			m_vScale_Floor[3] = { 0.f, 0.f, 0.f };			// �ٴ� �� ũ��

	/* Model_Preview */
	CModel_Preview*	m_pModel_Preview = { nullptr };				// ũ�� �񱳿� ���� ��
	CPart_Preview*	m_pPart_Preview = { nullptr };				// ���� ��Ʈ(Ʈ���� Ȯ�ο�)
	_float			m_vWorldPosition_Model[3] = { 0.f, 0.f, 0.f };	// ũ�� �񱳿� �� ���� �̵�
	_float			m_vWorldRotate_Model[3] = { 0.f, 0.f, 0.f };	// ũ�� �񱳿� �� ���� ȸ�� 
	_float			m_vScale_Model[3] = { 0.f, 0.f, 0.f };			// ũ�� �񱳿� �� ũ��
	_float			m_fModelRot = { 0.f };

	CEffect* m_pTestEffect = { nullptr };
#pragma endregion



public:
	static CWindow_EffectTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

