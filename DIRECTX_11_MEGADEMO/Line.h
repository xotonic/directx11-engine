#pragma once
#include "stdafx.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "DXResources.h"
#include "ConstantBuffer.h"
#include <array>
#include "debug.h"
#include "Camera.h"
class Line
{
public:
	Line(DXResources* _dx);
	~Line() { vertexBuffer->Release(); }
	void Draw(Camera& cam);
	void SetLine(VectorPair& line);
	std::shared_ptr<Shader> basic_shader;
private:
	DXResources* dx;
	std::array<ColVertex,2> lines;
	ID3D11Buffer*   vertexBuffer;
	std::shared_ptr<ConstantBuffer<MatrixBuffer>> matrices;
	
};

