#pragma once
#include <CrowdSim/HlaAllHeaders.h>
#include <CrowdSim/HlaAgent.h>
#include <CrowdSim/HlaAgentManagerListener.h>

using namespace CrowdSim;

static class AgentTransferHandler {
public:
	static void transferAgent(HlaAgentPtr, HlaWorldPtr, bool);
};

class BridgeAgentListener : public HlaAgentManagerListener::Adapter {
public:
	void hlaAgentInitialized(HlaAgentPtr agent, HlaTimeStampPtr timeStamp, HlaLogicalTimePtr logicalTime);
};

class SimAgentListener : public HlaAgentManagerListener::Adapter {
public:
	void hlaAgentInitialized(HlaAgentPtr agent, HlaTimeStampPtr timeStamp, HlaLogicalTimePtr logicalTime);
	static std::map<std::string, HlaAgentPtr> simAgentStash;
};

class AgentValueListener : public HlaAgentValueListener::Adapter {
public:
	void xUpdated(HlaAgentPtr agent, double x, bool validOldX, double oldX, HlaTimeStampPtr timeStamp, HlaLogicalTimePtr logicalTime);
	void zUpdated(HlaAgentPtr agent, double z, bool validOldZ, double oldZ, HlaTimeStampPtr timeStamp, HlaLogicalTimePtr logicalTime);
	void transferIfOutsideOfSim(HlaAgentPtr);
};