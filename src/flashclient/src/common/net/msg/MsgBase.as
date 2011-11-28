package common.net.msg
{
	import common.baseData.CustomByteArray;
	import common.baseData.UInt16;

	public class MsgBase
	{
		public var nMsgID:UInt16;
		public function MsgBase(msgID:int)
		{
			nMsgID = new UInt16(msgID);
		}
	}
}