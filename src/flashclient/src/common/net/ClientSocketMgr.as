package common.net
{
	import com.hurlant.crypto.prng.ARC4;
	import com.hurlant.crypto.rsa.RSAKey;
	
	import common.baseData.*;
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	import common.net.msg.system.Msg_System_CheckAliveReply;
	import common.net.msg.system.Msg_System_ClientPublicKey;
	
	import flash.events.Event;
	import flash.events.IOErrorEvent;
	import flash.events.ProgressEvent;
	import flash.events.SecurityErrorEvent;
	import flash.net.Socket;
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	import flash.utils.describeType;
	
	public class ClientSocketMgr extends Socket
	{
		public static const ERROR:String="socket_error";
		private static var _socket:ClientSocketMgr;
		private var m_MsgMgr:MsgMgr;
		
		private var m_bCompress:Boolean=true;
		private var m_bEncrypt:Boolean=true;
		
		private var m_MsgIDArray:Array;
		//		IP地址
		public var ip:String;
		//		端口
		public var port:int;
		
		private var m_ServerRSA:RSAKey;
		private var m_ClientRSA:RSAKey;
		private var m_EncryptRC4:ARC4;
		private var m_DecryptRC4:ARC4;
		
		
		//		数据缓冲区
		private var m_RemainBytes:CustomByteArray;
		private var m_RemainDataArray:Array=[];
		private var m_ReadDataFlag:Boolean=false;
		
		/**
		 * 包头长度
		 */
		private const HEADLENGTH:int=4;
		
		public function ClientSocketMgr()
		{
			super(null, 0);
			if (_socket != null)
			{
				throw new Error("单例模式类")
			}
			m_MsgMgr=MsgMgr.getInstance();
			m_MsgIDArray=[];
			this.endian=Endian.LITTLE_ENDIAN;
			m_ClientRSA = RSAKey.generate(512, "10001");
			//m_ClientRSA = RSAKey.generate(512, "11");
			var n:String = "00AA36ABCE88ACFDFF55523C7FC4523F90EFA00DF3774A259F2E62B4C5D99CB5ADB300A0285E5301930E0C70FB6876939CE616CE624A11E0086D341EBCACA0A1F5";
			var e:String = "11";
			m_ServerRSA = RSAKey.parsePrivateKey(n, e, e);
			m_EncryptRC4 = new ARC4();
			m_DecryptRC4 = new ARC4();
			m_bCompress = false;
			m_bEncrypt = false;
		}
		public static function getInstance():ClientSocketMgr
		{
			if (_socket == null)
			{
				_socket=new ClientSocketMgr();
			}
			return _socket;
		}
		/**
		 *启动自定义socket
		 *
		 */
		public function start(ip:String=null, port:int=0):void
		{
			this.ip=ip;
			this.port=port;
			this.configureListeners();
			super.connect(ip, port);
		}
		
		/**
		 *配置socket监听事件
		 *
		 */
		private function configureListeners():void
		{
			addEventListener(Event.CLOSE, closeHandler);
			addEventListener(Event.CONNECT, connectHandler);
			addEventListener(IOErrorEvent.IO_ERROR, ioErrorHandler);
			addEventListener(SecurityErrorEvent.SECURITY_ERROR, securityErrorHandler);
			addEventListener(ProgressEvent.SOCKET_DATA, socketDataHandler);
		}
		public function cancelHandler():void
		{
			removeEventListener(ProgressEvent.SOCKET_DATA, socketDataHandler);
		}
		/**
		 *当服务端关闭后触发
		 * @param event
		 *
		 */
		private function closeHandler(event:Event):void
		{
			//debug("socket已关闭");
			trace("socket已关闭");
			//this.dispatchEvent(new ParamEvent(ERROR,{code:1}));
		}
		
		
		private function connectHandler(event:Event):void
		{
			trace("connectHandler");
			sendPublicKey();
		}
		private function sendPublicKey():void
		{
			var cpkMsg:Msg_System_ClientPublicKey = new Msg_System_ClientPublicKey();
			
			var n:ByteArray = new ByteArray();
			var n1:ByteArray = new ByteArray();
			var n2:ByteArray = new ByteArray();
			var n3:ByteArray = new ByteArray();
			var e:ByteArray = new ByteArray();
			
			var strN:String = m_ClientRSA.n.toString();
			n.writeUTFBytes(strN);
			n.position = 0;
			n.readBytes(n1, 0, Msg_System_ClientPublicKey.cMAX_PART_OF_PUBLIC_KEY_LEN);
			n.readBytes(n2, 0, Msg_System_ClientPublicKey.cMAX_PART_OF_PUBLIC_KEY_LEN);
			n.readBytes(n3, 0);
			
			var strE:String = m_ClientRSA.e.toString(16);
			e.writeUTFBytes(strE);
			m_ServerRSA.encrypt(n1, cpkMsg.n1.value, Msg_System_ClientPublicKey.cMAX_PART_OF_PUBLIC_KEY_LEN);
			m_ServerRSA.encrypt(n2, cpkMsg.n2.value, Msg_System_ClientPublicKey.cMAX_PART_OF_PUBLIC_KEY_LEN);
			m_ServerRSA.encrypt(n3, cpkMsg.n3.value, n3.length);
			m_ServerRSA.encrypt(e, cpkMsg.e.value, e.length);
			cpkMsg.n1srcsize = Msg_System_ClientPublicKey.cMAX_PART_OF_PUBLIC_KEY_LEN;
			cpkMsg.n2srcsize = Msg_System_ClientPublicKey.cMAX_PART_OF_PUBLIC_KEY_LEN;
			cpkMsg.n3srcsize = n3.length;
			cpkMsg.esrcsize = e.length;
			
			//sendMsg(cpkMsg);
			
			
			var sendBytes:CustomByteArray=new CustomByteArray();
			this.packageData(cpkMsg, sendBytes);
			this.writeShort(sendBytes.length + 2);
			this.writeBytes(sendBytes);
			this.flush();
			sendBytes = null;
			
			strE = null;
			strN = null;
			e = null;
			n3 = null;
			n2 = null;
			n1 = null;
			n = null;
			cpkMsg = null;
		}
		
		
		/**
		 * IO异常
		 * @param event
		 *
		 */
		private function ioErrorHandler(event:IOErrorEvent):void
		{
			//debug("socket io错误");
			trace("socket io错误");
			
			//this.dispatchEvent(new ParamEvent(ERROR,{code:2}));
			try
			{
				this.close();
			}
			catch (e:Error)
			{
			}
		}
		
		/**
		 *安全异常
		 * @param event
		 *
		 */
		private function securityErrorHandler(event:SecurityErrorEvent):void
		{
			//this.dispatchEvent(new ParamEvent(ERROR,{code:3}));
			//debug("socket 安全沙箱错误");
			trace("socket 安全沙箱错误");
			try
			{
				this.close();
			}
			catch (e:Error)
			{
			}
			
		}
		/**
		 *收到服务端发送数据触发
		 * @param event
		 *
		 */
		private function socketDataHandler(event:ProgressEvent):void
		{
			//    	try{
			var bytes:CustomByteArray=new CustomByteArray(); //开辟缓冲区
			this.readBytes(bytes, 0, this.bytesAvailable); //将数据读入内存缓冲区
			m_RemainDataArray.push(bytes);
			
//			/*    打印二进制        */
//			var newbytes:ByteArray=new ByteArray();
//			
//			newbytes.writeBytes(bytes);
//			newbytes.position=0
//			var str:String="长度："+newbytes.length+",";
//			while(newbytes.bytesAvailable>0)
//			{
//				str+=(newbytes.readUnsignedByte().toString())+","
//			}
//			
//			debug(str); 
//			/*      结束打印          */
			
			if (!m_ReadDataFlag) //如果当前没有在数据处理中 将开始处理数据，否则等待处理
			{
				
				m_ReadDataFlag=true; //设置状态标志为处理中
				handleCashData(); //开始处理数据
			}
			event=null;
			bytes=null;
			
			
//			var nSize:int = bytes.readShort();
//			var nMsg:int = bytes.readShort();
//			if (nMsg == 4)
//			{
//				var sEKey1:ByteArray = new ByteArray();
//				var sEKey2:ByteArray = new ByteArray();
//				bytes.readBytes(sEKey1, 0, 64);
//				bytes.readBytes(sEKey2, 0, 64);
//				var sEKey1dst:ByteArray = new ByteArray();
//				var sEKey2dst:ByteArray = new ByteArray();
//				m_ServerRSA.decrypt(sEKey1, sEKey1dst, 64);
//				m_ServerRSA.decrypt(sEKey2, sEKey2dst, 64);
//				var cEKey:ByteArray = new ByteArray();
//				cEKey.writeBytes(sEKey1dst);
//				cEKey.writeBytes(sEKey2dst);
//				var key:ByteArray = new ByteArray();
//				m_ClientRSA.decrypt(cEKey, key, cEKey.length);
//				m_EncryptRC4.init(key);
//				m_DecryptRC4.init(key);
//			}
//			else if (nMsg == 13)
//			{
//				var eByteArray:CustomByteArray = new CustomByteArray();
//				bytes.readBytes(eByteArray);
//				m_DecryptRC4.decrypt(eByteArray)
//				var nCheckSum:int = eByteArray.readUnsignedInt();
//				var nSize2:int = eByteArray.readUnsignedShort();
//				var nMsg2:int = eByteArray.readUnsignedShort();
//				trace(nSize2, nMsg2);
//				
//				// send alivereplymsg
//				this.writeShort(4);
//				this.writeShort(10);
//				this.flush();
//			}
		}
		
		private function handleCashData():void
		{
			while (m_RemainDataArray.length > 0)
			{
				var bytesArray:CustomByteArray = this.m_RemainDataArray.shift(); //如果不为空 将读取队列头数据
				bytesArray.position=0; //将字节数组指针还原 	
				//			如果上一次缓存的字节数组里面有东西，将读取出来和这一次进行拼接
				if (m_RemainBytes != null && m_RemainBytes.bytesAvailable > 0)
				{
					var dataBytes:CustomByteArray=new CustomByteArray();
					m_RemainBytes.readBytes(dataBytes, 0, m_RemainBytes.bytesAvailable);
					bytesArray.readBytes(dataBytes, dataBytes.length, bytesArray.bytesAvailable);
					m_RemainBytes=null;
					bytesArray = dataBytes;
					bytesArray.position=0; //将字节数组指针还原 	
					dataBytes=null;
				}
//				if (bytesArray.bytesAvailable < HEADLENGTH)
//				{
//					if (m_RemainBytes == null)
//					{
//						m_RemainBytes=new CustomByteArray(); //开辟缓存数据
//					}
//					bytesArray.readBytes(m_RemainBytes, m_RemainBytes.length, bytesArray.bytesAvailable); //将当前数据放入缓冲区
//					bytesArray=null;
//					continue;
//				}
				//将字节数组转换成数据
				//bytesArray.position = 0;
				getBytes(bytesArray);
				if (bytesArray.bytesAvailable > 0)
				{
					if (m_RemainBytes == null)
					{
						m_RemainBytes=new CustomByteArray(); //开辟缓存数据
					}
					bytesArray.readBytes(m_RemainBytes, m_RemainBytes.length, bytesArray.bytesAvailable); //将当前数据放入缓冲区
					bytesArray=null;
				}
				else
				{
					bytesArray=null;
				}
			}
			m_ReadDataFlag=false; //将处理进行中状态标志为 否;
			
//			//    	try{    	
//			if (m_RemainDataArray.length <= 0) //当前数据缓冲区为空
//			{
//				m_ReadDataFlag=false; //将处理进行中状态标志为 否
//				return;
//			}
//			
//			var bytesArray:CustomByteArray = this.m_RemainDataArray.shift(); //如果不为空 将读取队列头数据
//			bytesArray.position=0; //将字节数组指针还原 	
//			//			如果上一次缓存的字节数组里面有东西，将读取出来和这一次进行拼接
//			if (m_RemainBytes != null && m_RemainBytes.bytesAvailable > 0)
//			{
//				var dataBytes:CustomByteArray=new CustomByteArray();
//				m_RemainBytes.readBytes(dataBytes, 0, m_RemainBytes.bytesAvailable);
//				bytesArray.readBytes(dataBytes, dataBytes.length, bytesArray.bytesAvailable);
//				m_RemainBytes=null;
//				bytesArray = dataBytes;
//				bytesArray.position=0; //将字节数组指针还原 	
//				dataBytes=null;
//			}
//			if (bytesArray.bytesAvailable < HEADLENGTH)
//			{
//				if (m_RemainBytes == null)
//				{
//					m_RemainBytes=new CustomByteArray(); //开辟缓存数据
//				}
//				bytesArray.readBytes(m_RemainBytes, m_RemainBytes.length, bytesArray.bytesAvailable); //将当前数据放入缓冲区
//				bytesArray=null;
//			}
//			else
//			{
//				//将字节数组转换成数据
//				getBytes2(bytesArray);
//				dataBytes=null;
//				bytesArray=null;
//			}
//			handleCashData(); //重新开始去队列数据		
			
			
//			if (m_CurrentMsgSize == 0 && bytesArray.bytesAvailable < HEADLENGTH) //当前数据不够需要的数据长度,且还未读取过包长度  将缓存数据
//			{
//				if (m_RemainBytes == null)
//				{
//					m_RemainBytes=new CustomByteArray(); //开辟缓存数据
//				}
//				bytesArray.readBytes(m_RemainBytes, m_RemainBytes.length, bytesArray.bytesAvailable); //将当前数据放入缓冲区
//				bytesArray=null;
//				handleCashData(); //重新开始去队列数据				
//			}
//			else
//			{
//				//将字节数组转换成数据
//				getBytes(bytesArray);
//				dataBytes=null;
//				bytesArray=null;
//			}
			
			
		}
		
		public function getBytes(bytesArray:CustomByteArray):void
		{
			while (true)
			{
				if (bytesArray.bytesAvailable <= 0)
				{
					break;
				}
				else if (bytesArray.bytesAvailable < HEADLENGTH)
				{
					break;
				}
				var nMsgSize:int = bytesArray.readUnsignedShort();
				if (bytesArray.bytesAvailable < nMsgSize - 2)
				{
					bytesArray.position -= 2;
					break;
				}
				var realData:CustomByteArray = new CustomByteArray();
				bytesArray.readBytes(realData, 0, nMsgSize - 2);
				disposeData(realData);
				if (realData.bytesAvailable != 0)
				{
					trace("Error. There are some data in the realData after disposeData with it.");
				}
				realData = null;
			}
			bytesArray = null;
		}
//		public function getBytes(bytesArray:CustomByteArray):void
//		{
//			// 	读取内容长度
//			if (m_CurrentMsgSize == 0)
//				m_CurrentMsgSize=bytesArray.readUnsignedShort() - 2; //计算出当前还需要的数据包长度 UnsignedShort为2个字节
//			if (bytesArray.bytesAvailable < m_CurrentMsgSize) //查看当前长度是否小于 需要的长度  
//			{ //数据包长度不足
//				if (m_RemainBytes == null) //开辟缓冲区 存取长度
//				{
//					m_RemainBytes=new CustomByteArray();
//				}
//				bytesArray.readBytes(m_RemainBytes, m_RemainBytes.length, bytesArray.bytesAvailable); //将数据放入缓冲区
//				bytesArray=null;
//				handleCashData(); //继续读取队列数据
//			}
//			else
//			{
//				// 读取两个字节的消息号
//				var cmd:int=bytesArray.readUnsignedShort();
//				m_CurrentMsgSize-=2; //减去协议号所占的2个字节
//				debug("---------收到服务端数据,消息号：", cmd, "总长度:", m_CurrentMsgSize + HEADLENGTH, " 字节");
//				var realDatas:CustomByteArray=new CustomByteArray(); //开辟数据区域，将实际数据读取出来
//				if (m_CurrentMsgSize != 0)
//				{
//					bytesArray.readBytes(realDatas, 0, m_CurrentMsgSize);
//				}
//				receiveData(cmd, realDatas);
//				m_CurrentMsgSize=0;
//				realDatas=null;
//				
//				// 如果缓冲区还有数据，则继续读
//				if (bytesArray.bytesAvailable >= HEADLENGTH)
//				{
//					getBytes(bytesArray);
//				}
//				else
//				{
//					if (bytesArray.bytesAvailable > 0)
//					{
//						if (m_RemainBytes == null)
//						{
//							m_RemainBytes=new CustomByteArray();
//						}
//						bytesArray.readBytes(m_RemainBytes, m_RemainBytes.length, bytesArray.bytesAvailable);
//						//bytesArray.readBytes(m_RemainBytes, m_RemainBytes.length, 0)
//					}
//					m_ReadDataFlag=false;
//					bytesArray=null;
//					handleCashData();
//				}
//			}
//		}
		
		private function disposeData(realData:CustomByteArray):void
		{
			var nMsgID:int = realData.readUnsignedShort();
			//trace("ClientSocketMgr.disposeData : ", nMsgID);
			var nCheckSum:int = 0;
			var nSrcDataSize:int = 0;
			var msgData:CustomByteArray = null;
			if (nMsgID == MsgID.MsgID_000004_System_S2C_SecretKey)
			{
				trace("ClientSocketMgr.disposeData : Get the secret key.");
				var sEKey1:ByteArray = new ByteArray();
				var sEKey2:ByteArray = new ByteArray();
				realData.readBytes(sEKey1, 0, 64);
				realData.readBytes(sEKey2, 0, 64);
				var sEKey1dst:ByteArray = new ByteArray();
				var sEKey2dst:ByteArray = new ByteArray();
				m_ServerRSA.decrypt(sEKey1, sEKey1dst, 64);
				m_ServerRSA.decrypt(sEKey2, sEKey2dst, 64);
				var cEKey:ByteArray = new ByteArray();
				cEKey.writeBytes(sEKey1dst);
				cEKey.writeBytes(sEKey2dst);
				var key:ByteArray = new ByteArray();
				m_ClientRSA.decrypt(cEKey, key, cEKey.length);
				m_EncryptRC4.init(key);
				m_DecryptRC4.init(key);
				sEKey1 = null;
				sEKey2 = null;
				sEKey1dst = null;
				sEKey2dst = null;
				cEKey = null;
				key = null;
			}
			else if (nMsgID == MsgID.MsgID_000011_System_Compressed)
			{
				//trace("ClientSocketMgr.disposeData : Get the compressed data.");
				nCheckSum = realData.readUnsignedInt();
				nSrcDataSize = realData.readUnsignedShort();
				msgData = new CustomByteArray();
				realData.readBytes(msgData);
				msgData.uncompress();
				if (msgData.length != nSrcDataSize)
				{
					trace("WARNING. msgData.length(", msgData.length, ") != nSrcDataSize(", nSrcDataSize, ")");
					msgData = null;
					realData = null;
					return;
				}
				disposeMsgData(msgData);
				if (msgData.bytesAvailable != 0)
				{
					trace("Error. There are some data in the msgData after disposeMsgData with it.");
				}
				msgData = null;
			}
			else if (nMsgID == MsgID.MsgID_000012_System_CompressedAndEncrypted)
			{
				//trace("ClientSocketMgr.disposeData : Get the compressed and encrypted data.");
				var encryptedData:CustomByteArray = new CustomByteArray();
				
				realData.readBytes(encryptedData);
				m_DecryptRC4.decrypt(encryptedData);
				
				nCheckSum = encryptedData.readUnsignedInt();
				nSrcDataSize = encryptedData.readUnsignedShort();
				msgData = new CustomByteArray();
				
				encryptedData.readBytes(msgData);
				encryptedData = null;
				
				msgData.uncompress();
				if (msgData.length != nSrcDataSize)
				{
					trace("WARNING. msgData.length(", msgData.length, ") != nSrcDataSize(", nSrcDataSize, ")");
					msgData = null;
					realData = null;
					return;
				}
				disposeMsgData(msgData);
				if (msgData.bytesAvailable != 0)
				{
					trace("Error. There are some data in the msgData after disposeMsgData with it.");
				}
				msgData = null;
			}
			else if (nMsgID == MsgID.MsgID_000013_System_Encrypted)
			{
				//trace("ClientSocketMgr.disposeData : Get the encrypted data.");
				var eByteArray:CustomByteArray = new CustomByteArray();
				realData.readBytes(eByteArray);
				m_DecryptRC4.decrypt(eByteArray);
				nCheckSum = eByteArray.readUnsignedInt();
				disposeMsgData(eByteArray);
				if (eByteArray.bytesAvailable != 0)
				{
					trace("Error. There are some data in the eByteArray after disposeMsgData with it.");
				}
				eByteArray = null;
			}
			else
			{
				var msg:MsgBase = m_MsgMgr.createMsg(nMsgID);
				if (msg == null)
				{
					var displayStr:String = "";
					while (realData.bytesAvailable)
					{
						var value:uint = realData.readUnsignedByte();
						if (value >= 32 && value <=126)
						{
							displayStr += String.fromCharCode(value);
						}
						else
						{
							displayStr += "(";
							displayStr += value.toString(16);
							displayStr += ")";
						}
					}
					displayStr = null;
					trace("WARNING. The MsgID(", nMsgID, ") is not registered. Content:[", displayStr, "]");
				}
				else
				{
					realData.position -= 2;
					unPackageData(msg, realData)
				}
				disposeMsg(nMsgID, msg);
				msg = null;
			}
			msgData = null;
			realData = null;
			
		}
		private function disposeMsgData(msgData:CustomByteArray):void
		{
			while(msgData.bytesAvailable >= HEADLENGTH)
			{
				var nSize:int = msgData.readUnsignedShort();
				if (nSize < HEADLENGTH)
				{
					trace("Error. disposeMsgData : nSize(", nSize, ") < HEADLENGTH(", HEADLENGTH, ")");
					break;
				}
				if (msgData.bytesAvailable < nSize - 2)
				{
					trace("Error. disposeMsgData : msgData.bytesAvailable(", msgData.bytesAvailable, ") < nSize - 2(", nSize - 2, ")");
					break;
				}
				var nMsgID:int = msgData.readUnsignedShort();
				var msg:MsgBase = m_MsgMgr.createMsg(nMsgID);
				if (msg == null)
				{
					var unregisterByteArray:CustomByteArray = new CustomByteArray();
					msgData.readBytes(unregisterByteArray, 0, nSize-HEADLENGTH);
					var displayStr:String = "";
					while (unregisterByteArray.bytesAvailable)
					{
						var value:uint = unregisterByteArray.readUnsignedByte();
						if (value >= 32 && value <=126)
						{
							displayStr += String.fromCharCode(value);
						}
						else
						{
							displayStr += "(";
							displayStr += value.toString(16);
							displayStr += ")";
						}
					}
					unregisterByteArray = null;
					displayStr = null;
					trace("WARNING. The MsgID(", nMsgID, ") is not registered. Content:[", displayStr, "]");
				}
				else
				{
					msgData.position -= 2;
					unPackageData(msg, msgData)
				}
				disposeMsg(nMsgID, msg);
				msg = null;
			}
			msgData = null;
		}
		private function disposeMsg(nMsgID:int, msg:MsgBase):void
		{
			trace("DEBUG. Get msg : ", nMsgID);
			if (nMsgID == MsgID.MsgID_000009_System_CheckAlive)
			{
				var carMsg:Msg_System_CheckAliveReply = new Msg_System_CheckAliveReply();
				sendMsg(carMsg);
				carMsg = null;
				msg = null;
				return;
			}
			
			var hander:Array = m_MsgIDArray[nMsgID];
			if (hander == null || hander.length <= 0)
			{
				//debug("没有对消息号:" + nMsgID + " 添加处理方法.");
				trace("没有对消息号:" + nMsgID + " 添加处理方法.");
			}
			else
			{
				//调用消息处理函数	
				for each (var fun:Function in hander)
				{
					fun(msg);
				}
				fun = null;
			}
			hander = null;
			msg = null;
		}
		
		private function checkSum(byteArray:ByteArray):uint
		{
			var checkSum:uint = 0;
			while (byteArray.bytesAvailable)
			{
				checkSum += byteArray.readUnsignedByte();
			}
			return checkSum;
		}
		public function sendMsg(msg:MsgBase):void
		{
			var sendBytes:CustomByteArray=new CustomByteArray();
			var bytesLen:CustomByteArray=new CustomByteArray();
			sendBytes.writeShort(0);
			this.packageData(msg, sendBytes);
			bytesLen.writeShort(sendBytes.length);
			bytesLen.position = 0;
			bytesLen.readBytes(sendBytes, 0, 2);
			
			var nSrcDataSize:uint = sendBytes.length;
			
			sendBytes.position = 0;
			var nCheckSum:uint = checkSum(sendBytes);
			
			var encryptBytes:CustomByteArray = null;
			
			if (m_bCompress)
			{
				sendBytes.compress();
				if (m_bEncrypt)
				{
					encryptBytes=new CustomByteArray();
					encryptBytes.writeUnsignedInt(nCheckSum);
					encryptBytes.writeShort(nSrcDataSize);
					encryptBytes.writeBytes(sendBytes);
					this.m_EncryptRC4.encrypt(encryptBytes);
					
					this.writeShort(4 + encryptBytes.length);
					this.writeShort(MsgID.MsgID_000012_System_CompressedAndEncrypted);
					this.writeBytes(encryptBytes);
					this.flush();
					
					encryptBytes = null;
				}
				else
				{
					this.writeShort(10 + sendBytes.length);
					this.writeShort(MsgID.MsgID_000011_System_Compressed);
					this.writeUnsignedInt(nCheckSum);
					this.writeShort(nSrcDataSize);
					this.writeBytes(sendBytes);
					this.flush();
				}
			}
			else
			{
				if (m_bEncrypt)
				{
					encryptBytes=new CustomByteArray();
					encryptBytes.writeUnsignedInt(nCheckSum);
					encryptBytes.writeBytes(sendBytes);
					this.m_EncryptRC4.encrypt(encryptBytes);
					
					this.writeShort(4 + encryptBytes.length);
					this.writeShort(MsgID.MsgID_000013_System_Encrypted);
					this.writeBytes(encryptBytes);
					this.flush();
					
					encryptBytes = null;
				}
				else
				{
					this.writeBytes(sendBytes);
					this.flush();
				}
			}
			encryptBytes = null;
			bytesLen = null;
			sendBytes = null;
			msg = null;
		}
		
		/**
		 * 封装数据发送
		 * @param object 需要发送的参数对象
		 * @return
		 *
		 */
		private function packageData(object:Object, byteArray:CustomByteArray):void
		{
			var objectXml:XML=describeType(object);
			var typeName:String=objectXml.@name;
			var num:Number;
			var str:String;
			if (typeName == "uint")
			{
				byteArray.writeUnsignedInt(uint(object));
			}
			else if (typeName == "int")
			{
				byteArray.writeInt(int(object));
			}
			else if (typeName == "Number")
			{
				num=Number(object);
				if (isNaN(num))
				{
					num=0;
				}
				byteArray.writeFloat(num);
			}
			else if (typeName == "String")
			{
				trace("WARNING. packageData : packet String.");
				
				str=String(object);
				if (str == null)
				{
					str="";
				}
				byteArray.writeUTF(str);
				str=null;
			}
			else if (typeName == "Boolean")
			{
				trace("WARNING. packageData : packet Boolean.");
				byteArray.writeBoolean(Boolean(object));
			}
			else if (typeName == "common.baseData::Int32")
			{
				byteArray.writeInt(object.value);
			}
			else if (typeName == "common.baseData::UInt32")
			{
				byteArray.writeUnsignedInt(object.value);
			}
			else if (typeName == "common.baseData::Int16")
			{
				byteArray.writeShort(object.value);
			}
			else if (typeName == "common.baseData::UInt16")
			{
				byteArray.writeShort(object.value);
			}
			else if (typeName == "common.baseData::Int8")
			{
				byteArray.writeByte(object.value);
			}
			else if (typeName == "common.baseData::UInt8")
			{
				byteArray.writeByte(object.value);
			}
			else if (typeName == "common.baseData::CFloat")
			{
				byteArray.writeFloat(object.value);
			}
			else if (typeName == "common.baseData::CDouble")
			{
				byteArray.writeDouble(object.value);
			}
			else if (typeName == "common.baseData::CString")
			{
				//var strExValue:CString = object.value as CString;
				object.value.writeToByteArray(byteArray);
				//strExValue = null;
			}
			else if (typeName == "common.baseData::CByteArray")
			{
				object.value.writeToByteArray(byteArray);
			}
			else
			{
				var variables:XMLList=objectXml.variable as XMLList;
				var tempMessagArray:Array=[];
				for each (var ms:XML in variables)
				{
					var args:XMLList = ms.metadata.arg as XMLList;
					for each (var arg:XML in args)
					{
						if (arg.@key == "pos")
						{
							tempMessagArray.push({name: ms.@name, type: ms.@type, pos: arg.@value});
							break;
						}
					}
					args = null;
					arg = null;
//					var arg:XML = ms.metadata.arg[0] as XML;
//					tempMessagArray.push({name: ms.@name, type: ms.@type, pos: arg.@value});
//					arg = null;
				}
				ms = null;
				variables = null;
				tempMessagArray=tempMessagArray.sortOn("pos");
				for each (var obj:Object in tempMessagArray)
				{
					if (obj.type == "uint")
					{
						byteArray.writeUnsignedInt(object[obj.name] as uint);
					}
					else if (obj.type == "int")
					{
						byteArray.writeInt(object[obj.name] as int);
					}
					else if (obj.type == "Number")
					{
						num = object[obj.name] as Number;
						if (isNaN(num))
						{
							num=0;
						}
						byteArray.writeFloat(num);
					}
					else if (obj.type == "String")
					{
						trace("WARNING. packageData : packet String.");
						str = object[obj.name];
						if (str == null)
						{
							str="";
						}
						byteArray.writeUTF(str);
						str = null;
					}
					else if (obj.type == "Boolean")
					{
						trace("WARNING. packageData : packet Boolean.");
						byteArray.writeBoolean(object[obj.name] as Boolean);
					}
					else if (obj.type == "common.baseData::Int32")
					{
						byteArray.writeInt(object[obj.name].value);
					}
					else if (obj.type == "common.baseData::UInt32")
					{
						byteArray.writeUnsignedInt(object[obj.name].value);
					}
					else if (obj.type == "common.baseData::Int16")
					{
						byteArray.writeShort(object[obj.name].value);
					}
					else if (obj.type == "common.baseData::UInt16")
					{
						byteArray.writeShort(object[obj.name].value);
					}
					else if (obj.type == "common.baseData::Int8")
					{
						byteArray.writeByte(object[obj.name].value);
					}
					else if (obj.type == "common.baseData::UInt8")
					{
						byteArray.writeByte(object[obj.name].value);
					}
					else if (obj.type == "common.baseData::CFloat")
					{
						byteArray.writeFloat(object[obj.name].value);
					}
					else if (obj.type == "common.baseData::CDouble")
					{
						byteArray.writeDouble(object[obj.name].value);
					}
					else if (obj.type == "common.baseData::CString")
					{
						//var strExValue:CString = object[obj.name] as CString;
						object[obj.name].writeToByteArray(byteArray);
						//strExValue = null;
					}
					else if (obj.type == "common.baseData::CByteArray")
					{
						object[obj.name].writeToByteArray(byteArray);
					}
					else
					{
						var tempObj:Object=object[obj.name];
						if (tempObj is Array)
						{
							for each (var innerObj:Object in tempObj)
							{
								packageData(innerObj, byteArray);
							}
							innerObj=null;
						}
						else
						{
							packageData(tempObj, byteArray);
						}
						tempObj=null;
					}
				}
				obj=null;
				tempMessagArray=null;
			}
			object=null;
			objectXml=null;
			object = null;
			byteArray = null;
		}
		
		/**
		 * 将二进制数据映射到对象
		 * @param valueObject  需要映射的对象
		 * @return
		 *
		 */
		private function unPackageData(valueObject:Object, dataBytes:CustomByteArray):void
		{
			var objectXml:XML=describeType(valueObject);
			var variables:XMLList=objectXml.variable as XMLList;
			var tempMessagArray:Array=[];
			for each (var ms:XML in variables)
			{
				var args:XMLList = ms.metadata.arg as XMLList;
				for each (var arg:XML in args)
				{
					if (arg.@key == "pos")
					{
						tempMessagArray.push({name: ms.@name, type: ms.@type, pos: arg.@value});
						break;
					}
				}
				args = null;
				arg = null;
//				var arg:XML = ms.metadata.arg[0] as XML;
//				tempMessagArray.push({name: ms.@name, type: ms.@type, pos: arg.@value});
//				arg = null;
			}
			tempMessagArray=tempMessagArray.sortOn("pos");
			
			//var bitArray:BitArray;
			for each (var obj:Object in tempMessagArray)
			{
				if (dataBytes.bytesAvailable <= 0)
				{ //如果数据包没有了  将停止解析
					trace("WARNING. unPackageData : dataBytes.bytesAvailable <= 0")
					break;
				}
//				if (!(obt.type == "common.baseData::Int4" || obt.type == "common.baseData::Int2" || obt.type == "common.baseData::Int1"))
//				{
//					bitArray=null;
//				}
				if (obj.type == "uint")
				{
					valueObject[obj.name]=dataBytes.readUnsignedInt();
				}
				else if (obj.type == "int")
				{
					valueObject[obj.name]=dataBytes.readInt();
					
				}
				else if (obj.type == "Number")
				{
					valueObject[obj.name]=dataBytes.readFloat();
				}
				else if (obj.type == "String")
				{
					trace("WARNING. unPackageData : unPacket String.");
					valueObject[obj.name]=dataBytes.readUTF();
				}
				else if (obj.type == "Boolean")
				{
					trace("WARNING. unPackageData : unPacket Boolean.");
					valueObject[obj.name]=dataBytes.readBoolean();
				}
				else if (obj.type == "common.baseData::Int32")
				{
					valueObject[obj.name].value = dataBytes.readInt();
					//valueObject[obj.name]= new Int32(dataBytes.readInt());
				}
				else if (obj.type == "common.baseData::UInt32")
				{
					valueObject[obj.name].value = dataBytes.readUnsignedInt();
					//valueObject[obj.name]= new UInt32(dataBytes.readUnsignedInt());
				}
				else if (obj.type == "common.baseData::Int16")
				{
					valueObject[obj.name].value = dataBytes.readShort();
					//valueObject[obj.name]= new Int16(dataBytes.readShort());
				}
				else if (obj.type == "common.baseData::UInt16")
				{
					valueObject[obj.name].value = dataBytes.readUnsignedShort();
					//valueObject[obj.name]= new UInt16(dataBytes.readUnsignedShort());
				}
				else if (obj.type == "common.baseData::Int8")
				{
					valueObject[obj.name].value = dataBytes.readByte();
					//valueObject[obj.name]= new Int8(dataBytes.readByte());
				}
				else if (obj.type == "common.baseData::UInt8")
				{
					valueObject[obj.name].value = dataBytes.readUnsignedByte();
					//valueObject[obj.name]= new UInt8(dataBytes.readUnsignedByte());
				}
//				else if (obt.type == "common.baseData::Int4")
//				{
//					if (bitArray == null || bitArray.position + 4 > 8)
//					{
//						bitArray=new BitArray(dataBytes.readUnsignedByte());
//					}
//					valueObject[obt.name]=new Int4(bitArray.getBits(4));
//				}
//				else if (obt.type == "common.baseData::Int2")
//				{
//					if (bitArray == null || bitArray.position + 2 > 8)
//					{
//						bitArray=new BitArray(dataBytes.readUnsignedByte());
//					}
//					valueObject[obt.name]=new Int2(bitArray.getBits(2));
//				}
//				else if (obt.type == "common.baseData::Int1")
//				{
//					if (bitArray == null || bitArray.position + 1 > 8)
//					{
//						bitArray=new BitArray(dataBytes.readUnsignedByte());
//					}
//					valueObject[obt.name]=new Int1(bitArray.getBits(1));
//				}
					
				else if (obj.type == "common.baseData::CFloat")
				{
					valueObject[obj.name].value = dataBytes.readFloat();
					//valueObject[obj.name]= new CFloat(dataBytes.readFloat());
				}
				else if (obj.type == "common.baseData::CDouble")
				{
					valueObject[obj.name].value = dataBytes.readDouble();
					//valueObject[obj.name]= new CDouble(dataBytes.readDouble());
				}
				else if (obj.type == "common.baseData::CString")
				{
					valueObject[obj.name].readFromByteArray(dataBytes);
				}
				else if (obj.type == "common.baseData::CByteArray")
				{
					valueObject[obj.name].readFromByteArray(dataBytes);
				}
				else
				{
					var tempObj:Object=valueObject[obj.name];
					if (tempObj is Array)
					{
						for each (var innerObj:Object in tempObj)
						{
							unPackageData(innerObj, dataBytes);
						}
						innerObj = null;
					}
					else
					{
						unPackageData(tempObj, dataBytes);
					}
					tempObj=null;
				}
			}
			objectXml=null;
			variables=null;
			tempMessagArray=null;
			ms=null;
			obj=null;
			valueObject = null;
			dataBytes = null;
		}
		
		/**
		 *添加某个消息号的监听
		 * @param nMsgID	消息号
		 * @param hander	处理句柄
		 *
		 */
		public function addMsgListener(nMsgID:int, hander:Function):void
		{
			if (m_MsgIDArray[nMsgID] == null)
			{
				m_MsgIDArray[nMsgID]=[];	
			}
			this.m_MsgIDArray[nMsgID].push(hander);
		}
		
		/**
		 *移除 消息号监听
		 * @param cmd
		 *
		 */
		public function removeMsgListener(nMsgID:int, listener:Function):void
		{
			var handers:Array=this.m_MsgIDArray[nMsgID];
			if (handers != null && handers.length > 0)
			{
				for (var i:int=(handers.length - 1); i >= 0; i--)
				{
					if (listener == handers[i])
					{
						handers.splice(i, 1);
					}
				}
			}
			
		}
	}
}