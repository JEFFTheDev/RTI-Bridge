#pragma once
#include <CrowdSim/HlaAllHeaders.h>
#include "ConnectionHandler.h"

class HlaEntryPoint {
private:
	HlaEntryPoint() {};
public:
	static HlaWorldPtr createWorld(const std::wstring&, int);
	static ConnectionHandler simConn;
	static ConnectionHandler bridgeConn;
};