#include <CrowdSim/HlaAllHeaders.h>
#include "ConfigLoader.h"
#include <fstream>

ConfigLoader::ConfigLoader(const std::string & configFile)
{
	this->configFile = configFile;
}

void ConfigLoader::configureFederate()
{
	readConfigFile();

	try 
	{

	}
	catch (exception e){

	}
}

void ConfigLoader::readConfigFile()
{
	std::ifstream file;
	file.open(configFile);

	std::string line;

	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::vector<std::string> results;
		boost::split(results, line, [](char c) { return c == '='; });
		properties.insert(std::make_pair(results[0], results[1]));

		/*for (std::string n : results) {
			std::cout << n << '\n';
			
		}*/

		//int a, b;
		//if (!(iss >> a >> b)) { break; } 				 
	}

	file.close();
}