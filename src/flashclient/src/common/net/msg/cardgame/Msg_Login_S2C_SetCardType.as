package common.net.msg.cardgame
{
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_S2C_SetCardType extends MsgBase
	{
		public function Msg_Login_S2C_SetCardType()
		{
			super(MsgID.MsgID_004116_Login_S2C_SetCardType);
		}
	}
}