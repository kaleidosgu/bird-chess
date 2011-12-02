package common.net.msg.cardgame
{
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_C2S_LeaveRoom extends MsgBase
	{
		public function Msg_Login_C2S_LeaveRoom()
		{
			super(MsgID.MsgID_004106_Login_C2S_LeaveRoom);
		}
	}
}