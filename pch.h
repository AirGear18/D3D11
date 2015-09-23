//
// pch.h
// Header for standard system include files.
//

#pragma once

#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0600
#include <SDKDDKVer.h>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <wrl/client.h>

#include <d3d11_1.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <algorithm>
#include <exception>
#include <memory>

const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
#define PI 3.14159f

using namespace std;
using namespace DirectX;
namespace DX
{
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            // Set a breakpoint on this line to catch DirectX API errors
            throw std::exception();
        }
    }
}

struct VertexInputType
{

	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT2 tex;
	DirectX::XMFLOAT3 normal;
};

enum ContextType
{
	Posnormuv,poscolor,
};