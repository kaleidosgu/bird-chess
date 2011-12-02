package common.net.msg.cardgame
{
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_S2C_EnterRoomResult extends MsgBase
	{
		public function Msg_Login_S2C_EnterRoomResult()
		{
			super(MsgID.MsgID_004105_Login_S2C_EnterRoomResult);
		}
	}
}