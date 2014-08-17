#include "../ServerLib/TcpServer.h"

#include "IoDispatcher.h"

int main()
{
    try {
        // Initialise the server.
        IoDispatcher dispatcher;
        server.reset(new TcpServer("0.0.0.0", "8992", 1,
            dispatcher.GetAcceptCallBack()));

        server->SetCallBacks(dispatcher.GetCallBacks());
        server->Run();
    } catch (std::exception& e) {
        LOG("exception: %s", e.what());
    }
}
