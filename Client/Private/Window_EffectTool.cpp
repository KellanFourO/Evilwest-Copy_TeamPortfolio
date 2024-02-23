#include "stdafx.h"
#include "Window_EffectTool.h"

#include "imNeoSequencer/imgui_neo_sequencer.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

#include "Effect.h"
#include "Effect_Rect.h"
#include "Effect_Particle.h"
#include "Effect_Instance.h"


CWindow_EffectTool::CWindow_EffectTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CImgui_Window(pDevice, pContext)
{

	Safe_AddRef(m_pGameInstance);
}


HRESULT CWindow_EffectTool::Initialize()
{
	//! ����� Ư���� ��ɾ���. ���� �ʿ��� �� ���Ƽ� ����.
	if (FAILED(__super::Initialize()))
		return E_FAIL;


	Load_CustomStyle();

	//if (FAILED(Ready_Layer_Greed(TEXT("Layer_Greed"))))
	//	return E_FAIL;

	return S_OK;
}

void CWindow_EffectTool::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	ShowDialog();

#pragma region ����Ʈ â
	SetUp_ImGuiDESC(" Object Lists ", ImVec2{ 1000.f, 400.f }, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse |  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |  ImGuiWindowFlags_NoBringToFrontOnFocus, ImVec4(0.f, 0.f, 0.f, 0.2f));
	__super::Begin();


	ImGui::Text("ImGui Window Size : %d, %d", (_int)ImGui::GetWindowContentRegionMax().x, (_int)ImGui::GetWindowContentRegionMax().y);

	Update_EffectList();

	__super::End();
#pragma endregion ����Ʈ â


#pragma region ������ â
	SetUp_ImGuiDESC(" Sequencer ", ImVec2{ 1000.f, 400.f }, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse |  ImGuiWindowFlags_NoResize |  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus, ImVec4(0.f, 0.f, 0.f, 0.2f));
	__super::Begin();

	ImGui::Text("ImGui Window Size : %d, %d", (_int)ImGui::GetWindowContentRegionMax().x, (_int)ImGui::GetWindowContentRegionMax().y);
	Update_Demo_Sequencer();

	__super::End();
#pragma endregion ������ â



#pragma region ����� â
	SetUp_ImGuiDESC(" Play ", ImVec2{ 400.f, 300.f },  ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus, ImVec4(0.f, 0.f, 0.f, 0.2f));
	__super::Begin();

	ImGui::Text("ImGui Window Size : %d, %d", (_int)ImGui::GetWindowContentRegionMax().x, (_int)ImGui::GetWindowContentRegionMax().y);
	Update_PlayBarArea();

	__super::End();
#pragma endregion ����� â



#pragma region ����Ʈ ��

	SetUp_ImGuiDESC(u8"����Ʈ ��", ImVec2{ 300.f, 800.f }, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | /*ImGuiWindowFlags_NoResize | */ ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus, ImVec4(0.f, 0.f, 0.f, 0.2f));
	__super::Begin();

	ImGui::Text("ImGui Window Size : %d, %d", (_int)ImGui::GetWindowContentRegionMax().x, (_int)ImGui::GetWindowContentRegionMax().y);
	POINT	pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	ImGui::Text("Mouse Pos : %d, %d", pt.x, pt.y);
	Update_SaveLoad();


	if (ImGui::BeginTabBar("Tab_Effect", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem(" Particle "))
		{
			Update_ParticleTab();
			ImGui::EndTabItem();
		}


		if (ImGui::BeginTabItem(" Rect "))
		{
			//ImGui::Text(" Texture Effect ");
			Update_RectTab();
			ImGui::EndTabItem();
		}


		if (ImGui::BeginTabItem(" Mesh "))
		{
			//ImGui::Text(" Mesh Effect ");
			Update_MeshTab();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(" Trail "))
		{
			//ImGui::Text(" Mesh Effect ");
			//Update_TrailTab();
			ImGui::EndTabItem();
		}

	}
	ImGui::EndTabBar();

	__super::End();
#pragma endregion ����Ʈ ��

}

void CWindow_EffectTool::Render()
{

}

