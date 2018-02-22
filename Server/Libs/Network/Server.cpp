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
        std::cout << "Waiting for client to connect..." << std::endl;
        //Accept client
        struct Client client;
        client._socketfd = accept(_socketfd,
                            (struct sockaddr *) &_addr,
                            &client._clilen);
        std::cout << "Client connected" << std::endl;
        receiveData(client);//blocking operation
        
        //check if path in request exist
        std::ifstream file(_buffer._data, std::ifstream::binary);
        file >> std::noskipws;
        if(!file.good())
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
        //Get file size
        file.seekg(0, file.end);
        int fileSize = file.tellg();
        file.seekg(0, file.beg);
        //Send length of file to client
        uint16_t frameSize = sizeof(int);
        





        
        //Copy file into vector
        std::vector<char> fileVec;
        std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), std::back_inserter(fileVec));
        std::vector<char>::iterator it = fileVec.begin();
        //Now send the data
        struct Buffer buff;
        while(true)
        {
            //copy next data block into buffer
            if(it+1024 <= fileVec.end())//Default size 1024
            {
                std::copy(it, it+1024, buff._data);
                buff._size = 1024;
                it += 1024;
                std::cout << "sent 1024 bytes to client" << std::endl;
            }
            else//If rest of the data isn't 1024 chars big, send rest
            {
                int left = fileVec.end() - it;
                std::copy(it, it+left, buff._data);
                buff._size = left;
                std::cout << "sent " << left << " bytes to client" << std::endl;
            }
            //Send data with blocking I/O
            send(client._socketfd, buff._data, buff._size, 0);
            
            //Check if everything is sent
            if(it == fileVec.end())
                break;
            else
            {
                //Reset buff for next payload
                bzero(buff._data, sizeof(buff._data)*buff._size);
                buff._size = 0;
            }
        }
    }
}



void Server::sendString(struct Client client, std::string &s)
{
    send(client._socketfd, s.c_str(), s.size(),0);
}

void Server::receiveData(struct Client client)
{
    //Wait for client to send path to requested file(BLOCKING)
    int readSize = recv(client._socketfd, _buffer._data, _buffer._max, 0);
    if(readSize > 0)//we do not care about datagram packets, those can be 0 in length
        Debug::error("ERROR on reading packet");
    _buffer._size = readSize;
}