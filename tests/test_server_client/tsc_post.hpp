#ifndef tsc_post_hpp
#define tsc_post_hpp

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "catch.hpp"
#include "rb_logger.hpp"
#include "bufferV2.hpp"
#include "client.hpp"
#include "tsc_testcase.hpp"

/**
* This function runs a post request based on a testcase and checks the response is as expected
*/
std::shared_ptr<Client> post_body_testcase(EchoTestcase& testcase, boost::asio::io_service& io);

class PostTest
{
    public:
        PostTest(boost::asio::io_service& io, EchoTestcase& testcase);
        void handler(Marvin::ErrorType& er, MessageReaderV2SPtr rdr);
        void exec();
    protected:
        boost::asio::io_service&    _io;
        MessageBaseSPtr             _msg;
        ClientSPtr                  _client;
        EchoTestcase&               _testcase;
};


#endif /* test_client_h */
