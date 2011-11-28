package common.baseData
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	public class CustomByteArray extends ByteArray
	{
		public function CustomByteArray()
		{
			super();
			this.endian=Endian.LITTLE_ENDIAN;
		}
		
	}
}