void CWindow_EffectTool::Update_PlayBarArea()
{
	/* ����� */
	if (nullptr != m_pCurEffect)
	{
		auto& style = ImGui::GetStyle();

		_float fSequenceTimePos = m_pCurEffectDesc->fSequenceAcc;
		ImGui::SliderFloat("SequenceTimeAcc", &fSequenceTimePos, 0.f, m_pCurEffectDesc->fSequenceTime);
	
		m_vTimeAccs_Effect[0] = (m_pCurEffectDesc->fWaitingTime != 0.0f) ? min(1.0f, m_pCurEffectDesc->fWaitingAcc / m_pCurEffectDesc->fWaitingTime) : 1.0f;
		m_vTimeAccs_Effect[1] = m_pCurEffectDesc->fLifeTimeRatio;
		m_vTimeAccs_Effect[2] = (m_pCurEffectDesc->fRemainTime != 0.0f) ? min(1.0f, m_pCurEffectDesc->fRemainAcc / m_pCurEffectDesc->fRemainTime) : 1.0f;
		ImGui::SliderFloat3("TimeAccs_Effect", m_vTimeAccs_Effect, 0.f, 1.f);

		if (nullptr != m_pCurPartEffect)
		{
			if (ImGui::CollapsingHeader("TimeAccs_Part"))
			{
				_float fSequenceTimePos = m_pCurPartEffect->Get_SequenceAcc();
				ImGui::SliderFloat("SequenceTimeAcc_Part", &fSequenceTimePos, 0.f, m_pCurEffectDesc->fSequenceTime);
				
				m_vTimeAccs_Part[0] = (m_pCurPartEffect->Get_WaitingTime() != 0.0f) ? min(1.0f, m_pCurPartEffect->Get_WaitingAcc() / m_pCurPartEffect->Get_WaitingTime()) : 1.0f;
				m_vTimeAccs_Part[1] = m_pCurPartEffect->Get_LifeTimeRatio();
				m_vTimeAccs_Part[2] = (m_pCurPartEffect->Get_RemainTime() != 0.0f) ? min(1.0f, m_pCurPartEffect->Get_RemainAcc() / m_pCurPartEffect->Get_RemainTime()) : 1.0f;
				ImGui::SliderFloat3("TimeAccs_Part", m_vTimeAccs_Part, 0.f, 1.f);
			}
		}

		/* ���, ����, ���� */
		if (ImGui::Button("   Play   ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 30)))
		{
			m_pCurEffectDesc->bPlay = TRUE;
		}
		//ImGui::SameLine();
		if (ImGui::Button("   Stop   ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 30)))
		{
			m_pCurEffectDesc->bPlay = FALSE;
		}
		//ImGui::SameLine();
		if (ImGui::Button("   Reset   ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 30)))
		{
			m_pCurEffect->ReSet_Effect();
		}

		///* ���� ���� */
		//ImGui::RadioButton("Loop", &m_iLoop, 0);  ImGui::SameLine();
		//ImGui::RadioButton("Once", &m_iLoop, 1);

		//if (0 == m_iLoop)
		//{
		//	m_pCurEffectDesc->bLoop = TRUE;
		//}
		//else if (1 == m_iLoop)
		//{
		//	m_pCurEffectDesc->bLoop = FALSE;
		//}


		///* ���, ����� */
		//if (ImGui::Button(" Normal "))
		//{
		//	m_pCurEffectDesc->bReverse = FALSE;
		//}

		//ImGui::SameLine();
		//if (ImGui::Button(" Reverse "))
		//{
		//	m_pCurEffectDesc->bReverse = TRUE;
		//}
		//ImGui::SameLine(); HelpMarker(u8"���, �����");

	}
}


void CWindow_EffectTool::Update_ParticleTab()
{
	if (nullptr != m_pCurEffect)
	{
		if (nullptr != m_pCurPartEffect)
		{
			CEffect_Void::TYPE_EFFECT eType_Effect = m_pCurPartEffect->Get_EffectType();

			if (CEffect_Void::PARTICLE == eType_Effect)
			{
				m_pParticleDesc = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_Desc();
				m_pParticlePointDesc = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_VIBufferCom()->Get_Desc();
				CVIBuffer_Particle_Point* pVIBuffer = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_VIBufferCom();

				/* �̸� */
				ImGui::Text(m_pGameInstance->ConverWStringtoC(m_pParticleDesc->strPartTag));

				/* �ؽ�ó ���� */
				if (ImGui::InputInt("Diffuse_Particle", &m_iTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE], 1))
				{
					if (m_iMaxTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE] <= m_iTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE])
						m_iTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE] = m_iMaxTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE];

					if (0 > m_iTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE])
						m_iTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE] = 0;

					m_pParticleDesc->iTextureIndex[CEffect_Void::TEXTURE_DIFFUSE] = m_iTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE];
				}

				if (ImGui::InputInt("Mask_Particle", &m_iTexIndex_Particle[CEffect_Void::TEXTURE_MASK], 1))
				{
					if (m_iMaxTexIndex_Particle[CEffect_Void::TEXTURE_MASK] <= m_iTexIndex_Particle[CEffect_Void::TEXTURE_MASK])
						m_iTexIndex_Particle[CEffect_Void::TEXTURE_MASK] = m_iMaxTexIndex_Particle[CEffect_Void::TEXTURE_MASK];

					if (0 > m_iTexIndex_Particle[CEffect_Void::TEXTURE_MASK])
						m_iTexIndex_Particle[CEffect_Void::TEXTURE_MASK] = 0;

					m_pParticleDesc->iTextureIndex[CEffect_Void::TEXTURE_MASK] = m_iTexIndex_Particle[CEffect_Void::TEXTURE_MASK];
				}

				if (ImGui::InputInt("Noise_Particle", &m_iTexIndex_Particle[CEffect_Void::TEXTURE_NOISE], 1))
				{
					if (m_iMaxTexIndex_Particle[CEffect_Void::TEXTURE_NOISE] <= m_iTexIndex_Particle[CEffect_Void::TEXTURE_NOISE])
						m_iTexIndex_Particle[CEffect_Void::TEXTURE_NOISE] = m_iMaxTexIndex_Particle[CEffect_Void::TEXTURE_NOISE];

					if (0 > m_iTexIndex_Particle[CEffect_Void::TEXTURE_NOISE])
						m_iTexIndex_Particle[CEffect_Void::TEXTURE_NOISE] = 0;

					m_pParticleDesc->iTextureIndex[CEffect_Void::TEXTURE_NOISE] = m_iTexIndex_Particle[CEffect_Void::TEXTURE_NOISE];
				}

				/* ���̴� �±� �̸� */
				ImGui::Text(m_pGameInstance->ConverWStringtoC(m_pParticleDesc->strShaderTag));		
				/* ���̴� �н� �ε��� ���� */
				if (ImGui::InputInt("Shader Pass_Particle", &m_iShaderPassIndex_Particle, 1))
				{
					if (m_iMaxShaderPassIndex_Particle < m_iShaderPassIndex_Particle)
						m_iShaderPassIndex_Particle = m_iMaxShaderPassIndex_Particle;

					if (0 > m_iShaderPassIndex_Particle)
						m_iShaderPassIndex_Particle = 0;

					m_pParticleDesc->iShaderPassIndex = m_iShaderPassIndex_Particle;
				}
				/* ���̴��� ���� ��ī�� �� ���� */
				//vColor_Clip
				if (ImGui::DragFloat4("Discard_Particle", m_vColor_Clip_Part, 0.1f, 0.f, 1.f))
				{
					m_pParticleDesc->vColor_Clip.x = m_vColor_Clip_Part[0];
					m_pParticleDesc->vColor_Clip.y = m_vColor_Clip_Part[1];
					m_pParticleDesc->vColor_Clip.z = m_vColor_Clip_Part[2];
					m_pParticleDesc->vColor_Clip.w = m_vColor_Clip_Part[3];
				}

				/* �����׷� ���� */
				ImGui::SeparatorText("");
				if (ImGui::InputInt("Render Group_Particle", &m_iRenderGroup_Particle, 1))
				{
					if ((_int)CRenderer::RENDER_END < m_iRenderGroup_Particle)
					{
						m_iRenderGroup_Particle = (_int)CRenderer::RENDER_END - 1;
					}
					m_pParticleDesc->iRenderGroup = m_iRenderGroup_Particle;
				}


				/* �ν��Ͻ� ���� ���� */
				ImGui::SeparatorText("");
				ImGui::Text("MaxInstance : %d", m_iMaxNumInstance);
				if (ImGui::DragInt("Instance Count", &m_iNumInstance, 1, 1, m_iMaxNumInstance))
					pVIBuffer->Set_NumInstance(m_iNumInstance);


				/* ������ Ű�� ���� */
				ImGui::SeparatorText("");
				ImGui::RadioButton("BillBoard", &m_iBillBoard, 0);  ImGui::SameLine();
				ImGui::RadioButton("Off BillBoard", &m_iBillBoard, 1);
				if (0 == m_iBillBoard)
					m_pParticleDesc->bBillBoard = TRUE;
				else if (1 == m_iBillBoard)
					m_pParticleDesc->bBillBoard = FALSE;


				/* ��ƼŬ ������Ʈ ���(Ÿ��) */
				ImGui::SeparatorText(" Action Type ");
				if (ImGui::Button(" Sphere "))
				{
					m_pParticlePointDesc->eType_Action = CVIBuffer_Particle_Point::TYPE_ACTION::SPHERE;
				}
				ImGui::SameLine();
				if (ImGui::Button(" Spark "))
				{
					m_pParticlePointDesc->eType_Action = CVIBuffer_Particle_Point::TYPE_ACTION::SPARK;
				}
				ImGui::SameLine();
				if (ImGui::Button(" Fall "))
				{
					m_pParticlePointDesc->eType_Action = CVIBuffer_Particle_Point::TYPE_ACTION::FALL;
				}
				ImGui::SameLine();
				if (ImGui::Button(" Rise "))
				{
					m_pParticlePointDesc->eType_Action = CVIBuffer_Particle_Point::TYPE_ACTION::RISE;
				}
				ImGui::SameLine();
				if (ImGui::Button(" Blink "))
				{
					m_pParticlePointDesc->eType_Action = CVIBuffer_Particle_Point::TYPE_ACTION::BLINK;
				}
				ImGui::SameLine();
				if (ImGui::Button(" Tornado "))
				{
					m_pParticlePointDesc->eType_Action = CVIBuffer_Particle_Point::TYPE_ACTION::TORNADO;
				}

				if (CVIBuffer_Particle_Point::TYPE_ACTION::SPHERE == m_pParticlePointDesc->eType_Action)
				{
					if (ImGui::DragFloat2("MinMaxLength", m_vMinMaxLengthPosition, 1.f, 0.1f, 360.f))
					{
						m_pParticlePointDesc->vMinMaxLengthPosition.x = m_vMinMaxLengthPosition[0];
						m_pParticlePointDesc->vMinMaxLengthPosition.y = m_vMinMaxLengthPosition[1];
					}
						
				}


				/* ���, ����� */
				ImGui::SeparatorText(" Play Type ");
				if (ImGui::Button(" Normal "))
				{
					m_pParticlePointDesc->bReverse = FALSE;
				}

				ImGui::SameLine();
				if (ImGui::Button(" Reverse "))
				{
					m_pParticlePointDesc->bReverse = TRUE;
				}
				ImGui::SameLine(); HelpMarker(u8"���, �����");


				/* ��ƼŬ ���̵� �ξƿ� ��� */
				ImGui::SeparatorText(" Fade Type ");
				if (ImGui::Button(" Fade_None "))
				{
					m_pParticlePointDesc->eType_Fade = CVIBuffer_Particle_Point::TYPE_FADE::FADE_NONE;
				}
				ImGui::SameLine();
				if (ImGui::Button(" Fade_In "))
				{
					m_pParticlePointDesc->eType_Fade = CVIBuffer_Particle_Point::TYPE_FADE::FADE_IN;
				}
				ImGui::SameLine();
				if (ImGui::Button(" Fade_Out "))
				{
					m_pParticlePointDesc->eType_Fade = CVIBuffer_Particle_Point::TYPE_FADE::FADE_OUT;
				}


				/* �ؽ�ó UVȸ�� */
				ImGui::SeparatorText("");
				if (ImGui::DragFloat("Uv Degree", &m_fUV_RotDegree, 1.f, 0.f, 360.f))
					m_pParticleDesc->fUV_RotDegree = m_fUV_RotDegree;


				/* �߰� ũ�� ���� */
				ImGui::SeparatorText("");
				if (ImGui::DragFloat("AddScale_1", &m_fAddScale, 1.f, 0.f, 360.f))
				{
					m_pParticlePointDesc->vAddScale.x = m_fAddScale;
					m_pParticlePointDesc->vAddScale.y = m_fAddScale;
				}

				if (ImGui::DragFloat2("AddScale_2", m_vAddScale, 1.f, 0.f, 360.f))
				{
					if (0 > m_vAddScale[0])
						m_vAddScale[0] = 0.f;

					if (0 > m_vAddScale[1])
						m_vAddScale[1] = 0.f;

					m_pParticlePointDesc->vAddScale.x = m_vAddScale[0];
					m_pParticlePointDesc->vAddScale.y = m_vAddScale[1];
				}

				/* �� ���� */
				ImGui::ColorEdit4("Start_Color_Particle", m_fColor_Start_Particle, ImGuiColorEditFlags_None);
				ImGui::ColorEdit4("End_Color_Particle", m_fColor_End_Particle, ImGuiColorEditFlags_None);
	
				m_pParticlePointDesc->vMinMaxRed.x = m_fColor_Start_Particle[0];
				m_pParticlePointDesc->vMinMaxRed.y = m_fColor_End_Particle[0];

				m_pParticlePointDesc->vMinMaxGreen.x = m_fColor_Start_Particle[1];
				m_pParticlePointDesc->vMinMaxGreen.y = m_fColor_End_Particle[1];

				m_pParticlePointDesc->vMinMaxBlue.x = m_fColor_Start_Particle[2];
				m_pParticlePointDesc->vMinMaxBlue.y = m_fColor_End_Particle[2];

				m_pParticlePointDesc->vMinMaxAlpha.x = m_fColor_Start_Particle[3];
				m_pParticlePointDesc->vMinMaxAlpha.y = m_fColor_End_Particle[3];

	
				/* Easing Type : ��¡ Ÿ�� */
				if (ImGui::CollapsingHeader(" Easing Types "))
				{
					if (ImGui::Button("LINEAR"))
					{
						m_pParticlePointDesc->eType_Easing = LINEAR;
					}
					if (ImGui::Button("QUAD_IN"))
					{
						m_pParticlePointDesc->eType_Easing = QUAD_IN;
					}
					ImGui::SameLine();
					if (ImGui::Button("QUAD_OUT"))
					{
						m_pParticlePointDesc->eType_Easing = QUAD_OUT;
					}
					ImGui::SameLine();
					if (ImGui::Button("QUAD_INOUT"))
					{
						m_pParticlePointDesc->eType_Easing = QUAD_INOUT;
					}
					if (ImGui::Button("CUBIC_IN"))
					{
						m_pParticlePointDesc->eType_Easing = CUBIC_IN;
					}
					ImGui::SameLine();
					if (ImGui::Button("CUBIC_OUT"))
					{
						m_pParticlePointDesc->eType_Easing = CUBIC_OUT;
					}
					ImGui::SameLine();
					if (ImGui::Button("CUBIC_INOUT"))
					{
						m_pParticlePointDesc->eType_Easing = CUBIC_INOUT;
					}
					if (ImGui::Button("QUART_IN"))
					{
						m_pParticlePointDesc->eType_Easing = QUART_IN;
					}
					ImGui::SameLine();
					if (ImGui::Button("QUART_OUT"))
					{
						m_pParticlePointDesc->eType_Easing = QUART_OUT;
					}
					ImGui::SameLine();
					if (ImGui::Button("QUART_INOUT"))
					{
						m_pParticlePointDesc->eType_Easing = QUART_INOUT;
					}
					if (ImGui::Button("QUINT_IN"))
					{
						m_pParticlePointDesc->eType_Easing = QUINT_IN;
					}
					ImGui::SameLine();
					if (ImGui::Button("QUINT_OUT"))
					{
						m_pParticlePointDesc->eType_Easing = QUINT_OUT;
					}
					ImGui::SameLine();
					if (ImGui::Button("QUINT_INOUT"))
					{
						m_pParticlePointDesc->eType_Easing = QUINT_INOUT;
					}
					if (ImGui::Button("SINE_IN"))
					{
						m_pParticlePointDesc->eType_Easing = SINE_IN;
					}
					ImGui::SameLine();
					if (ImGui::Button("SINE_OUT"))
					{
						m_pParticlePointDesc->eType_Easing = SINE_OUT;
					}
					ImGui::SameLine();
					if (ImGui::Button("SINE_INOUT"))
					{
						m_pParticlePointDesc->eType_Easing = SINE_INOUT;
					}
					if (ImGui::Button("EXPO_IN"))
					{
						m_pParticlePointDesc->eType_Easing = EXPO_IN;
					}
					ImGui::SameLine();
					if (ImGui::Button("EXPO_OUT"))
					{
						m_pParticlePointDesc->eType_Easing = EXPO_OUT;
					}
					ImGui::SameLine();
					if (ImGui::Button("EXPO_INOUT"))
					{
						m_pParticlePointDesc->eType_Easing = EXPO_INOUT;
					}
					if (ImGui::Button("CIRC_IN"))
					{
						m_pParticlePointDesc->eType_Easing = CIRC_IN;
					}
					ImGui::SameLine();
					if (ImGui::Button("CIRC_OUT"))
					{
						m_pParticlePointDesc->eType_Easing = CIRC_OUT;
					}
					ImGui::SameLine();
					if (ImGui::Button("CIRC_INOUT"))
					{
						m_pParticlePointDesc->eType_Easing = CIRC_INOUT;
					}
					if (ImGui::Button("ELASTIC_IN"))
					{
						m_pParticlePointDesc->eType_Easing = ELASTIC_IN;
					}
					ImGui::SameLine();
					if (ImGui::Button("ELASTIC_OUT"))
					{
						m_pParticlePointDesc->eType_Easing = ELASTIC_OUT;
					}
					ImGui::SameLine();
					if (ImGui::Button("ELASTIC_INOUT"))
					{
						m_pParticlePointDesc->eType_Easing = ELASTIC_INOUT;
					}
					if (ImGui::Button("BOUNCE_IN"))
					{
						m_pParticlePointDesc->eType_Easing = BOUNCE_IN;
					}
					ImGui::SameLine();
					if (ImGui::Button("BOUNCE_OUT"))
					{
						m_pParticlePointDesc->eType_Easing = BOUNCE_OUT;
					}
					ImGui::SameLine();
					if (ImGui::Button("ELASTIC_INOUT"))
					{
						m_pParticlePointDesc->eType_Easing = ELASTIC_INOUT;
					}
				}
				
				//_float fSequenceTime = m_pCurPartEffect->Get_SequenceTime();
				//for (_int i = 0; i < 3; ++i)
				//{
				//	m_fColor_Cur_Particle[i] = Easing::Linear(m_fColor_Start_Particle[i], m_fColor_End_Particle[i], m_pCurEffectDesc->fTimeAcc, fSequenceTime);				
				//}
				//m_pParticlePointDesc->vCurrentColor.x = m_fColor_Cur_Particle[0];
				//m_pParticlePointDesc->vCurrentColor.y = m_fColor_Cur_Particle[1];
				//m_pParticlePointDesc->vCurrentColor.z = m_fColor_Cur_Particle[2];
				//m_pParticlePointDesc->vCurrentColor.w = m_fColor_Cur_Particle[3];


				ImGui::SeparatorText("");
				/* ������ Ÿ�� */
				if (ImGui::DragFloat2("MinMaxLifeTime", m_vMinMaxLifeTime, 1.f, 0.f, 360.f))
				{
					if (m_vMinMaxLifeTime[0] > m_vMinMaxLifeTime[1])
						m_vMinMaxLifeTime[0] = m_vMinMaxLifeTime[1];

					m_pParticlePointDesc->vMinMaxLifeTime.x = m_vMinMaxLifeTime[0];
					m_pParticlePointDesc->vMinMaxLifeTime.y = m_vMinMaxLifeTime[1];
				}


				/* ������ ���� */
				if (ImGui::DragFloat2("MinMaxRange", m_vMinMaxRange, 1.f, 0.1f, 360.f))
				{
					if (m_vMinMaxRange[0] > m_vMinMaxRange[1])
						m_vMinMaxRange[0] = m_vMinMaxRange[1];

					m_pParticlePointDesc->vMinMaxRange.x = m_vMinMaxRange[0];
					m_pParticlePointDesc->vMinMaxRange.y = m_vMinMaxRange[1];

				}

				/* ȸ�� ����(������) */
				/* RotX */
				if (ImGui::DragFloat2("RotationX", m_vRotationOffsetX, 1.f, 0.f, 360.f))
				{
					if (0 > m_vRotationOffsetX[0])
						m_vRotationOffsetX[0] = 0.f;

					if (m_vRotationOffsetX[0] > m_vRotationOffsetX[1])
						m_vRotationOffsetX[1] = m_vRotationOffsetX[0];

					m_pParticlePointDesc->vMinMaxRotationOffsetX.x = m_vRotationOffsetX[0];
					m_pParticlePointDesc->vMinMaxRotationOffsetX.y = m_vRotationOffsetX[1];

				}

				/* RotY */
				if (ImGui::DragFloat2("RotationY", m_vRotationOffsetY, 1.f, 0.f, 360.f))
				{
					if (0 > m_vRotationOffsetY[0])
						m_vRotationOffsetY[0] = 0.f;

					if (m_vRotationOffsetY[0] > m_vRotationOffsetY[1])
						m_vRotationOffsetY[1] = m_vRotationOffsetY[0];

					m_pParticlePointDesc->vMinMaxRotationOffsetY.x = m_vRotationOffsetY[0];
					m_pParticlePointDesc->vMinMaxRotationOffsetY.y = m_vRotationOffsetY[1];
				}

				/* RotZ */
				if (ImGui::DragFloat2("RotationZ", m_vRotationOffsetZ, 1.f, 0.f, 360.f))
				{
					if (0 > m_vRotationOffsetZ[0])
						m_vRotationOffsetZ[0] = 0.f;

					if (m_vRotationOffsetZ[0] > m_vRotationOffsetZ[1])
						m_vRotationOffsetZ[1] = m_vRotationOffsetZ[0];

					m_pParticlePointDesc->vMinMaxRotationOffsetZ.x = m_vRotationOffsetZ[0];
					m_pParticlePointDesc->vMinMaxRotationOffsetZ.y = m_vRotationOffsetZ[1];
				}

				/* ���ӵ� */
				ImGui::SeparatorText("");
				if (ImGui::DragFloat("AccPos_Particle", &m_fParticleAccPosition, 0.1f, 0.f, 1.f))
					m_pParticlePointDesc->fAccPosition = m_fParticleAccPosition;

				if (ImGui::DragFloat("ACC_Particle", &m_fParticleAcceleration, 0.5f, 0.f, 100.f))
					m_pParticlePointDesc->fSpeedAcc = m_fParticleAcceleration;

				/* �߷� ��� ���� */
				ImGui::SeparatorText("");
				if (ImGui::Button("Gravity"))
				{
					m_pParticlePointDesc->bUseGravity = TRUE;
				}
				ImGui::SameLine();
				if (ImGui::Button("NoGravity"))
				{
					m_pParticlePointDesc->bUseGravity = FALSE;
				}

				if (ImGui::DragFloat("UseGravityPosition", &m_fUseGravityPosition, 0.f, 0.f, 1.f))
					m_pParticlePointDesc->fUseGravityPosition = m_fUseGravityPosition;

				if (ImGui::DragFloat("fGravityAcc", &m_fGravityAcc, -100.f, 0.f, 1.f))
					m_pParticlePointDesc->fGravityAcc = m_fGravityAcc;

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
			CEffect_Void::TYPE_EFFECT eType_Effect = m_pCurPartEffect->Get_EffectType();

			if (CEffect_Void::RECT == eType_Effect)
			{
				m_pRectDesc = dynamic_cast<CEffect_Rect*>(m_pCurPartEffect)->Get_Desc();




			}
		}
	}
}

