package common.net.msg.cardgame
{
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_S2C_KickOffPlayer extends MsgBase
	{
		public function Msg_Login_S2C_KickOffPlayer()
		{
			super(MsgID.MsgID_004122_Login_S2C_KickOffPlayer);
		}
	}
}