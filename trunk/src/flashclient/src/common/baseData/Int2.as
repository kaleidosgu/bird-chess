package common.baseData
{
	public class Int2
	{
		public var value:int;
		public var size:int;
		public function Int2(value:int)
		{
			this.value = value;
			this.size = 2;
		}
		public function toString():String
		{
			return this.value.toString();
		}
	}
}