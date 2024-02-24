#include "..\Public\VIBuffer_Particle.h"

#include "GameInstance.h"
#include "SMath.h"

CVIBuffer_Particle::CVIBuffer_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer_Instancing(pDevice, pContext)
{
}

CVIBuffer_Particle::CVIBuffer_Particle(const CVIBuffer_Particle& rhs)
	: CVIBuffer_Instancing(rhs)
	, m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}


void CVIBuffer_Particle::Restart_ParticleBufferDesc(_uint iCount)
{
	m_iNumInstance = iCount;
	if (m_iNumInstance > m_iMaxCount)
		m_iNumInstance = m_iMaxCount;

	m_bFinished = false;

	D3D11_MAPPED_SUBRESOURCE SubResource;
	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		m_vecParticleInfoDesc[i].bIsDie = false;

		// 위치 (분포 범위)
		((VTXINSTANCE*)SubResource.pData)[i].vPosition = Get_NewPosition_Particle();
		if ((*m_tParticleDesc.pVelocityChangeRandom))
		{
			
			m_vecParticleInfoDesc[i].fVelocityRanTimeAccs = 0.f;
			m_vecParticleInfoDesc[i].fVelocityRanChange   = SMath::fRandom((*m_tParticleDesc.pVelocityChangeTime).x, (*m_tParticleDesc.pVelocityChangeTime).y);
		}

#pragma region 크기
		if ((*m_tParticleDesc.pScaleSameRate))
		{
			_float fScale = SMath::fRandom((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y);
			((VTXINSTANCE*)SubResource.pData)[i].vRight = _float4(fScale, 0.f, 0.f, 0.f);
			((VTXINSTANCE*)SubResource.pData)[i].vUp    = _float4(0.f, fScale, 0.f, 0.f);
			((VTXINSTANCE*)SubResource.pData)[i].vLook = _float4(0.f, 0.f, fScale, 0.f);
		}
		else
		{
			((VTXINSTANCE*)SubResource.pData)[i].vRight = _float4(SMath::fRandom((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y), 0.f, 0.f, 0.f);
			((VTXINSTANCE*)SubResource.pData)[i].vUp    = _float4(0.f, SMath::fRandom((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y), 0.f, 0.f);
			((VTXINSTANCE*)SubResource.pData)[i].vLook  = _float4(0.f, 0.f, SMath::fRandom((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y), 0.f);
		}

		if ((*m_tParticleDesc.pScaleChange))
		{
			m_vecParticleInfoDesc[i].bScaleAdd = (*m_tParticleDesc.pScaleAdd);
			m_vecParticleInfoDesc[i].fScaleSpeed = SMath::fRandom((*m_tParticleDesc.pScaleSpeed).x, (*m_tParticleDesc.pScaleSpeed).y);

			m_vecParticleInfoDesc[i].fScaleTimeAccs = 0.f;
			m_vecParticleInfoDesc[i].fScaleChange   = SMath::fRandom((*m_tParticleDesc.pScaleChangeTime).x, (*m_tParticleDesc.pScaleChangeTime).y);
		
			m_vecParticleInfoDesc[i].fScaleChangeStartTime  = 0.f;
			m_vecParticleInfoDesc[i].fScaleChangeStartDelay = SMath::fRandom((*m_tParticleDesc.pScaleChangeStartDelay).x, (*m_tParticleDesc.pScaleChangeStartDelay).y);
		}
#pragma endregion

#pragma region 이동 방향
		m_vecParticleInfoDesc[i].fVelocitySpeeds = SMath::fRandom((*m_tParticleDesc.pVelocitySpeed).x, (*m_tParticleDesc.pVelocitySpeed).y);

		if (!(*m_tParticleDesc.pVelocityChange))
		{
			m_vecParticleInfoDesc[i].vVelocity = Get_NewVelocity_Particle();
		}
		else
		{
			// 랜덤 x
			if (!(*m_tParticleDesc.pVelocityChangeRandom) && (*m_tParticleDesc.pVelocityUse) > 0)
			{
				if (m_vecParticleInfoDesc[i].pVelocity == nullptr)
					m_vecParticleInfoDesc[i].pVelocity = new _float3[(*m_tParticleDesc.pVelocityUse)];
				if (m_vecParticleInfoDesc[i].pVelocityChange == nullptr)
					m_vecParticleInfoDesc[i].pVelocityChange = new _float[(*m_tParticleDesc.pVelocityUse)];

				for (size_t j = 0; j < (*m_tParticleDesc.pVelocityCountMax); ++j)
				{
					_vector vVelocity = XMVector3Normalize(
						XMVectorSet(
							SMath::fRandom(m_tParticleDesc.pVelocityMin[j].x, m_tParticleDesc.pVelocityMax[j].x),
							SMath::fRandom(m_tParticleDesc.pVelocityMin[j].y, m_tParticleDesc.pVelocityMax[j].y),
							SMath::fRandom(m_tParticleDesc.pVelocityMin[j].z, m_tParticleDesc.pVelocityMax[j].z),
							0.f));
					m_vecParticleInfoDesc[i].pVelocity[j] = _float3(XMVectorGetX(vVelocity), XMVectorGetY(vVelocity), XMVectorGetZ(vVelocity));
					m_vecParticleInfoDesc[i].pVelocityChange[j] = SMath::fRandom(m_tParticleDesc.pVelocityTime[j].x, m_tParticleDesc.pVelocityTime[j].y);
				}
			}
			else
			{
				m_vecParticleInfoDesc[i].vVelocity = Get_NewVelocity_Particle();
			}

			m_vecParticleInfoDesc[i].iVelocityCountCur = 0;
			m_vecParticleInfoDesc[i].fVeloityTimeAccs  = 0.f;

			m_vecParticleInfoDesc[i].fVelocityChangeStartTime  = 0.f;
			m_vecParticleInfoDesc[i].fVelocityChangeStartDelay = SMath::fRandom((*m_tParticleDesc.pVelocityChangeStartDelay).x, (*m_tParticleDesc.pVelocityChangeStartDelay).y);
		}
#pragma endregion

		// 시작 위치 거리 셋팅
		_float fDistance = SMath::fRandom((*m_tParticleDesc.pRangeDistance).x, (*m_tParticleDesc.pRangeDistance).y);
		((VTXINSTANCE*)SubResource.pData)[i].vPosition.x += m_vecParticleInfoDesc[i].vVelocity.x * fDistance;
		((VTXINSTANCE*)SubResource.pData)[i].vPosition.y += m_vecParticleInfoDesc[i].vVelocity.y * fDistance;
		((VTXINSTANCE*)SubResource.pData)[i].vPosition.z += m_vecParticleInfoDesc[i].vVelocity.z * fDistance;

#pragma region 회전
		if (!(*m_tParticleDesc.pBillboard))
		{
			if ((*m_tParticleDesc.pRandomAxis))
			{
				_uint iRandomCount = (_uint)SMath::fRandom(0.f, 2.9);
				switch (iRandomCount)
				{
				case 0:
					m_vecParticleShaderDesc[i].fAxis = _float3(1.f, 0.f, 0.f);
					break;
				case 1:
					m_vecParticleShaderDesc[i].fAxis = _float3(0.f, 1.f, 0.f);
					break;
				default:
					m_vecParticleShaderDesc[i].fAxis = _float3(0.f, 0.f, 1.f);
					break;
				}
			}
			else
			{
				m_vecParticleShaderDesc[i].fAxis = _float3(XMVectorGetX((*m_tParticleDesc.pAxis)), XMVectorGetY((*m_tParticleDesc.pAxis)), XMVectorGetZ((*m_tParticleDesc.pAxis)));
			}

			if ((*m_tParticleDesc.pRandomAngle))
				m_vecParticleShaderDesc[i].fAngle = SMath::fRandom(0.f, 360.f);
			else
				m_vecParticleShaderDesc[i].fAngle = (*m_tParticleDesc.pAngle);

			if ((*m_tParticleDesc.pRotationChange))
			{
				if ((*m_tParticleDesc.pRotationChangeRandom))
				{
					m_vecParticleInfoDesc[i].fRotationTime = 0.f;
					m_vecParticleInfoDesc[i].fRotationChangeTime = SMath::fRandom((*m_tParticleDesc.pRotationChangeTime).x, (*m_tParticleDesc.pRotationChangeTime).y);
				}

				m_vecParticleInfoDesc[i].fRotationSpeed = SMath::fRandom((*m_tParticleDesc.pRotationSpeed).x, (*m_tParticleDesc.pRotationSpeed).y);
			
				m_vecParticleInfoDesc[i].fRotationChangeStartTime  = 0.f;
				m_vecParticleInfoDesc[i].fRotationChangeStartDelay = SMath::fRandom((*m_tParticleDesc.pRotationChangeStartDelay).x, (*m_tParticleDesc.pRotationChangeStartDelay).y);
			}
		}
#pragma endregion

		// 시간
		m_vecParticleInfoDesc[i].fTimeAccs  = 0.f;
		m_vecParticleInfoDesc[i].fLifeTimes = SMath::fRandom((*m_tParticleDesc.pLifeTime).x, (*m_tParticleDesc.pLifeTime).y);

#pragma region 텍스처 정보
		m_vecParticleInfoDesc[i].bAmimationFinish = false;
		m_vecParticleInfoDesc[i].fAccIndex        = 0.f;
		m_vecParticleInfoDesc[i].fAnimationSpeed  = SMath::fRandom((*m_tParticleDesc.pAnimationSpeed).x, (*m_tParticleDesc.pAnimationSpeed).y);

		if ((*m_tParticleDesc.pRandomStartIndex))
			m_vecParticleShaderDesc[i].fUVIndex = _float2(
				(_int)SMath::fRandom(0.f, (*m_tParticleDesc.pUVMaxCount).x),
				(_int)SMath::fRandom(0.f, (*m_tParticleDesc.pUVMaxCount).y));
		else
			m_vecParticleShaderDesc[i].fUVIndex = (*m_tParticleDesc.pUVIndex);

		m_vecParticleShaderDesc[i].fMaxCount = (*m_tParticleDesc.pUVMaxCount);
#pragma endregion

#pragma region 알파
		if (!(*m_tParticleDesc.pFadeChange))
			m_vecParticleShaderDesc[i].fAlpha = SMath::fRandom((*m_tParticleDesc.pStartAlpha).x, (*m_tParticleDesc.pStartAlpha).y);
		else
		{
			m_vecParticleInfoDesc[i].fAlphaChangeStartTime  = 0.f;
			m_vecParticleInfoDesc[i].fAlphaChangeStartDelay = SMath::fRandom((*m_tParticleDesc.pFadeChangeStartDelay).x, (*m_tParticleDesc.pFadeChangeStartDelay).y);

			m_vecParticleShaderDesc[i].fAlpha = SMath::fRandom((*m_tParticleDesc.pStartAlpha).x, (*m_tParticleDesc.pStartAlpha).y);
		}

		if ((*m_tParticleDesc.pFadeCreate))
		{
			m_vecParticleShaderDesc[i].fAlpha        = 1.f;
			m_vecParticleInfoDesc[i].bFadeCreateSucc = false;
		}
		else if((*m_tParticleDesc.pFadeDelete))
			m_vecParticleInfoDesc[i].bFadeCreateSucc = true;

		m_vecParticleInfoDesc[i].fAlphaSpeed = SMath::fRandom((*m_tParticleDesc.pFadeSpeed).x, (*m_tParticleDesc.pFadeSpeed).y);
#pragma endregion

#pragma region 색상
		if ((*m_tParticleDesc.pColorChange))
		{
			m_vecParticleInfoDesc[i].LerpInfo = {};
			m_vecParticleInfoDesc[i].fColorChangeEndTime = SMath::fRandom((*m_tParticleDesc.pColorDuration).x, (*m_tParticleDesc.pColorDuration).y);

			if ((*m_tParticleDesc.pColorChangeRandom))
			{
				m_vecParticleInfoDesc[i].fNextColor = _float3(SMath::fRandom(0.f, 1.f), SMath::fRandom(0.f, 1.f), SMath::fRandom(0.f, 1.f));
				m_vecParticleInfoDesc[i].fColorAccs = 0.f;
				m_vecParticleInfoDesc[i].fColorChangeTime = SMath::fRandom((*m_tParticleDesc.pColorChangeRandomTime).x, (*m_tParticleDesc.pColorChangeRandomTime).y);
			}
			else
			{
				m_vecParticleInfoDesc[i].fColorChangeStartTime = 0.f;
				m_vecParticleInfoDesc[i].fColorChangeStartDelay = SMath::fRandom((*m_tParticleDesc.pColorChangeStartDelay).x, (*m_tParticleDesc.pColorChangeStartDelay).y);

				m_vecParticleInfoDesc[i].iColorIndex = 0;
				m_vecParticleInfoDesc[i].fColorChangeStartM = SMath::fRandom((*m_tParticleDesc.pColorChangeStartM).x, (*m_tParticleDesc.pColorChangeStartM).y);
				m_vecParticleInfoDesc[i].fColorChangeStartF = SMath::fRandom((*m_tParticleDesc.pColorChangeStartF).x, (*m_tParticleDesc.pColorChangeStartF).y);
			}
		}

		if ((*m_tParticleDesc.pColorRandom))
			m_vecParticleShaderDesc[i].fColor = _float3(SMath::fRandom(0.f, 1.f), SMath::fRandom(0.f, 1.f), SMath::fRandom(0.f, 1.f));
		else
			m_vecParticleShaderDesc[i].fColor = _float3((*m_tParticleDesc.pColorS).x, (*m_tParticleDesc.pColorS).y, (*m_tParticleDesc.pColorS).z);
#pragma endregion

#pragma region 블러
		if ((*m_tParticleDesc.pBloomPowerRandom))
			m_vecParticleShaderDesc[i].fBloomPower = _float3(SMath::fRandom(0.f, 1.f), SMath::fRandom(0.f, 1.f), SMath::fRandom(0.f, 1.f));
		else
			m_vecParticleShaderDesc[i].fBloomPower = _float3((*m_tParticleDesc.pBloomPower).x, (*m_tParticleDesc.pBloomPower).y, (*m_tParticleDesc.pBloomPower).z);

		if ((*m_tParticleDesc.pBlurPowerRandom))
			m_vecParticleShaderDesc[i].fBlurPower = SMath::fRandom(0.0f, 1.f);
		else
			m_vecParticleShaderDesc[i].fBlurPower = (*m_tParticleDesc.pBlurPower);
#pragma endregion

#pragma region 리지드바디
		if (true == (*m_tParticleDesc.pRigidbody))
		{
			if (m_vecParticleRigidbodyDesc.size() < m_iNumInstance)
			{
				PARTICLE_RIGIDBODY_DESC ParticleRigidbody = {};
				m_vecParticleRigidbodyDesc.push_back(ParticleRigidbody);
			}

			m_vecParticleRigidbodyDesc[i].vVelocity = _float4(0.f, 0.f, 0.f, 0.f);
		}
#pragma endregion
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Particle::Sort_Z(_uint iCount)
{
	D3D11_MAPPED_SUBRESOURCE SubResource;
	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	vector<VTXINSTANCE> instanceData;
	instanceData.resize(iCount);
	memcpy(instanceData.data(), SubResource.pData, iCount * sizeof(VTXINSTANCE));

	
	_float4 fCamPos = m_pGameInstance->Get_CamPosition();
	_vector vCampos = XMVectorSet(fCamPos.x, fCamPos.y, fCamPos.z, fCamPos.w);

	// 현재 순서 그대로 해당 위치의 값 뷰Z를 구함.
	vector<_float> vecViewZ;
	for (size_t i = 0; i < iCount; i++) {
		_vector vPosition = XMVectorSet(instanceData[i].vPosition.x, instanceData[i].vPosition.y, instanceData[i].vPosition.z, instanceData[i].vPosition.w);
		vecViewZ.push_back(XMVectorGetX(XMVector3Length(vCampos - vPosition)));
	}

	// m_vecViewZ를 기준으로 정렬된 인덱스 구함.
	vector<size_t> sortedIndices(vecViewZ.size());
	iota(sortedIndices.begin(), sortedIndices.end(), 0);
	sort(sortedIndices.begin(), sortedIndices.end(), [&](size_t a, size_t b) {
		return vecViewZ[a] > vecViewZ[b]; }
	);

	// 정렬된 인덱스를 기반으로 다른 컨테이너들도 정렬
	vector<VTXINSTANCE>          sortedInstanceData(instanceData.size());
	vector<PARTICLE_INFO_DESC>   sortedParticleInfoDesc(m_vecParticleInfoDesc.size());
	vector<PARTICLE_SHADER_DESC> sortedParticleShaderDesc(m_vecParticleShaderDesc.size());
	for (size_t i = 0; i < sortedIndices.size(); ++i) {
		size_t index = sortedIndices[i];
		sortedInstanceData[i] = instanceData[index];
		sortedParticleInfoDesc[i] = m_vecParticleInfoDesc[index];
		sortedParticleShaderDesc[i] = m_vecParticleShaderDesc[index];
	}

	// 정렬된 결과 다시 할당
	m_vecParticleInfoDesc = sortedParticleInfoDesc;
	m_vecParticleShaderDesc = sortedParticleShaderDesc;
	memcpy(SubResource.pData, sortedInstanceData.data(), iCount * sizeof(VTXINSTANCE));


	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Particle::Add_Velocity(_uint iCount, _float4 _vMinVelocity, _float4 _vMaxVelocity)
{
	if (iCount > m_vecParticleRigidbodyDesc.size())
		return;

	for (size_t i = 0; i < iCount; i++)
	{
		m_vecParticleRigidbodyDesc[i].vVelocity.x += SMath::fRandom(_vMinVelocity.x, _vMaxVelocity.x);
		m_vecParticleRigidbodyDesc[i].vVelocity.y += SMath::fRandom(_vMinVelocity.y, _vMaxVelocity.y);
		m_vecParticleRigidbodyDesc[i].vVelocity.z += SMath::fRandom(_vMinVelocity.z, _vMaxVelocity.z);
	}
}

void CVIBuffer_Particle::Set_Color(_float3 fColor)
{
	for (auto& iter : m_vecParticleShaderDesc)
		iter.fColor = fColor;
}

HRESULT CVIBuffer_Particle::Initialize_Prototype()
{
	m_iNumVertexBuffers = 2;
	m_iNumVertices = 1;
	m_iStride = sizeof(VTXPOINT);
	m_iInstanceStride = sizeof(VTXINSTANCE);
	m_iIndexCountPerInstance = 1;


	//m_iNumIndices = iNumInstance;
	m_iNumIndices = sizeof(_ushort);
	m_iIndexStride = 1;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT; //m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

#pragma region VERTEX_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXPOINT* pVertices = new VTXPOINT;
	ZeroMemory(pVertices, sizeof(VTXPOINT));

	pVertices->vPosition = _float3(0.0f, 0.0f, 0.f);
	pVertices->vPSize = _float2(1.f, 1.f);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	/* pVertices에 할당하여 채워놨던 정점들의 정보를 ID3D11Buffer로 할당한 공간에 복사하여 채워넣는다. */
	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
#pragma endregion

#pragma region INDEX_BUFFER

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices; //m_iNumPrimitives * m_iIndexSizeofPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	_ushort* pIndices = new _ushort[m_iIndexStride];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iIndexStride);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Particle::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	m_tParticleDesc = *static_cast<PARTICLE_BUFFER_DESC*>(pArg);

	if ((*m_tParticleDesc.pNumEffectMaxCount) <= m_iMaxCount)
		m_iMaxCount = (*m_tParticleDesc.pNumEffectMaxCount);

	m_vecParticleInfoDesc.reserve(m_iMaxCount);
	m_vecParticleShaderDesc.reserve(m_iMaxCount);

#pragma region INSTANCEVERTEXBUFFER
	m_iStride = sizeof(VTXINSTANCE);
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iInstanceStride * m_iMaxCount;
	m_BufferDesc.Usage     = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags           = 0;
	m_BufferDesc.StructureByteStride = m_iInstanceStride;

	m_pVertices = new VTXINSTANCE[m_iMaxCount];
	ZeroMemory(m_pVertices, sizeof(VTXINSTANCE) * m_iMaxCount);

	for (size_t i = 0; i < m_iMaxCount; i++)
	{
		PARTICLE_INFO_DESC   ParticleInfo       = {};
		PARTICLE_SHADER_DESC ParticleShaderInfo = {};
		PARTICLE_RIGIDBODY_DESC ParticleRigidbody = {};

		// 위치 (분포 범위)
		m_pVertices[i].vPosition = Get_NewPosition_Particle();
		if ((*m_tParticleDesc.pVelocityChangeRandom))
		{
			ParticleInfo.fVelocityRanTimeAccs = 0.f;
			ParticleInfo.fVelocityRanChange = SMath::fRandom((*m_tParticleDesc.pVelocityChangeTime).x, (*m_tParticleDesc.pVelocityChangeTime).y);
		}

#pragma region 크기
		if ((*m_tParticleDesc.pScaleSameRate))
		{
			_float fScale = SMath::fRandom((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y);
			m_pVertices[i].vRight = _float4(fScale, 0.f, 0.f, 0.f);
			m_pVertices[i].vUp    = _float4(0.f, fScale, 0.f, 0.f);
			m_pVertices[i].vLook  = _float4(0.f, 0.f, fScale, 0.f);
		}
		else
		{
			m_pVertices[i].vRight = _float4(SMath::fRandom((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y), 0.f, 0.f, 0.f);
			m_pVertices[i].vUp    = _float4(0.f, SMath::fRandom((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y), 0.f, 0.f);
			m_pVertices[i].vLook  = _float4(0.f, 0.f, SMath::fRandom((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y), 0.f);
		}

		if ((*m_tParticleDesc.pScaleChange))
		{
			ParticleInfo.bScaleAdd = (*m_tParticleDesc.pScaleAdd);
			ParticleInfo.fScaleSpeed = SMath::fRandom((*m_tParticleDesc.pScaleSpeed).x, (*m_tParticleDesc.pScaleSpeed).y);

			ParticleInfo.fScaleTimeAccs = 0.f;
			ParticleInfo.fScaleChange   = SMath::fRandom((*m_tParticleDesc.pScaleChangeTime).x, (*m_tParticleDesc.pScaleChangeTime).y);
		
			ParticleInfo.fScaleChangeStartTime  = 0.f;
			ParticleInfo.fScaleChangeStartDelay = SMath::fRandom((*m_tParticleDesc.pScaleChangeStartDelay).x, (*m_tParticleDesc.pScaleChangeStartDelay).y);
		}
#pragma endregion

#pragma region 이동 방향
		ParticleInfo.fVelocitySpeeds = SMath::fRandom((*m_tParticleDesc.pVelocitySpeed).x, (*m_tParticleDesc.pVelocitySpeed).y);

		if (!(*m_tParticleDesc.pVelocityChange))
		{
			ParticleInfo.vVelocity = Get_NewVelocity_Particle();
		}
		else
		{
			// 랜덤 x
			if (!(*m_tParticleDesc.pVelocityChangeRandom) && (*m_tParticleDesc.pVelocityUse) > 0 )
			{
				if (ParticleInfo.pVelocity == nullptr)
					ParticleInfo.pVelocity = new _float3[(*m_tParticleDesc.pVelocityUse)];
				if (ParticleInfo.pVelocityChange == nullptr)
					ParticleInfo.pVelocityChange = new _float[(*m_tParticleDesc.pVelocityUse)];

				for (size_t j = 0; j < (*m_tParticleDesc.pVelocityCountMax); ++j)
				{
					_vector vVelocity = XMVector3Normalize(
						XMVectorSet(
							SMath::fRandom(m_tParticleDesc.pVelocityMin[j].x, m_tParticleDesc.pVelocityMax[j].x),
							SMath::fRandom(m_tParticleDesc.pVelocityMin[j].y, m_tParticleDesc.pVelocityMax[j].y),
							SMath::fRandom(m_tParticleDesc.pVelocityMin[j].z, m_tParticleDesc.pVelocityMax[j].z),
							0.f));
					ParticleInfo.pVelocity[j] = _float3(XMVectorGetX(vVelocity), XMVectorGetY(vVelocity), XMVectorGetZ(vVelocity));
					ParticleInfo.pVelocityChange[j] = SMath::fRandom(m_tParticleDesc.pVelocityTime[j].x, m_tParticleDesc.pVelocityTime[j].y);
				}
			}
			else
			{
				ParticleInfo.vVelocity = Get_NewVelocity_Particle();
			}

			ParticleInfo.iVelocityCountCur = 0;
			ParticleInfo.fVeloityTimeAccs  = 0.f;

			ParticleInfo.fVelocityChangeStartTime  = 0.f;
			ParticleInfo.fVelocityChangeStartDelay = SMath::fRandom((*m_tParticleDesc.pVelocityChangeStartDelay).x, (*m_tParticleDesc.pVelocityChangeStartDelay).y);
		}
#pragma endregion

		// 시작 위치 거리 셋팅
		_float fDistance = SMath::fRandom((*m_tParticleDesc.pRangeDistance).x, (*m_tParticleDesc.pRangeDistance).y);
		m_pVertices[i].vPosition.x += ParticleInfo.vVelocity.x * fDistance;
		m_pVertices[i].vPosition.y += ParticleInfo.vVelocity.y * fDistance;
		m_pVertices[i].vPosition.z += ParticleInfo.vVelocity.z * fDistance;

#pragma region 회전
		if (!(*m_tParticleDesc.pBillboard))
		{
			if ((*m_tParticleDesc.pRandomAxis))
			{
				_uint iRandomCount = (_uint)SMath::fRandom(0.f, 2.9);
				switch (iRandomCount)
				{
				case 0:
					ParticleShaderInfo.fAxis = _float3(1.f, 0.f, 0.f);
					break;
				case 1:
					ParticleShaderInfo.fAxis = _float3(0.f, 1.f, 0.f);
					break;
				default:
					ParticleShaderInfo.fAxis = _float3(0.f, 0.f, 1.f);
					break;
				}
			}
			else
			{
				ParticleShaderInfo.fAxis = _float3(XMVectorGetX((*m_tParticleDesc.pAxis)), XMVectorGetY((*m_tParticleDesc.pAxis)), XMVectorGetZ((*m_tParticleDesc.pAxis)));
			}

			if ((*m_tParticleDesc.pRandomAngle))
				ParticleShaderInfo.fAngle = SMath::fRandom(0.f, 360.f);
			else
				ParticleShaderInfo.fAngle = (*m_tParticleDesc.pAngle);

			if ((*m_tParticleDesc.pRotationChange))
			{
				
				if ((*m_tParticleDesc.pRotationChangeRandom))
				{
					ParticleInfo.fRotationTime = 0.f;
					ParticleInfo.fRotationChangeTime = SMath::fRandom((*m_tParticleDesc.pRotationChangeTime).x, (*m_tParticleDesc.pRotationChangeTime).y);
				}

				ParticleInfo.fRotationSpeed = SMath::fRandom((*m_tParticleDesc.pRotationSpeed).x, (*m_tParticleDesc.pRotationSpeed).y);

				ParticleInfo.fRotationChangeStartTime  = 0.f;
				ParticleInfo.fRotationChangeStartDelay = SMath::fRandom((*m_tParticleDesc.pRotationChangeStartDelay).x, (*m_tParticleDesc.pRotationChangeStartDelay).y);
			}
		}
#pragma endregion

		// 시간
		ParticleInfo.fTimeAccs  = 0.f;
		ParticleInfo.fLifeTimes = SMath::fRandom((*m_tParticleDesc.pLifeTime).x, (*m_tParticleDesc.pLifeTime).y);

#pragma region 텍스처 정보
		ParticleInfo.bAmimationFinish = false;
		ParticleInfo.fAccIndex        = 0.f;
		ParticleInfo.fAnimationSpeed  = SMath::fRandom((*m_tParticleDesc.pAnimationSpeed).x, (*m_tParticleDesc.pAnimationSpeed).y);

		if ((*m_tParticleDesc.pRandomStartIndex))
			ParticleShaderInfo.fUVIndex = _float2(
				(_int)SMath::fRandom(0.f, (*m_tParticleDesc.pUVMaxCount).x),
				(_int)SMath::fRandom(0.f, (*m_tParticleDesc.pUVMaxCount).y));
		else
			ParticleShaderInfo.fUVIndex = (*m_tParticleDesc.pUVIndex);

		ParticleShaderInfo.fMaxCount = (*m_tParticleDesc.pUVMaxCount);
#pragma endregion

#pragma region 알파
		if (!(*m_tParticleDesc.pFadeChange))
			ParticleShaderInfo.fAlpha = SMath::fRandom((*m_tParticleDesc.pStartAlpha).x, (*m_tParticleDesc.pStartAlpha).y);
		else
		{
			ParticleInfo.fAlphaChangeStartTime  = 0.f;
			ParticleInfo.fAlphaChangeStartDelay = SMath::fRandom((*m_tParticleDesc.pFadeChangeStartDelay).x, (*m_tParticleDesc.pFadeChangeStartDelay).y);

			ParticleShaderInfo.fAlpha = SMath::fRandom((*m_tParticleDesc.pStartAlpha).x, (*m_tParticleDesc.pStartAlpha).y);
		}

		if ((*m_tParticleDesc.pFadeCreate))
		{
			ParticleShaderInfo.fAlpha    = 1.f;
			ParticleInfo.bFadeCreateSucc = false;
		}
		else if ((*m_tParticleDesc.pFadeDelete))
			ParticleInfo.bFadeCreateSucc = true;

		ParticleInfo.fAlphaSpeed = SMath::fRandom((*m_tParticleDesc.pFadeSpeed).x, (*m_tParticleDesc.pFadeSpeed).y);
#pragma endregion

#pragma region 색상
		if ((*m_tParticleDesc.pColorChange))
		{
			ParticleInfo.LerpInfo = {};
			ParticleInfo.fColorChangeEndTime = SMath::fRandom((*m_tParticleDesc.pColorDuration).x, (*m_tParticleDesc.pColorDuration).y);

			if ((*m_tParticleDesc.pColorChangeRandom))
			{
				// 다음에 보간할 랜덤 색상
				ParticleInfo.fNextColor = _float3(SMath::fRandom(0.f, 1.f), SMath::fRandom(0.f, 1.f), SMath::fRandom(0.f, 1.f));
				// 누적 시간
				ParticleInfo.fColorAccs = 0.f;
				// 다음 보간 교체할 시간
				ParticleInfo.fColorChangeTime = SMath::fRandom((*m_tParticleDesc.pColorChangeRandomTime).x, (*m_tParticleDesc.pColorChangeRandomTime).y);
			}
			else
			{
				ParticleInfo.fColorChangeStartTime  = 0.f;
				ParticleInfo.fColorChangeStartDelay = SMath::fRandom((*m_tParticleDesc.pColorChangeStartDelay).x, (*m_tParticleDesc.pColorChangeStartDelay).y);

				ParticleInfo.iColorIndex = 0;
				ParticleInfo.fColorChangeStartM = SMath::fRandom((*m_tParticleDesc.pColorChangeStartM).x, (*m_tParticleDesc.pColorChangeStartM).y);
				ParticleInfo.fColorChangeStartF = SMath::fRandom((*m_tParticleDesc.pColorChangeStartF).x, (*m_tParticleDesc.pColorChangeStartF).y);
			}
		}

		if ((*m_tParticleDesc.pColorRandom))
			ParticleShaderInfo.fColor = _float3(SMath::fRandom(0.f, 1.f), SMath::fRandom(0.f, 1.f), SMath::fRandom(0.f, 1.f));
		else
			ParticleShaderInfo.fColor = _float3((*m_tParticleDesc.pColorS).x, (*m_tParticleDesc.pColorS).y, (*m_tParticleDesc.pColorS).z);
#pragma endregion

#pragma region 블러
		if ((*m_tParticleDesc.pBloomPowerRandom))
			ParticleShaderInfo.fBloomPower = _float3(SMath::fRandom(0.f, 1.f), SMath::fRandom(0.f, 1.f), SMath::fRandom(0.f, 1.f));
		else
			ParticleShaderInfo.fBloomPower = _float3((*m_tParticleDesc.pBloomPower).x, (*m_tParticleDesc.pBloomPower).y, (*m_tParticleDesc.pBloomPower).z);

		if ((*m_tParticleDesc.pBlurPowerRandom))
			ParticleShaderInfo.fBlurPower = SMath::fRandom(0.1f, 1.f);
		else
			ParticleShaderInfo.fBlurPower = (*m_tParticleDesc.pBlurPower);
#pragma endregion

		m_vecParticleInfoDesc.push_back(ParticleInfo);
		m_vecParticleShaderDesc.push_back(ParticleShaderInfo);

#pragma region 리지드바디
		if (true == (*m_tParticleDesc.pRigidbody))
		{
			m_vecParticleRigidbodyDesc.push_back(ParticleRigidbody);
		}
#pragma endregion
	}

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = m_pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVBInstance)))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

void CVIBuffer_Particle::Tick(_float fTimeDelta)
{
	if (m_bFinished)
		return;

	D3D11_MAPPED_SUBRESOURCE SubResource;
	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	_uint iDieParticleCount = 0;
	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		if (m_vecParticleInfoDesc[i].bIsDie)
		{
			// 반복O
			if ((*m_tParticleDesc.pParticleLoop))
			{
				m_vecParticleInfoDesc[i].bIsDie = false;
				m_vecParticleInfoDesc[i].fTimeAccs = 0.f;

				// 포지션
				((VTXINSTANCE*)SubResource.pData)[i].vPosition = Get_NewPosition_Particle();

				// 이동
				m_vecParticleInfoDesc[i].vVelocity             = Get_NewVelocity_Particle();
				m_vecParticleInfoDesc[i].fVelocitySpeeds       = SMath::fRandom((*m_tParticleDesc.pVelocitySpeed).x, (*m_tParticleDesc.pVelocitySpeed).y);

				// 스케일
				if ((*m_tParticleDesc.pScaleSameRate))
				{
					_float fScale = SMath::fRandom((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y);
					((VTXINSTANCE*)SubResource.pData)[i].vRight = _float4(fScale, 0.f, 0.f, 0.f);
					((VTXINSTANCE*)SubResource.pData)[i].vUp    = _float4(0.f, fScale, 0.f, 0.f);
					((VTXINSTANCE*)SubResource.pData)[i].vLook  = _float4(0.f, 0.f, fScale, 0.f);
				}
				else
				{
					((VTXINSTANCE*)SubResource.pData)[i].vRight = _float4(SMath::fRandom((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y), 0.f, 0.f, 0.f);
					((VTXINSTANCE*)SubResource.pData)[i].vUp    = _float4(0.f, SMath::fRandom((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y), 0.f, 0.f);
					((VTXINSTANCE*)SubResource.pData)[i].vLook  = _float4(0.f, 0.f, SMath::fRandom((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y), 0.f);
				}


				// 시작 위치 거리 셋팅
				_float fDistance = SMath::fRandom((*m_tParticleDesc.pRangeDistance).x, (*m_tParticleDesc.pRangeDistance).y);
				((VTXINSTANCE*)SubResource.pData)[i].vPosition.x += m_vecParticleInfoDesc[i].vVelocity.x * fDistance;
				((VTXINSTANCE*)SubResource.pData)[i].vPosition.y += m_vecParticleInfoDesc[i].vVelocity.y * fDistance;
				((VTXINSTANCE*)SubResource.pData)[i].vPosition.z += m_vecParticleInfoDesc[i].vVelocity.z * fDistance;


				// 알파
				if ((*m_tParticleDesc.pFadeCreate))
				{
					m_vecParticleShaderDesc[i].fAlpha = 1.f;
					m_vecParticleInfoDesc[i].bFadeCreateSucc = false;
				}
				else
					m_vecParticleShaderDesc[i].fAlpha = SMath::fRandom((*m_tParticleDesc.pStartAlpha).x, (*m_tParticleDesc.pStartAlpha).y);
			}
			else // 반복X
			{
				iDieParticleCount++;
				if (iDieParticleCount == m_iNumInstance)
					m_bFinished = true;

				((VTXINSTANCE*)SubResource.pData)[i].vRight.x = 0.f;
				((VTXINSTANCE*)SubResource.pData)[i].vUp.y    = 0.f;
				((VTXINSTANCE*)SubResource.pData)[i].vLook.z  = 0.f;
			}
		}
		else
		{
			m_vecParticleInfoDesc[i].fTimeAccs += fTimeDelta;

#pragma region 알파 변경
			if ((*m_tParticleDesc.pFadeCreate) && !m_vecParticleInfoDesc[i].bFadeCreateSucc && m_vecParticleShaderDesc[i].fAlpha > 0.f)
			{
				m_vecParticleShaderDesc[i].fAlpha -= fTimeDelta * m_vecParticleInfoDesc[i].fAlphaSpeed;
				if (m_vecParticleShaderDesc[i].fAlpha <= 0.f)
				{
					m_vecParticleShaderDesc[i].fAlpha = 0.f;
					m_vecParticleInfoDesc[i].bFadeCreateSucc = true;
				}
			}
			else
			{
				if ((*m_tParticleDesc.pFadeDelete)
					&& m_vecParticleInfoDesc[i].bFadeCreateSucc
					&& m_vecParticleInfoDesc[i].fTimeAccs > m_vecParticleInfoDesc[i].fLifeTimes - 2.f)
				{
					m_vecParticleShaderDesc[i].fAlpha += fTimeDelta * m_vecParticleInfoDesc[i].fAlphaSpeed;
					if (m_vecParticleShaderDesc[i].fAlpha >= 1.f)
					{
						m_vecParticleShaderDesc[i].fAlpha = 1.f;
						if ((*m_tParticleDesc.pFadeCreate))
							m_vecParticleInfoDesc[i].bFadeCreateSucc = false;

						m_vecParticleInfoDesc[i].fTimeAccs = m_vecParticleInfoDesc[i].fLifeTimes;
					}
				}
				else
				{
					if ((*m_tParticleDesc.pFadeChange))
					{
						m_vecParticleInfoDesc[i].fAlphaChangeStartTime += fTimeDelta;
						if (m_vecParticleInfoDesc[i].fAlphaChangeStartTime >= m_vecParticleInfoDesc[i].fAlphaChangeStartDelay)
						{
							if ((*m_tParticleDesc.pFadeIn)) // 1 -> 0  // 투명 -> 색상
							{
								if (m_vecParticleShaderDesc[i].fAlpha > 0.f)
									m_vecParticleShaderDesc[i].fAlpha -= fTimeDelta * m_vecParticleInfoDesc[i].fAlphaSpeed;
								else
								{
									m_vecParticleShaderDesc[i].fAlpha = 0.f;
									if ((*m_tParticleDesc.pParticleLoop))
									{
										m_vecParticleInfoDesc[i].fAlphaChangeStartTime  = 0.f;
										m_vecParticleInfoDesc[i].fAlphaChangeStartDelay = SMath::fRandom((*m_tParticleDesc.pFadeChangeStartDelay).x, (*m_tParticleDesc.pFadeChangeStartDelay).y);

										m_vecParticleShaderDesc[i].fAlpha = SMath::fRandom((*m_tParticleDesc.pStartAlpha).x, (*m_tParticleDesc.pStartAlpha).y);
									}
								}
							}
							else // 0 -> 1 // 색상 -> 투명
							{
								if (m_vecParticleShaderDesc[i].fAlpha < 1.f)
									m_vecParticleShaderDesc[i].fAlpha += fTimeDelta * m_vecParticleInfoDesc[i].fAlphaSpeed;
								else
								{
									m_vecParticleShaderDesc[i].fAlpha = 1.f;
									if ((*m_tParticleDesc.pParticleLoop))
									{
										m_vecParticleInfoDesc[i].fAlphaChangeStartTime  = 0.f;
										m_vecParticleInfoDesc[i].fAlphaChangeStartDelay = SMath::fRandom((*m_tParticleDesc.pFadeChangeStartDelay).x, (*m_tParticleDesc.pFadeChangeStartDelay).y);

										if ((*m_tParticleDesc.pFadeCreate))
										{
											m_vecParticleInfoDesc[i].bFadeCreateSucc = false;
											m_vecParticleShaderDesc[i].fAlpha = 1.f;
										}
										else
											m_vecParticleShaderDesc[i].fAlpha = SMath::fRandom((*m_tParticleDesc.pStartAlpha).x, (*m_tParticleDesc.pStartAlpha).y);
									}
									else
										m_vecParticleInfoDesc[i].bIsDie   = true;
								}
							}
						}
					}
				}
			}
#pragma endregion

#pragma region 위치 변경
			if (!(*m_tParticleDesc.pVelocityChange))
			{
				// 위치 기본 변경
				if (true == (*m_tParticleDesc.pRigidbody))
				{
					if (0 != m_vecParticleRigidbodyDesc.size())
					{
						// 속도에 따른 이동 (중력)
						if ((*m_tParticleDesc.pGravity))
						{
							// 추가 가속도 중력 추가
							m_vecParticleRigidbodyDesc[i].vAccelA = _float4(0.f, -30.f, 0.f, 0.f);

							_float4 vNewVelocity = m_vecParticleRigidbodyDesc[i].vVelocity * fTimeDelta;
							((VTXINSTANCE*)SubResource.pData)[i].vPosition.x += vNewVelocity.x;
							((VTXINSTANCE*)SubResource.pData)[i].vPosition.y += vNewVelocity.y;
							((VTXINSTANCE*)SubResource.pData)[i].vPosition.z += vNewVelocity.z;
						}

						m_vecParticleRigidbodyDesc[i].vForce.x += m_vecParticleInfoDesc[i].vVelocity.x * m_vecParticleInfoDesc[i].fVelocitySpeeds;
						m_vecParticleRigidbodyDesc[i].vForce.y += m_vecParticleInfoDesc[i].vVelocity.y * m_vecParticleInfoDesc[i].fVelocitySpeeds;
						m_vecParticleRigidbodyDesc[i].vForce.z += m_vecParticleInfoDesc[i].vVelocity.z * m_vecParticleInfoDesc[i].fVelocitySpeeds;

						_float4 vNewVelocity = m_vecParticleRigidbodyDesc[i].vVelocity * fTimeDelta;
						((VTXINSTANCE*)SubResource.pData)[i].vPosition.x += vNewVelocity.x;
						((VTXINSTANCE*)SubResource.pData)[i].vPosition.y += vNewVelocity.y;
						((VTXINSTANCE*)SubResource.pData)[i].vPosition.z += vNewVelocity.z;

						Tick_Rigidbody(fTimeDelta, i);

						if ((*m_tParticleDesc.pStopZero))
						{
							if (0.f > ((VTXINSTANCE*)SubResource.pData)[i].vPosition.y)
							{
								((VTXINSTANCE*)SubResource.pData)[i].vPosition.y = 0.f;
								if (!(*m_tParticleDesc.pGroundSlide))
								{
									m_vecParticleRigidbodyDesc[i].vVelocity = _float4(0.f, 0.f, 0.f, 0.f);
									(*m_tParticleDesc.pRotationChange) = false;
								}
								//m_vecParticleInfoDesc[i].bIsDie = true;
							}
						}
						else if ((*m_tParticleDesc.pStopStartY))
						{
							if (m_pVertices[i].vPosition.y > ((VTXINSTANCE*)SubResource.pData)[i].vPosition.y)
							{
								((VTXINSTANCE*)SubResource.pData)[i].vPosition.y = m_pVertices[i].vPosition.y;
								if (!(*m_tParticleDesc.pGroundSlide))
								{
									m_vecParticleRigidbodyDesc[i].vVelocity = _float4(0.f, 0.f, 0.f, 0.f);
									(*m_tParticleDesc.pRotationChange) = false;
								}
								//m_vecParticleInfoDesc[i].bIsDie = true;
							}
						}
					}
				}
				else
				{
					((VTXINSTANCE*)SubResource.pData)[i].vPosition.x += m_vecParticleInfoDesc[i].vVelocity.x * m_vecParticleInfoDesc[i].fVelocitySpeeds * fTimeDelta;
					((VTXINSTANCE*)SubResource.pData)[i].vPosition.y += m_vecParticleInfoDesc[i].vVelocity.y * m_vecParticleInfoDesc[i].fVelocitySpeeds * fTimeDelta;
					((VTXINSTANCE*)SubResource.pData)[i].vPosition.z += m_vecParticleInfoDesc[i].vVelocity.z * m_vecParticleInfoDesc[i].fVelocitySpeeds * fTimeDelta;
				}
			}
			else
			{
				// 위치 추가 변경
				m_vecParticleInfoDesc[i].fVelocityChangeStartTime += fTimeDelta;
				if (m_vecParticleInfoDesc[i].fVelocityChangeStartTime >= m_vecParticleInfoDesc[i].fVelocityChangeStartDelay)
				{
					if ((*m_tParticleDesc.pVelocityChangeRandom))
					{
						// 자동
						((VTXINSTANCE*)SubResource.pData)[i].vPosition.x += m_vecParticleInfoDesc[i].vVelocity.x * m_vecParticleInfoDesc[i].fVelocitySpeeds * fTimeDelta;
						((VTXINSTANCE*)SubResource.pData)[i].vPosition.y += m_vecParticleInfoDesc[i].vVelocity.y * m_vecParticleInfoDesc[i].fVelocitySpeeds * fTimeDelta;
						((VTXINSTANCE*)SubResource.pData)[i].vPosition.z += m_vecParticleInfoDesc[i].vVelocity.z * m_vecParticleInfoDesc[i].fVelocitySpeeds * fTimeDelta;

						m_vecParticleInfoDesc[i].fVelocityRanTimeAccs += m_vecParticleInfoDesc[i].fVelocitySpeeds * fTimeDelta;
						if (m_vecParticleInfoDesc[i].fVelocityRanTimeAccs >= m_vecParticleInfoDesc[i].fVelocityRanChange)
						{
							m_vecParticleInfoDesc[i].fVelocityRanTimeAccs = 0.f;

							m_vecParticleInfoDesc[i].vVelocity = Get_NewVelocity_Particle();
							m_vecParticleInfoDesc[i].fVelocityRanChange = SMath::fRandom((*m_tParticleDesc.pVelocityChangeTime).x, (*m_tParticleDesc.pVelocityChangeTime).y);
						}
					}
					else
					{
						// 수동
						if (nullptr != m_vecParticleInfoDesc[i].pVelocity)
						{
							((VTXINSTANCE*)SubResource.pData)[i].vPosition.x += m_vecParticleInfoDesc[i].pVelocity[m_vecParticleInfoDesc[i].iVelocityCountCur].x * m_vecParticleInfoDesc[i].fVelocitySpeeds * fTimeDelta;
							((VTXINSTANCE*)SubResource.pData)[i].vPosition.y += m_vecParticleInfoDesc[i].pVelocity[m_vecParticleInfoDesc[i].iVelocityCountCur].y * m_vecParticleInfoDesc[i].fVelocitySpeeds * fTimeDelta;
							((VTXINSTANCE*)SubResource.pData)[i].vPosition.z += m_vecParticleInfoDesc[i].pVelocity[m_vecParticleInfoDesc[i].iVelocityCountCur].z * m_vecParticleInfoDesc[i].fVelocitySpeeds * fTimeDelta;
						}
					
						m_vecParticleInfoDesc[i].fVeloityTimeAccs += fTimeDelta;
						if (nullptr != m_vecParticleInfoDesc[i].pVelocityChange && m_vecParticleInfoDesc[i].fVeloityTimeAccs >= m_vecParticleInfoDesc[i].pVelocityChange[m_vecParticleInfoDesc[i].iVelocityCountCur])
						{
							if (m_vecParticleInfoDesc[i].iVelocityCountCur < (*m_tParticleDesc.pVelocityCountMax) - 1)
							{
								m_vecParticleInfoDesc[i].iVelocityCountCur++;
								m_vecParticleInfoDesc[i].fVeloityTimeAccs = 0.f;
							}
							else
							{
								if ((*m_tParticleDesc.pVelocityLoop))
								{
									m_vecParticleInfoDesc[i].iVelocityCountCur = 0;
									m_vecParticleInfoDesc[i].fVeloityTimeAccs = 0.f;
								}
							}
						}
					}
				}
			}
#pragma endregion

#pragma region 박스 범위 검사
			if ((*m_tParticleDesc.pUseBox))
			{
				if (((VTXINSTANCE*)SubResource.pData)[i].vPosition.x < (*m_tParticleDesc.pBoxMin).x ||
					((VTXINSTANCE*)SubResource.pData)[i].vPosition.y < (*m_tParticleDesc.pBoxMin).y ||
					((VTXINSTANCE*)SubResource.pData)[i].vPosition.z < (*m_tParticleDesc.pBoxMin).z ||
					((VTXINSTANCE*)SubResource.pData)[i].vPosition.x >(*m_tParticleDesc.pBoxMax).x ||
					((VTXINSTANCE*)SubResource.pData)[i].vPosition.y >(*m_tParticleDesc.pBoxMax).y ||
					((VTXINSTANCE*)SubResource.pData)[i].vPosition.z >(*m_tParticleDesc.pBoxMax).z)
				{
					m_vecParticleInfoDesc[i].bIsDie = true;
				}
			}
#pragma endregion

#pragma region 크기 변경
			if ((*m_tParticleDesc.pScaleChange))
			{
				m_vecParticleInfoDesc[i].fScaleChangeStartTime += fTimeDelta;
				if (m_vecParticleInfoDesc[i].fScaleChangeStartTime >= m_vecParticleInfoDesc[i].fScaleChangeStartDelay)
				{
					if ((*m_tParticleDesc.pScaleChangeRandom))
					{
						m_vecParticleInfoDesc[i].fScaleTimeAccs += m_vecParticleInfoDesc[i].fScaleSpeed * fTimeDelta;
						if (m_vecParticleInfoDesc[i].fScaleTimeAccs >= m_vecParticleInfoDesc[i].fScaleChange)
						{
							m_vecParticleInfoDesc[i].fScaleTimeAccs = 0.f;

							((VTXINSTANCE*)SubResource.pData)[i].vRight.x = SMath::fRandom((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y);
							((VTXINSTANCE*)SubResource.pData)[i].vUp.y = ((VTXINSTANCE*)SubResource.pData)[i].vRight.x;

							m_vecParticleInfoDesc[i].fScaleChange = SMath::fRandom((*m_tParticleDesc.pScaleChangeTime).x, (*m_tParticleDesc.pScaleChangeTime).y);
						}
					}
					else
					{
						// 확대
						if (m_vecParticleInfoDesc[i].bScaleAdd)
						{
							((VTXINSTANCE*)SubResource.pData)[i].vRight.x += m_vecParticleInfoDesc[i].fScaleSpeed * fTimeDelta;
							((VTXINSTANCE*)SubResource.pData)[i].vUp.y = ((VTXINSTANCE*)SubResource.pData)[i].vRight.x;

							if (((VTXINSTANCE*)SubResource.pData)[i].vRight.x > (*m_tParticleDesc.pScaleMax).x ||
								((VTXINSTANCE*)SubResource.pData)[i].vUp.y > (*m_tParticleDesc.pScaleMax).y)
							{
								// 반복 O
								if ((*m_tParticleDesc.pScaleLoop))
								{
									// 첫 크기로 반복
									if ((*m_tParticleDesc.pScaleLoopStart))
									{
										((VTXINSTANCE*)SubResource.pData)[i].vRight.x = SMath::fRandom((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y);
										((VTXINSTANCE*)SubResource.pData)[i].vUp.y = ((VTXINSTANCE*)SubResource.pData)[i].vRight.x;
									}
									else
									{
										// 반대로 진행
										m_vecParticleInfoDesc[i].bScaleAdd = false;
									}
								}
								// 반복 X
								else
								{
									((VTXINSTANCE*)SubResource.pData)[i].vRight.x = (*m_tParticleDesc.pScaleMax).x;
									((VTXINSTANCE*)SubResource.pData)[i].vUp.y = ((VTXINSTANCE*)SubResource.pData)[i].vRight.x;
								}
							}
						}
						// 축소
						else
						{
							((VTXINSTANCE*)SubResource.pData)[i].vRight.x -= m_vecParticleInfoDesc[i].fScaleSpeed * fTimeDelta;
							((VTXINSTANCE*)SubResource.pData)[i].vUp.y = ((VTXINSTANCE*)SubResource.pData)[i].vRight.x;

							if (((VTXINSTANCE*)SubResource.pData)[i].vRight.x < (*m_tParticleDesc.pScaleMin).x ||
								((VTXINSTANCE*)SubResource.pData)[i].vUp.y < (*m_tParticleDesc.pScaleMin).y)
							{
								// 반복 O
								if ((*m_tParticleDesc.pScaleLoop))
								{
									// 첫 크기로 반복
									if ((*m_tParticleDesc.pScaleLoopStart))
									{
										((VTXINSTANCE*)SubResource.pData)[i].vRight.x = SMath::fRandom((*m_tParticleDesc.pScaleStart).x, (*m_tParticleDesc.pScaleStart).y);
										((VTXINSTANCE*)SubResource.pData)[i].vUp.y = ((VTXINSTANCE*)SubResource.pData)[i].vRight.x;
									}
									else
									{
										m_vecParticleInfoDesc[i].bScaleAdd = true;
									}
								}
								// 반복 X
								else
								{
									((VTXINSTANCE*)SubResource.pData)[i].vRight.x = (*m_tParticleDesc.pScaleMin).x;
									((VTXINSTANCE*)SubResource.pData)[i].vUp.y = ((VTXINSTANCE*)SubResource.pData)[i].vRight.x;
								}
							}
						}
					}
				}
			}
#pragma endregion

#pragma region 회전 변경
			if ((*m_tParticleDesc.pRotationChange))
			{
				m_vecParticleInfoDesc[i].fRotationChangeStartTime += fTimeDelta;
				if (m_vecParticleInfoDesc[i].fRotationChangeStartTime >= m_vecParticleInfoDesc[i].fRotationChangeStartDelay)
				{
					if (!(*m_tParticleDesc.pRotationChangeRandom))
					{
						if ((*m_tParticleDesc.pRotationAdd))
							m_vecParticleShaderDesc[i].fAngle += fTimeDelta * m_vecParticleInfoDesc[i].fRotationSpeed;
						else
							m_vecParticleShaderDesc[i].fAngle -= fTimeDelta * m_vecParticleInfoDesc[i].fRotationSpeed;
					}
					else // 랜덤 체인지
					{
						m_vecParticleInfoDesc[i].fRotationTime += fTimeDelta;
						if (m_vecParticleInfoDesc[i].fRotationTime >= m_vecParticleInfoDesc[i].fRotationChangeTime)
						{
							m_vecParticleInfoDesc[i].fRotationTime = 0.f;
							m_vecParticleInfoDesc[i].fRotationChangeTime = SMath::fRandom((*m_tParticleDesc.pRotationChangeTime).x, (*m_tParticleDesc.pRotationChangeTime).y);
							m_vecParticleInfoDesc[i].fRotationSpeed = SMath::fRandom((*m_tParticleDesc.pRotationSpeed).x, (*m_tParticleDesc.pRotationSpeed).y);

							_uint iRandomCount = (_uint)SMath::fRandom(0.f, 2.9);
							switch (iRandomCount)
							{
							case 0:
								m_vecParticleShaderDesc[i].fAxis = _float3(1.f, 0.f, 0.f);
								break;
							case 1:
								m_vecParticleShaderDesc[i].fAxis = _float3(0.f, 1.f, 0.f);
								break;
							default:
								m_vecParticleShaderDesc[i].fAxis = _float3(0.f, 0.f, 1.f);
								break;
							}

							m_vecParticleShaderDesc[i].fAngle = SMath::fRandom(0.f, 360.f);
						}
					}
				}
			}
#pragma endregion

#pragma region 텍스처 애니메이션
			if ((*m_tParticleDesc.pAnimation))
			{
				if (!m_vecParticleInfoDesc[i].bAmimationFinish)
				{
					m_vecParticleInfoDesc[i].fAccIndex += m_vecParticleInfoDesc[i].fAnimationSpeed * fTimeDelta;
					if (m_vecParticleInfoDesc[i].fAccIndex >= 1.f)
					{
						m_vecParticleInfoDesc[i].fAccIndex = 0.f;
						m_vecParticleShaderDesc[i].fUVIndex.x++;
						if (m_vecParticleShaderDesc[i].fMaxCount.x <= m_vecParticleShaderDesc[i].fUVIndex.x)
						{
							m_vecParticleShaderDesc[i].fUVIndex.x = 0.f;
							m_vecParticleShaderDesc[i].fUVIndex.y++;
							if (m_vecParticleShaderDesc[i].fMaxCount.y <= m_vecParticleShaderDesc[i].fUVIndex.y)
							{
								if ((*m_tParticleDesc.pAnimationLoop))
									m_vecParticleShaderDesc[i].fUVIndex = _float2(0.f, 0.f);
								else
								{
									//m_vecParticleInfoDesc[i].bAmimationFinish = true;
									m_bFinished = true;
									m_vecParticleShaderDesc[i].fUVIndex = _float2(m_vecParticleShaderDesc[i].fMaxCount.x - 1, m_vecParticleShaderDesc[i].fMaxCount.y - 1);
								}
							}
						}
					}
				}
			}
#pragma endregion

#pragma region 색상 변경
			if ((*m_tParticleDesc.pColorChange))
			{
				m_vecParticleInfoDesc[i].fColorChangeStartTime += fTimeDelta;
				if (m_vecParticleInfoDesc[i].fColorChangeStartTime >= m_vecParticleInfoDesc[i].fColorChangeStartDelay)
				{
					// 랜덤 체인지
					if ((*m_tParticleDesc.pColorChangeRandom))
					{
						m_vecParticleInfoDesc[i].fColorAccs += fTimeDelta;
						if (m_vecParticleInfoDesc[i].fColorAccs >= m_vecParticleInfoDesc[i].fColorChangeTime)
						{
							if (!m_vecParticleInfoDesc[i].LerpInfo.bActive)
							{
								m_vecParticleInfoDesc[i].LerpInfo.Set_Start(_float3(m_vecParticleShaderDesc[i].fColor.x, m_vecParticleShaderDesc[i].fColor.y, m_vecParticleShaderDesc[i].fColor.z),
									_float3(m_vecParticleInfoDesc[i].fNextColor.x, m_vecParticleInfoDesc[i].fNextColor.y, m_vecParticleInfoDesc[i].fNextColor.z), m_vecParticleInfoDesc[i].fColorChangeEndTime);
							}
							else if(m_vecParticleInfoDesc[i].LerpInfo.bActive && m_vecParticleInfoDesc[i].LerpInfo.fTimeAcc >= m_vecParticleInfoDesc[i].LerpInfo.fEndTime)
							{
								m_vecParticleInfoDesc[i].LerpInfo   = {};
								m_vecParticleInfoDesc[i].fNextColor = _float3(SMath::fRandom(0.f, 1.f), SMath::fRandom(0.f, 1.f), SMath::fRandom(0.f, 1.f));
								m_vecParticleInfoDesc[i].fColorAccs = 0.f;
								m_vecParticleInfoDesc[i].fColorChangeTime = SMath::fRandom((*m_tParticleDesc.pColorChangeRandomTime).x, (*m_tParticleDesc.pColorChangeRandomTime).y);
							}
						}

						if (m_vecParticleInfoDesc[i].LerpInfo.bActive)
						{
							_float3 vNewColor = m_vecParticleInfoDesc[i].LerpInfo.Update_Lerp(fTimeDelta);
							m_vecParticleShaderDesc[i].fColor = _float3(vNewColor.x, vNewColor.y, vNewColor.z);
						}
					}
					else
					{
						// S -> M
						if (m_vecParticleInfoDesc[i].iColorIndex == 0 && m_vecParticleInfoDesc[i].fColorChangeStartTime >= m_vecParticleInfoDesc[i].fColorChangeStartM)
						{
							m_vecParticleInfoDesc[i].LerpInfo.Set_Start(_float3((*m_tParticleDesc.pColorS).x, (*m_tParticleDesc.pColorS).y, (*m_tParticleDesc.pColorS).z),
								_float3((*m_tParticleDesc.pColorM).x, (*m_tParticleDesc.pColorM).y, (*m_tParticleDesc.pColorM).z), m_vecParticleInfoDesc[i].fColorChangeEndTime);

							m_vecParticleInfoDesc[i].iColorIndex++;
						}
						// M -> F
						else if (m_vecParticleInfoDesc[i].iColorIndex == 1 && m_vecParticleInfoDesc[i].fColorChangeStartTime >= m_vecParticleInfoDesc[i].fColorChangeStartF)
						{
							if (m_vecParticleInfoDesc[i].LerpInfo.fTimeAcc >= m_vecParticleInfoDesc[i].LerpInfo.fEndTime)
							{
								m_vecParticleInfoDesc[i].LerpInfo.Set_Start(_float3((*m_tParticleDesc.pColorM).x, (*m_tParticleDesc.pColorM).y, (*m_tParticleDesc.pColorM).z),
									_float3((*m_tParticleDesc.pColorF).x, (*m_tParticleDesc.pColorF).y, (*m_tParticleDesc.pColorF).z), m_vecParticleInfoDesc[i].fColorChangeEndTime);

								m_vecParticleInfoDesc[i].iColorIndex++;
							}
						}

						if (m_vecParticleInfoDesc[i].LerpInfo.bActive)
						{
							_float3 vNewColor = m_vecParticleInfoDesc[i].LerpInfo.Update_Lerp(fTimeDelta);
							m_vecParticleShaderDesc[i].fColor = _float3(vNewColor.x, vNewColor.y, vNewColor.z);

							if (m_vecParticleInfoDesc[i].iColorIndex >= 2 && m_vecParticleInfoDesc[i].LerpInfo.fTimeAcc >= m_vecParticleInfoDesc[i].LerpInfo.fEndTime)
							{
								if ((*m_tParticleDesc.pColorLoop))
								{
									if (m_vecParticleInfoDesc[i].iColorIndex == 2 && m_vecParticleInfoDesc[i].LerpInfo.fTimeAcc >= m_vecParticleInfoDesc[i].LerpInfo.fEndTime)
									{
										m_vecParticleInfoDesc[i].LerpInfo.Set_Start(_float3((*m_tParticleDesc.pColorF).x, (*m_tParticleDesc.pColorF).y, (*m_tParticleDesc.pColorF).z),
											_float3((*m_tParticleDesc.pColorS).x, (*m_tParticleDesc.pColorS).y, (*m_tParticleDesc.pColorS).z), m_vecParticleInfoDesc[i].fColorChangeEndTime);

										m_vecParticleInfoDesc[i].iColorIndex++;
									}
									else if (m_vecParticleInfoDesc[i].iColorIndex == 3 && m_vecParticleInfoDesc[i].LerpInfo.fTimeAcc >= m_vecParticleInfoDesc[i].LerpInfo.fEndTime)
									{
										m_vecParticleInfoDesc[i].LerpInfo = {};

										//m_vecParticleInfoDesc[i].fColorChangeStartTime  = 0.f;
										//m_vecParticleInfoDesc[i].fColorChangeStartDelay = SMath::fRandom((*m_tParticleDesc.pColorChangeStartDelay).x, (*m_tParticleDesc.pColorChangeStartDelay).y);
										m_vecParticleInfoDesc[i].fColorChangeStartTime = m_vecParticleInfoDesc[i].fColorChangeStartM;

										m_vecParticleInfoDesc[i].iColorIndex = 0;
										m_vecParticleInfoDesc[i].fColorChangeStartM = SMath::fRandom((*m_tParticleDesc.pColorChangeStartM).x, (*m_tParticleDesc.pColorChangeStartM).y);
										m_vecParticleInfoDesc[i].fColorChangeStartF = SMath::fRandom((*m_tParticleDesc.pColorChangeStartF).x, (*m_tParticleDesc.pColorChangeStartF).y);

										m_vecParticleInfoDesc[i].fColorChangeEndTime = SMath::fRandom((*m_tParticleDesc.pColorDuration).x, (*m_tParticleDesc.pColorDuration).y);

										if ((*m_tParticleDesc.pColorRandom))
											m_vecParticleShaderDesc[i].fColor = _float3(SMath::fRandom(0.f, 1.f), SMath::fRandom(0.f, 1.f), SMath::fRandom(0.f, 1.f));
										else
											m_vecParticleShaderDesc[i].fColor = _float3((*m_tParticleDesc.pColorS).x, (*m_tParticleDesc.pColorS).y, (*m_tParticleDesc.pColorS).z);
									}
								}
								else
								{
									(*m_tParticleDesc.pColorChange) = false;
								}
							}
						}
					}
				}
			}
#pragma endregion

			// 지속 시간 검사
			if (m_vecParticleInfoDesc[i].fTimeAccs >= m_vecParticleInfoDesc[i].fLifeTimes)
				m_vecParticleInfoDesc[i].bIsDie = true;
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

HRESULT CVIBuffer_Particle::Bind_VIBuffers(_uint iCount)
{
	if (nullptr == m_pVB ||
		nullptr == m_pIB)
		return E_FAIL;

	m_iNumInstance = iCount;
	if (m_iNumInstance > m_iMaxCount)
		m_iNumInstance = m_iMaxCount;

	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB,
		m_pVBInstance,
	};

	_uint				iStrides[] = {
		m_iStride,
		m_iInstanceStride

	};

	_uint				iOffsets[] = {
		0,
		0,
	};


	/* 어떤 버텍스 버퍼들을 이용할거다. */
	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);

	/* 어떤 인덱스 버퍼를 이용할거다. */
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);

	/* 정점을 어떤식으로 이어서 그릴거다. */
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	return S_OK;
}

HRESULT CVIBuffer_Particle::Render()
{
	/* 인덱스가 가르키는 정점을 활용하여 그린다. */
	//m_pContext->DrawIndexedInstanced(m_iIndexStride * m_iNumIndices, m_iNumInstance, 0, 0, 0);
	m_pContext->DrawIndexedInstanced(m_iIndexCountPerInstance, m_iNumInstance, 0, 0, 0);

	return S_OK;
}


_float4 CVIBuffer_Particle::Get_NewPosition_Particle()
{
	return _float4(
		SMath::fRandom(-(*m_tParticleDesc.pRange).x, (*m_tParticleDesc.pRange).x),
		SMath::fRandom(-(*m_tParticleDesc.pRange).y, (*m_tParticleDesc.pRange).y),
		SMath::fRandom(-(*m_tParticleDesc.pRange).z, (*m_tParticleDesc.pRange).z), 1.f);
}

_float4 CVIBuffer_Particle::Get_NewVelocity_Particle()
{
	_vector vVelocity = XMVector3Normalize(
		XMVectorSet(
			SMath::fRandom((*m_tParticleDesc.pVelocityMinStart).x, (*m_tParticleDesc.pVelocityMaxStart).x),
			SMath::fRandom((*m_tParticleDesc.pVelocityMinStart).y, (*m_tParticleDesc.pVelocityMaxStart).y),
			SMath::fRandom((*m_tParticleDesc.pVelocityMinStart).z, (*m_tParticleDesc.pVelocityMaxStart).z),
			0.f));

	return _float4(XMVectorGetX(vVelocity), XMVectorGetY(vVelocity), XMVectorGetZ(vVelocity), XMVectorGetW(vVelocity));
}

void CVIBuffer_Particle::Tick_Rigidbody(_float fTimeDelta, _uint iParticleID)
{
	// 힘의 크기
	_float fForce = XMVectorGetX(XMVector3Length(m_vecParticleRigidbodyDesc[iParticleID].vForce));
	if (fForce != 0.f) {
		// 힘의 방향
		m_vecParticleRigidbodyDesc[iParticleID].vForce = XMVector3Normalize(m_vecParticleRigidbodyDesc[iParticleID].vForce);
		// 가속도의 크기
		_float fAccel = fForce / (*m_tParticleDesc.pMass);
		// 가속도
		m_vecParticleRigidbodyDesc[iParticleID].vAccel = m_vecParticleRigidbodyDesc[iParticleID].vForce * fAccel;
	}

	// 추가 가속도 누적 (중력)
	m_vecParticleRigidbodyDesc[iParticleID].vAccel += m_vecParticleRigidbodyDesc[iParticleID].vAccelA;

	// 속도 
	m_vecParticleRigidbodyDesc[iParticleID].vVelocity += m_vecParticleRigidbodyDesc[iParticleID].vAccel * fTimeDelta;

	// 마찰력에 의한 반대 방향의 가속도
	if (XMVectorGetX(XMVector3Length(m_vecParticleRigidbodyDesc[iParticleID].vVelocity)) != 0.f)
	{
		_vector vFricDir = -m_vecParticleRigidbodyDesc[iParticleID].vVelocity;
		vFricDir = XMVector3Normalize(vFricDir);
		_vector vFriction = vFricDir * (*m_tParticleDesc.pFricCoeff) * fTimeDelta;
		if (XMVectorGetX(XMVector3Length(m_vecParticleRigidbodyDesc[iParticleID].vVelocity)) <= XMVectorGetX(XMVector3Length(vFriction)))
			m_vecParticleRigidbodyDesc[iParticleID].vVelocity = XMVectorSet(0.f, 0.f, 0.f, 0.f); // 마찰 가속도가 본래 속도보다 더 큰 경우
		else
			m_vecParticleRigidbodyDesc[iParticleID].vVelocity += vFriction;
	}

	// 속도 제한 검사
	if (abs(XMVectorGetX((*m_tParticleDesc.pMaxVelocity))) < abs(XMVectorGetX(m_vecParticleRigidbodyDesc[iParticleID].vVelocity)))
		m_vecParticleRigidbodyDesc[iParticleID].vVelocity = XMVectorSetX(m_vecParticleRigidbodyDesc[iParticleID].vVelocity, XMVectorGetX(m_vecParticleRigidbodyDesc[iParticleID].vVelocity) / abs(XMVectorGetX(m_vecParticleRigidbodyDesc[iParticleID].vVelocity)) * abs(XMVectorGetX((*m_tParticleDesc.pMaxVelocity))));
	if (abs(XMVectorGetY((*m_tParticleDesc.pMaxVelocity))) < abs(XMVectorGetY(m_vecParticleRigidbodyDesc[iParticleID].vVelocity)))
		m_vecParticleRigidbodyDesc[iParticleID].vVelocity = XMVectorSetY(m_vecParticleRigidbodyDesc[iParticleID].vVelocity, XMVectorGetY(m_vecParticleRigidbodyDesc[iParticleID].vVelocity) / abs(XMVectorGetY(m_vecParticleRigidbodyDesc[iParticleID].vVelocity)) * abs(XMVectorGetY((*m_tParticleDesc.pMaxVelocity))));
	if (abs(XMVectorGetZ((*m_tParticleDesc.pMaxVelocity))) < abs(XMVectorGetZ(m_vecParticleRigidbodyDesc[iParticleID].vVelocity)))
		m_vecParticleRigidbodyDesc[iParticleID].vVelocity = XMVectorSetZ(m_vecParticleRigidbodyDesc[iParticleID].vVelocity, XMVectorGetZ(m_vecParticleRigidbodyDesc[iParticleID].vVelocity) / abs(XMVectorGetZ(m_vecParticleRigidbodyDesc[iParticleID].vVelocity)) * abs(XMVectorGetZ((*m_tParticleDesc.pMaxVelocity))));

	// 힘 초기화
	m_vecParticleRigidbodyDesc[iParticleID].vForce = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	// 가속도 초기화
	m_vecParticleRigidbodyDesc[iParticleID].vAccel  = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	m_vecParticleRigidbodyDesc[iParticleID].vAccelA = XMVectorSet(0.f, 0.f, 0.f, 0.f);
}


CVIBuffer_Particle* CVIBuffer_Particle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Particle* pInstance = new CVIBuffer_Particle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CVIBuffer_Particle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Particle::Clone(void* pArg)
{
	CVIBuffer_Particle* pInstance = new CVIBuffer_Particle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CVIBuffer_Particle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Particle::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);

	for (auto& iter : m_vecParticleInfoDesc)
	{
		if (iter.pVelocity != nullptr)
			Safe_Delete_Array(iter.pVelocity);

		if (iter.pVelocityChange != nullptr)
			Safe_Delete_Array(iter.pVelocityChange);
	}

	Safe_Delete_Array(m_pVertices);
	Safe_Release(m_pVBInstance);
}