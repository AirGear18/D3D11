#include "../pch.h"
#include "EventManager.h"

//SignleTon
EventManager* EventManager::GetInstance()
{
	static EventManager s_Instance;	// stored in global memory once
	return &s_Instance;
}

EventManager::EventManager()
{
}


EventManager::~EventManager()
{
}

void EventManager::AddEvent(BaseObject*obj, const char *name)
{
	EventID temp(name);
	auto it = Events.find(obj);
	if (it != Events.end())
	{
		for (size_t i = 0; i < it->second.size(); i++)
		{
			if (it->second[i].ID == temp.ID)
				return;
		}
		it->second.push_back(temp);
	}
	else
	{
		vector<EventID> StartList;
		StartList.push_back(temp);
		Events[obj] = StartList;
	}

}


EventID::EventID(const char *Name)
{
	ID = Name;
}

EventID::~EventID()
{
}