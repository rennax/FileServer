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


struct Frame
{
    uint16_t _size;

    std::vector<char> _data;

    void copyStringToFrame(std::string str)
    {
        _size = str.length()+ sizeof(_size);
        _data.resize(_size);
        _size -= sizeof(_size);
        //copy _size into data
        memcpy(_data.data(), &_size, sizeof(_size));
        //Copy src into data
        memcpy(_data.data() + sizeof(_size), str.data(), str.length());
    }

    void copyBufferToFrame(char *buffer, size_t size)
    {
        std::copy(buffer+2, buffer+size, std::back_inserter(_data));
    }
};

class Client
{
public:
    Client();
    //Client(char *ip, int portno);
    ~Client();
    void connectTo(const char *ip, int port);//Should be used if client is constructed with default constructor
    void requestFile(int &fileSizeOut, std::string fileName);
    void requestFile(int &fileSizeOut, struct Frame fileName);
    void receiveFile(std::vector<char> &dataOut, std::string fileName);
private:
    char buff[1000];
    int socketfd;
    struct hostent *server;
    struct sockaddr_in serv_addr;
};