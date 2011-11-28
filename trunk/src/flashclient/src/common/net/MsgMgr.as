package common.net
{
	import flash.utils.Dictionary;
	import common.net.msg.*;
	import common.net.msg.login.*;
	import common.net.msg.system.*;
	
	public class MsgMgr
	{
		private static var s_instance:MsgMgr=null;
		public var m_MsgDic:Dictionary;
		public function MsgMgr()
		{
			m_MsgDic=new Dictionary();
			registerMsg();
		}
		
		public static function getInstance():MsgMgr
		{
			if (s_instance == null)
			{
				s_instance=new MsgMgr();
			}
			return s_instance;
		}
		private function registerMsg():void
		{
//			m_MsgDic[MsgID.MsgID_000002_System_ClientPublicKey] = Msg_System_ClientPublicKey;
			m_MsgDic[MsgID.MsgID_000009_System_CheckAlive] = Msg_System_CheckAlive;
//			m_MsgDic[MsgID.MsgID_000010_System_CheckAliveReply] = Msg_System_CheckAliveReply;
			
			m_MsgDic[MsgID.MsgID_004097_Login_LoginRequest] = Msg_Login_LoginRequest;
			m_MsgDic[MsgID.MsgID_004098_Login_LoginResult] = Msg_Login_LoginResult;
			m_MsgDic[MsgID.MsgID_004099_Login_S2C_PlayerInfo] = Msg_Login_S2C_PlayerInfo;
			m_MsgDic[MsgID.MsgID_004100_Login_S2C_RoomInfo] = Msg_Login_RoomInfo;
			
		}
		public function createMsg(nMsgID:int):MsgBase
		{
			if (m_MsgDic[nMsgID] == undefined)
			{
				return null;
			}
			return new m_MsgDic[nMsgID];
		}
	}
}