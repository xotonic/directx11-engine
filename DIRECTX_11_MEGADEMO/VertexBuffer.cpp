#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(ID3D11Device* dev, vector<TexNormVertex> &data) : device(dev),
	v_buf(data)
{
	dev->GetImmediateContext(&deviceContext);



	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage          = D3D11_USAGE_DEFAULT;
	bd.ByteWidth      = data.size()*sizeof(TexNormVertex);
	bd.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = data.data();

	CHECK_HRESULT(dev->CreateBuffer(&bd, &sd, &vertexBuffer), "Error creating vertex buffer");
}

VertexBuffer::VertexBuffer(ID3D11Device* dev, string filename)
{
	ReadFromOBJ(filename);

	dev->GetImmediateContext(&deviceContext);



	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = v_buf.size()*sizeof(TexNormVertex);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = v_buf.data();

	CHECK_HRESULT(dev->CreateBuffer(&bd, &sd, &vertexBuffer), "Error creating vertex buffer from file");
}

VertexBuffer::~VertexBuffer()
{
	if (vertexBuffer) vertexBuffer->Release();
}
int VertexBuffer::getSize()
{

	return v_buf.size()*sizeof(TexNormVertex);
}

int VertexBuffer::getCount()
{
	return v_buf.size();
}

void VertexBuffer::bind()
{
	UINT i = sizeof(TexNormVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &i, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

bool VertexBuffer::ReadFromOBJ(string filename)
{
	if (filename.find(".mesh") == string::npos)
		filename.append(".mesh");

	ifstream filein(filename);
	if (!filein)
	{
		string msg("File not found: ");
		msg.append(filename);
		MESSAGE(msg);
		return false;
	}

	int vnum;
	vector<TexNormVertex> unique_vertices;

	filein >> vnum;
	unique_vertices.reserve(vnum);



	for (int i = 0; i < vnum; i++)
	{
		float x, y, z, nx, ny, nz;

		filein >> x >> y >> z >> nx >> ny >> nz;

		TexNormVertex v{ { x, y, z }, { nx, ny, nz }, { 0.0f, 0.0f } };

		unique_vertices.push_back(v);

	}

	int inum;
	filein >> inum;
	v_buf.reserve(inum);

	for (int i = 0; i < inum; i++)
	{
		int first, second, third;
		float	first_uv_x, first_uv_y,
			second_uv_x, second_uv_y,
			third_uv_x, third_uv_y;
		filein >> first >> second >> third
			>> first_uv_x >> first_uv_y
			>> second_uv_x >> second_uv_y
			>> third_uv_x >> third_uv_y;



		v_buf.push_back({ unique_vertices[first].pos, unique_vertices[first].normal, { first_uv_x, first_uv_y } });
		v_buf.push_back({ unique_vertices[second].pos, unique_vertices[second].normal, { second_uv_x, second_uv_y } });
		v_buf.push_back({ unique_vertices[third].pos, unique_vertices[third].normal, { third_uv_x, third_uv_y } });

	}

	return true;
}