#include "VIBuffer_Field.h"
#include "GameInstance.h"


CVIBuffer_Field::CVIBuffer_Field(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice,pContext)
{
}

CVIBuffer_Field::CVIBuffer_Field(const CVIBuffer_Field& rhs)
	: CVIBuffer(rhs)
{
}

_bool CVIBuffer_Field::Get_Vertex(_uint _iIndex, VTXFIELD* _pOut)
{
	if (m_VertexInfo.size() <= _iIndex)
		return false;

	*_pOut = m_VertexInfo[_iIndex];

	return true;
}

_bool CVIBuffer_Field::Get_Indices(_uint _iIndex, _uint3* _pOut)
{
	if (m_Indices.size() <= _iIndex)
		return false;

	*_pOut = m_Indices[_iIndex];

	return true;
}

_bool CVIBuffer_Field::Compute_MousePos(RAY _Ray, _matrix _WorldMatrix, _float3* pOut)
{
	_matrix matWorld = XMMatrixInverse(nullptr, _WorldMatrix);
	_vector vRayPos, vRayDir;

	vRayPos = XMVector3TransformCoord(XMLoadFloat4(&_Ray.vPosition), matWorld);
	vRayDir = XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&_Ray.vDirection), matWorld));

	_float fDist;

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		_uint3 iIndices = m_Indices[i];
		_vector vPickedPos;

		_vector vPosX = XMLoadFloat3(&m_VertexInfo[iIndices.ix].vPosition);
		_vector vPosY = XMLoadFloat3(&m_VertexInfo[iIndices.iy].vPosition);
		_vector vPosZ = XMLoadFloat3(&m_VertexInfo[iIndices.iz].vPosition);

		if (true == DirectX::TriangleTests::Intersects(vRayPos, vRayDir, vPosX, vPosY, vPosZ, fDist))
		{
			vPickedPos = vRayPos + XMVector3Normalize(vRayDir) * fDist;
			XMStoreFloat3(pOut, vPickedPos);

			return true;
		}
	}
	
	return false;
}


HRESULT CVIBuffer_Field::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer_Field::Initialize(void* pArg)
{
	VTXFIELD* pInfo = (VTXFIELD*)pArg;
	DINFO* pDinfo = (DINFO*)pArg;
	_bool bTest = false;

	if (!pDinfo->vecVertexInfo.empty())
	{
		bTest = true;
	}

	m_fInterval = 10.f;
	m_iNumVerticesX = (_uint)pInfo->vPosition.x;
	m_iNumVerticesZ = (_uint)pInfo->vPosition.z;

	m_iStride = sizeof(VTXFIELD);
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
	m_iNumVertexBuffers = 1;

	
	//!  ���ؽ� ���� ����
	VTXFIELD* pVertices = new VTXFIELD[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXFIELD) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (size_t j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint iIndex = _uint(i * m_iNumVerticesX + j);

			if (bTest)
			{
					pVertices[iIndex].vPosition = pDinfo->vecVertexInfo[iIndex].vPosition;
					pVertices[iIndex].vTexcoord = pDinfo->vecVertexInfo[iIndex].vTexcoord;
					pVertices[iIndex].vNormal = pDinfo->vecVertexInfo[iIndex].vNormal;
					pVertices[iIndex].vTangent = pDinfo->vecVertexInfo[iIndex].vTangent;
			}
			else 
			{
				pVertices[iIndex].vPosition = _float3(_float(j * m_fInterval), 0.f, _float(i * m_fInterval));
				pVertices[iIndex].vTexcoord = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
				pVertices[iIndex].vNormal = _float3(0.f, 1.f, 0.f);
				pVertices[iIndex].vTangent = _float3(1.f, 0.f, 0.f);
			}
			

			m_VertexInfo.push_back(pVertices[iIndex]);
		}
	}
	//!���ؽ� ���� ����

	//! �ε��� ���� ����
	m_iIndexStride = sizeof(FACEINDICES32);
	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_iNumIndices = m_iNumPrimitive * 3;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	_ulong  iNumIndices = 0;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_ulong iIndex = i * m_iNumVerticesX + j;

			_ulong iIndices[] =
			{
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[iNumIndices] = { iIndices[0], iIndices[1], iIndices[2] };
			m_Indices.push_back(_uint3(pIndices[iNumIndices]._1, pIndices[iNumIndices]._2, pIndices[iNumIndices]._3));
			++iNumIndices;

			pIndices[iNumIndices] = { iIndices[0], iIndices[2], iIndices[3] };
			m_Indices.push_back(_uint3(pIndices[iNumIndices]._1, pIndices[iNumIndices]._2, pIndices[iNumIndices]._3));
			++iNumIndices;
		}
	}
	//! �ε��� ���� ����


	//! ���� ���� ����
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(Create_Buffer(&m_pVB)))
		return E_FAIL;
	//!���� ���� ����

	//!�ε��� ���� ����
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(Create_Buffer(&m_pIB)))
		return E_FAIL;
	//! �ε��� ���� ����

	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pIndices);

	return S_OK;
}

