#pragma once
#include <CrowdSim/HlaAllHeaders.h>
#include <CrowdSim/HlaWorld.h>

using namespace CrowdSim;

class ConnectionHandler {

public:
	static bool isPacer;
	ConnectionHandler();
	ConnectionHandler(HlaWorldPtr world);
	HlaWorldPtr getWorld();

private:
	HlaWorldPtr ownWorld;
};

class WorldListener : public HlaWorldListener::Adapter {
public:
	void connected(HlaTimeStampPtr timeStamp);
	void disconnected(HlaTimeStampPtr timeStamp);
};