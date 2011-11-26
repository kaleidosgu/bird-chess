#ifndef __cardgame_cardgamemsg_h_
#define __cardgame_cardgamemsg_h_

#include "../net/msgbasic.h"
#include "../net/systemmsg.h"
#include "basedefine.h"

#pragma pack(1)
enum
{
    MSGID_CARDGAME_Begin = 0x1000,
    MSGID_CARDGAME_C2S_LoginRequest,
    MSGID_CARDGAME_S2C_LoginResult,
    MSGID_CARDGAME_S2C_PlayerInfo,
    MSGID_CARDGAME_S2C_RoomInfo,
    MSGID_CARDGAME_C2S_NextRoomPage,
    MSGID_CARDGAME_C2S_CreateRoom,
    MSGID_CARDGAME_S2C_CreateRoomResult,
    MSGID_CARDGAME_C2S_EnterRoom,
    MSGID_CARDGAME_S2C_EnterRoomResult,
    MSGID_CARDGAME_C2S_LeaveRoom,
    MSGID_CARDGAME_S2C_LeaveRoomResult,
    MSGID_CARDGAME_C2S_StartGame,
    MSGID_CARDGAME_S2C_StartGameFailed,
    MSGID_CARDGAME_C2S_UseCard,
    MSGID_CARDGAME_S2C_UseCardFailed,
    MSGID_CARDGAME_S2C_PlayerEnter,
    MSGID_CARDGAME_S2C_RoomMaster,
    MSGID_CARDGAME_S2C_PlayerLeave,
    MSGID_CARDGAME_S2C_StartGame,
    MSGID_CARDGAME_S2C_SetCardType,
    MSGID_CARDGAME_S2C_AddNewCard,
    MSGID_CARDGAME_S2C_CurrentTurnPlayer,
    MSGID_CARDGAME_S2C_UseCard,
    MSGID_CARDGAME_S2C_MoveCard,
    MSGID_CARDGAME_S2C_Win,
    MSGID_CARDGAME_S2C_KickOffPlayer,

    MSGID_CARDGAME_X2X_ChatContent,
	
};

struct MSG_CARDGAME_C2S_LoginRequest : public MSG_BASE
{
    MSG_CARDGAME_C2S_LoginRequest()
    {
        nMsg = MSGID_CARDGAME_C2S_LoginRequest;
        nSize = sizeof(MSG_CARDGAME_C2S_LoginRequest);
        memset(szUsername, 0, cMAX_USERNAME_LEN);
        memset(szPassword, 0, cMAX_PASSWORD_LEN);
    }
    char szUsername[cMAX_USERNAME_LEN];
    char szPassword[cMAX_PASSWORD_LEN];
};

struct MSG_CARDGAME_S2C_LoginResult : public MSG_BASE
{
    enum Result
    {
        Result_Success,
        Result_Relogin,
        Result_Failed,
    };
    MSG_CARDGAME_S2C_LoginResult()
    {
        nMsg = MSGID_CARDGAME_S2C_LoginResult;
        nSize = sizeof(MSG_CARDGAME_S2C_LoginResult);
        nResult = Result_Success;
    }
    int nResult;
};

struct MSG_CARDGAME_S2C_PlayerInfo : public MSG_BASE
{
    MSG_CARDGAME_S2C_PlayerInfo()
    {
        nMsg = MSGID_CARDGAME_S2C_PlayerInfo;
        nSize = sizeof(MSG_CARDGAME_S2C_PlayerInfo);
        nPlayerID = -1;
        memset(szPlayerName, 0, cMAX_USERNAME_LEN); 
        nScore = 0;
        nWin = 0;
        nDogfall = 0;
        nLose = 0;
    }
    int nPlayerID;
    char szPlayerName[cMAX_USERNAME_LEN];
    int nScore;
    int nWin;
    int nDogfall;
    int nLose;
};

struct RoomInfo
{
    RoomInfo()
    {
        nRoomID = -1;
        nRoomIndex = -1;
        nPlayerCount = 0;
    }
    int nRoomID;
    int nRoomIndex;
    int nPlayerCount;
};

struct MSG_CARDGAME_S2C_RoomInfo : public MSG_BASE
{
    MSG_CARDGAME_S2C_RoomInfo()
    {
        nMsg = MSGID_CARDGAME_S2C_RoomInfo;
        nSize = sizeof(MSG_CARDGAME_S2C_RoomInfo);
    }
    int nRoomNumber;
    int nCurrentPage;
    RoomInfo aRoomInfo[cMAX_PAGE_ROOM_NUM];
};

struct MSG_CARDGAME_C2S_NextRoomPage : public MSG_BASE
{
    MSG_CARDGAME_C2S_NextRoomPage()
    {
        nMsg = MSGID_CARDGAME_C2S_NextRoomPage;
        nSize = sizeof(MSG_CARDGAME_C2S_NextRoomPage);
    }
    int nNextPageID;
};
struct MSG_CARDGAME_C2S_CreateRoom : public MSG_BASE
{
    MSG_CARDGAME_C2S_CreateRoom()
    {
        nMsg = MSGID_CARDGAME_C2S_CreateRoom;
        nSize = sizeof(MSG_CARDGAME_C2S_CreateRoom);
    }
};

