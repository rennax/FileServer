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
    //implement ip + file destionation reading
    std::cout << "Please enter IP to server" << std::endl;
    std::string ip;
    std::cin >> ip;
    std::string filePath = "/root/Desktop/Test.txt";
    size_t filePos = filePath.find_last_of("/");
    std::string fileName = filePath.substr(filePos+1);
    
    std::cout << std::endl<<"Please enter filepath to file on server" << std::endl;
    //std::cin >> filePath;
    const char* ipC_str = ip.c_str();
    Client client;
    client.connectTo(ipC_str, 9000);
    int fileSize = 0;
    DataFrame fileRequest(false);
    fileRequest.setDataInFrame(filePath.c_str(), filePath.length());
    client.requestFile(fileSize, fileRequest);
    //Receive file data
    std::vector<char> file;
    client.receiveFile(file);
    //Get exe filepath
    std::string arg1;
    char exePath[PATH_MAX+1];
    arg1 = "/proc/";
    arg1 += getpid();
    arg1 += "/exe";
    readlink(arg1.c_str(), exePath, sizeof(exePath));
    //Create file
    std::string createFile(exePath);
    createFile += "Downloads/" + fileName;
    std::ofstream of(createFile, std::ios::out | std::ios::binary);
    std::copy(file.begin(), file.end(), std::ostreambuf_iterator<char>(of));
    of.flush();
    of.close();
    
    return 0; 
}