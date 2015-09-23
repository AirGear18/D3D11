//
// Game.h
//

#pragma once

#include "pch.h"
#include "StepTimer.h"
class InputClass;
class CameraClass;
class DeferredBuffers;
class DeferredShader;
class OrthoWindowClass;
class LightClass;
class LightShaderClass;
class BaseGameState;
class SkyBox;
// A basic game implementation that creates a D3D11 device and
// provides a game loop
class Game
{
public:

	Game();

	// Initialization and management
	void Initialize(HWND window, HINSTANCE hInstance);

	// Basic game loop
	void Tick();
	void Render();

	// Rendering helpers
	void Clear();
	void Present();
	void RenderSceneToTexture();

	// Messages
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnWindowSizeChanged();
	void ShutDown();

	// Properites
	void GetDefaultSize(size_t& width, size_t& height) const;

	bool GetGameState(){ return GameRunning; }

	void ChangeState(BaseGameState* pNewState);

private:

	void Update(DX::StepTimer const& timer);

	void CreateDevice();
	void CreateResources();



	// Application state
	HWND                                            m_window;
	HINSTANCE hInstance;

	//input
	//InputClass* m_Input;
	//Cam
	CameraClass *m_Camera;
	//renderer
	DeferredBuffers * m_DeferredBuffer;
	DeferredShader * m_DeferredShader;
	OrthoWindowClass *m_QuadScreen;
	LightClass *m_Light;
	LightShaderClass *m_LightShader;

	SkyBox * m_SkyBox;

	// Game state
	DX::StepTimer                                   m_timer;

	// Current Game State
	BaseGameState*				m_pCurrState = nullptr;

	bool GameRunning;
};