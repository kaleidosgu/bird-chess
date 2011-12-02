package common.net.msg.cardgame
{
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_S2C_UseCard extends MsgBase
	{
		public function Msg_Login_S2C_UseCard()
		{
			super(MsgID.MsgID_004119_Login_S2C_UseCard);
		}
	}
}