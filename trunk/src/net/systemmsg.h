#ifndef __net_systemmsg_h_
#define __net_systemmsg_h_

#include "msgbasic.h"
#include <string.h>

const unsigned short MSGID_TYPE_SYSTEM                    = 0x0000;
//const unsigned short MSGID_SYSTEM_SocketConnectSuccess    = 0x0001;
const unsigned short MSGID_SYSTEM_ClientPublicKey         = 0x0002;
const unsigned short MSGID_SYSTEM_C2S_SecretKey           = 0x0003;
const unsigned short MSGID_SYSTEM_S2C_SecretKey           = 0x0004;
const unsigned short MSGID_SYSTEM_S2C_UpdateSecretKey     = 0x0005;
const unsigned short MSGID_SYSTEM_ConnectSuccess          = 0x0006;
//const unsigned short MSGID_SYSTEM_SocketDisconnect        = 0x0007;
const unsigned short MSGID_SYSTEM_Disconnect              = 0x0008;
const unsigned short MSGID_SYSTEM_CheckAlive              = 0x0009;
const unsigned short MSGID_SYSTEM_CheckAliveReply         = 0x000A;
const unsigned short MSGID_SYSTEM_Compressed              = 0x000B;
const unsigned short MSGID_SYSTEM_CompressedAndEncrypted  = 0x000C;
const unsigned short MSGID_SYSTEM_Encrypted               = 0x000D;

#pragma pack(1)

/*
struct MSG_SYSTEM_SocketConnectSuccess : public MSG_BASE
{
    MSG_SYSTEM_SocketConnectSuccess()
    {
        nMsg = MSGID_SYSTEM_SocketConnectSuccess;
        nSize = sizeof(MSG_SYSTEM_SocketConnectSuccess);
    }
};
*/

const unsigned int cMAX_CLIENT_PUBLIC_KEY_LEN = 512;
struct MSG_SYSTEM_ClientPublicKey : public MSG_BASE
{
    MSG_SYSTEM_ClientPublicKey()
    {
        nMsg = MSGID_SYSTEM_ClientPublicKey;
        nSize = sizeof(MSG_SYSTEM_ClientPublicKey);
        memset(key, 0, cMAX_CLIENT_PUBLIC_KEY_LEN);
    }
    char key[cMAX_CLIENT_PUBLIC_KEY_LEN];
};

const unsigned int cMAX_SECRETKEY_LEN = 16;
struct MSG_SYSTEM_C2S_SecretKey : public MSG_BASE
{
    MSG_SYSTEM_C2S_SecretKey()
    {
        nMsg = MSGID_SYSTEM_C2S_SecretKey;
        nSize = sizeof(MSG_SYSTEM_C2S_SecretKey);
        memset(key, 0, cMAX_SECRETKEY_LEN);
    }
    char key[cMAX_SECRETKEY_LEN];
};

struct MSG_SYSTEM_S2C_SecretKey : public MSG_BASE
{
    MSG_SYSTEM_S2C_SecretKey()
    {
        nMsg = MSGID_SYSTEM_S2C_SecretKey;
        nSize = sizeof(MSG_SYSTEM_S2C_SecretKey);
        memset(key, 0, cMAX_SECRETKEY_LEN);
    }
    char key[cMAX_SECRETKEY_LEN];
};

struct MSG_SYSTEM_S2C_UpdateSecretKey : public MSG_BASE
{
    MSG_SYSTEM_S2C_UpdateSecretKey()
    {
        nMsg = MSGID_SYSTEM_S2C_UpdateSecretKey;
        nSize = sizeof(MSG_SYSTEM_S2C_UpdateSecretKey);
        memset(key, 0, cMAX_SECRETKEY_LEN);
    }
    char key[cMAX_SECRETKEY_LEN];
};

struct MSG_SYSTEM_ConnectSuccess : public MSG_BASE
{
    MSG_SYSTEM_ConnectSuccess()
    {
        nMsg = MSGID_SYSTEM_ConnectSuccess;
        nSize = sizeof(MSG_SYSTEM_ConnectSuccess);
    }
};

