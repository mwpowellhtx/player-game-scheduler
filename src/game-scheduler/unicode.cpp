#include "unicode.h"

#include <istream>

namespace trx
{
    /// @see http://en.wikipedia.org/wiki/Byte_order_mark
    bool skip_bom(std::istream& in) {

        char test[4] = { 0 };
        in.read(test, 3);
        if (strcmp(test, "\xEF\xBB\xBF") == 0)
            return true;
        in.seekg(0);
        return false;
    }
}
