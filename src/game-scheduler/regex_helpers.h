#ifndef TRX_REGEX_HELPERS_H
#define TRX_REGEX_HELPERS_H

#include <functional>
#include <string>
#include <regex>
#include <map>

namespace trx
{
    void match_and_parse(const std::regex& pattern, const std::string& s,
        const std::function<void()>& onMatch,
        const std::map<int, std::function<void(std::string)>>& groupHandlerMap);
}

#endif //TRX_REGEX_HELPERS_H