/*
struct MSG_SYSTEM_SocketDisconnect : public MSG_BASE
{
    MSG_SYSTEM_SocketDisconnect()
    {
        nMsg = MSGID_SYSTEM_SocketDisconnect;
        nSize = sizeof(MSG_SYSTEM_SocketDisconnect);
    }
};
*/

struct MSG_SYSTEM_Disconnect : public MSG_BASE
{
    MSG_SYSTEM_Disconnect()
    {
        nMsg = MSGID_SYSTEM_Disconnect;
        nSize = sizeof(MSG_SYSTEM_Disconnect);
    }
};

struct MSG_SYSTEM_CheckAlive : public MSG_BASE
{
    MSG_SYSTEM_CheckAlive()
    {
        nMsg = MSGID_SYSTEM_CheckAlive;
        nSize = sizeof(MSG_SYSTEM_CheckAlive);
    }
};

struct MSG_SYSTEM_CheckAliveReply : public MSG_BASE
{
    MSG_SYSTEM_CheckAliveReply()
    {
        nMsg = MSGID_SYSTEM_CheckAliveReply;
        nSize = sizeof(MSG_SYSTEM_CheckAliveReply);
    }
};

struct MSG_SYSTEM_Compressed : public MSG_BASE
{
    MSG_SYSTEM_Compressed()
    {
        nMsg = MSGID_SYSTEM_Compressed;
        nSize = sizeof(MSG_SYSTEM_Compressed);
        nCheckSum = 0;
    }
    unsigned int nCheckSum;
    unsigned short nSrcDataSize;
    unsigned char data[1];
    unsigned short GetDataSize() const
    {
        return nSize - sizeof(MSG_SYSTEM_Compressed) + 1;
    }
};

struct MSG_SYSTEM_CompressedAndEncrypted : public MSG_BASE
{
    MSG_SYSTEM_CompressedAndEncrypted()
    {
        nMsg = MSGID_SYSTEM_CompressedAndEncrypted;
        nSize = sizeof(MSG_SYSTEM_CompressedAndEncrypted);
        nCheckSum = 0;
    }
    unsigned int nCheckSum;
    unsigned short nSrcDataSize;
    unsigned char data[1];
    unsigned short GetDataSize() const
    {
        return nSize - sizeof(MSG_SYSTEM_CompressedAndEncrypted) + 1;
    }
};

struct MSG_SYSTEM_Encrypted : public MSG_BASE
{
    MSG_SYSTEM_Encrypted()
    {
        nMsg = MSGID_SYSTEM_Encrypted;
        nSize = sizeof(MSG_SYSTEM_Encrypted);
        nCheckSum = 0;
    }
    unsigned int nCheckSum;
    unsigned char data[1];
    unsigned short GetDataSize() const
    {
        return nSize - sizeof(MSG_SYSTEM_Encrypted) + 1;
    }
};

const unsigned short cMAX_ENCRYPTED_DATA_SIZE = cMAX_MSG_SIZE - sizeof(MSG_SYSTEM_Encrypted) + 1;

// condition: cMAX_COMPRESSED_DATA_SIZE >= cMAX_LOGIC_MSG_SIZE
const unsigned short cMAX_COMPRESSED_DATA_SIZE = 65024;

// condition: cMAX_C_AND_E_DATA_SIZE >= cMAX_LOGIC_MSG_SIZE
const unsigned short cMAX_C_AND_E_DATA_SIZE = 65024;

// condition: compressBound(cMAX_COMPRESSED_DATA_SIZE) <= cMAX_COMPRESSED_DATA_SIZE_BOUND
// condition: compressBound(cMAX_C_AND_E_DATA_SIZE) <= cMAX_COMPRESSED_DATA_SIZE_BOUND
// condition: cMAX_COMPRESSED_DATA_SIZE_BOUND <= cMAX_MSG_SIZE - sizeof(MSG_SYSTEM_Compressed) + 1;
// condition: cMAX_COMPRESSED_DATA_SIZE_BOUND <= cMAX_MSG_SIZE - sizeof(MSG_SYSTEM_CompressedAndEncrypted) + 1;
const unsigned short cMAX_COMPRESSED_DATA_SIZE_BOUND = cMAX_MSG_SIZE - sizeof(MSG_SYSTEM_Compressed) + 1;

#pragma pack()

#endif // __net_systemmsg_h_