void CWindow_EffectTool::Update_MeshTab()
{
	if (nullptr != m_pCurEffect)
	{
		if (nullptr != m_pCurPartEffect)
		{
			CEffect_Void::TYPE_EFFECT eType_Effect = m_pCurPartEffect->Get_EffectType();

			if (CEffect_Void::INSTANCE == eType_Effect)
			{
				m_pInstanceDesc = dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Get_Desc();

				/* �̸� */
				ImGui::Text(m_pGameInstance->ConverWStringtoC(m_pInstanceDesc->strPartTag));

				/* �ؽ�ó ���� */
				if (ImGui::InputInt("Diffuse_Mesh", &m_iTexIndex_Mesh[CEffect_Void::TEXTURE_DIFFUSE], 1))
				{
					if (m_iMaxTexIndex_Mesh[CEffect_Void::TEXTURE_DIFFUSE] <= m_iTexIndex_Mesh[CEffect_Void::TEXTURE_DIFFUSE])
						m_iTexIndex_Mesh[CEffect_Void::TEXTURE_DIFFUSE] = m_iMaxTexIndex_Mesh[CEffect_Void::TEXTURE_DIFFUSE];

					if (0 > m_iTexIndex_Mesh[CEffect_Void::TEXTURE_DIFFUSE])
						m_iTexIndex_Mesh[CEffect_Void::TEXTURE_DIFFUSE] = 0;

					m_pInstanceDesc->iTextureIndex[CEffect_Void::TEXTURE_DIFFUSE] = m_iTexIndex_Mesh[CEffect_Void::TEXTURE_DIFFUSE];
				}

				if (ImGui::InputInt("Mask_Mesh", &m_iTexIndex_Mesh[CEffect_Void::TEXTURE_MASK], 1))
				{
					if (m_iMaxTexIndex_Mesh[CEffect_Void::TEXTURE_MASK] <= m_iTexIndex_Mesh[CEffect_Void::TEXTURE_MASK])
						m_iTexIndex_Mesh[CEffect_Void::TEXTURE_MASK] = m_iMaxTexIndex_Mesh[CEffect_Void::TEXTURE_MASK];

					if (0 > m_iTexIndex_Mesh[CEffect_Void::TEXTURE_MASK])
						m_iTexIndex_Mesh[CEffect_Void::TEXTURE_MASK] = 0;

					m_pInstanceDesc->iTextureIndex[CEffect_Void::TEXTURE_MASK] = m_iTexIndex_Mesh[CEffect_Void::TEXTURE_MASK];
				}

				if (ImGui::InputInt("Noise_Mesh", &m_iTexIndex_Mesh[CEffect_Void::TEXTURE_NOISE], 1))
				{
					if (m_iMaxTexIndex_Mesh[CEffect_Void::TEXTURE_NOISE] <= m_iTexIndex_Mesh[CEffect_Void::TEXTURE_NOISE])
						m_iTexIndex_Mesh[CEffect_Void::TEXTURE_NOISE] = m_iMaxTexIndex_Mesh[CEffect_Void::TEXTURE_NOISE];

					if (0 > m_iTexIndex_Mesh[CEffect_Void::TEXTURE_NOISE])
						m_iTexIndex_Mesh[CEffect_Void::TEXTURE_NOISE] = 0;

					m_pInstanceDesc->iTextureIndex[CEffect_Void::TEXTURE_NOISE] = m_iTexIndex_Mesh[CEffect_Void::TEXTURE_NOISE];
				}

				/* ���̴� �±� �̸� */
				ImGui::Text(m_pGameInstance->ConverWStringtoC(m_pInstanceDesc->strShaderTag));
				/* ���̴� �н� �ε��� ���� */
				if (ImGui::InputInt("Shader Pass_Mesh", &m_iShaderPassIndex_Mesh, 1))
				{
					if (m_iMaxShaderPassIndex_Mesh < m_iShaderPassIndex_Mesh)
						m_iShaderPassIndex_Mesh = m_iMaxShaderPassIndex_Mesh;

					if (0 > m_iShaderPassIndex_Mesh)
						m_iShaderPassIndex_Mesh = 0;

					m_pInstanceDesc->iShaderPassIndex = m_iShaderPassIndex_Mesh;
				}
				/* ���̴��� ���� ��ī�� �� ���� */
				//vColor_Clip
				if (ImGui::DragFloat4("Discard_Mesh", m_vColor_Clip_Part, 0.1f, 0.f, 1.f))
				{
					m_pInstanceDesc->vColor_Clip.x = m_vColor_Clip_Part[0];
					m_pInstanceDesc->vColor_Clip.y = m_vColor_Clip_Part[1];
					m_pInstanceDesc->vColor_Clip.z = m_vColor_Clip_Part[2];
					m_pInstanceDesc->vColor_Clip.w = m_vColor_Clip_Part[3];
				}

				/* �����׷� ���� */
				ImGui::SeparatorText("");
				if (ImGui::InputInt("Render Group_Mesh", &m_iRenderGroup_Mesh, 1))
				{
					if ((_int)CRenderer::RENDER_END < m_iRenderGroup_Mesh)
					{
						m_iRenderGroup_Mesh = (_int)CRenderer::RENDER_END - 1;
					}
					m_pInstanceDesc->iRenderGroup = m_iRenderGroup_Mesh;
				}

				/* UV �� ���� */
				ImGui::SeparatorText("");
				if (ImGui::DragFloat2(" UV_Offset ", m_fUV_Offset, 1.f, 0.f, 100.f))
				{
					m_pInstanceDesc->vUV_Offset.x = m_fUV_Offset[0];
					m_pInstanceDesc->vUV_Offset.y = m_fUV_Offset[1];
				}

				if (ImGui::DragFloat2(" UV_Scale ", m_vUV_Scale, 1.f, 0.f, 100.f))
				{
					m_pInstanceDesc->vUV_Scale.x = m_vUV_Scale[0];
					m_pInstanceDesc->vUV_Scale.y = m_vUV_Scale[1];
				}

				/* ������ �� Ȯ�� */
				ImGui::SeparatorText("");
				ImGui::SliderFloat(" DissolveAmount ", &m_pInstanceDesc->fDissolveAmount, 0.f, 1.f);
				ImGui::SameLine();
				HelpMarker(u8"����ũ:1/������:5/���̴��н�:6/�����׷�:7");


				/* Easing Type : ��¡ Ÿ�� */
				if (ImGui::CollapsingHeader(" Easing Types "))
				{
					if (ImGui::Button("LINEAR"))
					{
						m_pInstanceDesc->eType_Easing = LINEAR;
					}
					if (ImGui::Button("QUAD_IN"))
					{
						m_pInstanceDesc->eType_Easing = QUAD_IN;
					}
					ImGui::SameLine();
					if (ImGui::Button("QUAD_OUT"))
					{
						m_pInstanceDesc->eType_Easing = QUAD_OUT;
					}
					ImGui::SameLine();
					if (ImGui::Button("QUAD_INOUT"))
					{
						m_pInstanceDesc->eType_Easing = QUAD_INOUT;
					}
					if (ImGui::Button("CUBIC_IN"))
					{
						m_pInstanceDesc->eType_Easing = CUBIC_IN;
					}
					ImGui::SameLine();
					if (ImGui::Button("CUBIC_OUT"))
					{
						m_pInstanceDesc->eType_Easing = CUBIC_OUT;
					}
					ImGui::SameLine();
					if (ImGui::Button("CUBIC_INOUT"))
					{
						m_pInstanceDesc->eType_Easing = CUBIC_INOUT;
					}
					if (ImGui::Button("QUART_IN"))
					{
						m_pInstanceDesc->eType_Easing = QUART_IN;
					}
					ImGui::SameLine();
					if (ImGui::Button("QUART_OUT"))
					{
						m_pInstanceDesc->eType_Easing = QUART_OUT;
					}
					ImGui::SameLine();
					if (ImGui::Button("QUART_INOUT"))
					{
						m_pInstanceDesc->eType_Easing = QUART_INOUT;
					}
					if (ImGui::Button("QUINT_IN"))
					{
						m_pInstanceDesc->eType_Easing = QUINT_IN;
					}
					ImGui::SameLine();
					if (ImGui::Button("QUINT_OUT"))
					{
						m_pInstanceDesc->eType_Easing = QUINT_OUT;
					}
					ImGui::SameLine();
					if (ImGui::Button("QUINT_INOUT"))
					{
						m_pInstanceDesc->eType_Easing = QUINT_INOUT;
					}
					if (ImGui::Button("SINE_IN"))
					{
						m_pInstanceDesc->eType_Easing = SINE_IN;
					}
					ImGui::SameLine();
					if (ImGui::Button("SINE_OUT"))
					{
						m_pInstanceDesc->eType_Easing = SINE_OUT;
					}
					ImGui::SameLine();
					if (ImGui::Button("SINE_INOUT"))
					{
						m_pInstanceDesc->eType_Easing = SINE_INOUT;
					}
					if (ImGui::Button("EXPO_IN"))
					{
						m_pInstanceDesc->eType_Easing = EXPO_IN;
					}
					ImGui::SameLine();
					if (ImGui::Button("EXPO_OUT"))
					{
						m_pInstanceDesc->eType_Easing = EXPO_OUT;
					}
					ImGui::SameLine();
					if (ImGui::Button("EXPO_INOUT"))
					{
						m_pInstanceDesc->eType_Easing = EXPO_INOUT;
					}
					if (ImGui::Button("CIRC_IN"))
					{
						m_pInstanceDesc->eType_Easing = CIRC_IN;
					}
					ImGui::SameLine();
					if (ImGui::Button("CIRC_OUT"))
					{
						m_pInstanceDesc->eType_Easing = CIRC_OUT;
					}
					ImGui::SameLine();
					if (ImGui::Button("CIRC_INOUT"))
					{
						m_pInstanceDesc->eType_Easing = CIRC_INOUT;
					}
					if (ImGui::Button("ELASTIC_IN"))
					{
						m_pInstanceDesc->eType_Easing = ELASTIC_IN;
					}
					ImGui::SameLine();
					if (ImGui::Button("ELASTIC_OUT"))
					{
						m_pInstanceDesc->eType_Easing = ELASTIC_OUT;
					}
					ImGui::SameLine();
					if (ImGui::Button("ELASTIC_INOUT"))
					{
						m_pInstanceDesc->eType_Easing = ELASTIC_INOUT;
					}
					if (ImGui::Button("BOUNCE_IN"))
					{
						m_pInstanceDesc->eType_Easing = BOUNCE_IN;
					}
					ImGui::SameLine();
					if (ImGui::Button("BOUNCE_OUT"))
					{
						m_pInstanceDesc->eType_Easing = BOUNCE_OUT;
					}
					ImGui::SameLine();
					if (ImGui::Button("ELASTIC_INOUT"))
					{
						m_pInstanceDesc->eType_Easing = ELASTIC_INOUT;
					}
				}
				ImGui::SeparatorText("");

			}
		}
	}
}

