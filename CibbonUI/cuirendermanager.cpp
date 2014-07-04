#include "stdafx.h"
#include "cuirendermanager.h"

using namespace std;
using namespace D2D1;

shared_ptr<cuirendermanager> cuirendermanager::pManager = nullptr;

cuirendermanager::cuirendermanager() 
{
}


cuirendermanager::~cuirendermanager()
{
}

std::shared_ptr<cuirendermanager> cuirendermanager::getManager()
{
	if (!pManager)
		pManager = make_shared<cuirendermanager>();
	return pManager;
}

void cuirendermanager::CreateDeviceIndependentResources()
{
	HRESULT hr = E_FAIL;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	if (FAILED(hr)) std::abort();
}

void cuirendermanager::CreateDeviceResources()
{
	HRESULT hr;

	hr = pD2DFactory->CreateHwndRenderTarget(
		RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, 
		D2D1_ALPHA_MODE_PREMULTIPLIED)),

		)
}