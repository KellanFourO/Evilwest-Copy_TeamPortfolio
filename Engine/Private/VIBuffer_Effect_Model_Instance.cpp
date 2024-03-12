#include "VIBuffer_Effect_Model_Instance.h"
#include "Mesh.h"

#include "SMath.h"

CVIBuffer_Effect_Model_Instance::CVIBuffer_Effect_Model_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer_Model_Instance(pDevice,pContext)
{
}

CVIBuffer_Effect_Model_Instance::CVIBuffer_Effect_Model_Instance(const CVIBuffer_Effect_Model_Instance& rhs)
	: CVIBuffer_Model_Instance(rhs)
{
}

HRESULT CVIBuffer_Effect_Model_Instance::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer_Effect_Model_Instance::Initialize(void* pArg)
{
	m_tBufferDesc = *(EFFECT_MODEL_INSTANCE_DESC*)pArg;


	CModel* pModel[MORPH_END] = { nullptr };
	for (_uint i = 0; i < ECast(MORPH_END); ++i)
	{	
		if (nullptr != m_tBufferDesc.pModel[i])
		{
			Safe_AddRef(m_tBufferDesc.pModel[i]);
			pModel[i] = m_tBufferDesc.pModel[i];
		}
	}
	
	vector<CMesh*> Meshes[MORPH_END];
	for (_uint i = 0; i < ECast(MORPH_END); ++i)
	{
		if (nullptr != pModel[i])
		{
			Meshes[i] = pModel[i]->Get_Meshes();
			m_iNumMeshes = (_int)Meshes[i].size();

			for (_int j = 0; j < m_iNumMeshes; ++j)
			{
				m_vecInstanceMesh.push_back(Meshes[i][j]);
				Safe_AddRef(Meshes[i][j]);
			}

			m_iNumMaterials = pModel[i]->Get_NumMaterials();
		}
	}


	// ���� ���� ����(��ƼŬ����϶���)
	if (MODE_PARTICLE == m_tBufferDesc.eType_Mode)
	{
		m_vecParticleInfoDesc.reserve(m_tBufferDesc.iCurNumInstance);
		m_vecParticleShaderInfoDesc.reserve(m_tBufferDesc.iCurNumInstance);

	if (m_tBufferDesc.bUseRigidBody)
		m_vecParticleRigidbodyDesc.reserve(m_tBufferDesc.iCurNumInstance);
	}

	//
	Init_Instance(m_tBufferDesc.iCurNumInstance);


	return S_OK;
}


void CVIBuffer_Effect_Model_Instance::Init_Instance(_int iNumInstance)
{
	m_iInstanceStride = sizeof(VTXMODELINSTANCE);
	m_iNumInstance = iNumInstance;
	m_iNumVertexBuffers = 2;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iInstanceStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	VTXMODELINSTANCE* pModelInstance = new VTXMODELINSTANCE[m_iNumInstance];


	m_iNumInstance = m_tBufferDesc.iCurNumInstance;
	for (_uint i = 0; i < m_iNumInstance; ++i) // �ݺ��� ����
	{

		// ��ƼŬ ����϶���
		if (MODE_PARTICLE == m_tBufferDesc.eType_Mode)
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

			ReSet_ParticleInfo(i);
		}


		
		// ���� ��ġ�� ����
		//XMStoreFloat4(&pModelInstance[i].vTranslation, m_tBufferDesc.vCenterPosition);
		XMStoreFloat4(&pModelInstance[i].vTranslation, m_vecParticleInfoDesc[i].vCenterPositions);
		if (m_tBufferDesc.bRecycle)
		{
			// ���ۿ� �Ⱥ��̰�
			pModelInstance[i].vRight	= _float4(0.f, 0.f, 0.f, 0.f)	/* * ũ�� */;
			pModelInstance[i].vUp		= _float4(0.f, 0.f, 0.f, 0.f)		/* * ũ�� */;
			pModelInstance[i].vLook		= _float4(0.f, 0.f, 0.f, 0.f)	/* * ũ�� */;
		}
		else
		{
			pModelInstance[i].vRight = _float4(1.f, 0.f, 0.f, 0.f)	/* * ũ�� */;
			pModelInstance[i].vUp = _float4(0.f, 1.f, 0.f, 0.f)		/* * ũ�� */;
			pModelInstance[i].vLook = _float4(0.f, 0.f, 1.f, 0.f)	/* * ũ�� */;
		}


	} // �ݺ��� ��

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pModelInstance;

	Create_Buffer(&m_pVBInstance);

	Safe_Delete_Array(pModelInstance);

}

