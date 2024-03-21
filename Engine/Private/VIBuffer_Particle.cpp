#include "VIBuffer_Particle.h"

#include "SMath.h"
#include "Easing_Utillity.h"

#include "GameInstance.h"
#include "PipeLine.h"

CVIBuffer_Particle::CVIBuffer_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Particle::CVIBuffer_Particle(const CVIBuffer_Particle& rhs)
	: CVIBuffer(rhs)
	, m_tBufferDesc(rhs.m_tBufferDesc)
{
}

HRESULT CVIBuffer_Particle::Initialize_Prototype()
{
	m_iNumVertexBuffers = 2;
	m_iNumVertices = 1;
	m_iStride = sizeof(VTXPOINT);

	m_iNumIndices = m_tBufferDesc.iCurNumInstance;
	m_iIndexStride = 2;
	m_eIndexFormat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

#pragma region VERTEXBUFFER

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT /*D3D11_USAGE_DYNAMIC*/;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXPOINT* pVertices = new VTXPOINT;
	ZeroMemory(pVertices, sizeof(VTXPOINT));

	pVertices->vPosition = _float3(0.f, 0.f, 0.f);
	pVertices->vPSize = _float2(1.f, 1.f);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	/* pVertices�� �Ҵ��Ͽ� ä������ �������� ������ ID3D11Buffer�� �Ҵ��� ������ �����Ͽ� ä���ִ´�. */
	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
#pragma endregion

#pragma region INDEXBUFFER

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT /*D3D11_USAGE_DYNAMIC*/;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);

	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion


	return S_OK;

}

HRESULT CVIBuffer_Particle::Initialize(void* pArg)
{
	m_tBufferDesc = *(PARTICLE_BUFFER_DESC*)pArg;

	// ���� ���� ����
	m_vecParticleInfoDesc.reserve(m_tBufferDesc.iCurNumInstance);
	m_vecParticleShaderInfoDesc.reserve(m_tBufferDesc.iCurNumInstance);

	if (m_tBufferDesc.bUseRigidBody)
		m_vecParticleRigidbodyDesc.reserve(m_tBufferDesc.iCurNumInstance);


	// �ð� �ʱ�ȭ
	m_tBufferDesc.Reset_Times();

	// ==============================================================================================

	FAILED_CHECK(Init_Instance(m_tBufferDesc.iCurNumInstance));

	return S_OK;
}


HRESULT CVIBuffer_Particle::Bind_VIBuffers()
{
	if (nullptr == m_pVB ||
		nullptr == m_pIB)
		return E_FAIL;

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


	/* � ���ؽ� ���۵��� �̿��ҰŴ�. */
	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);

	/* � �ε��� ���۸� �̿��ҰŴ�. */
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);

	/* ������ ������� �̾ �׸��Ŵ�. */
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	return S_OK;
}

