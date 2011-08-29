#ifndef __base_crypt_h_
#define __base_crypt_h_

namespace base
{
    enum
    {
        ENCRYPT,
        DECRYPT
    };

    unsigned int EncryptProcXOR8(char *pData, unsigned int nSize, char *pKey, unsigned int nSizeKey);
    unsigned int DecryptProcXOR8(char *pData, unsigned int nSize, char *pKey, unsigned int nSizeKey);
    unsigned int EncryptProcXOR16(char *pData, unsigned int nSize, char *pKey, unsigned int nSizeKey);
    unsigned int DecryptProcXOR16(char *pData, unsigned int nSize, char *pKey, unsigned int nSizeKey);
    unsigned int EncryptProcXOR32(char *pData, unsigned int nSize, char *pKey, unsigned int nSizeKey);
    unsigned int DecryptProcXOR32(char *pData, unsigned int nSize, char *pKey, unsigned int nSizeKey);

#define CryptKeyLen	16

    class CryptKey
    {
        public:
            void SetKey(const char *pKey, unsigned int nSizeKey, unsigned int nStartPos = 0)
            {
                for (unsigned int i=0; i+nStartPos < CryptKeyLen && i< nSizeKey; i++)
                {
                    key[i+nStartPos] = pKey[i];
                }
            };

            void Update()
            {
                for (int i=0; i< CryptKeyLen; i++)
                {
                    key[i] = (char)(key[i]+i);
                }
            };
            char * GetKey()
            {
                return key;
            };
        protected:
            char key[CryptKeyLen];
    };

    class CryptDes
    {
        public:
            typedef bool (*PSubKey)[16][48];
            bool SubKey[2][16][48];
            bool Is3DES;
            char Tmp[256], deskey[16];

            bool Des_Go(char *Out, char *In, int datalen, const char *Key, int keylen, bool Type);
            void SetKey(const char *Key, int len);
            void DES(char Out[8], char In[8], const PSubKey pSubKey, bool Type);
            void SetSubKey(PSubKey pSubKey, const char Key[8]);
            void F_func(bool In[32], const bool Ki[48]);
            void S_func(bool Out[32], const bool In[48]);
            void Transform(bool *Out, bool *In, const char *Table, int len);
            void Xor(bool *InA, const bool *InB, int len);
            void RotateL(bool *In, int len, int loop);
            void ByteToBit(bool *Out, const char *In, int bits);
            void BitToByte(char *Out, const bool *In, int bits);
            unsigned int EncryptProcDES(char *Out, int dataLen, const char *Key, int keyLen);
            unsigned int DecryptProcDES(char *Out, unsigned int dataLen, const char *Key, int keyLen);
    };

}
#endif // __base_crypt_h_

