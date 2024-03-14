#include "..\Public\Cell.h"
#include "Shader.h"
#include "VIBuffer_Cell.h"

CCell::CCell(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)

{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

_int CCell::Get_NeighborIndex(LINE eLine)
{
	if (eLine < 0 || eLine >= LINE_END)
		return -1;

	return m_iNeighbors[eLine];
}

void CCell::Set_Point(POINT ePoint, _float3 vPoint)
{
	m_vPoints[ePoint] = vPoint;
#ifdef _DEBUG
	m_pVIBuffer->Update(&m_vPoints[ePoint]);
#endif // _DEBUG
}

void CCell::Set_PointY(POINT ePoint, _float fY)
{
	m_vPoints[ePoint].y = fY;
	#ifdef _DEBUG
m_pVIBuffer->Update(&m_vPoints[ePoint]);
#endif // _DEBUG
}

_float CCell::Get_Height(const _float3& vPosition)
{
	_vector vPlane = {};

	// CCell�� �� ���� �̿��Ͽ� ����� ����
	vPlane = XMPlaneFromPoints(XMLoadFloat3(&m_vPoints[POINT_A]),
		XMLoadFloat3(&m_vPoints[POINT_B]),
		XMLoadFloat3(&m_vPoints[POINT_C]));

	_float fA = XMVectorGetX(vPlane);
	_float fB = XMVectorGetY(vPlane);
	_float fC = XMVectorGetZ(vPlane);
	_float fD = XMVectorGetW(vPlane);

	// ���� ��ġ�� ��ǥ
	_float fX = vPosition.x;
	_float fY = vPosition.y;
	_float fZ = vPosition.z;

	// ��� �������� �̿��Ͽ� ���� ���
	_float height = (-fA * fX - fC * fZ - fD) / fB;

	return height;
}

_float3 CCell::Get_Compare_Point(const _float3* pPoint)
{
	_float3 vPoints[POINT_END] = { m_vPoints[POINT_A], m_vPoints[POINT_B], m_vPoints[POINT_C] };

	for (_int i = 0; i < POINT_END; ++i)
	{
		vPoints[i].y = 0.f;
	}

	_float3 vSourPoint = *pPoint;

	vSourPoint.y = 0.f;

	_float3 vPointBool = { 0.f, 0.f, 0.f };

	if (true == XMVector3Equal(XMLoadFloat3(&vPoints[POINT_A]), XMLoadFloat3(&vSourPoint)))
	{
		vPointBool.x = 1.f;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&vPoints[POINT_B]), XMLoadFloat3(&vSourPoint)))
	{
		vPointBool.y = 1.f;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&vPoints[POINT_C]), XMLoadFloat3(&vSourPoint)))
	{
		vPointBool.z = 1.f;
	}

	return vPointBool;
}

_bool CCell::Is_Out(_fvector vWorldPos, _fvector vLook, _fmatrix WorldMatrix, _int* pNeighborIndex, _float4* pSliding)
{
	/* ���缿�� ������ ������ �Լ� */
	for (size_t i = 0; i < LINE_END; i++)
	{
		_vector      vSour = XMVector3Normalize(XMVectorSetW(vWorldPos - XMLoadFloat3(&m_vPoints[i]), 0.f)); /* ������������ ������ ���������� ���� */
		_vector      vDest = XMVector3Normalize(XMLoadFloat3(&m_vNormals[i])); /* ������ ����Ű�� �븻�� */

		_float fRadian = XMVectorGetX(XMVector3Dot(vSour, vDest)); /* Src�� Dst������ ���� */
		if (0.f <= fRadian)
		{
			/* �ش��ϴ� ������ �̿����� ã�Ҵ�. - �̿����� �ε����� ������� */
			*pNeighborIndex = m_iNeighbors[i];

			if (nullptr != pSliding)
			{
				_vector vSliding = {};
				/* ������������ �ش������ ��ֺ��Ϳ� �����̸鼭 �÷��̾ �̵��ϴ� ���⿡ ���� ������ ���� ����
				 * ������ ���������ϰ��, V - (V*N)N ... * = ���� */
				vSliding = XMVector3Normalize(XMVectorSet(m_vNormals[i].z, 0.f, m_vNormals[i].x * -1.f, 0.f));

				/* 0~ pi/2 �� ��ȣ�� �ݴ�� �ϴ� �ݴ�� ���ϰ� �������� -1 ���ϱ� */
				if (XMVectorGetX(XMVector3Dot(vSliding, vLook)) >= XMConvertToRadians(0.f) && XMVectorGetX(XMVector3Dot(vSliding, vLook)) <= XMConvertToRadians(90.f))
					XMStoreFloat4(pSliding, vSliding);
				else
					XMStoreFloat4(pSliding, -1.f * vSliding);
			}
			return true;
		}
	}

	return false;

}

