#include "Driver.h"

#include <vector>
#include <string>

#include "../ServerLib/CfgReader.h"

static std::vector<std::string> GetConfig()
{
    std::vector<std::string> result;

    CfgReader reader;
    reader.Read("./Driver.cfg");
    std::string ip = reader["GatewayIP"];
    std::string port = reader["GatewayPort"];
    if (ip.empty() || port.empty()) {
        LOG("Please check Driver.cfg!");
        return result;
    }

    result.push_back(ip);
    result.push_back(port);
    return result;
}

int main()
{
    driver = new Driver;

    auto cfg = GetConfig();
    if (cfg.empty()) {
        return 0;
    }

    try {
        driver->Run(cfg[0], cfg[1]);
    } catch (std::exception& e) {
        LOG("exception: %s", e.what());
    }
}
