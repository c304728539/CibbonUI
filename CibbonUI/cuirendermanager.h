#pragma once

#include <d2d1.h>
#include <d2d1helper.h>
#include <wincodec.h>

class cuirendermanager
{

public:

	static std::shared_ptr<cuirendermanager> getManager();
	~cuirendermanager();

private:

	cuirendermanager();
	static std::shared_ptr<cuirendermanager> pManager ;

	ID2D1Factory* pD2DFactory;
	ID2D1HwndRenderTarget* pRT;
	ID2D1SolidColorBrush* pWhiteBrush;
	HWND m_hWnd;

	void CreateDeviceIndependentResources();
	void CreateDeviceResources();
};

