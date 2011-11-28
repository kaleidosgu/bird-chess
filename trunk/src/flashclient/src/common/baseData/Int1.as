package common.baseData
{
	public class Int1
	{	
		public var value:int;
		public var size:int;
		public function Int1(value:int)
		{
			this.value = value;
			this.size = 1;
		}
		public function toString():String
		{
			return this.value.toString();
		}
	}
}