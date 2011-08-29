#include "dbdataset.h"

namespace db
{
    ///////////////////////////////////////////////////////////////////////////////

    DBDataSet::DBDataSet() :
        m_pFields(NULL),
        m_nRowCount(0),
        m_nColCount(0),
        m_Row(NULL),
        m_pLengths(NULL)
    {
    }
    DBDataSet::~DBDataSet()
    {
        Close();
    }

    void DBDataSet::AddResult(MYSQL * pConn, MYSQL_RES * pResult)
    {
        if (pResult == NULL)
        {
            return;
        }

        int nRowCount = mysql_num_rows(pResult);
        if (nRowCount)
        {
            int nFieldCount = (int)mysql_field_count(pConn);
            if (m_nColCount == 0)
            {
                m_nColCount = nFieldCount;
            }
            else
            {
                //ASSERT(m_nColCount == nFieldCount);
                if (m_nColCount > nFieldCount)
                {
                    m_nColCount = nFieldCount;	// in case we overflow m_Row later
                }
            }

            m_nRowCount += nRowCount;
            m_ResultList.push_back(pResult);
        }
        else
        {
            mysql_free_result(pResult);
        }
    }

    void DBDataSet::AddDataSet(DBDataSet & rDataSet)
    {
        if (rDataSet.m_ResultList.empty())
        {
            return;
        }

        if (m_nColCount == 0)
        {
            m_nColCount = rDataSet.m_nColCount;
        }
        else
        {
            //ASSERT(m_nColCount == rDataSet.m_nColCount);
        }

        m_nRowCount += rDataSet.m_nRowCount;
        m_ResultList.insert(m_ResultList.end(), rDataSet.m_ResultList.begin(), rDataSet.m_ResultList.end());
        rDataSet.m_ResultList.clear();
        rDataSet.Close();
    }

    void DBDataSet::Close()
    {
        for (m_it = m_ResultList.begin(); m_it != m_ResultList.end(); ++m_it)
        {
            mysql_free_result(*m_it);
        }
        m_ResultList.clear();
        m_pFields = NULL;
        m_Row = NULL;
        m_pLengths = NULL;
        m_nRowCount = 0;
        m_nColCount = 0;
    }

    int DBDataSet::GetFieldIndex(const char *szFieldName)
    {
        //ASSERT(szFieldName && *szFieldName);

        // without any results, cannot really resolve field names
        if (m_ResultList.empty())
        {
            return -1;
        }

        if (m_pFields == NULL)
        {
            m_pFields = mysql_fetch_fields(m_ResultList.front());
        }

        for (int i = 0; i < m_nColCount; i++)
        {
            if (strcmp(m_pFields[i].name, szFieldName) == 0)
            {
                return i;
            }
        }
        return -1;
    }

    MYSQL_FIELD * DBDataSet::GetFields() const
    {
        if (m_pFields == NULL && !m_ResultList.empty())
        {
            m_pFields = mysql_fetch_fields(m_ResultList.front());
        }
        return m_pFields;
    }

    void DBDataSet::MoveFirst()
    {
        for (m_it = m_ResultList.begin(); m_it != m_ResultList.end(); ++m_it)
        {
            if (*m_it && mysql_num_rows(*m_it) > 0)
            {
                mysql_data_seek(*m_it, 0);
                m_Row = mysql_fetch_row(*m_it);
                m_pLengths = mysql_fetch_lengths(*m_it);
                //ASSERT(m_Row);
                //ASSERT(m_pLengths);
                return;
            }
        }
        m_Row = NULL;
        m_pLengths = NULL;
    }

    void DBDataSet::MoveNext()
    {
        if (m_it != m_ResultList.end())
        {
            if (*m_it)
            {
                m_Row = mysql_fetch_row(*m_it);
                m_pLengths = mysql_fetch_lengths(*m_it);
                if (m_Row)
                    return;
            }
            for (++m_it; m_it != m_ResultList.end(); ++m_it)
            {
                if (*m_it && mysql_num_rows(*m_it) > 0)
                {
                    mysql_data_seek(*m_it, 0);
                    m_Row = mysql_fetch_row(*m_it);
                    m_pLengths = mysql_fetch_lengths(*m_it);
                    //ASSERT(m_Row);
                    //ASSERT(m_pLengths);
                    return;
                }
            }
        }
        m_Row = NULL;
        m_pLengths = NULL;
    }

    const char * DBDataSet::GetField(int nField) const
    {
        //ASSERT(nField >= 0 && nField < m_nColCount);
        //ASSERT(m_Row);
        if (m_Row && nField >= 0 && nField < m_nColCount)
        {
            return m_Row[nField];
        }
        return NULL;
    }

    int DBDataSet::GetFieldLength(int nField) const
    {
        //ASSERT(nField >= 0 && nField < m_nColCount);
        //ASSERT(m_pLengths);
        if (m_pLengths && nField >= 0 && nField < m_nColCount)
        {
            return m_pLengths[nField];
        }
        return 0;
    }
}
