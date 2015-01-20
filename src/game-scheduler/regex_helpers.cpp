#include "regex_helpers.h"

namespace trx
{
    void match_and_parse(const std::regex& pattern, const std::string& s,
        const std::function<void()>& onMatch,
        const std::map<int, std::function<void(std::string)>>& groupHandlerMap) {

        auto m = std::cmatch{};

        if (!std::regex_match(s.c_str(), m, pattern))
            return;

        onMatch();

        for (const auto& h : groupHandlerMap)
            h.second(m[h.first].str());
    }
}
