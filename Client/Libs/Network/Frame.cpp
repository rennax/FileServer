#include "Frame.h"


Frame::Frame()
{

}

void Frame::addData(char *data, size_t size)
{
    std::copy(data, data+size, std::back_inserter(_data));
}

bool Frame::getData(std::vector<char> &dataOut)
{
    std::string content(_data.begin(), _data.end());
    //Check if strin contains deliminator substring
    std::string::iterator it = std::find(content.begin(), content.end(), _deliminator);
    if(it != content.end())
    {
        std::copy(_data.begin(), _data.end(), std::back_inserter(dataOut));
        return true;
    }
    else
        return false;
}