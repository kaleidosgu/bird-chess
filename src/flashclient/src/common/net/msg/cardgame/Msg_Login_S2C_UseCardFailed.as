package common.net.msg.cardgame
{
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_S2C_UseCardFailed extends MsgBase
	{
		public function Msg_Login_S2C_UseCardFailed()
		{
			super(MsgID.MsgID_004111_Login_S2C_UseCardFailed);
		}
	}
}