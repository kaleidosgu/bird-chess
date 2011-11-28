package common.baseData
{
	public class Int4
	{
		public var value:int;
		public var size:int;
		public function Int4(value:int)
		{
			this.value = value;
			this.size = 4;
		}
		public function toString():String
		{
			return this.value.toString();
		}
	}
}