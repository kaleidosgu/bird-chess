package common.baseData
{
	import flash.utils.ByteArray;

	public class UInt16
	{
		public var value:uint;
		public var size:int;
		public function UInt16(value:uint)
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