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
	}
}