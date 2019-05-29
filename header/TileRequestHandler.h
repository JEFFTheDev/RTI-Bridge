#pragma once
#include <CrowdSim/HlaAllHeaders.h>
#include <CrowdSim/HlaInteractionListener.h>
#include <CrowdSim/HlaGridManagerListener.h>

using namespace CrowdSim;

class TileHandler {
public:
	static bool positionIsInGridmap(int, int, std::map<std::wstring, HlaGridPtr>);
	static void sendTileRequest(double x, double z, TileStatus::TileStatus, HlaWorldPtr world);
	static std::map<std::wstring, HlaGridPtr> bridgeGridMap;
	static std::map<std::wstring, HlaGridPtr> simGridMap;

	// All the known grids in the distributed simulation
	//static map<HlaFederateIdPtr, HlaGrid> knownGridFederates;
};

class BridgeTileRequestListener : public HlaInteractionListener::Adapter {
public:
	// Is called when a tilerequest interaction is published into the federation
	void tileRequest(bool local,
		HlaTileRequestParametersPtr parameters,
		HlaTimeStampPtr timeStamp,
		HlaLogicalTimePtr logicalTime) override;
};

class SimTileRequestListener : public HlaInteractionListener::Adapter {
public:
	void tileRequest(bool local,
		HlaTileRequestParametersPtr parameters,
		HlaTimeStampPtr timeStamp,
		HlaLogicalTimePtr logicalTime) override;
};

class SimulationGridListener : public HlaGridManagerListener::Adapter {
public:
	// Is called whenever a grid with all its vars set is published into the federation
	void hlaGridInitialized(HlaGridPtr grid, HlaTimeStampPtr timeStamp, HlaLogicalTimePtr logicalTime) override;
};

class BridgeGridListener : public HlaGridManagerListener::Adapter {
public:
	// Is called whenever a grid with all its vars set is published into the federation
	void hlaGridInitialized(HlaGridPtr grid, HlaTimeStampPtr timeStamp, HlaLogicalTimePtr logicalTime) override;
};