void CVIBuffer_Field::Update(_vector _vMousePos, _float _fRadious, _float _fPower, _uint _iMode)
{
	D3D11_MAPPED_SUBRESOURCE SubResource;

	_fRadious *= m_fInterval;
	//TODO MAP
	//! ���� ���ҽ��� ���Ե� �����Ϳ� ���� �����͸� ��������, GPU �׼����� �ź��Ѵ�.
	
	//!�������̽� ������ : �ٲٷ��� �༮�� �ּ�(2D�ؽ�ó,���� ��� ID3D11Resource ��ӹ޴� ��ü �ּ�)
	//! ���� ���ҽ� �ε��� ��ȣ : ����, �Ӹ��� 	//! CPU ���� : DISCARD �Ǵ� NO_OVERWRTE������ �϶��� � �ӷ����� �ٲ� ����.

	//! DISCARD : ������ ������ ����ä���
	//! NO_OVERWRITE :  ������ �����ϰ�, �߰������� �� ä���
	//! GPU�� ������� �� �����ϴ� �۾� ���� : 0 ����^
	//! ��� �� : D3D11_MAPPED_SUBRESOURCE. MAP�� ���� ���� ���� ������� ����ü ������

	m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	_int2	iPickIndex = { _int(_vMousePos.m128_f32[0] / m_fInterval), _int(_vMousePos.m128_f32[2] / m_fInterval) };
	_int	iRoundIndx = (_int)(_fRadious / m_fInterval);

	_int2	iBeginIndex, iEndIndex;

	iBeginIndex.x = (0 > iPickIndex.x - iRoundIndx) ? (0) : (iPickIndex.x - iRoundIndx);
	iBeginIndex.y = (0 > iPickIndex.y - iRoundIndx) ? (0) : (iPickIndex.y - iRoundIndx);

	iEndIndex.x = ((_int)m_iNumVerticesX < iPickIndex.x + iRoundIndx) ? (m_iNumVerticesX) : (iPickIndex.x + iRoundIndx);
	iEndIndex.y = ((_int)m_iNumVerticesZ < iPickIndex.y + iRoundIndx) ? (m_iNumVerticesZ) : (iPickIndex.y + iRoundIndx);

	for (_uint iZ(iBeginIndex.y); iZ < (_uint)iEndIndex.y; ++iZ)
	{
		for (_uint iX(iBeginIndex.x); iX < (_uint)iEndIndex.x; ++iX)
		{
			_ulong	iIndex = iZ * m_iNumVerticesX + iX;

			
			_float3 vPos = ((VTXFIELD*)SubResource.pData)[iIndex].vPosition;
			_float  fLength = XMVectorGetX(XMVector3Length(XMLoadFloat3(&vPos) - _vMousePos));

			switch (_iMode)
			{
			case 0:
			{
				if (_fRadious < fLength)
					continue;

				((VTXFIELD*)SubResource.pData)[iIndex].vPosition.y += _fPower;
				m_VertexInfo[iIndex].vPosition = ((VTXFIELD*)SubResource.pData)[iIndex].vPosition;
			}
			break;

			case 1:
			{
				if (_fRadious < fLength)
					continue;

				_float fLerpPower = _fPower * (1.f - pow((fLength / _fRadious), 2.f));

				((VTXFIELD*)SubResource.pData)[iIndex].vPosition.y += fLerpPower;
				m_VertexInfo[iIndex].vPosition = ((VTXFIELD*)SubResource.pData)[iIndex].vPosition;
			}
			break;

			case 2:
			{
				((VTXFIELD*)SubResource.pData)[iIndex].vPosition.y = _fPower;
				m_VertexInfo[iIndex].vPosition = ((VTXFIELD*)SubResource.pData)[iIndex].vPosition;
			}
			break;
			}
		}
	}

	for (_uint iZ(iBeginIndex.y); iZ < (_uint)iEndIndex.y; ++iZ)
	{
		for (_uint iX(iBeginIndex.x); iX < (_uint)iEndIndex.x; ++iX)
		{
			_ulong	iIndex = iZ * m_iNumVerticesX + iX;

			_long  iAdjacency[] =
			{
				_long(iIndex + m_iNumVerticesX),	// ��
				_long(iIndex + 1),					// ������
				_long(iIndex - m_iNumVerticesX),	// �Ʒ�
				_long(iIndex - 1)					// ����
			};

			if (0 == iX)
				iAdjacency[3] = -1;

			if (m_iNumVerticesX - 1 == iX)
				iAdjacency[1] = -1;

			if (0 == iZ)
				iAdjacency[2] = -1;

			if (m_iNumVerticesZ - 1 == iZ)
				iAdjacency[0] = -1;

			_float3 vNorm = m_VertexInfo[iIndex].vNormal;

			// �븻 ���� ���
			_vector vComputeNorm = XMVectorSet(0.f, 0.f, 0.f, 0.f);

			for (_uint i = 0; i < 4; ++i)
			{
				_uint iNext = (3 == i) ? (0) : (i + 1);

				if (0 > iAdjacency[i] || 0 > iAdjacency[iNext])
					continue;

				_vector vLine_no1 = XMLoadFloat3(&m_VertexInfo[iAdjacency[i]].vPosition) - XMLoadFloat3(&m_VertexInfo[iIndex].vPosition);
				_vector vLine_no2 = XMLoadFloat3(&m_VertexInfo[iAdjacency[iNext]].vPosition) - XMLoadFloat3(&m_VertexInfo[iIndex].vPosition);
				_vector vLingNorm = XMVector3Normalize(XMVector3Cross(vLine_no1, vLine_no2));

				vComputeNorm = XMVector3Normalize(vComputeNorm + vLingNorm);
			}

			XMStoreFloat3(&vNorm, vComputeNorm);

			m_VertexInfo[iIndex].vNormal = vNorm;
			((VTXFIELD*)SubResource.pData)[iIndex].vNormal = vNorm;

			if (0 > iAdjacency[1])
				continue;

			
			// ź��Ʈ ���� ���
			_float3 vTempTangent;
			XMStoreFloat3(&vTempTangent, XMVector3Normalize(XMLoadFloat3(&m_VertexInfo[iAdjacency[1]].vPosition) - XMLoadFloat3(&m_VertexInfo[iIndex].vPosition)));

			m_VertexInfo[iIndex].vTangent = vTempTangent;
			((VTXFIELD*)SubResource.pData)[iIndex].vTangent = vTempTangent;
		}
	}

	m_pContext->Unmap(m_pVB, 0);

	
}


CVIBuffer_Field* CVIBuffer_Field::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Field* pInstance = new CVIBuffer_Field(pDevice, pContext);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Field");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CVIBuffer_Field::Clone(void* pArg)
{
	CVIBuffer_Field* pInstance = new CVIBuffer_Field(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Field");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Field::Free()
{
	__super::Free();
}