HRESULT CVIBuffer_Particle::Render()
{
	m_pContext->DrawIndexedInstanced(m_iIndexCountPerInstance, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

HRESULT CVIBuffer_Particle::Init_Instance(_int iNumInstance)
{
	m_iNumVertexBuffers = 2;
	m_iNumVertices = 1;
	m_iStride = sizeof(VTXPOINT);
	m_iInstanceStride = sizeof(VTXINSTANCE);
	m_iIndexCountPerInstance = 1;
	m_iNumInstance = iNumInstance;

	m_iNumIndices = iNumInstance;
	m_iIndexStride = 2;
	m_eIndexFormat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;


	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iInstanceStride;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);

	VTXINSTANCE* pVertices = new VTXINSTANCE[m_iNumInstance];

	m_iNumInstance = m_tBufferDesc.iCurNumInstance;
	for (_uint i = 0; i < m_iNumInstance; ++i)	// �ݺ��� ����
	{
		// �ʱ�ȭ�� ��ƼŬ ���� �ϳ��ϳ��� ���� Push_back
		PARTICLE_INFO_DESC tParticleInfo = {};
		m_vecParticleInfoDesc.push_back(tParticleInfo);

		PARTICLE_SHADER_INFO_DESC tParticleShaderInfo = {};
		m_vecParticleShaderInfoDesc.push_back(tParticleShaderInfo);


		// ������ٵ� ����̸� ���� �ϳ��ϳ��� ���� �ʱ�ȭ�� ������ٵ����� Push_back		
		if (m_tBufferDesc.bUseRigidBody)
		{
			PARTICLE_RIGIDBODY_DESC tParticleRigidbody = {};
			m_vecParticleRigidbodyDesc.push_back(tParticleRigidbody);
		}


		ReSet_Info(i);


		//// �ʱ�ȭ
		////XMStoreFloat4(&pVertices[i].vPosition, m_tBufferDesc.vCenterPosition);
		//XMStoreFloat4(&pVertices[i].vPosition, m_vecParticleInfoDesc[i].vCenterPositions);
		//pVertices[i].vRight = _float4(1.f, 0.f, 0.f, 0.f) * m_tBufferDesc.vMinMaxWidth.x;
		//pVertices[i].vUp	= _float4(0.f, 1.f, 0.f, 0.f) * m_tBufferDesc.vMinMaxHeight.x;
		//pVertices[i].vLook	= _float4(0.f, 0.f, 1.f, 0.f);


		// ���� ũ�� �ʱ�ȭ
		m_vecParticleInfoDesc[i].vCurScales.x = m_tBufferDesc.vMinMaxWidth.x;
		m_vecParticleInfoDesc[i].vCurScales.y = m_tBufferDesc.vMinMaxHeight.x;

		Rotation_Instance(i);	// �� ��ü ȸ��

		// ���̴��� ���� ����� ������ �ʱ�ȭ
		pVertices[i].vRight = m_vecParticleShaderInfoDesc[i].vRight;
		pVertices[i].vUp	= m_vecParticleShaderInfoDesc[i].vUp;
		pVertices[i].vLook	= m_vecParticleShaderInfoDesc[i].vLook;


		if (m_tBufferDesc.bUseRigidBody)	// ������ٵ� ����̸�
		{
			// ���� ��ġ�� ����
			XMStoreFloat4(&pVertices[i].vPosition, m_vecParticleInfoDesc[i].vCenterPositions);
		}
		else
		{
			// ������ٵ� ����� �ƴҰ��

			_vector		vDir = Make_Dir(i);						// ���� �����
			m_vecParticleShaderInfoDesc[i].vDir = vDir;			// ���̴��� ������ ���� ����
			if (SPARK == m_tBufferDesc.eType_Action)
			{
				Update_Spark_Rotation(i);
			}

			if (m_tBufferDesc.bReverse)
			{
				// �������� ���
				vDir = vDir * m_vecParticleInfoDesc[i].fMaxRange;
			}

			// ���� + ���� ��ġ�� ����
			XMStoreFloat4(&pVertices[i].vPosition, XMLoadFloat4(&m_vecParticleInfoDesc[i].vCenterPositions) + vDir);

		}


	} // �ݺ��� ��


	m_SubResourceData.pSysMem = pVertices;

	FAILED_CHECK(__super::Create_Buffer(&m_pVBInstance));

	Safe_Delete_Array(pVertices);

	return S_OK;

}


void CVIBuffer_Particle::ReSet()
{
	// �ð� �ʱ�ȭ
	m_tBufferDesc.Reset_Times();

	// ==============================================================================================

	D3D11_MAPPED_SUBRESOURCE			SubResource = {};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXINSTANCE* pVertices = ((VTXINSTANCE*)SubResource.pData);

	m_iNumInstance = m_tBufferDesc.iCurNumInstance;
	for (_uint i = 0; i < m_iNumInstance; ++i)	// �ݺ��� ����
	{

		ReSet_Info(i);


		//// �ʱ�ȭ
		////XMStoreFloat4(&pVertices[i].vPosition, m_tBufferDesc.vCenterPosition);
		//XMStoreFloat4(&pVertices[i].vPosition, m_vecParticleInfoDesc[i].vCenterPositions);
		//pVertices[i].vRight = _float4(1.f, 0.f, 0.f, 0.f) * m_tBufferDesc.vMinMaxWidth.x;
		//pVertices[i].vUp	= _float4(0.f, 1.f, 0.f, 0.f) * m_tBufferDesc.vMinMaxHeight.x;
		//pVertices[i].vLook	= _float4(0.f, 0.f, 1.f, 0.f);
		//pVertices[i].vColor.w = 1.f;
		

		// ���� ũ�� �ʱ�ȭ
		m_vecParticleInfoDesc[i].vCurScales.x = m_tBufferDesc.vMinMaxWidth.x;
		m_vecParticleInfoDesc[i].vCurScales.y = m_tBufferDesc.vMinMaxHeight.x;


		Rotation_Instance(i);	// �� ��ü ȸ��

		// ���̴��� ���� ����� ������ �ʱ�ȭ
		pVertices[i].vRight = m_vecParticleShaderInfoDesc[i].vRight;
		pVertices[i].vUp = m_vecParticleShaderInfoDesc[i].vUp;
		pVertices[i].vLook = m_vecParticleShaderInfoDesc[i].vLook;



		if (m_tBufferDesc.bUseRigidBody)	// ������ٵ� ����̸�
		{
			// ���� ��ġ�� ����
			XMStoreFloat4(&pVertices[i].vPosition, m_vecParticleInfoDesc[i].vCenterPositions);
		}
		else  
		{
			// ������ٵ� ����� �ƴҰ��

			_vector		vDir = Make_Dir(i);						// ���� �����
			m_vecParticleShaderInfoDesc[i].vDir = vDir;			// ���̴��� ������ ���� ����
			if (SPARK == m_tBufferDesc.eType_Action)
			{
				Update_Spark_Rotation(i);
			}

			if (m_tBufferDesc.bReverse)
			{
				// �������� ���
				vDir = vDir * m_vecParticleInfoDesc[i].fMaxRange;
			}

			// ���� + ���� ��ġ�� ����
			XMStoreFloat4(&pVertices[i].vPosition, XMLoadFloat4(&m_vecParticleInfoDesc[i].vCenterPositions) + vDir);

		}



	} // �ݺ��� ��


	m_pContext->Unmap(m_pVBInstance, 0);
}


void CVIBuffer_Particle::ReSet_Info(_uint iNum)
{

	// ������Ÿ��
	m_vecParticleInfoDesc[iNum].Reset_ParticleTimes();
	m_vecParticleInfoDesc[iNum].fLifeTime = SMath::fRandom(m_tBufferDesc.vMinMaxLifeTime.x, m_tBufferDesc.vMinMaxLifeTime.y);



	// ���� ������ Offset
	m_vecParticleInfoDesc[iNum].vCenterPositions.x = SMath::fRandom(m_tBufferDesc.vMinCenterOffsetPos.x, m_tBufferDesc.vMaxCenterOffsetPos.x);
	m_vecParticleInfoDesc[iNum].vCenterPositions.y = SMath::fRandom(m_tBufferDesc.vMinCenterOffsetPos.y, m_tBufferDesc.vMaxCenterOffsetPos.y);
	m_vecParticleInfoDesc[iNum].vCenterPositions.z = SMath::fRandom(m_tBufferDesc.vMinCenterOffsetPos.z, m_tBufferDesc.vMaxCenterOffsetPos.z);
	m_vecParticleInfoDesc[iNum].vCenterPositions.w = 1.f;


	// �ִ� �Ÿ�
	m_vecParticleInfoDesc[iNum].fMaxRange = SMath::fRandom(m_tBufferDesc.vMinMaxRange.x, m_tBufferDesc.vMinMaxRange.y);


	// ���ǵ�
	m_vecParticleInfoDesc[iNum].fCurSpeed = SMath::fRandom(m_tBufferDesc.vMinMaxSpeed.x, m_tBufferDesc.vMinMaxSpeed.y);


	// ũ��
	if (m_tBufferDesc.bUseScaleLerp)
	{
		// ������ ���� ����̸� �ִ� ũ�� ����
		m_vecParticleInfoDesc[iNum].vMaxScales.x = m_tBufferDesc.vMinMaxWidth.y;
		m_vecParticleInfoDesc[iNum].vMaxScales.y = m_tBufferDesc.vMinMaxHeight.y;
	}
	else
	{
		// ������ ���� ����� �ƴϸ� ���� �������� ���� �� ����
		m_vecParticleInfoDesc[iNum].vCurScales.x = SMath::fRandom(m_tBufferDesc.vMinMaxWidth.x, m_tBufferDesc.vMinMaxWidth.y);
		m_vecParticleInfoDesc[iNum].vCurScales.y = SMath::fRandom(m_tBufferDesc.vMinMaxHeight.x, m_tBufferDesc.vMinMaxHeight.y);
	}



#pragma region ������ٵ� ����
	// ������ �ٵ� ����̸�
	if (m_tBufferDesc.bUseRigidBody)
	{
		Clear_Power(iNum);	// �Ŀ� ����
		
		m_vecParticleRigidbodyDesc[iNum].fMass = SMath::fRandom(m_tBufferDesc.vMinMaxMass.x, m_tBufferDesc.vMinMaxMass.y);			// ���� ����

		// �̵� �������� �� �༭ �̵�
		_vector		vDir = Make_Dir(iNum);

		m_vecParticleShaderInfoDesc[iNum].vDir = vDir;			// ���̴��� ������ ���� ����
		m_vecParticleRigidbodyDesc[iNum].vDir = vDir;			// ������ٵ� ���� ����

		_vector vForce = vDir * SMath::fRandom(m_tBufferDesc.vMinMaxPower.x, m_tBufferDesc.vMinMaxPower.y);
		Add_Force(iNum, vForce, m_tBufferDesc.eForce_Mode);
	}
#pragma endregion ������ٵ� ��



}

_float4 CVIBuffer_Particle::Make_Dir(_uint iNum)
{
	_vector		vDir = XMVectorSet(1.f, 0.f, 0.f, 0.f);


	vDir = XMVector3Normalize(vDir) * m_vecParticleInfoDesc[iNum].fMaxRange;

	_float3 vRotationOffset = { XMConvertToRadians(SMath::fRandom(m_tBufferDesc.vMinMaxRotationOffsetX.x, m_tBufferDesc.vMinMaxRotationOffsetX.y))
							  , XMConvertToRadians(SMath::fRandom(m_tBufferDesc.vMinMaxRotationOffsetY.x, m_tBufferDesc.vMinMaxRotationOffsetY.y))
							  , XMConvertToRadians(SMath::fRandom(m_tBufferDesc.vMinMaxRotationOffsetZ.x, m_tBufferDesc.vMinMaxRotationOffsetZ.y)) };


	_vector		vRotation = XMQuaternionRotationRollPitchYaw(vRotationOffset.x, vRotationOffset.y, vRotationOffset.z);
	_matrix		RotationMatrix = XMMatrixRotationQuaternion(vRotation);

	vDir = XMVector3TransformNormal(vDir, RotationMatrix);	// ������ ���⺤�� ȸ�� ����


	return vDir;
}


void CVIBuffer_Particle::Rotation_Instance(_uint iNum)
{
	_vector		vRight		= XMVectorSet(1.f, 0.f, 0.f, 0.f) * m_vecParticleInfoDesc[iNum].vCurScales.x;
	_vector		vUp			= XMVectorSet(0.f, 1.f, 0.f, 0.f) * m_vecParticleInfoDesc[iNum].vCurScales.y;
	_vector		vLook		= XMVectorSet(0.f, 0.f, 1.f, 0.f);


	_vector		vRotation = XMQuaternionRotationRollPitchYaw( XMConvertToRadians(m_tBufferDesc.vRadian.x)
															, XMConvertToRadians(m_tBufferDesc.vRadian.y)
															, XMConvertToRadians(m_tBufferDesc.vRadian.z));

	_matrix		RotationMatrix = XMMatrixRotationQuaternion(vRotation);


	// ���̴��� ���� ��(�����) ���� 
	m_vecParticleShaderInfoDesc[iNum].vRight	= XMVector3TransformNormal(vRight, RotationMatrix);
	m_vecParticleShaderInfoDesc[iNum].vUp		= XMVector3TransformNormal(vUp, RotationMatrix);
	m_vecParticleShaderInfoDesc[iNum].vLook		= XMVector3TransformNormal(vLook, RotationMatrix);

}

void CVIBuffer_Particle::Update_Spark_Rotation(_uint iNum)
{	
	_vector		vRight, vUp, vLook;

	// �̵� ���� ���⺤�͸� Up���� �� ���ο� Right, Look �����ֱ� ===================================
	vUp		= XMVector4Normalize(m_vecParticleShaderInfoDesc[iNum].vDir) * m_vecParticleInfoDesc[iNum].vCurScales.y;
	vLook	= XMVector4Normalize(XMVector3Cross(float4(0.f, 1.f, 0.f, 0.f), vUp));
	vRight	= XMVector4Normalize(XMVector3Cross(vUp, vLook)) * m_vecParticleInfoDesc[iNum].vCurScales.x;
	vLook	= XMVector4Normalize(XMVector3Cross(vRight, vUp));


	m_vecParticleShaderInfoDesc[iNum].vRight = vRight;
	m_vecParticleShaderInfoDesc[iNum].vUp = vUp;
	m_vecParticleShaderInfoDesc[iNum].vLook = vLook;
}


void CVIBuffer_Particle::Update(_float fTimeDelta)
{
#pragma region Map UnMap �� ���� üũ ����
	if (0 >= m_iNumInstance)	// �ν��Ͻ� ������ 0�� ���ϸ� Ż��
		return;

	// ���� �ð��� �ִ� ������Ÿ�Ӻ��� Ŀ���� �ð� ���� ���� & Ż��
	if (m_tBufferDesc.fTimeAcc > m_tBufferDesc.vMinMaxLifeTime.y)
	{
		m_tBufferDesc.fTimeAcc = m_tBufferDesc.vMinMaxLifeTime.y;
		m_tBufferDesc.fLifeTimeRatio = 1.f;
		return;
	}

	// �ð� ����(��ü)
	m_tBufferDesc.fTimeAcc += fTimeDelta;
	m_tBufferDesc.fLifeTimeRatio = min(1.0f, m_tBufferDesc.fTimeAcc / m_tBufferDesc.vMinMaxLifeTime.y);
#pragma region Map UnMap �� ���� üũ ��


	D3D11_MAPPED_SUBRESOURCE			SubResource = {};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXINSTANCE* pVertices = ((VTXINSTANCE*)SubResource.pData);



	m_iNumInstance = m_tBufferDesc.iCurNumInstance;

	for (_uint i = 0; i < m_iNumInstance; i++)	// �ݺ��� ����
	{
		if (m_vecParticleInfoDesc[i].bDie)
		{
			if (m_tBufferDesc.bRecycle)
			{

			}
			else
			{

			}

		}

#pragma region ���ڵ� �ð� ����
		// ���ڵ��� �ð� ������ (���ڵ���)������Ÿ�Ӻ��� Ŀ���� �ð� ���� ���� & ���� �ݺ�����
		if (m_vecParticleInfoDesc[i].fTimeAccs > m_vecParticleInfoDesc[i].fLifeTime)
		{
			m_vecParticleInfoDesc[i].fTimeAccs = m_vecParticleInfoDesc[i].fLifeTime;
			m_vecParticleInfoDesc[i].fLifeTimeRatios = 1.f;

		}
		else
		{
			// �ð� ����(����)
			m_vecParticleInfoDesc[i].fTimeAccs += fTimeDelta;
			m_vecParticleInfoDesc[i].fLifeTimeRatios = min(1.0f, m_vecParticleInfoDesc[i].fTimeAccs / m_vecParticleInfoDesc[i].fLifeTime);
		}

#pragma region ���ڵ� �ð� ��



#pragma region ũ�� ���� ����
		if (m_tBufferDesc.bUseScaleLerp) // ũ�� ���� ���� ����̸�
		{
			if (m_tBufferDesc.fLifeTimeRatio >= m_tBufferDesc.vScaleLerp_Up_Pos.x)		// 0~1�� ������ ������Ÿ���� ũ�� ������ ������ Ÿ�� �������� �Ѿ��
			{
				if (m_tBufferDesc.fLifeTimeRatio >= m_tBufferDesc.vScaleLerp_Down_Pos.x) // 0~1�� ������ ������Ÿ���� ũ�� ���Ҹ� ������ Ÿ�� �����ǵ� �Ѿ�� ���� ����
				{
					// ũ�� ���Ҹ� ������ Ÿ�� ������ (ũ�� 0�� ��ǥ)
					_float fTotalTime = m_tBufferDesc.vMinMaxLifeTime.y * (1.f - m_tBufferDesc.vScaleLerp_Down_Pos.x);	// ������ Ÿ�� ��, ���ҿ��� �ʿ��� ��Ż�ð� ���
					if (m_vecParticleInfoDesc[i].fDownScaleTimeAccs >= fTotalTime)
					{
						m_vecParticleInfoDesc[i].fDownScaleTimeAccs = fTotalTime;
						m_vecParticleInfoDesc[i].vCurScales.x = 0.f;
						m_vecParticleInfoDesc[i].vCurScales.y = 0.f;
					}
					else
					{
						m_vecParticleInfoDesc[i].fDownScaleTimeAccs += fTimeDelta;	// �ð� ����	
						m_vecParticleInfoDesc[i].vCurScales.x = abs(Easing::LerpToType(m_vecParticleInfoDesc[i].vMaxScales.x, 0.f, m_vecParticleInfoDesc[i].fDownScaleTimeAccs, fTotalTime, m_tBufferDesc.eType_ScaleLerp));
						m_vecParticleInfoDesc[i].vCurScales.y = abs(Easing::LerpToType(m_vecParticleInfoDesc[i].vMaxScales.y, 0.f, m_vecParticleInfoDesc[i].fDownScaleTimeAccs, fTotalTime, m_tBufferDesc.eType_ScaleLerp));
					}
				}
				else
				{
					// Maxũ�Ⱑ ��ǥ
					_float fTotalTime = m_tBufferDesc.vMinMaxLifeTime.y * (m_tBufferDesc.vScaleLerp_Up_Pos.y - m_tBufferDesc.vScaleLerp_Up_Pos.x);	// ������ Ÿ�� ��, �������� �ʿ��� ��Ż�ð� ���
					if (m_vecParticleInfoDesc[i].fUpScaleTimeAccs >= fTotalTime)
					{
						m_vecParticleInfoDesc[i].fUpScaleTimeAccs = fTotalTime;
						m_vecParticleInfoDesc[i].vCurScales.x = m_vecParticleInfoDesc[i].vMaxScales.x;
						m_vecParticleInfoDesc[i].vCurScales.y = m_vecParticleInfoDesc[i].vMaxScales.y;
					}
					else
					{
						m_vecParticleInfoDesc[i].fUpScaleTimeAccs += fTimeDelta;	// �ð� ����		
						m_vecParticleInfoDesc[i].vCurScales.x = abs(Easing::LerpToType(m_tBufferDesc.vMinMaxWidth.x, m_vecParticleInfoDesc[i].vMaxScales.x, m_vecParticleInfoDesc[i].fUpScaleTimeAccs, fTotalTime, m_tBufferDesc.eType_ScaleLerp));
						m_vecParticleInfoDesc[i].vCurScales.y = abs(Easing::LerpToType(m_tBufferDesc.vMinMaxHeight.x, m_vecParticleInfoDesc[i].vMaxScales.y, m_vecParticleInfoDesc[i].fUpScaleTimeAccs, fTotalTime, m_tBufferDesc.eType_ScaleLerp));
					}

				}

			}
		}

		// ũ�⺯�� ����
		pVertices[i].vRight = _float4(1.f, 0.f, 0.f, 0.f) * m_vecParticleInfoDesc[i].vCurScales.x;
		pVertices[i].vUp	= _float4(0.f, 1.f, 0.f, 0.f) * m_vecParticleInfoDesc[i].vCurScales.y;

#pragma region ũ�� ���� ��


#pragma region ȸ�� : ��ü ȸ�� ����
		Rotation_Instance(i);

		// ���̴��� ���� ����� ������ �ʱ�ȭ
		pVertices[i].vRight = m_vecParticleShaderInfoDesc[i].vRight;
		pVertices[i].vUp = m_vecParticleShaderInfoDesc[i].vUp;
		pVertices[i].vLook = m_vecParticleShaderInfoDesc[i].vLook;
#pragma region ȸ�� : ��ü ȸ�� ��


#pragma region �̵� : ������ٵ� ����
		// ������ٵ� ����̸�
		if (m_tBufferDesc.bUseRigidBody)
		{
			if (!m_vecParticleRigidbodyDesc.empty())
			{
				if (!Check_Sleep(i))	// ������ �ƴϸ� ������ٵ� ������Ʈ
				{
					if (m_tBufferDesc.bKinetic)
					{
						// ���ǵ� ����
						m_vecParticleInfoDesc[i].fCurSpeed = abs(Easing::LerpToType(m_tBufferDesc.vMinMaxSpeed.x, m_tBufferDesc.vMinMaxSpeed.y, m_vecParticleInfoDesc[i].fTimeAccs, m_vecParticleInfoDesc[i].fLifeTime, m_tBufferDesc.eType_SpeedLerp));

						Update_Kinetic(i, fTimeDelta);	// �̵� �ӷ� ��� ������Ʈ

						// ���� �ӷ����� �̵��� ��ġ ��� / Translate : vMovePos = vPos + Get_State(CTransform::STATE_POSITION);
						_vector vMovePos = (XMLoadFloat3(&m_vecParticleRigidbodyDesc[i].vVelocity) * (m_vecParticleInfoDesc[i].fCurSpeed * fTimeDelta)) + XMLoadFloat4(&pVertices[i].vPosition); XMVectorSetW(vMovePos, 1.f);
						XMStoreFloat4(&pVertices[i].vPosition, vMovePos);	// ���� ��ġ �̵� ����


						m_vecParticleShaderInfoDesc[i].vDir = m_vecParticleRigidbodyDesc[i].vVelocity;			// ���̴��� ������ ���� ����
					}
					else
					{
						Update_Kinematic(i);
					}
				}

				if (SPARK == m_tBufferDesc.eType_Action)
				{
					Update_Spark_Rotation(i);
				}
			}

		}
#pragma endregion �̵� : ������ٵ� ��

#pragma region �̵� : ���� �̵� ����
		else
		{
			// ������ٵ� ����� �ƴϸ� ���� �̵�
			if (SPARK == m_tBufferDesc.eType_Action)
			{

				// �̵� :: ���� ������ + (���ڵ��� ���� * (���� ���ǵ� * Ÿ�ӵ�Ÿ))
				_vector vMovePos = XMLoadFloat4(&pVertices[i].vPosition) + m_vecParticleShaderInfoDesc[i].vDir * (m_vecParticleInfoDesc[i].fCurSpeed * fTimeDelta);
				XMStoreFloat4(&pVertices[i].vPosition, vMovePos);


				// �ʱ�ȭ ����		
				_vector vCurPos = XMLoadFloat4(&pVertices[i].vPosition);
				_float	fLength = XMVectorGetX(XMVector3Length(vCurPos - XMLoadFloat4(&m_vecParticleInfoDesc[i].vCenterPositions)));	// ���Ϳ��� ���� ��ġ������ �Ÿ�

				if (m_vecParticleInfoDesc[i].fMaxRange <= fLength)	// ���� �̵� �Ÿ��� �ƽ� ���������� ũ�ų� ������ �ʱ�ȭ or ����
				{
					if (m_tBufferDesc.bRecycle)	// ������ true�̸� �ʱ�ȭ
					{
						m_vecParticleInfoDesc[i].fMaxRange = SMath::fRandom(m_tBufferDesc.vMinMaxRange.x, m_tBufferDesc.vMinMaxRange.y);
						pVertices[i].vPosition = m_vecParticleInfoDesc[i].vCenterPositions;

						m_vecParticleInfoDesc[i].Reset_ParticleTimes(); // �ð� �ʱ�ȭ
					}
					else
					{
						m_vecParticleInfoDesc[i].bDie = TRUE;
					}
				}

			}
			else if (BLINK == m_tBufferDesc.eType_Action)
			{
				// �̵� ����



			}
			else if (FALL == m_tBufferDesc.eType_Action)
			{


			}
			else if (RISE == m_tBufferDesc.eType_Action)
			{
				pVertices[i].vPosition.y += m_vecParticleInfoDesc[i].fCurSpeed * fTimeDelta;	// �̵�

				// �ʱ�ȭ ����
				if (m_vecParticleInfoDesc[i].fMaxRange <= pVertices[i].vPosition.y)	// ���� y��ġ�� �ִ� �������� ũ�� �ʱ�ȭ or ����
				{
					if (m_tBufferDesc.bRecycle)	// ������ true�̸� �ʱ�ȭ
					{
						m_vecParticleInfoDesc[i].fMaxRange = SMath::fRandom(m_tBufferDesc.vMinMaxRange.x, m_tBufferDesc.vMinMaxRange.y);
						pVertices[i].vPosition.y = m_tBufferDesc.vMinMaxRange.x;

						m_vecParticleInfoDesc[i].Reset_ParticleTimes(); // �ð� �ʱ�ȭ
					}
					else
					{
						m_vecParticleInfoDesc[i].bDie = TRUE;
					}
				}
				
			}
			else if (TORNADO == m_tBufferDesc.eType_Action)
			{

				// �� ������ �ʱ� ��ġ�� �߽����� �ϴ� �� ������ ��θ� ���� ȸ���ϵ��� ����
				_float radius = 10.0f; // ���� ������
				_float angularSpeed = 0.5f; // ȸ�� �ӵ�

				_float theta = m_vecParticleInfoDesc[i].fCurSpeed * fTimeDelta * angularSpeed; // ȸ�� ����
				_float x = cos(theta) * radius; // x ��ǥ
				_float z = sin(theta) * radius; // z ��ǥ

				// ������ ��ġ�� ������Ʈ�Ͽ� ȸ�� ȿ�� ����
				pVertices[i].vPosition.x = x;
				pVertices[i].vPosition.z = z;

			}

		}
#pragma endregion �̵� : ���� �̵� ��



#pragma region �� ���� ����
		if (m_tBufferDesc.bDynamic_Color)	// ���ڸ��� �ٸ� �ֱ�� �� ����
		{
			m_vecParticleInfoDesc[i].vCurrentColors.x = abs(Easing::LerpToType(m_tBufferDesc.vMinMaxRed.x, m_tBufferDesc.vMinMaxRed.y, m_vecParticleInfoDesc[i].fTimeAccs, m_vecParticleInfoDesc[i].fLifeTime, m_tBufferDesc.eType_ColorLerp));
			m_vecParticleInfoDesc[i].vCurrentColors.y = abs(Easing::LerpToType(m_tBufferDesc.vMinMaxGreen.x, m_tBufferDesc.vMinMaxGreen.y, m_vecParticleInfoDesc[i].fTimeAccs, m_vecParticleInfoDesc[i].fLifeTime, m_tBufferDesc.eType_ColorLerp));
			m_vecParticleInfoDesc[i].vCurrentColors.z = abs(Easing::LerpToType(m_tBufferDesc.vMinMaxBlue.x, m_tBufferDesc.vMinMaxBlue.y, m_vecParticleInfoDesc[i].fTimeAccs, m_vecParticleInfoDesc[i].fLifeTime, m_tBufferDesc.eType_ColorLerp));

			// ���̴��� ���� ���ϱ� ���� �� ����
			m_vecParticleShaderInfoDesc[i].vCurrentColors = m_vecParticleInfoDesc[i].vCurrentColors;
			pVertices[i].vColor = m_vecParticleInfoDesc[i].vCurrentColors;
		}
		else // �ϰ� �� ����
		{
			m_tBufferDesc.vCurrentColor.x = abs(Easing::LerpToType(m_tBufferDesc.vMinMaxRed.x, m_tBufferDesc.vMinMaxRed.y, m_tBufferDesc.fTimeAcc, m_tBufferDesc.vMinMaxLifeTime.y, m_tBufferDesc.eType_ColorLerp));
			m_tBufferDesc.vCurrentColor.y = abs(Easing::LerpToType(m_tBufferDesc.vMinMaxGreen.x, m_tBufferDesc.vMinMaxGreen.y, m_tBufferDesc.fTimeAcc, m_tBufferDesc.vMinMaxLifeTime.y, m_tBufferDesc.eType_ColorLerp));
			m_tBufferDesc.vCurrentColor.z = abs(Easing::LerpToType(m_tBufferDesc.vMinMaxBlue.x, m_tBufferDesc.vMinMaxBlue.y, m_tBufferDesc.fTimeAcc, m_tBufferDesc.vMinMaxLifeTime.y, m_tBufferDesc.eType_ColorLerp));

			// ���̴��� ���� ���ϱ� ���� �� ����
			m_vecParticleShaderInfoDesc[i].vCurrentColors = m_tBufferDesc.vCurrentColor;
			pVertices[i].vColor = m_tBufferDesc.vCurrentColor;
		}

		// ����
		_float		fAlpha;
		if (FADE_NONE == m_tBufferDesc.eType_Fade)
		{
			fAlpha = 1.f;
		}
		else if (FADE_OUT == m_tBufferDesc.eType_Fade)
		{
			fAlpha = max((m_vecParticleInfoDesc[i].fLifeTime) - m_tBufferDesc.fTimeAcc, 0.f);	
		}
		else if (FADE_IN == m_tBufferDesc.eType_Fade)
		{
			fAlpha = min((m_vecParticleInfoDesc[i].fLifeTime) + m_tBufferDesc.fTimeAcc, 1.f);
		}
		pVertices[i].vColor.w = fAlpha;

#pragma region �� ���� ��




	} // �ݺ��� ��

	m_pContext->Unmap(m_pVBInstance, 0);

}

_float3 CVIBuffer_Particle::Update_Kinetic(_uint iNum, _float fTimeDelta)
{
	/* �߷� ��� �� �Ʒ��� ������ */
	if (m_tBufferDesc.bUseGravity)
		m_vecParticleRigidbodyDesc[iNum].vVelocity.y += m_tBufferDesc.fGravity * fTimeDelta;

	/* V += A * TD */
	/* m_vVelocity += m_vAccel * fTimeDelta; */
	XMStoreFloat3(&m_vecParticleRigidbodyDesc[iNum].vVelocity, XMLoadFloat3(&m_vecParticleRigidbodyDesc[iNum].vVelocity) += XMLoadFloat3(&m_vecParticleRigidbodyDesc[iNum].vAccel) * fTimeDelta);


	/* �����¿� ���� �ݴ���������� ���ӵ�(����) */
	if (1.f > m_tBufferDesc.fFriction)
	{
		/* (m_vVelocity * (1.f - m_fFriction)) */
		XMStoreFloat3(&m_vecParticleRigidbodyDesc[iNum].vVelocity, XMLoadFloat3(&m_vecParticleRigidbodyDesc[iNum].vVelocity) * (1.f - m_tBufferDesc.fFriction));
	}
	else
	{
		m_vecParticleRigidbodyDesc[iNum].vVelocity = { 0.f, 0.f, 0.f };
	}


	/* ��� �࿡ ���� �� ���� */
	if (m_tBufferDesc.byFreezeAxis)
	{
		for (_int i = 0; i < 3; ++i)
		{
			if (m_tBufferDesc.byFreezeAxis & 1 << i) // Ư�� ��Ʈ�� 1���� Ȯ��(1�̸� ��� ��?)
			{
				*((_float*)&m_vecParticleRigidbodyDesc[iNum].vVelocity + i) = 0.f;
			}
		}
	}

	return m_vecParticleRigidbodyDesc[iNum].vVelocity;
}

void CVIBuffer_Particle::Update_Kinematic(_uint iNum)
{
	/* ���� ����, ���� �ӵ� ��ȭ �ο� (Ű�׸�ƽ, �� �ӵ� ������) */
	Clear_Force(iNum, FORCE_MODE::VELOCITY_CHANGE);
}


void CVIBuffer_Particle::Add_Force(_uint iNum, _fvector vForce, FORCE_MODE eMode)
{
	switch (eMode)
	{
	case FORCE_MODE::FORCE:
		XMStoreFloat3(&m_vecParticleRigidbodyDesc[iNum].vAccel, XMLoadFloat3(&m_vecParticleRigidbodyDesc[iNum].vAccel) += vForce / m_vecParticleRigidbodyDesc[iNum].fMass);
		break;

	case FORCE_MODE::IMPULSE:
		XMStoreFloat3(&m_vecParticleRigidbodyDesc[iNum].vVelocity, XMLoadFloat3(&m_vecParticleRigidbodyDesc[iNum].vVelocity) += vForce / m_vecParticleRigidbodyDesc[iNum].fMass);
		break;

	case FORCE_MODE::ACCELERATION:
		XMStoreFloat3(&m_vecParticleRigidbodyDesc[iNum].vAccel, XMLoadFloat3(&m_vecParticleRigidbodyDesc[iNum].vAccel) += vForce);
		break;

	case FORCE_MODE::VELOCITY_CHANGE:
		XMStoreFloat3(&m_vecParticleRigidbodyDesc[iNum].vVelocity, XMLoadFloat3(&m_vecParticleRigidbodyDesc[iNum].vVelocity) += vForce);
		break;

	default:
		break;
	}

	Wake(iNum);
}

void CVIBuffer_Particle::Clear_Force(_uint iNum, const FORCE_MODE& eMode)
{
	switch (eMode)
	{
	case FORCE_MODE::FORCE:
		::ZeroMemory(&m_vecParticleRigidbodyDesc[iNum].vAccel, sizeof(_float3));
		break;

	case FORCE_MODE::IMPULSE:
		::ZeroMemory(&m_vecParticleRigidbodyDesc[iNum].vVelocity, sizeof(_float3));
		break;

	case FORCE_MODE::ACCELERATION:
		::ZeroMemory(&m_vecParticleRigidbodyDesc[iNum].vAccel, sizeof(_float3));
		break;

	case FORCE_MODE::VELOCITY_CHANGE:
		::ZeroMemory(&m_vecParticleRigidbodyDesc[iNum].vVelocity, sizeof(_float3));
		break;

	default:
		break;
	}
}


void CVIBuffer_Particle::Clear_Power(_uint iNum)
{
	ZeroMemory(&m_vecParticleRigidbodyDesc[iNum].vAccel, sizeof(_float3));
	ZeroMemory(&m_vecParticleRigidbodyDesc[iNum].vVelocity, sizeof(_float3));
}


const _bool CVIBuffer_Particle::Check_Sleep(_uint iNum)
{
	if (m_vecParticleRigidbodyDesc[iNum].bSleep)
		return TRUE;

	//if (m_tBufferDesc.bUseGravity)
	//{
	//	if (m_tBufferDesc.bRecycle)
	//	{
	//		_float2 vVelocityXZ = { m_vecParticleRigidbodyDesc[iNum].vVelocity.x, m_vecParticleRigidbodyDesc[iNum].vVelocity.z };
	//		_float fLengthXZ = XMVectorGetX(XMVector2Length(XMLoadFloat2(&vVelocityXZ)));

	//		if (m_tBufferDesc.fSleepThreshold > fLengthXZ)
	//		{
	//			Sleep(iNum);
	//			return TRUE;
	//		}
	//	}

	//}
	//else
	{
		_float fLength = XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_vecParticleRigidbodyDesc[iNum].vVelocity)));

		if (m_tBufferDesc.fSleepThreshold > fLength)
		{
			Sleep(iNum);
			return TRUE;
		}
	}


	///* ���� �ӵ��� ũ�Ⱑ m_fSleepThreshold���� ������ ����(�������) */
	//_float fLength = XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_vecParticleRigidbodyDesc[iNum].vVelocity)));
	//if (!m_tBufferDesc.bUseGravity && m_tBufferDesc.fSleepThreshold > fLength)
	//{
	//	Sleep(iNum);
	//	return TRUE;
	//}

	return FALSE;
}


