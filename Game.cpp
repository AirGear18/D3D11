//
// Game.cpp -
//

#include "pch.h"
#include "Game.h"
//SetUp Render
#include "Renderer\DeferredRenderer.h"
#include "Renderer\DeferredBuffers.h"
#include "Renderer\DeferredShader.h"
#include "Renderer\orthowindowclass.h"
#include "Renderer\lightclass.h"
#include "Renderer\lightshaderclass.h"

//Model loader - Jordans loader
#include "ModelObject.h"

//input and camera class from another person
#include "Input\inputclass.h"
#include "Camera\cameraclass.h"

#include <thread>

///EventManager
#include "Input\EventManager.h"
//ObjectManager
#include "Object\ObjectManager.h"

//Test Object update and renderer
#include "Object\BaseRenderer.h"

using namespace DirectX;

using Microsoft::WRL::ComPtr;

// Constructor.
Game::Game() :
m_window(0)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, HINSTANCE hInstance)
{
	m_window = window;
	this->hInstance = hInstance;

	CreateDevice();
	GameRunning = true;
	// Create the camera object.
	m_Camera = new CameraClass;

	// Set the initial position of the camera and build the matrices needed for rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -50);
	m_Camera->Render();
	m_Camera->RenderBaseViewMatrix();

	size_t w, h;
	GetDefaultSize(w, h);
	//deferred buffers set up
	m_DeferredBuffer = new DeferredBuffers();
	m_DeferredBuffer->Initialize(DeferredRenderer::GetInstance()->GetDevice(), w, h, SCREEN_DEPTH, SCREEN_NEAR);

	m_DeferredShader = new DeferredShader;
	// Initialize the deferred shader object.
	if (!m_DeferredShader->Initialize(DeferredRenderer::GetInstance()->GetDevice(), window))
	{
		MessageBox(window, L"Could not initialize the deferred shader object.", L"Error", MB_OK);
	}
	m_QuadScreen = new OrthoWindowClass;
	// Initialize the full screen ortho window object.

	if (!m_QuadScreen->Initialize(DeferredRenderer::GetInstance()->GetDevice(), w, h))
	{
		MessageBox(window, L"Could not initialize the full screen ortho window object.", L"Error", MB_OK);
	}


	// Create the light object.
	m_Light = new LightClass;

	// Initialize the light object.
	m_Light->SetDiffuseColor(255.0f, 255.0f, 255.0f, 255.0f);
	m_Light->SetDirection(0.0f, -1.0f, 0.0f);

	m_LightShader = new LightShaderClass;

	// Initialize the light shader object.
	if (!m_LightShader->Initialize(DeferredRenderer::GetInstance()->GetDevice(), window))
	{
		MessageBox(window, L"Could not initialize the light shader object.", L"Error", MB_OK);
	}

	
	BaseRenderer * temp = new BaseRenderer(1, "Assets/Models/Trex2.obj","Assets/Models/TrexTemp.dds",false,NULL,XMFLOAT3(0,0,0),"Trex2");
	
	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/


}

// Executes basic game loop.
void Game::Tick()
{
	m_timer.Tick([&]()
	{
		Update(m_timer);
	});

}

// Updates the world
void Game::Update(DX::StepTimer const& timer)
{
	float elapsedTime = float(timer.GetElapsedSeconds());

	// TODO: Add your game logic here
	elapsedTime;

	//Camera update
	m_Camera->Render();

	//check keyboard state if any keys
	m_Input->Frame();
	if (m_Input->IsEscapePressed())
	{
		GameRunning = false;
	}
	if (m_Input->IsUpPressed())
	{
		m_Camera->SetPosition(m_Camera->GetPosition().x, m_Camera->GetPosition().y + (10)*.01, m_Camera->GetPosition().z);
	}
	if (m_Input->IsDownPressed())
	{
		m_Camera->SetPosition(m_Camera->GetPosition().x, m_Camera->GetPosition().y - (10)*.01, m_Camera->GetPosition().z);
	}
	if (m_Input->IsLeftPressed())
	{
		m_Camera->SetPosition(m_Camera->GetPosition().x - (10)*.01, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
	}
	if (m_Input->IsRightPressed())
	{
		m_Camera->SetPosition(m_Camera->GetPosition().x + (10)*.01, m_Camera->GetPosition().y, m_Camera->GetPosition().z);

	}
	if (m_Input->IsPgDownPressed())
	{
		m_Camera->SetPosition(m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z - (10)*.01);

	}
	if (m_Input->IsPgUpPressed())
	{
		m_Camera->SetPosition(m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z + (10)*.01);

	}

	//Updated all moveable object that have been added.
	ObjectManager::GetInstance()->ObjectUpdate(elapsedTime);
	//	like name says
	Render();
}

// Draws the scene
void Game::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
		return;

	// TODO: Add your rendering code here

	Present();
}

