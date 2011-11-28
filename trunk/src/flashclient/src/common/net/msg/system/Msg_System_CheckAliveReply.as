package common.net.msg.system
{
	import common.baseData.CustomByteArray;
	
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_System_CheckAliveReply extends MsgBase
	{
		public function Msg_System_CheckAliveReply()
		{
			super(MsgID.MsgID_000010_System_CheckAliveReply);
		}
	}
}