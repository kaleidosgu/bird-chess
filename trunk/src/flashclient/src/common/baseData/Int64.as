package common.baseData
{
	import flash.utils.ByteArray;

	public class Int64
	{
		public var l_value:uint;
		public var h_value:int;
		public var size:int;
		public function Int64(h_value:int, l_value:uint)
		{
			this.h_value = h_value;
			this.l_value = l_value;
			this.size = 64;
		}
		override public function toString():String
		{
			return this.h_value.toString() + this.l_value.toString();
		}
		public function readFromByteArray(byteArray:ByteArray):void
		{
			this.h_value = byteArray.readInt();
			this.l_value = byteArray.readUnsignedInt();
		}
		public function writeToByteArray(byteArray:CustomByteArray):void
		{
			byteArray.writeInt(this.h_value);
			byteArray.writeUnsignedInt(this.l_value);
		}
	}
}
