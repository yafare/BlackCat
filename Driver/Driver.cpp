#include "../ServerLib/TcpServer.h"
#include "../ServerLib/SessionMgr.h"

#include "GameScript.h"
#include "IoDispatcher.h"

int main()
{
    try {
        // Init script env
        game_script.reset(new GameScript);
        game_script->Init();

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
