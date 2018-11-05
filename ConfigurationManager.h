#pragma once

#include <string>
#include <vector>
#include <map>
#include <tinyxml2.h>
#include "ApplicationException.h"

namespace FW
{
    class ConfigurationManager
    {
    public:
        typedef std::map<std::string, std::map<std::string, std::string>> ConfigurationMap; // map used to store config data

    public:
        ConfigurationManager(const std::string& filename);
        void write();
        ConfigurationMap& getConfigurationMap();

    private:
        std::string filename;
        ConfigurationMap configuration;
        std::vector<std::string> catagory_order;
        std::vector<std::string> setting_order;
    };
}