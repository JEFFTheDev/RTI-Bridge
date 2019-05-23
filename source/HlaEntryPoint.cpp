#include <CrowdSim/HlaAllHeaders.h>
#include "HlaEntryPoint.h"
#include <CrowdSim/HlaWorld.h>
#include <CrowdSim/HlaAgentManager.h>
#include "ConfigLoader.h"
#include <CrowdSim/HlaInteractionListener.h>
#include <CrowdSim/HlaInteractionManager.h>
#include <CrowdSim/HlaGridManager.h>
#include "TileRequestHandler.h"
#include "TimeHandler.h"
#include <thread>
#include <CrowdSim/HlaSettings.h>

using namespace CrowdSim;

ConnectionHandler HlaEntryPoint::simConn;
ConnectionHandler HlaEntryPoint::bridgeConn;

int main() throw(HlaException)
{
	ConnectionHandler::isPacer = false;

	// Intialize the bridge- and simulation world
	HlaWorldPtr simWorld = HlaEntryPoint::createWorld(L"localhost", 8987);
	HlaWorldPtr bridgeWorld = HlaEntryPoint::createWorld(L"localhost", 8989);
	
	/* 
	Give both worlds a tile requests listener because tile requests have to be passed between the 
	bridge- and simulation world
	*/
	HlaInteractionListenerPtr tileRequestListener(new TileRequestListener());
	simWorld->getHlaInteractionManager()->addHlaInteractionListener(tileRequestListener);
	bridgeWorld->getHlaInteractionManager()->addHlaInteractionListener(tileRequestListener);

	HlaGridManagerListenerPtr gridM(new GridListener());
	simWorld->getHlaGridManager()->addHlaGridManagerListener(gridM);
	
	HlaWorldListenerPtr worldL(new WorldListener());
	//simWorld->addHlaWorldListener(worldL);
	bridgeWorld->addHlaWorldListener(worldL);
	
	/* 
	The timeGrantListener listens to signals from federates and checks if all federates have reported back
	from since the last time tick
	*/
	HlaInteractionListenerPtr timeGrantListener(new TimeGrantListener());
	simWorld->getHlaInteractionManager()->addHlaInteractionListener(timeGrantListener);

	/* 
	Add a bridgeTimeGrantListener which listens to time ticks in the bridge world sent by the pacer
	*/
	HlaInteractionListenerPtr bridgeTimeGrantListener(new BridgeTimeGrantListener());
	bridgeWorld->getHlaInteractionManager()->addHlaInteractionListener(bridgeTimeGrantListener);
	
	// We only need a timegrantlistener in the bridgeworld if this federate is the pacer
	if(ConnectionHandler::isPacer)
	{
		bridgeWorld->getHlaInteractionManager()->addHlaInteractionListener(timeGrantListener);
	}

	// Create connectionhandlers for both worlds so we can acces them in other scripts
	HlaEntryPoint::simConn = ConnectionHandler(simWorld);
	HlaEntryPoint::bridgeConn = ConnectionHandler(bridgeWorld);
	
	try {
		simWorld->connect();
		bridgeWorld->connect();
	}
	catch (HlaException e) {
		throw;
	}

	// Make sure the program doesn't exit
	std::getchar();
}

HlaWorldPtr HlaEntryPoint::createWorld(const std::wstring& crcHost, int crcPort) 
{
	HlaSettingsPtr settings(new HlaConfiguration(crcHost, crcPort));
	HlaWorldPtr hlaWorld = HlaWorld::Factory::create(false, settings);

	return hlaWorld;
}