#pragma once
#ifdef _WIN32
#include <d3d11.h>
#include <dxgi.h>
#include <dxgi1_2.h>

bool CreateD3D11(void* Window, void*& Device, void*& Swapchain, void*& Context);
void DestroyD3D11(void* Device, void* Swapchain, void* Context);

#endif