#include "cardgame.h"
#include "../base/log.h"
#include "../base/assert.h"

using namespace base;

CCardGame g_CardGame;

int main()
{
    g_iniDefault.Load("./config/server.conf");
    g_CardGame.Start();
    WriteLog(LEVEL_DEBUG, "Exit gameserver.\n");
    /*
    Room room;
    Player p1;
    printf("P1' ID is %d.\n", p1.GetPlayerID());
    p1.EnterRoom(&room);
    Player p2;
    printf("P2' ID is %d.\n", p2.GetPlayerID());
    p2.EnterRoom(&room);
    p1.StartGame();
    room.PrintRoom();
    usleep(1000000);
    p1.UseCard1(CardType_Red);
    room.PrintRoom();
    usleep(1000000);
    p1.UseCard1(CardType_Red);
    room.PrintRoom();
    usleep(1000000);
    p1.UseCard1(CardType_Yellow);
    room.PrintRoom();
    usleep(1000000);
    for (int i=0; i<100; i++)
    {
        p1.UseCard1(CardType_Yellow);
        room.PrintRoom();
        usleep(1000000);
    }
    */
    return 0;
}
