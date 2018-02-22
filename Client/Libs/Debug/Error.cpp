#include "Error.h"



void Debug::error(const char *msg)
{
    perror(msg);
    exit(1);
};