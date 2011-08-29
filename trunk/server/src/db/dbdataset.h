#ifndef __db_dbdataset_h_
#define __db_dbdataset_h_

#include "dataset.h"
#include <list>

namespace db
{
    /**
     * A DataSet that wraps a result set directly from an SQL query.
     */
    class DBDataSet : public DataSet
    {
    public:
        DBDataSet();
        virtual ~DBDataSet();

        /**
         * Internally called by DBConn::Execute() to prepare a DBDataSet.
         */
        void AddResult(MYSQL * pConn, MYSQL_RES * pResult);

        /**
         * Merge ds into this, and clear ds.
         */
        void AddDataSet(DBDataSet & rDataSet);

        /**
         * Implementing DataSet.
         */
        virtual void Close();
        virtual int GetRowCount() const
        {
            return m_nRowCount;
        }
        virtual int GetColCount() const
        {
            return m_nColCount;
        }
        virtual int GetFieldIndex(const char *szFieldName);
        virtual MYSQL_FIELD * GetFields() const;
        virtual void MoveFirst();
        virtual MYSQL_ROW GetRow() const
        {
            return m_Row;
        }
        virtual void MoveNext();
        virtual const char * GetField(int nField) const;
        virtual int GetFieldLength(int nField) const;

    private:
        DBDataSet(DBDataSet & rDataSet)
        {
            //ASSERT(false);
        }// no copy constructor 
        typedef std::list < MYSQL_RES * > ResultList;
        ResultList m_ResultList;
        mutable MYSQL_FIELD * m_pFields;
        int m_nRowCount;
        int m_nColCount;

        ResultList::const_iterator m_it;
        MYSQL_ROW m_Row;
        unsigned long * m_pLengths;
    };
}

#endif // __db_dbdataset_h_
