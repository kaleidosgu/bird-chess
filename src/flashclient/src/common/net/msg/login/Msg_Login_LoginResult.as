package common.net.msg.login
{
	import common.baseData.CustomByteArray;
	import common.baseData.Int16;
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_LoginResult extends MsgBase
	{
		public static const cResult_Success:int = 0;
		public static const cResult_Relogin:int = 1;
		public static const cResult_Failed:int = 2;
		
		public var nResult:int;
		public function Msg_Login_LoginResult()
		{
			super(MsgID.MsgID_004098_Login_LoginResult);
			this.nResult = cResult_Success;
		}
	}
}