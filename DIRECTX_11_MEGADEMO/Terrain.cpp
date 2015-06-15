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

	wight = vx; height = vy;

	for (int i = 0; i < vx; i++)
	{
		for (int j = 0; j < vy; j++)
		{
			int y = 0;
			float ty = 0.0f;

			filein >> y;
			ty = 50 * float(y) / 255.0f;

			float tx = float(i);
			float tz = float(j);

			TexNormVertex v{ { tx, ty, tz }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } };

			unique_vertices.push_back(v);
		}
	}
	int face_num = 2 * vx * vy;

	// calculating normals
	vector<XMVECTOR> normals;
	//normals.reserve(vy*vx);
	normals.resize(vy*vx);

	for (int i = 0; i < vy - 1; i++)
	{
		for (int j = 0; j < vx - 1; j++)
		{
			int index_1 = i*vy + j;
			int index_2 = i*vy + j + 1;
			int index_3 = (i + 1)*vy + j + 1;
			int index_4 = (i + 1)*vy + j;

			XMVECTOR edge1 = XMLoadFloat3(&(unique_vertices[index_1].pos - unique_vertices[index_3].pos)); // диагональ
			XMVECTOR edge2 = XMLoadFloat3(&(unique_vertices[index_1].pos - unique_vertices[index_2].pos));
			XMVECTOR edge3 = XMLoadFloat3(&(unique_vertices[index_1].pos - unique_vertices[index_4].pos));

			XMVECTOR normal = XMVector3Cross(edge1, edge2);
			normal = XMVector3Normalize(-normal);
			normals[index_1] += normal;
			normals[index_2] += normal;
			normals[index_3] += normal;

			normal = XMVector3Cross(edge3, edge1);
			normal = XMVector3Normalize(-normal);
			normals[index_1] += normal;
			normals[index_3] += normal;
			normals[index_4] += normal;
		}
	}
	// normalizing normals
	for (int i = 0; i < normals.size(); i++)
	{
		XMVECTOR n = normals[i];
		n = XMVector3Normalize(n);
		XMStoreFloat3(&unique_vertices[i].normal, n);
	}

	// forming vertex buffer

	v_buf.reserve(face_num * 3 * 2);

	for (int i = 0; i < vy - 1; i++)
	{
		for (int j = 0; j < vx - 1; j++)
		{
			int index_1 = i*vy + j;
			int index_2 = i*vy + j + 1;
			int index_3 = (i + 1)*vy + j + 1;
			int index_4 = (i + 1)*vy + j;

			XMVECTOR n1 = to(unique_vertices[index_1].normal);
			XMVECTOR n2 = to(unique_vertices[index_2].normal);
			XMVECTOR n4 = to(unique_vertices[index_4].normal);
			XMVECTOR n3 = to(unique_vertices[index_3].normal);
		
			v_buf.push_back({ { unique_vertices[index_1].pos }, { to(n1) }, { 0.0f, 0.0f } });
			v_buf.push_back({ { unique_vertices[index_2].pos }, { to(n2) }, { 1.0f, 0.0f } });
			v_buf.push_back({ { unique_vertices[index_3].pos }, { to(n3) }, { 1.0f, 1.0f } });

		
			v_buf.push_back({ { unique_vertices[index_1].pos }, { to(n1) }, { 0.0f, 0.0f } });
			v_buf.push_back({ { unique_vertices[index_3].pos }, { to(n3) }, { 1.0f, 1.0f } });
			v_buf.push_back({ { unique_vertices[index_4].pos }, { to(n4) }, { 0.0f, 1.0f } });
		}
	}

	filein.close();

	return true;
}