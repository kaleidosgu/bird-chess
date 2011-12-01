package game.manager
{
	public class ConfigManager
	{
		public static var ip:String;
		public static var port:int;
		private static var s_configData:XML;
		
		public function ConfigManager()
		{
		}
		
		public static function setConfigData(data:String):void
		{
			s_configData=new XML(data);
			ip=ConfigManager.configData.socket.ip.toString();
			port=ConfigManager.configData.socket.port.toString();
			data=null;
		}
		
		public static function get configData():XML
		{
			return s_configData;
		}
	}
}