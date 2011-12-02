package common.net.msg.cardgame
{
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_C2S_EnterRoom extends MsgBase
	{
		public var roomID:int;
		public function Msg_Login_C2S_EnterRoom()
		{
			super(MsgID.MsgID_004104_Login_C2S_EnterRoom);
			roomID = -1;
		}
	}
}