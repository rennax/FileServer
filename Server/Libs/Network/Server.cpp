#include "Network/Server.h"


Server::Server(int portno)
{
    _socketfd = socket(AF_INET, SOCK_STREAM, 0);
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = INADDR_ANY;
    _addr.sin_port = htons(portno);
    //Bind socket, throw error if it fails
    if (bind(_socketfd, (struct sockaddr *) &_addr, sizeof(_addr)) < 0) 
        Debug::error("ERROR on binding socket");
    //Backlog of 10 clients
    listen(_socketfd, 10);

    run();
}

Server::~Server()
{
    //Close socket and free resources if any is allocated
    close(_socketfd);
}

void Server::run()
{
    while(true)
    {
        DataFrame recv, send(false);
        std::cout << "Waiting for client to connect..." << std::endl;
        //Accept client
        struct Client client;
        client._socketfd = accept(_socketfd,
                            (struct sockaddr *) &_addr,
                            &client._clilen);
        std::cout << "Client connected" << std::endl;
        receiveData(client, recv);//blocking operation
        struct File file;
        std::vector<char> recvData;
        recv.getDataFromFrame(recvData);
        std::string filePath(recvData.begin(), recvData.end());
        if(!file.loadFile(filePath))
        {
            //File doesn't exist
            //Inform client about and close connection
            std::string clistr("Filepath doesn't exist or couldn't be opened\nPlease try again");
            std::cout << "Failed find file client requested at location: " << _buffer._data << std::endl;
            sendString(client, clistr);
            close(client._socketfd);
            bzero(_buffer._data, _buffer._size);
            continue;
        }
        
        //Send file
        send.setDataInFrame(file._data.data(), file._fileSize);
        send.sendData(client._socketfd);
        // //Now send the data
        // struct Buffer buff;
        // while(true)
        // {
        //     //copy next data block into buffer
        //     if(file._it+1024 <= file._data.end())//Default size 1024
        //     {
        //         std::copy(file._it, file._it+1024, buff._data);
        //         buff._size = 1024;
        //         file._it += 1024;
        //         std::cout << "sent 1024 bytes to client" << std::endl;
        //     }
        //     else//If rest of the data isn't 1024 chars big, send rest
        //     {
        //         int left = file._data.end() - file._it;
        //         std::copy(file._it, file._it+left, buff._data);
        //         buff._size = left;
        //         std::cout << "sent " << left << " bytes to client" << std::endl;
        //     }
        //     //Send data with blocking I/O
        //     send(client._socketfd, buff._data, buff._size, 0);
            
        //     //Check if everything is sent
        //     if(file._it == file._data.end())
        //         break;
        //     else
        //     {
        //         //Reset buff for next payload
        //         bzero(buff._data, sizeof(buff._data)*buff._size);
        //         buff._size = 0;
        //     }
        // }
    }
}



void Server::sendString(struct Client client, std::string &s)
{
    send(client._socketfd, s.c_str(), s.size(),0);
}

void Server::receiveData(struct Client client, DataFrame &frame)
{
    //Wait for client to send path to requested file(BLOCKING)
    frame.recvData(client._socketfd);
}