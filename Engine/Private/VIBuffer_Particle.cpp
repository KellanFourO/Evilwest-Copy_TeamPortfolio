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
		

		// �׽�Ʈ�� ���� ��ġ�� �ʱ�ȭ
		XMStoreFloat4(&pVertices[i].vPosition, m_tBufferDesc.vCenterPosition);


		ReSet_Info(i);


		pVertices[i].vRight = _float4(1.f, 0.f, 0.f, 0.f)	/* * ũ�� */;
		pVertices[i].vUp	= _float4(0.f, 1.f, 0.f, 0.f)	/* * ũ�� */;
		pVertices[i].vLook	= _float4(0.f, 0.f, 1.f, 0.f)	/* * ũ�� */;



	} // �ݺ��� ��



	m_SubResourceData.pSysMem = pVertices;

	FAILED_CHECK(__super::Create_Buffer(&m_pVBInstance));

	Safe_Delete_Array(pVertices);

	return S_OK;

}


void CVIBuffer_Particle::ReSet()
{
	D3D11_MAPPED_SUBRESOURCE			SubResource = {};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXINSTANCE* pVertices = ((VTXINSTANCE*)SubResource.pData);

	m_iNumInstance = m_tBufferDesc.iCurNumInstance;
	for (_uint i = 0; i < m_iNumInstance; ++i)	// �ݺ��� ����
	{

		// �ʱ�ȭ
		XMStoreFloat4(&pVertices[i].vPosition, m_tBufferDesc.vCenterPosition);
		pVertices[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		pVertices[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		pVertices[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		//pVertices[i].vColor = _float4(m_tBufferDesc.vMinMaxRed.x, m_tBufferDesc.vMinMaxGreen.x, m_tBufferDesc.vMinMaxBlue.x, m_tBufferDesc.vMinMaxAlpha.x);


		ReSet_Info(i);

		if (!m_tBufferDesc.bBillBoard)
		{
			XMStoreFloat4(&pVertices[i].vRight, m_vecParticleRigidbodyDesc[i].vRight);
			XMStoreFloat4(&pVertices[i].vUp, m_vecParticleRigidbodyDesc[i].vUp);
			XMStoreFloat4(&pVertices[i].vLook, m_vecParticleRigidbodyDesc[i].vLook);
		}


	} // �ݺ��� ��



	m_pContext->Unmap(m_pVBInstance, 0);
}


void CVIBuffer_Particle::ReSet_Info(_uint iNum)
{

	// ������Ÿ��
	m_tBufferDesc.fTimeAcc = 0.f;
	m_vecParticleInfoDesc[iNum].fTimeAccs = 0.f;
	m_vecParticleInfoDesc[iNum].fLifeTime = SMath::fRandom(m_tBufferDesc.vMinMaxLifeTime.x, m_tBufferDesc.vMinMaxLifeTime.y);
	m_vecParticleInfoDesc[iNum].fLifeTimeRatio = 0.f;


#pragma region ������ٵ� ����.
	// ������ �ٵ� ����̸�
	if (m_tBufferDesc.bUseRigidBody)
	{
		Clear_Power(iNum);	// �Ŀ� ����
		m_vecParticleRigidbodyDesc[iNum].fMass = SMath::fRandom(m_tBufferDesc.vMinMaxMass.x, m_tBufferDesc.vMinMaxMass.y);	// ���� ����

		_vector		vDir = XMVectorSet(1.f, 0.f, 0.f, 0.f);
		vDir = XMVector3Normalize(vDir) * SMath::fRandom(m_tBufferDesc.vMinMaxRange.x, m_tBufferDesc.vMinMaxRange.y);

		_float3 vRotationOffset = { SMath::fRandom(m_tBufferDesc.vMinMaxRotationOffsetX.x, m_tBufferDesc.vMinMaxRotationOffsetX.y)
								  , SMath::fRandom(m_tBufferDesc.vMinMaxRotationOffsetY.x, m_tBufferDesc.vMinMaxRotationOffsetY.y)
								  , SMath::fRandom(m_tBufferDesc.vMinMaxRotationOffsetZ.x, m_tBufferDesc.vMinMaxRotationOffsetZ.y) };


		_vector		vRotation = XMQuaternionRotationRollPitchYaw(vRotationOffset.x, vRotationOffset.y, vRotationOffset.z);
		_matrix		RotationMatrix = XMMatrixRotationQuaternion(vRotation);


#pragma region ȸ�� ����
		vDir = XMVector3TransformNormal(vDir, RotationMatrix);	// ������ ���⺤�� ȸ�� ����
		m_vecParticleShaderInfoDesc[iNum].vDir = vDir;			// ���̴��� ������ ���� ����

		// TEST ���� : ī�޶� �ٶ󺸴� ������ �ݴ� ���Ϳ� ���⺤�� ������ ������ UVȸ�� ��Ű�� =========================================================================
		_vector vCamDirection = XMVector4Normalize(m_pGameInstance->Get_TransformMatrixInverse(CPipeLine::D3DTS_VIEW).r[2]);
		_float3 vCamDirectionFloat3 = {};
		XMStoreFloat3(&vCamDirectionFloat3, vCamDirection);
		vCamDirectionFloat3 = vCamDirectionFloat3 * -1.f;

		m_vecParticleShaderInfoDesc[iNum].fUV_RotDegree = SMath::Calculate_AngleBetweenVectors_Degree(vCamDirectionFloat3, m_vecParticleShaderInfoDesc[iNum].vDir);
		// TEST �� : ī�޶� �ٶ󺸴� ������ �ݴ� ���Ϳ� ���⺤�� ������ ������ UVȸ�� ��Ű�� ===========================================================================

		// ������⺤�͸� Look���� �� ���ο� Right, Up �����ֱ�
		_vector		vRight = _float4(1.f, 0.f, 0.f, 0.f)		/* * ũ�� */;
		_vector		vUp = _float4(0.f, 1.f, 0.f, 0.f)			/* * ũ�� */;
		_vector		vLook = XMVector4Normalize(vDir);

		vRight = XMVector4Normalize(XMVector3Cross(vUp, vDir));
		vUp = XMVector4Normalize(XMVector3Cross(vDir, vRight));

		XMStoreFloat4(&m_vecParticleRigidbodyDesc[iNum].vRight, vRight);
		XMStoreFloat4(&m_vecParticleRigidbodyDesc[iNum].vUp, vUp);
		XMStoreFloat4(&m_vecParticleRigidbodyDesc[iNum].vLook, vLook);

		//XMStoreFloat4(&m_vecParticleRigidbodyDesc[iNum].vRight, XMVector3TransformNormal(vRight, RotationMatrix));
		//XMStoreFloat4(&m_vecParticleRigidbodyDesc[iNum].vUp, XMVector3TransformNormal(vUp, RotationMatrix));
		//XMStoreFloat4(&m_vecParticleRigidbodyDesc[iNum].vLook, XMVector3TransformNormal(vLook, RotationMatrix));
#pragma endregion ȸ�� ��

		_vector vForce = vDir * SMath::fRandom(m_tBufferDesc.vMinMaxPower.x, m_tBufferDesc.vMinMaxPower.y);
		Add_Force(iNum, vForce, m_tBufferDesc.eForce_Mode);
	}
#pragma endregion ������ٵ� ��




}



void CVIBuffer_Particle::Update(_float fTimeDelta)
{
	if (0 >= m_iNumInstance)
		return;

	D3D11_MAPPED_SUBRESOURCE			SubResource = {};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXINSTANCE* pVertices = ((VTXINSTANCE*)SubResource.pData);

	for (_uint i = 0; i < m_iNumInstance; i++)	// �ݺ��� ����
	{
		// �ð� ����
		m_tBufferDesc.fTimeAcc += fTimeDelta;
		m_vecParticleInfoDesc[i].fTimeAccs += fTimeDelta;
		m_vecParticleInfoDesc[i].fLifeTimeRatio = min(1.0f, m_vecParticleInfoDesc[i].fTimeAccs / m_vecParticleInfoDesc[i].fLifeTime);
		

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
						Update_Kinetic(i, fTimeDelta);

						// Translate : vMovePos = vPos + Get_State(CTransform::STATE_POSITION);
						_vector vMovePos = (XMLoadFloat3(&m_vecParticleRigidbodyDesc[i].vVelocity) * fTimeDelta) + XMLoadFloat4(&pVertices[i].vPosition);
						XMVectorSetW(vMovePos, 1.f);

						// ���� ��ġ �̵�
						XMStoreFloat4(&pVertices[i].vPosition, vMovePos);
					}
					else
					{
						Update_Kinematic(i);
					}
				}
			}
		}
#pragma endregion �̵� : ������ٵ� ��


#pragma region �� ���� ����
		if (m_tBufferDesc.bDynamic_Color)	// ���ڸ��� �ٸ� �ֱ�� �� ����
		{
			m_vecParticleInfoDesc[i].vCurrentColors.x = abs(Easing::LerpToType(m_tBufferDesc.vMinMaxRed.x, m_tBufferDesc.vMinMaxRed.y, m_vecParticleInfoDesc[i].fTimeAccs, m_vecParticleInfoDesc[i].fLifeTime, m_tBufferDesc.eType_ColorLerp));
			m_vecParticleInfoDesc[i].vCurrentColors.y = abs(Easing::LerpToType(m_tBufferDesc.vMinMaxGreen.x, m_tBufferDesc.vMinMaxGreen.y, m_vecParticleInfoDesc[i].fTimeAccs, m_vecParticleInfoDesc[i].fLifeTime, m_tBufferDesc.eType_ColorLerp));
			m_vecParticleInfoDesc[i].vCurrentColors.z = abs(Easing::LerpToType(m_tBufferDesc.vMinMaxBlue.x, m_tBufferDesc.vMinMaxBlue.y, m_vecParticleInfoDesc[i].fTimeAccs, m_vecParticleInfoDesc[i].fLifeTime, m_tBufferDesc.eType_ColorLerp));
		
			pVertices[i].vColor = m_vecParticleInfoDesc[i].vCurrentColors;
		}
		else // �ϰ� �� ����
		{
			m_tBufferDesc.vCurrentColor.x = abs(Easing::LerpToType(m_tBufferDesc.vMinMaxRed.x, m_tBufferDesc.vMinMaxRed.y, m_tBufferDesc.fTimeAcc,     m_tBufferDesc.vMinMaxLifeTime.y, m_tBufferDesc.eType_ColorLerp));
			m_tBufferDesc.vCurrentColor.y = abs(Easing::LerpToType(m_tBufferDesc.vMinMaxGreen.x, m_tBufferDesc.vMinMaxGreen.y, m_tBufferDesc.fTimeAcc, m_tBufferDesc.vMinMaxLifeTime.y, m_tBufferDesc.eType_ColorLerp));
			m_tBufferDesc.vCurrentColor.z = abs(Easing::LerpToType(m_tBufferDesc.vMinMaxBlue.x, m_tBufferDesc.vMinMaxBlue.y, m_tBufferDesc.fTimeAcc,   m_tBufferDesc.vMinMaxLifeTime.y, m_tBufferDesc.eType_ColorLerp));

			pVertices[i].vColor = m_tBufferDesc.vCurrentColor;
		}

#pragma region �� ���� ��





	} // �ݺ��� ��

	m_pContext->Unmap(m_pVBInstance, 0);
}