HRESULT CCell::Initialize(const _float3 * pPoints, _uint iIndex)
{
	memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

	m_iIndex = iIndex;

	_vector		vLine = XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]);
	XMStoreFloat3(&m_vNormals[LINE_AB], XMVectorSet(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine), 0.f));

	vLine = XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]);
	XMStoreFloat3(&m_vNormals[LINE_BC], XMVectorSet(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine), 0.f));

	vLine = XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]);
	XMStoreFloat3(&m_vNormals[LINE_CA], XMVectorSet(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine), 0.f));

#ifdef _DEBUG


	m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, pPoints);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;
#endif

	return S_OK;
}

_bool CCell::Compare_Points(const _float3 * pSourPoint, const _float3 * pDestPoint)
{

	_float3 vPoints[POINT_END] = { m_vPoints[POINT_A], m_vPoints[POINT_B], m_vPoints[POINT_C] };

	for (_int i = 0; i < POINT_END; ++i)
	{
		vPoints[i].y = 0.f;
	}

	_float3 vSourPoint = *pSourPoint;
	_float3 vDestPoint = *pDestPoint;

	vSourPoint.y = 0.f;
	vDestPoint.y = 0.f;


	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(pSourPoint)))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(pDestPoint)))
			return true;

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(pDestPoint)))
			return true;		
	}
	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(pSourPoint)))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(pDestPoint)))
			return true;

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(pDestPoint)))
			return true;
	}
	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(pSourPoint)))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(pDestPoint)))
			return true;

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(pDestPoint)))
			return true;
	}

	return false;	
}

_bool CCell::isIn(_fvector vPosition, _fmatrix WorldMatrix, _int* pNeighborIndex)
{
	for (size_t i = 0; i < LINE_END; i++)
	{
		_vector	vStartPoint = XMVector3TransformCoord(XMLoadFloat3(&m_vPoints[i]), WorldMatrix);
		_vector	vNormal = XMVector3TransformNormal(XMLoadFloat3(&m_vNormals[i]), WorldMatrix);

		_vector	vSourDir = vPosition - vStartPoint;

		if (0 < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vSourDir),
			XMVector3Normalize(vNormal))))
		{
			*pNeighborIndex = m_iNeighbors[i];
			return false;
		}
	}

	return true;
}

_bool CCell::isInRange(_fvector vPosition, _fmatrix WorldMatrix)
{
	for (_int i = 0; i < ECast(LINE_END); ++i)
	{
		_vector vStartPoint = XMVector3TransformCoord(XMLoadFloat3(&m_vPoints[i]), WorldMatrix);
		_vector vNormal = XMVector3TransformNormal(XMLoadFloat3(&m_vNormals[i]), WorldMatrix);

		_vector vSourDir = vPosition - vStartPoint;

		if (0 < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vSourDir),
			XMVector3Normalize(vNormal))))
		{
			return false;
		}
	}

	return true;
}

void CCell::Reset_Line()
{
	for (_int i = 0; i < LINE_END; ++i)
	{
		m_iNeighbors[i] = -1;
	}
}

void CCell::Update(_fmatrix WorldMatrix)
{

}

#ifdef _DEBUG
HRESULT CCell::Render(class CShader* pShader)
{
	m_pVIBuffer->Bind_VIBuffers();

	m_pVIBuffer->Render();

	return S_OK;
}
#endif

CCell * CCell::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _float3 * pPoints, _uint iIndex)
{
	CCell*		pInstance = new CCell(pDevice, pContext);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pPoints, iIndex)))
	{
		MSG_BOX("Failed to Created : CCell");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CCell::Free()
{
#ifdef _DEBUG	
	Safe_Release(m_pVIBuffer);
#endif	

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
