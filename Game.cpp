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
#include "Renderer\SkyBox.h"
//PointLight
#include "Renderer\PointLight.h"
#include "Renderer\LightManager.h"

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

//GameStates
#include "GameStates\MainMenu.h"

//Debug
#include "DebugStuff\ShapeDebug.h"

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
	m_Camera->SetupViewMat();
	m_Camera->RenderBaseViewMatrix();

	size_t w, h;
	GetDefaultSize(w, h);
	//deferred buffers set up
	m_DeferredBuffer = new DeferredBuffers();
	m_DeferredBuffer->Initialize(DeferredRenderer::GetInstance()->GetDevice(), (int)w, (int)h, SCREEN_DEPTH, SCREEN_NEAR);

	m_DeferredShader = new DeferredShader;
	// Initialize the deferred shader object.
	if (!m_DeferredShader->Initialize(DeferredRenderer::GetInstance()->GetDevice(), window))
	{
		MessageBox(window, L"Could not initialize the deferred shader object.", L"Error", MB_OK);
	}
	m_QuadScreen = new OrthoWindowClass;
	// Initialize the full screen ortho window object.

	if (!m_QuadScreen->Initialize(DeferredRenderer::GetInstance()->GetDevice(), (int)w, (int)h))
	{
		MessageBox(window, L"Could not initialize the full screen ortho window object.", L"Error", MB_OK);
	}


	// Create the light object.
	m_Light = new LightClass;

	// Initialize the light object.
	m_Light->SetDiffuseColor(1, 1, 1, 1);
	m_Light->SetDirection(0.0f, -1.0f, 0.0f);
	m_Light->SetAmbientColor(1, 1, 1, 1);

	m_LightShader = new LightShaderClass;

	// Initialize the light shader object.
	if (!m_LightShader->Initialize(DeferredRenderer::GetInstance()->GetDevice(), window))
	{
		MessageBox(window, L"Could not initialize the light shader object.", L"Error", MB_OK);
	}

	BaseRenderer * temp = new BaseRenderer(1, "Assets/Models/Trex2.obj", "Assets/Models/TrexTemp.dds", false, NULL, XMFLOAT3(0, 0, 0), "Trex2");
	temp->SetPosition(XMFLOAT3(10, 0, 0));
	temp->SetContextType(0);
	ObjectManager::GetInstance()->AddToContextListSort(0, temp);
	temp = nullptr;

	temp = new BaseRenderer(1, "Assets/Models/Trex2.obj", "Assets/Models/TrexTemp.dds", false, NULL, XMFLOAT3(0, 0, 0), "Trex2");
	temp->SetPosition(XMFLOAT3(-10, 0, 0));
	temp->SetContextType(0);
	ObjectManager::GetInstance()->AddToContextListSort(0, temp);
	temp = nullptr;
	//for (size_t i = 0; i < 10; i++)
	//{
	//	for (size_t j = 0; j < 50; j++)
	//	{
	//		temp = new BaseRenderer(1, "Assets/Models/Trex2.obj", "Assets/Models/TrexTemp.dds", false, NULL, XMFLOAT3(0, 0, 0), "Trex2");
	//		temp->SetPosition(XMFLOAT3(i*2, 0, j*5));
	//		temp->SetContextType(0);
	//		ObjectManager::GetInstance()->AddToContextListSort(0, temp);
	//		temp = nullptr;
	//	}
	//}

	//	Floor/walls
	temp = new BaseRenderer(0, "Assets/Models/Layout.obj", "Assets/Models/TrexTemp.dds", false, NULL, XMFLOAT3(0, 0, 0), "FloorWall");
	temp->SetPosition(XMFLOAT3(0, 0, 0));
	ObjectManager::GetInstance()->AddToContextListSort(0, temp);

	ChangeState(MainMenu::GetInstance());
	ShapeDebug::GetInstance()->Initialize();

	m_SkyBox = new SkyBox();

	LightManager::Getinstance()->SetLightShaderClass(m_LightShader);
	LightManager::Getinstance()->SetDeferredBuffers(m_DeferredBuffer);

	PointLight * TestPointLight = new PointLight(XMFLOAT4(.5f, .2f, 0, 1), XMFLOAT3(0, 2, 0), XMFLOAT3(0, 0, 0), 0, Pos);

	//TestPointLight = new PointLight(XMFLOAT4(1, .2f, 0, 1), XMFLOAT3(0.1f, 0, 0), XMFLOAT3(0, 0, 0), 0, Pos);
	//TestPointLight = new PointLight(XMFLOAT4(0, 1, 0, 1), XMFLOAT3(0.2f, 0, 0), XMFLOAT3(0, 0, 0), 0, Pos);
	//TestPointLight = new PointLight(XMFLOAT4(0, .2f, .2f, 1), XMFLOAT3(-0.1f, 0, 0), XMFLOAT3(0, 0, 0), 0, Pos);
	TestPointLight = new PointLight(XMFLOAT4(1, 0, 0, 1), XMFLOAT3(-1, 0, 2), XMFLOAT3(0, 0, 0), 0, Pos);
	TestPointLight = new PointLight(XMFLOAT4(0, .2f, 1, 1), XMFLOAT3(1, 0, 0), XMFLOAT3(0, 0, 0), 0, Pos);
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
		if (m_pCurrState->Input() == false)
		{
			GameRunning = false;
		}

		Update(m_timer);

		float elapsedTime = float(m_timer.GetElapsedSeconds());
		m_pCurrState->Input();
		m_pCurrState->Update(elapsedTime);
		m_pCurrState->Render();
		int x = m_timer.GetFramesPerSecond();
	});

}

