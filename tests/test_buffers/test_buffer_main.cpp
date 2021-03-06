//
//  main.m
//  test_buffers
//
//  Created by ROBERT BLACKWELL on 11/26/17.
//  Copyright © 2017 Blackwellapps. All rights reserved.
//
#include <iostream>
#include <boost/asio.hpp>
#include <gtest/gtest.h>
#include "bufferV2.hpp"
#include "rb_logger.hpp"

#pragma mark - BufferChain
TEST( BufferChain, assignment)
{
    MBuffer mb(100);
    boost::asio::mutable_buffer bt = boost::asio::buffer(mb.data(), mb.capacity());
    BufferChain chain1;
    BufferChain chain2;
    std::string stmp = "GH";

    for( int i = 0; i < 10; i++) {
        MBufferSPtr mb = std::shared_ptr<MBuffer>(new MBuffer(i*10 + 10));
        stmp += "GH";
        mb->append((void*) stmp.c_str(), stmp.size());
        chain1.push_back(mb);
    }
    chain2 = chain1;
    auto mm = chain1.amalgamate();
    std::cout << "" << std::endl;
    std::cout << "" << std::endl;
}
TEST( BufferChain, makeboostbuffer)
{
    BufferChain chain1;
    BufferChain chain2;
    std::string stmp = "GH";

    for( int i = 0; i < 10; i++) {
        MBufferSPtr mb = std::shared_ptr<MBuffer>(new MBuffer(i*10 + 10));
        stmp += "GH";
        mb->append((void*) stmp.c_str(), stmp.size());
        chain1.push_back(mb);
    }
    auto bb = chain1.asio_buffer_sequence();
    auto asiobuf = boost::asio::buffer(bb);
    std::string  outbound_header_ = "outbound headers";
    std::string  outbound_data_ = "outbound data";
    std::vector<boost::asio::const_buffer> buffers;
    buffers.push_back(boost::asio::buffer(outbound_header_));
    buffers.push_back(boost::asio::buffer(outbound_data_));
    chain2 = chain1;
    std::cout << "" << std::endl;
    std::cout << "" << std::endl;
}



int doSomething()
{
    auto f = [](){
        EXPECT_TRUE(false);
    };
    f();
    return 2;
}
TEST(DUMMY, one)
{
    doSomething();
}


#pragma mark - main
int main(int argc, char * argv[]) {
    RBLogging::setEnabled(false);
    BufferChain chain;
//    void* p1 = malloc(100);
//    void* p2 = malloc(200);
//    boost::asio::mutable_buffer b1(p1, 100);
//    boost::asio::mutable_buffer b2(p2, 200);
//    auto b3 = b2 + 10;

//    testcase_mbuffer();
//    testcase_fbuffer();
//    testcase_fbuffer2();
    char* _argv[2] = {argv[0], (char*)"--gtest_filter=*.*"}; // change the filter to restrict the tests that are executed
    int _argc = 2;
    testing::InitGoogleTest(&_argc, _argv);
    return RUN_ALL_TESTS();

    return 0;
}
