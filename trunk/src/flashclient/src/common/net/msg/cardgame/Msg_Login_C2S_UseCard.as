package common.net.msg.cardgame
{
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_C2S_UseCard extends MsgBase
	{
		public function Msg_Login_C2S_UseCard()
		{
			super(MsgID.MsgID_004110_Login_C2S_UseCard);
		}
	}
}