#include "Error.h"


namespace Debug
{
    void error(const char *msg)
    {
        perror(msg);
        exit(1);
    }
};