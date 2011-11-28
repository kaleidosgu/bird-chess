package common.net.msg.system
{
	import common.baseData.CustomByteArray;
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_System_CheckAlive extends MsgBase
	{
		public function Msg_System_CheckAlive()
		{
			super(MsgID.MsgID_000009_System_CheckAlive);
		}
	}
}