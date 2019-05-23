#pragma once
#include <CrowdSim/HlaAllHeaders.h>
#include <CrowdSim/HlaSettings.h>
#include <iostream>
#include <map>

using namespace CrowdSim;

using namespace std;

class ConfigLoader{
	public:
		ConfigLoader(const std::string& configFile);
		void configureFederate();

	private:
		string configFile;
		map<string, string> properties;
		void readConfigFile();
};

class HlaConfiguration : public HlaSettings {
public:
	HlaConfiguration(const std::wstring& crcHost, int crcPort) : HlaSettings(crcHost, crcPort) {
		_federateName = L"RTI Bridge 2";
	}
};