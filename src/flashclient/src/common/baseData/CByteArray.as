package common.baseData
{
	import flash.utils.ByteArray;

	public class CByteArray
	{
		public var value:ByteArray;
		private var size:int;
		public function CByteArray(size:int)
		{
			this.size = size;
			value = new ByteArray();
		}
		public function toString():String
		{
			return value.toString();
		}
		
		public function writeToByteArray(byteArray:CustomByteArray):void
		{
			if (this.size <= value.length)
			{
				byteArray.writeBytes(value, 0, this.size);
			}
			else
			{
				byteArray.writeBytes(value, 0);
				for (var i:int; i<this.size - value.length; i++)
				{
					byteArray.writeByte(0);
				}
			}
		}
		public function readFromByteArray(byteArray:CustomByteArray):void
		{
			if (this.value.length > 0)
			{
				this.value.clear();
			}
			byteArray.readBytes(this.value, 0, this.size);
		}
	}
}