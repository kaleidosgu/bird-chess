#include "memserializer.h"
#include "iserializable.h"
#include "log.h"
#include <string>

namespace base
{
    MemSerializer::MemSerializer()
    {
        _New(0);
    }
    MemSerializer::MemSerializer(unsigned int nMaxSize)
    {
        _New(nMaxSize);
    }

    MemSerializer::~MemSerializer()
    {
        _Delete();
    }

    void MemSerializer::_New(unsigned int nMaxSize)
    {
        m_nMaxSize = nMaxSize;
        if (m_nMaxSize > 0)
        {
            m_pData = new char[m_nMaxSize];
            m_bNeedDeleteData = true;
        }
        else
        {
            m_pData = NULL;
            m_bNeedDeleteData = false;
        }
        m_pWriter = m_pData;
        m_pReader = m_pData;
    }
    void MemSerializer::_Delete()
    {
        if (m_bNeedDeleteData)
        {
            delete [] m_pData;
            m_pData = NULL;
        }
        m_pWriter = NULL;
        m_pReader = NULL;
        m_nMaxSize = 0;
    }

    bool MemSerializer::Attach(char * pData, unsigned int nSize)
    {
        _Delete();
        if (pData && nSize > 0)
        {
            m_nMaxSize = nSize;
            m_pData = pData;
            m_bNeedDeleteData = false;
            m_pWriter = m_pData + nSize;
            m_pReader = m_pData;
            return true;
        }
        else
        {
            return false;
        }
    }

    bool MemSerializer::Serialize(bool & rbValue, bool bIsRead)
    {
        return Serialize(&rbValue, sizeof(bool), bIsRead);
    }

    bool MemSerializer::Serialize(char & rsValue, bool bIsRead)
    {
        return Serialize(&rsValue, sizeof(char), bIsRead);
    }

    bool MemSerializer::Serialize(unsigned char & rsValue, bool bIsRead)
    {
        return Serialize(&rsValue, sizeof(unsigned char), bIsRead);
    }

    bool MemSerializer::Serialize(short & rnValue, bool bIsRead)
    {
        return Serialize(&rnValue, sizeof(short), bIsRead);
    }

    bool MemSerializer::Serialize(unsigned short & rnValue, bool bIsRead)
    {
        return Serialize(&rnValue, sizeof(unsigned short), bIsRead);
    }

    bool MemSerializer::Serialize(int & rnValue, bool bIsRead)
    {
        return Serialize(&rnValue, sizeof(int), bIsRead);
    }

    bool MemSerializer::Serialize(unsigned int & rnValue, bool bIsRead)
    {
        return Serialize(&rnValue, sizeof(unsigned int), bIsRead);
    }

    bool MemSerializer::Serialize(long long & rnValue, bool bIsRead)
    {
        return Serialize(&rnValue, sizeof(long long), bIsRead);
    }

    bool MemSerializer::Serialize(unsigned long long & rnValue, bool bIsRead)
    {
        return Serialize(&rnValue, sizeof(unsigned long long), bIsRead);
    }

    bool MemSerializer::Serialize(float & rfValue, bool bIsRead)
    {
        return Serialize(&rfValue, sizeof(float), bIsRead);
    }

    bool MemSerializer::Serialize(double & rdValue, bool bIsRead)
    {
        return Serialize(&rdValue, sizeof(double), bIsRead);
    }

    bool MemSerializer::Serialize(char *szValue, bool bIsRead)
    {
        size_t nLen = bIsRead ? 0 : strlen(szValue);
        bool bRes = Serialize(&nLen, sizeof(size_t), bIsRead);
        if (bRes)
        {
            bRes = Serialize(szValue, nLen, bIsRead);
            if (!bRes)
            {
                WriteLog(LEVEL_ERROR, "MemSerializer::Serialize. Serialize failed, so deserialize.\n");
                Serialize(szValue,nLen, !bIsRead);
            }
        }
        else
        {
            WriteLog(LEVEL_ERROR, "MemSerializer::Serialize. Serialize failed.\n");
        }
        return bRes;
    }

    bool MemSerializer::Serialize(void *pValue, const unsigned int nSize, bool bIsRead)
    {
        if (bIsRead)
        {
            return _Read(pValue, nSize);
        }
        else
        {
            return _Write(pValue, nSize);
        }
    }

    bool MemSerializer::_Read(void *pValue, const unsigned int nSize)
    {
        if (m_pWriter - m_pReader >= nSize)
        {
            memcpy(pValue, m_pReader, nSize);
            m_pReader += nSize;
            return true;
        }
        else
        {
            WriteLog(LEVEL_ERROR, "MemSerializer::_Read. The data size(%d) is less that request size(%d).\n", (m_pWriter - m_pReader), nSize);
            return false;
        }
    }

    bool MemSerializer::_Write(const void *pValue, const unsigned int nSize)
    {
        if (nSize <= m_pData + m_nMaxSize - m_pWriter)
        {
            memcpy(m_pWriter, pValue, nSize);
            m_pWriter += nSize;
            return true;
        }
        else
        {
            WriteLog(LEVEL_ERROR, "MemSerializer::_Write. The remain data size(%d) is less than request size(%d).\n", (m_pData + m_nMaxSize - m_pWriter), nSize);
            return false;
        }
    }
}
