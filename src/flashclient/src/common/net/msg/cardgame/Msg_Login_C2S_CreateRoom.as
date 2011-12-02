package common.net.msg.cardgame
{
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_C2S_CreateRoom extends MsgBase
	{
		public function Msg_Login_C2S_CreateRoom()
		{
			super(MsgID.MsgID_004102_Login_C2S_CreateRoom);
		}
	}
}