void CWindow_EffectTool::Update_CurParameters_Parts()
{
	if (nullptr != m_pCurPartEffect)
	{
		CEffect_Void::TYPE_EFFECT eType_Effect = m_pCurPartEffect->Get_EffectType();
		CTransform* pPartTransform = m_pCurPartEffect->Get_Transform();

		m_vTimes_Part[0] = m_pCurPartEffect->Get_WaitingTime();
		m_vTimes_Part[1] = m_pCurPartEffect->Get_LifeTime();
		m_vTimes_Part[2] = m_pCurPartEffect->Get_RemainTime();

		_float4 vPos = pPartTransform->Get_State(CTransform::STATE_POSITION);
		m_vWorldPosition_Part[0] = vPos.x;
		m_vWorldPosition_Part[1] = vPos.y;
		m_vWorldPosition_Part[2] = vPos.z;

		_float3 vScaled = pPartTransform->Get_Scaled();
		m_vScale_Part[0] = vScaled.x;
		m_vScale_Part[1] = vScaled.y;
		m_vScale_Part[2] = vScaled.z;

		_float3 vRotated = pPartTransform->Get_Rotated();
		m_vRotate_Part[0] = vRotated.x;
		m_vRotate_Part[1] = vRotated.y;
		m_vRotate_Part[2] = vRotated.z;

		if (CEffect_Void::PARTICLE == eType_Effect)
		{
			m_pParticleDesc = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_Desc();
			m_pParticlePointDesc = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_VIBufferCom()->Get_Desc();

			m_pCurPartEffect->Set_RemainTime(m_vTimes_Part[2]);

			m_iTexIndex_Particle[CEffect_Void::TEXTURE_DIFFUSE] = m_pParticleDesc->iTextureIndex[CEffect_Void::TEXTURE_DIFFUSE];
			m_iTexIndex_Particle[CEffect_Void::TEXTURE_MASK] = m_pParticleDesc->iTextureIndex[CEffect_Void::TEXTURE_MASK];
			m_iTexIndex_Particle[CEffect_Void::TEXTURE_NOISE] = m_pParticleDesc->iTextureIndex[CEffect_Void::TEXTURE_NOISE];
			m_iShaderPassIndex_Particle = m_pParticleDesc->iShaderPassIndex;
			m_iRenderGroup_Particle = m_pParticleDesc->iRenderGroup;
			

			m_iBillBoard = m_pParticleDesc->bBillBoard;

			m_fUV_RotDegree = m_pParticleDesc->fUV_RotDegree;

			if(m_pParticlePointDesc->vAddScale.x == m_pParticlePointDesc->vAddScale.y)
				m_fAddScale = m_pParticlePointDesc->vAddScale.x;
			m_vAddScale[0] = m_pParticlePointDesc->vAddScale.x;
			m_vAddScale[1] = m_pParticlePointDesc->vAddScale.y;

			m_vMinMaxLifeTime[0] = m_pParticlePointDesc->vMinMaxLifeTime.x;
			m_vMinMaxLifeTime[1] = m_pParticlePointDesc->vMinMaxLifeTime.y;

			m_vMinMaxRange[0] = m_pParticlePointDesc->vMinMaxRange.x;
			m_vMinMaxRange[1] = m_pParticlePointDesc->vMinMaxRange.y;


			m_vMinMaxLengthPosition[0] = m_pParticlePointDesc->vMinMaxLengthPosition.x;
			m_vMinMaxLengthPosition[1] = m_pParticlePointDesc->vMinMaxLengthPosition.y;

			m_vRotationOffsetX[0] = m_pParticlePointDesc->vMinMaxRotationOffsetX.x;
			m_vRotationOffsetX[1] = m_pParticlePointDesc->vMinMaxRotationOffsetX.y;

			m_vRotationOffsetY[0] = m_pParticlePointDesc->vMinMaxRotationOffsetY.x;
			m_vRotationOffsetY[1] = m_pParticlePointDesc->vMinMaxRotationOffsetY.y;

			m_vRotationOffsetZ[0] = m_pParticlePointDesc->vMinMaxRotationOffsetZ.x;
			m_vRotationOffsetZ[1] = m_pParticlePointDesc->vMinMaxRotationOffsetZ.y;

			m_fParticleAcceleration = m_pParticlePointDesc->fSpeedAcc;
			m_fParticleAccPosition = m_pParticlePointDesc->fAccPosition;

			m_fUseGravityPosition = m_pParticlePointDesc->fUseGravityPosition;
			m_fGravityAcc = m_pParticlePointDesc->fGravityAcc;

			m_fColor_Start_Particle[0] = m_pParticlePointDesc->vMinMaxRed.x;
			m_fColor_Start_Particle[1] = m_pParticlePointDesc->vMinMaxBlue.x;
			m_fColor_Start_Particle[2] = m_pParticlePointDesc->vMinMaxGreen.x;
			m_fColor_Start_Particle[3] = m_pParticlePointDesc->vMinMaxAlpha.x;

			m_fColor_End_Particle[0] = m_pParticlePointDesc->vMinMaxRed.y;
			m_fColor_End_Particle[1] = m_pParticlePointDesc->vMinMaxBlue.y;
			m_fColor_End_Particle[2] = m_pParticlePointDesc->vMinMaxGreen.y;
			m_fColor_End_Particle[3] = m_pParticlePointDesc->vMinMaxAlpha.y;

		}

		if (CEffect_Void::RECT == eType_Effect)
		{
			m_pRectDesc = dynamic_cast<CEffect_Rect*>(m_pCurPartEffect)->Get_Desc();


		}

		if (CEffect_Void::INSTANCE == eType_Effect)
		{
			m_pInstanceDesc = dynamic_cast<CEffect_Instance*>(m_pCurPartEffect)->Get_Desc();


			m_iTexIndex_Mesh[CEffect_Void::TEXTURE_DIFFUSE] = m_pInstanceDesc->iTextureIndex[CEffect_Void::TEXTURE_DIFFUSE];
			m_iTexIndex_Mesh[CEffect_Void::TEXTURE_MASK] = m_pInstanceDesc->iTextureIndex[CEffect_Void::TEXTURE_MASK];
			m_iTexIndex_Mesh[CEffect_Void::TEXTURE_NOISE] = m_pInstanceDesc->iTextureIndex[CEffect_Void::TEXTURE_NOISE];
			m_iShaderPassIndex_Mesh = m_pInstanceDesc->iShaderPassIndex;


			m_vColor_Clip_Part[0] = m_pInstanceDesc->vColor_Clip.x;
			m_vColor_Clip_Part[1] = m_pInstanceDesc->vColor_Clip.y;
			m_vColor_Clip_Part[2] = m_pInstanceDesc->vColor_Clip.z;
			m_vColor_Clip_Part[3] = m_pInstanceDesc->vColor_Clip.w;

			m_iRenderGroup_Mesh = m_pInstanceDesc->iRenderGroup;

		}

		if (CEffect_Void::MESH == eType_Effect)
		{

		}

	}
	
}


