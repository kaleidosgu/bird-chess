package common.net.msg.cardgame
{
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_S2C_RoomMaster extends MsgBase
	{
		public function Msg_Login_S2C_RoomMaster()
		{
			super(MsgID.MsgID_004113_Login_S2C_RoomMaster);
		}
	}
}