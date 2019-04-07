//
// Created by wang on 10/22/18.
//
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <iostream>
#include <sstream>
#include "Utils.h"
#include "../../yijinjing/log/KfLog.h"
#include "../gzip/decompress.hpp"
#include "../gzip/compress.hpp"
using namespace kungfu::yijinjing;
using namespace gzip;
namespace  kungfu
{
     std::string LDUtils::gzip_compress(const std::string& src)
     {
         try
         {
             return gzip::compress(src.c_str(), src.size());
         }
         catch (const std::exception& e)
         {
             std::cout<<"gzip compress exception,{error:"<<e.what()<<"}"<<std::endl;
         }
         return std::string();
     }

     std::string LDUtils::gzip_decompress(const std::string& src)
     {
         try
         {
             return gzip::decompress(src.c_str(), src.size());
         }
         catch (const std::exception& e)
         {
             std::cout<<"gzip decompress exception,{error:"<<e.what()<<"}"<<std::endl;
         }
         return std::string();
     }

    std::vector<std::string> LDUtils::split(const std::string& src, const std::string& pred)
    {
        std::vector<std::string> ret;
        try
        {
            boost::split(ret, src, boost::is_any_of(pred));
        }
        catch (const std::exception& e)
        {
            std::cout<<"split exception,{error:"<<e.what()<<"}"<<std::endl;
        }
        return std::move(ret);
    }

 }