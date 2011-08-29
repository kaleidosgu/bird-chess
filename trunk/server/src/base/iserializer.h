#ifndef __base_iserializer_h_
#define __base_iserializer_h_

namespace base
{
    class ISerializer
    {
    public:
        ISerializer(){};
        virtual ~ISerializer(){};
        virtual bool Serialize(bool & rbValue, bool bIsRead) = 0;
        virtual bool Serialize(char & rsValue, bool bIsRead) = 0;
        virtual bool Serialize(unsigned char & rsValue, bool bIsRead) = 0;
        virtual bool Serialize(short & rnValue, bool bIsRead) = 0;
        virtual bool Serialize(unsigned short & rnValue, bool bIsRead) = 0;
        virtual bool Serialize(int & rnValue, bool bIsRead) = 0;
        virtual bool Serialize(unsigned int & rnValue, bool bIsRead) = 0;
        virtual bool Serialize(long long & rnValue, bool bIsRead) = 0;
        virtual bool Serialize(unsigned long long & rnValue, bool bIsRead) = 0;
        virtual bool Serialize(float & rfValue, bool bIsRead) = 0;
        virtual bool Serialize(double & rdValue, bool bIsRead) = 0;
        virtual bool Serialize(char *szValue, bool bIsRead) = 0;
        virtual bool Serialize(void *pValue, const unsigned int nSize, bool bIsRead) = 0;
    };
}
#endif // __base_iserializer_h_
