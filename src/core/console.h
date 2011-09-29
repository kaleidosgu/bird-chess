#ifndef __core_console_h_
#define __core_console_h_

namespace core
{
    class Console
    {
    public:
        Console(bool & rbRunning);
        virtual ~Console();

        void Run();
    private:
        void _GetInput(char *szInput);
        int _ParseCommand(char *szInput);
        void _ExecuteCommand(const int nCommand);

        bool & m_rbRunning;
    };
}

#endif // __core_console_h_
