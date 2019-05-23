#include <CrowdSim/HlaAllHeaders.h>
#include "ConnectionHandler.h"
#include <CrowdSim/HlaWorld.h>
#include "TimeHandler.h"
#include <thread>

bool ConnectionHandler::isPacer = false;

using namespace std;

ConnectionHandler::ConnectionHandler()
{
}

ConnectionHandler::ConnectionHandler(HlaWorldPtr world)
{
	ownWorld = world;
}

HlaWorldPtr ConnectionHandler::getWorld()
{
	return ownWorld;
}

void WorldListener::connected(HlaTimeStampPtr timeStamp)
{
	std::cout << "Connected";

	if (ConnectionHandler::isPacer) 
	{
		TimeHandler::initializePacer();
	}
}

void WorldListener::disconnected(HlaTimeStampPtr timeStamp)
{
	std::cout << "Disconnected";
}
