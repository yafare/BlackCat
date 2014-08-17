#include "Driver.h"

int main()
{
    try {
        Driver driver;
        driver.Run();
    } catch (std::exception& e) {
        LOG("exception: %s", e.what());
    }
}