void CVIBuffer_Effect_Model_Instance::ReSet()
{
	D3D11_MAPPED_SUBRESOURCE			SubResource = {};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXMODELINSTANCE* pModelInstance = ((VTXMODELINSTANCE*)SubResource.pData);

	m_iNumInstance = m_tBufferDesc.iCurNumInstance;
	for (_uint i = 0; i < m_iNumInstance; i++)	// �ݺ��� ����
	{


		// ��ƼŬ ����϶���
		if (MODE_PARTICLE == m_tBufferDesc.eType_Mode)
		{
			ReSet_ParticleInfo(i);
		}

		

		// ���� ��ġ�� ����
		//XMStoreFloat4(&pModelInstance[i].vTranslation, m_tBufferDesc.vCenterPosition);
		XMStoreFloat4(&pModelInstance[i].vTranslation, m_vecParticleInfoDesc[i].vCenterPositions);
		if (m_tBufferDesc.bRecycle)
		{
			// ���ۿ� �Ⱥ��̰�
			pModelInstance[i].vRight = _float4(0.f, 0.f, 0.f, 0.f)	/* * ũ�� */;
			pModelInstance[i].vUp = _float4(0.f, 0.f, 0.f, 0.f)		/* * ũ�� */;
			pModelInstance[i].vLook = _float4(0.f, 0.f, 0.f, 0.f)	/* * ũ�� */;
		}
		else
		{
			pModelInstance[i].vRight = _float4(1.f, 0.f, 0.f, 0.f)	/* * ũ�� */;
			pModelInstance[i].vUp = _float4(0.f, 1.f, 0.f, 0.f)		/* * ũ�� */;
			pModelInstance[i].vLook = _float4(0.f, 0.f, 1.f, 0.f)	/* * ũ�� */;
		}



	} // �ݺ��� ��

	m_pContext->Unmap(m_pVBInstance, 0);
}



