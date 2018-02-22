#pragma once
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <vector>
#include <iterator>
#include <fstream>
#include <iostream>
#include "Debug/Error.h"

struct Client
{
    struct sockaddr_in _addr;
    socklen_t _clilen;
    int _socketfd;
    Client()
    {
        _clilen = sizeof(_addr);
    }
};

struct Buffer
{
    char _data[1024];
    size_t _size = 0;
    size_t _max = 1024;
};


class Server
{
public:
    Server(int portno = 10000);
    ~Server();
    void run();
    void sendString(struct Client client, std::string &str);
    //void sendData(std::vector<char> &payload);
    void receiveData(struct Client client);
private:
    struct sockaddr_in _addr;
    struct Buffer _buffer;
    struct Client _client;
    int _socketfd;
    int _portno;
};