// Updates the world
void Game::Update(DX::StepTimer const& timer)
{
	float elapsedTime = float(timer.GetElapsedSeconds());
	m_Camera->SetDeltaTime(elapsedTime);
	// TODO: Add your game logic here

	//Test Cam movement
	XMFLOAT3 Movement = XMFLOAT3(0, 0, 0);

	//check keyboard state if any keys
	InputClass::GetInstance()->Frame();
	if (InputClass::GetInstance()->IsEscapePressed())
	{
		GameRunning = false;
	}

	if (InputClass::GetInstance()->IsPgUpPressed())
	{
		m_Camera->SetPosition(m_Camera->GetPosition().x, m_Camera->GetPosition().y + (10)*elapsedTime, m_Camera->GetPosition().z);
	}
	if (InputClass::GetInstance()->IsPgDownPressed())
	{
		m_Camera->SetPosition(m_Camera->GetPosition().x, m_Camera->GetPosition().y - (10)*elapsedTime, m_Camera->GetPosition().z);
	}
	if (InputClass::GetInstance()->IsLeftPressed())
	{
		Movement.x -= (10)*elapsedTime;
		//m_Camera->SetRotation(m_Camera->GetRotation().x, m_Camera->GetRotation().y - (1)*elapsedTime, m_Camera->GetRotation().z);
	}
	if (InputClass::GetInstance()->IsRightPressed())
	{
		Movement.x += (10)*elapsedTime;
		//m_Camera->SetRotation(m_Camera->GetRotation().x, m_Camera->GetRotation().y + (1)*elapsedTime, m_Camera->GetRotation().z);
	}
	if (InputClass::GetInstance()->IsDownPressed())
	{
		Movement.z -= (10)*elapsedTime;
		//m_Camera->SetPosition(m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z - (10)*elapsedTime);
	}
	if (InputClass::GetInstance()->IsUpPressed())
	{
		Movement.z += (10)*elapsedTime;
		//m_Camera->SetPosition(m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z + (10)*elapsedTime);
	}
	m_Camera->Move(Movement);
	//m_Camera->Movement();
	//Camera update
	m_Camera->Render();

	//Updated all moveable object that have been added.
	ObjectManager::GetInstance()->ObjectUpdate(elapsedTime);
	LightManager::Getinstance()->Update(elapsedTime);
	LightManager::Getinstance()->SetCameraPosition(m_Camera->GetPosition());
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
}

//This where all the stuff get renderer but you dont see it yet.
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

	//Rendere SKyBox first.
	//m_SkyBox->Renderer(viewMatrix, projectionMatrix, m_DeferredShader);

	//Loops throguh all game objects
	ObjectManager::GetInstance()->ObjectRenderer(viewMatrix, projectionMatrix, m_DeferredShader);

	// Reset the render target back to the original back buffer and not the render buffers anymore.
	DeferredRenderer::GetInstance()->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	DeferredRenderer::GetInstance()->ResetViewport();
}

// Presents the backbuffer contents to the screen
void Game::Present()
{
	XMMATRIX worldMatrix, ViewMatrix, baseViewMatrix, orthoMatrix, projectionMatrix;

	//
	RenderSceneToTexture();

	//Clear backBuffer
	DeferredRenderer::GetInstance()->ClearScene(1, 0, 0, 1);

	//get world mat
	DeferredRenderer::GetInstance()->GetWorldMatrix(worldMatrix);
	//get view matrix for quad
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	//Get View matrix for lights
	m_Camera->GetViewMatrix(ViewMatrix);
	//Get Projection for lights
	DeferredRenderer::GetInstance()->GetProjectionMatrix(projectionMatrix);

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
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());


	DeferredRenderer::GetInstance()->TurnOnAddBlending();
	// Renderer pointlight and other later
	LightManager::Getinstance()->Renderer(ViewMatrix, projectionMatrix);

	// Turn the Z buffer back on now that all 2D rendering has completed.
	DeferredRenderer::GetInstance()->TurnZBufferOn();

	DeferredRenderer::GetInstance()->TurnOFFaddBlending();

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
	DeferredRenderer::GetInstance()->Initialize((int)w, (int)h, false, m_window, false, SCREEN_DEPTH, SCREEN_NEAR);

	InputClass::GetInstance()->Initialize(hInstance, m_window, (int)w, (int)h);
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
	ChangeState(nullptr);
	ObjectManager::GetInstance()->ShutDown();
	DeferredRenderer::GetInstance()->Shutdown();
	InputClass::GetInstance()->Shutdown();
	LightManager::Getinstance()->ShutDown();
	m_DeferredBuffer->Shutdown();
	m_DeferredShader->Shutdown();
	m_QuadScreen->Shutdown();
	m_LightShader->Shutdown();
	delete m_Light;
	delete m_SkyBox;
	/////////////////////////

}

void Game::ChangeState(BaseGameState* pNewState)
{
	// Exit the old state
	if (m_pCurrState != nullptr)
		m_pCurrState->Exit();

	// Store the new state
	m_pCurrState = pNewState;

	// Enter the new state
	if (m_pCurrState != nullptr)
		m_pCurrState->Enter();
}