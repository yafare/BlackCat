#include <vector>
#include <string>

#include "DataServer.h"

#include "../ServerLib/CfgReader.h"

static std::vector<std::string> GetConfig()
{
    std::vector<std::string> result;

    CfgReader reader;
    reader.Read("./DataServer.cfg");
    std::string ip = reader["IP"];
    std::string port = reader["Port"];
    if (ip.empty() || port.empty()) {
        printf("Please check the cfg file\n");
        return result;
    }

    result.push_back(ip);
    result.push_back(port);
    return result;
}

int main()
{
    auto cfg = GetConfig();
    if (cfg.empty()) {
        return 0;
    }

    DataServer data_server;
    try {
        data_server.Run(cfg[0], cfg[1]);
    } catch (std::exception& e) {
        printf("exception: %s\n", e.what());
    }
}
