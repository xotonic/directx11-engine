#include "Terrain.h"

Terrain::Terrain(ID3D11Device* dev, std::string file) : DeviceDependent(dev)
{
	ReadFromFile(file);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = v_buf.size()*sizeof(TexNormVertex);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = v_buf.data();

	CHECK_HRESULT(device->CreateBuffer(&bd, &sd, &vertexBuffer), "Error creating vertex buffer");
}

Terrain::~Terrain()
{
	if (vertexBuffer) vertexBuffer->Release();
}

int Terrain::getSize()
{
	return v_buf.size()*sizeof(TexNormVertex);
}

int Terrain::getCount()
{
	return v_buf.size();
}

void Terrain::bind()
{
	UINT i = sizeof(TexNormVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &i, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Terrain::draw()
{
	deviceContext->Draw(getCount(), 0);
}

bool Terrain::ReadFromFile(std::string filename)
{
	if (filename.find(".terrain") == string::npos)
		filename.append(".terrain");

	ifstream filein(filename);
	if (!filein)
	{
		string msg("File not found: ");
		msg.append(filename);
		MESSAGE(msg);
		return false;
	}

	int vx, vy;
	vector<TexNormVertex> unique_vertices;

	filein >> vx >> vy;
	unique_vertices.reserve(vx * vy);

	for (int i = 0; i < vx; i++)
	{
		for (int j = 0; j < vy; j++)
		{
			int y = 0;
			float ty = 0.0f;

			filein >> y;
			ty = float(y) / 255.0f;

			float tx = float(i);
			float tz = float(j);

			TexNormVertex v{ { tx, ty, tz }, { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f } };

			unique_vertices.push_back(v);
		}
	}
	int face_num = 2 * vx * vy;

	v_buf.reserve(face_num * 3 * 2);

	for (int i = 0; i < vy - 1; i++)
	{
		for (int j = 0; j < vx - 1; j++)
		{
			int index_1 = i*vy + j;
			int index_2 = i*vy + j + 1;
			int index_3 = (i + 1)*vy + j + 1;
			int index_4 = (i + 1)*vy + j;

			v_buf.push_back({ { unique_vertices[index_1].pos }, { unique_vertices[index_1].normal }, { 0.0f, 0.0f } });
			v_buf.push_back({ { unique_vertices[index_2].pos }, { unique_vertices[index_2].normal }, { 1.0f, 0.0f } });
			v_buf.push_back({ { unique_vertices[index_3].pos }, { unique_vertices[index_3].normal }, { 1.0f, 1.0f } });

			v_buf.push_back({ { unique_vertices[index_1].pos }, { unique_vertices[index_1].normal }, { 0.0f, 0.0f } });
			v_buf.push_back({ { unique_vertices[index_3].pos }, { unique_vertices[index_3].normal }, { 1.0f, 0.0f } });
			v_buf.push_back({ { unique_vertices[index_4].pos }, { unique_vertices[index_4].normal }, { 0.0f, 1.0f } });
		}
	}

	filein.close();
	return true;
}