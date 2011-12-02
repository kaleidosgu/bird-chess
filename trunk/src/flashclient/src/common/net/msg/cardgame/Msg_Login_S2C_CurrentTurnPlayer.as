package common.net.msg.cardgame
{
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_S2C_CurrentTurnPlayer extends MsgBase
	{
		public function Msg_Login_S2C_CurrentTurnPlayer()
		{
			super(MsgID.MsgID_004118_Login_S2C_CurrentTurnPlayer);
		}
	}
}