#include "Graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

HRESULT Graphics::Initialize()
{
	return S_OK;
}

void Graphics::Release()
{

}

Graphics* Graphics::GetInstance()
{
	static Graphics instance;
	return &instance;
}