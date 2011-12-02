package common.net.msg.cardgame
{
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_S2C_AddNewCard extends MsgBase
	{
		public function Msg_Login_S2C_AddNewCard()
		{
			super(MsgID.MsgID_004117_Login_S2C_AddNewCard);
		}
	}
}