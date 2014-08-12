#include "../ServerLib/TcpServer.h"

#include "GameScript.h"
#include "IoDispatcher.h"

int main()
{
    try {
        // Initialise the server.
        IoDispatcher dispatcher;
        server.reset(new TcpServer("0.0.0.0", "8992", 1,
            dispatcher.GetAcceptCallBack()));

        // Init script env
        game_script.reset(new GameScript);
        game_script->Init();

        server->SetCallBacks(dispatcher.GetCallBacks());
        server->Run();
    } catch (std::exception& e) {
        std::cerr << "exception: " << e.what() << "\n";
        logger_->Log("exception: %s", e.what());
    }
}
