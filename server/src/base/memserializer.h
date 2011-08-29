#ifndef __base_memserializer_h_
#define __base_memserializer_h_

#include "iserializer.h"

namespace base
{
    class MemSerializer : public ISerializer
    {
    public:
        MemSerializer();
        MemSerializer(unsigned int nMaxSize);
        virtual ~MemSerializer();

        bool Attach(char * pData, unsigned int nSize);
        virtual bool Serialize(bool & rbValue, bool bIsRead);
        virtual bool Serialize(char & rsValue, bool bIsRead);
        virtual bool Serialize(unsigned char & rsValue, bool bIsRead);
        virtual bool Serialize(short & rnValue, bool bIsRead);
        virtual bool Serialize(unsigned short & rnValue, bool bIsRead);
        virtual bool Serialize(int & rnValue, bool bIsRead);
        virtual bool Serialize(unsigned int & rnValue, bool bIsRead);
        virtual bool Serialize(long long & rnValue, bool bIsRead);
        virtual bool Serialize(unsigned long long & rnValue, bool bIsRead);
        virtual bool Serialize(float & rfValue, bool bIsRead);
        virtual bool Serialize(double & rdValue, bool bIsRead);
        virtual bool Serialize(char *szValue, bool bIsRead);
        virtual bool Serialize(void *pValue, const unsigned int nSize, bool bIsRead);
    private:
        void _New(unsigned int nMaxSize);
        void _Delete();
        bool _Read(void *pValue, const unsigned int nSize);
        bool _Write(const void *pValue, const unsigned int nSize);
        bool m_bNeedDeleteData;
        char * m_pData;
        char * m_pWriter;
        char * m_pReader;
        unsigned int m_nMaxSize;
        unsigned int m_nCurSize;
    };
}

#endif // __base_memserializer_h_
