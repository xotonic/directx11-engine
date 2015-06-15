#pragma once
#include "stdafx.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "DXResources.h"
#include "ConstantBuffer.h"
#include <vector>
#include "debug.h"
#include "Camera.h"
class Line
{
public:
	Line(DXResources* _dx, int count);
	~Line() { vertexBuffer->Release(); }
	void Draw(Camera& cam);
	void SetLines(const std::vector<XMVECTOR> &l);
	std::shared_ptr<Shader> basic_shader;
private:
	DXResources* dx;
	std::vector<ColVertex> lines;
	ID3D11Buffer*   vertexBuffer;
	std::shared_ptr<ConstantBuffer<MatrixBuffer>> matrices;
};