void CVIBuffer_Effect_Model_Instance::ReSet_ParticleInfo(_uint iNum)
{

	// ������Ÿ��
	m_vecParticleInfoDesc[iNum].fTimeAccs = 0.f;
	m_vecParticleInfoDesc[iNum].fLifeTime = SMath::fRandom(m_tBufferDesc.vMinMaxLifeTime.x, m_tBufferDesc.vMinMaxLifeTime.y);
	m_vecParticleInfoDesc[iNum].fLifeTimeRatios = 0.f;


	// ���� ������ Offset
	m_vecParticleInfoDesc[iNum].vCenterPositions.x = SMath::fRandom(m_tBufferDesc.vMinCenterOffsetPos.x, m_tBufferDesc.vMaxCenterOffsetPos.x);
	m_vecParticleInfoDesc[iNum].vCenterPositions.y = SMath::fRandom(m_tBufferDesc.vMinCenterOffsetPos.y, m_tBufferDesc.vMaxCenterOffsetPos.y);
	m_vecParticleInfoDesc[iNum].vCenterPositions.z = SMath::fRandom(m_tBufferDesc.vMinCenterOffsetPos.z, m_tBufferDesc.vMaxCenterOffsetPos.z);
	m_vecParticleInfoDesc[iNum].vCenterPositions.w = 1.f;


#pragma region �̵� : ������ٵ� ����
	// ������ �ٵ� ����̸�
	if (m_tBufferDesc.bUseRigidBody)
	{
		//if (m_tBufferDesc.bRecycle)
		//{
		//	m_vecParticleRigidbodyDesc[iNum].vVelocity.x = m_vecParticleInfoDesc[iNum].fLifeTime;
		//}
		//else
		{
			Clear_Power(iNum);	// �Ŀ� ����
		}

	
#pragma region �̵� ������� ȸ�� ����
		m_vecParticleInfoDesc[iNum].fRanges = SMath::fRandom(m_tBufferDesc.vMinMaxRange.x, m_tBufferDesc.vMinMaxRange.y);

		_vector		vDir = XMVectorSet(1.f, 0.f, 0.f, 0.f);
		vDir = XMVector3Normalize(vDir) * m_vecParticleInfoDesc[iNum].fRanges;

		_float3 vRotationOffset = { XMConvertToRadians(SMath::fRandom(m_tBufferDesc.vMinMaxRotationOffsetX.x, m_tBufferDesc.vMinMaxRotationOffsetX.y))
								  , XMConvertToRadians(SMath::fRandom(m_tBufferDesc.vMinMaxRotationOffsetY.x, m_tBufferDesc.vMinMaxRotationOffsetY.y))
								  , XMConvertToRadians(SMath::fRandom(m_tBufferDesc.vMinMaxRotationOffsetZ.x, m_tBufferDesc.vMinMaxRotationOffsetZ.y)) };


		_vector		vRotation		= XMQuaternionRotationRollPitchYaw(vRotationOffset.x, vRotationOffset.y, vRotationOffset.z);
		_matrix		RotationMatrix	= XMMatrixRotationQuaternion(vRotation);

		vDir = XMVector3TransformNormal(vDir, RotationMatrix);	// ������ ���⺤�� ȸ�� ����
		m_vecParticleShaderInfoDesc[iNum].vDir = vDir;			// ���̴��� ������ ���� ����
#pragma endregion �̵� ������� ȸ�� ��


		//if (m_tBufferDesc.bRecycle)
		//{

		//}
		//else
		//{
		//	// �̵� �������� �� �༭ �̵�
		//	_vector vForce = vDir * SMath::fRandom(m_tBufferDesc.vMinMaxPower.x, m_tBufferDesc.vMinMaxPower.y);
		//	Add_Force(iNum, vForce, m_tBufferDesc.eForce_Mode);
		//}


		// �̵� �������� �� �༭ �̵�
		_vector vForce = vDir * SMath::fRandom(m_tBufferDesc.vMinMaxPower.x, m_tBufferDesc.vMinMaxPower.y);
		Add_Force(iNum, vForce, m_tBufferDesc.eForce_Mode);



	}
#pragma endregion �̵� : ������ٵ� ��



}


void CVIBuffer_Effect_Model_Instance::Update(_float fTimeDelta)
{

	D3D11_MAPPED_SUBRESOURCE			SubResource = {};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXMODELINSTANCE* pModelInstance = ((VTXMODELINSTANCE*)SubResource.pData);

	for (_uint i = 0; i < m_iNumInstance; i++)	// �ݺ��� ����
	{


	}	// �ݺ��� ��

	m_pContext->Unmap(m_pVBInstance, 0);

}


