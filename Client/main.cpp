#include "Network/DataFrame.h"
#include "Network/Client.h"
#include <sys/types.h>
#include <linux/limits.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(int argc, char *argv[])
{
    if(argc < 2)
        return 0;
    std::string ip(argv[1]);
    std::string filePath(argv[2]);
    std::cout << "Connecting to server at "<< ip <<" please wait" << std::endl;

    const char* ipC_str = ip.c_str();
    Client client;
    client.connectTo(ipC_str, 9000);

    std::cout <<"Connected to server." << std::endl 
                << "Requesting file: " << filePath << std::endl;
    DataFrame fileRequest(false);
    fileRequest.setDataInFrame(filePath.c_str(), filePath.length());
    client.requestFile(fileRequest);
    //Receive file data
    std::vector<char> file;
    client.receiveFile(file);
    //If nothing was received, exit program
    if(file.size() == 0)
        return 0;
    //Get exe filepath
    std::string arg1;
    char exePath[PATH_MAX+1];
    arg1 = "/proc/";
    arg1 += getpid();
    arg1 += "/exe";
    readlink(arg1.c_str(), exePath, sizeof(exePath));
    //Create file
    size_t filePos = filePath.find_last_of("/");
    std::string fileName = filePath.substr(filePos+1);
    std::string createFile(exePath);
    createFile += "Downloads/" + fileName;
    std::ofstream of(createFile, std::ios::out | std::ios::binary);
    std::copy(file.begin(), file.end(), std::ostreambuf_iterator<char>(of));
    of.flush();
    of.close();
    
    return 0; 
}