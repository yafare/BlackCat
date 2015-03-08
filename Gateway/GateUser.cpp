#include "GateUser.h"

GateUser::GateUser(const ConnectionPtr& conn) :
    status_(GU_STATUS_INIT),
    conn_(conn)
{
}
