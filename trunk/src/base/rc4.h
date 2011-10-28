#ifndef __base_rc4_h_
#define __base_rc4_h_

#include <openssl/rc4.h>

namespace base
{
    class CRC4
    {
    public:
        CRC4(){};
        ~CRC4(){};
        void SetKey(const unsigned char* pKey, int nLen)
        {
            RC4_set_key(&m_Key, nLen, pKey); 
        }

        void Encrypt(unsigned char * inBuffer, int nSize, unsigned  char * outBuffer)
        {
            RC4(&m_Key, nSize, inBuffer, outBuffer); 
        }

        void Decrypt(unsigned  char * inBuffer, int nSize, unsigned  char * outBuffer)
        {
            RC4(&m_Key, nSize, inBuffer, outBuffer); 
        }
    private:
        RC4_KEY m_Key;
    };

}

#endif // __base_rc4_h_
