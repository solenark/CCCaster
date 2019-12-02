#pragma once

#include "HttpPost.hpp"

#include <string>
#include <memory>

class MatchStatisticsHttpService : private HttpPost::Owner
{
public:
    struct Owner
    {
        virtual void statisticsSentDataResult(MatchStatisticsHttpService *extIpAddr, const std::string &address) = 0;
        virtual void statisticsSentDataError(MatchStatisticsHttpService *extIpAddr) = 0;
        
    };

    Owner *owner = 0;

    std::string address;

    MatchStatisticsHttpService(Owner *owner);

    void start(const std::string &adress, const std::string &body);

    void stop();


private:
    std::shared_ptr<HttpPost> _httpPost;
    

    void httpResponse(HttpPost *httpPost, int code, const std::string &data, uint32_t remainingBytes) override;

    void httpFailed(HttpPost *httpPost) override;

    void httpProgress(HttpPost *httpPost, uint32_t receivedBytes, uint32_t totalBytes) override {}
};
