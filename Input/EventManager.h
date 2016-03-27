#pragma once
#include <map>
#include <vector>
using namespace std;


class EventID
{
public:
	EventID(const char *);
	~EventID();
	std::string ID;
private:


};


class BaseObject;
class EventManager
{
	map<BaseObject*, vector<EventID>> Events;
public:
	EventManager();
	~EventManager();

	//Singletion
	static EventManager *GetInstance();
	void AddEvent(BaseObject*, const char *);

};



