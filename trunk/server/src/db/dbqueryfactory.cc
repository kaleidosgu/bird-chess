#include "dbqueryfactory.h"
#include "../base/log.h"
#include "../base/ini.h"

using namespace base;

extern base::Logger db::g_loggerDBQuery;

namespace db
{
    DBQueryFactory::DBQueryFactory(unsigned int nMaxWorkerNum, unsigned int nMaxTaskNumPerWorker)
        : Factory(nMaxWorkerNum, nMaxTaskNumPerWorker)
    {
    }

    DBQueryFactory::~DBQueryFactory()
    {
        //WriteLog(LEVEL_DEBUG, "DBQueryFactory::~DBQueryFactory.\n");
    }
    
    long long DBQueryFactory::AddTask(int nDBQueryTaskType, DBQuery * pDBQuery)
    {
        DBQueryTask * pDBQueryTask = new DBQueryTask(nDBQueryTaskType);
        pDBQueryTask->AddDBQuery(pDBQuery);
        if (Factory::AddTask(pDBQueryTask))
        {
            return pDBQueryTask->GetTaskID();
        }
        else
        {
            WriteLog(LEVEL_WARNING, "DBQueryFactory::AddTask. AddTask Failed.\n");
            delete pDBQueryTask;
            pDBQueryTask = NULL;
            return -1;
        }
    }

    Worker * DBQueryFactory::RecruitWorker()
    {
        WriteLog(LEVEL_DEBUG, "Hello, new dbquery worker.\n");
        DBQueryWorker * pNewWorker = new DBQueryWorker();
        return pNewWorker;
    }

    DBQueryFactory * DBQueryFactory::s_pDBQueryFactory = NULL;
    Mutex DBQueryFactory::s_MutexForNewInstance;

    DBQueryFactory & DBQueryFactory::Instance()
    {
        bool bInitResult = true;
        return Instance(bInitResult);
    }
    DBQueryFactory & DBQueryFactory::Instance(bool & rbInitResult)
    {
        rbInitResult = true;
        if (s_pDBQueryFactory == NULL)
        {
            Mutex::ScopedLock lock(s_MutexForNewInstance);
            if (s_pDBQueryFactory == NULL)
            {
                g_loggerDBQuery.SetLogMode(LOGMODE_STDOUT | LOGMODE_FILE, "logs/dbquery.log");
                g_loggerDBQuery.Write("Start DBQuery Logging\n");
                unsigned int nMaxWorkerNum = GIni.GetInt("DBQueryFactory", "MaxWorkerNum", 1);
                unsigned int nMaxTaskNumPerWorker = GIni.GetInt("DBQueryFactory", "MaxTaskNumPerWorker", 1);
                s_pDBQueryFactory = new DBQueryFactory(nMaxWorkerNum, nMaxTaskNumPerWorker);
                rbInitResult = s_pDBQueryFactory->Init();
            }
        }
        return *s_pDBQueryFactory;
    }
    void DBQueryFactory::DestoryInstance()
    {
        Mutex::ScopedLock lock(s_MutexForNewInstance);
        delete s_pDBQueryFactory;
        s_pDBQueryFactory = NULL;
    }
}
