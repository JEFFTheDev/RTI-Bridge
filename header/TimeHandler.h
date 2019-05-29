#pragma once
#include <CrowdSim/HlaAllHeaders.h>
#include <CrowdSim/HlaInteractionListener.h>
#include <CrowdSim/datatypes/TimeType.h>
#include <thread>

using namespace CrowdSim;

static class TimeHandler {
public:
	static void sendTimeGrantToFederates(TimeType::TimeType, HlaWorldPtr, bool);
	static void initializePacer();
	static void pause();
	static void resume();
	static void stop();
	static bool isPacing;
	static TimeHandler pacer;
	static int timeSpeed;
	void operator()() const;
	static std::thread pacerThread;
	static bool isInSimWorld(std::wstring);
};

class TimeGrantListener : public HlaInteractionListener::Adapter {
public:
	void timeGrant(
		bool local,
		HlaTimeGrantParametersPtr parameters,
		HlaTimeStampPtr timeStamp,
		HlaLogicalTimePtr logicalTime
	);
};

class BridgeTimeGrantListener : public HlaInteractionListener::Adapter {
public:
	void timeGrant(
		bool local,
		HlaTimeGrantParametersPtr parameters,
		HlaTimeStampPtr timeStamp,
		HlaLogicalTimePtr logicalTime
	);
};