struct MSG_CARDGAME_S2C_CreateRoomResult : public MSG_BASE
{
    enum Result
    {
        Result_Success,
        Result_PlayerNotExist,
        Result_PlayerIsNotInGame,
    };
    MSG_CARDGAME_S2C_CreateRoomResult()
    {
        nMsg = MSGID_CARDGAME_S2C_CreateRoomResult;
        nSize = sizeof(MSG_CARDGAME_S2C_CreateRoomResult);
        nResult = Result_Success;
        nRoomID = -1;
    }
    int nResult;
    int nRoomID;
};

struct MSG_CARDGAME_C2S_EnterRoom : public MSG_BASE
{
    MSG_CARDGAME_C2S_EnterRoom()
    {
        nMsg = MSGID_CARDGAME_C2S_EnterRoom;
        nSize = sizeof(MSG_CARDGAME_C2S_EnterRoom);
        nRoomID = -1;
    }
    int nRoomID;
};

struct MSG_CARDGAME_S2C_EnterRoomResult : public MSG_BASE
{
    enum Result
    {
        Result_Success,
        Result_PlayerNotExist,
        //Result_PlayerIsNotInGame,
        Result_RoomNotExist,
        Result_RoomIsInvalid,
        Result_GameHasStarted,
        Result_RoomIsFull,
        Result_PlayerRoomError,
    };
    MSG_CARDGAME_S2C_EnterRoomResult()
    {
        nMsg = MSGID_CARDGAME_S2C_EnterRoomResult;
        nSize = sizeof(MSG_CARDGAME_S2C_EnterRoomResult);
        nResult = Result_Success;
        nRoomID = -1;
    }
    int nResult;
    int nRoomID;
};

struct MSG_CARDGAME_C2S_LeaveRoom : public MSG_BASE
{
    MSG_CARDGAME_C2S_LeaveRoom()
    {
        nMsg = MSGID_CARDGAME_C2S_LeaveRoom;
        nSize = sizeof(MSG_CARDGAME_C2S_LeaveRoom);
    }
};

struct MSG_CARDGAME_S2C_LeaveRoomResult : public MSG_BASE
{
    enum Result
    {
        Result_Success,
        Result_Failed,
    };
    MSG_CARDGAME_S2C_LeaveRoomResult()
    {
        nMsg = MSGID_CARDGAME_S2C_LeaveRoomResult;
        nSize = sizeof(MSG_CARDGAME_S2C_LeaveRoomResult);
        nResult = Result_Success;
    }
    int nResult;
};

struct MSG_CARDGAME_C2S_StartGame : public MSG_BASE
{
    MSG_CARDGAME_C2S_StartGame()
    {
        nMsg = MSGID_CARDGAME_C2S_StartGame;
        nSize = sizeof(MSG_CARDGAME_C2S_StartGame);
    }
};

struct MSG_CARDGAME_S2C_StartGame : public MSG_BASE
{
    MSG_CARDGAME_S2C_StartGame()
    {
        nMsg = MSGID_CARDGAME_S2C_StartGame;
        nSize = sizeof(MSG_CARDGAME_S2C_StartGame);
    }
};

struct MSG_CARDGAME_S2C_StartGameFailed : public MSG_BASE
{
    enum Reason
    {
        Reason_Success, // It is impossible
        Reason_RoomIsInvalid,
        Reason_GameHasStarted,
        Reason_NotRoomMaster,
        Reason_PlayerIsNotEnough,
        Reason_PlayerRoomError,
    };
    MSG_CARDGAME_S2C_StartGameFailed()
    {
        nMsg = MSGID_CARDGAME_S2C_StartGameFailed;
        nSize = sizeof(MSG_CARDGAME_S2C_StartGameFailed);
        nReason = Reason_Success;
    }
    int nReason;
};

struct MSG_CARDGAME_C2S_UseCard : public MSG_BASE
{
    MSG_CARDGAME_C2S_UseCard()
    {
        nMsg = MSGID_CARDGAME_C2S_UseCard;
        nSize = sizeof(MSG_CARDGAME_C2S_UseCard);
        nCardPos = -1;
        nCardType = -1;
    }
    int nCardPos;
    int nCardType;
};

struct MSG_CARDGAME_S2C_UseCardFailed : public MSG_BASE
{
    enum Reason
    {
        Reason_Success,
        Reason_CardPosIsError,
        Reason_CardIndexError,
        Reason_CardTypeIsError,
        Reason_CardIsError,
        Reason_PlayerRoomError,
        Reason_IsNotYourTurn,
        Reason_PathError,
        Reason_GameNotStarted,
    };
    MSG_CARDGAME_S2C_UseCardFailed()
    {
        nMsg = MSGID_CARDGAME_S2C_UseCardFailed;
        nSize = sizeof(MSG_CARDGAME_S2C_UseCardFailed);
        nReason = Reason_Success;
    }
    int nReason;
};

