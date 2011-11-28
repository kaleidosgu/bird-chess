package common.baseData
{
	public class CFloat
	{
		public var value:Number;
		public var size:int;
		public function CFloat(value:Number)
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