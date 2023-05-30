#include "share_handle_storage/MD5.hpp"
#include <fstream>
#include <vector>
#include <iterator>
#include <openssl/md5.h>

using namespace std;

namespace cti_robot
{
    namespace md5_tool
    {
        string Generate(const string filename)
        {
            string hash;
            ifstream ifs(filename);
            ifs.unsetf(std::ios::skipws);
            
            uint8_t md5[MD5_DIGEST_LENGTH];
            MD5_CTX ctx;
            MD5_Init(&ctx);
            vector<uint8_t> content { istream_iterator<uint8_t>(ifs), istream_iterator<uint8_t>() };
            for(int i=0;i<content.size();i+=1024)
            {
                auto size = content.size() - i;
                if (size > 1024) size = 1024;
                MD5_Update(&ctx, content.data() + i, size);
            }
            MD5_Final(md5, &ctx);
            for (auto i : md5)
            {
                char hex[4];
                sprintf(hex, "%02x", i);
                hash += hex;
            }
            return hash;
        }
    }
}
