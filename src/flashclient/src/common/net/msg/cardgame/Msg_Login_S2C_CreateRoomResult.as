package common.net.msg.cardgame
{
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_S2C_CreateRoomResult extends MsgBase
	{
		public function Msg_Login_S2C_CreateRoomResult()
		{
			super(MsgID.MsgID_004103_Login_S2C_CreateRoomResult);
		}
	}
}