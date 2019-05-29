#include <CrowdSim/HlaAllHeaders.h>
#include <CrowdSim/HlaInteractionManager.h>
#include <CrowdSim/datatypes/TileStatus.h>
#include "TileRequestHandler.h"
#include "HlaEntryPoint.h"
#include <CrowdSim/HlaGridManager.h>
#include <CrowdSim/datatypes/TileStatus.h>
#include <list>

using namespace CrowdSim;

std::map<std::wstring, HlaGridPtr> TileHandler::bridgeGridMap;
std::map<std::wstring, HlaGridPtr> TileHandler::simGridMap;

/*
Listens to every tile request in the federation. If the status of a tile is unknown. It will handle the request
and find out the actual status.
*/
void BridgeTileRequestListener::tileRequest(bool local, HlaTileRequestParametersPtr parameters, HlaTimeStampPtr timeStamp, HlaLogicalTimePtr logicalTime)
{
	// Return if the tile request was sent by this federate
	if (parameters->getProducingFederate()->getFederateName() == HlaEntryPoint::bridgeConn.getWorld()->getFederateId()->getFederateName()) 
	{
		return;
	}

	std::cout << "\nBridge request received...";
	TileHandler::sendTileRequest(parameters->getX(), parameters->getZ(), parameters->getStatus(), HlaEntryPoint::simConn.getWorld());
}

void SimTileRequestListener::tileRequest(bool local, HlaTileRequestParametersPtr parameters, HlaTimeStampPtr timeStamp, HlaLogicalTimePtr logicalTime)
{
	// Return if the tile request was sent by this federate
	if (parameters->getProducingFederate()->getFederateName() == HlaEntryPoint::simConn.getWorld()->getFederateId()->getFederateName()) 
	{
		return;
	}

	// If the tile doesn't exist, return the request with status occupied
	if (!TileHandler::positionIsInGridmap(parameters->getX(), parameters->getZ(), TileHandler::bridgeGridMap) && parameters->getStatus() == TileStatus::UNKNOWN) 
	{
		std::cout << "\nreturning request...";
		TileHandler::sendTileRequest(parameters->getX(), parameters->getZ(), TileStatus::OCCUPIED, HlaEntryPoint::simConn.getWorld());
		return;
	}

	// Pass the tile request to the bridge world
	TileHandler::sendTileRequest(parameters->getX(), parameters->getZ(), parameters->getStatus(), HlaEntryPoint::bridgeConn.getWorld());
}


bool TileHandler::positionIsInGridmap(int x, int z, std::map<std::wstring, HlaGridPtr> map)
{
	std::map<std::wstring, HlaGridPtr>::iterator it;
	bool isInGrid = false;

	for (it = map.begin(); it != map.end(); it++)
	{
		HlaGridPtr grid = it->second;
		isInGrid = (x < grid->getGridWEnd()) && x >= grid->getGridWStart()
			&& (z < grid->getGridHEnd()) && (z >= grid->getGridHStart()) ;
/*
		std::cout << "\nSim Grid discovered: [" << grid->getGridWStart() << ", " << grid->getGridWEnd() << ", "
			<< grid->getGridHStart() << ", " << grid->getGridHEnd() << " ] X: " << x << " / Z: " << z;*/

		if (isInGrid) 
		{
			return isInGrid;
		}
	}

	return isInGrid;
}

void TileHandler::sendTileRequest(double x, double z, TileStatus::TileStatus status, HlaWorldPtr world)
{
	HlaTileRequestInteractionPtr tileRequest = world->getHlaInteractionManager()->getHlaTileRequestInteraction();
	tileRequest->setStatus(status);
	tileRequest->setX(x);
	tileRequest->setY(0);
	tileRequest->setZ(z);
	tileRequest->sendInteraction();
}

/* 
When a simulation with a grid joins the federation, this stores the grid. This is later used to know wether a tile even
exists, and if so, find its federation.
*/
void SimulationGridListener::hlaGridInitialized(HlaGridPtr grid, HlaTimeStampPtr timeStamp, HlaLogicalTimePtr logicalTime)
{
	std::wstring federateName = grid->getProducingFederate()->getFederateName();

	// If the grid is sent by this federate return from the function
	if (grid->isLocal()) {
		return;
	}

	std::cout << "\nSim Grid discovered: [" << grid->getGridWStart() << ", " << grid->getGridWEnd() << ", " 
		<< grid->getGridHStart() << ", " << grid->getGridHEnd() << " ]";

	TileHandler::simGridMap.insert(std::make_pair(HlaEntryPoint::bridgeConn.getWorld()->getFederateId()->getFederateName(), grid));

	// Send the grid to the bridge federation
	HlaGridPtr bGrid = HlaEntryPoint::bridgeConn.getWorld()->getHlaGridManager()->createLocalHlaGrid();
	HlaGridUpdaterPtr updater = bGrid->getHlaGridUpdater();
	updater->setGridWStart(grid->getGridWStart());
	updater->setGridWEnd(grid->getGridWEnd());
	updater->setGridHStart(grid->getGridHStart());
	updater->setGridHEnd(grid->getGridHEnd());
	updater->sendUpdate();
}

void BridgeGridListener::hlaGridInitialized(HlaGridPtr grid, HlaTimeStampPtr timeStamp, HlaLogicalTimePtr logicalTime)
{
	// If the grid is sent by this federate return from the function
	if (grid->isLocal()) {
		return;
	}

	std::wstring federateName = grid->getProducingFederate()->getFederateName();

	std::cout << "\nBridge Grid discovered: [" << grid->getGridWStart() << ", " << grid->getGridWEnd() << ", "
		<< grid->getGridHStart() << ", " << grid->getGridHEnd() << " ]";

	TileHandler::bridgeGridMap.insert(std::make_pair(federateName, grid));
}