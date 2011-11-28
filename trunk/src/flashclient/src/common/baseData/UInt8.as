package common.baseData
{
	public class UInt8
	{
		public var value:uint;
		public var size:int;
		public function UInt8(value:uint)
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