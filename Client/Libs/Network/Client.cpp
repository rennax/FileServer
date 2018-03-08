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


void Client::requestFile(DataFrame fileName)
{
    //Size of the frame to be received, not including the framesize at the start of recv
    fileName.sendData(socketfd);
}

void Client::receiveFile(std::vector<char> &dataOut)
{
    DataFrame recvData;
    recvData.recvData(socketfd);
    std::vector<char> file;
    recvData.getDataFromFrame(file);
    std::copy(file.begin(), file.end(), std::back_inserter(dataOut));
}

