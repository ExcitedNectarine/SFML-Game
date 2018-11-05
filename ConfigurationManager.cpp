#include "ConfigurationManager.h"

namespace FW
{
    ConfigurationManager::ConfigurationManager(const std::string& filename)
        : filename(filename)
    {
        using namespace tinyxml2;

        XMLDocument doc;
        if (doc.LoadFile(filename.c_str()) != XML_SUCCESS)
            throw ApplicationException("Configuration file '" + filename + "' could not be opened. " + doc.ErrorName());

        for (XMLNode* catagory_node = doc.FirstChild(); catagory_node; catagory_node = catagory_node->NextSibling())
        {
            std::string catagory_name = catagory_node->ToElement()->Name();
            catagory_order.push_back(catagory_name);

            for (XMLNode* setting_node = catagory_node->FirstChild(); setting_node; setting_node = setting_node->NextSibling())
            {
                std::string setting_name = setting_node->ToElement()->Name();
                setting_order.push_back(setting_name);

                std::string setting = setting_node->ToElement()->GetText();
                configuration[catagory_name][setting_name] = setting;
            }
        }
    }

    void ConfigurationManager::write()
    {
        using namespace tinyxml2;

        XMLDocument doc;
        XMLElement* catagory_element;
        XMLElement* setting_element;

        for (std::string& catagory : catagory_order)
        {
            catagory_element = doc.NewElement(catagory.c_str());

            for (std::string& setting : setting_order)
            {
                if (configuration[catagory].find(setting) != configuration.at(catagory).end())
                {
                    setting_element = doc.NewElement(setting.c_str());
                    setting_element->SetText(configuration[catagory][setting].c_str());
                    catagory_element->LinkEndChild(setting_element);
                }
            }

            doc.LinkEndChild(catagory_element);
        }

        doc.SaveFile(filename.c_str());
    }

    ConfigurationManager::ConfigurationMap& ConfigurationManager::getConfigurationMap()
    {
        return configuration;
    }
}