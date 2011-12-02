package common.net.msg.cardgame
{
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_X2X_ChatContent extends MsgBase
	{
		public static var ChannelWorld:int = 0;
		public static var ChannelRoom:int = 1;
		public var channel:int;
		public var content:String;
		public function Msg_Login_X2X_ChatContent()
		{
			super(MsgID.MsgID_004123_Login_X2X_ChatContent);
			this.channel = ChannelRoom;
			this.content = "";
		}
	}
}