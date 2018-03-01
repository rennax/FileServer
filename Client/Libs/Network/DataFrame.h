#include <vector>
#include <string>
#include <iterator>
#include <string.h>


class DataFrame
{
public:
    DataFrame();
    //Sets data in the frame, only use for sending frames
    void setDataInFrame(const char *buffer, size_t size);
    //buffer to push received data to. See impl for more details
    bool getDataFromFrame(std::vector<char> &outBuffer);
    //Use this if data received is expected to be larger than 1000 bytes (including dataSize)
    //Once a full frame is received, it will be appended to outBuffer
    bool setGetDataFrame(char *buffer, size_t size, std::vector<char> &outBuffer);
private:
    uint16_t _frameSize;
    uint16_t _dataOffset;
    std::vector<char> _data;//2 first bytes are reserved for size of the actual data "_dataOffset"
};


// struct Frame
// {
//     uint16_t _size;

//     std::vector<char> _data;

//     void copyStringToFrame(std::string str)
//     {
//         _size = str.length()+ sizeof(_size);
//         _data.resize(_size);
//         _size -= sizeof(_size);
//         //copy _size into data
//         memcpy(_data.data(), &_size, sizeof(_size));
//         //Copy src into data
//         memcpy(_data.data() + sizeof(_size), str.data(), str.length());
//     }

//     void copyBufferToFrame(char *buffer, size_t size)
//     {
//         std::copy(buffer+2, buffer+size, std::back_inserter(_data));
//     }
// };