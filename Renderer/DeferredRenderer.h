#pragma once


//************************************************************
//************ INCLUDES & DEFINES ****************************
//************************************************************

#include <iostream>
#include <ctime>
#include <thread>
#include <string>
using namespace std;
// BEGIN PART 1
// TODO: PART 1 STEP 1a
#include <d3d11.h>
// TODO: PART 1 STEP 1b
#include <DirectXPackedVector.h>
#include<DirectXColors.h>
#include<DirectXCollision.h>

#include<DirectXMath.h>
using namespace DirectX;
// TODO: PART 2 STEP 6
//#include "Trivial_PS.csh"
//#include "Trivial_VS.csh"
//#include "DDSTextureLoader.h"
#include "../safe_thread.h"
//#define BACKBUFFER_WIDTH	600.0f
//#define BACKBUFFER_HEIGHT	600.0f
//#define PI 3.14159f
#include "../Definds.h"
#pragma comment(lib, "d3d11.lib")

class DeferredRenderer
{
public:
	static DeferredRenderer* GetInstance(void);
	DeferredRenderer();
	~DeferredRenderer();

	bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen,float screenDepth, float screenNear);
	void Shutdown();
	void ClearScene(float red, float green, float blue, float alpha);
	void ShowScene();

	//Get's
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	void TurnZBufferOn();
	void TurnZBufferOff();
	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();
	void TurnOnCulling();
	void TurnOffCulling();

	void TurnOnAddBlending();
	void TurnOFFaddBlending();

	void TurnOnCCWcullMode();
	void TurnOnCWcullMode();

	void SetBackBufferRenderTarget();
	void ResetViewport();

	//Get
	ID3D11Buffer *GetIndexBuffer(){ return indexBufferPtr; }

	//set
	UINT AddIndices(const UINT *_indices, UINT _numIndices);
private:

	UINT LastIndex;
	ID3D11Buffer *indexBufferPtr;

	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	ID3D11RasterizerState* m_rasterStateNoCulling;

	//for point light
	ID3D11RasterizerState* CCWcullMode;
	ID3D11RasterizerState* CWcullMode;


	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;
	ID3D11DepthStencilState* m_depthDisabledStencilState;
	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;
	ID3D11BlendState* AddBlendingState;
	D3D11_VIEWPORT m_viewport;
};

