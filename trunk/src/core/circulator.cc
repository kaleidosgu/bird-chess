#include "circulator.h"
#include "../base/log.h"
#include <sys/time.h>
#include "console.h"

using namespace core;

Circulator::Circulator()
    //: m_Condition(m_Mutex)
{

    m_nPeriodMS = 25;
    m_bRunning = false;
}

Circulator::~Circulator()
{
}

void Circulator::SetPeriod(unsigned int nPeriodMS)
{
    if (nPeriodMS == 0)
    {
        m_nPeriodMS = 25;
    }
    else
    {
        m_nPeriodMS = nPeriodMS;
    }
}

bool Circulator::PreProcess()
{
    return true;
}

void Circulator::Process()
{
    if (!PreProcess())
    {
        WriteLog(LEVEL_DEBUG, "Circulator Init Failed.\n");
        return;
    }
    //struct timeval tv;
    //gettimeofday(&tv, NULL);

    //struct timespec timeout;
    //timeout.tv_sec = tv.tv_sec;
    //timeout.tv_nsec = tv.tv_usec * 1000;

    ftime(&m_tbLast);

    struct timeb tbStart;
    ftime(&tbStart);
    struct timeb tbNow;
    unsigned int nCurrentInterval = 0;
    unsigned int nNextInterval = m_nPeriodMS;

    m_bRunning = true;

    while (m_bRunning)
    {
        //m_Mutex.Lock();
        //m_Condition.TimedWait(&timeout);
        //m_Mutex.Unlock();
        Pump();
        //timeout.tv_sec += m_nPeriodMS / 1000;
        //timeout.tv_nsec += (m_nPeriodMS % 1000) * 1000000;
        //if (timeout.tv_nsec >= 1000000000)
        //{
        //timeout.tv_sec++;
        //timeout.tv_nsec -= 1000000000;
        //}
        //WriteLog(LEVEL_DEBUG, "%d,%d", timeout.tv_sec, timeout.tv_nsec);

        ftime(&tbNow);
        nCurrentInterval = (tbNow.time - tbStart.time) * 1000 + tbNow.millitm - tbStart.millitm;

        // for correct the clock
        {
            if (nNextInterval - nCurrentInterval > m_nPeriodMS * 2 )
            {
                WriteLog(LEVEL_WARNING, "The clock turn back. Interval = (%d). Period=(%d).\n", nNextInterval - nCurrentInterval, m_nPeriodMS);
                ftime(&tbStart);
                nNextInterval = m_nPeriodMS;
                nCurrentInterval = 0;
                continue;
            }
            if (nCurrentInterval > (nNextInterval + (m_nPeriodMS * 2)))
            {
                WriteLog(LEVEL_WARNING, "The server is too busy. Adjust the clock. Interval=%d, PeriodMS=%d.\n", nCurrentInterval - nNextInterval,  m_nPeriodMS);
                ftime(&tbStart);
                nNextInterval = m_nPeriodMS;
                nCurrentInterval = 0;
            }
        }

        if (nCurrentInterval < nNextInterval)
        {
            usleep((nNextInterval-nCurrentInterval) * 1000);
        }
        else
        {
            WriteLog(LEVEL_DEBUG, "Server is busy. Interval=%d.\n", nCurrentInterval - nNextInterval);
        }

        nNextInterval += m_nPeriodMS;

        // for more longer running
        /*
        if (nNextInterval > 0xEFFFFFFF)
        {
            ftime(&tbStart);
            nNextInterval = m_nPeriodMS;
            nCurrentInterval = 0;
        }
        */
    }

    EndProcess();

    WriteLog(LEVEL_INFO, "Circulator::Process. Process Complete.\n");
}

void Circulator::EndProcess()
{
}


void Circulator::Pump()
{
    struct timeb tbNow;
    ftime(&tbNow);
    int nInterval = (tbNow.time - m_tbLast.time) * 1000 + tbNow.millitm - m_tbLast.millitm;
    if (nInterval > 0)
    {
        HeartBeat(nInterval);
    }
    else if (nInterval < 0)
    {
        WriteLog(LEVEL_WARNING, "The clock turn back(%d).\n", nInterval);
    }
    m_tbLast.time = tbNow.time;
    m_tbLast.millitm = tbNow.millitm;
}

bool Circulator::Initialize()
{
    return true;
}

bool Circulator::Start(bool bCreateThread)
{
    if (!Initialize())
    {
        WriteLog(LEVEL_ERROR, "Circulator::Start. Initialize failed.\n");
        return false;
    }
    WriteLog(LEVEL_DEBUG, "Initialize Completed.\n");
    bool bRes = true;

    m_bRunning = true;
    if (bCreateThread)
    {
        Run();
        Console console(m_bRunning);
        console.Run();
    }
    else
    {
        Process();
    }
    Destory();
    return bRes;
}

void Circulator::Stop()
{
    m_bRunning = false;
}


