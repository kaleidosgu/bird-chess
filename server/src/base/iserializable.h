#ifndef __base_iserializable_h_
#define __base_iserializable_h_

#include "iserializer.h"

namespace base
{
    class ISerializable
    {
    public:
        ISerializable(){}
        virtual ~ISerializable(){}
        virtual void Serialize(ISerializer & rSerializer, bool bIsRead) = 0;
    };
}

#endif // __base_iserializable_h_
