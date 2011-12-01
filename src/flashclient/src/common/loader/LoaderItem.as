package common.loader
{
	public class LoaderItem
	{
		public static const FILETYPE_SWF:String="swf";
		public static const FILETYPE_XML:String="xml";
		
		public var name:String;
		public var type:String;
		public var url:String;
		public var content:Object;
		public function LoaderItem(name:String, type:String, url:String)
		{
			this.name = name;
			this.type = type;
			this.url=url;
		}
	}
}