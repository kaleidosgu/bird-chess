package common.net.msg.login
{
	import common.baseData.CustomByteArray;
	
	public class RoomInfo
	{
		public var roomID:int;
		public var roomIndex:int;
		public var playerCount:int;
		public function RoomInfo()
		{
			this.roomID = -1;
			this.roomIndex = -1;
			this.playerCount = 0;
		}
	}
}