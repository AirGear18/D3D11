#pragma once

//	- abstract base class!
//	- declares interface for game state
class BaseGameState
{
public:
	BaseGameState() = default;
	~BaseGameState() = default;

	virtual void	Enter(void) = 0;	// load resources
	virtual void	Exit(void) = 0;	// unload resources

	virtual bool	Input(void) = 0;	// handle user input
	virtual void	Update(float elapsedTime) = 0;	// update entites
	virtual void	Render(void) = 0;	// render entities / menu

};