void CWindow_EffectTool::Update_EffectList()
{
	auto& style = ImGui::GetStyle();

	/* ����ִ� ����Ʈ ��ü ���� */
	ImGui::SeparatorText("");
	if (ImGui::Button("         Create         ", ImVec2(ImGui::GetWindowContentRegionMax().x - style.WindowPadding.x, 30)))
	{
		Create_EffectObject(TEXT("Layer_Effect"));
	}

	/* ����Ʈ ����Ʈ & ���� ����Ʈ ���� */
	if (ImGui::ListBox(" Effects ", &m_iCurEffectIndex, m_szEffectNames, m_pEffects.size(), (_int)6))
	{
		wstring strCurName = m_pGameInstance->Char_To_Wstring(m_szEffectNames[m_iCurEffectIndex]);
		m_pCurEffect = m_pEffects.find(strCurName)->second;

		/* ���ڿ� �ʱ�ȭ */
		m_iCurPartIndex = m_CurPartObjects.size();
		if (nullptr != m_szPartNames)
		{
			for (_int i = 0; i < m_iCurPartIndex; ++i)
			{
				m_szPartNames[i] = nullptr;
			}
			m_szPartNames = nullptr;
		}

		Update_CurMembers(strCurName);

		m_iCurPartIndex = m_CurPartObjects.size();
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
			m_pCurPartEffect = dynamic_cast<CEffect_Void*>(m_CurPartObjects.begin()->second);
		else
			m_pCurPartEffect = nullptr;

	} // ����Ʈ ����Ʈ END


	if (nullptr != m_pCurEffect)
	{
		/* ���� ����Ʈ Ȱ��ȭ */
		if (ImGui::Button(" Active_Effect "))
		{
			m_pCurEffectDesc->bActive_Tool = TRUE;
		}
		/* ���� ����Ʈ ��Ȱ��ȭ */
		ImGui::SameLine();
		if (ImGui::Button(" UnActive_Effect "))
		{
			m_pCurEffectDesc->bActive_Tool = FALSE;
		}


		// =========================================

		ImGui::SeparatorText(" Add Part Effect ");
		if (ImGui::Button(" Add Particle "))
		{
			Add_Part_Particle();
		}
		ImGui::SameLine();
		if (ImGui::Button(" Add Rect "))
		{
			Add_Part_Rect();
		}
		ImGui::SameLine();
		if (ImGui::Button(" Add Mesh "))
		{
			Add_Part_Mesh(TEXT("Prototype_Component_Model_splineMesh_tornado"));
		}
		ImGui::SameLine();
		if (ImGui::Button(" Add Trail "))
		{
			//Add_Part_Trail();
		}

		ImGui::SeparatorText("");

		// =========================================

		/* ����Ʈ ��Ʈ������Ʈ ����Ʈ & ���� ��Ʈ������Ʈ ���� */
		if (ImGui::ListBox(" Parts ", &m_iCurPartIndex, m_szPartNames, m_CurPartObjects.size(), (_int)6))
		{
			wstring strCurName = m_pGameInstance->Char_To_Wstring(m_szPartNames[m_iCurPartIndex]);
			m_pCurPartEffect = dynamic_cast<CEffect_Void*>(m_CurPartObjects.find(strCurName)->second);

			Update_CurParameters_Parts();
		}

		if (nullptr != m_pCurPartEffect)
		{
			CEffect_Void::TYPE_EFFECT eType_Effect = m_pCurPartEffect->Get_EffectType();

			/* ���� ��ƼŬ Ȱ��ȭ */
			if (ImGui::Button(" Active Part "))
			{
				if (CEffect_Void::PARTICLE == eType_Effect)
				{
					m_pParticleDesc = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_Desc();
					m_pParticlePointDesc = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_VIBufferCom()->Get_Desc();

					m_pParticleDesc->bActive_Tool = TRUE;
				}

				if (CEffect_Void::RECT == eType_Effect)
				{
					m_pRectDesc->bActive_Tool = TRUE;
				}

				if (CEffect_Void::INSTANCE == eType_Effect)
				{
					m_pInstanceDesc->bActive_Tool = TRUE;
				}

				if (CEffect_Void::MESH == eType_Effect)
				{

				}
			}
			ImGui::SameLine();
			/* ���� ��ƼŬ ��Ȱ��ȭ */
			if (ImGui::Button(" UnActive Part "))
			{
				if (CEffect_Void::PARTICLE == eType_Effect)
				{
					m_pParticleDesc = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_Desc();
					m_pParticlePointDesc = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_VIBufferCom()->Get_Desc();

					m_pParticleDesc->bActive_Tool = FALSE;
				}
				if (CEffect_Void::RECT == eType_Effect)
				{
					m_pRectDesc->bActive_Tool = FALSE;
				}

				if (CEffect_Void::INSTANCE == eType_Effect)
				{
					m_pInstanceDesc->bActive_Tool = FALSE;

				}

				if (CEffect_Void::MESH == eType_Effect)
				{

				}
			}
			ImGui::SameLine();
			if (ImGui::Button(" ReSet_Part "))
			{
				if (CEffect_Void::PARTICLE == eType_Effect)
				{
					m_pParticleDesc = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_Desc();
					m_pParticlePointDesc = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_VIBufferCom()->Get_Desc();

					dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_VIBufferCom()->ReSet();
				}
				if (CEffect_Void::RECT == eType_Effect)
				{

				}

				if (CEffect_Void::INSTANCE == eType_Effect)
				{

				}

				if (CEffect_Void::MESH == eType_Effect)
				{

				}
			}
		}


		/* Effect Edit Start */
		ImGui::SeparatorText("");
		if (ImGui::CollapsingHeader("Effect_Times"))
		{
			ImGui::Text("    Waiting   |   LifeTime   |   SequenceTime ");
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
			ImGui::SeparatorText("");
			if (ImGui::CollapsingHeader("Part_Times"))
			{
				CEffect_Void::TYPE_EFFECT eType_Effect = m_pCurPartEffect->Get_EffectType();

				ImGui::Text("    Waiting   |   LifeTime   |   SequenceTime ");
				if (ImGui::DragFloat3("Times_Part", m_vTimes_Part, 0.2f, 0.f))
				{
					m_pCurPartEffect->Set_WaitingTime(m_vTimes_Part[0]);
					m_pCurPartEffect->Set_LifeTime(m_vTimes_Part[1]);
					m_pCurPartEffect->Set_RemainTime(m_vTimes_Part[2]);

					if (CEffect_Void::PARTICLE == eType_Effect)
					{
						m_pParticleDesc = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_Desc();
						m_pParticlePointDesc = dynamic_cast<CEffect_Particle*>(m_pCurPartEffect)->Get_VIBufferCom()->Get_Desc();
			
					}
					else if (CEffect_Void::RECT == eType_Effect)
					{
						m_pRectDesc = dynamic_cast<CEffect_Rect*>(m_pCurPartEffect)->Get_Desc();

					}

					if (CEffect_Void::INSTANCE == eType_Effect)
					{
	
					}

					if (CEffect_Void::MESH == eType_Effect)
					{

					}
				}
				ImGui::Text("Total_Part : %.2f", m_vTimes_Part[0] + m_vTimes_Part[1] + m_vTimes_Part[2]);
			}
		}



		/* ����Ʈ ��ġ ǥ�� & Ʈ������ */
		ImGui::SeparatorText("");
		_float4 vPos = m_pCurEffect->Get_Transform()->Get_State(CTransform::STATE_POSITION);
		ImGui::Text("Effect Pos  : %.2f %.2f %.2f", vPos.x, vPos.y, vPos.z);
		if (ImGui::CollapsingHeader("Effect_Transform"))
		{
			CTransform* pTransform = m_pCurEffect->Get_Transform();
			if (ImGui::DragFloat3("Effect_Pos", m_vWorldPosition_Effect, 0.5f))
			{
				m_pCurEffect->Set_Position(_float3(m_vWorldPosition_Effect[0], m_vWorldPosition_Effect[1], m_vWorldPosition_Effect[2]));
			}

			_float3 vScaled = pTransform->Get_Scaled();
			ImGui::Text("Effect Scaled  : %.2f %.2f %.2f", vScaled.x, vScaled.y, vScaled.z);
			if (ImGui::DragFloat3("Effect_Scale", m_vScale_Part, 0.5f))
			{
				pTransform->Set_Scaling(m_vScale_Part[0], m_vScale_Part[1], m_vScale_Part[2]);
			}

			_float3 vRotated = pTransform->Get_Rotated();
			ImGui::Text("Effect Rotated  : %.2f %.2f %.2f", vRotated.x, vRotated.y, vRotated.z);
			if (ImGui::DragFloat3("Effect_Rotate", m_vRotate_Effect, 0.5f))
			{
				pTransform->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(m_vRotate_Effect[0]));
				pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_vRotate_Effect[1]));
				pTransform->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(m_vRotate_Effect[2]));
			}
		}
	

		if (nullptr != m_pCurPartEffect)
		{
			/* ��Ʈ ��ġ ǥ�� & Ʈ������ */
			ImGui::SeparatorText("");
			_float4 vPos = m_pCurPartEffect->Get_Transform()->Get_State(CTransform::STATE_POSITION);
			ImGui::Text("Part Pos  : %.2f %.2f %.2f", vPos.x, vPos.y, vPos.z);
			if (ImGui::CollapsingHeader("Part_Transform"))
			{
				CTransform* pPartTransform = m_pCurPartEffect->Get_Transform();

				if (ImGui::DragFloat3("Part_Pos", m_vWorldPosition_Part, 0.5f))
				{
					m_pCurPartEffect->Set_Position(_float3(m_vWorldPosition_Part[0], m_vWorldPosition_Part[1], m_vWorldPosition_Part[2]));
				}
				_float3 vScaled = pPartTransform->Get_Scaled();
				ImGui::Text("Part Scaled  : %.2f %.2f %.2f", vScaled.x, vScaled.y, vScaled.z);
				if (ImGui::DragFloat3("Part_Scale", m_vScale_Part, 0.5f))
				{
					m_pCurPartEffect->Get_Transform()->Set_Scaling(m_vScale_Part[0], m_vScale_Part[1], m_vScale_Part[2]);
				}

				_float3 vRotated = pPartTransform->Get_Rotated();
				ImGui::Text("Part Rotated  : %.2f %.2f %.2f", vRotated.x, vRotated.y, vRotated.z);
				if (ImGui::DragFloat3("Part_Rotate", m_vRotate_Part, 0.5f))
				{
					pPartTransform->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(m_vRotate_Part[0]));
					pPartTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_vRotate_Part[1]));
					pPartTransform->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(m_vRotate_Part[2]));
				}
			}
			ImGui::SeparatorText("");	
		}
	}
}

