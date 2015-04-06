#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(ID3D11Device* dev, vector<unsigned int> &data)
	: device(dev),
	i_buf(data)
{
	dev->GetImmediateContext(&deviceContext);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = data.size()*sizeof(unsigned int);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = data.data();

	CHECK_HRESULT(dev->CreateBuffer(&bd, &sd, &indexBuffer), "Error creating index buffer");
}

IndexBuffer::~IndexBuffer()
{
}

int IndexBuffer::getSize()
{
	return i_buf.size()*sizeof(unsigned int);
}

int IndexBuffer::getCount()
{
	return i_buf.size();
}

void IndexBuffer::bind()
{
	UINT i = sizeof(unsigned int);
	UINT offset = 0;
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}