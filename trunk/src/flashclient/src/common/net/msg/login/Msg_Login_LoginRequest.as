package common.net.msg.login
{
	import common.baseData.CustomByteArray;
	import common.baseData.Int16;
	import common.baseData.CString;
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_LoginRequest extends MsgBase
	{
		public static const cMAX_USERNAME_LEN:int = 50;
		public static const cMAX_PASSWORD_LEN:int = 20;
		public var username:CString;
		public var password:CString;
		public function Msg_Login_LoginRequest()
		{
			super(MsgID.MsgID_004097_Login_LoginRequest);
			this.username = new CString(cMAX_USERNAME_LEN);
			this.password = new CString(cMAX_PASSWORD_LEN);
		}
	}
}