#include "stdafx.h"
#include "Window_EffectTool.h"

/* ImGui ���� */
#include "imNeoSequencer/imgui_neo_sequencer.h"

/* Util */
#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Effect_Manager.h"
#include "Data_Manager.h"

/* Effect */
#include "Effect.h"
#include "Effect_Rect.h"
#include "Effect_Particle.h"
#include "Effect_Instance.h"
#include "Effect_Trail.h"
//#include "Mesh.h"

/* Level */
#include "MasterCamera.h"
#include "Sky.h"
#include "Grid.h"
#include "Model_Preview.h"
#include "Part_Preview.h"

#include "Player.h"
#include "VampireCommander.h"

CWindow_EffectTool::CWindow_EffectTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CImgui_Window(pDevice, pContext)
{

}


HRESULT CWindow_EffectTool::Initialize()
{
	//! ����� Ư���� ��ɾ���. ���� �ʿ��� �� ���Ƽ� ����.
	if (FAILED(__super::Initialize()))
		return E_FAIL;


	Load_CustomStyle();			// ��Ÿ�� ���� ���� �ε�
	

	ReSet_CameraPos();			// ī�޶� ������, ��ġ ����
	FAILED_CHECK(Ready_Sky());	// ��ī�̹ڽ� ������

	return S_OK;
}

void CWindow_EffectTool::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	ShowDialog();


	// ===========================================================================================================
	// ��� �˾���
	// ===========================================================================================================
	if (ImGui::BeginPopupModal(u8"Ȯ��:Ʈ���� �ε�", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(u8"���� Ʈ������ �����ϰ� �ε����ּ���.");		// ��� �޽��� ǥ��

		if (ImGui::Button(u8"Ȯ��"))
		{
			ImGui::CloseCurrentPopup();				// ��� �˾� �ݱ�
		}
		ImGui::EndPopup();
	}



#pragma region ����Ʈ �� â
	SetUp_ImGuiDESC(u8"����Ʈ ��", ImVec2{ 1000.f, 400.f }, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking /*| ImGuiWindowFlags_NoCollapse */ /* | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove */ /* | ImGuiWindowFlags_NoBringToFrontOnFocus*/, ImVec4(0.f, 0.f, 0.f, 1.f));
	
	__super::Begin();


	if (ImGui::BeginTabBar("Tab_Effect", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem(" Effect "))
		{
			m_eEdit = EDIT_EFFECT;

			Update_SaveLoad_Menu();		// ���� �ҷ����� �޴� ������Ʈ(����Ʈ)
			Update_EffectList_Window();	// ����Ʈ ����Ʈ�ڽ� â ������Ʈ

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(" Trail "))
		{
			m_eEdit = EDIT_TRAIL;

			Update_SaveLoad_Trail_Menu();		// ���� �ҷ����� �޴� ������Ʈ(����Ʈ)
			Update_EffectTrail_Window();		// ����Ʈ Ʈ���� â ������Ʈ

			ImGui::EndTabItem();
		}

	}
	ImGui::EndTabBar();


	Update_EffectTransform_Window(); // ����Ʈ Ʈ������ ������Ʈ


	// ImGuiâ ������
	Show_ImGui_WindowSize();

	__super::End();	
#pragma endregion


#pragma region ����Ʈ ���� ���� â
	SetUp_ImGuiDESC(u8"����Ʈ ���� ����", ImVec2{ 300.f, 700.f }, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking /*| ImGuiWindowFlags_NoCollapse */ /* | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove */ /* | ImGuiWindowFlags_NoBringToFrontOnFocus*/, ImVec4(0.f, 0.f, 0.f, 1.f));

	__super::Begin();

	if (EDIT_EFFECT == m_eEdit)
	{
		Update_SaveLoad_Part_Menu();		// ���� �ҷ����� �޴� ������Ʈ(��Ʈ)

		if (ImGui::BeginTabBar("Tab_Effect_Part", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem(" Particle "))
			{
				Update_ParticleTab();	// ��ƼŬ �� ������Ʈ
				ImGui::EndTabItem();
			}

			//if (ImGui::BeginTabItem(" Rect "))
			//{
			//	Update_RectTab();		// ��Ʈ �� ������Ʈ
			//	ImGui::EndTabItem();
			//}

			if (ImGui::BeginTabItem(" Mesh "))
			{
				Update_MeshTab();		// �޽� �� ������Ʈ
				ImGui::EndTabItem();
			}

			//if (ImGui::BeginTabItem(" Trail "))
			//{
			//	Update_TrailTab();		// Ʈ���� �� ������Ʈ
			//	ImGui::EndTabItem();
			//}

		}
		ImGui::EndTabBar();
	}

	if (EDIT_TRAIL == m_eEdit)
	{
		Update_TrailTab(fTimeDelta);		// Ʈ���� �� ������Ʈ
	}


	// ImGuiâ ������
	Show_ImGui_WindowSize();

	__super::End();
#pragma endregion


//#pragma region ����Ʈ Ʈ������ â
//	SetUp_ImGuiDESC(u8"Ʈ������", ImVec2{ 1000.f, 400.f }, ImGuiWindowFlags_NoDocking /*| ImGuiWindowFlags_NoCollapse */ /* | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove */ /* | ImGuiWindowFlags_NoBringToFrontOnFocus*/, ImVec4(0.f, 0.f, 0.f, 1.f));
//
//	__super::Begin();
//
//	//if (EDIT_EFFECT == m_eEdit)
//	{
//		Update_EffectTransform_Window(); // ����Ʈ Ʈ������ â ������Ʈ
//	}
//
//
//	// ImGuiâ ������
//	Show_ImGui_WindowSize();
//
//	__super::End();
//#pragma endregion



//#pragma region �׿� ������ â
//	SetUp_ImGuiDESC(u8"�׿� ������", ImVec2{ 1000.f, 400.f }, ImGuiWindowFlags_NoDocking /*| ImGuiWindowFlags_NoCollapse */ /* | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove */ /* | ImGuiWindowFlags_NoBringToFrontOnFocus*/, ImVec4(0.f, 0.f, 0.f, 0.2f));
//	
//	__super::Begin();
//
//	Update_NeoSequencer_Window();	// ������ â ������Ʈ
//
//	// ImGuiâ ������
//	Show_ImGui_WindowSize();
//
//	__super::End();
//#pragma endregion



#pragma region Ÿ�Ӷ��� â
	SetUp_ImGuiDESC(u8"Ÿ�Ӷ���", ImVec2{ 400.f, 300.f }, ImGuiWindowFlags_NoDocking /*| ImGuiWindowFlags_NoCollapse */ /* | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove */ /* | ImGuiWindowFlags_NoBringToFrontOnFocus*/, ImVec4(0.f, 0.f, 0.f, 1.f));
	__super::Begin();

	if (EDIT_EFFECT == m_eEdit)
	{
		Update_Timeline_Window();	// Ÿ�Ӷ��� â (���, ����, ����) ������Ʈ
	}



	if (EDIT_TRAIL == m_eEdit)
	{



	}


	// ImGuiâ ������
	Show_ImGui_WindowSize();

	__super::End();
#pragma endregion



#pragma region ȯ��(����) ���� â (��ī�̹ڽ�, ũ��񱳿� �� ��)
	SetUp_ImGuiDESC(u8"ȯ�� ����", ImVec2{ 400.f, 300.f }, ImGuiWindowFlags_NoDocking /*| ImGuiWindowFlags_NoCollapse */ /* | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove */ /* | ImGuiWindowFlags_NoBringToFrontOnFocus*/, ImVec4(0.f, 0.f, 0.f, 1.f));

	__super::Begin();

	Update_LevelSetting_Window();	// ����(ȯ��) ���� â(ī�޶�, ��ī�̹ڽ�, ũ��񱳿� �� ��...) ������Ʈ

	// ImGuiâ ������
	Show_ImGui_WindowSize();

	__super::End();
#pragma endregion



#pragma region �̹��� ����Ʈ â
//	SetUp_ImGuiDESC(u8"�̹��� ����Ʈ", ImVec2{ 400.f, 300.f }, ImGuiWindowFlags_NoDocking /*| ImGuiWindowFlags_NoCollapse */ /* | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove */ /* | ImGuiWindowFlags_NoBringToFrontOnFocus*/, ImVec4(0.f, 0.f, 0.f, 0.2f));
//	__super::Begin();
//
//	Update_ImageList_Window();	// �ؽ�ó �̹��� �̸�����, ����Ʈ ������Ʈ
//
//	// ImGuiâ ������
//	Show_ImGui_WindowSize();
//
//	__super::End();
#pragma endregion



}

void CWindow_EffectTool::Render()
{

}

void CWindow_EffectTool::Show_ImGui_WindowSize()
{
	ImGui::SeparatorText("");
	ImGui::Text("ImGui Window Size : %d, %d", (_int)ImGui::GetWindowContentRegionMax().x, (_int)ImGui::GetWindowContentRegionMax().y);
	ImGui::Spacing();
}

void CWindow_EffectTool::Show_MousePos()
{
	POINT	pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	ImGui::Text("Mouse Pos : %d, %d", pt.x, pt.y);
}

void CWindow_EffectTool::Show_CameraInfo()
{
	_float4 vCamPos = m_pGameInstance->Get_CamPosition();
	_float4 vCamDir = m_pGameInstance->Get_CamDirection();

	ImGui::Text("Cam Pos  : %.2f %.2f %.2f", vCamPos.x, vCamPos.y, vCamPos.z);
	ImGui::Text("Cam Dir : %.2f %.2f %.2f", vCamDir.x, vCamDir.y, vCamDir.z);
}


void CWindow_EffectTool::ReSet_CameraPos()
{
	if (nullptr == m_pCamera)
	{
		// ī�޶� �޾ƿ���
		m_pCamera = CData_Manager::GetInstance()->Get_MasterCamera()->Get_vectorCamera()[CMasterCamera::DynamicCamera];
	}
	else
	{
		m_pCamera->Set_Position(m_Camera_ResetPos);	// ī�޶� ��ġ ����
	}

}

HRESULT CWindow_EffectTool::Ready_Grid()
{
	CGrid::GRID_DESC	tDesc = {};

	tDesc.vGridColor = { 0.5f, 0.5f, 0.5f, 1.f };	// �׸��� �� ����

	//tDesc.strTextureTag[CGrid::TEXTURE_DIFFUSE] = { TEXT("Prototype_Component_Texture_Effect_Diffuse") };
	tDesc.strTextureTag[CGrid::TEXTURE_DIFFUSE] = { TEXT("") };
	tDesc.iTextureIndex[CGrid::TEXTURE_DIFFUSE] = { 0 };

	tDesc.strTextureTag[CGrid::TEXTURE_MASK] = { TEXT("") };
	tDesc.iTextureIndex[CGrid::TEXTURE_MASK] = { 0 };


	CGameObject* pObj = m_pGameInstance->Add_CloneObject_And_Get(LEVEL_TOOL, TEXT("Layer_Grid"), TEXT("Prototype_GameObject_Grid"), &tDesc);
	if (nullptr != pObj)
	{
		m_pGrid = dynamic_cast<CGrid*>(pObj);
	}
		
	return S_OK;
}


HRESULT CWindow_EffectTool::Ready_Sky()
{
	// ��ī�̹ڽ� ������
	m_pSky = CData_Manager::GetInstance()->Get_pSkyBox();

	if (m_pSky == nullptr)
		return E_FAIL;

	m_pSky->Set_SkyType(CSky::SKYTYPE::SKY_STAGE1);

	return S_OK;
}

void CWindow_EffectTool::Set_SkyTexture()
{
	if(nullptr == m_pSky)
		return;

	m_pSky->Set_SkyType((CSky::SKYTYPE)m_iSkyTextureIndex);

}

HRESULT CWindow_EffectTool::Ready_Model_Preview(wstring strModelTag)
{
	CModel_Preview::MODEL_PREVIEW_DESC	tDesc = {};
	tDesc.strProtoTag = { TEXT("Prototype_GameObject_Model_Preview") };
	tDesc.strModelTag = { strModelTag };

	if (strModelTag == TEXT("Prototype_Component_Model_Rentier"))
	{
		tDesc.iAnimIndex = { 41 };	// �÷��̾� Idle�ִϸ��̼��� 8��
	}

	if (strModelTag == TEXT("Prototype_Component_Model_VampireCommander"))
	{
		tDesc.iAnimIndex = { 9 };	// ���� Idle�ִϸ��̼��� 9�� VampireCommander_Idle
	}




	CGameObject* pObj = m_pGameInstance->Add_CloneObject_And_Get(LEVEL_TOOL, TEXT("Layer_Model_Preview"), TEXT("Prototype_GameObject_Model_Preview"), &tDesc);
	if (nullptr != pObj)
		m_pModel_Preview = dynamic_cast<CModel_Preview*>(pObj);


	if (CModel::TYPE_ANIM == m_pModel_Preview->Get_Desc()->eType)
	{
		m_pPart_Preview = dynamic_cast<CPart_Preview*>(dynamic_cast<CModel_Preview*>(m_pModel_Preview)->Find_PartObject(TEXT("Part_Preview")));
	}

	return S_OK;
}


void CWindow_EffectTool::Update_Timeline_Window()
{
	/* Ÿ�Ӷ��� */
	if (nullptr != m_pCurEffect)
	{
		auto& style = ImGui::GetStyle(); 

		_float fSequenceTimePos = m_pCurEffectDesc->fSequenceAcc;
		//_float fSequenceTimePos = m_pCurEffect->Get_EasingTimeAcc();

		ImGui::Text("%.2f : %.2f : %.2f", m_vTimes_Effect[0], m_vTimes_Effect[1], m_vTimes_Effect[2]);

		ImGui::SliderFloat("SequenceTimeAcc", &fSequenceTimePos, 0.f, m_pCurEffectDesc->fSequenceTime);

		m_vTimeAccs_Effect[0] = (m_pCurEffectDesc->fWaitingTime != 0.0f) ? min(1.0f, m_pCurEffectDesc->fWaitingAcc / m_pCurEffectDesc->fWaitingTime) : 1.0f;
		m_vTimeAccs_Effect[1] = m_pCurEffectDesc->fLifeTimeRatio;
		m_vTimeAccs_Effect[2] = (m_pCurEffectDesc->fRemainTime != 0.0f) ? min(1.0f, m_pCurEffectDesc->fRemainAcc / m_pCurEffectDesc->fRemainTime) : 1.0f;
		ImGui::SliderFloat3("TimeAccs_Effect", m_vTimeAccs_Effect, 0.f, 1.f);

		if (nullptr != m_pCurPartEffect)
		{
			if (ImGui::CollapsingHeader("TimeAccs_Part"))
			{			
				ImGui::SliderFloat("SequenceTimeAcc_Part", &m_pCurVoidDesc->fSequenceAcc, 0.f, m_pCurEffectDesc->fSequenceTime);

				m_vTimeAccs_Part[0] = (m_pCurVoidDesc->fWaitingTime != 0.0f) ? min(1.0f, m_pCurVoidDesc->fWaitingAcc / m_pCurVoidDesc->fWaitingTime) : 1.0f;
				m_vTimeAccs_Part[1] = m_pCurVoidDesc->fLifeTimeRatio;
				m_vTimeAccs_Part[2] = (m_pCurVoidDesc->fRemainTime != 0.0f) ? min(1.0f, m_pCurVoidDesc->fRemainAcc / m_pCurVoidDesc->fRemainTime) : 1.0f;
				ImGui::SliderFloat3("TimeAccs_Part", m_vTimeAccs_Part, 0.f, 1.f);
			}
		}

		/* ���, ����, ���� */
		if (ImGui::Button("   Play   ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 30)))
		{
			m_pCurEffectDesc->bPlay = TRUE;
			if (nullptr != m_pModel_Preview)
			{
				dynamic_cast<CModel_Preview*>(m_pModel_Preview)->Get_Desc()->bPlay = TRUE;
			}
		}
		//ImGui::SameLine();
		if (ImGui::Button("   Stop   ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 30)))
		{
			m_pCurEffectDesc->bPlay = FALSE;
			if (nullptr != m_pModel_Preview)
			{
				dynamic_cast<CModel_Preview*>(m_pModel_Preview)->Get_Desc()->bPlay = FALSE;
			}

		}
		//ImGui::SameLine();
		if (ImGui::Button("   Reset   ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 30)))
		{
			m_pCurEffect->Init_ReSet_Effect();

			if (nullptr != m_pModel_Preview)
			{
				dynamic_cast<CModel_Preview*>(m_pModel_Preview)->Get_ModelCom()->Reset_Animation();
			}
		}

	}
}


void CWindow_EffectTool::Update_ParticleTab()
{
	if (nullptr != m_pCurEffect)
	{
		if (nullptr != m_pCurPartEffect)
		{
			auto& style = ImGui::GetStyle();

			CEffect_Void::TYPE_EFFECT eType_Effect = m_pCurVoidDesc->eType_Effect;

			if (CEffect_Void::PARTICLE == eType_Effect)
			{

#pragma region Desc ������ ������Ʈ_��ƼŬ
				m_pCurVoidDesc = m_pCurPartEffect->Get_Desc();
				m_pSpriteDesc_Particle = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_Sprite_Desc();
				CVIBuffer_Particle* pVIBuffer = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_VIBufferCom();
				m_pParticleBufferDesc = pVIBuffer->Get_Desc();
#pragma endregion 


				/* �̸�_��ƼŬ */
				ImGui::SeparatorText(" NAME ");
				//ImGui::Text(m_pGameInstance->ConverWStringtoC(m_pCurVoidDesc->strPartTag));
				ImGui::Text(m_szPartNames[m_iCurPartIndex]);


				/* ��Ʈ ���� Ű�� ���� */
				ImGui::SeparatorText("Loop_Part");
				ImGui::RadioButton("Loop_Part", &m_iLoop_Part, 0);
				ImGui::RadioButton("None Loop_Part", &m_iLoop_Part, 1);
				if (0 == m_iLoop_Part)
					m_pCurVoidDesc->bLoop = TRUE;
				else if (1 == m_iLoop_Part)
					m_pCurVoidDesc->bLoop = FALSE;


				/* ������ Ÿ�� */
				ImGui::SeparatorText(" LIFE TIME ");
				if (ImGui::DragFloat2("MinMaxLifeTime_Particle", m_vMinMaxLifeTime_Particle, 1.f, 0.f, 360.f))
				{
					if (m_vMinMaxLifeTime_Particle[0] > m_vMinMaxLifeTime_Particle[1])	// Min�� Max���� ũ�� Max�� Min����
						m_vMinMaxLifeTime_Particle[1] = m_vMinMaxLifeTime_Particle[0];


					m_pParticleBufferDesc->vMinMaxLifeTime.x = m_vMinMaxLifeTime_Particle[0];
					m_pParticleBufferDesc->vMinMaxLifeTime.y = m_vMinMaxLifeTime_Particle[1];
				}



#pragma region �ؽ�ó ����_��ƼŬ
				/* �ؽ�ó ����_��ƼŬ */
				ImGui::SeparatorText("");
				if (ImGui::CollapsingHeader("Texture_Particle"))
				{
					// ��ǻ��_��ƼŬ �ؽ�ó ===============================================================================================
					if (ImGui::TreeNode("Diffuse_Particle"))
					{
						if (ImGui::Button("Diffuse_Base"))	// ���̽� ��ǻ��� ����
						{
							dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Change_TextureCom(TEXT("Prototype_Component_Texture_Effect_Diffuse"));
							m_iMaxTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE] = 25;
							m_iTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE] = 0;
						}

						// ��������Ʈ �ؽ�ó ����
						if (ImGui::Button("Sprite_Base"))	// ���̽� ��������Ʈ�� ����
						{
							m_eType_Sprite_Particle = CEffect_Void::TEXTURE_DIFFUSE;

							dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Change_TextureCom(TEXT("Prototype_Component_Texture_Effect_Diffuse_Sprite"));
							m_iMaxTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE] = 25;
							m_iTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE] = 0;
						}
						if (ImGui::Button("Sprite_Blood"))	// �� ��������Ʈ�� ����
						{
							m_eType_Sprite_Particle = CEffect_Void::TEXTURE_DIFFUSE;

							dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Change_TextureCom(TEXT("Prototype_Component_Texture_Effect_Diffuse_Sprite_Blood"));
							m_iMaxTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE] = 29;
							m_iTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE] = 0;
						}
						ImGui::SameLine();
						if (ImGui::Button("Sprite_Smokes"))	// ����ũ ��������Ʈ�� ����
						{
							m_eType_Sprite_Particle = CEffect_Void::TEXTURE_DIFFUSE;

							dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Change_TextureCom(TEXT("Prototype_Component_Texture_Effect_Diffuse_Sprite_Smokes"));
							m_iMaxTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE] = 35;
							m_iTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE] = 0;
						}
						// ��������Ʈ �ؽ�ó ��


						if (ImGui::InputInt("Diffuse_Particle", &m_iTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE], 1))
						{
							if (m_iMaxTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE] <= m_iTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE])
								m_iTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE] = m_iMaxTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE] - 1;

							if (0 > m_iTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE])
								m_iTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE] = 0;

							m_pCurVoidDesc->iTextureIndex[CEffect_Void::TEXTURE_DIFFUSE] = m_iTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE];
						}


						// ��ǻ�� �ؽ�ó ����
						if (ImGui::Button(" Remove Diffuse_Particle ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25)))
						{
							dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Remove_TextureCom(CEffect_Void::TEXTURE_DIFFUSE);
						}


						ImGui::SeparatorText("");
						ImGui::TreePop();
					}



					// �븻_��ƼŬ �ؽ�ó =====================================================================================================
					if (ImGui::TreeNode("Normal_Texture_Particle"))
					{
						if (ImGui::Button("Normal_Base_Particle"))	// ���̽� �븻�� ����
						{
							dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Change_TextureCom(TEXT("Prototype_Component_Texture_Effect_Normal"));
							m_iMaxTexIndex_Particle[CEffect_Void::TEXTURE_NORAML] = 9;
							m_iTexIndex_Particle[CEffect_Void::TEXTURE_NORAML] = 0;
						}


						if (ImGui::InputInt("Noraml_Particle", &m_iTexIndex_Particle[CEffect_Void::TEXTURE_NORAML], 1))
						{
							if (m_iMaxTexIndex_Particle[CEffect_Void::TEXTURE_NORAML] <= m_iTexIndex_Particle[CEffect_Void::TEXTURE_NORAML])
								m_iTexIndex_Particle[CEffect_Void::TEXTURE_NORAML] = m_iMaxTexIndex_Particle[CEffect_Void::TEXTURE_NORAML] - 1;

							if (0 > m_iTexIndex_Particle[CEffect_Void::TEXTURE_NORAML])
								m_iTexIndex_Particle[CEffect_Void::TEXTURE_NORAML] = 0;

							m_pCurVoidDesc->iTextureIndex[CEffect_Void::TEXTURE_NORAML] = m_iTexIndex_Particle[CEffect_Void::TEXTURE_NORAML];
						}


						if (ImGui::Button(" Remove_Noraml_Particle ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25))) // �븻 �ؽ�ó ������Ʈ ����
						{
							dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Remove_TextureCom(CEffect_Void::TEXTURE_NORAML);
						}

						ImGui::SeparatorText("");
						ImGui::TreePop();
					}



					// ����ũ_��ƼŬ �ؽ�ó =====================================================================================================
					if (ImGui::TreeNode("Mask_Texture_Particle"))
					{
						if (ImGui::Button("Mask_Base"))	// ���̽� ����ũ�� ����
						{
							dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Change_TextureCom(TEXT("Prototype_Component_Texture_Effect_Mask"));
							m_iMaxTexIndex_Particle[CEffect_Void::TEXTURE_MASK] = 170;
							m_iTexIndex_Particle[CEffect_Void::TEXTURE_MASK] = 0;

						}ImGui::SameLine();
						if (ImGui::Button("Mask_Waves"))	// ���̺� ����ũ�� ����
						{
							dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Change_TextureCom(TEXT("Prototype_Component_Texture_Effect_Mask_Waves"));
							m_iMaxTexIndex_Particle[CEffect_Void::TEXTURE_MASK] = 6;
							m_iTexIndex_Particle[CEffect_Void::TEXTURE_MASK] = 0;
						}

						if (ImGui::Button("Mask_Sprites"))	// ��������Ʈ ����ũ�� ����
						{
							m_eType_Sprite_Particle = CEffect_Void::TEXTURE_MASK;

							dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Change_TextureCom(TEXT("Prototype_Component_Texture_Effect_Mask_Sprite"));
							m_iMaxTexIndex_Particle[CEffect_Void::TEXTURE_MASK] = 28;
							m_iTexIndex_Particle[CEffect_Void::TEXTURE_MASK] = 0;
						}


						if (ImGui::InputInt("Mask_Particle", &m_iTexIndex_Particle[CEffect_Void::TEXTURE_MASK], 1))
						{
							if (m_iMaxTexIndex_Particle[CEffect_Void::TEXTURE_MASK] <= m_iTexIndex_Particle[CEffect_Void::TEXTURE_MASK])
								m_iTexIndex_Particle[CEffect_Void::TEXTURE_MASK] = m_iMaxTexIndex_Particle[CEffect_Void::TEXTURE_MASK] - 1;

							if (0 > m_iTexIndex_Particle[CEffect_Void::TEXTURE_MASK])
								m_iTexIndex_Particle[CEffect_Void::TEXTURE_MASK] = 0;

							m_pCurVoidDesc->iTextureIndex[CEffect_Void::TEXTURE_MASK] = m_iTexIndex_Particle[CEffect_Void::TEXTURE_MASK];
						}


						// ����ũ �ؽ�ó ����
						if (ImGui::Button(" Remove Mask_Particle ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25))) // ����ũ �ؽ�ó ������Ʈ ����
						{
							dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Remove_TextureCom(CEffect_Void::TEXTURE_MASK);
						}


						ImGui::SeparatorText("");
						ImGui::TreePop();
					}



					// ������_��ƼŬ �ؽ�ó =====================================================================================================
					if (ImGui::TreeNode("Noise_Texture_Particle"))
					{
						if (ImGui::Button("Noise_Base"))	// ���̽� ������� ����
						{
							dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Change_TextureCom(TEXT("Prototype_Component_Texture_Effect_Noise"));
							m_iMaxTexIndex_Particle[CEffect_Void::TEXTURE_NOISE] = 243;
							m_iTexIndex_Particle[CEffect_Void::TEXTURE_NOISE] = 0;
						}


						if (ImGui::InputInt("Noise_Particle", &m_iTexIndex_Particle[CEffect_Void::TEXTURE_NOISE], 1))
						{
							if (m_iMaxTexIndex_Particle[CEffect_Void::TEXTURE_NOISE] <= m_iTexIndex_Particle[CEffect_Void::TEXTURE_NOISE])
								m_iTexIndex_Particle[CEffect_Void::TEXTURE_NOISE] = m_iMaxTexIndex_Particle[CEffect_Void::TEXTURE_NOISE] - 1;

							if (0 > m_iTexIndex_Particle[CEffect_Void::TEXTURE_NOISE])
								m_iTexIndex_Particle[CEffect_Void::TEXTURE_NOISE] = 0;

							m_pCurVoidDesc->iTextureIndex[CEffect_Void::TEXTURE_NOISE] = m_iTexIndex_Particle[CEffect_Void::TEXTURE_NOISE];
						}


						if (ImGui::Button(" Remove_Noise_Particle ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25))) // ������ �ؽ�ó ������Ʈ ����
						{
							dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Remove_TextureCom(CEffect_Void::TEXTURE_NOISE);
						}

						ImGui::SeparatorText("");
						ImGui::TreePop();
					}

					ImGui::SeparatorText("");
				}
#pragma endregion


				/* UV �� ���� */
				if (ImGui::CollapsingHeader("UV Option_Particle"))
				{
					if (ImGui::DragFloat2(" UV_Offset_Particle ", m_vUV_Offset_Particle, 1.f, 0.f, 100.f))
					{
						m_pCurVoidDesc->vUV_Offset.x = m_vUV_Offset_Particle[0];
						m_pCurVoidDesc->vUV_Offset.y = m_vUV_Offset_Particle[1];
					}ImGui::SameLine();
					HelpMarker(u8"UV �̵�");

					if (ImGui::DragFloat2(" UV_Scale_Particle ", m_vUV_Scale_Particle, 1.f, 0.f, 100.f))
					{
						m_pCurVoidDesc->vUV_Scale.x = m_vUV_Scale_Particle[0];
						m_pCurVoidDesc->vUV_Scale.y = m_vUV_Scale_Particle[1];
					}ImGui::SameLine();
					HelpMarker(u8"UV ũ��(Ÿ�ϸ�)");


					/* �ؽ�ó UVȸ��_��ƼŬ */
					if (ImGui::DragFloat(" UV Degree_Particle ", &m_fUV_RotDegree_Particle, 1.f, 0.f, 360.f))
					{
						m_pCurVoidDesc->fUV_RotDegree = m_fUV_RotDegree_Particle;

					}ImGui::SameLine();
					HelpMarker(u8"ȸ��");

					ImGui::SeparatorText("");
				}


				/* ������ Ű�� ���� */
				ImGui::SeparatorText("BillBoard");
				ImGui::RadioButton("BillBoard", &m_iBillBoard, 0);
				ImGui::RadioButton("Off BillBoard", &m_iBillBoard, 1);
				if (0 == m_iBillBoard)
					m_pCurVoidDesc->bBillBoard = TRUE;
				else if (1 == m_iBillBoard)
					m_pCurVoidDesc->bBillBoard = FALSE;
				ImGui::SeparatorText("");


#pragma region ��������Ʈ ����_��ƼŬ
				if (ImGui::CollapsingHeader("Sprite_Particle"))
				{
					/* ��������Ʈ Ű�� ���� */
					ImGui::RadioButton("Off Sprite_Particle", &m_iSprite_Particle, 0);
					ImGui::RadioButton("On Sprite_Particle", &m_iSprite_Particle, 1);
					if (0 == m_iSprite_Particle)
					{
						m_pCurVoidDesc->bUseSpriteAnim = FALSE;
					}
					else if (1 == m_iSprite_Particle)
					{
						// 1�� Ų��.
						m_pCurVoidDesc->bUseSpriteAnim = TRUE;
					}


					if (m_pCurVoidDesc->bUseSpriteAnim)
					{
						m_pSpriteDesc_Particle = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_Sprite_Desc();

						// ��������Ʈ�� ������ Ű�� ����_��ƼŬ
						ImGui::SeparatorText("Loop Sprite_Particle");
						ImGui::RadioButton("Loop Sprite_Particle", &m_iSpriteLoop, 0);
						ImGui::RadioButton("None Loop Sprite_Particle", &m_iSpriteLoop, 1);
						if (0 == m_iSpriteLoop)
						{
							m_pSpriteDesc_Particle->bLoop = TRUE;
							//m_pSpriteDesc_Particle->Reset_Sprite();
						}
						else if (1 == m_iSpriteLoop)
						{
							m_pSpriteDesc_Particle->bLoop = FALSE;
							//m_pSpriteDesc_Particle->Reset_Sprite();
						}


						/* ��������Ʈ ��� �ӵ�_��ƼŬ */
						ImGui::SeparatorText("Sprite Term");
						if (ImGui::DragFloat("Sprite Term_Particle", &m_fSequenceTerm_Particle, 1, 1))
						{
							m_pSpriteDesc_Particle->fSequenceTerm = m_fSequenceTerm_Particle;
							//m_pSpriteDesc_Particle->Reset_Sprite();
						}


						/* ��������Ʈ �ִ� ���� ���� ����_��ƼŬ */
						if (CEffect_Void::TEXTURE_END != m_eType_Sprite_Particle)
						{
							ImGui::SeparatorText("Max_TileCoun");
							if (ImGui::InputInt2("Max_TileCount_Particle", m_vUV_MaxTileCount_Particle, 1))
							{
								_uint iX, iY;
								dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_TextureCom(m_eType_Sprite_Particle)->Get_TextureSize(&iX, &iY, m_iTexIndex_Particle[m_eType_Sprite_Particle]);
								m_pSpriteDesc_Particle->vTextureSize.x = (_float)iX;
								m_pSpriteDesc_Particle->vTextureSize.y = (_float)iY;

								_float fTileX, fTileY;
								fTileX = (_float)iX / m_vUV_MaxTileCount_Particle[0];
								fTileY = (_float)iY / m_vUV_MaxTileCount_Particle[1];

								m_pSpriteDesc_Particle->vTileSize.x = fTileX;
								m_pSpriteDesc_Particle->vTileSize.y = fTileY;

								m_pSpriteDesc_Particle->vUV_MaxTileCount.x = (_float)m_vUV_MaxTileCount_Particle[0];
								m_pSpriteDesc_Particle->vUV_MaxTileCount.y = (_float)m_vUV_MaxTileCount_Particle[1];

								m_pSpriteDesc_Particle->Reset_Sprite();
								m_pCurPartEffect->ReSet_Effect();
							}

							ImGui::Text("Current Index : %d, %d", m_pSpriteDesc_Particle->vUV_CurTileIndex.x, m_pSpriteDesc_Particle->vUV_CurTileIndex.y);
						}
					}
					else
					{
						m_eType_Sprite_Particle = CEffect_Void::TEXTURE_END;
					}

					ImGui::SeparatorText("");
				}
#pragma endregion


				if (ImGui::CollapsingHeader(" Shader & RenderGroup _Particle"))
				{
					/* ���̴� �н� �ε��� ����_��ƼŬ */
					if (ImGui::InputInt("Shader Pass_Particle", &m_iShaderPassIndex_Particle, 1))
					{
						if (m_iMaxShaderPassIndex_Particle < m_iShaderPassIndex_Particle)
							m_iShaderPassIndex_Particle = m_iMaxShaderPassIndex_Particle;

						if (0 > m_iShaderPassIndex_Particle)
							m_iShaderPassIndex_Particle = 0;

						m_pCurVoidDesc->iShaderPassIndex = m_iShaderPassIndex_Particle;
					}


					/* ���̴��� ���� ��ī�� �� ����_��ƼŬ */
					ImGui::SeparatorText("Discard_Value");
					if (ImGui::DragFloat4("Discard_Particle", m_vColor_Clip_Part, 0.1f, 0.f, 1.f))
					{
						m_pCurVoidDesc->vColor_Clip.x = m_vColor_Clip_Part[0];
						m_pCurVoidDesc->vColor_Clip.y = m_vColor_Clip_Part[1];
						m_pCurVoidDesc->vColor_Clip.z = m_vColor_Clip_Part[2];
						m_pCurVoidDesc->vColor_Clip.w = m_vColor_Clip_Part[3];
					}


					/* �����׷� ����_��ƼŬ(����� Effect�׷����� ����(9��) */
					//ImGui::SeparatorText(" Render Group_Particle ");
					//if (ImGui::InputInt(" Render Group_Particle ", &m_iRenderGroup_Particle, 1))
					//{
					//	if ((_int)CRenderer::RENDER_END < m_iRenderGroup_Particle)
					//	{
					//		m_iRenderGroup_Particle = (_int)CRenderer::RENDER_END - 1;
					//	}
					//	m_pCurVoidDesc->iRenderGroup = m_iRenderGroup_Particle;
					//}

					ImGui::SeparatorText("");
				}



				///* Z��Ʈ Ű�� ���� */
				//ImGui::SeparatorText("SortZ");
				//ImGui::RadioButton("SortZ", &m_iSortZ, 0);  ImGui::SameLine();
				//ImGui::RadioButton("Off SortZ", &m_iSortZ, 1);
				//if (0 == m_iSortZ)
				//	dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Set_SortZ(TRUE);
				//else if (1 == m_iSortZ)
				//	dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Set_SortZ(FALSE);


				/* �ν��Ͻ� ���� ���� */
				ImGui::SeparatorText(" Instance Num ");
				ImGui::Text("MaxInstance : %d", m_iMaxNumInstance_Particle);
				if (ImGui::DragInt("Instance Num_Particle", &m_iNumInstance_Particle, 1, 1, m_iNumInstance_Particle))
				{
					// 1���� �۾��� �� ����
					if (1 > m_iNumInstance_Particle)
						m_iNumInstance_Particle = 1;

					// �ƽ� ������ �Ѿ �� ����
					if (m_iMaxNumInstance_Particle < m_iNumInstance_Particle)
						m_iNumInstance_Particle = m_iMaxNumInstance_Particle;

					m_pCurVoidDesc->iCurNumInstance = m_iNumInstance_Particle;
					m_pParticleBufferDesc->iCurNumInstance = m_iNumInstance_Particle;
				}


				/* ����(Emitter) */
				ImGui::SeparatorText(u8" ���� ����(Emitter)_Particle "); ImGui::SameLine(); HelpMarker(u8"���� �ð� ��, �ѹ� ������ �� ����Ǵ� ���� ��(0�� �� 1���� ����)");
				if (ImGui::DragFloat("EmissionTime_Particle", &m_fEmissionTime_Particle, 0.01f, 0.f, 1000.f))
				{
					if (0.f >= m_fEmissionTime_Particle)	// 0.f���� �۾����� 0����
						m_fEmissionTime_Particle = 0.f;

					m_pParticleBufferDesc->fEmissionTime = m_fEmissionTime_Particle;
				}

				if (ImGui::DragInt("AddEmitCount_Particle", &m_iAddEmitCount_Particle, 1, 0, m_pParticleBufferDesc->iCurNumInstance))
				{
					// ���� �� ����
					m_pCurEffectDesc->bPlay = FALSE;
					m_pCurEffect->Init_ReSet_Effect();

					if (0 >= m_iAddEmitCount_Particle)	// 0���� �۾����� 0����
						m_iAddEmitCount_Particle = 0;

					if (m_pParticleBufferDesc->iCurNumInstance <= m_iAddEmitCount_Particle)	// ���� �ν��Ͻ� ������ ���ų� Ŀ���� ���� �ν��Ͻ� ������
						m_iAddEmitCount_Particle = m_pParticleBufferDesc->iCurNumInstance;

					m_pParticleBufferDesc->iAddEmitCount = (_uint)m_iAddEmitCount_Particle;
				}



				/* ������ġ ������(000 ���� ������� ������ �������� ���� �ɰ���) */
				ImGui::SeparatorText("Center Offset_Particle");	ImGui::SameLine(); HelpMarker(u8"����0,0,0 ���� ������� ������ �������� �����ɰ���");
				/* MinCenterOffsetPos */
				if (ImGui::DragFloat3("MinCenterOffsetPos_Mesh", m_vMinCenterOffsetPos_Particle, 0.1f, -500.f, 500.f))
				{
					m_pParticleBufferDesc->vMinCenterOffsetPos.x = m_vMinCenterOffsetPos_Particle[0];
					m_pParticleBufferDesc->vMinCenterOffsetPos.y = m_vMinCenterOffsetPos_Particle[1];
					m_pParticleBufferDesc->vMinCenterOffsetPos.z = m_vMinCenterOffsetPos_Particle[2];
				}
				/* MaxCenterOffsetPos */
				if (ImGui::DragFloat3("MaxCenterOffsetPos_Mesh", m_vMaxCenterOffsetPos_Particle, 0.1f, -500.f, 500.f))
				{
					m_pParticleBufferDesc->vMaxCenterOffsetPos.x = m_vMaxCenterOffsetPos_Particle[0];
					m_pParticleBufferDesc->vMaxCenterOffsetPos.y = m_vMaxCenterOffsetPos_Particle[1];
					m_pParticleBufferDesc->vMaxCenterOffsetPos.z = m_vMaxCenterOffsetPos_Particle[2];
				}


				/* ������ ����(���� ����) */
				ImGui::SeparatorText(" Range_Particle "); ImGui::SameLine(); HelpMarker(u8"������ ����(���� ����)");
				if (ImGui::DragFloat2("MinMaxRange_Particle", m_vMinMaxRange_Particle, 1.f, 0.1f, 360.f))
				{
					if (m_vMinMaxRange_Particle[0] > m_vMinMaxRange_Particle[1])	// Min�� Max���� ũ�� Max�� Min����
						m_vMinMaxRange_Particle[1] = m_vMinMaxRange_Particle[0];


					m_pParticleBufferDesc->vMinMaxRange.x = m_vMinMaxRange_Particle[0];
					m_pParticleBufferDesc->vMinMaxRange.y = m_vMinMaxRange_Particle[1];
				}

				/* �߰� ���� ����(������)*/
				ImGui::SeparatorText(" Add Range_Particle "); ImGui::SameLine(); HelpMarker(u8"�߰� ���� ����");
				if (ImGui::DragFloat2("MinMax Add Range_Particle", m_fMinMaxAddRange_Particle, 0.1f, -500.f, 500.f))
				{
					m_pParticleBufferDesc->vMinMaxAddRange.x = m_fMinMaxAddRange_Particle[0];
					m_pParticleBufferDesc->vMinMaxAddRange.y = m_fMinMaxAddRange_Particle[1];

				}


				ImGui::SeparatorText(" Theta "); ImGui::SameLine();	HelpMarker(u8"Degree�� �Է�(Radian �ڵ���ȯ)");
				if (ImGui::DragFloat2("vMinMaxTheta_Particle", m_vMinMaxTheta_Particle, 0.f, 1.f, 360.f))
				{
					m_pParticleBufferDesc->vMinMaxTheta.x = m_vMinMaxTheta_Particle[0];
					m_pParticleBufferDesc->vMinMaxTheta.y = m_vMinMaxTheta_Particle[1];
				}


				if (CVIBuffer_Particle::FALL == m_pParticleBufferDesc->eType_Action
					|| CVIBuffer_Particle::RISE == m_pParticleBufferDesc->eType_Action
					|| CVIBuffer_Particle::TORNADO == m_pParticleBufferDesc->eType_Action
					)
				{

					ImGui::SeparatorText(" Y_Max Pos ");
					if (ImGui::DragFloat2("vMinMaxPosY_Particle", m_vMinMaxPosY_Particle, 0.f, 1.f, 1000.f))
					{
						m_pParticleBufferDesc->vMinMaxPosY.x = m_vMinMaxPosY_Particle[0];
						m_pParticleBufferDesc->vMinMaxPosY.y = m_vMinMaxPosY_Particle[1];
					}
				}



				/* ���ǵ� ����(���� ����) */
				ImGui::SeparatorText(" Speed ");

				if (ImGui::DragFloat2("vMinMaxSpeed_Particle", m_vMinMaxSpeed_Particle, 0.1f, -1000.f, 1000.f))
				{
					if (m_pParticleBufferDesc->bUseRigidBody)
					{
						if (0.f > m_vMinMaxSpeed_Particle[0])	// Min�� 0���� �۾����� 0���� ����
							m_vMinMaxSpeed_Particle[0] = 0.f;

						if (m_vMinMaxSpeed_Particle[0] > m_vMinMaxSpeed_Particle[1])	// Min�� Max���� ũ�� Max�� Min����
							m_vMinMaxSpeed_Particle[1] = m_vMinMaxSpeed_Particle[0];
					}


					m_pParticleBufferDesc->vMinMaxSpeed.x = m_vMinMaxSpeed_Particle[0];
					m_pParticleBufferDesc->vMinMaxSpeed.y = m_vMinMaxSpeed_Particle[1];
				}


				/* ����̵� ���ǵ� */
				if (CVIBuffer_Particle::TORNADO == m_pParticleBufferDesc->eType_Action)
				{
					ImGui::SeparatorText(" Tornado Speed_Particle ");
					if (ImGui::DragFloat2("vMinMaxTornadoSpeed_Particle", m_vMinMaxTornadoSpeed_Particle, 0.1f, 0.f, 5000.f))
					{
						m_pParticleBufferDesc->vMinMaxTornadoSpeed.x = m_vMinMaxTornadoSpeed_Particle[0];
						m_pParticleBufferDesc->vMinMaxTornadoSpeed.y = m_vMinMaxTornadoSpeed_Particle[1];
					}
				}


				if (ImGui::TreeNode(" Speed_Easing Types "))
				{
					ImGui::Text("Selected Type : %d", m_pParticleBufferDesc->eType_SpeedLerp);

					Select_EasingType(&m_pParticleBufferDesc->eType_SpeedLerp);

					ImGui::TreePop();
				}



				/* ��ƼŬ ������Ŭ */
				ImGui::SeparatorText(" Recycle_Particle ");
				ImGui::RadioButton("Recycle_Particle", &m_iRecycle_Particle, 0);  ImGui::SameLine();
				ImGui::RadioButton("None Recycle_Particle", &m_iRecycle_Particle, 1);
				if (0 == m_iRecycle_Particle)
					m_pParticleBufferDesc->bRecycle = TRUE;
				else if (1 == m_iRecycle_Particle)
					m_pParticleBufferDesc->bRecycle = FALSE;


				/* ��ƼŬ ������� ������ Reverse */
				ImGui::SeparatorText(" Reverse_Particle ");
				ImGui::RadioButton("Reverse_Particle", &m_iReverse_Particle, 0);  ImGui::SameLine();
				ImGui::RadioButton("None Reverse_Particle", &m_iReverse_Particle, 1);
				if (0 == m_iReverse_Particle)
					m_pParticleBufferDesc->bReverse = TRUE;
				else if (1 == m_iReverse_Particle)
					m_pParticleBufferDesc->bReverse = FALSE;


				/* ��ƼŬ �׼� Ÿ�� */
				ImGui::SeparatorText(" Action Type_Particle ");
				if (ImGui::RadioButton("SPARK_Particle ", &m_iType_Action_Particle, 0))
					pVIBuffer->ReSet();
				if (ImGui::RadioButton("BLINK_Particle", &m_iType_Action_Particle, 1))
					pVIBuffer->ReSet();
				if (ImGui::RadioButton("FALL_Particle", &m_iType_Action_Particle, 2))
					pVIBuffer->ReSet();
				if (ImGui::RadioButton("RISE_Particle", &m_iType_Action_Particle, 3))
					pVIBuffer->ReSet();
				if (ImGui::RadioButton("TORNADO_Particle", &m_iType_Action_Particle, 4))
					pVIBuffer->ReSet();
				ImGui::RadioButton("TYPE_ACTION_END_Particle", &m_iType_Action_Particle, 5);


				if (0 == m_iType_Action_Particle)
					m_pParticleBufferDesc->eType_Action = CVIBuffer_Particle::SPARK;
				else if (1 == m_iType_Action_Particle)
					m_pParticleBufferDesc->eType_Action = CVIBuffer_Particle::BLINK;
				else if (2 == m_iType_Action_Particle)
					m_pParticleBufferDesc->eType_Action = CVIBuffer_Particle::FALL;
				else if (3 == m_iType_Action_Particle)
					m_pParticleBufferDesc->eType_Action = CVIBuffer_Particle::RISE;
				else if (4 == m_iType_Action_Particle)
					m_pParticleBufferDesc->eType_Action = CVIBuffer_Particle::TORNADO;
				else if (5 == m_iType_Action_Particle)
					m_pParticleBufferDesc->eType_Action = CVIBuffer_Particle::TYPE_ACTION_END;


#pragma region ������ٵ� �ɼ� ����_��ƼŬ
				ImGui::SeparatorText("");
				if (ImGui::CollapsingHeader(" RigidBody_Particle "))
				{

					/* ������ٵ� Ű�� ���� */
					ImGui::SeparatorText("RigidBody");
					ImGui::RadioButton(" ON  RigidBody_Particle ", &m_iUseRigidBody_Particle, 0);
					ImGui::RadioButton(" OFF RigidBody_Particle ", &m_iUseRigidBody_Particle, 1);
					if (0 == m_iUseRigidBody_Particle)
					{
						m_pCurVoidDesc->bUseRigidBody = TRUE;
						m_pParticleBufferDesc->bUseRigidBody = TRUE;

						m_pParticleBufferDesc->eType_Action = CVIBuffer_Particle::SPARK;
					}
					else if (1 == m_iUseRigidBody_Particle)
					{
						m_pCurVoidDesc->bUseRigidBody = FALSE;
						m_pParticleBufferDesc->bUseRigidBody = FALSE;
					}


					if (0 == m_iUseRigidBody_Particle)	// ������ٵ� ����̸� ����
					{
						/* Ű��ƽ Ű�� ���� */
						ImGui::SeparatorText(" Kinetic_Particle ");
						ImGui::RadioButton(" Kinetic_Particle ", &m_iKinetic_Particle, 0);  ImGui::SameLine();
						ImGui::RadioButton(" Kinematic_Particle ", &m_iKinetic_Particle, 1);
						if (0 == m_iKinetic_Particle)
						{
							m_pParticleBufferDesc->bKinetic = TRUE;
						}
						else if (1 == m_iKinetic_Particle)
						{
							m_pParticleBufferDesc->bKinetic = FALSE;
						}

						/* �߷� Ű�� ���� */
						ImGui::RadioButton(" ON   Gravity_Particle ", &m_iUseGravity_Particle, 0);  ImGui::SameLine();
						ImGui::RadioButton(" OFF  Gravity_Particle ", &m_iUseGravity_Particle, 1);
						if (0 == m_iUseGravity_Particle)
						{
							m_pParticleBufferDesc->bUseGravity = TRUE;
						}
						else if (1 == m_iUseGravity_Particle)
						{
							m_pParticleBufferDesc->bUseGravity = FALSE;
						}

						/* �߷� ���ӵ� ���� */
						if (0 == m_iUseGravity_Particle)
						{
							if (ImGui::DragFloat("Gravity_Particle", &m_fGravity_Particle, 0.5f, -100.f, 1000.f))
							{
								m_pParticleBufferDesc->fGravity = m_fGravity_Particle;
							}
						}

						///* ������ٵ� : Force Mode */
						//ImGui::SeparatorText("Force Mode");
						//if (ImGui::Button(" FORCE "))
						//{
						//	m_pParticleBufferDesc->eForce_Mode = FORCE_MODE::FORCE;
						//}
						//ImGui::SameLine();
						//if (ImGui::Button(" IMPULSE "))
						//{
						//	m_pParticleBufferDesc->eForce_Mode = FORCE_MODE::IMPULSE;
						//}
						//ImGui::SameLine();
						//if (ImGui::Button(" ACCELERATION "))
						//{
						//	m_pParticleBufferDesc->eForce_Mode = FORCE_MODE::ACCELERATION;
						//}
						//ImGui::SameLine();
						//if (ImGui::Button(" VELOCITY_CHANGE "))
						//{
						//	m_pParticleBufferDesc->eForce_Mode = FORCE_MODE::VELOCITY_CHANGE;
						//}

						/* �Ŀ� ���� */
						ImGui::SeparatorText("Power");
						if (ImGui::DragFloat2("MinMaxPower_Particle", m_vMinMaxPower_Particle, 10.f, 0.1f, 5000.f))
						{
							if (m_vMinMaxPower_Particle[0] > m_vMinMaxPower_Particle[1])	// Min�� Max���� ũ�� Max�� Min����
								m_vMinMaxPower_Particle[1] = m_vMinMaxPower_Particle[0];

							m_pParticleBufferDesc->vMinMaxPower.x = m_vMinMaxPower_Particle[0];
							m_pParticleBufferDesc->vMinMaxPower.y = m_vMinMaxPower_Particle[1];
						}

						/* Mass(����) ���� */
						ImGui::SeparatorText(u8"����(Mass)");
						if (ImGui::DragFloat2("vMinMaxMass_Particle", m_vMinMaxMass_Particle, 1.f, 0.f, 1000.f))
						{
							if (m_vMinMaxMass_Particle[0] > m_vMinMaxMass_Particle[1])	// Min�� Max���� ũ�� Max�� Min����
								m_vMinMaxMass_Particle[1] = m_vMinMaxMass_Particle[0];


							m_pParticleBufferDesc->vMinMaxMass.x = m_vMinMaxMass_Particle[0];
							m_pParticleBufferDesc->vMinMaxMass.y = m_vMinMaxMass_Particle[1];
						}


						/* ������� ���� */
						//ImGui::SeparatorText("Friction");
						//if (ImGui::DragFloat("Friction_Particle", &m_fFriction_Particle, 0.1f, 0.1f, 100.f))
						//{
						//	m_pParticleBufferDesc->fFriction = m_fFriction_Particle;
						//}

						ImGui::SeparatorText(u8"����(Friction) Lerp_Particle");
						if (ImGui::DragFloat2("Start End Friction_Particle", m_vStartEnd_Friction_Particle, 0.1f, 0.f, 1.f))
						{
							m_pParticleBufferDesc->vStartEnd_Friction.x = m_vStartEnd_Friction_Particle[0];
							m_pParticleBufferDesc->vStartEnd_Friction.y = m_vStartEnd_Friction_Particle[1];
						}

						if (ImGui::DragFloat2("FrictionLerp_Pos_Particle", m_vFrictionLerp_Pos_Particle, 0.1f, 0.f, 1.f))
						{
							if (m_vFrictionLerp_Pos_Particle[0] > m_vFrictionLerp_Pos_Particle[1])	// Min�� Max���� ũ�� Max�� Min����
								m_vFrictionLerp_Pos_Particle[1] = m_vFrictionLerp_Pos_Particle[0];

							m_pParticleBufferDesc->vFrictionLerp_Pos.x = m_vFrictionLerp_Pos_Particle[0];
							m_pParticleBufferDesc->vFrictionLerp_Pos.y = m_vFrictionLerp_Pos_Particle[1];
						}

						if (ImGui::TreeNode(" Friction_Easing Types "))
						{
							ImGui::Text("Selected Type : %d", m_pParticleBufferDesc->eType_FrictionLerp);
							Select_EasingType(&m_pParticleBufferDesc->eType_FrictionLerp);

							ImGui::TreePop();
						}


						/* ���� �Ѱ��� ����(1���Ͽ��� ��) */
						ImGui::SeparatorText(u8"���� �Ѱ���(SleepThreshold)");
						if (ImGui::DragFloat("SleepThreshold_Particle", &m_fSleepThreshold_Particle, 0.01f, 0.f, 1.f))
						{
							m_pParticleBufferDesc->fSleepThreshold = m_fSleepThreshold_Particle;
						}
					}

					ImGui::SeparatorText("");

				}
#pragma endregion ������ٵ� �ɼ� ����_��ƼŬ ��


#pragma region ũ�� ���� ����_��ƼŬ ����
				if (ImGui::CollapsingHeader(" Scale_Particle "))
				{
					if (1 == m_iUseScaleLerp_Particle)
					{
						// ũ�ⷯ�� �Ⱦ� ���� ������ �������� ����_�޽� ��ƼŬ
						ImGui::SeparatorText(" ScaleRatio_Particle ");
						ImGui::RadioButton(u8"ũ�� ���� ����_Particle", &m_iScaleRatio_Particle, 0);
						ImGui::RadioButton(u8"ũ�� ���� ����_Particle", &m_iScaleRatio_Particle, 1);
						if (0 == m_iScaleRatio_Particle)
							m_pParticleBufferDesc->bScaleRatio = TRUE;
						else if (1 == m_iScaleRatio_Particle)
							m_pParticleBufferDesc->bScaleRatio = FALSE;
					}

					ImGui::SeparatorText("MinMax Width");
					if (ImGui::DragFloat2("MinMax Width_Particle", m_vMinMaxWidth_Particle, 0.1f, 0.f, 5000.f))
					{
						if (m_vMinMaxWidth_Particle[0] > m_vMinMaxWidth_Particle[1])	// Min�� Max���� ũ�� Max�� Min����
							m_vMinMaxWidth_Particle[1] = m_vMinMaxWidth_Particle[0];

						m_pParticleBufferDesc->vMinMaxWidth.x = m_vMinMaxWidth_Particle[0];
						m_pParticleBufferDesc->vMinMaxWidth.y = m_vMinMaxWidth_Particle[1];
					}

					ImGui::SeparatorText("MinMax Height");
					if (ImGui::DragFloat2("MinMax Height_Particle", m_vMinMaxHeight_Particle, 0.1f, 0.f, 5000.f))
					{
						if (m_vMinMaxHeight_Particle[0] > m_vMinMaxHeight_Particle[1])	// Min�� Max���� ũ�� Max�� Min����
							m_vMinMaxHeight_Particle[1] = m_vMinMaxHeight_Particle[0];

						m_pParticleBufferDesc->vMinMaxHeight.x = m_vMinMaxHeight_Particle[0];
						m_pParticleBufferDesc->vMinMaxHeight.y = m_vMinMaxHeight_Particle[1];
					}


					/* ��ƼŬ ũ�� ���� ���� ��� */
					ImGui::SeparatorText(" Scale Mode_Particle ");
					ImGui::RadioButton(u8"ũ�� ���� ���_Particle", &m_iUseScaleLerp_Particle, 0);
					ImGui::RadioButton(u8"ũ�� ���� ���_Particle", &m_iUseScaleLerp_Particle, 1);
					if (0 == m_iUseScaleLerp_Particle)
						m_pParticleBufferDesc->bUseScaleLerp = TRUE;
					else if (1 == m_iUseScaleLerp_Particle)
						m_pParticleBufferDesc->bUseScaleLerp = FALSE;

					if (TRUE == m_pParticleBufferDesc->bUseScaleLerp)
					{
						ImGui::SeparatorText("ScaleLerp Pos");	 ImGui::SameLine(); HelpMarker(u8"0~1�� ������ ������Ÿ�� ���� ũ�Ⱑ ��ȭ�� ���� ����, �� ����");
						if (ImGui::DragFloat2("Up ScaleLerp Pos_Particle", m_vScaleLerp_Up_Pos, 0.1f, 0.f, 1.f))
						{
							if (m_vScaleLerp_Up_Pos[0] > m_vScaleLerp_Up_Pos[1])	// Min�� Max���� ũ�� Max�� Min����
								m_vScaleLerp_Up_Pos[1] = m_vScaleLerp_Up_Pos[0];

							m_pParticleBufferDesc->vScaleLerp_Up_Pos.x = m_vScaleLerp_Up_Pos[0];
							m_pParticleBufferDesc->vScaleLerp_Up_Pos.y = m_vScaleLerp_Up_Pos[1];
						}

						if (ImGui::DragFloat2("Down ScaleLerp Pos_Particle", m_vScaleLerp_Down_Pos, 0.1f, 0.f, 1.f))
						{
							if (m_vScaleLerp_Down_Pos[0] > m_vScaleLerp_Down_Pos[1])	// Min�� Max���� ũ�� Max�� Min����
								m_vScaleLerp_Down_Pos[1] = m_vScaleLerp_Down_Pos[0];

							m_pParticleBufferDesc->vScaleLerp_Down_Pos.x = m_vScaleLerp_Down_Pos[0];
							m_pParticleBufferDesc->vScaleLerp_Down_Pos.y = 1.f; // 1�� ����
						}

						if (ImGui::TreeNode(" Scale_Easing Types "))
						{
							ImGui::Text("Selected Type : %d", m_pParticleBufferDesc->eType_ScaleLerp);

							Select_EasingType(&m_pParticleBufferDesc->eType_ScaleLerp);

							ImGui::TreePop();
						}
					}

					ImGui::SeparatorText("");
				}
#pragma endregion ũ�� ���� ����_��ƼŬ ��


#pragma region ȸ��_��ƼŬ ����
				if (ImGui::CollapsingHeader(" Rotation_Particle "))
				{

					ImGui::SeparatorText(" Dir To Axis_Particle ");	ImGui::SameLine(); HelpMarker(u8"���⺤�Ͱ� �ش� ���̵˴ϴ�.");
					ImGui::RadioButton("DIR_RIGHT_Particle", &m_iType_Dir_Particle, 0);
					ImGui::RadioButton("DIR_UP_Particle ", &m_iType_Dir_Particle, 1);
					ImGui::RadioButton("DIR_LOOK_Particle", &m_iType_Dir_Particle, 2);

					if (0 == m_iType_Dir_Particle)
						m_pParticleBufferDesc->eType_Dir = CVIBuffer_Particle::DIR_RIGHT;
					else if (1 == m_iType_Dir_Particle)
						m_pParticleBufferDesc->eType_Dir = CVIBuffer_Particle::DIR_UP;
					else if (2 == m_iType_Dir_Particle)
						m_pParticleBufferDesc->eType_Dir = CVIBuffer_Particle::DIR_LOOK;


					/* ȸ�� ����(������) */
					/* RotX (Arc_X) */
					ImGui::SeparatorText(u8"���⺤�� ȸ������");
					if (ImGui::DragFloat2("Arc_X_Particle", m_vRotationOffsetX_Particle, 1.f, 0.f, 360.f))
					{
						if (m_vRotationOffsetX_Particle[0] > m_vRotationOffsetX_Particle[1])	// Min�� Max���� ũ�� Max�� Min����
							m_vRotationOffsetX_Particle[1] = m_vRotationOffsetX_Particle[0];


						m_pParticleBufferDesc->vMinMaxRotationOffsetX.x = m_vRotationOffsetX_Particle[0];
						m_pParticleBufferDesc->vMinMaxRotationOffsetX.y = m_vRotationOffsetX_Particle[1];

					}

					/* RotY (Arc_Y) */
					if (ImGui::DragFloat2("Arc_Y_Particle", m_vRotationOffsetY_Particle, 1.f, 0.f, 360.f))
					{
						if (m_vRotationOffsetY_Particle[0] > m_vRotationOffsetY_Particle[1])	// Min�� Max���� ũ�� Max�� Min����
							m_vRotationOffsetY_Particle[1] = m_vRotationOffsetY_Particle[0];


						m_pParticleBufferDesc->vMinMaxRotationOffsetY.x = m_vRotationOffsetY_Particle[0];
						m_pParticleBufferDesc->vMinMaxRotationOffsetY.y = m_vRotationOffsetY_Particle[1];
					}

					/* RotZ (Arc_Z) */
					if (ImGui::DragFloat2("Arc_Z_Particle", m_vRotationOffsetZ_Particle, 1.f, 0.f, 360.f))
					{
						if (m_vRotationOffsetZ_Particle[0] > m_vRotationOffsetZ_Particle[1])	// Min�� Max���� ũ�� Max�� Min����
							m_vRotationOffsetZ_Particle[1] = m_vRotationOffsetZ_Particle[0];

						m_pParticleBufferDesc->vMinMaxRotationOffsetZ.x = m_vRotationOffsetZ_Particle[0];
						m_pParticleBufferDesc->vMinMaxRotationOffsetZ.y = m_vRotationOffsetZ_Particle[1];
					}


					ImGui::SeparatorText(u8"�ν��Ͻ� ȸ��");
					if (ImGui::DragFloat(" Radian_X_Particle ", &m_vRadian_Particle[0], 1.f, 0.f, 360.f))
						m_pParticleBufferDesc->vRadian.x = m_vRadian_Particle[0];

					if (ImGui::DragFloat(" Radian_Y_Particle ", &m_vRadian_Particle[1], 1.f, 0.f, 360.f))
						m_pParticleBufferDesc->vRadian.y = m_vRadian_Particle[1];

					if (ImGui::DragFloat(" Radian_Z_Particle ", &m_vRadian_Particle[2], 1.f, 0.f, 360.f))
						m_pParticleBufferDesc->vRadian.z = m_vRadian_Particle[2];


					ImGui::SeparatorText("");
				}

#pragma endregion ȸ��_��ƼŬ ��



#pragma region �� ����_��ƼŬ

				if (ImGui::CollapsingHeader(" Color_Particle "))
				{

					/* ��ƼŬ ���̵� �ξƿ� ��� */
					ImGui::SeparatorText(" Fade Type ");
					ImGui::RadioButton("FADE_NONE_Particle", &m_iType_Fade_Particle, 0);
					ImGui::RadioButton("FADE_OUT_Particle", &m_iType_Fade_Particle, 1);
					ImGui::RadioButton("FADE_IN_Particle", &m_iType_Fade_Particle, 2);
					if (0 == m_iType_Fade_Particle)
						m_pParticleBufferDesc->eType_Fade = CVIBuffer_Particle::TYPE_FADE::FADE_NONE;
					else if (1 == m_iType_Fade_Particle)
						m_pParticleBufferDesc->eType_Fade = CVIBuffer_Particle::TYPE_FADE::FADE_OUT;
					else if (2 == m_iType_Fade_Particle)
						m_pParticleBufferDesc->eType_Fade = CVIBuffer_Particle::TYPE_FADE::FADE_IN;


					/* ��ƼŬ ���̵� �ξƿ� ���� */
					ImGui::SeparatorText(u8" Fade ���� ");
					ImGui::RadioButton(u8"������Ÿ��_Particle", &m_iType_Fade_Takes_Particle, 0);
					ImGui::RadioButton(u8"�Ÿ�_Particle", &m_iType_Fade_Takes_Particle, 1);
					ImGui::RadioButton(u8"����_Particle", &m_iType_Fade_Takes_Particle, 2);
					ImGui::RadioButton(u8"ũ��_Particle", &m_iType_Fade_Takes_Particle, 3);
					ImGui::RadioButton(u8"����_Particle", &m_iType_Fade_Takes_Particle, 4); ImGui::SameLine(); HelpMarker(u8"���� ����Ʈ ������ ���� �����ѹ�");
					ImGui::RadioButton(u8"TYPE_FADE_TAKES_END_Particle", &m_iType_Fade_Takes_Particle, 5);
					if (0 == m_iType_Fade_Takes_Particle)
						m_pParticleBufferDesc->eType_Fade_Takes = CVIBuffer_Particle::TYPE_FADE_TAKES::LIFE;
					else if (1 == m_iType_Fade_Takes_Particle)
						m_pParticleBufferDesc->eType_Fade_Takes = CVIBuffer_Particle::TYPE_FADE_TAKES::DIST;
					else if (2 == m_iType_Fade_Takes_Particle)
						m_pParticleBufferDesc->eType_Fade_Takes = CVIBuffer_Particle::TYPE_FADE_TAKES::HEIGHT;
					else if (3 == m_iType_Fade_Takes_Particle)
						m_pParticleBufferDesc->eType_Fade_Takes = CVIBuffer_Particle::TYPE_FADE_TAKES::SCALE;
					else if (4 == m_iType_Fade_Takes_Particle)
						m_pParticleBufferDesc->eType_Fade_Takes = CVIBuffer_Particle::TYPE_FADE_TAKES::MAGIC;
					else if (5 == m_iType_Fade_Takes_Particle)
						m_pParticleBufferDesc->eType_Fade_Takes = CVIBuffer_Particle::TYPE_FADE_TAKES::TYPE_FADE_TAKES_END;



					/* ��ǻ�� ����ȥ�� ���_Particle */
					ImGui::SeparatorText("Color Blend Mode_Particle");
					ImGui::RadioButton(u8"���ϱ�_Particle", &m_iColor_Mode_Particle, 0);
					ImGui::RadioButton(u8"��ũ��_Particle", &m_iColor_Mode_Particle, 1);
					ImGui::RadioButton(u8"��������_Particle", &m_iColor_Mode_Particle, 2);
					ImGui::RadioButton(u8"���ϱ�_Particle", &m_iColor_Mode_Particle, 3);
					ImGui::RadioButton(u8"��_Particle", &m_iColor_Mode_Particle, 4);
					ImGui::RadioButton(u8"���� ����Ʈ_Particle", &m_iColor_Mode_Particle, 5);
					ImGui::RadioButton(u8"����Ʈ ����Ʈ_Particle", &m_iColor_Mode_Particle, 6);
					ImGui::RadioButton(u8"�ϵ� ����Ʈ_Particle", &m_iColor_Mode_Particle, 7);
					ImGui::RadioButton(u8"�÷� ����_Particle", &m_iColor_Mode_Particle, 8);
					ImGui::RadioButton(u8"ȥ�� ��_Particle", &m_iColor_Mode_Particle, 9);
					ImGui::RadioButton(u8"ȥ�վ���_Particle", &m_iColor_Mode_Particle, 10);


					if (0 == m_iColor_Mode_Particle)
						m_pCurVoidDesc->eMode_Color = MODE_COLOR::MUL;
					else if (1 == m_iColor_Mode_Particle)
						m_pCurVoidDesc->eMode_Color = MODE_COLOR::SCREEN;
					else if (2 == m_iColor_Mode_Particle)
						m_pCurVoidDesc->eMode_Color = MODE_COLOR::OVERLAY;
					else if (3 == m_iColor_Mode_Particle)
						m_pCurVoidDesc->eMode_Color = MODE_COLOR::ADD;
					else if (4 == m_iColor_Mode_Particle)
						m_pCurVoidDesc->eMode_Color = MODE_COLOR::BURN;
					else if (5 == m_iColor_Mode_Particle)
						m_pCurVoidDesc->eMode_Color = MODE_COLOR::VIVID_RIGHT;
					else if (6 == m_iColor_Mode_Particle)
						m_pCurVoidDesc->eMode_Color = MODE_COLOR::SOFT_RIGHT;
					else if (7 == m_iColor_Mode_Particle)
						m_pCurVoidDesc->eMode_Color = MODE_COLOR::HARD_RIGHT;
					else if (8 == m_iColor_Mode_Particle)
						m_pCurVoidDesc->eMode_Color = MODE_COLOR::COLOR_DODGE;
					else if (9 == m_iColor_Mode_Particle)
						m_pCurVoidDesc->eMode_Color = MODE_COLOR::MIX_BURN;
					else if (10 == m_iColor_Mode_Particle)
						m_pCurVoidDesc->eMode_Color = MODE_COLOR::MODE_COLOR_END;


					/* ���ڸ��� �ٸ� �ֱ�� �� ���� Ű�� ���� */
					ImGui::SeparatorText("Color Lerp_Particle");
					ImGui::RadioButton(" ON   Dynamic_Color_Particle ", &m_iDynamic_Color_Particle, 0);  ImGui::SameLine(); HelpMarker(u8"���ڵ� ������ ������Ÿ�ӿ� ���� �÷� ����");
					ImGui::RadioButton(" OFF  Dynamic_Color_Particle ", &m_iDynamic_Color_Particle, 1);	 ImGui::SameLine(); HelpMarker(u8"��ü �ϳ��� ������Ÿ�ӿ� ���� �÷� ����");
					if (0 == m_iDynamic_Color_Particle)
						m_pParticleBufferDesc->bDynamic_Color = TRUE;
					else if (1 == m_iDynamic_Color_Particle)
						m_pParticleBufferDesc->bDynamic_Color = FALSE;
				

					/* Min �� ����_��ƼŬ */
					if (ImGui::ColorEdit4("Min_Color_Particle", m_fColor_Min_Particle, ImGuiColorEditFlags_None))
					{
						m_pParticleBufferDesc->vMinMaxRed.x = m_fColor_Min_Particle[0];
						m_pParticleBufferDesc->vMinMaxGreen.x = m_fColor_Min_Particle[1];
						m_pParticleBufferDesc->vMinMaxBlue.x = m_fColor_Min_Particle[2];
					}

					/* Max �� ����_��ƼŬ */
					if (ImGui::ColorEdit4("Max_Color_Particle", m_fColor_Max_Particle, ImGuiColorEditFlags_None))
					{
						m_pParticleBufferDesc->vMinMaxRed.y = m_fColor_Max_Particle[0];
						m_pParticleBufferDesc->vMinMaxGreen.y = m_fColor_Max_Particle[1];
						m_pParticleBufferDesc->vMinMaxBlue.y = m_fColor_Max_Particle[2];
					}

					if (1 == m_iDynamic_Color_Particle) // ���� �� �ϰ������̸� ���� �� ��ȭ �����ֱ�
					{
						ImGui::ColorEdit4("Cur_Color_Particle", m_fColor_Cur_Particle, ImGuiColorEditFlags_None);
						m_fColor_Cur_Particle[0] = m_pParticleBufferDesc->vCurrentColor.x;
						m_fColor_Cur_Particle[1] = m_pParticleBufferDesc->vCurrentColor.y;
						m_fColor_Cur_Particle[2] = m_pParticleBufferDesc->vCurrentColor.z;
					}


					if (ImGui::TreeNode(" Color_Easing Types "))
					{
						ImGui::Text("Selected Type : %d", m_pParticleBufferDesc->eType_ColorLerp);
						Select_EasingType(&m_pParticleBufferDesc->eType_ColorLerp);

						ImGui::TreePop();
					}


					/* ���� ����ġ */
					ImGui::SeparatorText(u8"���� ����ġ_Particle");
					if (ImGui::DragFloat2("Add Alpha_Particle", m_fMinMaxAlpha_Particle, 0.1f, -255.f, 255.f))
					{
						m_pParticleBufferDesc->vMinMaxAlpha.x = m_fMinMaxAlpha_Particle[0];
						m_pParticleBufferDesc->vMinMaxAlpha.y = m_fMinMaxAlpha_Particle[1];
					}


					ImGui::SeparatorText("");

				}
#pragma endregion �� ����_��ƼŬ ��


				/* ������Ʈ �� ����_��ƼŬ */
				if (ImGui::CollapsingHeader(" Rim Bloom_Particle "))
				{
					if (ImGui::ColorEdit4("RimColor_Particle", m_fRimColor_Particle, ImGuiColorEditFlags_None))
					{
						m_pCurVoidDesc->vRimColor.x = m_fRimColor_Particle[0];
						m_pCurVoidDesc->vRimColor.y = m_fRimColor_Particle[1];
						m_pCurVoidDesc->vRimColor.z = m_fRimColor_Particle[2];
						m_pCurVoidDesc->vRimColor.w = m_fRimColor_Particle[3];
					}

					if (ImGui::ColorEdit3("BloomPower_Particle", m_vBloomPower_Particle, ImGuiColorEditFlags_None))
					{
						m_pCurVoidDesc->vBloomPower.x = m_vBloomPower_Particle[0];
						m_pCurVoidDesc->vBloomPower.y = m_vBloomPower_Particle[1];
						m_pCurVoidDesc->vBloomPower.z = m_vBloomPower_Particle[2];
					}

					ImGui::SeparatorText("");
				}


#pragma region �����_��ƼŬ
				// ����� �� ����_��ƼŬ
				if (ImGui::CollapsingHeader("Distortion_Particle"))
				{
					CEffect_Void::DISTORTION_DESC* pDistortionDesc = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_Distortion_Desc();
					
					if (ImGui::TreeNode(" Distortion Preset "))
					{
						if (ImGui::Button("  ZERO  "))
						{
							pDistortionDesc->eType_Scroll = { CEffect_Void::TYPE_SCROLL_END };

							pDistortionDesc->vScrollSpeeds = { 0.f, 0.f, 0.f };
							pDistortionDesc->vScales = { 0.f, 0.f, 0.f };

							pDistortionDesc->vDistortion1 = { 0.f, 0.f };
							pDistortionDesc->vDistortion2 = { 0.f, 0.f };
							pDistortionDesc->vDistortion3 = { 0.f, 0.f };

							pDistortionDesc->fDistortionScale = { 0.f };
							pDistortionDesc->fDistortionBias = { 0.f };

							Update_CurParameters_Parts();
						}

						if (ImGui::Button("   FIRE   "))
						{
							pDistortionDesc->eType_Scroll = { CEffect_Void::SCROLL_COL };

							pDistortionDesc->vScrollSpeeds = { 1.f, 1.f, 1.f };
							pDistortionDesc->vScales = { 1.f, 1.f, 1.f };

							pDistortionDesc->vDistortion1 = { 0.1f, 0.1f };
							pDistortionDesc->vDistortion2 = { 0.f, 0.f };
							pDistortionDesc->vDistortion3 = { 0.f, 0.1f };

							pDistortionDesc->fDistortionScale = { 1.f };
							pDistortionDesc->fDistortionBias = { 1.f };

							Update_CurParameters_Parts();
						}

						ImGui::TreePop();
					}
		
					/* ����� ��ũ�� ���� ���� */
					ImGui::SeparatorText("Scroll_Distortion_Particle");
					ImGui::RadioButton("Row_Scroll_Particle", &m_iType_Scroll_Particle, 0);  ImGui::SameLine();
					ImGui::RadioButton("Col_Scroll_Particle", &m_iType_Scroll_Particle, 1);
					ImGui::RadioButton("Both_Scroll_Particle", &m_iType_Scroll_Particle, 2);
					ImGui::RadioButton("Rotat_Scroll_Particle", &m_iType_Scroll_Particle, 3);
					ImGui::RadioButton("End_Scroll_Particle", &m_iType_Scroll_Particle, 4);
					if (0 == m_iType_Scroll_Particle)
						pDistortionDesc->eType_Scroll = { CEffect_Void::SCROLL_ROW };
					else if (1 == m_iType_Scroll_Particle)
						pDistortionDesc->eType_Scroll = { CEffect_Void::SCROLL_COL };
					else if (2 == m_iType_Scroll_Particle)
						pDistortionDesc->eType_Scroll = { CEffect_Void::SCROLL_BOTH };
					else if (3 == m_iType_Scroll_Particle)
						pDistortionDesc->eType_Scroll = { CEffect_Void::SCROLL_ROTATE };
					else if (4 == m_iType_Scroll_Particle)
						pDistortionDesc->eType_Scroll = { CEffect_Void::TYPE_SCROLL_END };


					ImGui::SeparatorText("Values_Distortion_Particle");
					if (ImGui::DragFloat3("ScrollSpeeds", m_vScrollSpeeds_Particle, 0.1f, -100.f))
					{
						pDistortionDesc->vScrollSpeeds.x = m_vScrollSpeeds_Particle[0];
						pDistortionDesc->vScrollSpeeds.y = m_vScrollSpeeds_Particle[1];
						pDistortionDesc->vScrollSpeeds.z = m_vScrollSpeeds_Particle[2];
					}
					if (ImGui::DragFloat3("Distortion_Scales", m_vScales_Distortion_Particle, 0.1f, 0.f))
					{
						pDistortionDesc->vScales.x = m_vScales_Distortion_Particle[0];
						pDistortionDesc->vScales.y = m_vScales_Distortion_Particle[1];
						pDistortionDesc->vScales.z = m_vScales_Distortion_Particle[2];
					}
					if (ImGui::DragFloat2("Distortion1", m_vDistortion1_Particle, 0.1f, 0.f))
					{
						pDistortionDesc->vDistortion1.x = m_vDistortion1_Particle[0];
						pDistortionDesc->vDistortion1.y = m_vDistortion1_Particle[1];
					}
					if (ImGui::DragFloat2("Distortion2", m_vDistortion2_Particle, 0.1f, 0.f))
					{
						pDistortionDesc->vDistortion2.x = m_vDistortion2_Particle[0];
						pDistortionDesc->vDistortion2.y = m_vDistortion2_Particle[1];
					}
					if (ImGui::DragFloat2("Distortion3", m_vDistortion3_Particle, 0.1f, 0.f))
					{
						pDistortionDesc->vDistortion3.x = m_vDistortion3_Particle[0];
						pDistortionDesc->vDistortion3.y = m_vDistortion3_Particle[1];
					}
					if (ImGui::DragFloat("Distortion_Scale", &m_fDistortionScale_Particle, 1.f, 0.f))
					{
						pDistortionDesc->fDistortionScale = m_fDistortionScale_Particle;
					}
					if (ImGui::DragFloat("DistortionBias", &m_fDistortionBias_Particle, 1.f, 0.f))
					{
						pDistortionDesc->fDistortionBias = m_fDistortionBias_Particle;
					}

					ImGui::SeparatorText("");
				}
#pragma endregion �����_��ƼŬ ��


			}
		}
	}
}

void CWindow_EffectTool::Update_RectTab()
{
	if (nullptr != m_pCurEffect)
	{
		if (nullptr != m_pCurPartEffect)
		{
			CEffect_Void::TYPE_EFFECT eType_Effect = m_pCurVoidDesc->eType_Effect;

			if (CEffect_Void::RECT == eType_Effect)
			{
				m_pCurVoidDesc = m_pCurPartEffect->Get_Desc();
				m_pRectDesc = dynamic_cast<CEffect_Rect*>(m_pCurPartEffect)->Get_Desc();
				CEffect_Void::DISTORTION_DESC* pDistortionDesc = dynamic_cast<CEffect_Rect*>(m_pCurPartEffect)->Get_Distortion_Desc();

				/* �̸� */
				//ImGui::Text(m_pGameInstance->ConverWStringtoC(m_pCurVoidDesc->strPartTag));
				ImGui::Text(m_szPartNames[m_iCurPartIndex]);

				/* �ؽ�ó ���� */

#pragma region �ؽ�ó ����_��Ʈ
				/* �ؽ�ó ����_��Ʈ */
				ImGui::CollapsingHeader(" Texture_Rect ");
				if (m_pCurVoidDesc->bUseSpriteAnim)
				{
					// ��ǻ��_��������Ʈ_��Ʈ �ؽ�ó ============================================================================================
					ImGui::SeparatorText("Diffuse_Sprte_Rect");

					if (ImGui::Button("Sprite_Base_Rect"))	// ���̽� ��������Ʈ�� ����
					{
						dynamic_cast<CEffect_Rect*>(m_pCurPartEffect)->Change_TextureCom(TEXT("Prototype_Component_Texture_Effect_Diffuse_Sprite"));
						m_iMaxTexIndex_Rect[CEffect_Void::TEXTURE_SPRITE] = 25;
						m_iTexIndex_Rect[CEffect_Void::TEXTURE_SPRITE] = 0;
					}
					ImGui::SameLine();
					if (ImGui::Button("Sprite_Smokes_Rect"))	// ����ũ ��������Ʈ�� ����
					{
						dynamic_cast<CEffect_Rect*>(m_pCurPartEffect)->Change_TextureCom(TEXT("Prototype_Component_Texture_Effect_Diffuse_Sprite_Smokes"));
						m_iMaxTexIndex_Rect[CEffect_Void::TEXTURE_SPRITE] = 34;
						m_iTexIndex_Rect[CEffect_Void::TEXTURE_SPRITE] = 0;
					}

					if (ImGui::InputInt("Sprite_Rect", &m_iTexIndex_Rect[CEffect_Void::TEXTURE_SPRITE], 1))
					{
						if (m_iMaxTexIndex_Rect[CEffect_Void::TEXTURE_SPRITE] <= m_iTexIndex_Rect[CEffect_Void::TEXTURE_SPRITE])
							m_iTexIndex_Rect[CEffect_Void::TEXTURE_SPRITE] = m_iMaxTexIndex_Rect[CEffect_Void::TEXTURE_SPRITE] - 1;

						if (0 > m_iTexIndex_Rect[CEffect_Void::TEXTURE_SPRITE])
							m_iTexIndex_Rect[CEffect_Void::TEXTURE_SPRITE] = 0;

						m_pCurVoidDesc->iTextureIndex[CEffect_Void::TEXTURE_SPRITE] = m_iTexIndex_Rect[CEffect_Void::TEXTURE_SPRITE];
					}


				}
				else
				{
					// ��ǻ��_��Ʈ �ؽ�ó ===============================================================================================
					ImGui::SeparatorText("Diffuse_Rect");

					if (ImGui::Button("Diffuse_Base"))	// ���̽� ��ǻ��� ����
					{
						dynamic_cast<CEffect_Rect*>(m_pCurPartEffect)->Change_TextureCom(TEXT("Prototype_Component_Texture_Effect_Diffuse"));
						m_iMaxTexIndex_Rect[CEffect_Void::TEXTURE_DIFFUSE] = 24;
						m_iTexIndex_Rect[CEffect_Void::TEXTURE_DIFFUSE] = 0;
					}


					if (ImGui::InputInt("Diffuse_Rect", &m_iTexIndex_Rect[CEffect_Void::TEXTURE_DIFFUSE], 1))
					{
						if (m_iMaxTexIndex_Rect[CEffect_Void::TEXTURE_DIFFUSE] <= m_iTexIndex_Rect[CEffect_Void::TEXTURE_DIFFUSE])
							m_iTexIndex_Rect[CEffect_Void::TEXTURE_DIFFUSE] = m_iMaxTexIndex_Rect[CEffect_Void::TEXTURE_DIFFUSE] - 1;

						if (0 > m_iTexIndex_Rect[CEffect_Void::TEXTURE_DIFFUSE])
							m_iTexIndex_Rect[CEffect_Void::TEXTURE_DIFFUSE] = 0;

						m_pCurVoidDesc->iTextureIndex[CEffect_Void::TEXTURE_DIFFUSE] = m_iTexIndex_Rect[CEffect_Void::TEXTURE_DIFFUSE];
					}
				}


				// ����ũ_��Ʈ �ؽ�ó =====================================================================================================
				ImGui::SeparatorText("Mask_Rect");

				if (ImGui::Button("Mask_Base"))	// ���̽� ����ũ�� ����
				{
					dynamic_cast<CEffect_Rect*>(m_pCurPartEffect)->Change_TextureCom(TEXT("Prototype_Component_Texture_Effect_Mask"));
					m_iMaxTexIndex_Rect[CEffect_Void::TEXTURE_MASK] = 170;
					m_iTexIndex_Rect[CEffect_Void::TEXTURE_MASK] = 0;

				}ImGui::SameLine();
				if (ImGui::Button("Mask_Waves"))	// ���̺� ����ũ�� ����
				{
					dynamic_cast<CEffect_Rect*>(m_pCurPartEffect)->Change_TextureCom(TEXT("Prototype_Component_Texture_Effect_Mask_Waves"));
					m_iMaxTexIndex_Rect[CEffect_Void::TEXTURE_MASK] = 6;
					m_iTexIndex_Rect[CEffect_Void::TEXTURE_MASK] = 0;
				}

				if (ImGui::Button(" Remove Mask_Rect "))
				{
					dynamic_cast<CEffect_Rect*>(m_pCurPartEffect)->Remove_TextureCom(CEffect_Void::TEXTURE_MASK);
				}


				if (ImGui::InputInt("Mask_Rect", &m_iTexIndex_Rect[CEffect_Void::TEXTURE_MASK], 1))
				{
					if (m_iMaxTexIndex_Rect[CEffect_Void::TEXTURE_MASK] <= m_iTexIndex_Rect[CEffect_Void::TEXTURE_MASK])
						m_iTexIndex_Rect[CEffect_Void::TEXTURE_MASK] = m_iMaxTexIndex_Rect[CEffect_Void::TEXTURE_MASK] - 1;

					if (0 > m_iTexIndex_Rect[CEffect_Void::TEXTURE_MASK])
						m_iTexIndex_Rect[CEffect_Void::TEXTURE_MASK] = 0;

					m_pCurVoidDesc->iTextureIndex[CEffect_Void::TEXTURE_MASK] = m_iTexIndex_Rect[CEffect_Void::TEXTURE_MASK];
				}


				// ������_��Ʈ �ؽ�ó =====================================================================================================
				ImGui::SeparatorText("Noise_Rect");
				if (ImGui::Button("Noise_Base"))	// ���̽� ������� ����
				{
					dynamic_cast<CEffect_Rect*>(m_pCurPartEffect)->Change_TextureCom(TEXT("Prototype_Component_Texture_Effect_Noise"));
					m_iMaxTexIndex_Rect[CEffect_Void::TEXTURE_NOISE] = 243;
					m_iTexIndex_Rect[CEffect_Void::TEXTURE_NOISE] = 0;
				}
				ImGui::SameLine();
				if (ImGui::Button("Noise_Lens"))	// ���� ������� ����
				{
					dynamic_cast<CEffect_Rect*>(m_pCurPartEffect)->Change_TextureCom(TEXT("Prototype_Component_Texture_Effect_Noise_Lens"));
					m_iMaxTexIndex_Rect[CEffect_Void::TEXTURE_NOISE] = 1;
					m_iTexIndex_Rect[CEffect_Void::TEXTURE_NOISE] = 0;
				}

				if (ImGui::InputInt("Noise_Rect", &m_iTexIndex_Rect[CEffect_Void::TEXTURE_NOISE], 1))
				{
					if (m_iMaxTexIndex_Rect[CEffect_Void::TEXTURE_NOISE] <= m_iTexIndex_Rect[CEffect_Void::TEXTURE_NOISE])
						m_iTexIndex_Rect[CEffect_Void::TEXTURE_NOISE] = m_iMaxTexIndex_Rect[CEffect_Void::TEXTURE_NOISE] - 1;

					if (0 > m_iTexIndex_Rect[CEffect_Void::TEXTURE_NOISE])
						m_iTexIndex_Rect[CEffect_Void::TEXTURE_NOISE] = 0;

					m_pCurVoidDesc->iTextureIndex[CEffect_Void::TEXTURE_NOISE] = m_iTexIndex_Rect[CEffect_Void::TEXTURE_NOISE];
				}
#pragma endregion

				/* ���̴��� ���� ���ϱ� �÷� ��_��ƼŬ */
				ImGui::SeparatorText("Color_Mul");
				if (ImGui::ColorEdit4("Color_Mul_Rect", m_fColor_Mul_Rect, ImGuiColorEditFlags_None))
				{
					m_pCurVoidDesc->vColor_Mul.x = m_fColor_Mul_Rect[0];
					m_pCurVoidDesc->vColor_Mul.y = m_fColor_Mul_Rect[1];
					m_pCurVoidDesc->vColor_Mul.z = m_fColor_Mul_Rect[2];
					m_pCurVoidDesc->vColor_Mul.w = m_fColor_Mul_Rect[3];
				}


				/* ������ Ű�� ���� */
				ImGui::SeparatorText("Shader_Rect");

				ImGui::RadioButton("BillBoard_Rect", &m_iBillBoard_Rect, 0);  ImGui::SameLine();
				ImGui::RadioButton("Off BillBoard_Rect", &m_iBillBoard_Rect, 1);
				if (0 == m_iBillBoard_Rect)
					m_pCurVoidDesc->bBillBoard = TRUE;
				else if (1 == m_iBillBoard_Rect)
					m_pCurVoidDesc->bBillBoard = FALSE;


				/* ���̴� �н� �ε��� ���� */
				if (ImGui::InputInt("Shader Pass_Rect", &m_iShaderPassIndex_Rect, 1))
				{
					if (m_iMaxShaderPassIndex_Rect < m_iShaderPassIndex_Rect)
						m_iShaderPassIndex_Rect = m_iMaxShaderPassIndex_Rect;

					if (0 > m_iShaderPassIndex_Rect)
						m_iShaderPassIndex_Rect = 0;

					m_pCurVoidDesc->iShaderPassIndex = m_iShaderPassIndex_Rect;
				}
				/* ���̴��� ���� ��ī�� �� ���� */
				//vColor_Clip
				if (ImGui::DragFloat4("Discard_Rect", m_vColor_Clip_Rect, 0.1f, 0.f, 1.f))
				{
					m_pCurVoidDesc->vColor_Clip.x = m_vColor_Clip_Rect[0];
					m_pCurVoidDesc->vColor_Clip.y = m_vColor_Clip_Rect[1];
					m_pCurVoidDesc->vColor_Clip.z = m_vColor_Clip_Rect[2];
					m_pCurVoidDesc->vColor_Clip.w = m_vColor_Clip_Rect[3];
				}

				/* �����׷� ���� */
				ImGui::SeparatorText("Render Group_Rect");
				if (ImGui::InputInt("Render Group_Rect", &m_iRenderGroup_Rect, 1))
				{
					if ((_int)CRenderer::RENDER_END < m_iRenderGroup_Rect)
					{
						m_iRenderGroup_Rect = (_int)CRenderer::RENDER_END - 1;
					}
					m_pCurVoidDesc->iRenderGroup = m_iRenderGroup_Rect;
				}



				ImGui::SeparatorText("");
				if (ImGui::TreeNode(" Distortion Preset "))
				{
					if (ImGui::Button("FIRE"))
					{
						m_pCurVoidDesc->iShaderPassIndex = { 2 };
						m_pCurVoidDesc->iTextureIndex[CEffect_Void::TEXTURE_DIFFUSE] = { 1 };
						m_pCurVoidDesc->iTextureIndex[CEffect_Void::TEXTURE_MASK] = { 17 };
						m_pCurVoidDesc->iTextureIndex[CEffect_Void::TEXTURE_NOISE] = { 1 };

						pDistortionDesc->vScrollSpeeds = { 1.f, 1.f, 1.f };
						pDistortionDesc->vScales = { 1.f, 1.f, 1.f };

						pDistortionDesc->vDistortion1 = { 0.1f, 0.1f };
						pDistortionDesc->vDistortion2 = { 0.f, 0.f };
						pDistortionDesc->vDistortion3 = { 0.f, 0.1f };

						pDistortionDesc->fDistortionScale = { 1.f };
						pDistortionDesc->fDistortionBias = { 1.f };

						Update_CurParameters_Parts();

						ImGui::TreePop();
					}

					ImGui::SeparatorText(" Distortion Values_Rect ");


					/* ����� ��ũ�� ���� ���� */
					ImGui::RadioButton("Row_Scroll_Rect", &m_iType_Scroll_Rect, 0);  ImGui::SameLine();
					ImGui::RadioButton("Col_Scroll_Rect", &m_iType_Scroll_Rect, 1);
					ImGui::RadioButton("Both_Scroll_Rect", &m_iType_Scroll_Rect, 2);
					ImGui::RadioButton("Rotat_Scroll_Rect", &m_iType_Scroll_Rect, 3);
					ImGui::RadioButton("End_Scroll_Rect", &m_iType_Scroll_Rect, 4);
					if (0 == m_iType_Scroll_Rect)
						pDistortionDesc->eType_Scroll = { CEffect_Void::SCROLL_ROW };
					else if (1 == m_iType_Scroll_Rect)
						pDistortionDesc->eType_Scroll = { CEffect_Void::SCROLL_COL };
					else if (2 == m_iType_Scroll_Rect)
						pDistortionDesc->eType_Scroll = { CEffect_Void::SCROLL_BOTH };
					else if (3 == m_iType_Scroll_Rect)
						pDistortionDesc->eType_Scroll = { CEffect_Void::SCROLL_ROTATE };
					else if (4 == m_iType_Scroll_Rect)
						pDistortionDesc->eType_Scroll = { CEffect_Void::TYPE_SCROLL_END };


					if (ImGui::DragFloat3("ScrollSpeeds", m_vScrollSpeeds, 1.f, 0.f))
					{
						pDistortionDesc->vScrollSpeeds.x = m_vScrollSpeeds[0];
						pDistortionDesc->vScrollSpeeds.y = m_vScrollSpeeds[1];
						pDistortionDesc->vScrollSpeeds.z = m_vScrollSpeeds[2];
					}
					if (ImGui::DragFloat3("Distortion_Scales", m_vScales_Distortion, 1.f, 0.f))
					{
						pDistortionDesc->vScales.x = m_vScales_Distortion[0];
						pDistortionDesc->vScales.y = m_vScales_Distortion[1];
						pDistortionDesc->vScales.z = m_vScales_Distortion[2];
					}
					if (ImGui::DragFloat2("Distortion1", m_vDistortion1, 1.f, 0.f))
					{
						pDistortionDesc->vDistortion1.x = m_vDistortion1[0];
						pDistortionDesc->vDistortion1.y = m_vDistortion1[1];
					}
					if (ImGui::DragFloat2("Distortion2", m_vDistortion2, 1.f, 0.f))
					{
						pDistortionDesc->vDistortion2.x = m_vDistortion2[0];
						pDistortionDesc->vDistortion2.y = m_vDistortion2[1];
					}
					if (ImGui::DragFloat2("Distortion3", m_vDistortion3, 1.f, 0.f))
					{
						pDistortionDesc->vDistortion3.x = m_vDistortion3[0];
						pDistortionDesc->vDistortion3.y = m_vDistortion3[1];
					}
					if (ImGui::DragFloat("Distortion_Scale", &m_fDistortionScale, 1.f, 0.f))
					{
						pDistortionDesc->fDistortionScale = m_fDistortionScale;
					}
					if (ImGui::DragFloat("DistortionBias", &m_fDistortionBias, 1.f, 0.f))
					{
						pDistortionDesc->fDistortionBias = m_fDistortionBias;
					}
				}



				/* UV �� ����_Rect */
				if (ImGui::CollapsingHeader("UV Option_Rect"))
				{
					if (ImGui::DragFloat2(" UV_Offset_Rect ", m_fUV_Offset_Rect, 1.f, 0.f, 100.f))
					{
						m_pCurVoidDesc->vUV_Offset.x = m_fUV_Offset_Rect[0];
						m_pCurVoidDesc->vUV_Offset.y = m_fUV_Offset_Rect[1];
					}ImGui::SameLine();
					HelpMarker(u8"UV �̵�");

					if (ImGui::DragFloat2(" UV_Scale ", m_vUV_Scale_Rect, 1.f, 0.f, 100.f))
					{
						m_pCurVoidDesc->vUV_Scale.x = m_vUV_Scale_Rect[0];
						m_pCurVoidDesc->vUV_Scale.y = m_vUV_Scale_Rect[1];
					}ImGui::SameLine();
					HelpMarker(u8"UV ũ��(Ÿ�ϸ�)");

					if (ImGui::DragFloat(" RotDegree_Rect ", &m_fUV_RotDegree_Rect, 1.f, 0.f, 360.f))
					{
						m_pCurVoidDesc->fUV_RotDegree = m_fUV_RotDegree_Rect;
					}ImGui::SameLine();
					HelpMarker(u8"ȸ��");

					ImGui::SeparatorText("");
				}




#pragma region ��������Ʈ ����_��Ʈ
				/* ��������Ʈ Ű�� ���� */
				ImGui::SeparatorText(" Sprite ");
				ImGui::RadioButton("Off Sprite_Rect", &m_iSprite_Rect, 0); ImGui::SameLine();
				ImGui::RadioButton("Sprite_Rect", &m_iSprite_Rect, 1);
				if (0 == m_iSprite_Rect)
				{
					m_pCurVoidDesc->bUseSpriteAnim = FALSE;
				}
				else if (1 == m_iSprite_Rect)
				{
					CEffect_Void::UVSPRITE_DESC* pSpriteDesc = dynamic_cast<CEffect_Rect*>(m_pCurPartEffect)->Get_Sprite_Desc();

					// 1�� Ų��.
					m_pCurVoidDesc->bUseSpriteAnim = TRUE;

					// ��������Ʈ�� ������ Ű�� ����_��Ʈ
					ImGui::RadioButton("Loop Sprite_Rect", &m_iSpriteLoop, 0); ImGui::SameLine();
					ImGui::RadioButton("None Loop Sprite_Rect", &m_iSpriteLoop, 1);
					if (0 == m_iSpriteLoop)
					{
						pSpriteDesc->bLoop = TRUE;
					}
					else if (1 == m_iSpriteLoop)
					{
						pSpriteDesc->bLoop = FALSE;
					}

				}

				ImGui::SeparatorText(" Sprite Setting_Rect ");
				if (m_pCurVoidDesc->bUseSpriteAnim)
				{
					CEffect_Void::UVSPRITE_DESC* pSpriteDesc = dynamic_cast<CEffect_Rect*>(m_pCurPartEffect)->Get_Sprite_Desc();

					/* ��������Ʈ ��� �ӵ�_��Ʈ */
					ImGui::SeparatorText("");
					if (ImGui::DragFloat(" Sprite Term ", &m_fSequenceTerm_Rect, 1, 1))
					{
						pSpriteDesc->fSequenceTerm = m_fSequenceTerm_Rect;
						//m_pSpriteDesc_Rect->Reset_Sprite();
					}

					/* ��������Ʈ �ִ� ���� ���� ����_��Ʈ */
					if (ImGui::InputInt2(" Max_TileCount ", m_vUV_MaxTileCount_Rect, 1))
					{
						_uint iX, iY;
						dynamic_cast<CEffect_Rect*>(m_pCurPartEffect)->Get_TextureCom(CEffect_Void::TEXTURE_SPRITE)->Get_TextureSize(&iX, &iY, m_iTexIndex_Rect[CEffect_Void::TEXTURE_SPRITE]);
						pSpriteDesc->vTextureSize.x = (_float)iX;
						pSpriteDesc->vTextureSize.y = (_float)iY;

						_float fTileX, fTileY;
						fTileX = (_float)iX / m_vUV_MaxTileCount_Rect[0];
						fTileY = (_float)iY / m_vUV_MaxTileCount_Rect[1];

						pSpriteDesc->vTileSize.x = fTileX;
						pSpriteDesc->vTileSize.y = fTileY;

						pSpriteDesc->vUV_MaxTileCount.x = (_float)m_vUV_MaxTileCount_Rect[0];
						pSpriteDesc->vUV_MaxTileCount.y = (_float)m_vUV_MaxTileCount_Rect[1];

						pSpriteDesc->Reset_Sprite();
						m_pCurPartEffect->ReSet_Effect();
					}

					ImGui::Text("Current Index : %d, %d", pSpriteDesc->vUV_CurTileIndex.x, pSpriteDesc->vUV_CurTileIndex.y);
				}
#pragma endregion bUseSpriteAnim ��������Ʈ ���



				/* Rim & Bloom */
				/* ������Ʈ & ��� */
				ImGui::SeparatorText("Rim Bloom_Rect");
				if (ImGui::ColorEdit4("RimColor_Rect", m_fRimColor_Rect, ImGuiColorEditFlags_None))
				{
					m_pCurVoidDesc->vRimColor.x = m_fRimColor_Rect[0];
					m_pCurVoidDesc->vRimColor.y = m_fRimColor_Rect[1];
					m_pCurVoidDesc->vRimColor.z = m_fRimColor_Rect[2];
					m_pCurVoidDesc->vRimColor.w = m_fRimColor_Rect[3];
				}

				if (ImGui::ColorEdit3("BloomPower_Rect", m_vBloomPower_Rect, ImGuiColorEditFlags_None))
				{
					m_pCurVoidDesc->vBloomPower.x = m_vBloomPower_Rect[0];
					m_pCurVoidDesc->vBloomPower.y = m_vBloomPower_Rect[1];
					m_pCurVoidDesc->vBloomPower.z = m_vBloomPower_Rect[2];
				}





			} // Rect Ÿ�� ������Ʈ ����
			
		}
	}
}

void CWindow_EffectTool::Update_MeshTab()
{
	if (nullptr != m_pCurEffect)
	{
		if (nullptr != m_pCurPartEffect)
		{
			auto& style = ImGui::GetStyle();

			CEffect_Void::TYPE_EFFECT eType_Effect = m_pCurVoidDesc->eType_Effect;

			if (CEffect_Void::MESH == eType_Effect)
			{
#pragma region Desc ������ ������Ʈ_�޽�
				m_pCurVoidDesc = m_pCurPartEffect->Get_Desc();												// ����Ʈ_���̵� ����ü
				m_pInstanceDesc = dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Get_InstanceDesc();	// ����Ʈ_�ν��Ͻ� ����ü
				CVIBuffer_Effect_Model_Instance* pVIBuffer = dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Get_VIBufferCom();	// ����Ʈ �� �ν��Ͻ� VIBuffer��ü ������
				m_pMeshBufferDesc = pVIBuffer->Get_Desc();	// ������ ����ü ������
#pragma endregion

				/* �̸� */
				ImGui::SeparatorText(" NAME ");
				//ImGui::Text(m_pGameInstance->ConverWStringtoC(m_pCurVoidDesc->strPartTag));
				ImGui::Text(m_szPartNames[m_iCurPartIndex]);


				/* ��Ʈ ���� Ű�� ���� */
				ImGui::SeparatorText("Loop_Part");
				ImGui::RadioButton("Loop_Part", &m_iLoop_Part, 0);
				ImGui::RadioButton("None Loop_Part", &m_iLoop_Part, 1);
				if (0 == m_iLoop_Part)
					m_pCurVoidDesc->bLoop = TRUE;
				else if (1 == m_iLoop_Part)
					m_pCurVoidDesc->bLoop = FALSE;


				/* ������ Ÿ�� */
				ImGui::SeparatorText(" LIFE TIME ");
				if (ImGui::DragFloat2("MinMaxLifeTime_Mesh", m_vMinMaxLifeTime_Mesh, 1.f, 0.f, 360.f))
				{
					if (m_vMinMaxLifeTime_Mesh[0] > m_vMinMaxLifeTime_Mesh[1])	// Min�� Max���� ũ�� Max�� Min����
						m_vMinMaxLifeTime_Mesh[1] = m_vMinMaxLifeTime_Mesh[0];


					m_pMeshBufferDesc->vMinMaxLifeTime.x = m_vMinMaxLifeTime_Mesh[0];
					m_pMeshBufferDesc->vMinMaxLifeTime.y = m_vMinMaxLifeTime_Mesh[1];
				}
				ImGui::SeparatorText("");

				/* �� ���� */
				ImGui::PushID(4);
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.f, 0.f, 0.8f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.f, 0.f, 0.9f, 0.9f));
				if (ImGui::CollapsingHeader(" Change Model "))
				{
					// ����Ʈ�� ����ƽ �޽��� ����
					if (ImGui::TreeNode(" Mesh_Static "))
					{
						if (ImGui::Button("Aoe_Lens"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_Aoe_Lens"));

						if (ImGui::Button("Billboard_Circle_00"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_Billboard_Circle_00"));

						if (ImGui::Button("Ring"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_Ring"));
						
						ImGui::SameLine();		
						if (ImGui::Button("Sphere"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_Sphere"));

						if (ImGui::Button("Projectile"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_Projectile"));

						if (ImGui::Button("Corn"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_Corn"));
						
						ImGui::SameLine();
						if (ImGui::Button("ShieldDome"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_ShieldDome"));

						if (ImGui::Button("Tornado"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_Tornado"));

						ImGui::SameLine();
						if (ImGui::Button("Tornado_Cream"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_Tornado_cream"));

						if (ImGui::Button("Tornado_splineMesh"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_Tornado_splineMesh"));

						if (ImGui::Button("WinchesterElectric"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_WinchesterElectric"));

						if (ImGui::Button("LightningFast"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_LightningFast"));
						
						ImGui::SameLine();
						if (ImGui::Button("LightningsPack"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_LightningsPack"));

						if (ImGui::Button("HemiSphere"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_HemiSphere"));

						if (ImGui::Button("Coil"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_Coil"));


						ImGui::SeparatorText("");
						ImGui::TreePop();
					}

					// ����(����ü) ����Ʈ �޽� ����
					if (ImGui::TreeNode(" Mesh_Static_Attack "))
					{
						if (ImGui::Button("Slash_00"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_Slash_00"));
						
						ImGui::SameLine();
						if (ImGui::Button("Slash_01"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_Slash_01"));

						if (ImGui::Button("Bioball_00"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_Bioball_00"));

						if (ImGui::Button("Bioball_01"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_Bioball_01"));

						if (ImGui::Button("Bioball_02"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_Bioball_02"));

						ImGui::SeparatorText("");
						ImGui::TreePop();
					}


					// ��ƼŬ�� ����Ʈ �޽� ����
					if (ImGui::TreeNode(" Mesh_Particle "))
					{
						if (ImGui::Button("Rock_00"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_Rock_00"));

						ImGui::SameLine();
						if (ImGui::Button("Rock_01"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_Rock_01"));

						if (ImGui::Button("Rock_02"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_Rock_02"));
						
						ImGui::SameLine();
						if (ImGui::Button("Rock_03"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_Rock_03"));

						if (ImGui::Button("LightningParticle_00"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_LightningParticle_00"));

						if (ImGui::Button("LightningParticle_01"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_LightningParticle_01"));

						if (ImGui::Button("LeafPlane"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_LeafPlane"));

						ImGui::SeparatorText("");
						ImGui::TreePop();
					}


					// ��Ʈ�� ������ ����Ʈ �޽� ����
					if (ImGui::TreeNode(" Mesh_VampireCommander "))
					{
						//ImGui::SeparatorText("VampireCommander");
						if (ImGui::Button("Demo_BeastSkull"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_BeastSkull"));

						ImGui::SameLine();
						if (ImGui::Button("Demo_Impact"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_Impact"));

						//if (ImGui::Button(" Add Bat Test "))
						//	Add_Part_Mesh_Morph(TEXT("Prototype_Component_Model_Effect_BatStorm_01"), TEXT("Prototype_Component_Model_Effect_BatStorm_02"));

						ImGui::SameLine();
						if (ImGui::Button(" Add Torch Test "))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_Torch"));

						ImGui::SeparatorText("");
						ImGui::TreePop();
					}

					// �ι�° ������ ����Ʈ �޽� ����
					if (ImGui::TreeNode(" Mesh_Parasiter "))
					{
						if (ImGui::Button("Egg"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_Egg"));

						if (ImGui::Button("Egg_Mother"))
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(TEXT("Prototype_Component_Model_Effect_Egg_Mother"));

						ImGui::SeparatorText("");
						ImGui::TreePop();
					}

				}
				ImGui::PopStyleColor(3);
				ImGui::PopID();


				/* �ؽ�ó ���� */
				ImGui::SeparatorText("");
				if (ImGui::CollapsingHeader(" Texture_Mesh "))
				{
					/* �� �ؽ�ó�� ������, ���� �ؽ�ó�� �����ٰ��� */
					if (ImGui::Button(" Use CustomTex "))
					{
						m_pInstanceDesc->bUseCustomTex = TRUE;
					}ImGui::SameLine();
					if (ImGui::Button("  Use ModelTex "))
					{
						m_pInstanceDesc->bUseCustomTex = FALSE;
					}
					ImGui::SeparatorText("");

					// ��ǻ��_�޽� �ؽ�ó =====================================================================================================
					if (ImGui::TreeNode(" Diffuse_Texture_Mesh "))
					{
						if (ImGui::Button("Diffuse_Base"))	// ���̽� ��ǻ��� ����
						{
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_TextureCom(TEXT("Prototype_Component_Texture_Effect_Diffuse"));
							m_iMaxTexIndex_Mesh[CEffect_Void::TEXTURE_DIFFUSE] = 25;
							m_iTexIndex_Mesh[CEffect_Void::TEXTURE_DIFFUSE] = 0;


						}ImGui::SameLine();
						if (ImGui::Button("Diffuse_Waves"))	// ���̺� ��ǻ��� ����
						{
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_TextureCom(TEXT("Prototype_Component_Texture_Effect_Diffuse_Waves"));
							m_iMaxTexIndex_Mesh[CEffect_Void::TEXTURE_DIFFUSE] = 6;
							m_iTexIndex_Mesh[CEffect_Void::TEXTURE_DIFFUSE] = 0;
						}


						if (ImGui::InputInt("Diffuse_Mesh", &m_iTexIndex_Mesh[CEffect_Void::TEXTURE_DIFFUSE], 1))
						{
							if (m_iMaxTexIndex_Mesh[CEffect_Void::TEXTURE_DIFFUSE] <= m_iTexIndex_Mesh[CEffect_Void::TEXTURE_DIFFUSE])
								m_iTexIndex_Mesh[CEffect_Void::TEXTURE_DIFFUSE] = m_iMaxTexIndex_Mesh[CEffect_Void::TEXTURE_DIFFUSE] - 1;

							if (0 > m_iTexIndex_Mesh[CEffect_Void::TEXTURE_DIFFUSE])
								m_iTexIndex_Mesh[CEffect_Void::TEXTURE_DIFFUSE] = 0;

							m_pCurVoidDesc->iTextureIndex[CEffect_Void::TEXTURE_DIFFUSE] = m_iTexIndex_Mesh[CEffect_Void::TEXTURE_DIFFUSE];
						}

						if (ImGui::Button(" Remove Diffuse_Mesh ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25)))
						{
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Remove_TextureCom(CEffect_Void::TEXTURE_DIFFUSE);
						}

						ImGui::SeparatorText("");
						ImGui::TreePop();
					}
					// ��ǻ��_�޽� �ؽ�ó =====================================================================================================


					// �븻_�޽� �ؽ�ó =====================================================================================================
					if (ImGui::TreeNode(" Normal_Texture_Mesh "))
					{
						if (ImGui::Button("Normal_Base_Mesh"))	// ���̽� �븻�� ����
						{
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_TextureCom(TEXT("Prototype_Component_Texture_Effect_Normal"));
							m_iMaxTexIndex_Mesh[CEffect_Void::TEXTURE_NORAML] = 9;
							m_iTexIndex_Mesh[CEffect_Void::TEXTURE_NORAML] = 0;
						}

						if (ImGui::InputInt("Noraml_Mesh", &m_iTexIndex_Mesh[CEffect_Void::TEXTURE_NORAML], 1))
						{
							if (m_iMaxTexIndex_Mesh[CEffect_Void::TEXTURE_NORAML] <= m_iTexIndex_Mesh[CEffect_Void::TEXTURE_NORAML])
								m_iTexIndex_Mesh[CEffect_Void::TEXTURE_NORAML] = m_iMaxTexIndex_Mesh[CEffect_Void::TEXTURE_NORAML] - 1;

							if (0 > m_iTexIndex_Mesh[CEffect_Void::TEXTURE_NORAML])
								m_iTexIndex_Mesh[CEffect_Void::TEXTURE_NORAML] = 0;

							m_pCurVoidDesc->iTextureIndex[CEffect_Void::TEXTURE_NORAML] = m_iTexIndex_Mesh[CEffect_Void::TEXTURE_NORAML];
						}

						if (ImGui::Button(" Remove_Noraml_Mesh ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25))) // �븻 �ؽ�ó ������Ʈ ����
						{
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Remove_TextureCom(CEffect_Void::TEXTURE_NORAML);
						}

						ImGui::SeparatorText("");
						ImGui::TreePop();
					}


					// ����ũ_�޽� �ؽ�ó =====================================================================================================
					if (ImGui::TreeNode(" Mask_Texture_Mesh "))
					{
						if (ImGui::Button("Mask_Base_Mesh"))	// ���̽� ����ũ�� ����
						{
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_TextureCom(TEXT("Prototype_Component_Texture_Effect_Mask"));
							m_iMaxTexIndex_Mesh[CEffect_Void::TEXTURE_MASK] = 170;
							m_iTexIndex_Mesh[CEffect_Void::TEXTURE_MASK] = 0;

						}ImGui::SameLine();
						if (ImGui::Button("Mask_Waves_Mesh"))	// ���̺� ����ũ�� ����
						{
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_TextureCom(TEXT("Prototype_Component_Texture_Effect_Mask_Waves"));
							m_iMaxTexIndex_Mesh[CEffect_Void::TEXTURE_MASK] = 6;
							m_iTexIndex_Mesh[CEffect_Void::TEXTURE_MASK] = 0;
						}



						if (ImGui::InputInt("Mask_Mesh", &m_iTexIndex_Mesh[CEffect_Void::TEXTURE_MASK], 1))
						{
							if (m_iMaxTexIndex_Mesh[CEffect_Void::TEXTURE_MASK] <= m_iTexIndex_Mesh[CEffect_Void::TEXTURE_MASK])
								m_iTexIndex_Mesh[CEffect_Void::TEXTURE_MASK] = m_iMaxTexIndex_Mesh[CEffect_Void::TEXTURE_MASK] - 1;

							if (0 > m_iTexIndex_Mesh[CEffect_Void::TEXTURE_MASK])
								m_iTexIndex_Mesh[CEffect_Void::TEXTURE_MASK] = 0;

							m_pCurVoidDesc->iTextureIndex[CEffect_Void::TEXTURE_MASK] = m_iTexIndex_Mesh[CEffect_Void::TEXTURE_MASK];
						}

						if (ImGui::Button(" Remove_Mask_Mesh ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25))) // ����ũ �ؽ�ó ������Ʈ ����
						{
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Remove_TextureCom(CEffect_Void::TEXTURE_MASK);
						}

						ImGui::SeparatorText("");
						ImGui::TreePop();
					}


					// ������_�޽� �ؽ�ó =====================================================================================================			
					if (ImGui::TreeNode(" Noise_Texture_Mesh "))
					{
						if (ImGui::Button("Noise_Base_Mesh"))	// ���̽� ������� ����
						{
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_TextureCom(TEXT("Prototype_Component_Texture_Effect_Noise"));
							m_iMaxTexIndex_Mesh[CEffect_Void::TEXTURE_NOISE] = 243;
							m_iTexIndex_Mesh[CEffect_Void::TEXTURE_NOISE] = 0;

						}


						ImGui::SeparatorText(" Noise ");
						if (ImGui::InputInt("Noise_Mesh", &m_iTexIndex_Mesh[CEffect_Void::TEXTURE_NOISE], 1))
						{
							if (m_iMaxTexIndex_Mesh[CEffect_Void::TEXTURE_NOISE] <= m_iTexIndex_Mesh[CEffect_Void::TEXTURE_NOISE])
								m_iTexIndex_Mesh[CEffect_Void::TEXTURE_NOISE] = m_iMaxTexIndex_Mesh[CEffect_Void::TEXTURE_NOISE] - 1;

							if (0 > m_iTexIndex_Mesh[CEffect_Void::TEXTURE_NOISE])
								m_iTexIndex_Mesh[CEffect_Void::TEXTURE_NOISE] = 0;

							m_pCurVoidDesc->iTextureIndex[CEffect_Void::TEXTURE_NOISE] = m_iTexIndex_Mesh[CEffect_Void::TEXTURE_NOISE];
						}


						if (ImGui::Button(" Remove_Noise_Mesh ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25))) // ������ �ؽ�ó ������Ʈ ����
						{
							dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Remove_TextureCom(CEffect_Void::TEXTURE_NOISE);
						}

						ImGui::SeparatorText("");
						ImGui::TreePop();
					}


					ImGui::SeparatorText("");
				}



				/* UV �� ���� */
				if (ImGui::CollapsingHeader(" UV Option_Mesh "))
				{
					if (ImGui::DragFloat2(" UV_Offset ", m_fUV_Offset_Mesh, 1.f, 0.f, 100.f))
					{
						m_pCurVoidDesc->vUV_Offset.x = m_fUV_Offset_Mesh[0];
						m_pCurVoidDesc->vUV_Offset.y = m_fUV_Offset_Mesh[1];
					}ImGui::SameLine();
					HelpMarker(u8"UV �̵�");

					if (ImGui::DragFloat2(" UV_Scale ", m_vUV_Scale_Mesh, 1.f, 0.f, 100.f))
					{
						m_pCurVoidDesc->vUV_Scale.x = m_vUV_Scale_Mesh[0];
						m_pCurVoidDesc->vUV_Scale.y = m_vUV_Scale_Mesh[1];
					}ImGui::SameLine();
					HelpMarker(u8"UV ũ��(Ÿ�ϸ�)");

					if (ImGui::DragFloat(" RotDegree_Mesh ", &m_fUV_RotDegree_Mesh, 1.f, 0.f, 360.f))
					{
						m_pCurVoidDesc->fUV_RotDegree = m_fUV_RotDegree_Mesh;
					}ImGui::SameLine();
					HelpMarker(u8"ȸ��");

					ImGui::SeparatorText("");
				}


				/* ���̴� �н� �ε��� ���� */
				if (ImGui::CollapsingHeader(" Shader & Render _MESH "))
				{
					if (ImGui::InputInt("Shader Pass_Mesh", &m_iShaderPassIndex_Mesh, 1))
					{
						if (m_iMaxShaderPassIndex_Mesh < m_iShaderPassIndex_Mesh)
							m_iShaderPassIndex_Mesh = m_iMaxShaderPassIndex_Mesh;

						if (0 > m_iShaderPassIndex_Mesh)
							m_iShaderPassIndex_Mesh = 0;

						m_pCurVoidDesc->iShaderPassIndex = m_iShaderPassIndex_Mesh;
					}

					/* ���̴��� ���� ��ī�� �� ���� */
					if (ImGui::DragFloat4("Discard_Mesh", m_vColor_Clip_Part, 0.1f, 0.f, 1.f))
					{
						m_pCurVoidDesc->vColor_Clip.x = m_vColor_Clip_Part[0];
						m_pCurVoidDesc->vColor_Clip.y = m_vColor_Clip_Part[1];
						m_pCurVoidDesc->vColor_Clip.z = m_vColor_Clip_Part[2];
						m_pCurVoidDesc->vColor_Clip.w = m_vColor_Clip_Part[3];
					}


					///* �����׷� ���� */
					//ImGui::SeparatorText("Render Group_MESH");
					//if (ImGui::InputInt("Render Group_Mesh", &m_iRenderGroup_Mesh, 1))
					//{
					//	if ((_int)CRenderer::RENDER_END < m_iRenderGroup_Mesh)
					//	{
					//		m_iRenderGroup_Mesh = (_int)CRenderer::RENDER_END - 1;
					//	}
					//	m_pCurVoidDesc->iRenderGroup = m_iRenderGroup_Mesh;
					//}

					ImGui::SeparatorText("");
				}



				//if (ImGui::CollapsingHeader(" Morp_Mesh (Bat Test) "))
				//{
				//	/* �� �ٲ㳢�� */
				//	if (ImGui::Button("ON Morph"))
				//	{
				//		m_pMeshBufferDesc->bMorph = TRUE;

				//	}ImGui::SameLine();
				//	if (ImGui::Button("OFF Morph"))
				//	{
				//		m_pMeshBufferDesc->bMorph = FALSE;
				//	}

				//	/* �� �ٲ㳢�� �ð� �� ����*/
				//	if (ImGui::DragFloat("MorphTimeTerm", &m_fMorphTimeTerm, 0.01f, 0.f, 360.f))
				//	{
				//		if (0.f > m_fMorphTimeTerm)
				//			m_fMorphTimeTerm = 0.f;

				//		m_pMeshBufferDesc->fMorphTimeTerm = m_fMorphTimeTerm;

				//	}

				//	ImGui::SeparatorText("");
				//}


				//// �޽� ��� ���� : MODE_STATIC, MODE_PARTICLE, MODE_END
				//ImGui::SeparatorText(" Mode_Mesh ");
				//ImGui::RadioButton("STATIC_Mode_Mesh ", &m_iType_Mode_Mesh, 0);
				//ImGui::RadioButton("PARTICLE_Mesh", &m_iType_Mode_Mesh, 1);

				//if (0 == m_iType_Mode_Mesh)
				//	m_pMeshBufferDesc->eType_Mode = CVIBuffer_Effect_Model_Instance::MODE_STATIC;
				//else if (1 == m_iType_Mode_Mesh)
				//	m_pMeshBufferDesc->eType_Mode = CVIBuffer_Effect_Model_Instance::MODE_PARTICLE;



				/* �ν��Ͻ� ���� ���� */
				ImGui::SeparatorText(" Instance Num_Mesh");
				ImGui::Text("MaxInstance : %d", m_iMaxNumInstance_Mesh);
				if (ImGui::DragInt("Instance Count", &m_iNumInstance_Mesh, 1, 1, m_iNumInstance_Mesh))
				{
					// 1���� �۾��� �� ����
					if (1 > m_iNumInstance_Mesh)
						m_iNumInstance_Mesh = 1;

					// �ƽ� ������ �Ѿ �� ����
					if (m_iMaxNumInstance_Mesh < m_iNumInstance_Mesh)
						m_iNumInstance_Mesh = m_iMaxNumInstance_Mesh;

					m_pCurVoidDesc->iCurNumInstance = m_iNumInstance_Mesh;
					m_pMeshBufferDesc->iCurNumInstance = m_iNumInstance_Mesh;
				}


				/* ����(Emitter) */
				ImGui::SeparatorText(u8"���� ����(Emitter)_Mesh "); ImGui::SameLine(); HelpMarker(u8"���� �ð� ��, �ѹ� ������ �� ����Ǵ� ���� ��(0�� �� 1���� ����)");
				if (ImGui::DragFloat("EmissionTime_Mesh", &m_fEmissionTime_Mesh, 0.01f, 0.f, 1000.f))
				{
					if (0.f >= m_fEmissionTime_Mesh)	// 0.f���� �۾����� 0����
						m_fEmissionTime_Mesh = 0.f;

					m_pMeshBufferDesc->fEmissionTime = m_fEmissionTime_Mesh;
				}

				if (ImGui::DragInt("AddEmitCount_Mesh", &m_iAddEmitCount_Mesh, 1, 0, m_pMeshBufferDesc->iCurNumInstance))
				{
					// ���� �� ����
					m_pCurEffectDesc->bPlay = FALSE;
					m_pCurEffect->Init_ReSet_Effect();

					if (0 >= m_iAddEmitCount_Mesh)	// 0���� �۾����� 0����
						m_iAddEmitCount_Mesh = 0;

					if (m_pMeshBufferDesc->iCurNumInstance <= m_iAddEmitCount_Mesh)	// ���� �ν��Ͻ� ������ ���ų� Ŀ���� ���� �ν��Ͻ� ������
						m_iAddEmitCount_Mesh = m_pMeshBufferDesc->iCurNumInstance;

					m_pMeshBufferDesc->iAddEmitCount = (_uint)m_iAddEmitCount_Mesh;
				}



				/* ���� ������ ������ */
				/* MinCenterOffsetPos */
				ImGui::SeparatorText(" Center Position_Mesh ");
				if (ImGui::DragFloat3("MinCenterOffsetPos_Mesh", m_vMinCenterOffsetPos_Mesh, 0.1f, -500.f, 500.f))
				{
					m_pMeshBufferDesc->vMinCenterOffsetPos.x = m_vMinCenterOffsetPos_Mesh[0];
					m_pMeshBufferDesc->vMinCenterOffsetPos.y = m_vMinCenterOffsetPos_Mesh[1];
					m_pMeshBufferDesc->vMinCenterOffsetPos.z = m_vMinCenterOffsetPos_Mesh[2];
				}

				/* MaxCenterOffsetPos */
				if (ImGui::DragFloat3("MaxCenterOffsetPos_Mesh", m_vMaxCenterOffsetPos_Mesh, 0.1f, -500.f, 500.f))
				{
					m_pMeshBufferDesc->vMaxCenterOffsetPos.x = m_vMaxCenterOffsetPos_Mesh[0];
					m_pMeshBufferDesc->vMaxCenterOffsetPos.y = m_vMaxCenterOffsetPos_Mesh[1];
					m_pMeshBufferDesc->vMaxCenterOffsetPos.z = m_vMaxCenterOffsetPos_Mesh[2];
				}


				/* ������ ���� */
				ImGui::SeparatorText(" Range ");
				if (ImGui::DragFloat2("MinMaxRange_Mesh", m_vMinMaxRange_Mesh, 1.f, 0.1f, 360.f))
				{
					if (m_vMinMaxRange_Mesh[0] > m_vMinMaxRange_Mesh[1])
						m_vMinMaxRange_Mesh[0] = m_vMinMaxRange_Mesh[1];

					m_pMeshBufferDesc->vMinMaxRange.x = m_vMinMaxRange_Mesh[0];
					m_pMeshBufferDesc->vMinMaxRange.y = m_vMinMaxRange_Mesh[1];
				}


				/* �߰� ���� ����(������)*/
				ImGui::SeparatorText(" Add Range_Mesh ");
				if (ImGui::DragFloat2("MinMax Add Range_Mesh", m_fMinMaxAddRange_Mesh, 0.1f, -500.f, 500.f))
				{
					m_pMeshBufferDesc->vMinMaxAddRange.x = m_fMinMaxAddRange_Mesh[0];
					m_pMeshBufferDesc->vMinMaxAddRange.y = m_fMinMaxAddRange_Mesh[1];

				}

				//if (CVIBuffer_Mesh::TORNADO == m_pMeshBufferDesc->eType_Action)
				{
					ImGui::SeparatorText(" Theta "); ImGui::SameLine();	HelpMarker(u8"Degree�� �Է�(Radian �ڵ���ȯ)");

					if (ImGui::DragFloat2("vMinMaxTheta_Mesh", m_vMinMaxTheta_Mesh, 0.f, 1.f, 360.f))
					{
						m_pMeshBufferDesc->vMinMaxTheta.x = m_vMinMaxTheta_Mesh[0];
						m_pMeshBufferDesc->vMinMaxTheta.y = m_vMinMaxTheta_Mesh[1];
					}
				}


				if (CVIBuffer_Effect_Model_Instance::FALL == m_pMeshBufferDesc->eType_Action
					|| CVIBuffer_Effect_Model_Instance::RISE == m_pMeshBufferDesc->eType_Action
					|| CVIBuffer_Effect_Model_Instance::TORNADO == m_pMeshBufferDesc->eType_Action
					|| 0 == m_iUseRigidBody_Mesh)
				{

					ImGui::SeparatorText(" Y_Max Pos ");
					if (ImGui::DragFloat2("vMinMaxPosY_Mesh", m_vMinMaxPosY_Mesh, 0.f, 1.f, 1000.f))
					{
						m_pMeshBufferDesc->vMinMaxPosY.x = m_vMinMaxPosY_Mesh[0];
						m_pMeshBufferDesc->vMinMaxPosY.y = m_vMinMaxPosY_Mesh[1];
					}
				}


				/* ���ǵ� ����(���� ����) */
				ImGui::SeparatorText(" Speed ");
				if (ImGui::DragFloat2("vMinMaxSpeed_Mesh", m_vMinMaxSpeed_Mesh, 0.f, 1.f, 1000.f))
				{
					if (0.f > m_vMinMaxSpeed_Mesh[0])	// Min�� 0���� �۾����� 0���� ����
						m_vMinMaxSpeed_Mesh[0] = 0.f;

					if (m_vMinMaxSpeed_Mesh[0] > m_vMinMaxSpeed_Mesh[1])	// Min�� Max���� ũ�� Max�� Min����
						m_vMinMaxSpeed_Mesh[1] = m_vMinMaxSpeed_Mesh[0];


					m_pMeshBufferDesc->vMinMaxSpeed.x = m_vMinMaxSpeed_Mesh[0];
					m_pMeshBufferDesc->vMinMaxSpeed.y = m_vMinMaxSpeed_Mesh[1];
				}

				if (ImGui::TreeNode(" Speed_Easing Types_Mesh "))
				{
					ImGui::Text("Selected Type : %d", m_pMeshBufferDesc->eType_SpeedLerp);

					Select_EasingType(&m_pMeshBufferDesc->eType_SpeedLerp);

					ImGui::TreePop();
				}


				/* �޽� ��ƼŬ ������Ŭ */
				ImGui::SeparatorText(" Recycle_Mesh ");
				ImGui::RadioButton("Recycle_Mesh", &m_iRecycle_Mesh, 0);  ImGui::SameLine();
				ImGui::RadioButton("None Recycle_Mesh", &m_iRecycle_Mesh, 1);
				if (0 == m_iRecycle_Mesh)
					m_pMeshBufferDesc->bRecycle = TRUE;
				else if (1 == m_iRecycle_Mesh)
					m_pMeshBufferDesc->bRecycle = FALSE;


				/* ��ƼŬ ������� ������ Reverse */
				ImGui::SeparatorText(" Reverse_Mesh ");
				ImGui::RadioButton("Reverse_Mesh", &m_iReverse_Mesh, 0);  ImGui::SameLine();
				ImGui::RadioButton("None Reverse_Mesh", &m_iReverse_Mesh, 1);
				if (0 == m_iReverse_Mesh)
					m_pMeshBufferDesc->bReverse = TRUE;
				else if (1 == m_iReverse_Mesh)
					m_pMeshBufferDesc->bReverse = FALSE;


				/* ��ƼŬ �׼� Ÿ�� */
				ImGui::SeparatorText(" Action Type_Mesh ");
				ImGui::RadioButton("SPARK_Mesh ", &m_iType_Action_Mesh, 0);
				ImGui::RadioButton("BLINK_Mesh", &m_iType_Action_Mesh, 1);
				ImGui::RadioButton("FALL_Mesh", &m_iType_Action_Mesh, 2);
				ImGui::RadioButton("RISE_Mesh", &m_iType_Action_Mesh, 3);
				ImGui::RadioButton("TORNADO_Mesh", &m_iType_Action_Mesh, 4);
				if (0 == m_iType_Action_Mesh)
					m_pMeshBufferDesc->eType_Action = CVIBuffer_Effect_Model_Instance::SPARK;
				else if (1 == m_iType_Action_Mesh)
					m_pMeshBufferDesc->eType_Action = CVIBuffer_Effect_Model_Instance::BLINK;
				else if (2 == m_iType_Action_Mesh)
					m_pMeshBufferDesc->eType_Action = CVIBuffer_Effect_Model_Instance::FALL;
				else if (3 == m_iType_Action_Mesh)
					m_pMeshBufferDesc->eType_Action = CVIBuffer_Effect_Model_Instance::RISE;
				else if (4 == m_iType_Action_Mesh)
					m_pMeshBufferDesc->eType_Action = CVIBuffer_Effect_Model_Instance::TORNADO;




#pragma region ������ٵ� �ɼ� ����_�޽� ��ƼŬ

				ImGui::SeparatorText("");
				if (ImGui::CollapsingHeader(" RigidBody_Mesh "))
				{

				/* ������ٵ� Ű�� ����_Mesh */
				ImGui::SeparatorText(" On Off RigidBody_Mesh ");
				ImGui::RadioButton(" ON  RigidBody_Mesh ", &m_iUseRigidBody_Mesh, 0);
				ImGui::RadioButton(" OFF RigidBody_Mesh ", &m_iUseRigidBody_Mesh, 1);
				if (0 == m_iUseRigidBody_Mesh)
				{
					m_pCurVoidDesc->bUseRigidBody = TRUE;
					m_pMeshBufferDesc->bUseRigidBody = TRUE;
				}
				else if (1 == m_iUseRigidBody_Mesh)
				{
					m_pCurVoidDesc->bUseRigidBody = FALSE;
					m_pMeshBufferDesc->bUseRigidBody = FALSE;
				}


				if (0 == m_iUseRigidBody_Mesh)
				{
					/* Ű��ƽ Ű�� ���� */
					ImGui::SeparatorText(" Kinetic_Mesh ");
					ImGui::RadioButton(" Kinetic_Mesh ", &m_iKinetic_Mesh, 0);
					ImGui::RadioButton(" Kinematic_Mesh ", &m_iKinetic_Mesh, 1);
					if (0 == m_iKinetic_Mesh)
					{
						m_pMeshBufferDesc->bKinetic = TRUE;
					}
					else if (1 == m_iKinetic_Mesh)
					{
						m_pMeshBufferDesc->bKinetic = FALSE;
					}

					/* �߷� Ű�� ���� */
					ImGui::SeparatorText(" Gravity_Mesh ");
					ImGui::RadioButton(" ON   Gravity_Mesh ", &m_iUseGravity_Mesh, 0);
					ImGui::RadioButton(" OFF  Gravity_Mesh ", &m_iUseGravity_Mesh, 1);
					if (0 == m_iUseGravity_Mesh)
					{
						m_pMeshBufferDesc->bUseGravity = TRUE;
					}
					else if (1 == m_iUseGravity_Mesh)
					{
						m_pMeshBufferDesc->bUseGravity = FALSE;
					}

					/* �߷� ���ӵ� ���� */
					if (0 == m_iUseGravity_Mesh)
					{
						if (ImGui::DragFloat("Gravity_Mesh", &m_fGravity_Mesh, 0.5f, -100.f, 1000.f))
						{
							m_pMeshBufferDesc->fGravity = m_fGravity_Mesh;
						}
					}

					//if (ImGui::Button(" FORCE "))
					//{
					//	m_pMeshBufferDesc->eForce_Mode = FORCE_MODE::FORCE;
					//}
					//ImGui::SameLine();
					//if (ImGui::Button(" IMPULSE "))
					//{
					//	m_pMeshBufferDesc->eForce_Mode = FORCE_MODE::IMPULSE;
					//}
					//ImGui::SameLine();
					//if (ImGui::Button(" ACCELERATION "))
					//{
					//	m_pMeshBufferDesc->eForce_Mode = FORCE_MODE::ACCELERATION;
					//}
					//ImGui::SameLine();
					//if (ImGui::Button(" VELOCITY_CHANGE "))
					//{
					//	m_pMeshBufferDesc->eForce_Mode = FORCE_MODE::VELOCITY_CHANGE;
					//}


					/* �Ŀ� ���� */
					ImGui::SeparatorText("Power_Mesh");
					if (ImGui::DragFloat2("Power_Mesh", m_vMinMaxPower_Mesh, 1.f, 0.1f))
					{
						m_pMeshBufferDesc->vMinMaxPower.x = m_vMinMaxPower_Mesh[0];
						m_pMeshBufferDesc->vMinMaxPower.y = m_vMinMaxPower_Mesh[1];
					}


					/* Mass(����) ���� */
					ImGui::SeparatorText("Mass");
					if (ImGui::DragFloat2("vMinMaxMass_Mesh", m_vMinMaxMass_Mesh, 1.f, 0.f, 1000.f))
					{
						if (m_vMinMaxMass_Mesh[0] > m_vMinMaxMass_Mesh[1])	// Min�� Max���� ũ�� Max�� Min����
							m_vMinMaxMass_Mesh[1] = m_vMinMaxMass_Mesh[0];


						m_pMeshBufferDesc->vMinMaxMass.x = m_vMinMaxMass_Mesh[0];
						m_pMeshBufferDesc->vMinMaxMass.y = m_vMinMaxMass_Mesh[1];
					}

					/* Friction(�������) ���� */
					ImGui::SeparatorText("Friction");
					if (ImGui::DragFloat2("vMinMaxFriction_Mesh", m_fMinMaxFriction_Mesh, 0.01f, 0.f, 10.f))
					{
						if (m_fMinMaxFriction_Mesh[0] > m_fMinMaxFriction_Mesh[1])	// Min�� Max���� ũ�� Max�� Min����
							m_fMinMaxFriction_Mesh[1] = m_fMinMaxFriction_Mesh[0];


						m_pMeshBufferDesc->vMinMaxFriction.x = m_fMinMaxFriction_Mesh[0];
						m_pMeshBufferDesc->vMinMaxFriction.y = m_fMinMaxFriction_Mesh[1];
					}


					/* ���� �Ѱ��� ����(1���Ͽ��� ��) */
					ImGui::SeparatorText(u8"���� �Ѱ���(SleepThreshold)");
					if (ImGui::DragFloat("SleepThreshold_Mesh", &m_fSleepThreshold_Mesh, 0.01f, 0.f, 1.f))
					{
						m_pMeshBufferDesc->fSleepThreshold = m_fSleepThreshold_Mesh;
					}

					///* �޽� ��ƼŬ ���̵� �ξƿ� ��� */
					//ImGui::SeparatorText(" Fade Type ");
					//ImGui::RadioButton("FADE_NONE_Mesh", &m_iType_Fade_Mesh, 0);
					//ImGui::RadioButton("FADE_OUT_Mesh", &m_iType_Fade_Mesh, 1);
					//ImGui::RadioButton("FADE_IN_Mesh", &m_iType_Fade_Mesh, 2);
					//if (0 == m_iType_Fade_Mesh)
					//	m_pMeshBufferDesc->eType_Fade = CVIBuffer_Effect_Model_Instance::TYPE_FADE::FADE_NONE;
					//else if (1 == m_iType_Fade_Mesh)
					//	m_pMeshBufferDesc->eType_Fade = CVIBuffer_Effect_Model_Instance::TYPE_FADE::FADE_OUT;
					//else if (2 == m_iType_Fade_Mesh)
					//	m_pMeshBufferDesc->eType_Fade = CVIBuffer_Effect_Model_Instance::TYPE_FADE::FADE_IN;


					///* �޽� ��ƼŬ ���̵� �ξƿ� ���� */
					//ImGui::SeparatorText(u8" Fade ���� ");
					//ImGui::RadioButton(u8"������Ÿ��_Mesh", &m_iType_Fade_Takes_Mesh, 0);
					//ImGui::RadioButton(u8"�Ÿ�_Mesh", &m_iType_Fade_Takes_Mesh, 1);
					//ImGui::RadioButton(u8"����_Mesh", &m_iType_Fade_Takes_Mesh, 2);
					//ImGui::RadioButton(u8"ũ��_Mesh", &m_iType_Fade_Takes_Mesh, 3);
					//ImGui::RadioButton(u8"TYPE_FADE_TAKES_END_Mesh", &m_iType_Fade_Takes_Mesh, 4);
					//if (0 == m_iType_Fade_Takes_Mesh)
					//	m_pMeshBufferDesc->eType_Fade_Takes = CVIBuffer_Effect_Model_Instance::TYPE_FADE_TAKES::LIFE;
					//else if (1 == m_iType_Fade_Takes_Mesh)
					//	m_pMeshBufferDesc->eType_Fade_Takes = CVIBuffer_Effect_Model_Instance::TYPE_FADE_TAKES::DIST;
					//else if (2 == m_iType_Fade_Takes_Mesh)
					//	m_pMeshBufferDesc->eType_Fade_Takes = CVIBuffer_Effect_Model_Instance::TYPE_FADE_TAKES::HEIGHT;
					//else if (3 == m_iType_Fade_Takes_Mesh)
					//	m_pMeshBufferDesc->eType_Fade_Takes = CVIBuffer_Effect_Model_Instance::TYPE_FADE_TAKES::SCALE;
					//else if (4 == m_iType_Fade_Takes_Mesh)
					//	m_pMeshBufferDesc->eType_Fade_Takes = CVIBuffer_Effect_Model_Instance::TYPE_FADE_TAKES::TYPE_FADE_TAKES_END;



					/* �޽� ������ٵ� ���� ���� (�޽� ��ƼŬ ���̵� �ξƿ� ���� ��Ȱ��)*/
					ImGui::SeparatorText(u8" ������ٵ� ���� ���� ");
					ImGui::RadioButton(u8"������Ÿ��_Mesh", &m_iType_Fade_Takes_Mesh, 0);
					ImGui::RadioButton(u8"�Ÿ�_Mesh", &m_iType_Fade_Takes_Mesh, 1);
					//ImGui::RadioButton(u8"����_Mesh", &m_iType_Fade_Takes_Mesh, 2);
					//ImGui::RadioButton(u8"ũ��_Mesh", &m_iType_Fade_Takes_Mesh, 3);
					ImGui::RadioButton(u8"TYPE_RIGID_DEAD_END_Mesh", &m_iType_Fade_Takes_Mesh, 4);
					if (0 == m_iType_Fade_Takes_Mesh)
						m_pMeshBufferDesc->eType_Fade_Takes = CVIBuffer_Effect_Model_Instance::TYPE_FADE_TAKES::LIFE;
					else if (1 == m_iType_Fade_Takes_Mesh)
						m_pMeshBufferDesc->eType_Fade_Takes = CVIBuffer_Effect_Model_Instance::TYPE_FADE_TAKES::DIST;
					//else if (2 == m_iType_Fade_Takes_Mesh)
					//	m_pMeshBufferDesc->eType_Fade_Takes = CVIBuffer_Effect_Model_Instance::TYPE_FADE_TAKES::HEIGHT;
					//else if (3 == m_iType_Fade_Takes_Mesh)
					//	m_pMeshBufferDesc->eType_Fade_Takes = CVIBuffer_Effect_Model_Instance::TYPE_FADE_TAKES::SCALE;
					else if (4 == m_iType_Fade_Takes_Mesh)
						m_pMeshBufferDesc->eType_Fade_Takes = CVIBuffer_Effect_Model_Instance::TYPE_FADE_TAKES::TYPE_FADE_TAKES_END;


					ImGui::SeparatorText("");
				}

			}
#pragma endregion ������ٵ� �ɼ� ����_�޽� ��ƼŬ ��



#pragma region ũ�� ���� ����_�޽� ��ƼŬ ����
				if (ImGui::CollapsingHeader(" Scale_Mesh "))
				{
					if (1 == m_iUseScaleLerp_Mesh)
					{
						// ũ�ⷯ�� �Ⱦ� ���� ������ �������� ����_�޽� ��ƼŬ
						ImGui::SeparatorText(" ScaleRatio_Mesh ");
						ImGui::RadioButton(u8"ũ�� ���� ����_Mesh", &m_iScaleRatio_Mesh, 0);
						ImGui::RadioButton(u8"ũ�� ���� ����_Mesh", &m_iScaleRatio_Mesh, 1); 
						if (0 == m_iScaleRatio_Mesh)
							m_pMeshBufferDesc->bScaleRatio = TRUE;
						else if (1 == m_iScaleRatio_Mesh)
							m_pMeshBufferDesc->bScaleRatio = FALSE;
					}

					ImGui::SeparatorText("Start Scale_Mesh");
					if (ImGui::DragFloat3("Start Scale_Mesh", m_vStartScale_Mesh, 0.1f, 0.f, 5000.f))
					{
						m_pMeshBufferDesc->vStartScale.x = m_vStartScale_Mesh[0];
						m_pMeshBufferDesc->vStartScale.y = m_vStartScale_Mesh[1];
						m_pMeshBufferDesc->vStartScale.z = m_vStartScale_Mesh[2];
					}

					ImGui::SeparatorText("End Scale_Mesh");
					if (ImGui::DragFloat3("End Scale_Mesh", m_vEndScale_Mesh, 0.1f, 0.f, 5000.f))
					{
						m_pMeshBufferDesc->vEndScale.x = m_vEndScale_Mesh[0];
						m_pMeshBufferDesc->vEndScale.y = m_vEndScale_Mesh[1];
						m_pMeshBufferDesc->vEndScale.z = m_vEndScale_Mesh[2];
					}


					/* �޽� ��ƼŬ ũ�� ���� ���� ��� */
					ImGui::SeparatorText(" Scale Mode_Mesh ");
					ImGui::RadioButton(u8"ũ�� ���� ���_Mesh", &m_iUseScaleLerp_Mesh, 0);
					ImGui::RadioButton(u8"ũ�� ���� ���_Mesh", &m_iUseScaleLerp_Mesh, 1);
					if (0 == m_iUseScaleLerp_Mesh)
						m_pMeshBufferDesc->bUseScaleLerp = TRUE;
					else if (1 == m_iUseScaleLerp_Mesh)
						m_pMeshBufferDesc->bUseScaleLerp = FALSE;

					if (TRUE == m_pMeshBufferDesc->bUseScaleLerp)
					{
						ImGui::SeparatorText("ScaleLerp Pos_Mesh"); ImGui::SameLine(); HelpMarker(u8"0~1�� ������ ������Ÿ�� ���� ũ�Ⱑ ��ȭ�� ���� ����, �� ����");

						if (ImGui::DragFloat2("Up ScaleLerp Pos_Mesh", m_vScaleLerp_Up_Pos_Mesh, 0.1f, 0.f, 1.f))
						{
							if (m_vScaleLerp_Up_Pos_Mesh[0] > m_vScaleLerp_Up_Pos_Mesh[1])	// Min�� Max���� ũ�� Max�� Min����
								m_vScaleLerp_Up_Pos_Mesh[1] = m_vScaleLerp_Up_Pos_Mesh[0];

							m_pMeshBufferDesc->vScaleLerp_Up_Pos.x = m_vScaleLerp_Up_Pos_Mesh[0];
							m_pMeshBufferDesc->vScaleLerp_Up_Pos.y = m_vScaleLerp_Up_Pos_Mesh[1];
						}

						if (ImGui::DragFloat2("Down ScaleLerp Pos_Mesh", m_vScaleLerp_Down_Pos_Mesh, 0.1f, 0.f, 1.f))
						{
							if (m_vScaleLerp_Down_Pos_Mesh[0] > m_vScaleLerp_Down_Pos_Mesh[1])	// Min�� Max���� ũ�� Max�� Min����
								m_vScaleLerp_Down_Pos_Mesh[1] = m_vScaleLerp_Down_Pos_Mesh[0];

							m_pMeshBufferDesc->vScaleLerp_Down_Pos.x = m_vScaleLerp_Down_Pos_Mesh[0];
							m_pMeshBufferDesc->vScaleLerp_Down_Pos.y = 1.f; // 1�� ����
						}

						if (ImGui::TreeNode(" Scale_Easing Types "))
						{
							Select_EasingType(&m_pMeshBufferDesc->eType_ScaleLerp);

							ImGui::TreePop();
						}
					}

					ImGui::SeparatorText("");
				}
#pragma endregion ũ�� ���� ����_�޽� ��ƼŬ ��


#pragma region ȸ��_�޽� ��ƼŬ ����
					if (ImGui::CollapsingHeader(" Rotation_Mesh "))
					{
						ImGui::SeparatorText("Dir To Axis_Mesh"); ImGui::SameLine(); HelpMarker(u8"���⺤�Ͱ� �ش� ���̵˴ϴ�.");
						ImGui::RadioButton("DIR_RIGHT_Mesh", &m_iType_Dir_Mesh, 0);
						ImGui::RadioButton("DIR_UP_Mesh ", &m_iType_Dir_Mesh, 1);
						ImGui::RadioButton("DIR_LOOK_Mesh", &m_iType_Dir_Mesh, 2);

						if (0 == m_iType_Dir_Mesh)
							m_pMeshBufferDesc->eType_Dir = CVIBuffer_Effect_Model_Instance::DIR_RIGHT;
						else if (1 == m_iType_Dir_Mesh)
							m_pMeshBufferDesc->eType_Dir = CVIBuffer_Effect_Model_Instance::DIR_UP;
						else if (2 == m_iType_Dir_Mesh)
							m_pMeshBufferDesc->eType_Dir = CVIBuffer_Effect_Model_Instance::DIR_LOOK;


						/* ȸ�� ����(������) */
						/* RotX (Arc_X) */
						ImGui::SeparatorText(u8"���⺤�� ȸ������");
						if (ImGui::DragFloat2("Arc_X_Mesh", m_vRotationOffsetX_Mesh, 1.f, 0.f, 360.f))
						{
							if (m_vRotationOffsetX_Mesh[0] > m_vRotationOffsetX_Mesh[1])	// Min�� Max���� ũ�� Max�� Min����
								m_vRotationOffsetX_Mesh[1] = m_vRotationOffsetX_Mesh[0];


							m_pMeshBufferDesc->vMinMaxRotationOffsetX.x = m_vRotationOffsetX_Mesh[0];
							m_pMeshBufferDesc->vMinMaxRotationOffsetX.y = m_vRotationOffsetX_Mesh[1];

						}

						/* RotY (Arc_Y) */
						if (ImGui::DragFloat2("Arc_Y_Mesh", m_vRotationOffsetY_Mesh, 1.f, 0.f, 360.f))
						{
							if (m_vRotationOffsetY_Mesh[0] > m_vRotationOffsetY_Mesh[1])	// Min�� Max���� ũ�� Max�� Min����
								m_vRotationOffsetY_Mesh[1] = m_vRotationOffsetY_Mesh[0];


							m_pMeshBufferDesc->vMinMaxRotationOffsetY.x = m_vRotationOffsetY_Mesh[0];
							m_pMeshBufferDesc->vMinMaxRotationOffsetY.y = m_vRotationOffsetY_Mesh[1];
						}

						/* RotZ (Arc_Z) */
						if (ImGui::DragFloat2("Arc_Z_Mesh", m_vRotationOffsetZ_Mesh, 1.f, 0.f, 360.f))
						{
							if (m_vRotationOffsetZ_Mesh[0] > m_vRotationOffsetZ_Mesh[1])	// Min�� Max���� ũ�� Max�� Min����
								m_vRotationOffsetZ_Mesh[1] = m_vRotationOffsetZ_Mesh[0];

							m_pMeshBufferDesc->vMinMaxRotationOffsetZ.x = m_vRotationOffsetZ_Mesh[0];
							m_pMeshBufferDesc->vMinMaxRotationOffsetZ.y = m_vRotationOffsetZ_Mesh[1];
						}


						ImGui::SeparatorText(u8"�ν��Ͻ� ȸ��");
						if (ImGui::DragFloat(" Radian_X_Mesh ", &m_vRadian_Mesh[0], 1.f, 0.f, 360.f))
							m_pMeshBufferDesc->vRadian.x = m_vRadian_Mesh[0];

						if (ImGui::DragFloat(" Radian_Y_Mesh ", &m_vRadian_Mesh[1], 1.f, 0.f, 360.f))
							m_pMeshBufferDesc->vRadian.y = m_vRadian_Mesh[1];

						if (ImGui::DragFloat(" Radian_Z_Mesh ", &m_vRadian_Mesh[2], 1.f, 0.f, 360.f))
							m_pMeshBufferDesc->vRadian.z = m_vRadian_Mesh[2];

						ImGui::SeparatorText("");
					}
#pragma endregion ȸ��_�޽� ��ƼŬ ��




#pragma region �� ����_�޽�
				/* �� ���� */
				if (ImGui::CollapsingHeader(" Color_Mesh "))
				{
					/* ��ǻ�� ����ȥ�� ���_Mesh */
					ImGui::SeparatorText("Color Blend Mode_Mesh");
					ImGui::RadioButton(u8"���ϱ�_Mesh", &m_iColor_Mode_Mesh, 0);
					ImGui::RadioButton(u8"��ũ��_Mesh", &m_iColor_Mode_Mesh, 1);
					ImGui::RadioButton(u8"��������_Mesh", &m_iColor_Mode_Mesh, 2);
					ImGui::RadioButton(u8"���ϱ�_Mesh", &m_iColor_Mode_Mesh, 3);
					ImGui::RadioButton(u8"��_Mesh", &m_iColor_Mode_Mesh, 4);
					ImGui::RadioButton(u8"���� ����Ʈ_Mesh", &m_iColor_Mode_Mesh, 5);
					ImGui::RadioButton(u8"����Ʈ ����Ʈ_Mesh", &m_iColor_Mode_Mesh, 6);
					ImGui::RadioButton(u8"�ϵ� ����Ʈ_Mesh", &m_iColor_Mode_Mesh, 7);
					ImGui::RadioButton(u8"�÷� ����_Mesh", &m_iColor_Mode_Mesh, 8);
					ImGui::RadioButton(u8"ȥ�� ��_Mesh", &m_iColor_Mode_Mesh, 9);
					ImGui::RadioButton(u8"ȥ�վ���_Mesh", &m_iColor_Mode_Mesh, 10);

					if (0 == m_iColor_Mode_Mesh)
						m_pCurVoidDesc->eMode_Color = MODE_COLOR::MUL;
					else if (1 == m_iColor_Mode_Mesh)
						m_pCurVoidDesc->eMode_Color = MODE_COLOR::SCREEN;
					else if (2 == m_iColor_Mode_Mesh)
						m_pCurVoidDesc->eMode_Color = MODE_COLOR::OVERLAY;
					else if (3 == m_iColor_Mode_Mesh)
						m_pCurVoidDesc->eMode_Color = MODE_COLOR::ADD;
					else if (4 == m_iColor_Mode_Mesh)
						m_pCurVoidDesc->eMode_Color = MODE_COLOR::BURN;
					else if (5 == m_iColor_Mode_Mesh)
						m_pCurVoidDesc->eMode_Color = MODE_COLOR::VIVID_RIGHT;
					else if (6 == m_iColor_Mode_Mesh)
						m_pCurVoidDesc->eMode_Color = MODE_COLOR::SOFT_RIGHT;
					else if (7 == m_iColor_Mode_Mesh)
						m_pCurVoidDesc->eMode_Color = MODE_COLOR::HARD_RIGHT;
					else if (8 == m_iColor_Mode_Mesh)
						m_pCurVoidDesc->eMode_Color = MODE_COLOR::COLOR_DODGE;
					else if (9 == m_iColor_Mode_Mesh)
						m_pCurVoidDesc->eMode_Color = MODE_COLOR::MIX_BURN;
					else if (10 == m_iColor_Mode_Mesh)
						m_pCurVoidDesc->eMode_Color = MODE_COLOR::MODE_COLOR_END;


					/* ���ڸ��� �ٸ� �ֱ�� �� ���� Ű�� ���� */
					ImGui::SeparatorText("Color Lerp_Mesh");
					ImGui::RadioButton(" ON   Dynamic_Color_Mesh ", &m_iDynamic_Color_Mesh, 0);  ImGui::SameLine(); HelpMarker(u8"���ڵ� ������ ������Ÿ�ӿ� ���� �÷� ����");
					ImGui::RadioButton(" OFF  Dynamic_Color_Mesh ", &m_iDynamic_Color_Mesh, 1);	 ImGui::SameLine(); HelpMarker(u8"��ü �ϳ��� ������Ÿ�ӿ� ���� �÷� ����");
					if (0 == m_iDynamic_Color_Mesh)
						m_pMeshBufferDesc->bDynamic_Color = TRUE;
					else if (1 == m_iDynamic_Color_Mesh)
						m_pMeshBufferDesc->bDynamic_Color = FALSE;


					/* Min �� ����_��ƼŬ */
					if (ImGui::ColorEdit4("Min_Color_Mesh", m_fColor_Min_Mesh, ImGuiColorEditFlags_None))
					{
						m_pMeshBufferDesc->vMinMaxRed.x = m_fColor_Min_Mesh[0];
						m_pMeshBufferDesc->vMinMaxGreen.x = m_fColor_Min_Mesh[1];
						m_pMeshBufferDesc->vMinMaxBlue.x = m_fColor_Min_Mesh[2];
					}

					/* Max �� ����_��ƼŬ */
					if (ImGui::ColorEdit4("Max_Color_Mesh", m_fColor_Max_Mesh, ImGuiColorEditFlags_None))
					{
						m_pMeshBufferDesc->vMinMaxRed.y = m_fColor_Max_Mesh[0];
						m_pMeshBufferDesc->vMinMaxGreen.y = m_fColor_Max_Mesh[1];
						m_pMeshBufferDesc->vMinMaxBlue.y = m_fColor_Max_Mesh[2];
					}

					if (1 == m_iDynamic_Color_Mesh) // ���� �� �ϰ������̸� ���� �� ��ȭ �����ֱ�
					{
						ImGui::ColorEdit4("Cur_Color_Mesh", m_fColor_Cur_Mesh, ImGuiColorEditFlags_None);
						m_fColor_Cur_Mesh[0] = m_pMeshBufferDesc->vCurrentColor.x;
						m_fColor_Cur_Mesh[1] = m_pMeshBufferDesc->vCurrentColor.y;
						m_fColor_Cur_Mesh[2] = m_pMeshBufferDesc->vCurrentColor.z;
					}


					if (ImGui::TreeNode(" Color_Easing Types "))
					{
						ImGui::Text("Selected Type : %d", m_pMeshBufferDesc->eType_ColorLerp);

						Select_EasingType(&m_pMeshBufferDesc->eType_ColorLerp);

						ImGui::TreePop();
					}


					/* ���� ����ġ */
					ImGui::SeparatorText(u8"���� ����ġ_Mesh");
					if (ImGui::DragFloat2("Add Alpha_Mesh", m_fMinMaxAlpha_Mesh, 0.1f, -255.f, 255.f))
					{
						m_pMeshBufferDesc->vMinMaxAlpha.x = m_fMinMaxAlpha_Mesh[0];
						m_pMeshBufferDesc->vMinMaxAlpha.y = m_fMinMaxAlpha_Mesh[1];
					}


					ImGui::SeparatorText("");
				}
#pragma endregion �� ����_�޽� ��

				/* Rim & Bloom */
				if (ImGui::CollapsingHeader(" Rim Bloom_Mesh "))
				{
					ImGui::SeparatorText("RimLight_Mesh");
					if (ImGui::ColorEdit4("RimColor", m_fRimColor_Mesh, ImGuiColorEditFlags_None))
					{
						m_pCurVoidDesc->vRimColor.x = m_fRimColor_Mesh[0];
						m_pCurVoidDesc->vRimColor.y = m_fRimColor_Mesh[1];
						m_pCurVoidDesc->vRimColor.z = m_fRimColor_Mesh[2];
						m_pCurVoidDesc->vRimColor.w = m_fRimColor_Mesh[3];
					}

					if (ImGui::DragFloat("RimPower", &m_fRimPower_Mesh, 1.f, 0.f, 5000.f))
					{
						m_pCurVoidDesc->fRimPower = m_fRimPower_Mesh;
					}

					ImGui::SeparatorText("Bloom_Mesh");
					if (ImGui::ColorEdit3("BloomPower", m_vBloomPower_Mesh, ImGuiColorEditFlags_None))
					{
						m_pCurVoidDesc->vBloomPower.x = m_vBloomPower_Mesh[0];
						m_pCurVoidDesc->vBloomPower.y = m_vBloomPower_Mesh[1];
						m_pCurVoidDesc->vBloomPower.z = m_vBloomPower_Mesh[2];
					}

					ImGui::SeparatorText("");
				}



				/* ������ �� Ȯ�� */
				if (ImGui::CollapsingHeader(" Dissolve_Mesh "))
				{
					ImGui::SeparatorText(" DissolveAmount_Mesh ");
					ImGui::SliderFloat(" DissolveAmount_Mesh ", &m_pCurVoidDesc->fDissolveAmount, 0.f, 1.f);

					if (ImGui::TreeNode(" Dissolve_Easing Types "))
					{
						ImGui::Text("Selected Type : %d", m_pCurVoidDesc->eType_Easing);
						Select_EasingType(&m_pCurVoidDesc->eType_Easing);

						ImGui::TreePop();
					}

					ImGui::SeparatorText("");
				}



				// ����� �� ����
				if (ImGui::CollapsingHeader(" Distortion_Mesh "))
				{
					CEffect_Void::DISTORTION_DESC* pDistortionDesc = dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Get_Distortion_Desc();
					if (ImGui::TreeNode(" Distortion Preset "))
					{
						if (ImGui::Button("  ZERO  "))
						{
							pDistortionDesc->eType_Scroll = { CEffect_Void::TYPE_SCROLL_END };

							pDistortionDesc->vScrollSpeeds = { 0.f, 0.f, 0.f };
							pDistortionDesc->vScales = { 0.f, 0.f, 0.f };

							pDistortionDesc->vDistortion1 = { 0.f, 0.f };
							pDistortionDesc->vDistortion2 = { 0.f, 0.f };
							pDistortionDesc->vDistortion3 = { 0.f, 0.f };

							pDistortionDesc->fDistortionScale = { 0.f };
							pDistortionDesc->fDistortionBias = { 0.f };

							Update_CurParameters_Parts();

							
						}

						if (ImGui::Button("   FIRE   "))
						{
							pDistortionDesc->eType_Scroll = { CEffect_Void::SCROLL_COL };

							pDistortionDesc->vScrollSpeeds = { 1.f, 1.f, 1.f };
							pDistortionDesc->vScales = { 1.f, 1.f, 1.f };

							pDistortionDesc->vDistortion1 = { 0.1f, 0.1f };
							pDistortionDesc->vDistortion2 = { 0.f, 0.f };
							pDistortionDesc->vDistortion3 = { 0.f, 0.1f };

							pDistortionDesc->fDistortionScale = { 1.f };
							pDistortionDesc->fDistortionBias = { 1.f };

							Update_CurParameters_Parts();
						}

						ImGui::TreePop();
					}

					/* ����� ��ũ�� ���� ���� */
					ImGui::SeparatorText("Scroll_Distortion_Mesh");
					ImGui::RadioButton("Row_Scroll_Mesh", &m_iType_Scroll_Mesh, 0);  ImGui::SameLine();
					ImGui::RadioButton("Col_Scroll_Mesh", &m_iType_Scroll_Mesh, 1);
					ImGui::RadioButton("Both_Scroll_Mesh", &m_iType_Scroll_Mesh, 2);
					ImGui::RadioButton("Rotate_Scroll_Mesh", &m_iType_Scroll_Mesh, 3);
					ImGui::RadioButton("End_Scroll_Mesh", &m_iType_Scroll_Mesh, 4);
					if (0 == m_iType_Scroll_Mesh)
						pDistortionDesc->eType_Scroll = { CEffect_Void::SCROLL_ROW };
					else if (1 == m_iType_Scroll_Mesh)
						pDistortionDesc->eType_Scroll = { CEffect_Void::SCROLL_COL };
					else if (2 == m_iType_Scroll_Mesh)
						pDistortionDesc->eType_Scroll = { CEffect_Void::SCROLL_BOTH };
					else if (3 == m_iType_Scroll_Mesh)
						pDistortionDesc->eType_Scroll = { CEffect_Void::SCROLL_ROTATE };
					else if (4 == m_iType_Scroll_Mesh)
						pDistortionDesc->eType_Scroll = { CEffect_Void::TYPE_SCROLL_END };


					ImGui::SeparatorText("Values_Distortion_Mesh");
					if (ImGui::DragFloat3("ScrollSpeeds", m_vScrollSpeeds_Mesh, 0.1f, -100.f))
					{
						pDistortionDesc->vScrollSpeeds.x = m_vScrollSpeeds_Mesh[0];
						pDistortionDesc->vScrollSpeeds.y = m_vScrollSpeeds_Mesh[1];
						pDistortionDesc->vScrollSpeeds.z = m_vScrollSpeeds_Mesh[2];
					}
					if (ImGui::DragFloat3("Distortion_Scales", m_vScales_Distortion_Mesh, 0.1f, 0.f))
					{
						pDistortionDesc->vScales.x = m_vScales_Distortion_Mesh[0];
						pDistortionDesc->vScales.y = m_vScales_Distortion_Mesh[1];
						pDistortionDesc->vScales.z = m_vScales_Distortion_Mesh[2];
					}
					if (ImGui::DragFloat2("Distortion1", m_vDistortion1_Mesh, 0.1f, 0.f))
					{
						pDistortionDesc->vDistortion1.x = m_vDistortion1_Mesh[0];
						pDistortionDesc->vDistortion1.y = m_vDistortion1_Mesh[1];
					}
					if (ImGui::DragFloat2("Distortion2", m_vDistortion2_Mesh, 0.1f, 0.f))
					{
						pDistortionDesc->vDistortion2.x = m_vDistortion2_Mesh[0];
						pDistortionDesc->vDistortion2.y = m_vDistortion2_Mesh[1];
					}
					if (ImGui::DragFloat2("Distortion3", m_vDistortion3_Mesh, 0.1f, 0.f))
					{
						pDistortionDesc->vDistortion3.x = m_vDistortion3_Mesh[0];
						pDistortionDesc->vDistortion3.y = m_vDistortion3_Mesh[1];
					}
					if (ImGui::DragFloat("Distortion_Scale", &m_fDistortionScale_Mesh, 1.f, 0.f))
					{
						pDistortionDesc->fDistortionScale = m_fDistortionScale_Mesh;
					}
					if (ImGui::DragFloat("DistortionBias", &m_fDistortionBias_Mesh, 1.f, 0.f))
					{
						pDistortionDesc->fDistortionBias = m_fDistortionBias_Mesh;
					}

					ImGui::SeparatorText("");
				}


			}
		}
	}
}

void CWindow_EffectTool::Update_TrailTab(_float fTimeDelta)
{
	auto& style = ImGui::GetStyle();


	if (nullptr != m_pTrail)
	{

#pragma region Desc ������ ������Ʈ_Ʈ����
		// Ʈ������ nullptr�� �ƴ� �� Ʈ���� ���� Desc�� ������
		m_pVoidTrailDesc = m_pTrail->Get_Desc();
		m_pTrailDesc = m_pTrail->Get_TrailDesc();
		CVIBuffer_Trail* pVIBuffer = m_pTrail->Get_VIBufferCom();
		m_pTrailBufferDesc = pVIBuffer->Get_Desc();
#pragma endregion 

		/* �̸� */
		ImGui::SeparatorText("NAME");
		ImGui::Text(m_pGameInstance->ConverWStringtoC(m_pVoidTrailDesc->strPartTag));
		ImGui::SeparatorText("");

#pragma region �ؽ�ó ����_Ʈ����
		/* �ؽ�ó ���� */
		if (ImGui::CollapsingHeader("Texture_Trail"))
		{
			if (ImGui::TreeNode("Diffuse_Trail"))
			{

				if (ImGui::InputInt("Diffuse_Trail", &m_iTexIndex_Trail[CEffect_Void::TEXTURE_DIFFUSE], 1))
				{
					if (m_iMaxTexIndex_Trail[CEffect_Void::TEXTURE_DIFFUSE] <= m_iTexIndex_Trail[CEffect_Void::TEXTURE_DIFFUSE])
						m_iTexIndex_Trail[CEffect_Void::TEXTURE_DIFFUSE] = m_iMaxTexIndex_Trail[CEffect_Void::TEXTURE_DIFFUSE] - 1;

					if (0 > m_iTexIndex_Trail[CEffect_Void::TEXTURE_DIFFUSE])
						m_iTexIndex_Trail[CEffect_Void::TEXTURE_DIFFUSE] = 0;

					m_pVoidTrailDesc->iTextureIndex[CEffect_Void::TEXTURE_DIFFUSE] = m_iTexIndex_Trail[CEffect_Void::TEXTURE_DIFFUSE];
				}

				ImGui::SeparatorText("");
				ImGui::TreePop();

			}


			if (ImGui::TreeNode("Mask_Trail"))
			{

				if (ImGui::InputInt("Mask_Trail", &m_iTexIndex_Trail[CEffect_Void::TEXTURE_MASK], 1))
				{
					if (m_iMaxTexIndex_Trail[CEffect_Void::TEXTURE_MASK] <= m_iTexIndex_Trail[CEffect_Void::TEXTURE_MASK])
						m_iTexIndex_Trail[CEffect_Void::TEXTURE_MASK] = m_iMaxTexIndex_Trail[CEffect_Void::TEXTURE_MASK] - 1;

					if (0 > m_iTexIndex_Trail[CEffect_Void::TEXTURE_MASK])
						m_iTexIndex_Trail[CEffect_Void::TEXTURE_MASK] = 0;

					m_pVoidTrailDesc->iTextureIndex[CEffect_Void::TEXTURE_MASK] = m_iTexIndex_Trail[CEffect_Void::TEXTURE_MASK];
				}


				ImGui::SeparatorText("");
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Noise_Trail"))
			{


				if (ImGui::InputInt("Noise_Trail", &m_iTexIndex_Trail[CEffect_Void::TEXTURE_NOISE], 1))
				{
					if (m_iMaxTexIndex_Trail[CEffect_Void::TEXTURE_NOISE] <= m_iTexIndex_Trail[CEffect_Void::TEXTURE_NOISE])
						m_iTexIndex_Trail[CEffect_Void::TEXTURE_NOISE] = m_iMaxTexIndex_Trail[CEffect_Void::TEXTURE_NOISE] - 1;

					if (0 > m_iTexIndex_Trail[CEffect_Void::TEXTURE_NOISE])
						m_iTexIndex_Trail[CEffect_Void::TEXTURE_NOISE] = 0;

					m_pVoidTrailDesc->iTextureIndex[CEffect_Void::TEXTURE_NOISE] = m_iTexIndex_Trail[CEffect_Void::TEXTURE_NOISE];
				}

				ImGui::SeparatorText("");
				ImGui::TreePop();
			}

			ImGui::SeparatorText("");
		}
#pragma endregion �ؽ�ó ����_Ʈ���� ��


		/* UV �� ���� */
		if (ImGui::CollapsingHeader("UV Option_Trail"))
		{



			ImGui::SeparatorText("");
		}


		if (ImGui::CollapsingHeader("Shader & RenderGroup_Trail"))
		{
			/* ���̴� �н� �ε��� ���� */
			if (ImGui::InputInt("Shader Pass_Trail", &m_iShaderPassIndex_Trail, 1))
			{
				if (m_iMaxShaderPassIndex_Trail < m_iShaderPassIndex_Trail)
					m_iShaderPassIndex_Trail = m_iMaxShaderPassIndex_Trail;

				if (0 > m_iShaderPassIndex_Trail)
					m_iShaderPassIndex_Trail = 0;

				m_pVoidTrailDesc->iShaderPassIndex = m_iShaderPassIndex_Trail;
			}


			/* ���̴��� ���� ��ī�� �� ���� */
			ImGui::SeparatorText("Discard_Value");
			if (ImGui::DragFloat4("Discard_Trail", m_vColor_Clip_Trail, 0.1f, 0.f, 1.f))
			{
				m_pVoidTrailDesc->vColor_Clip.x = m_vColor_Clip_Trail[0];
				m_pVoidTrailDesc->vColor_Clip.y = m_vColor_Clip_Trail[1];
				m_pVoidTrailDesc->vColor_Clip.z = m_vColor_Clip_Trail[2];
				m_pVoidTrailDesc->vColor_Clip.w = m_vColor_Clip_Trail[3];
			}

			///* �����׷� ���� */
			//ImGui::SeparatorText("");
			//if (ImGui::InputInt("Render Group_Trail", &m_iRenderGroup_Trail, 1))
			//{
			//	if ((_int)CRenderer::RENDER_END < m_iRenderGroup_Trail)
			//	{
			//		m_iRenderGroup_Trail = (_int)CRenderer::RENDER_END - 1;
			//	}
			//	m_pVoidTrailDesc->iRenderGroup = m_iRenderGroup_Trail;
			//}


			ImGui::SeparatorText("");
		}


#pragma region ���� Desc �� ����_Ʈ����

		if (ImGui::CollapsingHeader("Trail_Buffer"))
		{
			if (ImGui::TreeNode("Trail_Buffer_Preset"))
			{
				if (ImGui::Button("Boss_Trail_Preset"))
				{
					m_pTrailBufferDesc->vPos_0 = { 0.f, 0.f, -0.5f };
					m_pTrailBufferDesc->vPos_0 = { 0.f, 0.f, 0.5f };
					m_pTrailBufferDesc->iLerpPointNum = { 12 };
				}

				ImGui::TreePop();
				ImGui::SeparatorText("");
			}

			if (ImGui::DragFloat3("Pos_0", m_vPos_0, 0.1f, -100.f, 100.f))
			{
				m_pTrailBufferDesc->vPos_0.x = m_vPos_0[0];
				m_pTrailBufferDesc->vPos_0.y = m_vPos_0[1];
				m_pTrailBufferDesc->vPos_0.z = m_vPos_0[2];
			}

			if (ImGui::DragFloat3("Pos_1", m_vPos_1, 0.1f, -100.f, 100.f))
			{
				m_pTrailBufferDesc->vPos_1.x = m_vPos_1[0];
				m_pTrailBufferDesc->vPos_1.y = m_vPos_1[1];
				m_pTrailBufferDesc->vPos_1.z = m_vPos_1[2];
			}

			//if (ImGui::DragInt("iMaxCnt_Trail", &m_iMaxCnt_Trail, 1, 2.f, 100.f))
			//{
			//	m_pTrailBufferDesc->iMaxCnt = m_iMaxCnt_Trail;
			//}

			if (ImGui::DragInt("iLerpPointNum_Trail", &m_iLerpPointNum, 1, 2, 100))
			{
				m_pTrailBufferDesc->iLerpPointNum = m_iLerpPointNum;
			}

			ImGui::SeparatorText("");
		}
#pragma endregion ���� Desc �� ����_Ʈ���� ��


		if (ImGui::CollapsingHeader("Color_Trail"))
		{
			/* ���̴��� ���� ���ϱ� �÷� ��_Ʈ���� */
			if (ImGui::ColorEdit4("Color_Mul_Trail", m_fColor_Mul_Trail, ImGuiColorEditFlags_None))
			{
				m_pVoidTrailDesc->vColor_Mul.x = m_fColor_Mul_Trail[0];
				m_pVoidTrailDesc->vColor_Mul.y = m_fColor_Mul_Trail[1];
				m_pVoidTrailDesc->vColor_Mul.z = m_fColor_Mul_Trail[2];
				m_pVoidTrailDesc->vColor_Mul.w = m_fColor_Mul_Trail[3];
			}

			ImGui::SeparatorText("");
		}

#pragma region Rim & Bloom �� ����_Ʈ����

		/* ������Ʈ & ��� */
		if (ImGui::CollapsingHeader("Rim Bloom_Trail"))
		{
			if (ImGui::ColorEdit4("RimColor_Trail", m_fRimColor_Trail, ImGuiColorEditFlags_None))
			{
				m_pVoidTrailDesc->vRimColor.x = m_fRimColor_Trail[0];
				m_pVoidTrailDesc->vRimColor.y = m_fRimColor_Trail[1];
				m_pVoidTrailDesc->vRimColor.z = m_fRimColor_Trail[2];
				m_pVoidTrailDesc->vRimColor.w = m_fRimColor_Trail[3];
			}

			if (ImGui::DragFloat("RimPower_Trail", &m_fRimPower_Trail, 1.f, 0.f, 5000.f))
			{
				m_pVoidTrailDesc->fRimPower = m_fRimPower_Trail;
			}

			if (ImGui::ColorEdit3("BloomPower_Trail", m_vBloomPower_Trail, ImGuiColorEditFlags_None))
			{
				m_pVoidTrailDesc->vBloomPower.x = m_vBloomPower_Trail[0];
				m_pVoidTrailDesc->vBloomPower.y = m_vBloomPower_Trail[1];
				m_pVoidTrailDesc->vBloomPower.z = m_vBloomPower_Trail[2];
			}

			ImGui::SeparatorText("");
		}


	} // nullptr != m_pTrail

}


void CWindow_EffectTool::Update_CurParameters_Parts()
{
	//! ��ü�� ���� ������ ������ Ȯ���Ҽ� �ֵ���
	//! �� ���� ��� = ��ü�� ������� ����

	if (nullptr != m_pCurPartEffect)
	{
		m_pCurVoidDesc = m_pCurPartEffect->Get_Desc();	// ����Ʈ_���̵� Desc

		CEffect_Void::TYPE_EFFECT eType_Effect = m_pCurVoidDesc->eType_Effect;
		CTransform* pPartTransform = m_pCurPartEffect->Get_Transform();

		m_vTimes_Part[0] = m_pCurVoidDesc->fWaitingTime;
		m_vTimes_Part[1] = m_pCurVoidDesc->fLifeTime;
		m_vTimes_Part[2] = m_pCurVoidDesc->fRemainTime;

		_float4 vPos = pPartTransform->Get_State(CTransform::STATE_POSITION);
		m_vWorldPosition_Part[0] = vPos.x;
		m_vWorldPosition_Part[1] = vPos.y;
		m_vWorldPosition_Part[2] = vPos.z;

		_float3 vScaled = pPartTransform->Get_Scaled();
		m_vScale_Part[0] = vScaled.x;
		m_vScale_Part[1] = vScaled.y;
		m_vScale_Part[2] = vScaled.z;

		_float3 vRotated = pPartTransform->Get_Rotated();
		m_vRotate_Part[0] = XMConvertToDegrees(vRotated.x);
		m_vRotate_Part[1] = XMConvertToDegrees(vRotated.y);
		m_vRotate_Part[2] = XMConvertToDegrees(vRotated.z);


		// ��Ʈ ���� ������Ʈ
		if (m_pCurVoidDesc->bLoop)
			m_iLoop_Part = 0;
		else
			m_iLoop_Part = 1;


		if (CEffect_Void::PARTICLE == eType_Effect)
		{
#pragma region ��ƼŬ(+����) ��ũ���� ������ ����
			m_pCurVoidDesc = m_pCurPartEffect->Get_Desc();	// ����Ʈ_���̵� Desc
			m_pSpriteDesc_Particle = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_Sprite_Desc();		// ��ƼŬ ��������Ʈ ����ü
			CEffect_Void::DISTORTION_DESC* pDistortionDesc = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_Distortion_Desc(); // ��ƼŬ ����� ����ü
			CVIBuffer_Particle* pVIBuffer = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_VIBufferCom();	// ��ƼŬ ���� ������
			m_pParticleBufferDesc = pVIBuffer->Get_Desc(); // ������ Desc ������
#pragma endregion

			m_vTimes_Part[0] = m_pCurVoidDesc->fWaitingTime;
			m_vTimes_Part[1] = m_pCurVoidDesc->fLifeTime;
			m_vTimes_Part[2] = m_pCurVoidDesc->fRemainTime; // ������ Ÿ��??

			// �ؽ�ó ������Ʈ =============================================================================================================
			//if (m_pCurVoidDesc->bUseSpriteAnim)
			//{
			//	m_iTexIndex_Particle[CEffect_Void::TEXTURE_SPRITE] = m_pCurVoidDesc->iTextureIndex[CEffect_Void::TEXTURE_SPRITE];
			//}
			//else
			{
				m_iTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE] = m_pCurVoidDesc->iTextureIndex[CEffect_Void::TEXTURE_DIFFUSE];
			}

			m_iTexIndex_Particle[CEffect_Void::TEXTURE_MASK] = m_pCurVoidDesc->iTextureIndex[CEffect_Void::TEXTURE_MASK];
			m_iTexIndex_Particle[CEffect_Void::TEXTURE_NOISE] = m_pCurVoidDesc->iTextureIndex[CEffect_Void::TEXTURE_NOISE];
			// �ؽ�ó ������Ʈ =============================================================================================================

			m_iShaderPassIndex_Particle = m_pCurVoidDesc->iShaderPassIndex;		// ���̴� �н� �ε��� ������Ʈ

			m_iRenderGroup_Particle = m_pCurVoidDesc->iRenderGroup;				// �����׷� ������Ʈ

			m_iNumInstance_Particle = m_pParticleBufferDesc->iCurNumInstance;	// �ν��Ͻ� ���� ������Ʈ


			/* ������ ���� */
			if (m_pCurVoidDesc->bBillBoard)
				m_iBillBoard = 0;
			else
				m_iBillBoard = 1;

			/* Z���� ���� */
			if (dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_SortZ())
				m_iSortZ = 0;
			else
				m_iSortZ = 1;

			/* ��������Ʈ �ִϸ��̼� ��뿩�� */
			if (m_pCurVoidDesc->bUseSpriteAnim)
				m_iSprite_Particle = 1;
			else
				m_iSprite_Particle = 0;

			if (m_pSpriteDesc_Particle->bLoop)
				m_iSpriteLoop = 0;
			else
				m_iSpriteLoop = 1;
	

			m_vUV_MaxTileCount_Particle[0] = (_int)m_pSpriteDesc_Particle->vUV_MaxTileCount.x;
			m_vUV_MaxTileCount_Particle[1] = (_int)m_pSpriteDesc_Particle->vUV_MaxTileCount.y;

			m_fSequenceTerm_Particle = m_pSpriteDesc_Particle->fSequenceTerm;
			


			/* �÷� ���� ��� ������Ʈ */
			if (MODE_COLOR::MUL == m_pCurVoidDesc->eMode_Color)
				m_iColor_Mode_Particle = 0;
			else if (MODE_COLOR::SCREEN == m_pCurVoidDesc->eMode_Color)
				m_iColor_Mode_Particle = 1;
			else if (MODE_COLOR::OVERLAY == m_pCurVoidDesc->eMode_Color)
				m_iColor_Mode_Particle = 2;
			else if (MODE_COLOR::ADD == m_pCurVoidDesc->eMode_Color)
				m_iColor_Mode_Particle = 3;
			else if (MODE_COLOR::BURN == m_pCurVoidDesc->eMode_Color)
				m_iColor_Mode_Particle = 4;
			else if (MODE_COLOR::VIVID_RIGHT == m_pCurVoidDesc->eMode_Color)
				m_iColor_Mode_Particle = 5;
			else if (MODE_COLOR::SOFT_RIGHT == m_pCurVoidDesc->eMode_Color)
				m_iColor_Mode_Particle = 6;
			else if (MODE_COLOR::HARD_RIGHT == m_pCurVoidDesc->eMode_Color)
				m_iColor_Mode_Particle = 7;
			else if (MODE_COLOR::COLOR_DODGE == m_pCurVoidDesc->eMode_Color)
				m_iColor_Mode_Particle = 8;
			else if (MODE_COLOR::MIX_BURN == m_pCurVoidDesc->eMode_Color)
				m_iColor_Mode_Particle = 9;
			else if (MODE_COLOR::MODE_COLOR_END == m_pCurVoidDesc->eMode_Color)
				m_iColor_Mode_Particle = 10;


			/* ���̴��� ������ �ڸ� �� */
			m_vColor_Clip_Part[0] = m_pCurVoidDesc->vColor_Clip.x;
			m_vColor_Clip_Part[1] = m_pCurVoidDesc->vColor_Clip.y;
			m_vColor_Clip_Part[2] = m_pCurVoidDesc->vColor_Clip.z;
			m_vColor_Clip_Part[3] = m_pCurVoidDesc->vColor_Clip.w;

			
			/* UV �ɼ� */
			m_vUV_Offset_Particle[0] = m_pCurVoidDesc->vUV_Offset.x;
			m_vUV_Offset_Particle[1] = m_pCurVoidDesc->vUV_Offset.y;

			m_vUV_Scale_Particle[0] = m_pCurVoidDesc->vUV_Scale.x;
			m_vUV_Scale_Particle[1] = m_pCurVoidDesc->vUV_Scale.y;

			m_fUV_RotDegree_Particle = m_pCurVoidDesc->fUV_RotDegree;



			// ������ٵ� ������Ʈ =============================================================================================================
			
			/* ������ٵ� ��� ���� */
			if (m_pCurVoidDesc->bUseRigidBody)
				m_iUseRigidBody_Particle = 0;
			else
				m_iUseRigidBody_Particle = 1;
			
		
			if (0 == m_iUseRigidBody_Particle)	// ������ٵ� ����̸�
			{
				/* Ű��ƽ ���� */
				if (m_pParticleBufferDesc->bKinetic)
					m_iKinetic_Particle = 0;
				else
					m_iKinetic_Particle = 1;

				/* �߷� ��� ���� */
				if (m_pParticleBufferDesc->bUseGravity)
					m_iUseGravity_Particle = 0;
				else
					m_iUseGravity_Particle = 1;


				m_fGravity_Particle = m_pParticleBufferDesc->fGravity;	// �߷� ���ӵ�

				//m_fFriction_Particle = m_pParticleBufferDesc->fFriction;	// ���� ���
				m_vFrictionLerp_Pos_Particle[0] = m_pParticleBufferDesc->vFrictionLerp_Pos.x;
				m_vFrictionLerp_Pos_Particle[1] = m_pParticleBufferDesc->vFrictionLerp_Pos.y;

				m_vStartEnd_Friction_Particle[0] = m_pParticleBufferDesc->vStartEnd_Friction.x;
				m_vStartEnd_Friction_Particle[1] = m_pParticleBufferDesc->vStartEnd_Friction.y;


				m_fSleepThreshold_Particle = m_pParticleBufferDesc->fSleepThreshold;	// ���� �Ѱ���


				/* �Ŀ� */
				m_vMinMaxPower_Particle[0] = m_pParticleBufferDesc->vMinMaxPower.x;
				m_vMinMaxPower_Particle[1] = m_pParticleBufferDesc->vMinMaxPower.y;


				/* ����(Mass) */
				m_vMinMaxMass_Particle[0] = m_pParticleBufferDesc->vMinMaxMass.x;
				m_vMinMaxMass_Particle[1] = m_pParticleBufferDesc->vMinMaxMass.y;

	

			}
			// ������ٵ� ������Ʈ =============================================================================================================


			/* ������Ÿ��_��ƼŬ */
			m_vMinMaxLifeTime_Particle[0] = m_pParticleBufferDesc->vMinMaxLifeTime.x;
			m_vMinMaxLifeTime_Particle[1] = m_pParticleBufferDesc->vMinMaxLifeTime.y;


			/* ����(Emitter)_��ƼŬ */
			m_fEmissionTime_Particle = m_pParticleBufferDesc->fEmissionTime;
			m_iAddEmitCount_Particle = (_int)m_pParticleBufferDesc->iAddEmitCount;


			/* ������ ����(���� ����) */
			m_vMinMaxRange_Particle[0] = m_pParticleBufferDesc->vMinMaxRange.x;
			m_vMinMaxRange_Particle[1] = m_pParticleBufferDesc->vMinMaxRange.y;

			m_fMinMaxAddRange_Particle[0] = m_pParticleBufferDesc->vMinMaxAddRange.x;
			m_fMinMaxAddRange_Particle[1] = m_pParticleBufferDesc->vMinMaxAddRange.y;


			/* ��ƼŬ�� �ö� �ְ� ���� */
			m_vMinMaxPosY_Particle[0] = m_pParticleBufferDesc->vMinMaxPosY.x;
			m_vMinMaxPosY_Particle[1] = m_pParticleBufferDesc->vMinMaxPosY.y;


			/* ���ǵ� */
			m_vMinMaxSpeed_Particle[0] = m_pParticleBufferDesc->vMinMaxSpeed.x;
			m_vMinMaxSpeed_Particle[1] = m_pParticleBufferDesc->vMinMaxSpeed.y;

			m_vMinMaxTornadoSpeed_Particle[0] = m_pParticleBufferDesc->vMinMaxTornadoSpeed.x;
			m_vMinMaxTornadoSpeed_Particle[1] = m_pParticleBufferDesc->vMinMaxTornadoSpeed.y;


			/* ��ƼŬ �̵� ȸ�� ���� */
			m_vMinMaxTheta_Particle[0] = m_pParticleBufferDesc->vMinMaxTheta.x;
			m_vMinMaxTheta_Particle[1] = m_pParticleBufferDesc->vMinMaxTheta.y;


			/* ������Ŭ ���� */
			if (m_pParticleBufferDesc->bRecycle)
				m_iRecycle_Particle = 0;
			else
				m_iRecycle_Particle = 1;


			/* ������� ������ ���� */
			if (m_pParticleBufferDesc->bReverse)
				m_iReverse_Particle = 0;
			else
				m_iReverse_Particle = 1;


			/* �����̴� ��� ������Ʈ */
			if (CVIBuffer_Particle::SPARK == m_pParticleBufferDesc->eType_Action)
				m_iType_Action_Particle = 0;
			else if(CVIBuffer_Particle::BLINK == m_pParticleBufferDesc->eType_Action)
				m_iType_Action_Particle = 1;
			else if (CVIBuffer_Particle::FALL == m_pParticleBufferDesc->eType_Action)
				m_iType_Action_Particle = 2;
			else if (CVIBuffer_Particle::RISE == m_pParticleBufferDesc->eType_Action)
				m_iType_Action_Particle = 3;
			else if (CVIBuffer_Particle::TORNADO == m_pParticleBufferDesc->eType_Action)
				m_iType_Action_Particle = 4;
			else if (CVIBuffer_Particle::TYPE_ACTION_END == m_pParticleBufferDesc->eType_Action)
				m_iType_Action_Particle = 5;


			/* ���� ������ ��ġ_��ƼŬ */
			m_vMinCenterOffsetPos_Particle[0] = m_pParticleBufferDesc->vMinCenterOffsetPos.x;
			m_vMinCenterOffsetPos_Particle[1] = m_pParticleBufferDesc->vMinCenterOffsetPos.y;
			m_vMinCenterOffsetPos_Particle[2] = m_pParticleBufferDesc->vMinCenterOffsetPos.z;

			m_vMaxCenterOffsetPos_Particle[0] = m_pParticleBufferDesc->vMaxCenterOffsetPos.x;
			m_vMaxCenterOffsetPos_Particle[1] = m_pParticleBufferDesc->vMaxCenterOffsetPos.y;
			m_vMaxCenterOffsetPos_Particle[2] = m_pParticleBufferDesc->vMaxCenterOffsetPos.z;


			/* ��ü ȸ�� */
			m_vRadian_Particle[0] = m_pParticleBufferDesc->vRadian.x;
			m_vRadian_Particle[1] = m_pParticleBufferDesc->vRadian.y;
			m_vRadian_Particle[2] = m_pParticleBufferDesc->vRadian.z;


			/* ����� ��� ���� ����������� �Ұ��� ������Ʈ */
			if (CVIBuffer_Particle::DIR_RIGHT == m_pParticleBufferDesc->eType_Dir)
				m_iType_Dir_Particle = 0;
			else if (CVIBuffer_Particle::DIR_UP == m_pParticleBufferDesc->eType_Dir)
				m_iType_Dir_Particle = 1;
			else if (CVIBuffer_Particle::DIR_LOOK == m_pParticleBufferDesc->eType_Dir)
				m_iType_Dir_Particle = 2;


			/* ȸ�� ���� */
			m_vRotationOffsetX_Particle[0] = m_pParticleBufferDesc->vMinMaxRotationOffsetX.x;
			m_vRotationOffsetX_Particle[1] = m_pParticleBufferDesc->vMinMaxRotationOffsetX.y;

			m_vRotationOffsetY_Particle[0] = m_pParticleBufferDesc->vMinMaxRotationOffsetY.x;
			m_vRotationOffsetY_Particle[1] = m_pParticleBufferDesc->vMinMaxRotationOffsetY.y;

			m_vRotationOffsetZ_Particle[0] = m_pParticleBufferDesc->vMinMaxRotationOffsetZ.x;
			m_vRotationOffsetZ_Particle[1] = m_pParticleBufferDesc->vMinMaxRotationOffsetZ.y;


			/* ������ ���� */
			if (TRUE == m_pParticleBufferDesc->bUseScaleLerp)
				m_iUseScaleLerp_Particle = 0;
			else if (FALSE == m_pParticleBufferDesc->bUseScaleLerp)
				m_iUseScaleLerp_Particle = 1;


			m_vScaleLerp_Up_Pos[0] = m_pParticleBufferDesc->vScaleLerp_Up_Pos.x;
			m_vScaleLerp_Up_Pos[1] = m_pParticleBufferDesc->vScaleLerp_Up_Pos.y;

			m_vScaleLerp_Down_Pos[0] = m_pParticleBufferDesc->vScaleLerp_Down_Pos.x;
			m_vScaleLerp_Down_Pos[1] = m_pParticleBufferDesc->vScaleLerp_Down_Pos.y;

			m_vMinMaxWidth_Particle[0] = m_pParticleBufferDesc->vMinMaxWidth.x;
			m_vMinMaxWidth_Particle[1] = m_pParticleBufferDesc->vMinMaxWidth.y;

			m_vMinMaxHeight_Particle[0] = m_pParticleBufferDesc->vMinMaxHeight.x;
			m_vMinMaxHeight_Particle[1] = m_pParticleBufferDesc->vMinMaxHeight.y;


			/* ������ ������ */
			if (TRUE == m_pParticleBufferDesc->bScaleRatio)
				m_iScaleRatio_Particle = 0;
			else if (FALSE == m_pParticleBufferDesc->bScaleRatio)
				m_iScaleRatio_Particle = 1;



			/* ���� */
			/* ���ں� ���� ���� ���� */
			if (m_pParticleBufferDesc->bDynamic_Color)
				m_iDynamic_Color_Particle = 0;
			else
				m_iDynamic_Color_Particle = 1;

			m_fColor_Min_Particle[0] = m_pParticleBufferDesc->vMinMaxRed.x;
			m_fColor_Min_Particle[1] = m_pParticleBufferDesc->vMinMaxBlue.x;
			m_fColor_Min_Particle[2] = m_pParticleBufferDesc->vMinMaxGreen.x;
			m_fColor_Min_Particle[3] = m_pParticleBufferDesc->vMinMaxAlpha.x;

			m_fColor_Max_Particle[0] = m_pParticleBufferDesc->vMinMaxRed.y;
			m_fColor_Max_Particle[1] = m_pParticleBufferDesc->vMinMaxBlue.y;
			m_fColor_Max_Particle[2] = m_pParticleBufferDesc->vMinMaxGreen.y;
			m_fColor_Max_Particle[3] = m_pParticleBufferDesc->vMinMaxAlpha.y;

			if (0 == m_iDynamic_Color_Particle)
			{
				m_fColor_Cur_Particle[0] = m_pParticleBufferDesc->vCurrentColor.y;
				m_fColor_Cur_Particle[1] = m_pParticleBufferDesc->vCurrentColor.y;
				m_fColor_Cur_Particle[2] = m_pParticleBufferDesc->vCurrentColor.y;
				m_fColor_Cur_Particle[3] = m_pParticleBufferDesc->vCurrentColor.y;

			}
			

			/* ���̵� Ÿ�� ������Ʈ */
			if (CVIBuffer_Particle::FADE_NONE == m_pParticleBufferDesc->eType_Fade)
				m_iType_Fade_Particle = 0;
			else if (CVIBuffer_Particle::FADE_OUT == m_pParticleBufferDesc->eType_Fade)
				m_iType_Fade_Particle = 1;
			else if (CVIBuffer_Particle::FADE_IN == m_pParticleBufferDesc->eType_Fade)
				m_iType_Fade_Particle = 2;


			/* ���̵� ���� ������Ʈ */
			if (CVIBuffer_Particle::TYPE_FADE_TAKES::LIFE == m_pParticleBufferDesc->eType_Fade_Takes)
				m_iType_Fade_Takes_Particle = 0;
			else if (CVIBuffer_Particle::TYPE_FADE_TAKES::DIST == m_pParticleBufferDesc->eType_Fade_Takes)
				m_iType_Fade_Takes_Particle = 1;
			else if (CVIBuffer_Particle::TYPE_FADE_TAKES::HEIGHT == m_pParticleBufferDesc->eType_Fade_Takes)
				m_iType_Fade_Takes_Particle = 2;
			else if (CVIBuffer_Particle::TYPE_FADE_TAKES::SCALE == m_pParticleBufferDesc->eType_Fade_Takes)
				m_iType_Fade_Takes_Particle = 3;
			else if (CVIBuffer_Particle::TYPE_FADE_TAKES::MAGIC == m_pParticleBufferDesc->eType_Fade_Takes)
				m_iType_Fade_Takes_Particle = 4;
			else if (CVIBuffer_Particle::TYPE_FADE_TAKES::TYPE_FADE_TAKES_END == m_pParticleBufferDesc->eType_Fade_Takes)
				m_iType_Fade_Takes_Particle = 5;



			/* �� ��� �� ������Ʈ */
			m_vBloomPower_Particle[0] = m_pCurVoidDesc->vBloomPower.x;
			m_vBloomPower_Particle[1] = m_pCurVoidDesc->vBloomPower.y;
			m_vBloomPower_Particle[2] = m_pCurVoidDesc->vBloomPower.z;


			m_fRimColor_Particle[0] = m_pCurVoidDesc->vRimColor.x;
			m_fRimColor_Particle[1] = m_pCurVoidDesc->vRimColor.y;
			m_fRimColor_Particle[2] = m_pCurVoidDesc->vRimColor.z;
			m_fRimColor_Particle[3] = m_pCurVoidDesc->vRimColor.w;



			/* �����_��ƼŬ �� ������Ʈ */

			// ����� ��ũ�� ���� ������Ʈ
			if (CEffect_Void::SCROLL_ROW == pDistortionDesc->eType_Scroll)
				m_iType_Scroll_Particle = 0;
			else if (CEffect_Void::SCROLL_COL == pDistortionDesc->eType_Scroll)
				m_iType_Scroll_Particle = 1;
			else if (CEffect_Void::SCROLL_BOTH == pDistortionDesc->eType_Scroll)
				m_iType_Scroll_Particle = 2;
			else if (CEffect_Void::SCROLL_ROTATE == pDistortionDesc->eType_Scroll)
				m_iType_Scroll_Particle = 3;
			else if (CEffect_Void::TYPE_SCROLL_END == pDistortionDesc->eType_Scroll)
				m_iType_Scroll_Particle = 4;


			m_vScrollSpeeds_Particle[0] = pDistortionDesc->vScrollSpeeds.x;
			m_vScrollSpeeds_Particle[1] = pDistortionDesc->vScrollSpeeds.y;
			m_vScrollSpeeds_Particle[2] = pDistortionDesc->vScrollSpeeds.z;


			m_vScales_Distortion_Particle[0] = pDistortionDesc->vScales.x;
			m_vScales_Distortion_Particle[1] = pDistortionDesc->vScales.y;
			m_vScales_Distortion_Particle[2] = pDistortionDesc->vScales.z;


			m_vDistortion1_Particle[0] = pDistortionDesc->vDistortion1.x;
			m_vDistortion1_Particle[1] = pDistortionDesc->vDistortion1.y;

			m_vDistortion2_Particle[0] = pDistortionDesc->vDistortion2.x;
			m_vDistortion2_Particle[1] = pDistortionDesc->vDistortion2.y;

			m_vDistortion3_Particle[0] = pDistortionDesc->vDistortion3.x;
			m_vDistortion3_Particle[1] = pDistortionDesc->vDistortion3.y;

			m_fDistortionScale_Particle = pDistortionDesc->fDistortionScale;
			m_fDistortionBias_Particle = pDistortionDesc->fDistortionBias;


		}

		if (CEffect_Void::RECT == eType_Effect)
		{
			m_pCurVoidDesc = m_pCurPartEffect->Get_Desc();								// ����Ʈ_���̵� Desc (�θ�)
			m_pRectDesc = dynamic_cast<CEffect_Rect*>(m_pCurPartEffect)->Get_Desc();	// ��Ʈ�� Desc (�ڽ�)
			CEffect_Void::DISTORTION_DESC* pDistortionDesc = dynamic_cast<CEffect_Rect*>(m_pCurPartEffect)->Get_Distortion_Desc();
			CEffect_Void::UVSPRITE_DESC* pSpriteDesc = dynamic_cast<CEffect_Rect*>(m_pCurPartEffect)->Get_Sprite_Desc();


			m_iShaderPassIndex_Rect = m_pCurVoidDesc->iShaderPassIndex;		// ���̴� �н� �ε��� ������Ʈ

			m_iRenderGroup_Rect = m_pCurVoidDesc->iRenderGroup;				// �����׷� ������Ʈ


			/* ������ ����_Rect */
			if (m_pCurVoidDesc->bBillBoard)
				m_iBillBoard_Rect = 0;
			else
				m_iBillBoard_Rect = 1;


			/* ��������Ʈ �ִϸ��̼� ��뿩��_Rect */
			if (m_pCurVoidDesc->bUseSpriteAnim)
				m_iSprite_Rect = 1;
			else
				m_iSprite_Rect = 0;

			if (1 == m_iSprite_Rect)	// ��������Ʈ �ִϸ��̼� ����̸�(��Ʈ)
			{
				m_vUV_MaxTileCount_Particle[0] = (_int)pSpriteDesc->vUV_MaxTileCount.x;
				m_vUV_MaxTileCount_Particle[1] = (_int)pSpriteDesc->vUV_MaxTileCount.y;

				m_fSequenceTerm_Particle = pSpriteDesc->fSequenceTerm;
			}


			/* ���̴����� ������ ��_Rect*/
			m_fColor_Mul_Rect[0] = m_pCurVoidDesc->vColor_Mul.x;
			m_fColor_Mul_Rect[1] = m_pCurVoidDesc->vColor_Mul.y;
			m_fColor_Mul_Rect[2] = m_pCurVoidDesc->vColor_Mul.z;
			m_fColor_Mul_Rect[3] = m_pCurVoidDesc->vColor_Mul.w;


			/* ���̴����� discard�� �� */
			m_vColor_Clip_Rect[0] = m_pCurVoidDesc->vColor_Clip.x;
			m_vColor_Clip_Rect[1] = m_pCurVoidDesc->vColor_Clip.y;
			m_vColor_Clip_Rect[2] = m_pCurVoidDesc->vColor_Clip.z;
			m_vColor_Clip_Rect[3] = m_pCurVoidDesc->vColor_Clip.w;


			/* UV ȸ�� �� */
			m_fUV_RotDegree_Rect = m_pCurVoidDesc->fUV_RotDegree;

			m_fUV_Offset_Rect[0] = m_pCurVoidDesc->vUV_Offset.x;
			m_fUV_Offset_Rect[1] = m_pCurVoidDesc->vUV_Offset.y;

			m_vUV_Scale_Rect[0] = m_pCurVoidDesc->vUV_Scale.x;
			m_vUV_Scale_Rect[1] = m_pCurVoidDesc->vUV_Scale.y;



			/* ����� */
			// ����� ��ũ�� ���� ������Ʈ
			if (CEffect_Void::SCROLL_ROW == pDistortionDesc->eType_Scroll)
				m_iType_Scroll_Rect = 0;
			else if (CEffect_Void::SCROLL_COL == pDistortionDesc->eType_Scroll)
				m_iType_Scroll_Rect = 1;
			else if (CEffect_Void::SCROLL_BOTH == pDistortionDesc->eType_Scroll)
				m_iType_Scroll_Rect = 2;
			else if (CEffect_Void::SCROLL_ROTATE == pDistortionDesc->eType_Scroll)
				m_iType_Scroll_Rect = 3;
			else if (CEffect_Void::TYPE_SCROLL_END == pDistortionDesc->eType_Scroll)
				m_iType_Scroll_Rect = 4;


			m_vScrollSpeeds[0] = pDistortionDesc->vScrollSpeeds.x;
			m_vScrollSpeeds[1] = pDistortionDesc->vScrollSpeeds.y;
			m_vScrollSpeeds[2] = pDistortionDesc->vScrollSpeeds.z;

			m_vScales_Distortion[0] = pDistortionDesc->vScales.x;
			m_vScales_Distortion[1] = pDistortionDesc->vScales.y;


			m_vDistortion1[0] = pDistortionDesc->vDistortion1.x;
			m_vDistortion1[1] = pDistortionDesc->vDistortion1.y;

			m_vDistortion2[0] = pDistortionDesc->vDistortion2.x;
			m_vDistortion2[1] = pDistortionDesc->vDistortion2.y;

			m_vDistortion3[0] = pDistortionDesc->vDistortion3.x;
			m_vDistortion3[1] = pDistortionDesc->vDistortion3.y;

			m_fDistortionBias = pDistortionDesc->fDistortionBias;


			/* ��������Ʈ */
			m_fSequenceTerm_Rect = pSpriteDesc->fSequenceTerm;

			m_vUV_MaxTileCount_Rect[0] = (_int)pSpriteDesc->vUV_MaxTileCount.x;
			m_vUV_MaxTileCount_Rect[1] = (_int)pSpriteDesc->vUV_MaxTileCount.y;
		}


		if (CEffect_Void::MESH == eType_Effect)
		{
#pragma region �޽�(+����) ��ũ���� ������ ����
			m_pCurVoidDesc = m_pCurPartEffect->Get_Desc();	// ����Ʈ_���̵� Desc
			CEffect_Void::DISTORTION_DESC* pDistortionDesc = dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Get_Distortion_Desc();	// �޽�(�ν��Ͻ�)�� ����� Desc ������
			CVIBuffer_Effect_Model_Instance* pVIBuffer = dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Get_VIBufferCom();	// �޽�(�ν��Ͻ�)���� ������
			m_pMeshBufferDesc = pVIBuffer->Get_Desc(); // ������ Desc ������
#pragma endregion

			// �ؽ�ó ������Ʈ =============================================================================================================
			m_iTexIndex_Mesh[CEffect_Void::TEXTURE_DIFFUSE] = m_pCurVoidDesc->iTextureIndex[CEffect_Void::TEXTURE_DIFFUSE];
			m_iTexIndex_Mesh[CEffect_Void::TEXTURE_MASK] = m_pCurVoidDesc->iTextureIndex[CEffect_Void::TEXTURE_MASK];
			m_iTexIndex_Mesh[CEffect_Void::TEXTURE_NOISE] = m_pCurVoidDesc->iTextureIndex[CEffect_Void::TEXTURE_NOISE];
			// �ؽ�ó ������Ʈ =============================================================================================================

			/* �ν��Ͻ� ���� */
			m_iNumInstance_Mesh = m_pMeshBufferDesc->iCurNumInstance;

			/* ���� */
			m_fMorphTimeTerm = m_pMeshBufferDesc->fMorphTimeTerm;

			m_iShaderPassIndex_Mesh = m_pCurVoidDesc->iShaderPassIndex;

			m_iRenderGroup_Mesh = m_pCurVoidDesc->iRenderGroup;


			/* UV ���� �� ������Ʈ */
			// Offset 
			m_fUV_Offset_Mesh[0] = m_pCurVoidDesc->vUV_Offset.x;
			m_fUV_Offset_Mesh[1] = m_pCurVoidDesc->vUV_Offset.y;

			// Scale
			m_vUV_Scale_Mesh[0] = m_pCurVoidDesc->vUV_Scale.x;
			m_vUV_Scale_Mesh[1] = m_pCurVoidDesc->vUV_Scale.y;

			// UV ȸ��
			m_fUV_RotDegree_Mesh = m_pCurVoidDesc->fUV_RotDegree;



			// ������ٵ� ������Ʈ =============================================================================================================		
			/* ������ٵ� ��� ���� */
			if (m_pCurVoidDesc->bUseRigidBody)
				m_iUseRigidBody_Mesh = 0;
			else
				m_iUseRigidBody_Mesh = 1;	
			
			if (0 == m_iUseRigidBody_Mesh)	// ������ٵ� ����̸�
			{
				/* Ű��ƽ ���� */
				if (m_pMeshBufferDesc->bKinetic)
					m_iKinetic_Mesh = 0;
				else
					m_iKinetic_Mesh = 1;

				/* �߷� ��� ���� */
				if (m_pMeshBufferDesc->bUseGravity)
					m_iUseGravity_Mesh = 0;
				else
					m_iUseGravity_Mesh = 1;


				m_fGravity_Mesh			= m_pMeshBufferDesc->fGravity;			// �߷� ���ӵ�


				/* �Ŀ� */
				m_vMinMaxPower_Mesh[0] = m_pMeshBufferDesc->vMinMaxPower.x;
				m_vMinMaxPower_Mesh[1] = m_pMeshBufferDesc->vMinMaxPower.y;


				/* ����(Mass) */
				m_vMinMaxMass_Mesh[0] = m_pMeshBufferDesc->vMinMaxMass.x;
				m_vMinMaxMass_Mesh[1] = m_pMeshBufferDesc->vMinMaxMass.y;


				// ���� ���
				m_fMinMaxFriction_Mesh[0] = m_pMeshBufferDesc->vMinMaxFriction.x;
				m_fMinMaxFriction_Mesh[1] = m_pMeshBufferDesc->vMinMaxFriction.y;

				m_fSleepThreshold_Mesh = m_pMeshBufferDesc->fSleepThreshold;	// ���� �Ѱ���

			}


			///* ���̵� Ÿ�� ������Ʈ */
			//if (CVIBuffer_Effect_Model_Instance::FADE_NONE == m_pMeshBufferDesc->eType_Fade)
			//	m_iType_Fade_Mesh = 0;
			//else if (CVIBuffer_Effect_Model_Instance::FADE_OUT == m_pMeshBufferDesc->eType_Fade)
			//	m_iType_Fade_Mesh = 1;
			//else if (CVIBuffer_Effect_Model_Instance::FADE_IN == m_pMeshBufferDesc->eType_Fade)
			//	m_iType_Fade_Mesh = 2;


			/* �޽� ������ٵ� ���� ���� (�޽� ��ƼŬ ���̵� �ξƿ� ���� ��Ȱ��) ������Ʈ */
			if (CVIBuffer_Effect_Model_Instance::TYPE_FADE_TAKES::LIFE == m_pMeshBufferDesc->eType_Fade_Takes)
				m_iType_Fade_Takes_Mesh = 0;
			else if (CVIBuffer_Effect_Model_Instance::TYPE_FADE_TAKES::DIST == m_pMeshBufferDesc->eType_Fade_Takes)
				m_iType_Fade_Takes_Mesh = 1;
			else if (CVIBuffer_Effect_Model_Instance::TYPE_FADE_TAKES::TYPE_FADE_TAKES_END == m_pMeshBufferDesc->eType_Fade_Takes)
				m_iType_Fade_Takes_Mesh = 4;


			// ������ٵ� ������Ʈ =============================================================================================================
			
			/* ���ǵ� */
			m_vMinMaxSpeed_Mesh[0] = m_pMeshBufferDesc->vMinMaxSpeed.x;
			m_vMinMaxSpeed_Mesh[1] = m_pMeshBufferDesc->vMinMaxSpeed.y;


			/* ��ƼŬ �̵� ȸ�� ���� */
			m_vMinMaxTheta_Mesh[0] = m_pMeshBufferDesc->vMinMaxTheta.x;
			m_vMinMaxTheta_Mesh[1] = m_pMeshBufferDesc->vMinMaxTheta.y;


			/* ������Ŭ ����_�޽� ��ƼŬ */
			if (m_pMeshBufferDesc->bRecycle)
				m_iRecycle_Mesh = 0;
			else
				m_iRecycle_Mesh = 1;


			/* ������� ������ ���� */
			if (m_pMeshBufferDesc->bReverse)
				m_iReverse_Mesh = 0;
			else
				m_iReverse_Mesh = 1;


			/* �����̴� ��� ������Ʈ */
			if (CVIBuffer_Effect_Model_Instance::SPARK == m_pMeshBufferDesc->eType_Action)
				m_iType_Action_Mesh = 0;
			else if (CVIBuffer_Effect_Model_Instance::BLINK == m_pMeshBufferDesc->eType_Action)
				m_iType_Action_Mesh = 1;
			else if (CVIBuffer_Effect_Model_Instance::FALL == m_pMeshBufferDesc->eType_Action)
				m_iType_Action_Mesh = 2;
			else if (CVIBuffer_Effect_Model_Instance::RISE == m_pMeshBufferDesc->eType_Action)
				m_iType_Action_Mesh = 3;
			else if (CVIBuffer_Effect_Model_Instance::TORNADO == m_pMeshBufferDesc->eType_Action)
				m_iType_Action_Mesh = 4;



			/* ������Ÿ��_�޽� ��ƼŬ */
			m_vMinMaxLifeTime_Mesh[0] = m_pMeshBufferDesc->vMinMaxLifeTime.x;
			m_vMinMaxLifeTime_Mesh[1] = m_pMeshBufferDesc->vMinMaxLifeTime.y;


			/* ����(Emitter)_�޽� */
			m_fEmissionTime_Mesh = m_pMeshBufferDesc->fEmissionTime;
			m_iAddEmitCount_Mesh = (_int)m_pMeshBufferDesc->iAddEmitCount;


			/* ���� ����(Range) */
			m_vMinMaxRange_Mesh[0] = m_pMeshBufferDesc->vMinMaxRange.x;
			m_vMinMaxRange_Mesh[1] = m_pMeshBufferDesc->vMinMaxRange.y;

			/* �޽� ��ƼŬ �߰� ���� ���� */
			m_fMinMaxAddRange_Mesh[0] = m_pMeshBufferDesc->vMinMaxAddRange.x;
			m_fMinMaxAddRange_Mesh[1] = m_pMeshBufferDesc->vMinMaxAddRange.y;


			/* �޽� ��ƼŬ�� �ö� �ְ� ���� */
			m_vMinMaxPosY_Mesh[0] = m_pMeshBufferDesc->vMinMaxPosY.x;
			m_vMinMaxPosY_Mesh[1] = m_pMeshBufferDesc->vMinMaxPosY.y;


			/* ���� ������ ��ġ_�޽� */
			m_vMinCenterOffsetPos_Mesh[0] = m_pMeshBufferDesc->vMinCenterOffsetPos.x;
			m_vMinCenterOffsetPos_Mesh[1] = m_pMeshBufferDesc->vMinCenterOffsetPos.y;
			m_vMinCenterOffsetPos_Mesh[2] = m_pMeshBufferDesc->vMinCenterOffsetPos.z;

			m_vMaxCenterOffsetPos_Mesh[0] = m_pMeshBufferDesc->vMaxCenterOffsetPos.x;
			m_vMaxCenterOffsetPos_Mesh[1] = m_pMeshBufferDesc->vMaxCenterOffsetPos.y;
			m_vMaxCenterOffsetPos_Mesh[2] = m_pMeshBufferDesc->vMaxCenterOffsetPos.z;


			/* ��ü ȸ�� */
			m_vRadian_Mesh[0] = m_pMeshBufferDesc->vRadian.x;
			m_vRadian_Mesh[1] = m_pMeshBufferDesc->vRadian.y;
			m_vRadian_Mesh[2] = m_pMeshBufferDesc->vRadian.z;


			/* ����� ��� ���� ����������� �Ұ��� ������Ʈ */
			if (CVIBuffer_Effect_Model_Instance::DIR_RIGHT == m_pMeshBufferDesc->eType_Dir)
				m_iType_Dir_Mesh = 0;
			else if (CVIBuffer_Effect_Model_Instance::DIR_UP == m_pMeshBufferDesc->eType_Dir)
				m_iType_Dir_Mesh = 1;
			else if (CVIBuffer_Effect_Model_Instance::DIR_LOOK == m_pMeshBufferDesc->eType_Dir)
				m_iType_Dir_Mesh = 2;


			/* ȸ�� ���� */
			m_vRotationOffsetX_Mesh[0] = m_pMeshBufferDesc->vMinMaxRotationOffsetX.x;
			m_vRotationOffsetX_Mesh[1] = m_pMeshBufferDesc->vMinMaxRotationOffsetX.y;

			m_vRotationOffsetY_Mesh[0] = m_pMeshBufferDesc->vMinMaxRotationOffsetY.x;
			m_vRotationOffsetY_Mesh[1] = m_pMeshBufferDesc->vMinMaxRotationOffsetY.y;

			m_vRotationOffsetZ_Mesh[0] = m_pMeshBufferDesc->vMinMaxRotationOffsetZ.x;
			m_vRotationOffsetZ_Mesh[1] = m_pMeshBufferDesc->vMinMaxRotationOffsetZ.y;



			/* ������ ���� */
			if (TRUE == m_pMeshBufferDesc->bUseScaleLerp)
				m_iUseScaleLerp_Mesh = 0;
			else if (FALSE == m_pMeshBufferDesc->bUseScaleLerp)
				m_iUseScaleLerp_Mesh = 1;


			if (TRUE == m_pMeshBufferDesc->bScaleRatio)
				m_iScaleRatio_Mesh = 0;
			else if (FALSE == m_pMeshBufferDesc->bScaleRatio)
				m_iScaleRatio_Mesh = 1;


			m_vScaleLerp_Up_Pos_Mesh[0] = m_pMeshBufferDesc->vScaleLerp_Up_Pos.x;
			m_vScaleLerp_Up_Pos_Mesh[1] = m_pMeshBufferDesc->vScaleLerp_Up_Pos.y;

			m_vScaleLerp_Down_Pos_Mesh[0] = m_pMeshBufferDesc->vScaleLerp_Down_Pos.x;
			m_vScaleLerp_Down_Pos_Mesh[1] = m_pMeshBufferDesc->vScaleLerp_Down_Pos.y;

			m_vStartScale_Mesh[0] = m_pMeshBufferDesc->vStartScale.x;
			m_vStartScale_Mesh[1] = m_pMeshBufferDesc->vStartScale.y;
			m_vStartScale_Mesh[2] = m_pMeshBufferDesc->vStartScale.z;

			m_vEndScale_Mesh[0] = m_pMeshBufferDesc->vEndScale.x;
			m_vEndScale_Mesh[1] = m_pMeshBufferDesc->vEndScale.y;
			m_vEndScale_Mesh[2] = m_pMeshBufferDesc->vEndScale.z;



			/* ���� */

			/* �÷� ���� ��� ������Ʈ */
			if (MODE_COLOR::MUL == m_pCurVoidDesc->eMode_Color)
				m_iColor_Mode_Mesh = 0;
			else if (MODE_COLOR::SCREEN == m_pCurVoidDesc->eMode_Color)
				m_iColor_Mode_Mesh = 1;
			else if (MODE_COLOR::OVERLAY == m_pCurVoidDesc->eMode_Color)
				m_iColor_Mode_Mesh = 2;
			else if (MODE_COLOR::ADD == m_pCurVoidDesc->eMode_Color)
				m_iColor_Mode_Mesh = 3;
			else if (MODE_COLOR::BURN == m_pCurVoidDesc->eMode_Color)
				m_iColor_Mode_Mesh = 4;
			else if (MODE_COLOR::VIVID_RIGHT == m_pCurVoidDesc->eMode_Color)
				m_iColor_Mode_Mesh = 5;
			else if (MODE_COLOR::SOFT_RIGHT == m_pCurVoidDesc->eMode_Color)
				m_iColor_Mode_Mesh = 6;
			else if (MODE_COLOR::HARD_RIGHT == m_pCurVoidDesc->eMode_Color)
				m_iColor_Mode_Mesh = 7;
			else if (MODE_COLOR::COLOR_DODGE == m_pCurVoidDesc->eMode_Color)
				m_iColor_Mode_Mesh = 8;
			else if (MODE_COLOR::MIX_BURN == m_pCurVoidDesc->eMode_Color)
				m_iColor_Mode_Mesh = 9;
			else if (MODE_COLOR::MODE_COLOR_END == m_pCurVoidDesc->eMode_Color)
				m_iColor_Mode_Mesh = 10;


			/* ���̴��� ������ �ڸ� �� */
			m_vColor_Clip_Part[0] = m_pCurVoidDesc->vColor_Clip.x;
			m_vColor_Clip_Part[1] = m_pCurVoidDesc->vColor_Clip.y;
			m_vColor_Clip_Part[2] = m_pCurVoidDesc->vColor_Clip.z;
			m_vColor_Clip_Part[3] = m_pCurVoidDesc->vColor_Clip.w;


			/* ���ں� ���� ���� ���� */
			if (m_pMeshBufferDesc->bDynamic_Color)
				m_iDynamic_Color_Mesh = 0;
			else
				m_iDynamic_Color_Mesh = 1;

			m_fColor_Min_Mesh[0] = m_pMeshBufferDesc->vMinMaxRed.x;
			m_fColor_Min_Mesh[1] = m_pMeshBufferDesc->vMinMaxBlue.x;
			m_fColor_Min_Mesh[2] = m_pMeshBufferDesc->vMinMaxGreen.x;
			m_fColor_Min_Mesh[3] = m_pMeshBufferDesc->vMinMaxAlpha.x;

			m_fColor_Max_Mesh[0] = m_pMeshBufferDesc->vMinMaxRed.y;
			m_fColor_Max_Mesh[1] = m_pMeshBufferDesc->vMinMaxBlue.y;
			m_fColor_Max_Mesh[2] = m_pMeshBufferDesc->vMinMaxGreen.y;
			m_fColor_Max_Mesh[3] = m_pMeshBufferDesc->vMinMaxAlpha.y;

			if (0 == m_iDynamic_Color_Mesh)
			{
				m_fColor_Cur_Mesh[0] = m_pMeshBufferDesc->vCurrentColor.y;
				m_fColor_Cur_Mesh[1] = m_pMeshBufferDesc->vCurrentColor.y;
				m_fColor_Cur_Mesh[2] = m_pMeshBufferDesc->vCurrentColor.y;
				m_fColor_Cur_Mesh[3] = m_pMeshBufferDesc->vCurrentColor.y;

			}


			/* Rim Bloom ����� ���� */
			m_vBloomPower_Mesh[0] = m_pCurVoidDesc->vBloomPower.x;
			m_vBloomPower_Mesh[1] = m_pCurVoidDesc->vBloomPower.y;
			m_vBloomPower_Mesh[2] = m_pCurVoidDesc->vBloomPower.z;

			m_fRimColor_Mesh[0] = m_pCurVoidDesc->vRimColor.x;
			m_fRimColor_Mesh[1] = m_pCurVoidDesc->vRimColor.y;
			m_fRimColor_Mesh[2] = m_pCurVoidDesc->vRimColor.z;
			m_fRimColor_Mesh[3] = m_pCurVoidDesc->vRimColor.w;

			m_fRimPower_Mesh = m_pCurVoidDesc->fRimPower;



			/* �����_�޽� �� ������Ʈ */
			// ����� ��ũ�� ���� ������Ʈ
			if (CEffect_Void::SCROLL_ROW == pDistortionDesc->eType_Scroll)
				m_iType_Scroll_Mesh = 0;
			else if (CEffect_Void::SCROLL_COL == pDistortionDesc->eType_Scroll)
				m_iType_Scroll_Mesh = 1;
			else if (CEffect_Void::SCROLL_BOTH == pDistortionDesc->eType_Scroll)
				m_iType_Scroll_Mesh = 2;
			else if (CEffect_Void::SCROLL_ROTATE == pDistortionDesc->eType_Scroll)
				m_iType_Scroll_Mesh = 3;
			else if (CEffect_Void::TYPE_SCROLL_END == pDistortionDesc->eType_Scroll)
				m_iType_Scroll_Mesh = 4;



			m_vScrollSpeeds_Mesh[0] = pDistortionDesc->vScrollSpeeds.x;
			m_vScrollSpeeds_Mesh[1] = pDistortionDesc->vScrollSpeeds.y;
			m_vScrollSpeeds_Mesh[2] = pDistortionDesc->vScrollSpeeds.z;


			m_vScales_Distortion_Mesh[0] = pDistortionDesc->vScales.x;
			m_vScales_Distortion_Mesh[1] = pDistortionDesc->vScales.y;
			m_vScales_Distortion_Mesh[2] = pDistortionDesc->vScales.z;


			m_vDistortion1_Mesh[0] = pDistortionDesc->vDistortion1.x;
			m_vDistortion1_Mesh[1] = pDistortionDesc->vDistortion1.y;

			m_vDistortion2_Mesh[0] = pDistortionDesc->vDistortion2.x;
			m_vDistortion2_Mesh[1] = pDistortionDesc->vDistortion2.y;

			m_vDistortion3_Mesh[0] = pDistortionDesc->vDistortion3.x;
			m_vDistortion3_Mesh[1] = pDistortionDesc->vDistortion3.y;

			m_fDistortionScale_Mesh = pDistortionDesc->fDistortionScale;
			m_fDistortionBias_Mesh = pDistortionDesc->fDistortionBias;
		}

	}

}

void CWindow_EffectTool::Select_EasingType(EASING_TYPE* eType)
{
	/* Easing Type : ��¡ Ÿ�� */
	if (ImGui::Button("LINEAR_00"))
	{
		*eType = EASING_TYPE::LINEAR;
	}
	if (ImGui::Button("QUAD_IN_01"))
	{
		*eType = EASING_TYPE::QUAD_IN;
	}
	ImGui::SameLine();
	if (ImGui::Button("QUAD_OUT_02"))
	{
		*eType = EASING_TYPE::QUAD_OUT;
	}
	ImGui::SameLine();
	if (ImGui::Button("QUAD_INOUT_03"))
	{
		*eType = EASING_TYPE::QUAD_INOUT;
	}
	if (ImGui::Button("CUBIC_IN_04"))
	{
		*eType = EASING_TYPE::CUBIC_IN;
	}
	ImGui::SameLine();
	if (ImGui::Button("CUBIC_OUT_05"))
	{
		*eType = EASING_TYPE::CUBIC_OUT;
	}
	ImGui::SameLine();
	if (ImGui::Button("CUBIC_INOUT_06"))
	{
		*eType = EASING_TYPE::CUBIC_INOUT;
	}
	if (ImGui::Button("QUART_IN_07"))
	{
		*eType = EASING_TYPE::QUART_IN;
	}
	ImGui::SameLine();
	if (ImGui::Button("QUART_OUT_08"))
	{
		*eType = EASING_TYPE::QUART_OUT;
	}
	ImGui::SameLine();
	if (ImGui::Button("QUART_INOUT_09"))
	{
		*eType = EASING_TYPE::QUART_INOUT;
	}
	if (ImGui::Button("QUINT_IN_10"))
	{
		*eType = EASING_TYPE::QUINT_IN;
	}
	ImGui::SameLine();
	if (ImGui::Button("QUINT_OUT_11"))
	{
		*eType = EASING_TYPE::QUINT_OUT;
	}
	ImGui::SameLine();
	if (ImGui::Button("QUINT_INOUT_12"))
	{
		*eType = EASING_TYPE::QUINT_INOUT;
	}
	if (ImGui::Button("SINE_IN_13"))
	{
		*eType = EASING_TYPE::SINE_IN;
	}
	ImGui::SameLine();
	if (ImGui::Button("SINE_OUT_14"))
	{
		*eType = EASING_TYPE::SINE_OUT;
	}
	ImGui::SameLine();
	if (ImGui::Button("SINE_INOUT_15"))
	{
		*eType = EASING_TYPE::SINE_INOUT;
	}
	if (ImGui::Button("EXPO_IN_16"))
	{
		*eType = EASING_TYPE::EXPO_IN;
	}
	ImGui::SameLine();
	if (ImGui::Button("EXPO_OUT_17"))
	{
		*eType = EASING_TYPE::EXPO_OUT;
	}
	ImGui::SameLine();
	if (ImGui::Button("EXPO_INOUT_18"))
	{
		*eType = EASING_TYPE::EXPO_INOUT;
	}
	if (ImGui::Button("CIRC_IN_19"))
	{
		*eType = EASING_TYPE::CIRC_IN;
	}
	ImGui::SameLine();
	if (ImGui::Button("CIRC_OUT_20"))
	{
		*eType = EASING_TYPE::CIRC_OUT;
	}
	ImGui::SameLine();
	if (ImGui::Button("CIRC_INOUT_21"))
	{
		*eType = EASING_TYPE::CIRC_INOUT;
	}
	if (ImGui::Button("ELASTIC_IN_22"))
	{
		*eType = EASING_TYPE::ELASTIC_IN;
	}
	ImGui::SameLine();
	if (ImGui::Button("ELASTIC_OUT_23"))
	{
		*eType = EASING_TYPE::ELASTIC_OUT;
	}
	ImGui::SameLine();
	if (ImGui::Button("ELASTIC_INOUT_24"))
	{
		*eType = EASING_TYPE::ELASTIC_INOUT;
	}
	if (ImGui::Button("BOUNCE_IN_25"))
	{
		*eType = EASING_TYPE::BOUNCE_IN;
	}
	ImGui::SameLine();
	if (ImGui::Button("BOUNCE_OUT_26"))
	{
		*eType = EASING_TYPE::BOUNCE_OUT;
	}
	ImGui::SameLine();
	if (ImGui::Button("ELASTIC_INOUT_27"))
	{
		*eType = EASING_TYPE::ELASTIC_INOUT;
	}

}


void CWindow_EffectTool::Update_ImageList_Window()
{
	//const int ObjCount = m_CubeFile.size();

	//char** cObj = new char* [ObjCount];
	//int ObjIndex = 0;

	//for (auto& iter : m_CubeFile)
	//{
	//	vector<wstring>& stateKetyvec = iter.second;
	//	sort(stateKetyvec.begin(), stateKetyvec.end());
	//}
	//for (const auto& iter : m_CubeFile)
	//{
	//	const wstring& folder = iter.first;
	//	const string utf8Str = WstringToUTF8(folder);

	//	cObj[ObjIndex] = new char[utf8Str.length() + 1];
	//	strcpy(cObj[ObjIndex], utf8Str.c_str());

	//	ObjIndex++;
	//}

	////ImGui::Combo(u8"������", &m_iObjChoice, cObj, ObjCount);

	//char* str = cObj[m_iObjChoice];
	//wstring selectedObj = ConverCtoWC2(str);

	//if (m_CubeFile.find(selectedObj) != m_CubeFile.end())
	//{
	//	const vector<wstring>& fileNames = m_CubeFile[selectedObj];

	//	const int StateCount = fileNames.size();
	//	char** cState = new char* [StateCount];
	//	int stateIndex = 0;

	//	for (const wstring& fileName : fileNames)
	//	{
	//		const string utf8Str = WstringToUTF8(fileName);
	//		cState[stateIndex] = new char[utf8Str.length() + 1];
	//		strcpy(cState[stateIndex], utf8Str.c_str());

	//		stateIndex++;
	//	}

	//	//ImGui::Combo(u8"���ϸ�", &m_iStateChoice, cState, StateCount);


	//	if (m_pSelectedImage != nullptr)
	//	{
	//		ImGui::Text(u8"���õ� �̹���");
	//		ImGui::SameLine();
	//		ImGui::Image(m_pSelectedImage, ImVec2(80.f, 80.f));
	//	}

	//	char* StrObj = cObj[m_iObjChoice];
	//	char* StrState = cState[m_iStateChoice];
	//	wstring selectObj = ConverCtoWC2(StrObj);
	//	wstring selectState = ConverCtoWC2(StrState);

	//	int imagesPerRow = 7;
	//	int currentImageCount = 0;

	//	for (const wstring& fileName : fileNames)
	//	{
	//		LPDIRECT3DTEXTURE9 selecteTexture = FindTextureKey(selectedObj, fileName);

	//		if (selecteTexture != nullptr)
	//		{
	//			if (currentImageCount > 0 && currentImageCount % imagesPerRow == 0)
	//			{
	//				ImGui::NewLine();
	//			}

	//			ImVec4 backupButtonHoveredColor = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
	//			ImVec4 backupButtonActiveColor = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);

	//			ImVec4 clickedColor(0.8f, 0.5f, 0.3f, 1.0f);

	//			if (ImGui::ImageButton(selecteTexture, ImVec2(60.f, 60.f)))
	//			{
	//				m_pSelectedImage = selecteTexture;
	//				m_iTextureIndex = 0;
	//				for (wchar_t c : fileName)
	//				{
	//					if (iswdigit(c))
	//					{
	//						m_iTextureIndex = m_iTextureIndex * 10 + (c - L'0');
	//					}
	//				}

	//				ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = clickedColor;
	//				ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = clickedColor;
	//			}

	//			ImGui::SameLine();

	//			currentImageCount++;
	//		}
	//	}
	//	for (int i = 0; i < ObjIndex; ++i)
	//	{
	//		Safe_Delete_Array(cObj[i]);
	//	}
	//	for (int i = 0; i < stateIndex; ++i)
	//	{
	//		Safe_Delete_Array(cState[i]);
	//	}
	//	Safe_Delete_Array(cObj);
	//	Safe_Delete_Array(cState);
	//}




}

void CWindow_EffectTool::Update_LevelSetting_Window()
{
	auto& style = ImGui::GetStyle();

	// ī�޶� ��ġ ǥ��
	ImGui::SeparatorText("Camera");

	if (ImGui::TreeNode(" Camera Info "))
	{
		Show_CameraInfo();

		ImGui::TreePop();
	}


	// ī�޶� ��ġ ����
	if (ImGui::Button("Reset Camera", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25)))
	{
		ReSet_CameraPos();
		m_pCamera->Get_Transform()->Look_At(m_Camera_ResetLookAt);
	}

	if (nullptr != m_pCurEffect)	// ���� ����Ʈ�� �����ϸ�
	{
		if (ImGui::Button("Look Effect", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25)))
		{
			ReSet_CameraPos();

			// ī�޶� ����Ʈ�� �ٶ󺸵���
			m_pCamera->Get_Transform()->Look_At(m_pCurEffect->Get_Position_Vector());
		}
	}

	if (nullptr != m_pModel_Preview) // ���� ������ ���� �����ϸ�
	{
		if (ImGui::Button("Look Model", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25)))
		{
			ReSet_CameraPos();

			// ī�޶� ���� �ٶ󺸵���	
			m_pCamera->Get_Transform()->Look_At(m_pModel_Preview->Get_Position_Vector());
		}
	}


	if (ImGui::DragFloat("Camera Speed", &m_fCameraSpeed, 1.f, 0.f, 100.f))
	{
		m_pCamera->Get_Transform()->Set_Speed(m_fCameraSpeed);
	}


	// �׸���
	ImGui::SeparatorText("Grid");
	if (nullptr == m_pGrid)
	{
		if (ImGui::Button("Create Grid", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25)))	// �׸��� ����
		{
			Ready_Grid();
		}
	}
	else
	{
		// �׸��尡 �����ϸ�
		CGrid::GRID_DESC* pGridDesc = dynamic_cast<CGrid*>(m_pGrid)->Get_Desc();	// �׸��� Desc ������

		if (ImGui::Button(" ON Grid "))	// �׸��� �ѱ�
		{
			pGridDesc->bRender = TRUE;
		}
		ImGui::SameLine();
		if (ImGui::Button(" OFF Grid "))	// �׸��� ����
		{
			pGridDesc->bRender = FALSE;
		}

		// �׸��� �� ����
		if (ImGui::ColorEdit4("Grid Color", m_fColor_Grid, ImGuiColorEditFlags_None))
		{
			pGridDesc->vGridColor.x = m_fColor_Grid[0];
			pGridDesc->vGridColor.y = m_fColor_Grid[1];
			pGridDesc->vGridColor.z = m_fColor_Grid[2];
			pGridDesc->vGridColor.w = m_fColor_Grid[3];
		}

		////m_iShaderPass_Grid
		//if (ImGui::DragInt("ShaderPass_Grid", &m_iShaderPass_Grid, 1, 2))
		//{
		//	m_pGrid->Get_Desc()->iShaderPassIndex = m_iShaderPass_Grid;
		//}

		if (ImGui::Button("Delete Grid", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25)))	// �׸��� ����
		{
			if (nullptr != m_pGrid)
			{
				m_pGrid->Set_Dead(TRUE);
				m_pGrid = nullptr;
			}
		}
	}


	// �ٴ� �޽�
	ImGui::SeparatorText(" Floor ");
	if (nullptr == m_pFloor_Preview)
	{
		if (ImGui::Button("Create Floor", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25)))	// ���ٴ� ����	
		{
			//Ready_Model_Preview(TEXT("Prototype_Component_Model_EffectTool_IntroBoss_Floor"));

			CModel_Preview::MODEL_PREVIEW_DESC	tDesc = {};
			tDesc.strProtoTag = { TEXT("Prototype_GameObject_Model_Preview") };
			tDesc.strModelTag = { TEXT("Prototype_Component_Model_EffectTool_IntroBoss_Floor") };


			CGameObject* pObj = m_pGameInstance->Add_CloneObject_And_Get(LEVEL_TOOL, TEXT("Layer_Model_Preview"), TEXT("Prototype_GameObject_Model_Preview"), &tDesc);
			if (nullptr != pObj)
				m_pFloor_Preview = dynamic_cast<CModel_Preview*>(pObj);

		}
	}
	else
	{
		// �ٴ� �� �����䰡 �����ϸ�	
		CModel_Preview::MODEL_PREVIEW_DESC* pDesc = dynamic_cast<CModel_Preview*>(m_pFloor_Preview)->Get_Desc();

		// �ٴ� �� ���� �̵�
		CTransform* pTransform = m_pFloor_Preview->Get_Transform();
		_float4 vPos = pTransform->Get_State(CTransform::STATE_POSITION);
		ImGui::Text("Floor Pos  : %.2f %.2f %.2f", vPos.x, vPos.y, vPos.z);
		if (ImGui::DragFloat3("Floor_Pos", m_vWorldPosition_Floor, 0.2f))
		{
			m_pFloor_Preview->Set_Position(_float3(m_vWorldPosition_Floor[0], m_vWorldPosition_Floor[1], m_vWorldPosition_Floor[2]));
		}

		// �ٴ� �� ũ�� ����
		_float3 vScaled = pTransform->Get_Scaled();
		ImGui::Text("Floor Scaled  : %.2f %.2f %.2f", vScaled.x, vScaled.y, vScaled.z);
		if (ImGui::DragFloat3("Floor_Scale", m_vScale_Floor, 0.5f))
		{
			if (0.f == m_vScale_Floor[0])
				m_vScale_Floor[0] = 1.f;

			if (0.f == m_vScale_Floor[1])
				m_vScale_Floor[1] = 1.f;

			if (0.f == m_vScale_Floor[2])
				m_vScale_Floor[2] = 1.f;

			pTransform->Set_Scaling(m_vScale_Floor[0], m_vScale_Floor[1], m_vScale_Floor[2]);
		}

		if (ImGui::Button("Delete Floor", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25)))	// �ٴ� �� ���� ��ư
		{
			if (nullptr != m_pFloor_Preview)
			{
				m_pFloor_Preview->Set_Dead(TRUE);
				m_pFloor_Preview = nullptr;
			}
		}

	}



	// ��ī�̹ڽ�
	ImGui::SeparatorText("SkyBox");
	if (nullptr != m_pSky)
	{
		// ��ī�̹ڽ��� �����ϸ�

		if (ImGui::Button("ON Sky"))	// ��ī�̹ڽ� ���� Ŵ
		{
			m_pSky->Set_Render_Tool(TRUE);
		}
		ImGui::SameLine();
		if (ImGui::Button("OFF Sky"))	// ��ī�̹ڽ� ���� ��
		{
			m_pSky->Set_Render_Tool(FALSE);
		}

		// ��ī�̹ڽ� �ؽ�ó ����
		if (ImGui::InputInt(" Sky box Texture ", &m_iSkyTextureIndex, 1))
		{
			_uint iSkyTextureCount = m_pSky->Get_SkyTextureCount();

			if ((_int)iSkyTextureCount - 1 < m_iSkyTextureIndex)
				m_iSkyTextureIndex = iSkyTextureCount-1;

			if (0 > m_iSkyTextureIndex)
				m_iSkyTextureIndex = 0;

			Set_SkyTexture();
		}
	}


	// ũ��񱳿� ��
	ImGui::SeparatorText("Model_Preview");
	if (nullptr == m_pModel_Preview)
	{
		// �÷��̾� �� �ε�
		if (ImGui::Button("Load_Player"))	
		{
			_matrix PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
			m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_Rentier"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Player/Player", PivotMatrix));
		}

		if (ImGui::Button("Create Model_Player"))	// �� ����
		{
			Ready_Model_Preview(TEXT("Prototype_Component_Model_Rentier")); 
		}
		//ImGui::SameLine();
		if (ImGui::Button("Create Model_Boss"))	// �� ����
		{
			Ready_Model_Preview(TEXT("Prototype_Component_Model_VampireCommander"));
		}

		if (ImGui::Button("Create Model_Torch"))	// �� ����
		{
			Ready_Model_Preview(TEXT("Prototype_Component_Model_Effect_Torch"));
		}

	}
	else
	{
		// ��_�����䰡 �����ϸ�	
		CModel_Preview::MODEL_PREVIEW_DESC* pDesc = dynamic_cast<CModel_Preview*>(m_pModel_Preview)->Get_Desc();
		
		// �� ���� �̵�
		CTransform* pTransform = m_pModel_Preview->Get_Transform();
		_float4 vPos = pTransform->Get_State(CTransform::STATE_POSITION);
		ImGui::Text("Model Pos  : %.2f %.2f %.2f", vPos.x, vPos.y, vPos.z);
		if (ImGui::DragFloat3("Model_Pos", m_vWorldPosition_Model, 0.2f))
		{
			m_pModel_Preview->Set_Position(_float3(m_vWorldPosition_Model[0], m_vWorldPosition_Model[1], m_vWorldPosition_Model[2]));
		}

		// �� ���� ȸ��
		_float3 vRotated = pTransform->Get_Rotated();
		ImGui::Text("Model Rotated  : %.2f %.2f %.2f", XMConvertToDegrees(vRotated.x), XMConvertToDegrees(vRotated.y), XMConvertToDegrees(vRotated.z));
		if (ImGui::DragFloat3("Model_Rotate", m_vWorldRotate_Model, 0.1f, 0.f, 360.f))
		{
			pTransform->Rotation_Quaternion(_float3(m_vWorldRotate_Model[0], m_vWorldRotate_Model[1], m_vWorldRotate_Model[2]));
		}


		// �� ũ�� ����
		_float3 vScaled = pTransform->Get_Scaled();
		ImGui::Text("Model Scaled  : %.2f %.2f %.2f", vScaled.x, vScaled.y, vScaled.z);
		if (ImGui::DragFloat3("Model_Scale", m_vScale_Model, 0.5f))
		{
			if (0.f == m_vScale_Model[0])
				m_vScale_Model[0] = 1.f;

			if (0.f == m_vScale_Model[1])
				m_vScale_Model[1] = 1.f;

			if (0.f == m_vScale_Model[2])
				m_vScale_Model[2] = 1.f;

			pTransform->Set_Scaling(m_vScale_Model[0], m_vScale_Model[1], m_vScale_Model[2]);
		}


		if (nullptr != m_pCurEffect)
		{
			// ����Ʈ������ �����ֱ� �׽�Ʈ
			if (ImGui::Button(" ON  Pivot "))
			{
				m_pCurEffect->Set_Object_Owner(m_pModel_Preview);
				m_pCurEffect->Get_Desc()->bParentPivot = TRUE;
			}
			ImGui::SameLine();
			if (ImGui::Button(" OFF Pivot "))
			{
				m_pCurEffect->Get_Desc()->bParentPivot = FALSE;
				m_pCurEffect->Delete_Object_Owner();
			}

			if (ImGui::Button(" Set Position "))
			{
				m_pCurEffect->Set_Position(m_pModel_Preview->Get_Position());
			}

			if (ImGui::Button(" Look_At "))
			{
				m_pCurEffect->Get_Transform()->Look_At(m_pModel_Preview->Get_Position());
			}

		}



		// �ִ� ���� ��쿡�� �ִϸ��̼� ����
		if (CModel::TYPE_ANIM == m_pModel_Preview->Get_Desc()->eType)
		{
			ImGui::SeparatorText("Model Animation");
			if (ImGui::Button("Idle_"))
			{
				if (TEXT("Prototype_Component_Model_Rentier") == pDesc->strModelTag)
				{
					// �÷��̾� ���̵� // Index 8
					m_pModel_Preview->Set_AnimIndex(CPlayer::Player_IdleLoop);
				}

				if (TEXT("Prototype_Component_Model_VampireCommander") == pDesc->strModelTag)
				{
					// ���� ���̵� // Index 9
					m_pModel_Preview->Set_AnimIndex(CVampireCommander::VampireCommander_Idle);
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Attack_"))
			{
				if (TEXT("Prototype_Component_Model_Rentier") == pDesc->strModelTag)
				{
					// �÷��̾� ����
					//m_pModel_Preview->Set_AnimIndex(CPlayer::Player_EnergyWhip_CloseRange_01);

					m_pModel_Preview->Set_AnimIndex(CPlayer::Player_SlamDown_v2);
				}

				if (TEXT("Prototype_Component_Model_VampireCommander") == pDesc->strModelTag)
				{

					m_pModel_Preview->Set_AnimIndex(CVampireCommander::VampireCommander_BloodRange_02_Loop);
				}
			}
		}


		if (ImGui::Button("Delete Model", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25)))		// �� ���� ��ư
		{
			if (nullptr != m_pModel_Preview)
			{
				if(nullptr != m_pCurEffect->Get_Object_Owner())
				{
					// ���� ����Ʈ�� ���ʰ� nullptr�� �ƴϸ� ���ʺ��� ���� ���ְ� �� ����
					m_pCurEffect->Get_Desc()->bParentPivot = FALSE;
					m_pCurEffect->Delete_Object_Owner();
				}

				m_pModel_Preview->Set_Dead(TRUE);
				m_pModel_Preview = nullptr;
				m_pPart_Preview = nullptr;
			}
		}
	}


	// ���콺 ��ġ ǥ��
	ImGui::SeparatorText("Mouse");
	Show_MousePos();


}

void CWindow_EffectTool::Update_EffectList_Window()
{
	auto& style = ImGui::GetStyle();


	// ===========================================================================================================
	// ��� �˾��� ����
	// ===========================================================================================================
	if (ImGui::BeginPopupModal(u8"���:����Ʈ ����", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(u8"����Ʈ�� �����Ͻðڽ��ϱ�?");		// ��� �޽��� ǥ��
		if (ImGui::Button(u8"��"))
		{
			Delete_CurEffectObject();

			ImGui::CloseCurrentPopup();				// ��� �˾� �ݱ�
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"�ƴϿ�"))
		{
			ImGui::CloseCurrentPopup();				// ��� �˾� �ݱ�
		}

		ImGui::EndPopup();

		return;
	}

	if (ImGui::BeginPopupModal(u8"���:��Ʈ ����", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(u8"��Ʈ�� �����Ͻðڽ��ϱ�?");		// ��� �޽��� ǥ��
		if (ImGui::Button(u8"��"))
		{
			Delete_CurPart();
			m_pCurPartEffect = nullptr;

			ImGui::CloseCurrentPopup();				// ��� �˾� �ݱ�
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"�ƴϿ�"))
		{
			ImGui::CloseCurrentPopup();				// ��� �˾� �ݱ�
		}

		ImGui::EndPopup();

		return;
	}



	/* ����ִ� ����Ʈ ��ü ���� */
	ImGui::PushID(1);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0 / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0 / 7.0f, 0.7f, 0.7f));
	if (ImGui::Button("   Create Empty Effect   ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 30)))
	{
		Create_EffectObject(TEXT("Layer_Effect"));
	}
	ImGui::PopStyleColor(3);
	ImGui::PopID();


	ImGui::SeparatorText("");
	if (nullptr == m_pTestEffect)
	{
		if (ImGui::Button(" Test Create "))
		{
			//CEffect* pEffect = EFFECT_MANAGER->Create_Effect(LEVEL_TOOL, "Hit/", "Hit_Distortion.json");
			m_pTestEffect = EFFECT_MANAGER->Create_Effect(LEVEL_TOOL, "Parasiter/", "Yellow_Blood_Test.json");
		}
	}
	else
	{
		if (ImGui::Button(" Test Delete "))
		{
			m_pTestEffect->Set_Dead(TRUE);
			m_pTestEffect = nullptr;
		}
	}
	


	//ImGui::SameLine();
	//if (ImGui::Button("   Create Test Effect Trail   "))
	//{
	//	CEffect* pEffect = EFFECT_MANAGER->Create_Effect_With_Trail("Test_Effect.json", "Test_Effect_Trail.json");
	//}

	//if (nullptr != m_pModel_Preview)
	//{
	//	if (nullptr == m_pTestEffect)
	//	{
	//		if (ImGui::Button("         Skull Test        "))
	//		{
	//			m_pTestEffect = EFFECT_MANAGER->Create_Effect(LEVEL_TOOL, "VampireCommander/Projectile_Range1/", "Projectile_Range1_04.json", m_pModel_Preview);
	//		}
	//	}
	//	else
	//	{
	//		if (ImGui::Button("         Skull Delete        "))
	//		{
	//			m_pTestEffect->Set_Dead(TRUE);
	//			m_pTestEffect = nullptr;
	//		}
	//	}

	//}


	/* ����Ʈ ����Ʈ & ���� ����Ʈ ���� */
	ImGui::SeparatorText(" EFFECT LIST ");
	if (ImGui::ListBox(" Effect List ", &m_iCurEffectIndex, m_szEffectNames, (_int)m_pEffects.size(), (_int)6))
	{
		wstring strCurName = m_pGameInstance->Char_To_Wstring(m_szEffectNames[m_iCurEffectIndex]);
		m_pCurEffect = m_pEffects.find(strCurName)->second;

		/* ���ڿ� �ʱ�ȭ */
		m_iCurPartIndex = (_int)m_CurPartObjects.size();
		if (nullptr != m_szPartNames)
		{
			for (_int i = 0; i < m_iCurPartIndex; ++i)
			{
				m_szPartNames[i] = nullptr;
			}
			m_szPartNames = nullptr;
		}

		Update_CurMembers(strCurName);

		m_iCurPartIndex = (_int)m_CurPartObjects.size();
		m_szPartNames = new char* [m_iCurPartIndex];

		_int iCount = 0;
		for (auto& Pair : m_CurPartObjects)
		{
			const string utf8Str = m_pGameInstance->Wstring_To_UTF8(Pair.first);
			m_szPartNames[iCount] = new char[utf8Str.length() + 1];
			strcpy(m_szPartNames[iCount], utf8Str.c_str());

			iCount++;
		}

		m_iCurPartIndex = 0;
		if (!m_CurPartObjects.empty())
		{
			m_pCurPartEffect = dynamic_cast<CEffect_Void*>(m_CurPartObjects.begin()->second);
			Update_CurParameters_Parts();
		}
		else
			m_pCurPartEffect = nullptr;


		Update_CurParameters();

	} // ����Ʈ ����Ʈ END



	if (nullptr != m_pCurEffect)
	{
		ImGui::Text("Selected Effect : %s ", m_szEffectNames[m_iCurEffectIndex]);

		/* ���� ����Ʈ Ȱ��ȭ */
		if (ImGui::Button("  Active_Effect  "))
		{
			m_pCurEffectDesc->bActive_Tool = TRUE;
		}
		/* ���� ����Ʈ ��Ȱ��ȭ */
		ImGui::SameLine();
		if (ImGui::Button(" UnActive_Effect "))
		{
			m_pCurEffectDesc->bActive_Tool = FALSE;
		}


		if (ImGui::Button("   Reset_Effect   ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25)))
		{
			m_pCurEffect->Init_ReSet_Effect();

			if (nullptr != m_pModel_Preview)
			{
				dynamic_cast<CModel_Preview*>(m_pModel_Preview)->Get_ModelCom()->Reset_Animation();
			}
		}


		if (ImGui::Button(" Delete_Effect ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25)))
		{
			// ��� �˾� ����
			ImGui::OpenPopup(u8"���:����Ʈ ����");
		}

		// =========================================

		ImGui::SeparatorText(" ADD PART EFFECT ");


		ImGui::PushID(2);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.8f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.9f, 0.9f));
		if (ImGui::Button(" Add Particle ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25)))
		{
			Add_Part_Particle();
		}
		ImGui::PopStyleColor(3);
		ImGui::PopID();



		ImGui::PushID(3);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.f, 0.f, 0.8f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.f, 0.f, 0.9f, 0.9f));
		if (ImGui::CollapsingHeader(" Add Mesh (Select Model)"))
		{
			// �׽�Ʈ�� ���� ����Ʈ �޽� ����
			//if (ImGui::TreeNode(" Demo Meshs "))
			//{
			//	if (ImGui::Button("Demo_Xray"))
			//	{
			//		Add_Part_Mesh(TEXT("Prototype_Component_Model_Xray_ManHeavy"));
			//	}

			//	if (ImGui::Button("Demo_Tornado_spline"))
			//	{
			//		Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_Tornado_splineMesh"));
			//	}

			//	if (ImGui::Button("Demo_BloodPoolsRaid"))
			//	{
			//		Add_Part_Mesh(TEXT("Prototype_Component_Model_BloodPoolsRaid"));
			//	}

			//	ImGui::SeparatorText("");

			//	ImGui::TreePop();
			//}

			// ����Ʈ�� ����ƽ �޽� ����
			if (ImGui::TreeNode(" Mesh_Static "))
			{
				if (ImGui::Button("Aoe_Lens"))
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_Aoe_Lens"));

				if (ImGui::Button("Billboard_Circle_00"))
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_Billboard_Circle_00"));

				if (ImGui::Button("Ring"))
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_Ring"));
				
				ImGui::SameLine();
				if (ImGui::Button("Sphere"))
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_Sphere"));

				if (ImGui::Button("Projectile"))
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_Projectile"));

				if (ImGui::Button("Corn"))
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_Corn"));
				
				ImGui::SameLine();
				if (ImGui::Button("ShieldDome"))
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_ShieldDome"));
	
				if (ImGui::Button("Tornado"))
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_Tornado"));
				
				ImGui::SameLine();
				if (ImGui::Button("Tornado_Cream"))
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_Tornado_cream"));

				if (ImGui::Button("Tornado_splineMesh"))
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_Tornado_splineMesh"));

				if (ImGui::Button("WinchesterElectric"))
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_WinchesterElectric"));

				if (ImGui::Button("LightningFast"))
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_LightningFast"));

				ImGui::SameLine();
				if (ImGui::Button("LightningsPack"))
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_LightningsPack"));

				if (ImGui::Button("HemiSphere"))
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_HemiSphere"));

				if (ImGui::Button("Coil"))
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_Coil"));


				ImGui::SeparatorText("");
				ImGui::TreePop();
			}


			// ����(����ü) ����Ʈ �޽� ����
			if (ImGui::TreeNode(" Mesh_Static_Attack "))
			{
				if (ImGui::Button("Slash_00"))
				{
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_Slash_00"));
				}
				ImGui::SameLine();
				if (ImGui::Button("Slash_01"))
				{
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_Slash_01"));
				}

				if (ImGui::Button("Bioball_00"))
				{
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_Bioball_00"));
				}

				if (ImGui::Button("Bioball_01"))
				{
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_Bioball_01"));
				}

				if (ImGui::Button("Bioball_02"))
				{
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_Bioball_02"));
				}

				ImGui::SeparatorText("");

				ImGui::TreePop();
			}


			// ��ƼŬ�� ����Ʈ �޽� ����
			if (ImGui::TreeNode(" Mesh_Particle "))
			{
				if (ImGui::Button("Rock_00"))
				{
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_Rock_00"));
				}
				ImGui::SameLine();
				if (ImGui::Button("Rock_01"))
				{
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_Rock_01"));
				}

				if (ImGui::Button("Rock_02"))
				{
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_Rock_02"));
				}
				ImGui::SameLine();
				if (ImGui::Button("Rock_03"))
				{
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_Rock_03"));
				}

				if (ImGui::Button("LightningParticle_00"))
				{
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_LightningParticle_00"));
				}

				if (ImGui::Button("LightningParticle_01"))
				{
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_LightningParticle_01"));
				}

				if (ImGui::Button("LeafPlane"))
				{
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_LeafPlane"));
				}

				ImGui::SeparatorText("");

				ImGui::TreePop();
			}


			// ��Ʈ�� ������ ����Ʈ �޽� ����
			if (ImGui::TreeNode(" Mesh_VampireCommander "))
			{
				//ImGui::SeparatorText("VampireCommander");
				if (ImGui::Button("Demo_BeastSkull"))
				{
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_BeastSkull"));

				}ImGui::SameLine();
				if (ImGui::Button("Demo_Impact"))
				{
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_Impact"));
				}
				if (ImGui::Button(" Add Bat Test "))
				{
					Add_Part_Mesh_Morph(TEXT("Prototype_Component_Model_Effect_BatStorm_01"), TEXT("Prototype_Component_Model_Effect_BatStorm_02"));
				}
				ImGui::SameLine();
				if (ImGui::Button(" Add Torch Test "))
				{
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_Torch"));
				}

				ImGui::SeparatorText("");

				ImGui::TreePop();
			}

			// �ι�° ������ ����Ʈ �޽� ����
			if (ImGui::TreeNode(" Mesh_Parasiter "))
			{
				if (ImGui::Button("Egg"))
				{
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_Egg"));
				}

				if (ImGui::Button("Egg_Mother"))
				{
					Add_Part_Mesh(TEXT("Prototype_Component_Model_Effect_Egg_Mother"));
				}

				ImGui::SeparatorText("");

				ImGui::TreePop();
			}
		}
		ImGui::PopStyleColor(3);
		ImGui::PopID();


		//ImGui::SameLine();
		//if (ImGui::Button(" Add Rect "))
		//{
		//	Add_Part_Rect();
		//}
		//ImGui::SameLine();
		//if (ImGui::Button(" Add Mesh "))
		//{
		//	//Add_Part_Mesh(TEXT("Prototype_Component_Model_Xray_ManHeavy"));
		//	//Add_Part_Mesh(TEXT("Prototype_Component_Model_splineMesh_tornado"));
		//	//Add_Part_Mesh(TEXT("Prototype_Component_Model_ShieldDome"));
		//	//Add_Part_Mesh(TEXT("Prototype_Component_Model_Particle_Test"));
		//}

		//if (ImGui::Button(" Copy Part "))
		//{
		//	Copy_CurPart();
		//}


		// =========================================

		/* ����Ʈ ��Ʈ������Ʈ ����Ʈ & ���� ��Ʈ������Ʈ ���� */
		ImGui::SeparatorText(" PART LIST ");
		if (ImGui::ListBox(" Part List ", &m_iCurPartIndex, m_szPartNames, (_int)m_CurPartObjects.size(), (_int)6))
		{
			wstring strCurName = m_pGameInstance->Char_To_Wstring(m_szPartNames[m_iCurPartIndex]);

			m_pCurPartEffect = dynamic_cast<CEffect_Void*>(m_pCurEffect->Find_PartObject(strCurName));

			if (nullptr != m_pCurPartEffect)
				Update_CurParameters_Parts();

		}

		if (nullptr != m_pCurPartEffect)
		{
			ImGui::Text("Selected Part : %s ", m_szPartNames[m_iCurPartIndex]);

			CEffect_Void::TYPE_EFFECT eType_Effect = m_pCurVoidDesc->eType_Effect;

			if (CEffect_Void::PARTICLE == eType_Effect)
			{
				ImGui::SameLine();
				ImGui::Text(u8"  /  ��ƼŬ");
			}
			else if (CEffect_Void::MESH == eType_Effect)
			{
				ImGui::SameLine();
				ImGui::Text(u8"  /  �޽� ");
			}


			/* ���� ��ƼŬ Ȱ��ȭ */
			if (ImGui::Button("  Active Part  "))
			{
				if (CEffect_Void::PARTICLE == eType_Effect)
				{
					m_pCurVoidDesc = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_Desc();
					CVIBuffer_Particle* pVIBuffer = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_VIBufferCom();
					m_pParticleBufferDesc = pVIBuffer->Get_Desc();

					m_pCurVoidDesc->bActive_Tool = TRUE;
				}

				if (CEffect_Void::RECT == eType_Effect)
				{
					m_pCurVoidDesc->bActive_Tool = TRUE;
				}

				if (CEffect_Void::MESH == eType_Effect)
				{
					m_pCurVoidDesc->bActive_Tool = TRUE;
				}
			}
			ImGui::SameLine();
			/* ���� ��ƼŬ ��Ȱ��ȭ */
			if (ImGui::Button(" UnActive Part "))
			{
				if (CEffect_Void::PARTICLE == eType_Effect)
				{
					m_pCurVoidDesc = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_Desc();
					CVIBuffer_Particle* pVIBuffer = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_VIBufferCom();
					m_pParticleBufferDesc = pVIBuffer->Get_Desc();

					m_pCurVoidDesc->bActive_Tool = FALSE;
				}
				if (CEffect_Void::RECT == eType_Effect)
				{
					m_pCurVoidDesc->bActive_Tool = FALSE;
				}

				if (CEffect_Void::MESH == eType_Effect)
				{
					m_pCurVoidDesc->bActive_Tool = FALSE;
				}
			}

			if (ImGui::Button(" Reset_Part ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25)))
			{
				if (CEffect_Void::PARTICLE == eType_Effect)
				{
					m_pCurVoidDesc = m_pCurPartEffect->Get_Desc();
					CVIBuffer_Particle* pVIBuffer = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_VIBufferCom();
					m_pParticleBufferDesc = pVIBuffer->Get_Desc();

					dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_VIBufferCom()->ReSet();
				}
				if (CEffect_Void::RECT == eType_Effect)
				{

				}

				if (CEffect_Void::MESH == eType_Effect)
				{
					m_pCurPartEffect->ReSet_Effect();
				}
			}


			if (ImGui::Button(" Delete_Part ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25)))
			{
				// ��� �˾� ����
				ImGui::OpenPopup(u8"���:��Ʈ ����");
			}


		}


		/* Effect Edit Start */
		ImGui::SeparatorText("Effect_Times");
		//if (ImGui::CollapsingHeader("Effect_Times"))
		{
			ImGui::Text(" Waiting  |  LifeTime  |  RemainTime ");
			if (ImGui::DragFloat3("Times_Effect", m_vTimes_Effect, 0.2f, 0.f))
			{
				if (m_vTimes_Effect[0] > m_vTimes_Effect[1])
					m_vTimes_Effect[0] = m_vTimes_Effect[1];

				m_pCurEffectDesc->fWaitingTime = m_vTimes_Effect[0];

				m_pCurEffectDesc->fLifeTime = m_vTimes_Effect[1];
				m_pCurEffectDesc->fRemainTime = m_vTimes_Effect[2];
			}
			ImGui::Text("Total_Time : %.2f", m_vTimes_Effect[0] + m_vTimes_Effect[1] + m_vTimes_Effect[2]);


		}


		if (nullptr != m_pCurPartEffect)
		{
			//ImGui::CollapsingHeader(" Part_Times ");
			ImGui::SeparatorText("Part_Times");
			if (ImGui::TreeNode("Part_Times"))
			{
				CEffect_Void::TYPE_EFFECT eType_Effect = m_pCurPartEffect->Get_Desc()->eType_Effect;

				ImGui::Text(" Waiting  |  LifeTime  |  RemainTime ");
				if (ImGui::DragFloat3("Times_Part", m_vTimes_Part, 0.2f, 0.f))
				{
					m_pCurVoidDesc = m_pCurPartEffect->Get_Desc();

					m_pCurVoidDesc->fWaitingTime = m_vTimes_Part[0];
					m_pCurVoidDesc->fLifeTime = m_vTimes_Part[1];
					m_pCurVoidDesc->fRemainTime = m_vTimes_Part[2];


					if (CEffect_Void::PARTICLE == eType_Effect)
					{
						CVIBuffer_Particle* pVIBuffer = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_VIBufferCom();
						m_pParticleBufferDesc = pVIBuffer->Get_Desc();

					}
					else if (CEffect_Void::RECT == eType_Effect)
					{
						m_pRectDesc = dynamic_cast<CEffect_Rect*>(m_pCurPartEffect)->Get_Desc();

					}


					if (CEffect_Void::MESH == eType_Effect)
					{

					}
				}
				ImGui::Text("Total_Part : %.2f", m_vTimes_Part[0] + m_vTimes_Part[1] + m_vTimes_Part[2]);

				ImGui::TreePop();
			}
		}
		ImGui::SeparatorText("");


		/* ���� Ű�� ���� */
		ImGui::SeparatorText(" LOOP_EFFECT ");
		ImGui::RadioButton(" Loop_Effect ", &m_iLoop, 0);  ImGui::SameLine();
		ImGui::RadioButton("None Loop_Effect", &m_iLoop, 1);
		if (0 == m_iLoop)
			m_pCurEffectDesc->bLoop = TRUE;
		else if (1 == m_iLoop)
			m_pCurEffectDesc->bLoop = FALSE;


		/* ���� Ÿ�� ���ϱ� */
		ImGui::SeparatorText(" DEAD TYPE_EFFECT ");				ImGui::SameLine(); HelpMarker(u8"����Ʈ ���� Ÿ�� ���ϱ�");
		ImGui::RadioButton("DEAD_AUTO", &m_iType_Dead, 0);		ImGui::SameLine(); HelpMarker(u8"����Ʈ�� ������ �ƴϸ�, ����Ʈ�� ������Ÿ���� ������ �� �ڵ����� �׽��ϴ�.");
		ImGui::RadioButton("DEAD_OWNER", &m_iType_Dead, 1);		ImGui::SameLine(); HelpMarker(u8"�ܺο��� �׿���� �׽��ϴ�.(�����̸� ���� ��)");
		//ImGui::RadioButton("DEAD_NONE", &m_iType_Dead, 2);		
		if (0 == m_iType_Dead)
			m_pCurEffectDesc->eType_Dead = CEffect::DEAD_AUTO;
		else if (1 == m_iType_Dead)
			m_pCurEffectDesc->eType_Dead = CEffect::DEAD_OWNER;
		//else if (2 == m_iType_Dead)
		//	m_pCurEffectDesc->eType_Dead = CEffect::DEAD_NONE;


	}
}

void CWindow_EffectTool::Update_EffectTrail_Window()
{
	auto& style = ImGui::GetStyle();

	if (nullptr != m_pModel_Preview)
	{
		if (ImGui::CollapsingHeader(u8"�ε� �׽�Ʈ ����!!"))
		{
			if (ImGui::Button("Test Ready Trail"))
			{
				m_pPart_Preview->Ready_Trail(LEVEL_TOOL, "Test_Trail.json");
			}
		}
		ImGui::SeparatorText("");
	}


	// Ʈ���� �߰�
	if (nullptr == m_pTrail)
	{
		// Ʈ���� ��ü�� nullptr�̸� Ʈ���� ���� ����

		ImGui::PushID(5);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1 / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1 / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1 / 7.0f, 0.8f, 0.8f));
		if (ImGui::Button(" Create Trail ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 30)))
		{
			Create_Trail();
		}
		ImGui::PopStyleColor(3);
		ImGui::PopID();

	}
	else
	{
		// Ʈ���� ��ü�� nullptr�� �ƴϸ�
		//m_pTrail->Tick_Trail(fTimeDelta, );

		if (ImGui::Button(" Delete Trail ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 30)))
		{
			Delete_Trail();
			return;
		}



		/* ���, �Ͻ����� (Ʈ���ϸ�) */
		ImGui::SeparatorText("Play & Pause_Trail");
		if (ImGui::Button("   Play_Trail   ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25)))
		{
			m_pTrail->Set_Pause(FALSE);
		}
		if (ImGui::Button("   Pause_Trail  ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25)))
		{
			m_pTrail->Set_Pause(TRUE);
		}
	

		// =============================================================================================================================================================

		if (nullptr != m_pModel_Preview) // ���� �����ϸ�
		{
			ImGui::SeparatorText("");

			if (ImGui::CollapsingHeader("Trail For Model"))	// �𵨿� Ʈ���� �޾��ֱ�
			{
				// Model_Preview ��ũ���� ������
				CModel_Preview::MODEL_PREVIEW_DESC* pDesc = dynamic_cast<CModel_Preview*>(m_pModel_Preview)->Get_Desc();


				// ���� ���� ��Ʈ�� Ʈ���� �޾��ֱ�
				ImGui::Text(u8"�𵨿� Ʈ���� �ޱ� ��ư");
				if (ImGui::Button("Set Trail_On Model", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25)))
				{
					m_pPart_Preview->Set_Trail(m_pTrail);
				}


				/* ���, �Ͻ����� (��, Ʈ���� �� ��) */
				if (ImGui::Button("   Play_Model & Trail   "))
				{
					m_pModel_Preview->Get_Desc()->bPlay = TRUE;
					m_pTrail->Set_Pause(FALSE);
				}
				
				if (ImGui::Button("   Stop_Model & Trail   "))
				{
					m_pModel_Preview->Get_Desc()->bPlay = FALSE;
					m_pTrail->Set_Pause(TRUE);
				}


				// �ִϸ��̼� ����
				ImGui::SeparatorText("Model Animation");
				if (ImGui::Button("Idle_For Trail"))
				{
					if (TEXT("Prototype_Component_Model_Rentier") == pDesc->strModelTag)
					{
						// �÷��̾� ���̵� 
						m_pModel_Preview->Set_AnimIndex(CPlayer::Player_IdleLoop);
					}

					if (TEXT("Prototype_Component_Model_VampireCommander") == pDesc->strModelTag)
					{
						// ���� ���̵� // Index 9
						m_pModel_Preview->Set_AnimIndex(CVampireCommander::VampireCommander_Idle);
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Attack_For Trail"))
				{
					if (TEXT("Prototype_Component_Model_Rentier") == pDesc->strModelTag)
					{
						// �÷��̾� ����
						//m_pModel_Preview->Set_AnimIndex(CPlayer::Player_EnergyWhip_CloseRange_01);
						m_pModel_Preview->Set_AnimIndex(CPlayer::Player_SlamDown_v2);
					}

					if (TEXT("Prototype_Component_Model_VampireCommander") == pDesc->strModelTag)
					{
						// ���� VampireCommander_AttackMelee_02 // Index 55
						m_pModel_Preview->Set_AnimIndex(CVampireCommander::VampireCommander_BloodRange_02_Loop);
					}
				}


				ImGui::SeparatorText("");
			}
		}
		else
		{
			ImGui::Text(u8"Ʈ������ �� ���� �������ּ���.");
			ImGui::Text(u8"ȯ�� ���� â - Create Model_Boss ��õ");
		}
	}


	// =============================================================================================================================================================

	if (nullptr != m_pCurEffect) // ���� ����Ʈ�� ������
	{
		ImGui::SeparatorText("");

		if (ImGui::CollapsingHeader("Trail For Effect"))	// ����Ʈ�� Ʈ���� �޾��ֱ�
		{
			// ����Ʈ�� Ʈ���� �޾��ֱ�
			ImGui::Text(u8"����Ʈ�� Ʈ���� �ޱ� ��ư");
			if (ImGui::Button("Set Trail_On Effect", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 25)))
			{
				m_pCurEffect->Set_Trail(m_pTrail);
			}

			/* ���, �Ͻ����� */
			if (ImGui::Button("   Play_Effect & Trail    "))
			{
				m_pCurEffectDesc = m_pCurEffect->Get_Desc();
				m_pCurEffectDesc->bPlay = TRUE;

				m_pTrail->Set_Pause(FALSE);
			}
	
			if (ImGui::Button("   Pause_Effect & Trail   "))
			{
				m_pCurEffectDesc = m_pCurEffect->Get_Desc();
				m_pCurEffectDesc->bPlay = FALSE;

				m_pTrail->Set_Pause(TRUE);
			}


			ImGui::SeparatorText("");
		}
	}



}

void CWindow_EffectTool::Update_EffectTransform_Window()
{
	if (nullptr != m_pCurEffect)
	{
		ImGui::SeparatorText("");
		ImGui::PushID(6);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.3f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.4f, 0.9f, 0.9f));
		if (ImGui::CollapsingHeader("Effect Transform"))
		{
			/* ����Ʈ ��ġ ǥ�� & Ʈ������ */
			_float4 vPos = m_pCurEffect->Get_Transform()->Get_State(CTransform::STATE_POSITION);
			ImGui::Text("Effect Pos  : %.2f %.2f %.2f", vPos.x, vPos.y, vPos.z);

			CTransform* pTransform = m_pCurEffect->Get_Transform();
			if (ImGui::DragFloat3("Effect_Pos", m_vWorldPosition_Effect, 0.5f))
			{
				m_pCurEffect->Set_Position(_float3(m_vWorldPosition_Effect[0], m_vWorldPosition_Effect[1], m_vWorldPosition_Effect[2]));
			}

			_float3 vScaled = pTransform->Get_Scaled();
			ImGui::Text("Effect Scaled  : %.2f %.2f %.2f", vScaled.x, vScaled.y, vScaled.z);
			if (ImGui::DragFloat3("Effect_Scale", m_vScale_Effect, 0.5f))
			{
				if (0.f == m_vScale_Effect[0])
					m_vScale_Effect[0] = 1.f;

				if (0.f == m_vScale_Effect[1])
					m_vScale_Effect[1] = 1.f;

				if (0.f == m_vScale_Effect[2])
					m_vScale_Effect[2] = 1.f;

				pTransform->Set_Scaling(m_vScale_Effect[0], m_vScale_Effect[1], m_vScale_Effect[2]);
			}

			_float3 vRotated = pTransform->Get_Rotated();
			ImGui::Text("Effect Rotated  : %.2f %.2f %.2f", XMConvertToDegrees(vRotated.x), XMConvertToDegrees(vRotated.y), XMConvertToDegrees(vRotated.z));
			//Rotation_Quaternion
			if (ImGui::DragFloat3("Effect_Rotate", m_vRotate_Effect, 0.1f, 0.f, 360.f))
			{
				pTransform->Rotation_Quaternion(_float3(m_vRotate_Effect[0], m_vRotate_Effect[1], m_vRotate_Effect[2]));
			}



			if (nullptr != m_pCurPartEffect)
			{
				ImGui::SeparatorText("");

				/* ��Ʈ ��ġ ǥ�� & Ʈ������ */
				if (ImGui::TreeNode("Part_Transform"))
				{
					CTransform* pPartTransform = m_pCurPartEffect->Get_Transform();

					_float4 vPos = m_pCurPartEffect->Get_Transform()->Get_State(CTransform::STATE_POSITION);
					ImGui::Text("Part Pos  : %.2f %.2f %.2f", vPos.x, vPos.y, vPos.z);

					if (ImGui::DragFloat3("Part_Pos", m_vWorldPosition_Part, 0.5f))
					{
						m_pCurPartEffect->Set_Position(_float3(m_vWorldPosition_Part[0], m_vWorldPosition_Part[1], m_vWorldPosition_Part[2]));
					}
					_float3 vScaled = pPartTransform->Get_Scaled();
					ImGui::Text("Part Scaled  : %.2f %.2f %.2f", vScaled.x, vScaled.y, vScaled.z);
					if (ImGui::DragFloat3("Part_Scale", m_vScale_Part, 0.1f, 0.1f))
					{
						if (0.f == m_vScale_Part[0])
							m_vScale_Part[0] = 1.f;

						if (0.f == m_vScale_Part[1])
							m_vScale_Part[1] = 1.f;

						if (0.f == m_vScale_Part[2])
							m_vScale_Part[2] = 1.f;

						m_pCurPartEffect->Get_Transform()->Set_Scaling(m_vScale_Part[0], m_vScale_Part[1], m_vScale_Part[2]);
					}

					_float3 vRotated = pPartTransform->Get_Rotated();
					ImGui::Text("Part Rotated  : %.2f %.2f %.2f", XMConvertToDegrees(vRotated.x), XMConvertToDegrees(vRotated.y), XMConvertToDegrees(vRotated.z));
					//Rotation_Quaternion
					if (ImGui::DragFloat3("Part_Rotate", m_vRotate_Part, 0.1f, 0.f, 360.f))
					{
						pPartTransform->Rotation_Quaternion(_float3(m_vRotate_Part[0], m_vRotate_Part[1], m_vRotate_Part[2]));
					}

					ImGui::TreePop();

				}
			}
		}
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}

}

HRESULT CWindow_EffectTool::Create_EffectObject(const wstring& strLayerTag, CGameObject* pOwner)
{
	_uint iNextLevel = m_pGameInstance->Get_NextLevel();

	CEffect::EFFECT_DESC	tEffectDesc = {};
	tEffectDesc.fSpeedPerSec = { 5.f };
	tEffectDesc.fRotationPerSec = { XMConvertToRadians(50.0f) };

	tEffectDesc.bActive_Tool = { TRUE };
	tEffectDesc.bPlay		 = { TRUE };
	tEffectDesc.bLoop		 = { TRUE };
	tEffectDesc.bFinished	 = { FALSE };
	tEffectDesc.bReverse	 = { FALSE };
	tEffectDesc.bRender		 = { TRUE };

	tEffectDesc.fTimeAcc = { 0.f };
	tEffectDesc.fWaitingAcc = { 0.f };
	tEffectDesc.fSequenceAcc = { 0.f };

	tEffectDesc.fLifeTime = { 5.f };
	tEffectDesc.fWaitingTime = { 0.f };
	tEffectDesc.fRemainTime = { 0.f };
	tEffectDesc.fSequenceTime = tEffectDesc.fWaitingTime + tEffectDesc.fLifeTime + tEffectDesc.fRemainTime;
	tEffectDesc.fLifeTimeRatio = min(1.f, tEffectDesc.fTimeAcc / tEffectDesc.fLifeTime);


	if (nullptr != pOwner)
	{
		tEffectDesc.bParentPivot = TRUE;
		tEffectDesc.matPivot = pOwner->Get_Transform()->Get_WorldFloat4x4();
		XMStoreFloat4x4(&tEffectDesc.matCombined, XMMatrixIdentity());
	}
	else
	{
		tEffectDesc.bParentPivot = FALSE;
		XMStoreFloat4x4(&tEffectDesc.matPivot, XMMatrixIdentity());
		XMStoreFloat4x4(&tEffectDesc.matCombined, XMMatrixIdentity());
	}

	CEffect* pEffect = dynamic_cast<CEffect*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_TOOL, strLayerTag, TEXT("Prototype_GameObject_Effect"), &tEffectDesc));

#pragma region �̸� �ٿ��� �����̳ʿ� �ֱ�
	wstring strName = TEXT("");
	wstring strFrontName = TEXT("Effect");

	_int iMaxNum = -1;
	wstring strPin = TEXT("");


	for (auto& iter : m_pEffects)
	{
		if (nullptr == iter.second)
			continue;

		if (strFrontName == m_pGameInstance->Remove_LastNumChar(iter.first, 4))
		{
			_int iPinNum = stoi(m_pGameInstance->Get_LastNumChar(iter.first, 3));

			if (iMaxNum < iPinNum)
				iMaxNum = iPinNum;
		}
	}
	/* �ִ��� -1�̶�� ���� �ش� �̸��� ������ �����Ƿ� ������ȣ�� 000���� �����Ѵ�. */
	if (-1 == iMaxNum)
		strName = strFrontName + L"_000";
	else /* �ƴ϶�� �ִ񰪿� + 1�� �Ͽ� ���� ��ȣ�� �����Ѵ�. */
	{
		_int iPinNum = iMaxNum + 1;

		if (0 == iPinNum / 10)
			strPin = L"_00" + to_wstring(iPinNum);
		else if (0 == iPinNum / 100)
			strPin = L"_0" + to_wstring(iPinNum);
		else
			strPin = L"_" + to_wstring(iPinNum);

		strName = strFrontName + strPin;
	}
	m_pEffects.emplace(strName, pEffect);
	m_pCurEffect = pEffect;
	m_pCurEffectDesc = pEffect->Get_Desc();

#pragma endregion


#pragma region �̸� ����Ʈ �ʱ�ȭ&������Ʈ

	m_iCurEffectIndex = (_int)m_pEffects.size();
	/* ���ڿ� �ʱ�ȭ */
	if (nullptr != m_szEffectNames)
	{
		for (_int i = 0; i < m_iCurEffectIndex; ++i)
		{
			m_szEffectNames[i] = nullptr;
		}
		m_szEffectNames = nullptr;
	}
	m_szEffectNames = new char* [m_iCurEffectIndex];
	_int iCount = 0;
	for (auto& Pair : m_pEffects)
	{
		const string utf8Str = m_pGameInstance->Wstring_To_UTF8(Pair.first);
		m_szEffectNames[iCount] = new char[utf8Str.length() + 1];
		strcpy(m_szEffectNames[iCount], utf8Str.c_str());

		iCount++;
	}
	m_iCurEffectIndex -= 1;
#pragma endregion


	Update_CurMembers(m_pGameInstance->Char_To_Wstring(m_szEffectNames[m_iCurEffectIndex]));

	return S_OK;
}

HRESULT CWindow_EffectTool::Add_Part_Particle()
{
	if (nullptr != m_pCurEffect)
	{
		CEffect_Void::EFFECTVOID_DESC	tVoidDesc = {};
		tVoidDesc.fSpeedPerSec = { 5.f };
		tVoidDesc.fRotationPerSec = { XMConvertToRadians(50.0f) };

		tVoidDesc.eType_Effect = CEffect_Void::PARTICLE;

		tVoidDesc.strTextureTag[CEffect_Particle::TEXTURE_DIFFUSE] = TEXT("Prototype_Component_Texture_Effect_Diffuse");
		tVoidDesc.iTextureIndex[CEffect_Particle::TEXTURE_DIFFUSE] = { 1 };

		tVoidDesc.strTextureTag[CEffect_Void::TEXTURE_SPRITE] = TEXT("");
		tVoidDesc.iTextureIndex[CEffect_Void::TEXTURE_SPRITE] = { 0 };

		tVoidDesc.strTextureTag[CEffect_Particle::TEXTURE_MASK] = TEXT("Prototype_Component_Texture_Effect_Mask");
		tVoidDesc.iTextureIndex[CEffect_Particle::TEXTURE_MASK] = { 0 /*1*/ };

		tVoidDesc.strTextureTag[CEffect_Particle::TEXTURE_NOISE] = TEXT("Prototype_Component_Texture_Effect_Noise");
		tVoidDesc.iTextureIndex[CEffect_Particle::TEXTURE_NOISE] = { 0 };

		tVoidDesc.iRenderGroup = { CRenderer::RENDER_EFFECT };

		tVoidDesc.iShaderPassIndex = { 0 };

		tVoidDesc.bBillBoard = { TRUE };

		tVoidDesc.fUV_RotDegree = { 0.f };

		tVoidDesc.bPlay				= { TRUE };
		tVoidDesc.bUseSpriteAnim	= { FALSE };
		tVoidDesc.bUseRigidBody		= { TRUE };

		tVoidDesc.iCurNumInstance = { 1000 };


#pragma region ����Ʈ ���ڿ� ����


#pragma region �±� ����� ����

		// ��Ʈ �±� ����� ���� ========================================================================================================
		wstring strName = TEXT("");
		wstring strFrontName = TEXT("Part");

		wstring strPin = TEXT("");
		_int iMaxNum = -1;

		vector<_int> vecNumbers;

		for (auto& iter : m_CurPartObjects)
		{
			if (nullptr == iter.second)
				continue;

			wstring strTemp = dynamic_cast<CEffect_Void*>(iter.second)->Get_Desc()->strPartTag;

			// "Part"�� �����ϴ� �̸��̸�, ������ 3�ڸ��� ������ ���
			if (strFrontName == m_pGameInstance->Remove_LastNumChar(strTemp, 4))		//  "Part"  ("Part_000") 
			{
				_int iPinNum = stoi(m_pGameInstance->Get_LastNumChar(strTemp, 3));		//  "000"  ("Part_000") 

				vecNumbers.push_back(iPinNum);

				if (iMaxNum < iPinNum)
					iMaxNum = iPinNum;
			}
		}

		// ��ȣ���� ����
		sort(vecNumbers.begin(), vecNumbers.end());

		// �߰��� ���� ��ȣ�� ã�Ƽ� ó��
		_int iMissingNumber = -1;
		for (_int i = 0; i < vecNumbers.size(); ++i)
		{
			if (vecNumbers[i] != i)
			{
				iMissingNumber = i;
				break;
			}
		}

		// �߰��� ���� ��ȣ�� �ִٸ� �� ��ȣ�� ���
		if (iMissingNumber != -1)
		{
			// ���� ��ȣ�� 3�ڸ��� ���߾� ���ڿ��� ��ȯ
			wstring strPin = L"";
			if (iMissingNumber < 10)
				strPin = L"00" + to_wstring(iMissingNumber);
			else if (iMissingNumber < 100)
				strPin = L"0" + to_wstring(iMissingNumber);
			else
				strPin = to_wstring(iMissingNumber);

			strName = strFrontName + L"_" + strPin;
		}
		else
		{
			// ���� ��ȣ�� ������ ���� ū ��ȣ�� 1�� ���Ͽ� ���ο� ��ȣ�� ����
			int iPinNum = iMaxNum + 1;

			// ���ο� ��ȣ�� 3�ڸ��� ���߾� ���ڿ��� ��ȯ
			wstring strPin = L"";
			if (iPinNum < 10)
				strPin = L"00" + to_wstring(iPinNum);
			else if (iPinNum < 100)
				strPin = L"0" + to_wstring(iPinNum);
			else
				strPin = to_wstring(iPinNum);

			strName = strFrontName + L"_" + strPin;
		}
		// ��Ʈ �±� ����� ��   ========================================================================================================


		// ����Ʈ�� �� Ű ����� ���� =====================================================================================================
		wstring strName_Map = TEXT("");
		wstring strFrontName_Map = TEXT("Part");

		wstring strPin_Map = TEXT("");
		_int iMaxNum_Map = -1;

		for (auto& iter : m_CurPartObjects)
		{
			if (nullptr == iter.second)
				continue;

			if (strFrontName_Map == m_pGameInstance->Remove_LastNumChar(iter.first, 4))
			{
				_int iPinNum = stoi(m_pGameInstance->Get_LastNumChar(iter.first, 3));

				if (iMaxNum_Map < iPinNum)
					iMaxNum_Map = iPinNum;
			}
		}
		/* �ִ��� -1�̶�� ���� �ش� �̸��� ������ �����Ƿ� ������ȣ�� 000���� �����Ѵ�. */
		if (-1 == iMaxNum_Map)
			strName_Map = strFrontName_Map + L"_000";
		else /* �ƴ϶�� �ִ񰪿� + 1�� �Ͽ� ���� ��ȣ�� �����Ѵ�. */
		{
			_int iPinNum = iMaxNum_Map + 1;

			if (0 == iPinNum / 10)
				strPin_Map = L"_00" + to_wstring(iPinNum);
			else if (0 == iPinNum / 100)
				strPin_Map = L"_0" + to_wstring(iPinNum);
			else
				strPin_Map = L"_" + to_wstring(iPinNum);

			strName_Map = strFrontName_Map + strPin_Map;
		}
		// ����Ʈ�� �� Ű ����� �� =======================================================================================================

#pragma endregion �±� ����� ��


		// ����Ʈ�� �� Ű ����
		tVoidDesc.strPartTag = strName_Map;
		tVoidDesc.strProtoTag = TEXT("Prototype_GameObject_Effect_Particle");
		FAILED_CHECK(m_pCurEffect->Add_PartObject(TEXT("Prototype_GameObject_Effect_Particle"), strName_Map, &tVoidDesc));


		// ���� ��Ʈ ������Ʈ ����
		m_CurPartObjects = *m_pCurEffect->Get_PartObjects();
		m_pCurPartEffect = dynamic_cast<CEffect_Particle*>(m_pCurEffect->Find_PartObject(strName_Map));


		// ���� ���ĵ� ��Ʈ �±� �ٲ��ֱ�
		m_pCurVoidDesc = m_pCurPartEffect->Get_Desc();
		m_pCurVoidDesc->strPartTag = strName; // ��Ʈ �±� ����


		m_iCurPartIndex = (_int)m_CurPartObjects.size();
		/* ���ڿ� �ʱ�ȭ */
		if (nullptr != m_szPartNames)
		{
			for (_int i = 0; i < m_iCurPartIndex; ++i)
			{
				m_szPartNames[i] = nullptr;
			}
			m_szPartNames = nullptr;
		}

		m_szPartNames = new char* [m_iCurPartIndex];

		_int iCount = 0;
		for (auto& Pair : m_CurPartObjects)
		{
			const string utf8Str = m_pGameInstance->Wstring_To_UTF8(Pair.first);
			m_szPartNames[iCount] = new char[utf8Str.length() + 1];
			strcpy(m_szPartNames[iCount], utf8Str.c_str());

			iCount++;
		}
		

		for (_int i = 0; i <= m_iCurPartIndex; ++i)
		{
			wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			wstring wsPartName = converter.from_bytes(m_szPartNames[i]);

			if (wsPartName == strName)
			{
				m_iCurPartIndex = i;
				break; // �ش� �̸��� ã�����Ƿ� �� �̻� �ݺ��� �ʿ䰡 ����
			}
		}

#pragma endregion ����Ʈ ���ڿ� ���� ��

		Update_CurParameters_Parts();

	}

	return S_OK;
}

HRESULT CWindow_EffectTool::Add_Part_Rect()
{
	if (nullptr != m_pCurEffect)
	{
		CEffect_Void::EFFECTVOID_DESC tVoidDesc = {};
		tVoidDesc.fSpeedPerSec = { 5.f };
		tVoidDesc.fRotationPerSec = { XMConvertToRadians(50.0f) };

		tVoidDesc.eType_Effect = CEffect_Void::RECT;

		tVoidDesc.strTextureTag[CEffect_Void::TEXTURE_DIFFUSE] = TEXT("Prototype_Component_Texture_Effect_Diffuse");
		tVoidDesc.iTextureIndex[CEffect_Void::TEXTURE_DIFFUSE] = { 0 };

		tVoidDesc.strTextureTag[CEffect_Void::TEXTURE_SPRITE] = TEXT("");
		tVoidDesc.iTextureIndex[CEffect_Void::TEXTURE_SPRITE] = { 0 };

		tVoidDesc.strTextureTag[CEffect_Void::TEXTURE_MASK] = TEXT("Prototype_Component_Texture_Effect_Mask");
		tVoidDesc.iTextureIndex[CEffect_Void::TEXTURE_MASK] = { 0 };

		tVoidDesc.strTextureTag[CEffect_Void::TEXTURE_NOISE] = TEXT("Prototype_Component_Texture_Effect_Noise");
		tVoidDesc.iTextureIndex[CEffect_Void::TEXTURE_NOISE] = { 0 };


		tVoidDesc.iShaderPassIndex = { 0 };
		tVoidDesc.vColor_Mul = { 1.f, 1.f, 1.f, 1.f };

		tVoidDesc.iRenderGroup = { CRenderer::RENDER_EFFECT };


		tVoidDesc.bPlay = { TRUE };


#pragma region ����Ʈ ���ڿ� ����
		wstring strName = TEXT("");
		wstring strFrontName = TEXT("Part");

		_int iMaxNum = -1;
		wstring strPin = TEXT("");

		for (auto& iter : m_CurPartObjects)
		{
			if (nullptr == iter.second)
				continue;

			if (strFrontName == m_pGameInstance->Remove_LastNumChar(iter.first, 4))
			{
				_int iPinNum = stoi(m_pGameInstance->Get_LastNumChar(iter.first, 3));

				if (iMaxNum < iPinNum)
					iMaxNum = iPinNum;
			}
		}
		/* �ִ��� -1�̶�� ���� �ش� �̸��� ������ �����Ƿ� ������ȣ�� 000���� �����Ѵ�. */
		if (-1 == iMaxNum)
			strName = strFrontName + L"_000";
		else /* �ƴ϶�� �ִ񰪿� + 1�� �Ͽ� ���� ��ȣ�� �����Ѵ�. */
		{
			_int iPinNum = iMaxNum + 1;

			if (0 == iPinNum / 10)
				strPin = L"_00" + to_wstring(iPinNum);
			else if (0 == iPinNum / 100)
				strPin = L"_0" + to_wstring(iPinNum);
			else
				strPin = L"_" + to_wstring(iPinNum);

			strName = strFrontName + strPin;
		}

		tVoidDesc.strPartTag = strName;
		tVoidDesc.strProtoTag = TEXT("Prototype_GameObject_Effect_Rect");
		FAILED_CHECK(m_pCurEffect->Add_PartObject(TEXT("Prototype_GameObject_Effect_Rect"), strName, &tVoidDesc));

		m_CurPartObjects = *m_pCurEffect->Get_PartObjects();
		m_pCurPartEffect = dynamic_cast<CEffect_Rect*>(m_pCurEffect->Find_PartObject(strName));

		m_pCurVoidDesc = m_pCurPartEffect->Get_Desc();

		m_iCurPartIndex = (_int)m_CurPartObjects.size();
		/* ���ڿ� �ʱ�ȭ */
		if (nullptr != m_szPartNames)
		{
			for (_int i = 0; i < m_iCurPartIndex; ++i)
			{
				m_szPartNames[i] = nullptr;
			}
			m_szPartNames = nullptr;
		}

		m_szPartNames = new char* [m_iCurPartIndex];

		_int iCount = 0;
		for (auto& Pair : m_CurPartObjects)
		{
			const string utf8Str = m_pGameInstance->Wstring_To_UTF8(Pair.first);
			m_szPartNames[iCount] = new char[utf8Str.length() + 1];
			strcpy(m_szPartNames[iCount], utf8Str.c_str());

			iCount++;
		}
		m_iCurPartIndex -= 1;

		Update_CurParameters_Parts();
#pragma endregion

	}

	return S_OK;
}

HRESULT CWindow_EffectTool::Add_Part_Mesh(wstring strModelTag)
{
	if (nullptr != m_pCurEffect)
	{
		CEffect_Void::EFFECTVOID_DESC tVoidDesc = {};
		tVoidDesc.fSpeedPerSec = { 5.f };
		tVoidDesc.fRotationPerSec = { XMConvertToRadians(50.0f) };

		tVoidDesc.eType_Effect = { CEffect_Void::MESH };

		tVoidDesc.strTextureTag[CEffect_Void::TEXTURE_DIFFUSE] = TEXT("Prototype_Component_Texture_Effect_Diffuse");
		tVoidDesc.iTextureIndex[CEffect_Void::TEXTURE_DIFFUSE] = { 1 };

		tVoidDesc.strTextureTag[CEffect_Void::TEXTURE_MASK] = TEXT("Prototype_Component_Texture_Effect_Mask");
		tVoidDesc.iTextureIndex[CEffect_Void::TEXTURE_MASK] = { 0 };

		tVoidDesc.strTextureTag[CEffect_Void::TEXTURE_NOISE] = TEXT("Prototype_Component_Texture_Effect_Noise");
		tVoidDesc.iTextureIndex[CEffect_Void::TEXTURE_NOISE] = { 0 };

		tVoidDesc.iShaderPassIndex = { 1 };

		tVoidDesc.iRenderGroup = { CRenderer::RENDER_EFFECT };

		tVoidDesc.bBillBoard = { FALSE };


		tVoidDesc.strModelTag[CVIBuffer_Effect_Model_Instance::MORPH_01] = strModelTag;
		tVoidDesc.strModelTag[CVIBuffer_Effect_Model_Instance::MORPH_02] = TEXT("");


		tVoidDesc.bPlay				= { TRUE };
		tVoidDesc.bUseRigidBody		= { TRUE };
		tVoidDesc.iCurNumInstance	= { 1000 };

		CEffect_Instance::EFFECT_INSTANCE_DESC tInstanceDesc = {};
		//tInstanceDesc.vRimColor = { 2.f, 10.f, 255.f, 255.f };
		//tInstanceDesc.fRimPower = { 950.f };

#pragma region ����Ʈ ���ڿ� ����


#pragma region �±� ����� ����

		// ��Ʈ �±� ����� ���� ========================================================================================================
		wstring strName = TEXT("");
		wstring strFrontName = TEXT("Part");

		wstring strPin = TEXT("");
		_int iMaxNum = -1;

		vector<_int> vecNumbers;

		for (auto& iter : m_CurPartObjects)
		{
			if (nullptr == iter.second)
				continue;

			wstring strTemp = dynamic_cast<CEffect_Void*>(iter.second)->Get_Desc()->strPartTag;

			// "Part"�� �����ϴ� �̸��̸�, ������ 3�ڸ��� ������ ���
			if (strFrontName == m_pGameInstance->Remove_LastNumChar(strTemp, 4))		//  "Part"  ("Part_000") 
			{
				_int iPinNum = stoi(m_pGameInstance->Get_LastNumChar(strTemp, 3));		//  "000"  ("Part_000") 
				
				vecNumbers.push_back(iPinNum);

				if (iMaxNum < iPinNum)
					iMaxNum = iPinNum;
			}
		}

		// ��ȣ���� ����
		sort(vecNumbers.begin(), vecNumbers.end());

		// �߰��� ���� ��ȣ�� ã�Ƽ� ó��
		_int iMissingNumber = -1;
		for (_int i = 0; i < vecNumbers.size(); ++i)
		{
			if (vecNumbers[i] != i)
			{
				iMissingNumber = i;
				break;
			}
		}

		// �߰��� ���� ��ȣ�� �ִٸ� �� ��ȣ�� ���
		if (iMissingNumber != -1)
		{
			// ���� ��ȣ�� 3�ڸ��� ���߾� ���ڿ��� ��ȯ
			wstring strPin = L"";
			if (iMissingNumber < 10)
				strPin = L"00" + to_wstring(iMissingNumber);
			else if (iMissingNumber < 100)
				strPin = L"0" + to_wstring(iMissingNumber);
			else
				strPin = to_wstring(iMissingNumber);

			strName = strFrontName + L"_" + strPin;
		}
		else
		{
			// ���� ��ȣ�� ������ ���� ū ��ȣ�� 1�� ���Ͽ� ���ο� ��ȣ�� ����
			int iPinNum = iMaxNum + 1;

			// ���ο� ��ȣ�� 3�ڸ��� ���߾� ���ڿ��� ��ȯ
			wstring strPin = L"";
			if (iPinNum < 10)
				strPin = L"00" + to_wstring(iPinNum);
			else if (iPinNum < 100)
				strPin = L"0" + to_wstring(iPinNum);
			else
				strPin = to_wstring(iPinNum);

			strName = strFrontName + L"_" + strPin;
		}
		// ��Ʈ �±� ����� ��   ========================================================================================================


		// ����Ʈ�� �� Ű ����� ���� =====================================================================================================
		wstring strName_Map = TEXT("");
		wstring strFrontName_Map = TEXT("Part");

		wstring strPin_Map = TEXT("");
		_int iMaxNum_Map = -1;

		for (auto& iter : m_CurPartObjects)
		{
			if (nullptr == iter.second)
				continue;

			if (strFrontName_Map == m_pGameInstance->Remove_LastNumChar(iter.first, 4))
			{
				_int iPinNum = stoi(m_pGameInstance->Get_LastNumChar(iter.first, 3));

				if (iMaxNum_Map < iPinNum)
					iMaxNum_Map = iPinNum;
			}
		}
		/* �ִ��� -1�̶�� ���� �ش� �̸��� ������ �����Ƿ� ������ȣ�� 000���� �����Ѵ�. */
		if (-1 == iMaxNum_Map)
			strName_Map = strFrontName_Map + L"_000";
		else /* �ƴ϶�� �ִ񰪿� + 1�� �Ͽ� ���� ��ȣ�� �����Ѵ�. */
		{
			_int iPinNum = iMaxNum_Map + 1;

			if (0 == iPinNum / 10)
				strPin_Map = L"_00" + to_wstring(iPinNum);
			else if (0 == iPinNum / 100)
				strPin_Map = L"_0" + to_wstring(iPinNum);
			else
				strPin_Map = L"_" + to_wstring(iPinNum);

			strName_Map = strFrontName_Map + strPin_Map;
		}
		// ����Ʈ�� �� Ű ����� �� =======================================================================================================

#pragma endregion �±� ����� ��


		// ����Ʈ�� �� Ű ����
		tVoidDesc.strPartTag = strName_Map;
		tVoidDesc.strProtoTag = TEXT("Prototype_GameObject_Effect_Instance");
		FAILED_CHECK(m_pCurEffect->Add_PartObject(TEXT("Prototype_GameObject_Effect_Instance"), strName_Map, &tVoidDesc));


		// ���� ��Ʈ ������Ʈ ����
		m_CurPartObjects = *m_pCurEffect->Get_PartObjects();
		m_pCurPartEffect = dynamic_cast<CEffect_Instance*>(m_pCurEffect->Find_PartObject(strName_Map));


		// ���� ���ĵ� ��Ʈ �±� �ٲ��ֱ�
		m_pCurVoidDesc = m_pCurPartEffect->Get_Desc();
		m_pCurVoidDesc->strPartTag = strName; // ��Ʈ �±� ����


		m_iCurPartIndex = (_int)m_CurPartObjects.size();
		/* ���ڿ� �ʱ�ȭ */
		if (nullptr != m_szPartNames)
		{
			for (_int i = 0; i < m_iCurPartIndex; ++i)
			{
				m_szPartNames[i] = nullptr;
			}
			m_szPartNames = nullptr;
		}

		m_szPartNames = new char* [m_iCurPartIndex];

		_int iCount = 0;
		for (auto& Pair : m_CurPartObjects)
		{
			const string utf8Str = m_pGameInstance->Wstring_To_UTF8(Pair.first);
			m_szPartNames[iCount] = new char[utf8Str.length() + 1];
			strcpy(m_szPartNames[iCount], utf8Str.c_str());

			iCount++;
		}


		for (_int i = 0; i <= m_iCurPartIndex; ++i)
		{
			wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			wstring wsPartName = converter.from_bytes(m_szPartNames[i]);

			if (wsPartName == strName_Map)
			{
				m_iCurPartIndex = i;
				break; // �ش� �̸��� ã�����Ƿ� �� �̻� �ݺ��� �ʿ䰡 ����
			}
		}


		Update_CurParameters_Parts();
#pragma endregion
	}

	return S_OK;
}

HRESULT CWindow_EffectTool::Add_Part_Mesh_Morph(wstring strModelTag1, wstring strModelTag2)
{
	if (nullptr != m_pCurEffect)
	{
		CEffect_Void::EFFECTVOID_DESC tVoidDesc = {};
		tVoidDesc.fSpeedPerSec = { 5.f };
		tVoidDesc.fRotationPerSec = { XMConvertToRadians(50.0f) };

		tVoidDesc.eType_Effect = { CEffect_Void::MESH };

		tVoidDesc.strTextureTag[CEffect_Void::TEXTURE_DIFFUSE] = TEXT("Prototype_Component_Texture_Effect_Diffuse");
		tVoidDesc.iTextureIndex[CEffect_Void::TEXTURE_DIFFUSE] = { 0 };

		tVoidDesc.strTextureTag[CEffect_Void::TEXTURE_MASK] = TEXT("Prototype_Component_Texture_Effect_Mask");
		tVoidDesc.iTextureIndex[CEffect_Void::TEXTURE_MASK] = { 0 };

		tVoidDesc.strTextureTag[CEffect_Void::TEXTURE_NOISE] = TEXT("Prototype_Component_Texture_Effect_Noise");
		tVoidDesc.iTextureIndex[CEffect_Void::TEXTURE_NOISE] = { 0 };

		tVoidDesc.iShaderPassIndex = { 0 };

		tVoidDesc.iRenderGroup = { CRenderer::RENDER_EFFECT };

		tVoidDesc.bBillBoard = { FALSE };

		tVoidDesc.strModelTag[CVIBuffer_Effect_Model_Instance::MORPH_01] = strModelTag1;
		tVoidDesc.strModelTag[CVIBuffer_Effect_Model_Instance::MORPH_02] = strModelTag2;

		tVoidDesc.bPlay = { TRUE };
		tVoidDesc.bUseRigidBody = { TRUE };

		CEffect_Instance::EFFECT_INSTANCE_DESC tInstanceDesc = {};
		//tVoidDesc.vRimColor = { 2.f, 10.f, 255.f, 255.f };
		//tVoidDesc.vBloomPower = { 950.f };

#pragma region ����Ʈ ���ڿ� ����
		wstring strName = TEXT("");
		wstring strFrontName = TEXT("Part");

		_int iMaxNum = -1;
		wstring strPin = TEXT("");

		for (auto& iter : m_CurPartObjects)
		{
			if (nullptr == iter.second)
				continue;

			if (strFrontName == m_pGameInstance->Remove_LastNumChar(iter.first, 4))
			{
				_int iPinNum = stoi(m_pGameInstance->Get_LastNumChar(iter.first, 3));

				if (iMaxNum < iPinNum)
					iMaxNum = iPinNum;
			}
		}
		/* �ִ��� -1�̶�� ���� �ش� �̸��� ������ �����Ƿ� ������ȣ�� 000���� �����Ѵ�. */
		if (-1 == iMaxNum)
			strName = strFrontName + L"_000";
		else /* �ƴ϶�� �ִ񰪿� + 1�� �Ͽ� ���� ��ȣ�� �����Ѵ�. */
		{
			_int iPinNum = iMaxNum + 1;

			if (0 == iPinNum / 10)
				strPin = L"_00" + to_wstring(iPinNum);
			else if (0 == iPinNum / 100)
				strPin = L"_0" + to_wstring(iPinNum);
			else
				strPin = L"_" + to_wstring(iPinNum);

			strName = strFrontName + strPin;
		}

		tVoidDesc.strPartTag = strName;
		tVoidDesc.strProtoTag = TEXT("Prototype_GameObject_Effect_Instance");
		FAILED_CHECK(m_pCurEffect->Add_PartObject(TEXT("Prototype_GameObject_Effect_Instance"), strName, &tVoidDesc));

		m_CurPartObjects = *m_pCurEffect->Get_PartObjects();
		m_pCurPartEffect = dynamic_cast<CEffect_Instance*>(m_pCurEffect->Find_PartObject(strName));

		m_pCurVoidDesc = m_pCurPartEffect->Get_Desc();

		m_iCurPartIndex = (_int)m_CurPartObjects.size();
		/* ���ڿ� �ʱ�ȭ */
		if (nullptr != m_szPartNames)
		{
			for (_int i = 0; i < m_iCurPartIndex; ++i)
			{
				m_szPartNames[i] = nullptr;
			}
			m_szPartNames = nullptr;
		}

		m_szPartNames = new char* [m_iCurPartIndex];

		_int iCount = 0;
		for (auto& Pair : m_CurPartObjects)
		{
			const string utf8Str = m_pGameInstance->Wstring_To_UTF8(Pair.first);
			m_szPartNames[iCount] = new char[utf8Str.length() + 1];
			strcpy(m_szPartNames[iCount], utf8Str.c_str());

			iCount++;
		}
		m_iCurPartIndex -= 1;

		
		for (_int i = 0; i <= m_iCurPartIndex; ++i)
		{
			wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			wstring wsPartName = converter.from_bytes(m_szPartNames[i]);

			if (wsPartName == strName)
			{
				m_iCurPartIndex = i;
				break; // �ش� �̸��� ã�����Ƿ� �� �̻� �ݺ��� �ʿ䰡 ����
			}

		}

		
		Update_CurParameters_Parts();
#pragma endregion
	}

	return S_OK;
}

HRESULT CWindow_EffectTool::Create_Trail()
{

	// ���̵� Desc �ʱ�ȭ
	CEffect_Void::EFFECTVOID_DESC tVoidDesc = {};
	tVoidDesc.fSpeedPerSec = { 0.f };
	tVoidDesc.fRotationPerSec = { XMConvertToRadians(0.0f) };

	tVoidDesc.eType_Effect = { CEffect_Void::TRAIL };

	tVoidDesc.strTextureTag[CEffect_Void::TEXTURE_DIFFUSE] = TEXT("Prototype_Component_Texture_Effect_Diffuse");
	tVoidDesc.iTextureIndex[CEffect_Void::TEXTURE_DIFFUSE] = { 1 };

	tVoidDesc.strTextureTag[CEffect_Void::TEXTURE_MASK] = TEXT("Prototype_Component_Texture_Effect_Mask");
	tVoidDesc.iTextureIndex[CEffect_Void::TEXTURE_MASK] = { 0 };

	tVoidDesc.strTextureTag[CEffect_Void::TEXTURE_NOISE] = TEXT("Prototype_Component_Texture_Effect_Noise");
	tVoidDesc.iTextureIndex[CEffect_Void::TEXTURE_NOISE] = { 0 };

	tVoidDesc.iShaderPassIndex = { 0 };

	tVoidDesc.iRenderGroup = { CRenderer::RENDER_EFFECT };

	tVoidDesc.bBillBoard = { FALSE };

	tVoidDesc.bPlay = { TRUE };


	tVoidDesc.bParentPivot = { FALSE };
	XMStoreFloat4x4(&tVoidDesc.matPivot, XMMatrixIdentity());
	XMStoreFloat4x4(&tVoidDesc.matCombined, XMMatrixIdentity());


	// Ʈ���� ��ü Desc �ʱ�ȭ
	//CEffect_Trail::TRAIL_DESC	tTrailDesc = {};
	//tTrailDesc.bTrailOn = { TRUE };
	//tTrailDesc.vLocalSwordLow = {};
	//tTrailDesc.vLocalSwordHigh = {};


	wstring strName = TEXT("Effect_Trail");

	tVoidDesc.strPartTag = strName;
	tVoidDesc.strProtoTag = TEXT("Prototype_GameObject_Effect_Trail");
	
	m_pTrail = dynamic_cast<CEffect_Trail*>(m_pGameInstance->Add_CloneObject_And_Get(LEVEL_TOOL, LAYER_EFFECT, tVoidDesc.strProtoTag, &tVoidDesc));


	return S_OK;

}

HRESULT CWindow_EffectTool::Delete_Trail()
{
	if (nullptr != m_pTrail)
	{
		m_pTrail->Set_Dead(TRUE);
		m_pTrail = nullptr;
	}

	return S_OK;
}

void CWindow_EffectTool::Delete_CurEffectObject()
{
	m_pCurEffect->Set_Dead(TRUE);
	m_pCurEffect = nullptr;
	m_pCurPartEffect = nullptr;

	wstring strCurName = m_pGameInstance->Char_To_Wstring(m_szEffectNames[m_iCurEffectIndex]);
	m_pEffects.erase(strCurName);


	m_iCurEffectIndex = (_int)m_pEffects.size();
	/* ���ڿ� �ʱ�ȭ */
	if (nullptr != m_szEffectNames)
	{
		for (_int i = 0; i < m_iCurEffectIndex; ++i)
		{
			m_szEffectNames[i] = nullptr;
		}
		m_szEffectNames = nullptr;
	}
	m_szEffectNames = new char* [m_iCurEffectIndex];
	_int iCount = 0;
	for (auto& Pair : m_pEffects)
	{
		const string utf8Str = m_pGameInstance->Wstring_To_UTF8(Pair.first);
		m_szEffectNames[iCount] = new char[utf8Str.length() + 1];
		strcpy(m_szEffectNames[iCount], utf8Str.c_str());

		iCount++;
	}
	m_iCurEffectIndex -= 1;
#pragma endregion

}

void CWindow_EffectTool::Delete_CurPart()
{
	wstring strCurName = m_pGameInstance->Char_To_Wstring(m_szEffectNames[m_iCurEffectIndex]);
	wstring strCurPartName = m_pGameInstance->Char_To_Wstring(m_szPartNames[m_iCurPartIndex]);

	m_pCurEffect->Delete_PartObject(strCurPartName);


	/* ���ڿ� �ʱ�ȭ */
	m_iCurPartIndex = (_int)m_CurPartObjects.size();
	if (nullptr != m_szPartNames)
	{
		for (_int i = 0; i < m_iCurPartIndex; ++i)
		{
			m_szPartNames[i] = nullptr;
		}
		m_szPartNames = nullptr;
	}

	Update_CurMembers(strCurName);

	m_iCurPartIndex = (_int)m_CurPartObjects.size();
	m_szPartNames = new char* [m_iCurPartIndex];

	_int iCount = 0;
	for (auto& Pair : m_CurPartObjects)
	{
		//const string utf8Str = m_pGameInstance->Wstring_To_UTF8(dynamic_cast<CEffect_Void*>(Pair.second)->Get_Desc()->strPartTag);
		const string utf8Str = m_pGameInstance->Wstring_To_UTF8(Pair.first);
		m_szPartNames[iCount] = new char[utf8Str.length() + 1];
		strcpy(m_szPartNames[iCount], utf8Str.c_str());

		iCount++;
	}

	m_iCurPartIndex = 0;
	if (!m_CurPartObjects.empty())
		m_pCurPartEffect = dynamic_cast<CEffect_Void*>(m_CurPartObjects.begin()->second);
	else
		m_pCurPartEffect = nullptr;



}

void CWindow_EffectTool::Copy_CurPart()
{

	if (nullptr != m_pCurPartEffect)
	{
		// ���� ���õ� ��Ʈ ����Ʈ�� ���� ���




	}

}

void CWindow_EffectTool::Update_CurMembers(wstring strName)
{
	if (nullptr != m_pCurEffect)
	{
		if (TEXT("") != strName)
		{
			const string utf8Str = m_pGameInstance->Wstring_To_UTF8(strName);
			m_cCurEffectName = new char[utf8Str.length() + 1];
			strcpy(m_cCurEffectName, utf8Str.c_str());
		}

		m_pCurEffectDesc = m_pCurEffect->Get_Desc();
		m_CurPartObjects = *m_pCurEffect->Get_PartObjects();
	}
}

void CWindow_EffectTool::Update_CurParameters()
{
	if (nullptr != m_pCurEffect)
	{
		CEffect::EFFECT_DESC* pDesc = m_pCurEffect->Get_Desc();
		CTransform* pTransform = m_pCurEffect->Get_Transform();

		m_vTimes_Effect[0] = pDesc->fWaitingTime;
		m_vTimes_Effect[1] = pDesc->fLifeTime;
		m_vTimes_Effect[2] = pDesc->fRemainTime;

		_float4 vPos = pTransform->Get_State(CTransform::STATE_POSITION);
		m_vWorldPosition_Effect[0] = vPos.x;
		m_vWorldPosition_Effect[1] = vPos.y;
		m_vWorldPosition_Effect[2] = vPos.z;

		_float3 vScaled = pTransform->Get_Scaled();
		m_vScale_Effect[0] = vScaled.x;
		m_vScale_Effect[1] = vScaled.y;
		m_vScale_Effect[2] = vScaled.z;

		_float3 vRotated = pTransform->Get_Rotated();
		m_vRotate_Effect[0] = XMConvertToDegrees(vRotated.x);
		m_vRotate_Effect[1] = XMConvertToDegrees(vRotated.y);
		m_vRotate_Effect[2] = XMConvertToDegrees(vRotated.z);


		m_iType_Dead = pDesc->eType_Dead;

	}

}

void CWindow_EffectTool::Update_NeoSequencer_Window()
{

	if (nullptr != m_pCurEffect)
	{
		int32_t startFrame = -2;
		int32_t endFrame = static_cast<int32_t>((m_pCurEffectDesc->fSequenceTime) + 2);

		m_iCurSequenceTimePos = static_cast<int32_t>(m_pCurEffectDesc->fSequenceAcc);

		// ������
		if (ImGui::BeginNeoSequencer("Sequencer", &m_iCurSequenceTimePos, &startFrame, &endFrame, { 0, 0 },
			ImGuiNeoSequencerFlags_EnableSelection |
			ImGuiNeoSequencerFlags_Selection_EnableDragging |
			ImGuiNeoSequencerFlags_Selection_EnableDeletion)
			)
		{
			_bool	bTransformOpen = true;
			if (ImGui::BeginNeoGroup(m_cCurEffectName, &bTransformOpen))
			{
				if (!m_CurPartObjects.empty())
				{
					_int iCount = 0;
					for (auto& iter : m_CurPartObjects)
					{
						vector<ImGui::FrameIndexType> keys = {};

						if (!keys.empty())
							keys.clear();

						CEffect_Void* pPartObj = dynamic_cast<CEffect_Void*>(iter.second);
						_float fWaitingTime = pPartObj->Get_Desc()->fWaitingTime;
						_float fPartLifeTime = pPartObj->Get_Desc()->fLifeTime;
						_float fSequenceTime = pPartObj->Get_Desc()->fSequenceTime;

						keys.push_back((_int)fWaitingTime);
						keys.push_back((_int)fPartLifeTime);
						keys.push_back((_int)fSequenceTime);

						if (ImGui::BeginNeoTimelineEx(m_szPartNames[iCount]))
						{
							for (auto&& v : keys)
							{
								ImGui::NeoKeyframe(&v);
								// Per keyframe code here
							}


							ImGui::EndNeoTimeLine();
						}

						iCount++;
					}

				}
				ImGui::EndNeoGroup();
			}

			ImGui::EndNeoSequencer();
		}
	}

}



void CWindow_EffectTool::Update_SaveLoad_Menu()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::MenuItem(" Save Effect "))
			{
				m_eFile = FILE_EFFECT;
				m_eDialogType = DIALOG_TYPE::SAVE_DIALOG;
				m_strDialogPath = "../Bin/DataFiles/Data_Effect/";

				OpenDialog(IMGUI_EFFECTTOOL_WINDOW);

			}
			if (ImGui::MenuItem(" Load Effect "))
			{
				m_eFile = FILE_EFFECT;
				m_eDialogType = DIALOG_TYPE::LOAD_DIALOG;
				m_strDialogPath = "../Bin/DataFiles/Data_Effect/";

				OpenDialog(IMGUI_EFFECTTOOL_WINDOW);

			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

}

void CWindow_EffectTool::Update_SaveLoad_Trail_Menu()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::MenuItem(" Save Trail "))
			{
				m_eFile = FILE_TRAIL;
				m_eDialogType = DIALOG_TYPE::SAVE_DIALOG;
				m_strDialogPath = "../Bin/DataFiles/Data_Effect/Data_Trail/";

				OpenDialog(IMGUI_EFFECTTOOL_WINDOW);

			}
			if (ImGui::MenuItem(" Load Trail "))
			{
				m_eFile = FILE_TRAIL;
				m_eDialogType = DIALOG_TYPE::LOAD_DIALOG;
				m_strDialogPath = "../Bin/DataFiles/Data_Effect/Data_Trail/";

				OpenDialog(IMGUI_EFFECTTOOL_WINDOW);

			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

}


void CWindow_EffectTool::Update_SaveLoad_Part_Menu()
{

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (nullptr != m_pCurEffect)	// ���� ����Ʈ�� �����ϰ�
			{
				if (nullptr != m_pCurPartEffect)	// ���� ��Ʈ����Ʈ�� �����ϸ�
				{
					CEffect_Void::TYPE_EFFECT eType_Effect = m_pCurVoidDesc->eType_Effect;

					if (CEffect_Void::PARTICLE == eType_Effect) 
					{
						// ��ƼŬ ���� �ε�
						if (ImGui::MenuItem("Save_Particle"))
						{
							m_eFile = FILE_PART_PARTICLE;
							m_eDialogType = DIALOG_TYPE::SAVE_DIALOG;
							m_strDialogPath = "../Bin/DataFiles/Data_Effect/Data_Particle/";

							OpenDialog(IMGUI_EFFECTTOOL_WINDOW);

						}
						if (ImGui::MenuItem("Load_Particle"))
						{
							m_eFile = FILE_PART_PARTICLE;
							m_eDialogType = DIALOG_TYPE::LOAD_DIALOG;
							m_strDialogPath = "../Bin/DataFiles/Data_Effect/Data_Particle/";

							OpenDialog(IMGUI_EFFECTTOOL_WINDOW);

						}
					}
					else if (CEffect_Void::MESH == eType_Effect)
					{
						// �޽� ���� �ε�
						if (ImGui::MenuItem("Save_Mesh"))
						{
							m_eFile = FILE_PART_MESH;
							m_eDialogType = DIALOG_TYPE::SAVE_DIALOG;
							m_strDialogPath = "../Bin/DataFiles/Data_Effect/Data_Mesh/";

							OpenDialog(IMGUI_EFFECTTOOL_WINDOW);

						}
						if (ImGui::MenuItem("Load_Mesh"))
						{
							m_eFile = FILE_PART_MESH;
							m_eDialogType = DIALOG_TYPE::LOAD_DIALOG;
							m_strDialogPath = "../Bin/DataFiles/Data_Effect/Data_Mesh/";

							OpenDialog(IMGUI_EFFECTTOOL_WINDOW);
						}
					}

				}

			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}


HRESULT CWindow_EffectTool::Save_Function(string strPath, string strFileName)
{
	json Out_Json;

	/* ����Ʈ ���� */
	if (FILE_EFFECT == m_eFile)
	{
		if (nullptr != m_pCurEffect)
		{
			if (!m_CurPartObjects.empty())
			{
				m_pCurEffect->Write_Json(Out_Json);
			}
		}
	}


	/* ��Ʈ - ��ƼŬ ���� */
	if (FILE_PART_PARTICLE == m_eFile)
	{
		m_pCurPartEffect->Write_Json(Out_Json);

		string strFrontName = "Part_000";
		Out_Json["strPartTag"] = strFrontName;
	}


	/* ��Ʈ - �޽� ���� */
	if (FILE_PART_MESH == m_eFile)
	{
		m_pCurPartEffect->Write_Json(Out_Json);

		string strFrontName = "Part_000";
		Out_Json["strPartTag"] = strFrontName;
	}


	/* Ʈ���� ���� */
	if (FILE_TRAIL == m_eFile)
	{
		if (nullptr != m_pTrail)
		{
			m_pTrail->Write_Json(Out_Json);
		}
	}

	string strSavePath = strPath + "\\" + strFileName;
	CJson_Utility::Save_Json(strSavePath.c_str(), Out_Json);

	return S_OK;
}

HRESULT CWindow_EffectTool::Load_Function(string strPath, string strFileName)
{
	json In_Json;
	string strLoadPath = strPath + "\\" + strFileName;
	CJson_Utility::Load_Json(strLoadPath.c_str(), In_Json);

	if (FILE_EFFECT == m_eFile)
	{
		Create_EffectObject(TEXT("Layer_Effect"));

		m_pCurEffect->Load_FromJson(In_Json);
		m_pCurEffectDesc = m_pCurEffect->Get_Desc();
		m_pCurEffectDesc->bLoop = TRUE;

		Update_CurParameters();

		m_pCurPartEffect = dynamic_cast<CEffect_Void*>(m_pCurEffect->Get_FirstPartObject());
		Update_CurParameters_Parts();


		/* ���ڿ� �ʱ�ȭ */
		m_iCurPartIndex = (_int)m_CurPartObjects.size();
		if (nullptr != m_szPartNames)
		{
			for (_int i = 0; i < m_iCurPartIndex; ++i)
			{
				m_szPartNames[i] = nullptr;
			}
			m_szPartNames = nullptr;
		}

		wstring strCurName = m_pGameInstance->Char_To_Wstring(m_szEffectNames[m_iCurEffectIndex]);
		Update_CurMembers(strCurName);

		m_iCurPartIndex = (_int)m_CurPartObjects.size();
		m_szPartNames = new char* [m_iCurPartIndex];

		_int iCount = 0;
		for (auto& Pair : m_CurPartObjects)
		{
			const string utf8Str = m_pGameInstance->Wstring_To_UTF8(Pair.first);
			m_szPartNames[iCount] = new char[utf8Str.length() + 1];
			strcpy(m_szPartNames[iCount], utf8Str.c_str());

			iCount++;
		}

		m_iCurPartIndex = 0;
		if (!m_CurPartObjects.empty())
		{
			m_pCurPartEffect = dynamic_cast<CEffect_Void*>(m_CurPartObjects.begin()->second);
			Update_CurParameters_Parts();
		}
		else
			m_pCurPartEffect = nullptr;

	}


	// ��Ʈ_��ƼŬ �ε�
	if (FILE_PART_PARTICLE == m_eFile)
	{
		//wstring strCurName = m_pGameInstance->Char_To_Wstring(m_szPartNames[m_iCurPartIndex]);
		wstring strCurName = m_pCurPartEffect->Get_Desc()->strPartTag;

		m_pCurPartEffect->Load_FromJson(In_Json);
		m_pCurPartEffect->Get_Desc()->strPartTag = strCurName;

		_int iTextureIndex[CEffect_Void::TEXTURE_END];

		for (_int i = 0; i < ECast(CEffect_Void::TEXTURE_END); ++i)
		{
			iTextureIndex[i] = m_pCurPartEffect->Get_Desc()->iTextureIndex[i];
		}

		// �ؽ�ó �缳��
		for (_int i = 0; i < ECast(CEffect_Void::TEXTURE_END); ++i)
		{
			dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Change_TextureCom(m_pCurPartEffect->Get_Desc()->strTextureTag[i]);
			m_pCurPartEffect->Get_Desc()->iTextureIndex[i] = iTextureIndex[i];
		}


		Update_CurParameters();
		Update_CurParameters_Parts();
		return S_OK;
	}


	// ��Ʈ_�޽� �ε�
	if (FILE_PART_MESH == m_eFile)
	{
		wstring strCurName = m_pCurPartEffect->Get_Desc()->strPartTag;

		m_pCurPartEffect->Load_FromJson(In_Json);
		m_pCurPartEffect->Get_Desc()->strPartTag = strCurName;


		_int iTextureIndex[CEffect_Void::TEXTURE_END];

		for (_int i = 0; i < ECast(CEffect_Void::TEXTURE_END); ++i)
		{
			iTextureIndex[i] = m_pCurPartEffect->Get_Desc()->iTextureIndex[i];
		}

		// �ؽ�ó �缳��
		for (_int i = 0; i < ECast(CEffect_Void::TEXTURE_END); ++i)
		{
			dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_TextureCom(m_pCurPartEffect->Get_Desc()->strTextureTag[i]);
			m_pCurPartEffect->Get_Desc()->iTextureIndex[i] = iTextureIndex[i];
		}


		// �� �缳��
		dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Change_ModelCom(m_pCurPartEffect->Get_Desc()->strModelTag[0]);

		Update_CurParameters();
		Update_CurParameters_Parts();
		return S_OK;
	}


	// Ʈ���� �ε�
	if (FILE_TRAIL == m_eFile)
	{

		if (nullptr != m_pTrail) // nullptr�� �ƴϸ� ���â
		{
			// ��� �˾� ����
			ImGui::OpenPopup(u8"Ȯ��:Ʈ���� �ε�");
		}
		else
		{
			Create_Trail();
			m_pTrail->Load_FromJson(In_Json);

			if (nullptr != m_pPart_Preview) // ���� ��Ʈ �����䰡 nullptr�� �ƴϸ� Set���ֱ�
			{
				m_pPart_Preview->Set_Trail(m_pTrail);
			}

			return S_OK;
		}

	}

	return S_OK;
}



CWindow_EffectTool* CWindow_EffectTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWindow_EffectTool* pInstance = new CWindow_EffectTool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CWindow_EffectTool");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CWindow_EffectTool::Free()
{
	__super::Free();

	if (!m_CurPartObjects.empty())
	{
		for (auto& Pair : m_CurPartObjects)
			Safe_Release(Pair.second);
		m_CurPartObjects.clear();
	}


	for (auto& Pair : m_pEffects)
		Safe_Release(Pair.second);
	m_pEffects.clear();


	if (nullptr != m_pCurEffect)
		Safe_Release(m_pCurEffect);

	if (nullptr != m_pCurPartEffect)
		Safe_Release(m_pCurPartEffect);


	m_cCurEffectName = nullptr;
	m_cCurPartName = nullptr;

	if (nullptr != m_szEffectNames)
	{
		for (_int i = 0; i < m_iCurEffectIndex; ++i)
		{
			m_szEffectNames[i] = nullptr;
		}
		m_szEffectNames = nullptr;
	}

	if (nullptr != m_szPartNames)
	{
		for (_int i = 0; i < m_iCurPartIndex; ++i)
		{
			m_szPartNames[i] = nullptr;
		}
		m_szPartNames = nullptr;
	}

}