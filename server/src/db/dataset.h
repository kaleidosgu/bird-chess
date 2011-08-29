#ifndef __db_dataset_h_
#define __db_dataset_h_

#include <mysql/mysql.h>
#include <string>

namespace db
{
    class DataSet
    {
    public:
        virtual ~DataSet()
        {
        }
        /**
         * Close dataset and free up resources.
         */
        virtual void Close() = 0;

        /**
         * Informational functions.
         */
        virtual int GetRowCount() const = 0;
        virtual int GetColCount() const = 0;
        virtual int GetFieldIndex(const char *szFieldName) = 0;
        virtual MYSQL_FIELD * GetFields() const = 0;

        /**
         * Iteration functions.
         */
        virtual void MoveFirst() = 0;
        virtual MYSQL_ROW GetRow() const = 0;
        virtual void MoveNext() = 0;

        /**
         * Field value retrieval.
         */
        virtual const char *GetField(int nField) const = 0;
        virtual int GetFieldLength(int nField) const = 0;
        const char *GetStringField(int nField) const;	// NULL will become ""
        int GetIntField(int nField) const;
        unsigned int GetUIntField(int nField) const;
        long long GetInt64Field(int nField) const;
        unsigned long long GetUInt64Field(int nField) const;

        /**
         * Returns a one-liner of all fields of current row.
         */
        std::string GetRowString() const;

    private:

        /**
         * Helper function for getRowString().
         */
        static std::string Escape(const char *s);
    };

    ///////////////////////////////////////////////////////////////////////////////
}

#endif // __db_dataset_h_
