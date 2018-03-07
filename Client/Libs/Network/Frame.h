#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

class Frame
{
public:
    Frame();
    void createFrame(std::vector<char> data);
    void addData(char *data, size_t size);
    bool getData(std::vector<char> &dataOut);
private:
    std::vector<char> _data;
    std::string _deliminator = "<EOF>";
};