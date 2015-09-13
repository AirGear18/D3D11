#pragma once
#include "BaseGameState.h"
class MainMenu :
	public BaseGameState
{
	MainMenu() = default;
	~MainMenu() = default;
public:

	static MainMenu* GetInstance();

	void Enter(void) override;	// load resources
	void Exit(void) override;	// unload resources

	bool Input(void) override;	// handle user input
	void Update(float elapsedTime) override;	// update entites
	void Render(void) override;	// render entities / menu
};

