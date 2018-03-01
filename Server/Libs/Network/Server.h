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

    Buffer()
    {
        clearBuffer();
    }

    void clearBuffer()
    {
        //Clear buffer for all values before first use
        bzero(_data, _max);
    }

};

struct Frame
{
    uint16_t _size;
    std::vector<char> _data;

    const char* deliminator = "<EOF>";

    template<typename T>
    void copyDataToFrame(T src)
    {
        _size = sizeof(src)+sizeof(_size);
        _data.resize(_size);
        //copy _size into data
        memcpy(_data.data(), &_size, sizeof(_size));
        //Copy src into data
        memcpy(_data.data() + sizeof(_size), &src, sizeof(src));
    }
};

struct File
{
    std::vector<char> _data;
    std::vector<char>::iterator _it;
    int _fileSize;

    bool loadFile(std::string filePath)
    {
        //check if path in request exist
        std::ifstream file(filePath, std::ifstream::binary);
        file >> std::noskipws;

        //Copy file into vector
        std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), std::back_inserter(_data));
        _it = _data.begin();

        //Get file size
        file.seekg(0, file.end);
        _fileSize = file.tellg();
        return _fileSize > 0 ? true : false;
    }
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