void CVIBuffer_Effect_Model_Instance::Update_Particle(_float fTimeDelta)
{

	// ��ƼŬ ����϶���
	if (MODE_PARTICLE == m_tBufferDesc.eType_Mode)
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



#pragma region �� �ٲ㳢�� ����
		// ������ True�̸� 
		if (m_tBufferDesc.bMorph)
		{
			m_tBufferDesc.fMorphTimeAcc += fTimeDelta;

			_int iNum = ECast(m_tBufferDesc.eCurModelNum);
			if (m_tBufferDesc.fMorphTimeAcc >= m_tBufferDesc.fMorphTimeTerm)
			{
				iNum += 1;
				m_tBufferDesc.eCurModelNum = (MODEL_MORPH)iNum;

				if (m_tBufferDesc.eCurModelNum >= MORPH_END)
				{
					m_tBufferDesc.eCurModelNum = MORPH_01;
				}

				m_tBufferDesc.fMorphTimeAcc = 0.f;
			}
		}
#pragma region �� �ٲ㳢�� ��


#pragma region Map UnMap �� ���� üũ ��

		D3D11_MAPPED_SUBRESOURCE			SubResource = {};

		m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

		VTXMODELINSTANCE* pModelInstance = ((VTXMODELINSTANCE*)SubResource.pData);

#ifdef _DEBUG
		m_iNumInstance = m_tBufferDesc.iCurNumInstance;
#endif // _DEBUG
		for (_uint i = 0; i < m_iNumInstance; i++)	// �ݺ��� ����
		{
#pragma region ���ڵ� �ð� ����
			// ���ڵ��� �ð� ������ (���ڵ���)������Ÿ�Ӻ��� Ŀ���� �ð� ���� ���� & ���� �ݺ�����
			if (m_vecParticleInfoDesc[i].fTimeAccs > m_vecParticleInfoDesc[i].fLifeTime)
			{
				m_vecParticleInfoDesc[i].fTimeAccs = m_vecParticleInfoDesc[i].fLifeTime;
				m_vecParticleInfoDesc[i].fLifeTimeRatios = 1.f;
				//continue;
			}
			else
			{
				// �ð� ����(����)
				m_vecParticleInfoDesc[i].fTimeAccs += fTimeDelta;
				m_vecParticleInfoDesc[i].fLifeTimeRatios = min(1.0f, m_vecParticleInfoDesc[i].fTimeAccs / m_vecParticleInfoDesc[i].fLifeTime);
			}

#pragma region ���ڵ� �ð� ��



#pragma region �̵� : ������ٵ� ����
			// ������ �ٵ� ����̸�
			if (m_tBufferDesc.bUseRigidBody)
			{
				if (!m_vecParticleRigidbodyDesc.empty())
				{
					if (!Check_Sleep(i))	// ������ �ƴϸ� ������ٵ� ������Ʈ
					{
						if (m_tBufferDesc.bKinetic)
						{
							Update_Kinetic(i, fTimeDelta);	// �̵� �ӷ� ��� ������Ʈ

							// Translate : vMovePos = vPos + Get_State(CTransform::STATE_POSITION);
							_vector vMovePos = (XMLoadFloat3(&m_vecParticleRigidbodyDesc[i].vVelocity) * fTimeDelta) + XMLoadFloat4(&pModelInstance[i].vTranslation);
							XMVectorSetW(vMovePos, 1.f);

							// ���� ��ġ �̵�
							XMStoreFloat4(&pModelInstance[i].vTranslation, vMovePos);

							//m_vecParticleShaderInfoDesc[i].vDir = m_vecParticleRigidbodyDesc[i].vVelocity;
						}
						else
						{
							Update_Kinematic(i);
						}
					}
					else
					{
						if (m_tBufferDesc.bRecycle)
						{
							// ������Ŭ ���� ������ ���� �� �ʱ�ȭ �� �� �ٽ��ֱ�
							XMStoreFloat4(&pModelInstance[i].vTranslation, m_vecParticleInfoDesc[i].vCenterPositions);
							pModelInstance[i].vRight = _float4(1.f, 0.f, 0.f, 0.f)	/* * ũ�� */;
							pModelInstance[i].vUp = _float4(0.f, 1.f, 0.f, 0.f)		/* * ũ�� */;
							pModelInstance[i].vLook = _float4(0.f, 0.f, 1.f, 0.f)	/* * ũ�� */;
							_vector vForce = XMVector3Normalize(m_vecParticleShaderInfoDesc[i].vDir) * SMath::fRandom(m_tBufferDesc.vMinMaxPower.x, m_tBufferDesc.vMinMaxPower.y);
							Add_Force(i, vForce, m_tBufferDesc.eForce_Mode);
						}

					}

				}
			}
			else // ������ �ٵ� ����� �ƴϸ�
			{
				// ���� ��ġ�� ����
				XMStoreFloat4(&pModelInstance[i].vTranslation, m_vecParticleInfoDesc[i].vCenterPositions);
			}






#pragma endregion �̵� : ������ٵ� ��


		}	// �ݺ��� ��

		m_pContext->Unmap(m_pVBInstance, 0);
	}
}


