#include "GateUser.h"

GateUser::GateUser(const ConnectionPtr& conn) :
    type_(GU_TYPE_NA),
    status_(GU_STATUS_INIT),
    conn_(conn)
{
}
