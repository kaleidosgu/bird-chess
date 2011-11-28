package common.net.msg.system
{
	import common.baseData.CByteArray;
	import common.baseData.CustomByteArray;
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	import flash.utils.ByteArray;

	public class Msg_System_ClientPublicKey extends MsgBase
	{
		public static const cMAX_PART_OF_PUBLIC_KEY_LEN:int = 48;
		
		public var n1srcsize:uint;
		public var n1:CByteArray;
		public var n2srcsize:uint;
		public var n2:CByteArray;
		public var n3srcsize:uint;
		public var n3:CByteArray;
		public var esrcsize:uint;
		public var e:CByteArray;
		public function Msg_System_ClientPublicKey()
		{
			super(MsgID.MsgID_000002_System_ClientPublicKey);
			n1srcsize = 0;
			n1 = new CByteArray(64);
			n2srcsize = 0;
			n2 = new CByteArray(64);
			n3srcsize = 0;
			n3 = new CByteArray(64);
			esrcsize = 0;
			e = new CByteArray(64);
		}
	}
}