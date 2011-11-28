package common.baseData
{
	public class CDouble
	{
		public var value:Number;
		public var size:int;
		public function CDouble(value:Number)
		{
			this.value = value;
			this.size=64;
		}
		public function toString():String
		{
			return this.value.toString();
		}
	}
}