#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <iterator>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

class DataFrame
{
public:
    DataFrame(bool usedToRecv = true);
    //Sets data in the frame, only use for sending frames
    void setDataInFrame(const char *buffer, size_t size);
    //buffer to push received data to. See impl for more details
    bool getDataFromFrame(std::vector<char> &outBuffer);
    //Receives a frame from sockfd
    bool recvData(int sockfd);
    //Send data set in "setDataInFrame"
    void sendData(int sockfd);
private:
    void setFrameSize(char *buffer);
    uint32_t _frameSize;
    uint32_t _recvDataSize;
    bool _usedToRecv;
    std::vector<char> _data;
};
