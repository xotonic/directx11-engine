#include "Console.h"

Console::Console(DXResources* _dx) : dx(_dx)
{
	Debug::if_failed(dx->dwFactory->CreateTextFormat(L"Consolas", NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		12,
		L"",
		&textFormat),
		"Cant create text format");

	textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	dx->d2dRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);
}

void Console::SetParam(string name, wstring value)
{
	auto param = params.find(name);
	if (param == params.end()) params.insert(pair<string, wstring>(name, value));
	else params[name] = value;
}

void Console::SetParam(string name, wstring caption, int value)
{
	wostringstream os;
	os << caption << value << endl;
	SetParam(name, os.str());
}

void Console::SetParam(string name, wstring caption, float value)
{
	wostringstream os;
	os << caption << value << endl;
	SetParam(name, os.str());
}

void Console::SetParam(string name, wstring caption, XMFLOAT3 xyz)
{
	wostringstream os;
	os << caption << endl <<
		"x: " << xyz.x << endl <<
		"y: " << xyz.y << endl <<
		"z: " << xyz.z << endl;
	SetParam(name, os.str());
}

void Console::SetParam(string name, wstring caption, XMMATRIX& mat)
{
	wostringstream os;
	os << caption << endl << std::fixed //<< std::setrecision(2)
		<< mat._11 << ' ' << mat._12 << ' ' << mat._13 << ' ' << mat._14 << endl
		<< mat._21 << ' ' << mat._22 << ' ' << mat._23 << ' ' << mat._24 << endl
		<< mat._31 << ' ' << mat._32 << ' ' << mat._33 << ' ' << mat._34 << endl
		<< mat._41 << ' ' << mat._42 << ' ' << mat._43 << ' ' << mat._44 << endl;

	SetParam(name, os.str());
}

void Console::SetMessage(wstring value)
{
	messages.push_back(value);
}

void Console::Update()
{
	wostringstream os;

	for (auto param : params)
		os << param.second << endl;
	os << endl;

	t++;
	if (t == message_life_time)
	{
		if (!messages.empty())
			messages.pop_front();

		t = 0;
	}

	if (!messages.empty())
		for (auto msg : messages)
			os << msg << endl;

	dx->d2dRT->SetTransform(D2D1::Matrix3x2F::Identity());
	//dx->d2dRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

	dx->d2dRT->DrawText(os.str().c_str(), os.str().size(), textFormat,
		D2D1::RectF(10, 10, float(dx->winDesc.size.width / 2), float(dx->winDesc.size.height / 2)), brush);
}