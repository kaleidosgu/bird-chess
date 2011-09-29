#include "dataset.h"

using namespace std;

namespace db
{
    const char * DataSet::GetStringField(int nField) const
    {
        const char *szValue = GetField(nField);
        return szValue ? szValue : "";
    }
    int DataSet::GetIntField(int nField) const
    {
        const char *szValue = GetField(nField);
        return szValue ? atoi(szValue) : 0;
    }
    unsigned int DataSet::GetUIntField(int nField) const
    {
        const char *szValue = GetField(nField);
        return szValue ? atoi(szValue) : 0;
    }
    long long DataSet::GetInt64Field(int nField) const
    {
        const char *szValue = GetField(nField);
        return szValue ? atoll(szValue) : 0;
    }
    unsigned long long DataSet::GetUInt64Field(int nField) const
    {
        const char *szValue = GetField(nField);
        return szValue ? atoll(szValue) : 0;
    }
    std::string DataSet::GetRowString() const
    {
        string ret;

        int nNonameCount = 0;
        for (int i = 0; i < GetColCount(); i++)
        {
            if (i > 0)
            {
                ret += ", ";
            }
            ret += "'";
            const char *szFieldName = GetFields()[i].name;
            if (szFieldName && *szFieldName)
            {
                ret += szFieldName;
            }
            else
            {
                ret += "Noname";
                char szNonameCount[11] = {0};
                sprintf(szNonameCount, "%d", nNonameCount);
                ret += szNonameCount;
            }
            ret += "' => ";
            const char *s = GetField(i);
            if (s)
            {
                ret += "'";
                ret += Escape(s);
                ret += "'";
            }
            else
            {
                ret += "NULL";
            }
        }
        return ret;
    }

    std::string DataSet::Escape(const char *s)
    {
        string ret;
        for (const char *p = s; *p; p++)
        {
            switch (*p)
            {
                case '\'':
                    ret += "\\'";
                    break;
                case '\r':
                    ret += "\\r";
                    break;
                case '\n':
                    ret += "\\n";
                    break;
                case '\t':
                    ret += "\\t";
                    break;
                case '\\':
                    ret += "\\\\";
                    break;
                default:
                    ret += *p;
            }
        }
        return ret;
    }
}
