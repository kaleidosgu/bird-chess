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
    inline void key1(RSA *key,	const char*n,
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
		BN_hex2bn(&key->d,d);
		BN_hex2bn(&key->p,p);
		BN_hex2bn(&key->q,q);
		BN_hex2bn(&key->dmp1,dmp1);
		BN_hex2bn(&key->iqmp,iqmp);

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
				//’‚¿Ô∂¡≈‰÷√
				const char n[] = "00AA36ABCE88ACFDFF55523C7FC4523F90EFA00DF3774A259F2E62B4C5D99CB5ADB300A0285E5301930E0C70FB6876939CE616CE624A11E0086D341EBCACA0A1F5";
				const char e[] = "11";
				const char d[] ="0A033748626487695F5F30BC38B98B44C2CD2DFF434098CD20D8A138D090BF64797C3FA7A2CDCB3CD1E0BDBA2654B4F9DF8E8AE59D733D9F33B301624AFD1D51";
				const char p[] ="00D840B41666B42E92EA0DA3B43204B5CFCE3352524D0416A5A441E700AF46120D";
				const char q[] ="00C97FB1F027F453F6341233EAAAD1D9353F6C42D08866B1D05A0F2035028B9D89";
				const char dmp1[] ="590B9572A2C2A9C406059DC2AB2F1DAFEB7E8B4F10A7549E8EEDF5B4FCE09E05";
				const char dmq1[] ="008E3C0521FE15E0EA06A36FF0F10C9952C35B7A7514FD3238B80AAD5298628D51";
				const char iqmp[] ="363FF7189DA8E90B1D341F71D09B76A8A943E11D10B24D249F2DEAFEF80C1826";
                key1(m_pRSAKey,n,e,d,p,q,dmp1,dmq1,iqmp);
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
