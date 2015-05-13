#include "Line.h"

Line::Line(DXResources* _dx) : dx(_dx)
{
	matrices = std::make_shared<ConstantBuffer<MatrixBuffer>>(dx->device);
	D3D11_BUFFER_DESC bd = { 0 };
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(ColVertex)*lines.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA sd = { 0 };
	sd.pSysMem = lines.data();
	sd.SysMemPitch = 0;
	sd.SysMemSlicePitch = 0;

	CHECK_HRESULT(dx->device->CreateBuffer(&bd, &sd, &vertexBuffer), "Error creating vertex buffer");

	basic_shader = std::make_shared<Shader>(dx->device, "basic.vsh", "basic.psh", true, false, false);
}


void Line::Draw(Camera& cam)
{

	auto dc = dx->deviceContext;
	
	// updating vertex buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	ZeroMemory(&ms, sizeof(ms));

	CHECK_HRESULT(
		dc->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms),
		"error mapping subresource");

	memcpy(ms.pData, lines.data(), lines.size()*sizeof(ColVertex));

	dc->Unmap(vertexBuffer, 0);
	dc->PSSetShaderResources(0, 0, nullptr);

	//setting states
	dc->VSSetConstantBuffers(0, 0, nullptr);
	dc->PSSetConstantBuffers(0, 0, nullptr);

	basic_shader->bind();

	matrices->data.world = XMMatrixTranspose(XMMatrixIdentity());
	matrices->data.view = cam.View();
	matrices->data.projection = cam.Proj();
			
	matrices->update();
	matrices->bind_VS(0);
	matrices->bind_PS(0);


	UINT i = sizeof(TexNormVertex);
	UINT offset = 0;
	dc->IASetVertexBuffers(0, 1, &vertexBuffer, &i, &offset);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	dc->Draw(lines.size(), 0);
}

void Line::SetLine(XMVECTOR v1, XMVECTOR v2)
{
	lines[0] = { { to(v1) }, { 1.0, 1.0, 1.0, 1.0 } };
	lines[1] ={ { to(v2) }, { 1.0, 1.0, 1.0, 1.0 } };
}
