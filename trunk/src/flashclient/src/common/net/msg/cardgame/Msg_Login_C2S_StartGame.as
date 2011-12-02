package common.net.msg.cardgame
{
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_C2S_StartGame extends MsgBase
	{
		public function Msg_Login_C2S_StartGame()
		{
			super(MsgID.MsgID_004108_Login_C2S_StartGame);
		}
	}
}