struct MSG_CARDGAME_S2C_PlayerEnter : public MSG_CARDGAME_S2C_PlayerInfo
{
    MSG_CARDGAME_S2C_PlayerEnter()
    {
        nMsg = MSGID_CARDGAME_S2C_PlayerEnter;
        nSize = sizeof(MSG_CARDGAME_S2C_PlayerEnter);
    }
};

struct MSG_CARDGAME_S2C_RoomMaster : public MSG_BASE
{
    MSG_CARDGAME_S2C_RoomMaster()
    {
        nMsg = MSGID_CARDGAME_S2C_RoomMaster;
        nSize = sizeof(MSG_CARDGAME_S2C_RoomMaster);
        nPlayerID = -1;
    }
    int nPlayerID;
};

struct MSG_CARDGAME_S2C_PlayerLeave : public MSG_BASE
{
    MSG_CARDGAME_S2C_PlayerLeave()
    {
        nMsg = MSGID_CARDGAME_S2C_PlayerLeave;
        nSize = sizeof(MSG_CARDGAME_S2C_PlayerLeave);
        nPlayerID = -1;
    }
    int nPlayerID;
};

struct MSG_CARDGAME_S2C_SetCardType : public MSG_BASE
{
    MSG_CARDGAME_S2C_SetCardType()
    {
        nMsg = MSGID_CARDGAME_S2C_SetCardType;
        nSize = sizeof(MSG_CARDGAME_S2C_SetCardType);
        nCardType = -1;
    }
    int nCardType;
};

struct MSG_CARDGAME_S2C_AddNewCard : public MSG_BASE
{
    MSG_CARDGAME_S2C_AddNewCard()
    {
        nMsg = MSGID_CARDGAME_S2C_AddNewCard;
        nSize = sizeof(MSG_CARDGAME_S2C_AddNewCard);
        nCardPos = -1;
        nCardID = -1;
        nCardType = -1;
        nCardInstruction = 0;
    }
    unsigned int nCardPos;
    int nCardID;
    int nCardType;
    int nCardInstruction;
};

struct MSG_CARDGAME_S2C_CurrentTurnPlayer : public MSG_BASE
{
    MSG_CARDGAME_S2C_CurrentTurnPlayer()
    {
        nMsg = MSGID_CARDGAME_S2C_CurrentTurnPlayer;
        nSize = sizeof(MSG_CARDGAME_S2C_CurrentTurnPlayer);
        nPlayerID = -1;
    }
    int nPlayerID;
};

struct MSG_CARDGAME_S2C_UseCard : public MSG_BASE
{
    MSG_CARDGAME_S2C_UseCard()
    {
        nMsg = MSGID_CARDGAME_S2C_UseCard;
        nSize = sizeof(MSG_CARDGAME_S2C_UseCard);
        nCardType = -1;
        nCardInstruction = 0;
        nTargetCardType = -1;
    }
    int nCardType;
    int nCardInstruction;
    int nTargetCardType;
};

struct MSG_CARDGAME_S2C_MoveCard : public MSG_BASE
{
    MSG_CARDGAME_S2C_MoveCard()
    {
        nMsg = MSGID_CARDGAME_S2C_MoveCard;
        nSize = sizeof(MSG_CARDGAME_S2C_MoveCard);
        nCardType = -1;
        nSrcPathID = -1;
        nDesPathID = -1;
    }
    int nCardType;
    int nSrcPathID;
    int nDesPathID;
};

struct MSG_CARDGAME_S2C_Win : public MSG_BASE
{
    MSG_CARDGAME_S2C_Win()
    {
        nMsg = MSGID_CARDGAME_S2C_Win;
        nSize = sizeof(MSG_CARDGAME_S2C_Win);
		nWinCardType = -1;
		nLoseCardType = -1;
        nPlayerID = -1;
    }
    int nPlayerID;
	int nWinCardType;
	int nLoseCardType;
};

struct MSG_CARDGAME_S2C_KickOffPlayer : public MSG_BASE
{
    enum Reason
    {
        Reason_Unknown,
        Reason_Relogin,
    };
    MSG_CARDGAME_S2C_KickOffPlayer()
    {
        nMsg = MSGID_CARDGAME_S2C_KickOffPlayer;
        nSize = sizeof(MSG_CARDGAME_S2C_KickOffPlayer);
        nReason = Reason_Unknown;
    }
    int nReason;
};

struct MSG_CARDGAME_X2X_ChatContent : public MSG_BASE
{
    enum Channel
    {
        Channel_World,
        Channel_Room,
    };
    MSG_CARDGAME_X2X_ChatContent()
    {
        nMsg = MSGID_CARDGAME_X2X_ChatContent;
        nSize = sizeof(MSG_CARDGAME_X2X_ChatContent);
        nChannel = Channel_Room;
    }
    int nChannel;
    char pData[1];
};

#pragma pack()
#endif // __cardgame_cardgamemsg_h_
