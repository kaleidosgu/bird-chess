package common.net.msg.cardgame
{
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	
	public class Msg_Login_RoomInfo extends MsgBase
	{
		public static const cMAX_PAGE_ROOM_NUM:int = 8;
		public var roomNumber:int;
		public var currentPage:int;
		public var roomArray:Array;
		public function Msg_Login_RoomInfo()
		{
			super(MsgID.MsgID_004100_Login_S2C_RoomInfo);
			this.roomNumber = 0;
			this.currentPage = 0;
			this.roomArray = new Array();
			for (var i:int=0; i<cMAX_PAGE_ROOM_NUM; i++)
			{
				var ri:RoomInfo = new RoomInfo();
				this.roomArray.push(ri);
				ri = null;
			}
		}
	}
}