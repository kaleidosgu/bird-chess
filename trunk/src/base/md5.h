#ifndef __base_md5_h_
#define __base_md5_h_

#include <string>

namespace base
{
    typedef struct MD5Digest_struct {
        unsigned char a[16];
    } MD5Digest;

    typedef char MD5Context[88];

    void MD5Init(MD5Context *pCtx);
    void MD5Update(MD5Context *pCtx, const void* buf, size_t len);
    void MD5Final(MD5Digest* digest, MD5Context* pCtx);

    void MD5Sum(const void* data, size_t length, MD5Digest* digest);
    bool MD5File(char *szFileName, MD5Digest* digest);
    void MD5String(char * szStr, MD5Digest* digest);
    std::string MD5String(const std::string& str);

    void MD5Print(const MD5Digest & digest);
    std::string MD5DigestToBase16(const MD5Digest & digest);
}
#endif // __base_md5_h_ 

