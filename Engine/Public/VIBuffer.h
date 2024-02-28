#pragma once

#include "Component.h"

/* Ư�� ����(Rect, Cube, Terrain, Model) �� �����ϱ����� Ŭ�������� �θ� �Ǵ� Ŭ������ .*/
/* VIBuffer = Vertex(����) + Index(�׸��� ������ ���� ������ �ε��� ����.) + Buffer(�޸𸮰���) */

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render();

public:
	ID3D11Buffer*				Get_VertexBuffer() { return m_pVB; }
	ID3D11Buffer*				Get_IndexBuffer() { return m_pIB; }
	_uint						Get_NumVertices() { return m_iNumVertices; }


	virtual HRESULT Bind_VIBuffers();
	/* ����, �ε���, �ν��Ͻ� ���۵��� �����Ѵ�. */
	HRESULT Create_Buffer(_Inout_ ID3D11Buffer**	ppBuffer);

protected:
	ID3D11Buffer*				m_pVB = { nullptr };
	ID3D11Buffer*				m_pIB = { nullptr };

	/* ���� ���� ������� �ϴ� ������ �Ӽ��� �����Ѵ�. */
	D3D11_BUFFER_DESC			m_BufferDesc;
	D3D11_SUBRESOURCE_DATA		m_SubResourceData;

protected:
	_uint						m_iNumVertices = { 0 };
	_uint						m_iStride = { 0 };
	_uint						m_iNumVertexBuffers = { 0 };

	_uint						m_iNumIndices = { 0 };	//m_iIndexSizeofPrimitive
	_uint						m_iIndexStride = { 0 }; // m_iNumPrimitives
	DXGI_FORMAT					m_eIndexFormat = {  };
	D3D11_PRIMITIVE_TOPOLOGY	m_eTopology = { };

	_float3*					m_pVerticesPos = { nullptr };

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END