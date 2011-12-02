package common.net.msg.cardgame
{
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_S2C_PlayerLeave extends MsgBase
	{
		public function Msg_Login_S2C_PlayerLeave()
		{
			super(MsgID.MsgID_004114_Login_S2C_PlayerLeave);
		}
	}
}