package common.net.msg
{
	public class MsgID
	{
		public function MsgID()
		{
		}
		
		public static const MsgID_Unknown:int = 0;
		
		// system msg
		public static const MsgID_000002_System_ClientPublicKey			:int = 2;
		public static const MsgID_000004_System_S2C_SecretKey				:int = 4;
		public static const MsgID_000009_System_CheckAlive				:int = 9;
		public static const MsgID_000010_System_CheckAliveReply			:int = 10;
		public static const MsgID_000011_System_Compressed				:int = 11;
		public static const MsgID_000012_System_CompressedAndEncrypted	:int = 12;
		public static const MsgID_000013_System_Encrypted					:int = 13;
		
		// login msg
		public static const MsgID_004097_Login_LoginRequest				:int = 4097;
		public static const MsgID_004098_Login_LoginResult				:int = 4098;
		public static const MsgID_004099_Login_S2C_PlayerInfo					:int = 4099;
		public static const MsgID_004100_Login_S2C_RoomInfo						:int = 4100;
		
		public static const MsgID_004101_Login_C2S_NextRoomPage					:int = 4101;
		public static const MsgID_004102_Login_C2S_CreateRoom					:int = 4102;
		public static const MsgID_004103_Login_S2C_CreateRoomResult				:int = 4103;
		public static const MsgID_004104_Login_C2S_EnterRoom					:int = 4104;
		public static const MsgID_004105_Login_S2C_EnterRoomResult				:int = 4105;
		public static const MsgID_004106_Login_C2S_LeaveRoom					:int = 4106;
		public static const MsgID_004107_Login_S2C_LeaveRoomResult				:int = 4107;
		public static const MsgID_004108_Login_C2S_StartGame					:int = 4108;
		public static const MsgID_004109_Login_S2C_StartGameFailed				:int = 4109;
		public static const MsgID_004110_Login_C2S_UseCard						:int = 4110;
		public static const MsgID_004111_Login_S2C_UseCardFailed				:int = 4111;
		
		public static const MsgID_004112_Login_S2C_PlayerEnter				:int = 4112;
		public static const MsgID_004113_Login_S2C_RoomMaster				:int = 4113;
		public static const MsgID_004114_Login_S2C_PlayerLeave				:int = 4114;
		public static const MsgID_004115_Login_S2C_StartGame				:int = 4115;
		public static const MsgID_004116_Login_S2C_SetCardType				:int = 4116;
		public static const MsgID_004117_Login_S2C_AddNewCard				:int = 4117;
		public static const MsgID_004118_Login_S2C_CurrentTurnPlayer		:int = 4118;
		public static const MsgID_004119_Login_S2C_UseCard					:int = 4119;
		public static const MsgID_004120_Login_S2C_MoveCard					:int = 4120;
		public static const MsgID_004121_Login_S2C_Win						:int = 4121;
		public static const MsgID_004122_Login_S2C_KickOffPlayer			:int = 4122;
		public static const MsgID_004123_Login_X2X_ChatContent				:int = 4123;
	}
}