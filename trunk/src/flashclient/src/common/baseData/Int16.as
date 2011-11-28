package common.baseData
{
	public class Int16
	{
		public var value:int;
		public var size:int;
		public function Int16(value:int)
		{
			this.value = value;
			this.size = 16;
		}
		public function toString():String
		{
			return this.value.toString();
		}
	}
}