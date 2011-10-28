#ifndef __base_rsa_h
#define __base_rsa_h

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <string.h>

#define PUBLIC_EXPONENT RSA_F4

namespace base
{
    inline int key1(RSA *key, unsigned char *c )
    {
        static unsigned char n[] =
            "\x00\xAA\x36\xAB\xCE\x88\xAC\xFD\xFF\x55\x52\x3C\x7F\xC4\x52\x3F"
            "\x90\xEF\xA0\x0D\xF3\x77\x4A\x25\x9F\x2E\x62\xB4\xC5\xD9\x9C\xB5"
            "\xAD\xB3\x00\xA0\x28\x5E\x53\x01\x93\x0E\x0C\x70\xFB\x68\x76\x93"
            "\x9C\xE6\x16\xCE\x62\x4A\x11\xE0\x08\x6D\x34\x1E\xBC\xAC\xA0\xA1"
            "\xF5";

        static unsigned char e[] = "\x11";

        static unsigned char d[] =
            "\x0A\x03\x37\x48\x62\x64\x87\x69\x5F\x5F\x30\xBC\x38\xB9\x8B\x44"
            "\xC2\xCD\x2D\xFF\x43\x40\x98\xCD\x20\xD8\xA1\x38\xD0\x90\xBF\x64"
            "\x79\x7C\x3F\xA7\xA2\xCD\xCB\x3C\xD1\xE0\xBD\xBA\x26\x54\xB4\xF9"
            "\xDF\x8E\x8A\xE5\x9D\x73\x3D\x9F\x33\xB3\x01\x62\x4A\xFD\x1D\x51";

        static unsigned char p[] =
            "\x00\xD8\x40\xB4\x16\x66\xB4\x2E\x92\xEA\x0D\xA3\xB4\x32\x04\xB5"
            "\xCF\xCE\x33\x52\x52\x4D\x04\x16\xA5\xA4\x41\xE7\x00\xAF\x46\x12"
            "\x0D";

        static unsigned char q[] =
            "\x00\xC9\x7F\xB1\xF0\x27\xF4\x53\xF6\x34\x12\x33\xEA\xAA\xD1\xD9"
            "\x35\x3F\x6C\x42\xD0\x88\x66\xB1\xD0\x5A\x0F\x20\x35\x02\x8B\x9D"
            "\x89";

        static unsigned char dmp1[] =
            "\x59\x0B\x95\x72\xA2\xC2\xA9\xC4\x06\x05\x9D\xC2\xAB\x2F\x1D\xAF"
            "\xEB\x7E\x8B\x4F\x10\xA7\x54\x9E\x8E\xED\xF5\xB4\xFC\xE0\x9E\x05";

        static unsigned char dmq1[] =
            "\x00\x8E\x3C\x05\x21\xFE\x15\xE0\xEA\x06\xA3\x6F\xF0\xF1\x0C\x99"
            "\x52\xC3\x5B\x7A\x75\x14\xFD\x32\x38\xB8\x0A\xAD\x52\x98\x62\x8D"
            "\x51";

        static unsigned char iqmp[] =
            "\x36\x3F\xF7\x18\x9D\xA8\xE9\x0B\x1D\x34\x1F\x71\xD0\x9B\x76\xA8"
            "\xA9\x43\xE1\x1D\x10\xB2\x4D\x24\x9F\x2D\xEA\xFE\xF8\x0C\x18\x26";

        static unsigned char ctext_ex[] =
            "\x1b\x8f\x05\xf9\xca\x1a\x79\x52\x6e\x53\xf3\xcc\x51\x4f\xdb\x89"
            "\x2b\xfb\x91\x93\x23\x1e\x78\xb9\x92\xe6\x8d\x50\xa4\x80\xcb\x52"
            "\x33\x89\x5c\x74\x95\x8d\x5d\x02\xab\x8c\x0f\xd0\x40\xeb\x58\x44"
            "\xb0\x05\xc3\x9e\xd8\x27\x4a\x9d\xbf\xa8\x06\x71\x40\x94\x39\xd2";

        key->n = BN_bin2bn(n, sizeof(n)-1, key->n); 
        key->e = BN_bin2bn(e, sizeof(e)-1, key->e); 
        key->d = BN_bin2bn(d, sizeof(d)-1, key->d); 
        key->p = BN_bin2bn(p, sizeof(p)-1, key->p); 
        key->q = BN_bin2bn(q, sizeof(q)-1, key->q); 
        key->dmp1 = BN_bin2bn(dmp1, sizeof(dmp1)-1, key->dmp1); 
        key->dmq1 = BN_bin2bn(dmq1, sizeof(dmq1)-1, key->dmq1); 
        key->iqmp = BN_bin2bn(iqmp, sizeof(iqmp)-1, key->iqmp); 
        memcpy(c, ctext_ex, sizeof(ctext_ex) - 1);
        return (sizeof(ctext_ex) - 1);
    }

    class CRSA
    {
    public:
        CRSA(): m_pRSAKey(NULL){};
        ~CRSA()
        {
            RSA_free(m_pRSAKey);
        };

        void GenerateKey()
        {
            if(m_pRSAKey == NULL)
            {
                m_pRSAKey = RSA_generate_key(512, PUBLIC_EXPONENT, NULL, NULL);
            }
        }

        void InitKey() 
        {
            if(m_pRSAKey == NULL)
            {
                m_pRSAKey = RSA_new();
                unsigned char ctext_ex[256];
                key1(m_pRSAKey, ctext_ex);
            }
        }
        int GetPublicKey(unsigned char ** ppPublicKey)
        {
            int nLen(-1);
            if(m_pRSAKey)
            {
                nLen = i2d_RSAPublicKey(m_pRSAKey, ppPublicKey);
            }
            return nLen;
        }
        void SetPublicKey(const unsigned char * pPublicKey, int nSize)
        {
            if (m_pRSAKey)
            {
                RSA_free(m_pRSAKey);
                m_pRSAKey = NULL;
            }
            m_pRSAKey = d2i_RSAPublicKey(NULL, &pPublicKey, nSize);
        }

        int PublicEncrypt(unsigned char * inBuffer, int nSize, unsigned  char * outBuffer)
        {
            return RSA_public_encrypt(nSize, inBuffer, outBuffer, m_pRSAKey, RSA_PKCS1_PADDING);
        }
        int PublicDecrypt(unsigned char * inBuffer, int nSize, unsigned char * outBuffer)
        {
            return RSA_public_decrypt(nSize, inBuffer, outBuffer, m_pRSAKey, RSA_PKCS1_PADDING);
        }
        int PrivateEncrypt(unsigned char * inBuffer, int nSize, unsigned char * outBuffer)
        {
            return  RSA_private_encrypt(nSize, inBuffer, outBuffer, m_pRSAKey, RSA_PKCS1_PADDING);
        }
        int PrivateDecrypt(unsigned char * inBuffer, int nSize, unsigned char * outBuffer)
        {
            return  RSA_private_decrypt(nSize, inBuffer, outBuffer, m_pRSAKey, RSA_PKCS1_PADDING);
        }

    private:
        RSA * m_pRSAKey;
    };
}

#endif // __base_rsa_h
