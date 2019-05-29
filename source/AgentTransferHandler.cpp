#include <CrowdSim/HlaAllHeaders.h>
#include "AgentTransferHandler.h"
#include "TimeHandler.h"
#include "TileRequestHandler.h"
#include "HlaEntryPoint.h"
#include <CrowdSim/HlaAgentManager.h>

using namespace CrowdSim;

std::map<std::string, HlaAgentPtr> SimAgentListener::simAgentStash;

void AgentTransferHandler::transferAgent(HlaAgentPtr agent, HlaWorldPtr toWorld, bool addValueListener)
{
	HlaAgentPtr a = toWorld->getHlaAgentManager()->createLocalHlaAgent();

	if (addValueListener) {
		a->addHlaAgentValueListener(HlaAgentValueListenerPtr(new AgentValueListener()));
	}

	// Add the discovered agent to the agent stash so we can delete it when it's transferred
	SimAgentListener::simAgentStash.insert(std::make_pair(agent->getName(), a));

	HlaAgentUpdaterPtr updater = a->getHlaAgentUpdater();
	updater->setName(agent->getName());
	updater->setX(agent->getX());
	updater->setY(agent->getY());
	updater->setZ(agent->getZ());
	updater->sendUpdate();

	std::cout << "\nTRANSFERRING AGENT";
}

void BridgeAgentListener::hlaAgentInitialized(HlaAgentPtr agent, HlaTimeStampPtr timeStamp, HlaLogicalTimePtr logicalTime)
{
	// Return if the agent was made by this federate
	if (agent->getProducingFederate()->getFederateName() == HlaEntryPoint::bridgeConn.getWorld()->getFederateId()->getFederateName()) 
	{
		return;
	}

	std::cout << "\nBridge agent found: " << agent->getName() << " X: " << agent->getX() << " Z: " << agent->getZ() << " ";

	// Check if the position of the agent is in our simulation world
	bool posIsInSimWorld = TileHandler::positionIsInGridmap(agent->getX(), agent->getZ(), TileHandler::simGridMap);

	std::cout << "\n" << posIsInSimWorld;

	// Transfer the agent to our world because it's position is in our simulation world
	if (posIsInSimWorld) 
	{
		SimAgentListener::simAgentStash.insert(std::make_pair(agent->getName(), agent));

		std::cout << "Transferring agent to simworld";
		AgentTransferHandler::transferAgent(agent, HlaEntryPoint::simConn.getWorld(), true);
	}
}

void SimAgentListener::hlaAgentInitialized(HlaAgentPtr agent, HlaTimeStampPtr timeStamp, HlaLogicalTimePtr logicalTime)
{
	if (agent->getProducingFederate()->getFederateName() == HlaEntryPoint::simConn.getWorld()->getFederateId()->getFederateName())
	{
		return;
	}

	std::cout << "\nSimulation agent found: " << agent->getName() << " X: " << agent->getX() << " Z: " << agent->getZ() << " ";

	HlaAgentValueListenerPtr valueListener(new AgentValueListener());
	agent->addHlaAgentValueListener(valueListener);
}

void AgentValueListener::xUpdated(HlaAgentPtr agent, double x, bool validOldX, double oldX, HlaTimeStampPtr timeStamp, HlaLogicalTimePtr logicalTime)
{
	// Disable for now to prevent double agents
	//transferIfOutsideOfSim(agent);
}

void AgentValueListener::zUpdated(HlaAgentPtr agent, double z, bool validOldZ, double oldZ, HlaTimeStampPtr timeStamp, HlaLogicalTimePtr logicalTime)
{
	transferIfOutsideOfSim(agent);
}

void AgentValueListener::transferIfOutsideOfSim(HlaAgentPtr agent)
{
	bool posIsInSimWorld = TileHandler::positionIsInGridmap(agent->getX(), agent->getZ(), TileHandler::simGridMap);

	// If the agents position is updated to not be in the simulation world, we need to transfer it
	if (!posIsInSimWorld) {
		AgentTransferHandler::transferAgent(agent, HlaEntryPoint::bridgeConn.getWorld(), false);
	}
}
