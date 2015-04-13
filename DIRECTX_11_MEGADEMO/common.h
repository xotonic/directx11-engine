#pragma once
//#ifndef COMMON_H_
//#define COMMON_H_
#define _XM_NO_INTRINSICS_
#include <d3d11.h>
#include <DirectXMath.h>
#include <Windows.h>
#include <string>
#include "debug.h"

using namespace DirectX;

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
	XMFLOAT3 pos = { 0.0, 0.0, 0.0 };
};

struct ColVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
};

struct ColNormVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
	XMFLOAT2 normal;
};

struct NormVertex
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
};

struct TexNormVertex
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 uv;
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
/*LPCWSTR stringToLPCWSTR(std::string str)
{
std::wstring temp = std::wstring(str.begin(), str.end());
return temp.c_str();
}*/
/*struct perFrameCBuffer
{
XMMATRIX view;
XMMATRIX projection;
};

struct perObjectCBuffer
{
XMMATRIX world = XMMatrixIdentity();
};*/

//#endif