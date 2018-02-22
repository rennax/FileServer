#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include "Debug/Error.h"

class Client
{
public:
    Client();
    //Client(char *ip, int portno);
    ~Client();
    void connectTo(const char *ip, int port);//Should be used if client is constructed with default constructor
    void requestFile(int &fileSizeOut, std::string fileName);
    void receiveFile(std::vector<char> &dataOut, std::string fileName);
private:
    char buff[1000];
    int socketfd;
    struct hostent *server;
    struct sockaddr_in serv_addr;
};