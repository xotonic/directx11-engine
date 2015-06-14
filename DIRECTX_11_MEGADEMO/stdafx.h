#pragma once
#define _XM_NO_INTRINSICS_
#include <wrl.h>
#include <wrl/client.h>

#include <d3d11_2.h>
#include <d2d1_2.h>
#include <dwrite_2.h>
#include <wincodec.h>
#include <DirectXMath.h>

#include <Windows.h>
#include <string>
#include <memory>
#include <array>
#include "debug.h"

using namespace DirectX;
using namespace Microsoft::WRL;

typedef std::pair<XMVECTOR, XMVECTOR> VectorPair;

struct WindowSize
{
	int width, height;
};

struct WindowDescriptor
{
	HWND hWnd;
	WindowSize size;
};

struct Vertex
{
	XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
};

struct ColVertex
{
	XMFLOAT4 pos;
	XMFLOAT4 color ;

};

struct ColNormVertex
{
	XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
	XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMFLOAT3 normal = {0.0f, 0.0f, 0.0f};
};

struct TexNormVertex
{
	XMFLOAT3 pos;// = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 normal;// = { 0.0f, 0.0f, 0.0f };
	XMFLOAT2 uv;// = { 0.0f, 0.0f };
};

struct MatrixBuffer
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

struct DirLightBuffer
{
	XMFLOAT4 dir;
	XMFLOAT4 color;
};

static std::wstring stringToWstring(const std::string str)
{
	std::wstring temp = std::wstring(str.begin(), str.end());
	return temp;
}

inline XMVECTOR to(const XMFLOAT3& x)
{
	return XMLoadFloat3(&x);
}

inline XMFLOAT3 to(const XMVECTOR& x)
{
	XMFLOAT3 val;
	XMStoreFloat3(&val, x);
	return val;
}

inline XMFLOAT4 to2(const XMVECTOR& x)
{
	XMFLOAT4 val;
	XMStoreFloat4(&val, x);
	return val;
}

inline XMFLOAT4X4 to(const XMMATRIX& x)
{
	XMFLOAT4X4 val;
	XMStoreFloat4x4(&val, x);
	return val;
}

inline XMMATRIX to(const XMFLOAT4X4& x)
{
	return XMLoadFloat4x4(&x);	
}

inline XMFLOAT3 to(const XMFLOAT4& x)
{
	return XMFLOAT3(x.x,x.y, x.z);
}

