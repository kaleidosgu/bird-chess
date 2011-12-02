package common.net.msg.cardgame
{
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_S2C_PlayerEnter extends MsgBase
	{
		public function Msg_Login_S2C_PlayerEnter()
		{
			super(MsgID.MsgID_004112_Login_S2C_PlayerEnter);
		}
	}
}