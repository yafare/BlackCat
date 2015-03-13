#include "RpcProxyServer.h"

#include <vector>
#include <string>

static std::vector<std::string> GetConfig()
{
    return{ "0.0.0.0", "10001" };
}

int main()
{
    RpcProxyServer serv;

    auto cfg = GetConfig();
    if (cfg.empty()) {
        return 0;
    }

    try {
        serv.Run(cfg[0], cfg[1]);
    } catch (std::exception& e) {
        LOG("exception: %s", e.what());
    }
}