HRESULT CWindow_EffectTool::Create_EffectObject(const wstring& strLayerTag, CGameObject* pOwner)
{
	_uint iNextLevel = m_pGameInstance->Get_NextLevel();

	CEffect::EFFECT_DESC	tEffectDesc = {};
	tEffectDesc.fSpeedPerSec = { 5.f };
	tEffectDesc.fRotationPerSec = { XMConvertToRadians(50.0f) };

	tEffectDesc.bActive_Tool	= { TRUE };
	tEffectDesc.bPlay			= { TRUE };
	tEffectDesc.bLoop			= { TRUE };
	tEffectDesc.bFinished		= { FALSE };
	tEffectDesc.bReverse		= { FALSE };
	tEffectDesc.bRender			= { TRUE };

	tEffectDesc.fTimeAcc		= { 0.f };
	tEffectDesc.fWaitingAcc		= { 0.f };
	tEffectDesc.fSequenceAcc	= { 0.f };

	tEffectDesc.fLifeTime		= { 5.f };
	tEffectDesc.fWaitingTime	= { 0.f };
	tEffectDesc.fRemainTime		= { 0.f };
	tEffectDesc.fSequenceTime	= tEffectDesc.fWaitingTime + tEffectDesc.fLifeTime + tEffectDesc.fRemainTime;
	tEffectDesc.fLifeTimeRatio	= min(1.f, tEffectDesc.fTimeAcc / tEffectDesc.fLifeTime);
	tEffectDesc.fDissolveStartTime = { 1.f };

	tEffectDesc.pOwner = pOwner;
	if (nullptr != pOwner)
	{
		tEffectDesc.bParentPivot = TRUE;
		tEffectDesc.matPivot = pOwner->Get_Transform()->Get_WorldFloat4x4();
		XMStoreFloat4x4(&tEffectDesc.matOffset, XMMatrixIdentity());
	}
	else
	{
		tEffectDesc.bParentPivot = FALSE;
		XMStoreFloat4x4(&tEffectDesc.matPivot, XMMatrixIdentity());
		XMStoreFloat4x4(&tEffectDesc.matOffset, XMMatrixIdentity());
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
	m_iCurEffectIndex = m_pEffects.size();
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
		CEffect_Particle::PARTICLE_DESC	tParticleDesc = {};
		tParticleDesc.fSpeedPerSec		= { 5.f };
		tParticleDesc.fRotationPerSec	= { XMConvertToRadians(50.0f) };

		tParticleDesc.eType = CEffect_Particle::SINGLE;
		tParticleDesc.strTextureTag[CEffect_Particle::TEXTURE_DIFFUSE] = TEXT("Prototype_Component_Texture_Effect_Particle_Base");
		//tDesc.strTextureTag[CEffect_Particle::TEXTURE_DIFFUSE] = TEXT("Prototype_Component_Texture_Effect_Diffuse");
		tParticleDesc.iTextureIndex[CEffect_Particle::TEXTURE_DIFFUSE] = { 0 };

		tParticleDesc.strTextureTag[CEffect_Particle::TEXTURE_MASK] = TEXT("Prototype_Component_Texture_Effect_Mask");
		//tParticleDesc.strTextureTag[CEffect_Particle::TEXTURE_MASK] = TEXT("");
		tParticleDesc.iTextureIndex[CEffect_Particle::TEXTURE_MASK] = { 0 /*1*/ };

		tParticleDesc.strTextureTag[CEffect_Particle::TEXTURE_NOISE] = TEXT("Prototype_Component_Texture_Effect_Noise");
		tParticleDesc.iTextureIndex[CEffect_Particle::TEXTURE_NOISE] = { 0 };

		tParticleDesc.strShaderTag = TEXT("Prototype_Component_Shader_Particle_Point");
		tParticleDesc.iShaderPassIndex = { 0 };
		tParticleDesc.iRenderGroup = { 7 };

		tParticleDesc.iCurInstanceCnt = { (_uint)m_iNumInstance };
		tParticleDesc.iMaxInstanceCnt = { (_uint)m_iMaxNumInstance };

		tParticleDesc.fUV_RotDegree = { 0.f };



		tParticleDesc.bPlay = { TRUE };

		tParticleDesc.pOwner = m_pCurEffect;


#pragma region ����Ʈ ���ڿ� ����
		wstring strName = TEXT("");
		wstring strFrontName = TEXT("Part_Particle");

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

		tParticleDesc.strPartTag = strName;

		FAILED_CHECK(m_pCurEffect->Add_PartObject(TEXT("Prototype_GameObject_Effect_Particle"), strName, &tParticleDesc));

		m_CurPartObjects = *m_pCurEffect->Get_PartObjects();
		m_pCurPartEffect = dynamic_cast<CEffect_Void*>(m_pCurEffect->Find_PartObject(strName));
		m_pCurPartEffect->Set_EffectType(CEffect_Void::PARTICLE);
		
		m_pCurPartEffect->Set_WaitingTime(1.f);


		m_iCurPartIndex = m_CurPartObjects.size();
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
#pragma endregion


	}

	return S_OK;
}