HRESULT CVIBuffer_Effect_Model_Instance::Bind_VIBuffers(_uint iMeshContainerIndex)
{

	__super::Bind_VIBuffers(iMeshContainerIndex);

	return S_OK;
}


HRESULT CVIBuffer_Effect_Model_Instance::Render(_int iMeshIndex)
{

	if (m_tBufferDesc.bMorph)	// ������ True�̸� (���� ��)
	{
		Bind_VIBuffers(m_tBufferDesc.eCurModelNum);
		m_pContext->DrawIndexedInstanced(m_vecInstanceMesh[m_tBufferDesc.eCurModelNum]->Get_NumIndices(), m_iNumInstance, 0, 0, 0);

		return S_OK;
	}
	else
	{
		Bind_VIBuffers(iMeshIndex);
		m_pContext->DrawIndexedInstanced(m_vecInstanceMesh[iMeshIndex]->Get_NumIndices(), m_iNumInstance, 0, 0, 0);
	}


	return S_OK;
}


_bool CVIBuffer_Effect_Model_Instance::Write_Json(json& Out_Json)
{
	/* �ν��Ͻ� ���� */
	Out_Json["Com_VIBuffer"]["iCurNumInstance"] = m_tBufferDesc.iCurNumInstance;	

	/* States */
	Out_Json["Com_VIBuffer"]["eType_Mode"] = m_tBufferDesc.eType_Mode;
	Out_Json["Com_VIBuffer"]["eType_Action"] = m_tBufferDesc.eType_Action;
	Out_Json["Com_VIBuffer"]["bRecycle"] = m_tBufferDesc.bRecycle;


	/* LifeTime */
	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vMinMaxLifeTime"], m_tBufferDesc.vMinMaxLifeTime);


	/* Morph */
	Out_Json["Com_VIBuffer"]["bMorph"] = m_tBufferDesc.bMorph;
	Out_Json["Com_VIBuffer"]["fMorphTimeTerm"] = m_tBufferDesc.fMorphTimeTerm;


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


	/* For.Position */
	//CJson_Utility::Write_Float4(Out_Json["Com_VIBuffer"]["vCurrentPosition"], m_tBufferDesc.vCenterPosition);
	CJson_Utility::Write_Float3(Out_Json["Com_VIBuffer"]["vMinCenterOffsetPos"], m_tBufferDesc.vMinCenterOffsetPos);
	CJson_Utility::Write_Float3(Out_Json["Com_VIBuffer"]["vMaxCenterOffsetPos"], m_tBufferDesc.vMaxCenterOffsetPos);
	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vMinMaxRange"], m_tBufferDesc.vMinMaxRange);


	/* For.Rotation */
	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vMinMaxRotationOffsetX"], m_tBufferDesc.vMinMaxRotationOffsetX);
	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vMinMaxRotationOffsetY"], m_tBufferDesc.vMinMaxRotationOffsetY);
	CJson_Utility::Write_Float2(Out_Json["Com_VIBuffer"]["vMinMaxRotationOffsetZ"], m_tBufferDesc.vMinMaxRotationOffsetZ);



	return true;
}

