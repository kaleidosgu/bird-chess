package common.baseData
{
	import flash.utils.ByteArray;
	
	public class UInt64
	{
		public var l_value:uint;
		public var h_value:uint;
		public var size:int;
		public function UInt64(h_value:uint, l_value:uint)
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
			this.h_value = byteArray.readUnsignedInt();
			this.l_value = byteArray.readUnsignedInt();
		}
		public function writeToByteArray(byteArray:CustomByteArray):void
		{
			byteArray.writeUnsignedInt(this.h_value);
			byteArray.writeUnsignedInt(this.l_value);
		}
	}
}