_bool CVIBuffer_Particle::Write_Json(json& Out_Json)
{
	Out_Json["Com_VIBuffer"]["iCurNumInstance"] = m_tBufferDesc.iCurNumInstance;


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
	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vMinMaxMass"], m_tBufferDesc.vMinMaxMass);

	/* For.Position */
	CJson_Utility::Write_Float4(Out_Json["Com_VIBuffer"]["vCurrentPosition"], m_tBufferDesc.vCenterPosition);
	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vMinMaxRange"], m_tBufferDesc.vMinMaxRange);


	/* For.Rotation */
	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vMinMaxRotationOffsetX"], m_tBufferDesc.vMinMaxRotationOffsetX);
	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vMinMaxRotationOffsetY"], m_tBufferDesc.vMinMaxRotationOffsetY);
	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vMinMaxRotationOffsetZ"], m_tBufferDesc.vMinMaxRotationOffsetZ);


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
	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxMass"], m_tBufferDesc.vMinMaxMass);

	/* For.Position */
	CJson_Utility::Load_Float4(In_Json["Com_VIBuffer"]["vCurrentPosition"], m_tBufferDesc.vCenterPosition);
	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxRange"], m_tBufferDesc.vMinMaxRange);


	/* For.Rotation */
	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxRotationOffsetX"], m_tBufferDesc.vMinMaxRotationOffsetX);
	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxRotationOffsetY"], m_tBufferDesc.vMinMaxRotationOffsetY);
	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxRotationOffsetZ"], m_tBufferDesc.vMinMaxRotationOffsetZ);

	/* For.Color */
	m_tBufferDesc.eType_ColorLerp = In_Json["Com_VIBuffer"]["eType_ColorLerp"];
	m_tBufferDesc.bDynamic_Color = In_Json["Com_VIBuffer"]["bDynamic_Color"];
	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxRed"], m_tBufferDesc.vMinMaxRed);
	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxGreen"], m_tBufferDesc.vMinMaxGreen);
	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxBlue"], m_tBufferDesc.vMinMaxBlue);
	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxAlpha"], m_tBufferDesc.vMinMaxAlpha);


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

	/* ���� �ӵ��� ũ�Ⱑ m_fSleepThreshold���� ������ ����(�������) */
	_float fLength = XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_vecParticleRigidbodyDesc[iNum].vVelocity)));
	if (!m_tBufferDesc.bUseGravity && m_tBufferDesc.fSleepThreshold > fLength)
	{
		Sleep(iNum);
		return TRUE;
	}

	return FALSE;
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