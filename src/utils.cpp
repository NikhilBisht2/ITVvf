#include "utils.hpp"
#include <openssl/evp.h>

namespace utils
{

    std::string base64(const unsigned char *data, int len)
    {
        std::string out;
        out.resize(4 * ((len + 2) / 3));
        EVP_EncodeBlock(reinterpret_cast<unsigned char *>(&out[0]), data, len);
        return out;
    }

    bool write_all(SSL *ssl, const void *data, int len)
    {
        const char *p = static_cast<const char *>(data);
        while (len > 0)
        {
            int r = SSL_write(ssl, p, len);
            if (r <= 0)
                return false;
            p += r;
            len -= r;
        }
        return true;
    }

}
