#ifndef TRX_TOKENIZE_HPP
#define TRX_TOKENIZE_HPP

#include <string>
#include <sstream>

namespace trx
{
    // @see http://stackoverflow.com/questions/236129/split-a-string-in-c
    template<class ContainerT>
    void tokenize(const std::string& str, ContainerT& tokens,
        const std::string& delimiters = " ", bool trimEmpty = false)
    {
        std::string::size_type pos, lastPos = 0;

        typedef ContainerT Base;
        typedef typename Base::value_type ValueType;
        typedef typename ValueType::size_type SizeType;

        while (true)
        {
            pos = str.find_first_of(delimiters, lastPos);
            if (pos == std::string::npos)
            {
                pos = str.length();

                if (pos != lastPos || !trimEmpty)
                    tokens.push_back(ValueType(str.data() + lastPos,
                    (SizeType)pos - lastPos));

                break;
            }
            else
            {
                if (pos != lastPos || !trimEmpty)
                    tokens.push_back(ValueType(str.data() + lastPos,
                    (SizeType)pos - lastPos));
            }

            lastPos = pos + 1;
        }
    }
}

#endif //TRX_TOKENIZE_HPP