_bool CVIBuffer_Particle::Write_Json(json& Out_Json)
{
	Out_Json["Com_VIBuffer"]["iCurNumInstance"] = m_tBufferDesc.iCurNumInstance;


	Out_Json["Com_VIBuffer"]["bRecycle"] = m_tBufferDesc.bRecycle;
	Out_Json["Com_VIBuffer"]["bReverse"] = m_tBufferDesc.bReverse;
	Out_Json["Com_VIBuffer"]["eType_Action"] = m_tBufferDesc.eType_Action;
	Out_Json["Com_VIBuffer"]["eType_Fade"] = m_tBufferDesc.eType_Fade;
	
	/* LifeTime */
	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vMinMaxLifeTime"], m_tBufferDesc.vMinMaxLifeTime);

	/* RigidBody */
	Out_Json["Com_VIBuffer"]["bUseRigidBody"] = m_tBufferDesc.bUseRigidBody;
	Out_Json["Com_VIBuffer"]["bKinetic"] = m_tBufferDesc.bKinetic;
	Out_Json["Com_VIBuffer"]["bUseGravity"] = m_tBufferDesc.bUseGravity;
	Out_Json["Com_VIBuffer"]["eForce_Mode"] = m_tBufferDesc.eForce_Mode;

	Out_Json["Com_VIBuffer"]["fGravity"] = m_tBufferDesc.fGravity;
	Out_Json["Com_VIBuffer"]["fFriction"] = m_tBufferDesc.fFriction;
	Out_Json["Com_VIBuffer"]["fSleepThreshold"] = m_tBufferDesc.fSleepThreshold;
	Out_Json["Com_VIBuffer"]["byFreezeAxis"] = m_tBufferDesc.byFreezeAxis;

	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vMinMaxPower"], m_tBufferDesc.vMinMaxPower);
	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vMinMaxSpeed"], m_tBufferDesc.vMinMaxSpeed);
	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vMinMaxMass"], m_tBufferDesc.vMinMaxMass);

	/* For.Position */
	CJson_Utility::Write_Float3(Out_Json["Com_VIBuffer"]["vMinCenterOffsetPos"], m_tBufferDesc.vMinCenterOffsetPos);
	CJson_Utility::Write_Float3(Out_Json["Com_VIBuffer"]["vMaxCenterOffsetPos"], m_tBufferDesc.vMaxCenterOffsetPos);

	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vMinMaxRange"], m_tBufferDesc.vMinMaxRange);


	/* For.Rotation */
	CJson_Utility::Write_Float3(Out_Json["Com_VIBuffer"]["vRadian"], m_tBufferDesc.vRadian);

	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vMinMaxRotationOffsetX"], m_tBufferDesc.vMinMaxRotationOffsetX);
	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vMinMaxRotationOffsetY"], m_tBufferDesc.vMinMaxRotationOffsetY);
	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vMinMaxRotationOffsetZ"], m_tBufferDesc.vMinMaxRotationOffsetZ);


	/* For.Scale */
	Out_Json["Com_VIBuffer"]["bUseScaleLerp"] = m_tBufferDesc.bUseScaleLerp;
	Out_Json["Com_VIBuffer"]["eType_ScaleLerp"] = m_tBufferDesc.eType_ScaleLerp;
	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vScaleLerp_Up_Pos"], m_tBufferDesc.vScaleLerp_Up_Pos);
	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vScaleLerp_Down_Pos"], m_tBufferDesc.vScaleLerp_Down_Pos);

	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vMinMaxWidth"], m_tBufferDesc.vMinMaxWidth);
	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vMinMaxHeight"], m_tBufferDesc.vMinMaxHeight);



	/* For.Color */
	Out_Json["Com_VIBuffer"]["eType_ColorLerp"] = m_tBufferDesc.eType_ColorLerp;
	Out_Json["Com_VIBuffer"]["bDynamic_Color"] = m_tBufferDesc.bDynamic_Color;
	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vMinMaxRed"], m_tBufferDesc.vMinMaxRed);
	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vMinMaxGreen"], m_tBufferDesc.vMinMaxGreen);
	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vMinMaxBlue"], m_tBufferDesc.vMinMaxBlue);
	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vMinMaxAlpha"], m_tBufferDesc.vMinMaxAlpha);


	return true;
}

