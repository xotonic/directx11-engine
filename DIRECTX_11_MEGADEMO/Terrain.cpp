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

//DirectX::XMVECTOR Terrain::findIntersection( const VectorPair& ray)
//{
//	XMVECTOR point = XMVectorZero();
//	for (int i = 0; i < v_buf.size(); i += 3)
//	{
//		XMVECTOR t1 = to(v_buf[i].pos);
//		XMVECTOR t2 = to(v_buf[i+1].pos);
//		XMVECTOR t3 = to(v_buf[i+2].pos);
//
//		float dist = 0.0;
//		if (TriangleTests::Intersects(ray.first, XMVector3Normalize(ray.second), t1, t2, t3, dist))
//		{
//			point = ray.first + dist*XMVector3Normalize(ray.second);
//		}
//	}
//	return point;
//}

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
			ty = 20*float(y) / 255.0f;

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
	model = newCollisionModel3D();

	for (int i = 0; i < vy - 1; i++)
	{
		for (int j = 0; j < vx - 1; j++)
		{
			int index_1 = i*vy + j;
			int index_2 = i*vy + j + 1;
			int index_3 = (i + 1)*vy + j + 1;
			int index_4 = (i + 1)*vy + j;

			//XMVECTOR edge1 = XMLoadFloat3(&(unique_vertices[index_1].pos - unique_vertices[index_3].pos)); // диагональ
			//XMVECTOR edge2 = XMLoadFloat3(&(unique_vertices[index_1].pos - unique_vertices[index_2].pos));
			//XMVECTOR normal = XMVector3Cross(edge1, edge2);
			//normal = XMVector3Normalize(-normal);
			XMVECTOR n1 = to(unique_vertices[index_1].normal);
			XMVECTOR n2 = to(unique_vertices[index_2].normal);
			XMVECTOR n4 = to(unique_vertices[index_4].normal);
			XMVECTOR n3 = to(unique_vertices[index_3].normal);
/*
			float a12 = XMVector3AngleBetweenVectors(n1, n2).vector4_f32[0];
			float a13 = XMVector3AngleBetweenVectors(n1, n3).vector4_f32[0];
			float a23 = XMVector3AngleBetweenVectors(n2, n3).vector4_f32[0];

			if (a12 > 0.7) if (a12 > a23) n1 = n2; else n2 = n1;
			if (a13 > 0.7) if (a13 > a23) n1 = n3; else n3 = n1;
			if (a23 > 0.7) if (a23 > a13) n2 = n3; else n3 = n2;
*/

			v_buf.push_back({ { unique_vertices[index_1].pos }, { to(n1) }, { 0.0f, 0.0f } });
			v_buf.push_back({ { unique_vertices[index_2].pos }, { to(n2) }, { 1.0f, 0.0f } });
			v_buf.push_back({ { unique_vertices[index_3].pos }, { to(n3) }, { 1.0f, 1.0f } });

			
			model->addTriangle(unique_vertices[index_1].pos.x, unique_vertices[index_1].pos.y, unique_vertices[index_1].pos.z,
				unique_vertices[index_2].pos.x, unique_vertices[index_2].pos.y, unique_vertices[index_2].pos.z,
				unique_vertices[index_3].pos.x, unique_vertices[index_3].pos.y, unique_vertices[index_3].pos.z);
				
				/*	XMVECTOR edge3 = XMunique_vertices[index_3].posLoadFloat3(&(unique_vertices[index_1].pos - unique_vertices[index_4].pos));
			normal = XMVector3Cross(edge3, edge1);
			normal = XMVector3Normalize(-normal);*/
			/*float a41 = XMVector3AngleBetweenVectors(n1, n1).vector4_f32[0];
			float a43 = XMVector3AngleBetweenVectors(n4, n3).vector4_f32[0];

			if (a41 > 0.7) if (a41 > a43) n1 = n4; else n4 = n1;
			if (a13 > 0.7) if (a13 > a43) n1 = n3; else n3 = n1;
			if (a43 > 0.7) if (a43 > a13) n4 = n3; else n3 = n4;
*/
			v_buf.push_back({ { unique_vertices[index_1].pos }, { to(n1) }, { 0.0f, 0.0f } });
			v_buf.push_back({ { unique_vertices[index_3].pos }, { to(n3) }, { 1.0f, 1.0f } });
			v_buf.push_back({ { unique_vertices[index_4].pos }, { to(n4) }, { 0.0f, 1.0f } });

			model->addTriangle(unique_vertices[index_1].pos.x, unique_vertices[index_1].pos.y, unique_vertices[index_1].pos.z,
				unique_vertices[index_3].pos.x, unique_vertices[index_3].pos.y, unique_vertices[index_3].pos.z,
				unique_vertices[index_4].pos.x, unique_vertices[index_4].pos.y, unique_vertices[index_4].pos.z);
		}
	}
	model->finalize();

	for (int x = 0; x < vx - 1; x++)
		for (int y = 0; y < vy - 1; y++)
		{
			Quad q = getQuad(x, y);
			
			BoundingBox bb;
			XMVECTOR p0 = q.top_left, p1 = q.bot_right;
			p0 = XMVectorSetY(p0, -0.1);
			p1 = XMVectorSetY(p1, q.maxHeigth());
			BoundingBox::CreateFromPoints(bb, p0, p1);

			boxes.push_back(bb);
		}



	filein.close();
	
	
	return true;
}
