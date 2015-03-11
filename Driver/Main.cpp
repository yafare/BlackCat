#include "Driver.h"

#include <vector>
#include <string>

#include "../ServerLib/CfgReader.h"

static DriverStartupConfig GetConfig()
{
    DriverStartupConfig config;

    CfgReader reader;
    reader.Read("./Gateway.cfg");
    config.service_name = reader["ServiceName"];
    config.rpc_server_addr = reader["RpcProxyServerAddr"];
    config.gateway_ip = reader["GatewayIP"];
    config.gateway_port = reader["GatewayPort"];
    std::string pool_size = reader["IoServicePoolSize"];
    if (config.service_name.empty() || config.rpc_server_addr.empty() ||
        config.gateway_ip.empty() || config.gateway_port.empty()) {
        printf("Please check cfg file\n");
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
    driver = new Driver;

    auto cfg = GetConfig();
    try {
        driver->Run(cfg);
    } catch (std::exception& e) {
        LOG("exception: %s", e.what());
    }
}
