#include "DataServer.h"

#include "../ServerLib/TcpServer.h"
#include "../ServerLib/TcpConnection.h"

DataServer::DataServer()
{
}

void DataServer::Dispatch(const ConnectionPtr& conn, const uint8 *buf, int32 len)
{
}
