#include "DataFrame.h"


DataFrame::DataFrame(bool usedToRecv)
{
    _frameSize = 0;
    _recvDataSize = 0;
    _usedToRecv = usedToRecv;
}

void DataFrame::setDataInFrame(const char *buffer, size_t size)
{
    if(_usedToRecv)
    {
        std::cout << "this frame is used only to recv, create new frame with " << std::endl 
                    << "usedToRecv = false in the constructor" << std::endl; 
        return;
    }
    _frameSize = size + sizeof(_frameSize);
    std::cout << "Framesize: " << _frameSize << std::endl;
    _data.resize(_frameSize);
    //copy framesize into data
    //Copy bytes of framesize into _data
    memcpy(_data.data(), &_frameSize, sizeof(_frameSize));
    //Copy buffer into _data
    memcpy(_data.data()+sizeof(_frameSize), buffer, size);
}



bool DataFrame::getDataFromFrame(std::vector<char> &outBuffer)
{
    //The first 2 bytes are just size of the buffer
    //if the framesizr isn't the size of data dont return anything yet
    if(_data.size() == _frameSize - sizeof(uint32_t))
    {
        std::copy(_data.begin(), _data.end(), std::back_inserter(outBuffer));
        return true;
    }
    else
        return false;
}


void DataFrame::setFrameSize(char *buffer)
{
    //set framesize
    memcpy(&_frameSize, buffer, sizeof(_frameSize));
}

void DataFrame::sendData(int sockfd)
{
    uint32_t sent = 0;
    int err;
    while(sent < _frameSize)
    {
        int toSend = (_frameSize - sent >= 1000 ? 1000 : _frameSize - sent);
        err = send(sockfd, _data.data() + sent, toSend , 0);
        if(err < 0)
        {
            std::cout << "something went wrong with sending to server" << std::endl;
            return; 
        }
        sent += toSend;
    }
}

bool DataFrame::recvData(int sockfd)
{
    char buff[1000];
    bzero(buff, 1000);
    int size = recv(sockfd, buff, 1000, 0);
    //First recv framesize
    while(size < 4)
    {
        size = recv(sockfd, buff+size, 1000-size, 0);
    }
    setFrameSize(buff);

    std::cout << "Framesize of current recv " << _frameSize << std::endl;

    //Copy remaining data into vector
    std::copy(buff+4, buff+size, std::back_inserter(_data));

    _recvDataSize += size;

    while(_recvDataSize < _frameSize)
    {
        bzero(buff, 1000);//clear buffer
        if(_frameSize - _recvDataSize >= 1000)
        {
            size = recv(sockfd, buff, 1000, 0);
            if(size < 0)
                return false;
        }
        else
        {
            size = recv(sockfd, buff, _frameSize - _recvDataSize, 0);
            if(size < 0)
                return false;
            
        }
        //Copy received data to _data, from buff
        std::copy(buff, buff+size, std::back_inserter(_data));
        _recvDataSize += size;
        std::cout << "Recieved " << size << " bytes, still remaining " 
                    << _recvDataSize << "/"<<_frameSize << std::endl;
    }
    return true;
}