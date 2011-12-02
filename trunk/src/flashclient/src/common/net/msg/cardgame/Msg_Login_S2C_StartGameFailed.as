package common.net.msg.cardgame
{
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_S2C_StartGameFailed extends MsgBase
	{
		public function Msg_Login_S2C_StartGameFailed()
		{
			super(MsgID.MsgID_004109_Login_S2C_StartGameFailed);
		}
	}
}