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


void DataFrame::setFrameSize(char *buffer, size_t size)
{
    size_t uint16Size = sizeof(uint16_t);
    _frameSize = 0; //Makes sure setFramesize is called again if framesize couldn't be set
    //set framesize
    if(size >= 2)
    {
        if(_msbFrameSize != NULL)
        {
            _frameSize = _msbFrameSize << 8;//Bitshift msb into framesize
            memcpy(&_frameSize, buffer, uint16Size);
            _dataOffset = 0;
        }
        else
        {
            memcpy(&_frameSize, buffer, uint16Size);
            _dataOffset = 0;
        }

    }
    else if(size == 1)
        _msbFrameSize = buffer[0];
    else
        _dataOffset = 1;//Not enough data received to get framesize, wait for next buffer
}

/*
Data from socket can be passed directly into dataframe.
When a full frame is received, data is returned via outbuffer and true is returned. Any remaining data,
is moved into the frame
*/
bool DataFrame::setGetDataFrame(char *buffer, size_t size, std::vector<char> &outBuffer)
{
    size_t uint16Size = sizeof(uint16_t);
    if(size + _dataOffset > _frameSize)
    {
        size_t remaining = (size + _dataOffset) - _frameSize;
        size_t toCopy = _frameSize-_dataOffset;
        //copy remaining of current frame into data
        memcpy(_data.data() + _dataOffset, buffer, toCopy);
        //return data - framesize via out buffer
        std::copy(_data.begin() + uint16Size, _data.end(), std::back_inserter(outBuffer));
        setFrameSize(buffer+toCopy, remaining);
        //copy remaining data into frame
        memcpy(_data.data(), buffer+toCopy, remaining);
        return true;
    }
    else if(size + _dataOffset < _frameSize)
    {
        //Copy data into frame

        //return data via out buffer if whole frame is received
        if(_data.size() == 0)

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
            size_t remaining = (size + _dataOffset) - _frameSize;
            return true;
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
            return true;
        }

    }
    else
        return false;
}