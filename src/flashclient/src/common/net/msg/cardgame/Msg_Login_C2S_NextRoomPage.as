package common.net.msg.cardgame
{
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_C2S_NextRoomPage extends MsgBase
	{
		public function Msg_Login_C2S_NextRoomPage()
		{
			super(MsgID.MsgID_004101_Login_C2S_NextRoomPage);
		}
	}
}