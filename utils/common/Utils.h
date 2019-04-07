//
// Created by wang on 10/22/18.
//

#include <string>
#include <vector>
#ifndef KUNGFU_UTILS_H
#define KUNGFU_UTILS_H

namespace kungfu
{
    class LDUtils
    {
    public:
        static std::string gzip_compress(const std::string& src);
        static std::string gzip_decompress(const std::string& src);
        static std::vector<std::string> split(const std::string& src, const std::string& pred);
    };
}


#endif //KUNGFU_ZIPPER_H
