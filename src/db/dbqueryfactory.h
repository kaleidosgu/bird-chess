#ifndef __db_dbqueryfactory_h_
#define __db_dbqueryfactory_h_

#include "../core/factory.h"
#include "dbqueryworker.h"

using namespace core;

namespace db
{
    class DBQueryFactory : public Factory
    {
    public:
        long long AddTask(int nDBQueryTaskType, DBQuery * pDBQuery);
        static DBQueryFactory & Instance();
        static DBQueryFactory & Instance(bool & rbInitResult);
        static void DestoryInstance();
    protected:
        DBQueryFactory(unsigned int nMaxWorkerNum, unsigned int nMaxTaskNumPerWorker);
        virtual ~DBQueryFactory();

        virtual Worker * RecruitWorker();

        static DBQueryFactory * s_pDBQueryFactory;
        static Mutex s_MutexForNewInstance;
    };
}
#endif // __db_dbqueryfactory_h_
