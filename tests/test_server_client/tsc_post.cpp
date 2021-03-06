
#include "tsc_post.hpp"

PostTest::PostTest(boost::asio::io_service& io, EchoTestcase& testcase): _io(io), _testcase(testcase)
{

}
void PostTest::handler(Marvin::ErrorType& er, MessageReaderV2SPtr rdr)
{
    std::string sx = rdr->get_body_chain().to_string();
    std::string sy = _testcase.buffers_as_string();
#ifdef _VERBOSE
//        std::cout << "echo'ed body " << sx << std::endl;
//        std::cout << "testcasebody " << testcase.buffers_as_string() << std::endl;
#endif
    assert(rdr->statusCode() == 200);
    assert(sx == sy);
    REQUIRE(rdr->statusCode() == 200);
    REQUIRE(sx == sy);
//    std::cout << "SUCCESS: " << _testcase._description <<  std::endl;
    
}

void PostTest::exec()
{
    auto url = _testcase._url;
    _client = std::shared_ptr<Client>(new Client(_io, url));
    _msg = std::shared_ptr<MessageBase>(new MessageBase());
    _msg->setMethod(HttpMethod::POST);

    auto f = std::bind(&PostTest::handler, this, std::placeholders::_1, std::placeholders::_2);

    switch(_testcase._test_type){
        case EchoTestType::WITH_STRING :
        {
            auto str_buf = _testcase.buffers_as_string();
            _client->asyncWrite(_msg, str_buf, f);
            break;
        }
            
        case EchoTestType::WITH_MBUFFER :
        {
            auto mbufptr = _testcase.buffers_as_mbuffer();
            _client->asyncWrite(_msg, mbufptr, f);
            break;
        }
            
        case EchoTestType::WITH_BUFFER_CHAIN :
        {
            auto tmp = _testcase.buffers_as_buffer_chain();
            _client->asyncWrite(_msg, tmp, f);
            break;
        }
    }

}

