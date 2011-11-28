package common.baseData
{
	public class UInt32
	{
		public var value:uint;
		public var size:int;
		public function UInt32(value:uint)
		{
			this.value = value;
			this.size = 32;
		}
		public function toString():String
		{
			return this.value.toString();
		}
	}
}