
#include <iostream>
#include <sstream>
#include <string>
#include <boost/asio.hpp>
#include <pthread.h>
#include "rb_logger.hpp"
RBLOGGER_SETLEVEL(LOG_LEVEL_DEBUG)

#include "server.hpp"
#include "handler_interface.hpp"
#include "request.hpp"
#ifdef GHDF
class MyHandler : public RequestHandlerInterface
{
public:
    MyHandler()
    {
    }
    void handle_request(
        boost::asio::io_service& io,
        MessageReader& req,
        MessageWriter& resp,
        HandlerDoneCallbackType done
    ){
        std::cout << "got to a new handler " << std::endl;
        std::ostringstream os;
        std::string uri = req.uri();
        
        os << "<!DOCTYPE html>";
        os << "<html><head></head><body>";
        os << "<p>Method: "     << req.getMethodAsString() << "</p>";
        os << "<p>Vers Maj: "   << req.httpVersMajor() << "</p>";
        os << "<p>Vers Minor: " << req.httpVersMinor() << "</p>";
        os << "<p>uri: "       << req.uri() << "</p>";
        req.dumpHeaders(os);
        for(int i = 0; i < 10; i++ ){
            os << "<p>This is simple a line to fill out the transmission</p>";
        }
        os << "</body>";
        
        resp.setStatusCode(200);
        resp.setStatus("OK");
        resp.setHttpVersMajor(1);
        resp.setHttpVersMinor(1);
        std::string s = os.str();
        resp.setContent(s);
        resp.setHeader("Content-length", std::to_string(s.length() ));
        resp.setHeader("Connection","close");
        resp.asyncWrite([done](Marvin::ErrorType& err){;
            done(true);
        });
    }
};

void
runAServer()
{
    try
    {
        MyHandler handler;
        Server server(handler);
        server.listen();
    }
    catch (std::exception& e)
    {
        std::cerr << "exception: " << e.what() << "\n";
    }
}
#endif
#include "test_server.hpp"
int main(int argc, char* argv[])
{
    runTestServer();
    return 0;
}
