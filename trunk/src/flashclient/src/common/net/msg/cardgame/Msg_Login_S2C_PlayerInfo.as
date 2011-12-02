package common.net.msg.cardgame
{
	import common.baseData.CustomByteArray;
	import common.baseData.Int16;
	import common.baseData.CString;
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_S2C_PlayerInfo extends MsgBase
	{
		public static const cMAX_USERNAME_LEN:int = 50;
		public var nPlayerID:int;
		public var playerName:CString;
		public var nScore:int;
		public var nWin:int;
		public var nDogfall:int;
		public var nLose:int;
		public function Msg_Login_S2C_PlayerInfo()
		{
			super(MsgID.MsgID_004099_Login_S2C_PlayerInfo);
			this.nPlayerID = -1;
			this.playerName = new CString(cMAX_USERNAME_LEN);
			this.nScore = 0;
			this.nWin = 0;
			this.nDogfall = 0;
			this.nLose = 0;
		}
	}
}