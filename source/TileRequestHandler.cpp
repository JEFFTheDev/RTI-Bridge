#include <CrowdSim/HlaAllHeaders.h>
#include <CrowdSim/HlaInteractionManager.h>
#include <CrowdSim/datatypes/TileStatus.h>
#include "TileRequestHandler.h"
#include <list>

// TODO use vector for this or a map
std::list<HlaGridPtr> grids;
std::list<HlaGridPtr>::iterator it = grids.begin();

/*
Listens to every tile request in the federation. If the status of a tile is unknown. It will handle the request
and find out the actual status.
*/
void TileRequestListener::tileRequest(bool local, HlaTileRequestParametersPtr parameters, HlaTimeStampPtr timeStamp, HlaLogicalTimePtr logicalTime)
{
	std::cout << "\nTile requested: [" << parameters->getX() << " ," << parameters->getY() << ", " 
		<< parameters->getZ() << "] Status: " + std::to_string(parameters->getStatus());

	if (parameters->getStatus() == TileStatus::UNKNOWN) {
		/* 
		The status of the tile is unknown. Now we need to find out wether the tile is occupied or available. The tile is (in most cases)
		in another federation. We ask the other federation what the status of this tile is and then return a tile request into our own federation
		with the now discovered status.
		*/
		TileHandler::handleTileRequest(parameters);
	}
}

/* 
When a simulation with a grid joins the federation, this stores the grid. This is later used to know wether a tile even
exists, and if so, find its federation.
*/
void GridListener::hlaGridInitialized(HlaGridPtr grid, HlaTimeStampPtr timeStamp, HlaLogicalTimePtr logicalTime)
{
	std::cout << "\nGrid discovered: [" << grid->getGridWStart() << ", " << grid->getGridWEnd() << ", " 
		<< grid->getGridHStart() << ", " << grid->getGridHEnd() << " ]";
}

// Finds the status of the given tile 
void TileHandler::handleTileRequest(HlaTileRequestParametersPtr paramaters)
{

}

bool isInGrid(int x, HlaGridPtr grid) {
	return false;
}