// Helper method to clear the backbuffers
void Game::Clear()
{
	// Clear the views
	//m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
	//m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	//m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
}


void Game::RenderSceneToTexture()
{
	XMMATRIX viewMatrix, projectionMatrix;
	// Set the render buffers to be the render target.
	m_DeferredBuffer->SetRenderTargets(DeferredRenderer::GetInstance()->GetDeviceContext());

	// Clear the render buffers.
	m_DeferredBuffer->ClearRenderTargets(DeferredRenderer::GetInstance()->GetDeviceContext(), 0.2f, 0.0f, 0.0f, 1.0f);

	m_Camera->GetViewMatrix(viewMatrix);
	DeferredRenderer::GetInstance()->GetProjectionMatrix(projectionMatrix);

	//TODO: Place renderer object here to renderer to buffers
	ObjectManager::GetInstance()->ObjectRenderer(viewMatrix,projectionMatrix,m_DeferredShader);


	// Reset the render target back to the original back buffer and not the render buffers anymore.
	DeferredRenderer::GetInstance()->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	DeferredRenderer::GetInstance()->ResetViewport();
}

// Presents the backbuffer contents to the screen
void Game::Present()
{
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

	//
	RenderSceneToTexture();

	//Clear backBuffer
	DeferredRenderer::GetInstance()->ClearScene(1, 0, 0, 1);

	//get world mat
	DeferredRenderer::GetInstance()->GetWorldMatrix(worldMatrix);
	//get view matrix for quad
	m_Camera->GetBaseViewMatrix(baseViewMatrix);
	//get GetOrthoMatrix
	DeferredRenderer::GetInstance()->GetOrthoMatrix(orthoMatrix);
	//Magic teehee
	orthoMatrix = XMMatrixMultiply(baseViewMatrix, orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	DeferredRenderer::GetInstance()->TurnZBufferOff();

	// Put the full screen ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_QuadScreen->Render(DeferredRenderer::GetInstance()->GetDeviceContext());

	// Render the full screen ortho window using the deferred light shader and the render buffers.
	m_LightShader->Render(DeferredRenderer::GetInstance()->GetDeviceContext(), m_QuadScreen->GetIndexCount(), worldMatrix, baseViewMatrix, orthoMatrix,
		m_DeferredBuffer->GetShaderResourceView(0), m_DeferredBuffer->GetShaderResourceView(1),
		m_Light->GetDirection());

	// Turn the Z buffer back on now that all 2D rendering has completed.
	DeferredRenderer::GetInstance()->TurnZBufferOn();



	DeferredRenderer::GetInstance()->ShowScene();

}

// Message handlers
void Game::OnActivated()
{
	// TODO: Game is becoming active window
}

void Game::OnDeactivated()
{
	// TODO: Game is becoming background window
}

void Game::OnSuspending()
{
	// TODO: Game is being power-suspended (or minimized)
}

void Game::OnResuming()
{
	m_timer.ResetElapsedTime();

	// TODO: Game is being power-resumed (or returning from minimize)
}

void Game::OnWindowSizeChanged()
{
	// TODO: Game window is being resized
}

// Properties
void Game::GetDefaultSize(size_t& width, size_t& height) const
{
	// TODO: Change to desired default window size (note minimum size is 320x200)
	width = 800;
	height = 600;
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
	//Basic setup
	size_t w, h;
	GetDefaultSize(w, h);
	DeferredRenderer::GetInstance()->Initialize(w, h, false, m_window, false, SCREEN_DEPTH, SCREEN_NEAR);

	m_Input = new InputClass;

	// Initialize the input object.
	if (!m_Input->Initialize(hInstance, m_window, w, h))
	{
		MessageBox(m_window, L"Could not initialize the input object.", L"Error", MB_OK);
	}


}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{




	// TODO: Initialize windows-size dependent objects here
}

void Game::ShutDown()
{
	// TODO: cleanup here
	//Base clean up
	DeferredRenderer::GetInstance()->Shutdown();
	m_Input->Shutdown();
	m_DeferredBuffer->Shutdown();
	m_DeferredShader->Shutdown();
	m_QuadScreen->Shutdown();
	m_LightShader->Shutdown();
	delete m_Light;

	/////////////////////////

}