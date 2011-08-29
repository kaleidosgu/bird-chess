#include <stdio.h>
#include "ini.h"
#include "crypt.h"
#include "md5.h"
#include "util.h"

#ifndef MAX_STRING
#define MAX_STRING 256
#endif

namespace base
{
    using namespace util;
    Ini g_iniDefault;

    int Ini::GetSameKey(const char *szKey, int nValue, string & strBuff)
    {
        char szValue[MAX_STRING];
        sprintf(szValue, "%d", nValue);
        int nRes = 0;
        for (_tMapIni::iterator it = m_mapOption.begin(); it != m_mapOption.end(); ++it)
        {
            for (_tMapIniSection::iterator itSec = it->second.begin(); itSec != it->second.end(); ++itSec)
            {
                if (strcmp(szKey, itSec->first.c_str()) == 0
                        && strcmp(szValue, itSec->second.c_str()) == 0)
                {
                    strBuff += it->first;
                    strBuff += "|";
                    nRes++;
                }
            }
        }
        return nRes;
    }

    bool Ini::Load(const char *szFileName)
    {
        return LoadIni(m_mapOption, szFileName);
    }

    bool Ini::Load(const string & strFileContent)
    {
        return LoadIni(m_mapOption, strFileContent);
    }

    bool Ini::Save(const char *szFileName)
    {
        return SaveIni(m_mapOption, szFileName);
    }

    bool Ini::Save(const char *szFileName, const char *szSection)
    {
        return SaveIni(m_mapOption, szFileName, szSection);
    }

    void Ini::Clear()
    {
        m_mapOption.clear();
    }

    int Ini::GetInt(const char *szSection, const char *szKey, int nDefault)
    {
        _tMapIni::iterator it = m_mapOption.find(szSection);
        if (it != m_mapOption.end())
        {
            _tMapIniSection::iterator itValue = it->second.find(szKey);
            if (itValue != it->second.end())
            {
                return atoi(itValue->second.c_str());
            }
        }
        return nDefault;
    }

    int Ini::GetString(const char *szSection, const char *szKey, string & strBuff, const char *szDefault)
    {
        char szBuff[MAX_STRING] = "";
        int nRes = GetString(szSection, szKey, szBuff, MAX_STRING, szDefault);
        strBuff = szBuff;
        return nRes;
    }

    int Ini::GetString(const char *szSection, const char *szKey, char *szBuff, const unsigned int cbLen, const char *szDefault)
    {
        _tMapIni::iterator it = m_mapOption.find(szSection);
        if (it != m_mapOption.end())
        {
            _tMapIniSection::iterator itValue = it->second.find(szKey);
            if (itValue != it->second.end())
            {
                if (cbLen <= itValue->second.size())
                {
                    return 0;
                }
                else
                {
                    strcpy(szBuff, itValue->second.c_str());
                    return itValue->second.size();
                }
            }
        }
        if (szDefault != NULL && cbLen > strlen(szDefault))
        {
            strcpy(szBuff, szDefault);
            return strlen(szDefault);
        }
        else
        {
            return 0;
        }
    }

    void Ini::SetIni(const char *szSection, const char *szKey, int nNewValue)
    {
        char szBuff[MAX_STRING];
        sprintf(szBuff, "%d", nNewValue);
        SetString(szSection, szKey, szBuff);
    }

    void Ini::SetString(const char *szSection, const char *szKey, const char *szNewValue)
    {
        string str = szNewValue;
        string strKey = szKey;
        string strSec = szSection;

        _tMapIni::iterator it = m_mapOption.find(szSection);
        if (it != m_mapOption.end())
        {
            it->second[strKey] = str;
        }
        else
        {
            _tMapIniSection sec;
            sec[strKey] = str;
            m_mapOption[strSec] = sec;
        }
    }

    bool DepartString(string & str, string & str1, string & str2)
    {
        TrimLeft(str);
        TrimLeft(str, '\t');
        TrimLeft(str);
        TrimRight(str);
        TrimRight(str, '\t');
        TrimRight(str);
        if (str.size() > 0)
        {
            int nEqu = str.find_first_of('=');
            if (nEqu > 0 && (unsigned) nEqu < str.size() - 1)
            {
                str1 = str.substr(0, nEqu);
                TrimRight(str1);
                TrimRight(str1, '\t');
                str2 = str.substr(nEqu + 1, str.size() - nEqu - 1);
                TrimLeft(str2);
                TrimLeft(str2, '\t');
                return true;
            }
        }
        return false;
    }

    bool IsSection(string & str)
    {
        TrimCR(str);
        TrimLeft(str);
        TrimRight(str);
        TrimLeft(str, '\t');
        TrimRight(str, '\t');
        if (str[0] == '[' && str[str.size() - 1] == ']')
        {
            str = str.substr(1, str.size() - 2);
            return true;
        }
        else
        {
            return false;
        }
    }

#ifndef BUFF_LEN
#define BUFF_LEN 200
#endif

