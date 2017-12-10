//
//  http_header.hpp
//  MarvinCpp
//
//  Created by ROBERT BLACKWELL on 12/26/16.
//  Copyright © 2016 Blackwellapps. All rights reserved.
//

#ifndef http_header_hpp
#define http_header_hpp

#include <stdio.h>
#include <map>
#include <string>
#include <vector>
#include <set>


typedef std::map<std::string, std::string> HttpHeadersType;
typedef std::set<std::string> HttpHeaderFilterSetType;

namespace HttpHeader
{
    /// Converts a c-string header key to canonical form
    void canonicalKey(char* key, int length);

    /// Converts a std::string header key to canonical form
    void canonicalKey(std::string& key);
    
    /// Filters a header map to remove all headers whose keys are IN the filterList
    void filterOut(HttpHeadersType& hdrs, std::vector<std::string> list);
    
    /// Filters a header map to remove all headers whose keys are NOT IN the filterList
    void filterIn(HttpHeadersType& hdrs, std::vector<std::string> list);
    
    /// Filters a header map, calls the cb function for all headers
    /// whose keys ARE NOT IN the filterList
    void filterNotInList(
        HttpHeadersType&        hdrs,
        HttpHeaderFilterSetType list,
        std::function<void(HttpHeadersType& hdrs,
                            std::string key,
                            std::string value)> cb);

    /// Filters a header map, calls the cb function for all headers
    /// whose keys ARE IN the filterList
    void filterInList(
        HttpHeadersType&        hdrs,
        HttpHeaderFilterSetType list,
        std::function<void(HttpHeadersType& hdrs,
                            std::string key,
                            std::string value)> cb);


    /// Selected header keys as named constants in canonical form
    typedef std::string Keys;
    namespace Name{
        static const std::string Host = "HOST";
        static const std::string Connection = "CONNECTION";
        static const std::string ContentLength = "CONTENT-LENGTH";
        static const std::string ContentType = "CONTENT-TYPE";
        static const std::string AcceptEncoding = "ACCEPT-ENCODING";
        static const std::string ProxyConnection = "PROXY-CONNECTION";
        static const std::string TransferEncoding = "TRANSFER-ENCODING";
        static const std::string ETag = "ETAG";
        static const std::string ConnectionHandlerId = "CONNECT-HANDLER-ID";
        static const std::string RequestHandlerId = "REQUEST-HANDLER-ID";
    };
    namespace Value{
        static const std::string ConnectionClose = "CLOSE";
        static const std::string ConnectionKeepAlive = "KEEP-ALIVE";
    }
    typedef std::string SchemeType;
    namespace Scheme {
        static const std::string https = "https";
        static const std::string http = "http";
    }
};
#endif /* http_header_hpp */
