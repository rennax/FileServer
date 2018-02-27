#include "Error.h"


namespace Debug
{
    void error(const char *msg)
    {
        perror(msg);
        std::cin.get();
        exit(1);
    }
};