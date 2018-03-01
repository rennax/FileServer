#include "DataFrame.h"


DataFrame::DataFrame()
{
    _frameSize = 0;
    _dataOffset = 0;
}

void DataFrame::setDataInFrame(const char *buffer, size_t size)
{
    _frameSize = size + sizeof(uint16_t);
    _dataOffset = size;
    _data.resize(_frameSize);
    //copy framesize into data
    size_t uintSize = sizeof(uint16_t);
    //Copy bytes of framesize into _data
    memcpy(_data.data(), &_frameSize, uintSize);
    //Copy string into data
    memcpy(_data.data()+uintSize, buffer, _dataOffset);
}


/*
if the size of data received is equal to the framesize-2 then full frame is received and data is returned via outBuffer
If there is new data returned, true is returned else false.
*/
bool DataFrame::getDataFromFrame(std::vector<char> &outBuffer)
{
    //The first 2 bytes are just size of the buffer
    //if the framesizr isn't the size of data dont return anything yet
    if(_data.size() == _frameSize - sizeof(uint16_t))
    {
        
        std::copy(_data.begin()+2, _data.end(), std::back_inserter(outBuffer));
        return true;
    }
    else
        return false;
}


/*
Data from socket can be passed directly into dataframe.
When a full frame is received, data is returned via outbuffer and true is returned.l

*/
bool setGetDataFrame(char *buffer, size_t size, std::vector<char> &outBuffer)
{
    size_t uint16Size = sizeof(uint16_t);
    //First time frame is used
    if(_frameSize == 0)
        if(size > 2)
            memcpy(&_frameSize, buffer, uint16Size);//Set frame size
    if(size + _dataOffset > _frameSize)
    {
        size_t remaining = (size + _dataOffset) - _frameSize;
        memcpy(_data.data() + _dataOffset, buffer, remaining);
        //copy remaining of current frame into data

        //return data via out buffer

        //copy remaining data into frame
        return true;
    }
    else if(size + _dataOffset <= _frameSize)
    {
        //Copy data into frame

        //return data via out buffer if whole frame is received
        if(_data.size() == )

        //reset frame
        return true;
    }
    else
        return false;


    if(_data.size() == _frameSize - uint16Size && _frameSize > 0)
    {
        std::copy(_data.begin()+2, _data.end(), std::back_inserter(outBuffer));
        return true;
    }
    else if(_data.size() < _frameSize - uint16Size)
    {
        //frame hasn't been set yet/first receive
        
        
        //frame has been set, but current recv contains data from next frame
        if(size + _dataOffset > 1000)
        {
            //Check remaining bytes of current frame
            size_t remaining = (size + _dataOffset) - _frameSize
        }
        else
        {
            _dataOffset = size;
            _data.resize(_frameSize);
            //copy framesize into data

            //Copy bytes of framesize into _data
            memcpy(_data.data(), &_frameSize, uint16Size);
            //Copy string into data
            memcpy(_data.data() + uint16Size, &_frameSize, _dataOffset);
            return true
        }

    }
    else
        return false;
}