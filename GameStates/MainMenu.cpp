#include "../pch.h"
#include "MainMenu.h"


MainMenu* MainMenu::GetInstance()
{
	static MainMenu s_Instance;

	return &s_Instance;
}

// load resources
void MainMenu::Enter()
{

}
// unload resources
void MainMenu::Exit()
{

}
// handle user input
bool MainMenu::Input()
{
	return true;
}
// update entites
void MainMenu::Update(float elapsedTime)
{

}
// render entities / menu
void MainMenu::Render()
{

}