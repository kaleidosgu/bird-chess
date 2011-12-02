package common.net.msg.cardgame
{
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_S2C_StartGame extends MsgBase
	{
		public function Msg_Login_S2C_StartGame()
		{
			super(MsgID.MsgID_004115_Login_S2C_StartGame);
		}
	}
}