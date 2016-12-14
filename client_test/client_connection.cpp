//
// async_client.cpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "marvin_error.hpp"
#include "callback_typedefs.hpp"
#include "rb_logger.hpp"

RBLOGGER_SETLEVEL(LOG_LEVEL_DEBUG)

#include "client_connection.hpp"
#include <cassert>

using boost::asio::ip::tcp;
using boost::bind;
using boost::function;
using boost::system::error_code;
using boost::asio::io_service;

/**
 * Constructor
 *  @param {io_service} io_service  -   to use for running
 *  @param {string}     scheme      -   "http" or "https"
 *  @param {string}     server      -   domain string
 */
ClientConnection::ClientConnection(
            boost::asio::io_service& io_service,
            const std::string& scheme,
            const std::string& server,
            const std::string& port
            )
            :
            _io(io_service),
            _resolver(io_service),
            _boost_socket(io_service),
            _scheme(scheme),
            _server(server),
            _port(port)
{
}
ClientConnection::ClientConnection(
    boost::asio::io_service& io_service
    ):   _io(io_service),
         _resolver(io_service), // dont really need this
         _boost_socket(io_service)

{
}
ClientConnection::~ClientConnection()
{
    LogDebug("");
}
void ClientConnection::close()
{
    LogDebug(" fd: ", nativeSocketFD());
    _boost_socket.cancel();
    _boost_socket.close();
}
boost::asio::ip::tcp::socket& ClientConnection::getSocketRef()
{
    return _boost_socket;
}
int ClientConnection::nativeSocketFD()
{
    return _boost_socket.native_handle();
}
void ClientConnection::asyncConnect(ConnectCallbackType final_cb)
{
    tcp::resolver::query query(this->_server, _port);
    _finalCb = final_cb; // save the final callback
    
    _resolver.async_resolve(query, [this](const boost::system::error_code& ec,
                                          tcp::resolver::iterator endpoint_iterator){
        LogDebug("resolve OK","so now connect");
        if( ec ){
            Marvin::ErrorType me = ec;
            LogError("error_value", ec.value(), " message: ", ec.message());
            completeWithError(me);
        } else {
            tcp::endpoint endpoint = *endpoint_iterator;
            auto connect_cb = bind(&ClientConnection::handle_connect, this, _1, ++endpoint_iterator);
            _boost_socket.async_connect(endpoint, connect_cb);
            LogDebug("leaving");
        }
    });
}

void ClientConnection::handle_resolve(
                    const error_code& err,
                    tcp::resolver::iterator endpoint_iterator)
{
    LogDebug("entry");
    if (err)
    {
        LogError("resolve FAILED","Error: ",err.message());
        Marvin::ErrorType me = err;
        completeWithError(me);
        return;
    }else{
        LogDebug("resolve OK","so now connect");
        tcp::endpoint endpoint = *endpoint_iterator;
        auto connect_cb = bind(&ClientConnection::handle_connect, this, _1, ++endpoint_iterator);
        _boost_socket.async_connect(endpoint, connect_cb);
        LogDebug("leaving");
    }
}


void ClientConnection::handle_connect(
                    const boost::system::error_code& err,
                    tcp::resolver::iterator endpoint_iterator)
{
    LogDebug("entry");
    if (!err)
    {
        LogDebug("connect OK");
        _boost_socket.non_blocking(true);
        completeWithSuccess();
    }
    else if (endpoint_iterator != tcp::resolver::iterator())
    {
        LogDebug("try next iterator");
        _boost_socket.close();
        tcp::endpoint endpoint = *endpoint_iterator;
        auto handler = boost::bind(&ClientConnection::handle_connect, this, _1, ++endpoint_iterator);
        _boost_socket.async_connect(endpoint, handler);
    }
    else
    {
        LogError("resolve FAILED","Error: ",err.message());
        Marvin::ErrorType me = err;
        completeWithError(me);
    }
    LogDebug("leaving");
}
void ClientConnection::completeWithError(Marvin::ErrorType& ec)
{
    _finalCb(ec, nullptr);
}
void ClientConnection::completeWithSuccess()
{
    Marvin::ErrorType err = Marvin::make_error_ok();
    _finalCb(err, this);
}
/**
 * read
 */
void ClientConnection::asyncRead(MBuffer& buffer, AsyncReadCallbackType cb)
{
    // if a read is active - throw an exception
    //    async_read(this->socket_, buf, )
    //
    // start a boost async_read, on callback pass the data to the http parser
    //
    auto b = boost::asio::buffer(buffer.data(), buffer.capacity());
    _boost_socket.async_read_some(b, [this, cb](const Marvin::ErrorType& err, std::size_t bytes_transfered){
        Marvin::ErrorType m_err = err;
        cb(m_err, bytes_transfered);
    });
    
}
/**
 * write
 */
void ClientConnection::asyncWrite(FBuffer& buffer, AsyncWriteCallbackType cb)
{
    LogDebug("buffer size: ");
//    auto wcb2 = boost::bind(&ClientConnection::handle_write_request,this,_1, _2);
    /// use the boost function that ONLY returns when the write is DONE
    char* x;
    assert(false);
//    boost::asio::async_write(_boost_socket, boost::asio::buffer(x, 0), cb);
}
void ClientConnection::asyncWriteStreamBuf(boost::asio::streambuf& sb, AsyncWriteCallback cb)
{
    LogDebug("");
    boost::asio::async_write(
        (this->_boost_socket),
        sb,
        [this, cb](
            const Marvin::ErrorType& err,
            std::size_t bytes_transfered
            )
        {
        LogDebug("");
        if( !err ){
            Marvin::ErrorType m_err = Marvin::make_error_ok();
            cb(m_err, bytes_transfered);
        }else{
            Marvin::ErrorType m_err = err;
            cb(m_err, bytes_transfered);
        }
    });
}

void close()
{
    
}


