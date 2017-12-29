//
// Start both a server and a number of client tthreads and have each of the client
// threads make a number of requests from the server.
//
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <thread>
#include <pthread.h>
#include <gtest/gtest.h>
#include "boost_stuff.hpp"
#include "tcp_connection.hpp"
#include "rb_logger.hpp"
RBLOGGER_SETLEVEL(LOG_LEVEL_INFO)

#include "http_server.hpp"
#include "request_handler_base.hpp"
TEST(connect, failBadHost)
{
    boost::asio::io_service io;
    auto conn_sptr = std::make_shared<TCPConnection>(io, "https", "ddddgoogle.com", "443");
    conn_sptr->asyncConnect([](Marvin::ErrorType& err, ISocket* conn)
    {
        std::cout << Marvin::make_error_description(err) << std::endl;
//        auto x1 = boost::system::errc::host_unreachable;
//        auto y = err.default_error_condition();
//        auto b1 = (x1 == y);
//        auto b2 = (err == x1);
        auto not_found = boost::asio::error::make_error_code(boost::asio::error::netdb_errors::host_not_found);
        auto not_found_try_again = boost::asio::error::make_error_code(boost::asio::error::netdb_errors::host_not_found_try_again);
        ASSERT_TRUE( (err == not_found) || (err == not_found_try_again));
    });
    io.run();
}

TEST(connect, failTimeout)
{
    boost::asio::io_service io;
    auto conn_sptr = std::make_shared<TCPConnection>(io, "http", "localhost", "3333");
    conn_sptr->asyncConnect([](Marvin::ErrorType& err, ISocket* conn)
    {
        std::cout << Marvin::make_error_description(err) << std::endl;

        std::cout << Marvin::make_error_description(err) << std::endl;
        ASSERT_TRUE( err == boost::system::errc::connection_refused );
    });
    io.run();
}

TEST(connect, succeed)
{
    boost::asio::io_service io;
    auto conn_sptr = std::make_shared<TCPConnection>(io, "https", "google.com", "443");
    conn_sptr->asyncConnect([](Marvin::ErrorType& err, ISocket* conn)
    {
        std::cout << err.message() << std::endl;
        ASSERT_TRUE(! err);
    });
    io.run();
}

int main( int argc, char* argv[] )
{
    // global setup - run a server
    RBLogging::setEnabled(false);
    char* _argv[2] = {argv[0], (char*)"--gtest_filter=*.*"}; // change the filter to restrict the tests that are executed
    int _argc = 2;
    testing::InitGoogleTest(&_argc, _argv);
    auto res = RUN_ALL_TESTS();
    return res;
}

