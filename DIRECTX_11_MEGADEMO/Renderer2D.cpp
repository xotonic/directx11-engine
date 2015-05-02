#include "Renderer2D.h"


Renderer2D::Renderer2D(HWND hwnd)
{
	CHECK_HRESULT(
		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_2dFactory),
		"error creating Direct2D factory"
		);
	CHECK_HRESULT(
		DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&dwFactory)),
		"error creating DirectWrite factory"
		);

	CHECK_HRESULT(
		dwFactory->CreateTextFormat(L"Arial", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 12.0f / 72.0f*96.0f, L"en-us",
		&dwTextFormat),
		"error creating DirectWrite textFormat"
		);

	CHECK_HRESULT(dwTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING),
		"error setting text aligment");

	RECT rc;
	GetClientRect(hwnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	CHECK_HRESULT(
		_2dFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd, size), &_2dHwndRT),
		"error creating Direct2D render target"
		);

	CHECK_HRESULT(
		_2dHwndRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &brush),
		"error creating brush"
		);

	D2D1_SIZE_F sizeRT = _2dHwndRT->GetSize();

	wchar_t* text = L"YOU ARE FUCKED AZAZAZAZA";

	CHECK_HRESULT(
		dwFactory->CreateTextLayout(text, wcslen(text), dwTextFormat, sizeRT.width, sizeRT.height,
		&dwTextLayout),
		"error creting text layout"
		);
}


Renderer2D::~Renderer2D()
{
	if (dwTextLayout) dwTextLayout->Release();
	if (brush) brush->Release();
	if (_2dHwndRT) _2dHwndRT->Release();

	if (dwTextFormat) dwTextFormat->Release();
	if (dwFactory) dwFactory->Release();
	if (_2dFactory) _2dFactory->Release();

}

void Renderer2D::Render()
{
	_2dHwndRT->BeginDraw();

	_2dHwndRT->SetTransform(D2D1::IdentityMatrix());
	D2D1_POINT_2F origin;
	origin.x = 10;
	origin.y = 10;
	_2dHwndRT->DrawTextLayout(origin, dwTextLayout, brush);
	
	CHECK_HRESULT(
		_2dHwndRT->EndDraw(),
		"error in EndDraw() of Direct2D");
}
