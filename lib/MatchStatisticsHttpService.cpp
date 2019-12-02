#include "MatchStatisticsHttpService.hpp"
#include "Logger.hpp"

#include <vector>

using namespace std;

MatchStatisticsHttpService::MatchStatisticsHttpService(Owner *owner) : owner(owner) {}

void MatchStatisticsHttpService::httpResponse(HttpPost *httpPost, int code, const string &data, uint32_t remainingBytes)
{
    ASSERT(_httpPost.get() == httpPost);

    LOG("Received HTTP response (%d): '%s'", code, data);

    if (code != 200) // Min IPv4 length, eg "1.1.1.1" TODO actually validate this
    {
        httpFailed(httpPost);
        return;
    }

    address = trimmed(data);

    _httpPost.reset();

    if (owner)
        owner->statisticsSentDataResult(this, address);
}

void MatchStatisticsHttpService::httpFailed(HttpPost *httpPost)
{
    ASSERT(_httpPost.get() == httpPost);

    LOG("HTTP POST failed for: %s", _httpPost->url);

    _httpPost.reset();

    if (owner)
        owner->statisticsSentDataError(this);
    return;
}

void MatchStatisticsHttpService::start(const string &adress, const string &body)
{
    address.clear();

    _httpPost.reset(new HttpPost(this, adress, body));
    _httpPost->start();
}

void MatchStatisticsHttpService::stop()
{
    _httpPost.reset();
}