    bool LoadIni(_tMapIni & mapOption, const string & strFileContent)
    {
        char szBuff[BUFF_LEN];
        string str, str1, str2;
        pair < string, _tMapIniSection > section;
        istringstream istr(strFileContent);
        if (istr.peek())
        {
            while (istr.getline(szBuff, BUFF_LEN) != NULL)
            {
                str = szBuff;
                if (IsSection(str))
                {
                    section.first = str;
                    mapOption.insert(section);
                }
                else if (DepartString(str, str1, str2))
                {
                    mapOption[section.first][str1] = str2;
                }
            }
            return true;
        }
        return false;
    }

    bool LoadIni(_tMapIni & mapOption, const char *szFileName)
    {
        char szBuff[BUFF_LEN];
        string str, str1, str2;
        pair < string, _tMapIniSection > section;
        FILE *fp;
        if ((fp = fopen(szFileName, "r")) != NULL)
        {
            if (fgetc(fp) != 0)
            {
                fseek(fp, SEEK_SET, 0);
                while (fgets(szBuff, BUFF_LEN, fp) != NULL)
                {
                    str = szBuff;
                    if (IsSection(str))
                    {
                        section.first = str;
                        mapOption.insert(section);
                    }
                    else if (DepartString(str, str1, str2))
                    {
                        mapOption[section.first][str1] = str2;
                    }
                }
            }
            else
            {
                char szKey[16];
                char szKeyKey[16];
                fread(szKey, 16, 1, fp);
                fread(szKeyKey, 16, 1, fp);
                int nLen;
                CryptDes des;
                while (!feof(fp) && 0 != (nLen = fgetc(fp)))
                {
                    fread(szBuff, nLen, 1, fp);
                    des.Des_Go(szKey, szKey, 16, szKeyKey, 16, ENCRYPT);
                    des.Des_Go(szBuff, szBuff, 16, szKey, 16, DECRYPT);
                    str = szBuff;
                    if (IsSection(str))
                    {
                        section.first = str;
                        mapOption.insert(section);
                    }
                    else if (DepartString(str, str1, str2))
                    {
                        mapOption[section.first][str1] = str2;
                    }
                }
            }
            fclose(fp);
            return true;
        }
        return false;
    }

    bool SaveIni(_tMapIni & mapOption, const char *szFileName, const char *szSection)
    {
        FILE *fp = fopen(szFileName, "w");
        if (fp == NULL)
        {
            return false;
        }
        _tMapIniSection mpSection = mapOption[szSection];
        fprintf(fp, "[%s]\r\n", szSection);
        for (_tMapIniSection::iterator itSec = mpSection.begin(); itSec != mpSection.end(); ++itSec)
        {
            fprintf(fp, "%s=%s\r\n", itSec->first.c_str(), itSec->second.c_str());
        }
        fprintf(fp, "\r\n");
        fclose(fp);
        return true;
    }

    bool SaveIni(_tMapIni & mapOption, const char *szFileName)
    {
        FILE *fp = fopen(szFileName, "w");
        if (fp == NULL)
        {
            return false;
        }
        for (_tMapIni::iterator it = mapOption.begin(); it != mapOption.end(); ++it)
        {
            fprintf(fp, "[%s]\r\n", it->first.c_str());
            for (_tMapIniSection::iterator itSec = it->second.begin(); itSec != it->second.end(); ++itSec)
            {
                fprintf(fp, "%s=%s\r\n", itSec->first.c_str(), itSec->second.c_str());
            }
            fprintf(fp, "\r\n");
        }
        fclose(fp);
        return true;
    }

    bool CrypIni(const char *szSource, const char *szDest, const char *szKeyString)
    {
        if (szKeyString == NULL || strlen(szKeyString) < 2)
        {
            return false;
        }
        char szBuff[BUFF_LEN];
        MD5Digest digestKey;
        MD5Digest digestKeyKey;
        MD5String((char *) szKeyString, &digestKey);
        MD5String((char *) szKeyString + 1, &digestKeyKey);
        FILE *fpSrc = fopen(szSource, "r");
        bool bRes = false;
        if (fpSrc != NULL)
        {
            FILE *fpDst = fopen(szDest, "w");
            if (fpDst != NULL)
            {
                fputc(0, fpDst);
                fwrite(digestKey.a, 16, 1, fpDst);
                fwrite(digestKeyKey.a, 16, 1, fpDst);
                CryptDes des;
                while (fgets(szBuff, BUFF_LEN, fpSrc) != NULL)
                {
                    int nLen = strlen(szBuff);
                    nLen = nLen + 8 & 0x000000F8;
                    fputc(nLen, fpDst);
                    des.Des_Go((char*)(digestKey.a), (char*)(digestKey.a), 16, (const char*)(digestKeyKey.a), 16, ENCRYPT);
                    des.Des_Go(szBuff, szBuff, nLen, (const char *)(digestKey.a), 16, ENCRYPT);
                    fwrite(szBuff, nLen, 1, fpDst);
                }
                putc(0, fpDst);
                fclose(fpDst);
                bRes = true;
            }
            fclose(fpSrc);
        }
        return bRes;
    }
}


