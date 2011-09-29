#ifndef __base_ini_h_
#define __base_ini_h_
#include <string>
#include <list>
#include <map>
#include <sstream>
#include <iostream>

using namespace std;

namespace base
{
    typedef map < string, map < string, string > > _tMapIni;
    typedef map < string, string > _tMapIniSection;

    class Ini
    {
        public:
            int GetSameKey(const char *szKey, int nValue, string & strBuff);
            bool Load(const char *szFileName);
            int GetInt(const char *szSection, const char *szKey, int nDefault);
            int GetString(const char *szSection, const char *szKey, char *szBuff, const unsigned int cbLen, const char *szDefault);
            int GetString(const char *szSection, const char *szKey, string & strBuff, const char *szDefault);
            void SetString(const char *szSection, const char *szKey, const char *szNewValue);
            void SetIni(const char *szSection, const char *szKey, int nNewValue);
            void Clear();
            bool Save(const char *szFileName);
            bool Save(const char *szFileName, const char *szSection);
            bool Load(const string & strFileContent);
        private:
            _tMapIni m_mapOption;
    };
    bool DepartString(string & str, string & str1, string & str2);
    bool IsSection(string & str);
    bool LoadIni(_tMapIni & mapOption, const char *szFileName);
    bool CryptIni(const char *szSource, const char *szDest, const char *szKeyString);
    bool SaveIni(_tMapIni & mapOption, const char *szFileName);
    bool SaveIni(_tMapIni & mapOption, const char *szFileName, const char *szSection);
    bool LoadIni(_tMapIni & mapOption, const string & strFileContent);

    extern Ini g_iniDefault;
}
#define GIni base::g_iniDefault
#endif  // __base_ini_h_

