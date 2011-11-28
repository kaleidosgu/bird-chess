package common.baseData
{
	public class Int8
	{		
		public var value:int;
		public var size:int;
		public function Int8(value:int)
		{
			this.value = value;
			this.size = 8;
		}
		public function toString():String
		{
			return this.value.toString();
		}
	}
}