HRESULT CWindow_EffectTool::Add_Part_Rect()
{
	if (nullptr != m_pCurEffect)
	{
		CEffect_Rect::EFFECT_RECT_DESC tRectDesc = {};
		tRectDesc.fSpeedPerSec = { 5.f };
		tRectDesc.fRotationPerSec = { XMConvertToRadians(50.0f) };

		tRectDesc.eType = CEffect_Rect::SINGLE;
		tRectDesc.strTextureTag[CEffect_Void::TEXTURE_DIFFUSE] = TEXT("Prototype_Component_Texture_Effect_Diffuse");
		tRectDesc.iTextureIndex[CEffect_Void::TEXTURE_DIFFUSE] = { 0 };

		tRectDesc.strTextureTag[CEffect_Void::TEXTURE_MASK] = TEXT("Prototype_Component_Texture_Effect_Mask");
		tRectDesc.iTextureIndex[CEffect_Void::TEXTURE_MASK] = { 0 };

		tRectDesc.strTextureTag[CEffect_Void::TEXTURE_NOISE] = TEXT("Prototype_Component_Texture_Effect_Noise");
		tRectDesc.iTextureIndex[CEffect_Void::TEXTURE_NOISE] = { 0 };

		tRectDesc.strShaderTag = TEXT("Prototype_Component_Shader_EffectTex");

		if (CEffect_Rect::TYPE::SINGLE)
		{
			tRectDesc.iShaderPassIndex = { 2 };
		}
		else
		{
			tRectDesc.iShaderPassIndex = { 1 };
		}

		tRectDesc.iRenderGroup = { 7 };


		tRectDesc.bPlay = { TRUE };


		tRectDesc.pOwner = m_pCurEffect;


#pragma region ����Ʈ ���ڿ� ����
		wstring strName = TEXT("");
		wstring strFrontName = TEXT("Part_Rect");

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

		tRectDesc.strPartTag = strName;

		FAILED_CHECK(m_pCurEffect->Add_PartObject(TEXT("Prototype_GameObject_Effect_Rect"), strName, &tRectDesc));

		m_CurPartObjects = *m_pCurEffect->Get_PartObjects();
		m_pCurPartEffect = dynamic_cast<CEffect_Void*>(m_pCurEffect->Find_PartObject(strName));
		m_pCurPartEffect->Set_EffectType(CEffect_Void::RECT);

		m_pCurPartEffect->Set_WaitingTime(1.f);


		m_iCurPartIndex = m_CurPartObjects.size();
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
#pragma endregion


	}

	return S_OK;
}

