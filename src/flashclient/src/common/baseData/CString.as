package common.baseData
{
	public class CString
	{
		public var value:String;
		private var maxLen:int;
		public function CString(maxLen:int)
		{
			this.value = "";
			this.maxLen = maxLen;
		}
		public function toString():String
		{
			return value.toString();
		}
		
		public function writeToByteArray(byteArray:CustomByteArray):void
		{
			var strByteArray:CustomByteArray = new CustomByteArray();
			strByteArray.writeUTFBytes(value);
			if (this.maxLen <= strByteArray.length)
			{
				byteArray.writeBytes(strByteArray, 0, this.maxLen);
				byteArray.writeByte(0);
			}
			else
			{
				byteArray.writeBytes(strByteArray, 0);
				for (var i:int; i<this.maxLen - strByteArray.length; i++)
				{
					byteArray.writeByte(0);
				}
				byteArray.writeByte(0);
			}
			strByteArray = null;
		}
		public function readFromByteArray(byteArray:CustomByteArray):void
		{
			var strArray:CustomByteArray = new CustomByteArray();
			byteArray.readBytes(strArray, 0, this.maxLen+1);
			value = strArray.toString();
			strArray = null;
		}
	}
}