#pragma once
#include <CrowdSim/HlaAllHeaders.h>
#include <CrowdSim/HlaInteractionListener.h>
#include <CrowdSim/HlaGridManagerListener.h>

using namespace CrowdSim;

class TileHandler {
public:
	static void handleTileRequest(HlaTileRequestParametersPtr paramaters);

	// All the known grids in the distributed simulation
	//static map<HlaFederateIdPtr, HlaGrid> knownGridFederates;
};

class TileRequestListener : public HlaInteractionListener::Adapter {
public:
	// Is called when a tilerequest interaction is published into the federation
	void tileRequest(bool local,
		HlaTileRequestParametersPtr parameters,
		HlaTimeStampPtr timeStamp,
		HlaLogicalTimePtr logicalTime) override;
};

class GridListener : public HlaGridManagerListener::Adapter {
public:
	// Is called whenever a grid with all its vars set is published into the federation
	void hlaGridInitialized(HlaGridPtr grid, HlaTimeStampPtr timeStamp, HlaLogicalTimePtr logicalTime) override;
};