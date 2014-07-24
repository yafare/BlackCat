#include "../ServerLib/TcpServer.h"
#include "../ServerLib/SessionMgr.h"

#include "IoDispatcher.h"

int main()
{
    try {
        // Initialise the server.
        server.reset(new TcpServer("0.0.0.0", "8992", 1,
            [=](ConnectionPtr conn) { GetSessionMgr().Add(conn); }));

        IoDispatcher dispatcher;
        server->SetCallBacks(dispatcher.GetCallBacks());
        server->Run();
    } catch (std::exception& e) {
        std::cerr << "exception: " << e.what() << "\n";
        logger_->Log("exception: %s", e.what());
    }
}