void CVIBuffer_Effect_Model_Instance::Load_FromJson(const json& In_Json)
{
	/* �ν��Ͻ� ���� */
	m_tBufferDesc.iCurNumInstance = In_Json["Com_VIBuffer"]["iCurNumInstance"];

	/* States */
	m_tBufferDesc.eType_Mode = In_Json["Com_VIBuffer"]["eType_Mode"];
	if(In_Json.contains("eType_Action"))
		m_tBufferDesc.eType_Mode = In_Json["Com_VIBuffer"]["eType_Action"];

	if (In_Json.contains("bRecycle"))
		m_tBufferDesc.eType_Mode = In_Json["Com_VIBuffer"]["bRecycle"];


	/* LifeTime */
	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxLifeTime"], m_tBufferDesc.vMinMaxLifeTime);


	/* Morph */
	m_tBufferDesc.bMorph = In_Json["Com_VIBuffer"]["bMorph"];
	m_tBufferDesc.fMorphTimeTerm = In_Json["Com_VIBuffer"]["fMorphTimeTerm"];


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


	/* For.Position */
	//CJson_Utility::Load_Float4(In_Json["Com_VIBuffer"]["vCurrentPosition"], m_tBufferDesc.vCenterPosition);
	CJson_Utility::Load_Float3(In_Json["Com_VIBuffer"]["vMinCenterOffsetPos"], m_tBufferDesc.vMinCenterOffsetPos);
	CJson_Utility::Load_Float3(In_Json["Com_VIBuffer"]["vMaxCenterOffsetPos"], m_tBufferDesc.vMaxCenterOffsetPos);


	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxRange"], m_tBufferDesc.vMinMaxRange);


	/* For.Rotation */
	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxRotationOffsetX"], m_tBufferDesc.vMinMaxRotationOffsetX);
	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxRotationOffsetY"], m_tBufferDesc.vMinMaxRotationOffsetY);
	CJson_Utility::Load_Float2(In_Json["Com_VIBuffer"]["vMinMaxRotationOffsetZ"], m_tBufferDesc.vMinMaxRotationOffsetZ);


}


_float3 CVIBuffer_Effect_Model_Instance::Update_Kinetic(_uint iNum, _float fTimeDelta)
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

void CVIBuffer_Effect_Model_Instance::Update_Kinematic(_uint iNum)
{
	/* ���� ����, ���� �ӵ� ��ȭ �ο� (Ű�׸�ƽ, �� ������ٵ� ������) */
	Clear_Force(iNum, FORCE_MODE::VELOCITY_CHANGE);
}


void CVIBuffer_Effect_Model_Instance::Add_Force(_uint iNum, _fvector vForce, FORCE_MODE eMode)
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

void CVIBuffer_Effect_Model_Instance::Clear_Force(_uint iNum, const FORCE_MODE& eMode)
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


void CVIBuffer_Effect_Model_Instance::Clear_Power(_uint iNum)
{
	ZeroMemory(&m_vecParticleRigidbodyDesc[iNum].vAccel, sizeof(_float3));
	ZeroMemory(&m_vecParticleRigidbodyDesc[iNum].vVelocity, sizeof(_float3));
}


const _bool CVIBuffer_Effect_Model_Instance::Check_Sleep(_uint iNum)
{
	if (m_vecParticleRigidbodyDesc[iNum].bSleep)
		return TRUE;

	if (m_tBufferDesc.bUseGravity)
	{
		if (m_tBufferDesc.bRecycle)
		{
			_float2 vVelocityXZ = { m_vecParticleRigidbodyDesc[iNum].vVelocity.x, m_vecParticleRigidbodyDesc[iNum].vVelocity.z };
			_float fLengthXZ = XMVectorGetX(XMVector2Length(XMLoadFloat2(&vVelocityXZ)));

			if (m_tBufferDesc.fSleepThreshold > fLengthXZ)
			{
				Sleep(iNum);
				return TRUE;
			}
		}

	}
	else
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

CVIBuffer_Effect_Model_Instance* CVIBuffer_Effect_Model_Instance::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Effect_Model_Instance* pInstance = new CVIBuffer_Effect_Model_Instance(pDevice, pContext);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Effect_Model_Instance");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CVIBuffer_Effect_Model_Instance::Clone(void* pArg)
{
	CVIBuffer_Effect_Model_Instance* pInstance = new CVIBuffer_Effect_Model_Instance(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Effect_Model_Instance");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Effect_Model_Instance::Free()
{
	__super::Free();

}