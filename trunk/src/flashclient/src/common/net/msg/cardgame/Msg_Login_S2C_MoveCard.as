package common.net.msg.cardgame
{
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_S2C_MoveCard extends MsgBase
	{
		public function Msg_Login_S2C_MoveCard()
		{
			super(MsgID.MsgID_004120_Login_S2C_MoveCard);
		}
	}
}