HRESULT CWindow_EffectTool::Add_Part_Mesh(wstring strModelTag)
{
	if (nullptr != m_pCurEffect)
	{
		CEffect_Instance::EFFECT_INSTANCE_DESC tMeshDesc = {};
		tMeshDesc.fSpeedPerSec = { 5.f };
		tMeshDesc.fRotationPerSec = { XMConvertToRadians(50.0f) };

		tMeshDesc.eType_Mesh = CEffect_Instance::FLAT;
		tMeshDesc.strTextureTag[CEffect_Void::TEXTURE_DIFFUSE] = TEXT("Prototype_Component_Texture_Effect_Diffuse");
		tMeshDesc.iTextureIndex[CEffect_Void::TEXTURE_DIFFUSE] = { 0 };

		tMeshDesc.strTextureTag[CEffect_Void::TEXTURE_MASK] = TEXT("Prototype_Component_Texture_Effect_Mask");
		tMeshDesc.iTextureIndex[CEffect_Void::TEXTURE_MASK] = { 0 };

		tMeshDesc.strTextureTag[CEffect_Void::TEXTURE_NOISE] = TEXT("Prototype_Component_Texture_Effect_Noise");
		tMeshDesc.iTextureIndex[CEffect_Void::TEXTURE_NOISE] = { 0 };

		tMeshDesc.strShaderTag = TEXT("Prototype_Component_Shader_Effect_Model_Instance");
		tMeshDesc.iShaderPassIndex = { 4 };

		tMeshDesc.iRenderGroup = { 7 };
		tMeshDesc.iCurInstanceCnt = { 1 };

		tMeshDesc.bBillBoard = { FALSE };

		tMeshDesc.strModelTag = strModelTag;

		tMeshDesc.bPlay = { TRUE };

		tMeshDesc.pOwner = m_pCurEffect;


#pragma region ����Ʈ ���ڿ� ����
		wstring strName = TEXT("");
		wstring strFrontName = TEXT("Part_Mesh");

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

		tMeshDesc.strPartTag = strName;

		FAILED_CHECK(m_pCurEffect->Add_PartObject(TEXT("Prototype_GameObject_Effect_Instance"), strName, &tMeshDesc));

		m_CurPartObjects = *m_pCurEffect->Get_PartObjects();
		m_pCurPartEffect = dynamic_cast<CEffect_Void*>(m_pCurEffect->Find_PartObject(strName));
		m_pCurPartEffect->Set_EffectType(CEffect_Void::INSTANCE);

		m_pCurPartEffect->Set_WaitingTime(0.5f);

		m_iCurPartIndex = m_CurPartObjects.size();
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
#pragma endregion
	}

	return S_OK;
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
		m_vTimes_Effect[2] = pDesc->fSequenceTime;

		m_vTimes_Effect[0] = pDesc->fWaitingTime;
		m_vTimes_Effect[1] = pDesc->fLifeTime;

		_float4 vPos = pTransform->Get_State(CTransform::STATE_POSITION);
		m_vWorldPosition_Effect[0] = vPos.x;
		m_vWorldPosition_Effect[1] = vPos.y;
		m_vWorldPosition_Effect[2] = vPos.z;

		_float3 vScaled = pTransform->Get_Scaled();
		m_vScale_Effect[0] = vScaled.x;
		m_vScale_Effect[1] = vScaled.y;
		m_vScale_Effect[2] = vScaled.z;

		_float3 vRotated = pTransform->Get_Rotated();
		m_vRotate_Effect[0] = vRotated.x;
		m_vRotate_Effect[1] = vRotated.y;
		m_vRotate_Effect[2] = vRotated.z;
	}

}

void CWindow_EffectTool::Update_Demo_Sequencer()
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

						if(!keys.empty())
							keys.clear();

						CEffect_Void* pPartObj = dynamic_cast<CEffect_Void*>(iter.second);
						_float fWaitingTime = pPartObj->Get_WaitingTime();
						_float fPartLifeTime = pPartObj->Get_LifeTime();
						_float fSequenceTime = pPartObj->Get_SequenceTime();

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



void CWindow_EffectTool::Update_SaveLoad()
{
}

HRESULT CWindow_EffectTool::Save_Function(string strPath, string strFileName)
{
	return S_OK;
}

HRESULT CWindow_EffectTool::Load_Function(string strPath, string strFileName)
{
	return S_OK;
}

HRESULT CWindow_EffectTool::Ready_Layer_Greed(const wstring& strLayerTag)
{
	CGameObject::GAMEOBJECT_DESC	tDesc = {};
	tDesc.fSpeedPerSec = { 0.f };
	tDesc.fRotationPerSec = { XMConvertToRadians(0.0f) };

	FAILED_CHECK(m_pGameInstance->Add_CloneObject(LEVEL_TOOL, strLayerTag, TEXT("Prototype_GameObject_Greed"), &tDesc));

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

	for (auto& Pair : m_CurPartObjects)
		Safe_Release(Pair.second);
	m_CurPartObjects.clear();

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


	Safe_Release(m_pGameInstance);
}