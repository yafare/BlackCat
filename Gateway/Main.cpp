#include "Gateway.h"

#include <vector>
#include <string>

#include "../ServerLib/CfgReader.h"

static GatewayStartupConfig GetConfig()
{
    GatewayStartupConfig config;

    CfgReader reader;
    reader.Read("./Gateway.cfg");
    config.service_name = reader["ServiceName"];
    config.rpc_server_addr = reader["RpcProxyServerAddr"];
    config.ip = reader["IP"];
    config.port = reader["Port"];
    std::string pool_size = reader["IoServicePoolSize"];
    if (config.service_name.empty() || config.rpc_server_addr.empty() ||
        config.ip.empty() || config.port.empty()) {
        printf("Please check Gateway.cfg");
        return config;
    }

    if (pool_size.empty()) {
        pool_size = "1";
    }

    config.pool_size = atoi(pool_size.c_str());
    return config;
}

int main()
{
    Gateway gate;

    auto cfg = GetConfig();
    try {
        gate.Run(cfg);
    } catch (std::exception& e) {
        LOG("exception: %s", e.what());
    }
}
