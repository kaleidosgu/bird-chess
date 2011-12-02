package common.net.msg.cardgame
{
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_S2C_Win extends MsgBase
	{
		public function Msg_Login_S2C_Win()
		{
			super(MsgID.MsgID_004121_Login_S2C_Win);
		}
	}
}