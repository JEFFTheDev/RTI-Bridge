#include <CrowdSim/HlaAllHeaders.h>
#include <CrowdSim/HlaInteractionManager.h>
#include <CrowdSim/HlaWorld.h>
#include <CrowdSim/datatypes/TimeType.h>
#include <ctime>
#include "TimeHandler.h"
#include "HlaEntryPoint.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>
#include <iostream>
#include <windows.h>
#include <map>
#include <CrowdSim/HlaFederateId.h>

using namespace boost;
using namespace CrowdSim;

std::map<std::wstring, bool> federateTimeMap;
bool TimeHandler::isPacing;
int TimeHandler::timeSpeed;
std::thread TimeHandler::pacerThread;
TimeHandler TimeHandler::pacer;


void TimeGrantListener::timeGrant(bool local, HlaTimeGrantParametersPtr parameters, HlaTimeStampPtr timeStamp, HlaLogicalTimePtr logicalTime)
{
	std::wstring federateName = parameters->getProducingFederate()->getFederateName();

	/*
	Make sure we ignore ourselves
	*/
	if (federateName == HlaEntryPoint::simConn.getWorld()->getFederateId()->getFederateName())
	{
		return;
	}

	std::map<std::wstring, bool>::iterator it = federateTimeMap.find(federateName);

	if (it == federateTimeMap.end())
	{
		// Federate is not in our time map yet, so add it
		federateTimeMap.insert(std::make_pair(federateName, parameters->getReady()));

		std::cout << "\nNew federate discovered...";
	}
	else
	{
		federateTimeMap[it->first] = parameters->getReady();
		//std::cout << "\nCallback received from federate...";
	}

	/*
	Check if all federates in the map have returned true from last time tick.
	Otherwise return from the function
	*/
	for (it = federateTimeMap.begin(); it != federateTimeMap.end(); it++)
	{
		// If one federate hasn't returned a time tick with true, return from the function
		if (it->second == false) {
			return;
		}
	}

	// Set all booleans in the time map to false for the next time tick
	for (it = federateTimeMap.begin(); it != federateTimeMap.end(); it++) 
	{
		federateTimeMap[it->first] = false;
	}

	if (ConnectionHandler::isPacer) 
	{
		// Allow pacer to continue if this federate is the pacer of the bridge world
		TimeHandler::resume();
	}
	else 
	{
		/*
		Let the pacer in the bridge world know we're done with this time phase because all 
		federates have reported back since the last time tick.
		*/
		TimeHandler::sendTimeGrantToFederates(TimeType::TimeType::PREPARE, HlaEntryPoint::bridgeConn.getWorld(), true);
	}
}

void BridgeTimeGrantListener::timeGrant(bool local, HlaTimeGrantParametersPtr parameters, HlaTimeStampPtr timeStamp, HlaLogicalTimePtr logicalTime)
{
	/*
	Check If ready is set to true, it's not a new time tick sent by the pacer so we can ignore it
	*/
	if (parameters->getReady()) {
		return;
	}
	
	/* 
	The pacer has allowed us to continue to the next phase, so send the time grant
	to the federates in the simulation world
	*/
	TimeHandler::sendTimeGrantToFederates(parameters->getType(), HlaEntryPoint::simConn.getWorld(), false);
}

/*
Sends the given timegrant to all the connected federates in our simulation federation
*/
void TimeHandler::sendTimeGrantToFederates(TimeType::TimeType phase, HlaWorldPtr world, bool ready)
{
	// Configure the interaction and send it to the federation this bridge is responsible for
	HlaTimeGrantInteractionPtr grantTime = world->getHlaInteractionManager()->getHlaTimeGrantInteraction();
	grantTime->setType(phase);
	grantTime->setReady(ready);
	grantTime->sendInteraction();
}

void TimeHandler::initializePacer()
{
	timeSpeed = 333;
	isPacing = true;
	pacer = (TimeHandler());
	pacerThread = std::thread(pacer);
}

void TimeHandler::pause()
{
	SuspendThread(pacerThread.native_handle());
}

void TimeHandler::resume()
{
	ResumeThread(pacerThread.native_handle());
}

void TimeHandler::stop()
{
	isPacing = false;

	// Attach the pacer thread to the main thread
	pacerThread.join();
}

void TimeHandler::operator()() const
{
	isPacing = true;

	while (isPacing)
	{
		for (int phase = TimeType::PREPARE; phase < TimeType::COMPLETE + 1; phase++)
		{
			TimeType::TimeType type = static_cast<TimeType::TimeType>(phase);
			//std::cout << "\nPacer: advancing time to phase " + std::to_string(type);
			Sleep(timeSpeed);
			sendTimeGrantToFederates(type, HlaEntryPoint::bridgeConn.getWorld(), false);

			/*
			New time grants have been sent, pacer can't continue sending ticks until each federate
			has reported back. So it will pause itself
			*/
			pause();
		}
	}
}

bool TimeHandler::isInSimWorld(std::wstring federateName) 
{
	std::map<std::wstring, bool>::iterator it = federateTimeMap.find(federateName);

	return it == federateTimeMap.end();
}
