package common.net.msg.cardgame
{
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_S2C_LeaveRoomResult extends MsgBase
	{
		public function Msg_Login_S2C_LeaveRoomResult()
		{
			super(MsgID.MsgID_004107_Login_S2C_LeaveRoomResult);
		}
	}
}