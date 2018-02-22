#include "Client.h"


Client::Client()
{
    server = nullptr;
    socketfd = -1;

}

// Client::Client(char *ip, int portno)
// {

// }

Client::~Client()
{

}

void Client::connectTo(const char *ip, int port)
{
    //Create socket
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    
    server = gethostbyname(ip);
    serv_addr.sin_family = AF_INET;
    if(server == NULL)
    {
       Debug::error("Error, no such host exist");
    }

    bcopy((char *)server->h_addr,
            (char *)&serv_addr.sin_addr.s_addr,
            server->h_length);
    serv_addr.sin_port = htons(port);
    //Connect to server
    if(connect(socketfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        Debug::error("Error, connecting to file server");
    }
}

void Client::requestFile(int &fileSizeOut, std::string fileName)
{
    //Size of the frame to be received, not including the framesize at the start of recv
    uint16_t frameSize = 0;//Needs to be uint on the server too!
    //Request file
    int err = send(socketfd, fileName.c_str(), fileName.length(), 0);
    if(err < 0)
        Debug::error("Error, failed to send request to server");
    //Recv file size, if 0 file doesn't exist
    int size = recv(socketfd, buff, 1000, 0);
    if(size<0)
        Debug::error("Error, couldn't recv from server");
    //Calc the frameSize from 2 first received bytes
    frameSize = (buff[0]<<8) |(buff[1]);
    //Allocate another buffer to hold the current frame
    
    if(frameSize == size-2) //Everything was received in first recv
    {
        fileSizeOut = (buff[2]<<24)|(buff[3]<<16)|(buff[4]<<8)|(buff[5]);
        return;
    }
    char *result = new char[frameSize];
    int bytesMoved = 0;
    bytesMoved = size-2;
    std::copy(buff+2, buff+bytesMoved,result);
    bzero(buff, size);
    while(frameSize > bytesMoved)
    {
        size = recv(socketfd, buff, 1000, 0);
        std::copy(buff, buff+size,result);
        bytesMoved += size;
        bzero(buff, size);
    }
    fileSizeOut = (result[0]<<24)|(result[1]<<16)|(result[2]<<8)|(result[3]);
   
}

void Client::receiveFile(std::vector<char> &dataOut, std::string fileName)
{

}

