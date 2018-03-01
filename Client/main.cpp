
#include "Network/Client.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    //implement ip + file destionation reading
    std::cout << "Please enter IP to server" << std::endl;
    std::string ip;
    std::cin >> ip;
    std::string filePath = "/root/Desktop/rarepepe3.jpeg";
    std::cout << std::endl<<"Please enter filepath to file on server" << std::endl;
    //std::cin >> filePath;
    const char* ipC_str = ip.c_str();
    Client client;
    client.connectTo(ipC_str, 9000);
    int fileSize = 0;
    struct Frame frame;
    frame.copyStringToFrame(filePath);
    client.requestFile(fileSize, frame);
    

    
     return 0; 
}