#ifndef __net_senddataelement_h_
#define __net_senddataelement_h_

#include <sys/uio.h>

struct MSG_BASE;
namespace net
{
    class CSendDataElement : protected iovec
    {
        public:
            CSendDataElement();
            ~CSendDataElement();

            void SetData(MSG_BASE * pMsg);
            void DeleteData();
            MSG_BASE * GetMsgData() const;
            unsigned short GetMsgDataSize() const;
            iovec * GetIovec();
    };
}
#endif // __net_senddataelement_h_

