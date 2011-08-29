#include "serverfunc.h"
#include "cardgame.h"

extern CCardGame g_CardGame;
bool SendMsg(MSG_BASE & rMsg, unsigned int nSlotIndex)
{
    return g_CardGame.SendMsg(rMsg, nSlotIndex);
}