void CVIBuffer_Particle::Load_FromJson(const json& In_Json)
{
	m_tBufferDesc.iCurNumInstance = In_Json["Com_VIBuffer"]["iCurNumInstance"];


	m_tBufferDesc.bRecycle = In_Json["Com_VIBuffer"]["bRecycle"];
	m_tBufferDesc.bReverse = In_Json["Com_VIBuffer"]["bReverse"];
	m_tBufferDesc.eType_Action = In_Json["Com_VIBuffer"]["eType_Action"];
	m_tBufferDesc.eType_Fade = In_Json["Com_VIBuffer"]["eType_Fade"];


	/* LifeTime */
	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxLifeTime"], m_tBufferDesc.vMinMaxLifeTime);


	/* RigidBody */
	m_tBufferDesc.bUseRigidBody = In_Json["Com_VIBuffer"]["bUseRigidBody"];
	m_tBufferDesc.bKinetic = In_Json["Com_VIBuffer"]["bKinetic"];
	m_tBufferDesc.bUseGravity = In_Json["Com_VIBuffer"]["bUseGravity"];
	m_tBufferDesc.eForce_Mode = In_Json["Com_VIBuffer"]["eForce_Mode"];

	m_tBufferDesc.fGravity = In_Json["Com_VIBuffer"]["fGravity"];
	m_tBufferDesc.fFriction = In_Json["Com_VIBuffer"]["fFriction"];
	m_tBufferDesc.fSleepThreshold = In_Json["Com_VIBuffer"]["fSleepThreshold"];
	m_tBufferDesc.byFreezeAxis = In_Json["Com_VIBuffer"]["byFreezeAxis"];

	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxPower"], m_tBufferDesc.vMinMaxPower);
	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxSpeed"], m_tBufferDesc.vMinMaxSpeed);
	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxMass"], m_tBufferDesc.vMinMaxMass);

	/* For.Position */
	CJson_Utility::Load_Float3(In_Json["Com_VIBuffer"]["vMinCenterOffsetPos"], m_tBufferDesc.vMinCenterOffsetPos);
	CJson_Utility::Load_Float3(In_Json["Com_VIBuffer"]["vMaxCenterOffsetPos"], m_tBufferDesc.vMaxCenterOffsetPos);

	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxRange"], m_tBufferDesc.vMinMaxRange);


	/* For.Rotation */
	if (In_Json.contains("vRadian")) // �ٽ� ���� �� ����
		CJson_Utility::Load_Float3(In_Json["Com_VIBuffer"]["vRadian"], m_tBufferDesc.vRadian);

	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxRotationOffsetX"], m_tBufferDesc.vMinMaxRotationOffsetX);
	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxRotationOffsetY"], m_tBufferDesc.vMinMaxRotationOffsetY);
	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxRotationOffsetZ"], m_tBufferDesc.vMinMaxRotationOffsetZ);


	/* For.Scale */
	if (In_Json.contains("bUseScaleLerp")) // �ٽ� ���� �� ����
		m_tBufferDesc.bUseScaleLerp = In_Json["Com_VIBuffer"]["bUseScaleLerp"];

	m_tBufferDesc.eType_ScaleLerp = In_Json["Com_VIBuffer"]["eType_ScaleLerp"];

	if (In_Json.contains("vScaleLerp_Up_Pos")) // �ٽ� ���� �� ����
		CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vScaleLerp_Up_Pos"], m_tBufferDesc.vScaleLerp_Up_Pos);

	if (In_Json.contains("vScaleLerp_Down_Pos")) // �ٽ� ���� �� ����
		CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vScaleLerp_Down_Pos"], m_tBufferDesc.vScaleLerp_Down_Pos);

	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxWidth"], m_tBufferDesc.vMinMaxWidth);
	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxHeight"], m_tBufferDesc.vMinMaxHeight);


	/* For.Color */
	m_tBufferDesc.eType_ColorLerp = In_Json["Com_VIBuffer"]["eType_ColorLerp"];
	m_tBufferDesc.bDynamic_Color = In_Json["Com_VIBuffer"]["bDynamic_Color"];
	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxRed"], m_tBufferDesc.vMinMaxRed);
	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxGreen"], m_tBufferDesc.vMinMaxGreen);
	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxBlue"], m_tBufferDesc.vMinMaxBlue);
	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxAlpha"], m_tBufferDesc.vMinMaxAlpha);


	//// vTest�� ������ �ε��ϱ�
	//if (In_Json.contains("vTest"))
	//	m_tBufferDesc.vTest = In_Json["Com_VIBuffer"]["vTest"];

}

CVIBuffer_Particle* CVIBuffer_Particle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Particle* pInstance = new CVIBuffer_Particle(pDevice, pContext);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Particle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CVIBuffer_Particle::Clone(void* pArg)
{
	CVIBuffer_Particle* pInstance = new CVIBuffer_Particle(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Particle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Particle::Free()
{
	__super::Free();

	Safe_Release(m_pVBInstance);
}