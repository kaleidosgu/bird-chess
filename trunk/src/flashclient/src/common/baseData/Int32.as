package common.baseData
{
	public class Int32
	{
		public var value:int;
		public var size:int;
		public function Int32(value:int)
		{
			this.value = value;
			this.size=32;
		}
		public function toString():String
		{
			return this.value.toString();
		}
	}
}