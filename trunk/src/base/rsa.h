#ifndef __base_rsa_h
#define __base_rsa_h

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/bn.h>
#include <string.h>

#define PUBLIC_EXPONENT RSA_F4

namespace base
{
    inline void SetKey(RSA *key,
            const char*n,
            const char*e,
            const char*d = NULL,
            const char*p = NULL,
            const char*q = NULL,
            const char*dmp1 = NULL,
            const char*dmq1 = NULL,
            const char*iqmp = NULL)
    {
        //key->n = BN_bin2bn(n, sizeof(n)-1, key->n); 
        BN_hex2bn(&key->n,n);
        BN_hex2bn(&key->e,e);
        if (d != NULL)
        {
            BN_hex2bn(&key->d,d);
        }
        if (p != NULL)
        {
            BN_hex2bn(&key->p,p);
        }
        if (q != NULL)
        {
            BN_hex2bn(&key->q,q);
        }
        if (dmp1 != NULL)
        {
            BN_hex2bn(&key->dmp1,dmp1);
        }
        if (dmq1 != NULL)
        {
            BN_hex2bn(&key->dmq1,dmq1);
        }
        if (iqmp != NULL)
        {
            BN_hex2bn(&key->iqmp,iqmp);
        }

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

        void InitPublicKey(const char * n, const char * e)
        {
            InitPrivateKey(n, e, NULL, NULL, NULL, NULL, NULL, NULL);
        }
        void InitPrivateKey(
                const char * n,
                const char * e,
                const char * d,
                const char * p,
                const char * q,
                const char * dmp1,
                const char * dmq1,
                const char * iqmp
                ) 
        {
            if(m_pRSAKey == NULL)
            {
                m_pRSAKey = RSA_new();
                SetKey(m_pRSAKey,n,e,d,p,q,dmp1,dmq1,iqmp);
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
