#ifndef TRX_UNICODE_H
#define TRX_UNICODE_H

#include <istream>

namespace trx
{
    bool skip_bom(std::istream& in);
}

#endif //TRX_UNICODE_H
