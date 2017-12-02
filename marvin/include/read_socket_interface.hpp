//
//  read_socket_interface.hpp
//  MarvinCpp
//
//  Created by ROBERT BLACKWELL on 12/10/16.
//  Copyright © 2016 Blackwellapps. All rights reserved.
//

#ifndef read_socket_interface_h
#define read_socket_interface_h
#include "bufferV2.hpp"
#include "marvin_error.hpp"
#include "callback_typedefs.hpp"

typedef std::function<void(Marvin::ErrorType& er, std::size_t bytes_transfered)> AsyncReadCallback;
typedef std::function<void(Marvin::ErrorType& er, std::size_t bytes_transfered)> AsyncWriteCallback;

class ReadSocketInterface{
public:
    virtual void asyncRead(MBuffer& mb, AsyncReadCallback cb) = 0;
    virtual long nativeSocketFD() = 0;

};

class WriteSocketInterface{
public:
    virtual long nativeSocketFD() = 0;
    virtual void asyncWrite(FBuffer& fb, AsyncWriteCallback) = 0;
    virtual void asyncWriteStreamBuf(boost::asio::streambuf& sb, AsyncWriteCallback) = 0;
};
#endif /* read_socket_interface_h */
