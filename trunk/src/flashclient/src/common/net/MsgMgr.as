package common.net
{
	import flash.utils.Dictionary;
	import common.net.msg.*;
	import common.net.msg.login.*;
	import common.net.msg.system.*;
	import common.net.msg.cardgame.*;
	
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
			m_MsgDic[MsgID.MsgID_004101_Login_C2S_NextRoomPage] = Msg_Login_C2S_NextRoomPage;
			
			m_MsgDic[MsgID.MsgID_004102_Login_C2S_CreateRoom] = Msg_Login_C2S_CreateRoom;
			m_MsgDic[MsgID.MsgID_004103_Login_S2C_CreateRoomResult] = Msg_Login_S2C_CreateRoomResult;
			m_MsgDic[MsgID.MsgID_004104_Login_C2S_EnterRoom] = Msg_Login_C2S_EnterRoom;
			m_MsgDic[MsgID.MsgID_004105_Login_S2C_EnterRoomResult] = Msg_Login_S2C_EnterRoomResult;
			m_MsgDic[MsgID.MsgID_004106_Login_C2S_LeaveRoom] = Msg_Login_C2S_LeaveRoom;
			m_MsgDic[MsgID.MsgID_004107_Login_S2C_LeaveRoomResult] = Msg_Login_S2C_LeaveRoomResult;
			m_MsgDic[MsgID.MsgID_004108_Login_C2S_StartGame] = Msg_Login_C2S_StartGame;
			m_MsgDic[MsgID.MsgID_004109_Login_S2C_StartGameFailed] = Msg_Login_S2C_StartGameFailed;
			m_MsgDic[MsgID.MsgID_004110_Login_C2S_UseCard] = Msg_Login_C2S_UseCard;
			m_MsgDic[MsgID.MsgID_004111_Login_S2C_UseCardFailed] = Msg_Login_S2C_UseCardFailed;
			
			m_MsgDic[MsgID.MsgID_004112_Login_S2C_PlayerEnter] = Msg_Login_S2C_PlayerEnter;
			m_MsgDic[MsgID.MsgID_004113_Login_S2C_RoomMaster] = Msg_Login_S2C_RoomMaster;
			m_MsgDic[MsgID.MsgID_004114_Login_S2C_PlayerLeave] = Msg_Login_S2C_PlayerLeave;
			m_MsgDic[MsgID.MsgID_004115_Login_S2C_StartGame] = Msg_Login_S2C_StartGame;
			m_MsgDic[MsgID.MsgID_004116_Login_S2C_SetCardType] = Msg_Login_S2C_SetCardType;
			m_MsgDic[MsgID.MsgID_004117_Login_S2C_AddNewCard] = Msg_Login_S2C_AddNewCard;
			m_MsgDic[MsgID.MsgID_004118_Login_S2C_CurrentTurnPlayer] = Msg_Login_S2C_CurrentTurnPlayer;
			m_MsgDic[MsgID.MsgID_004119_Login_S2C_UseCard] = Msg_Login_S2C_UseCard;
			m_MsgDic[MsgID.MsgID_004120_Login_S2C_MoveCard] = Msg_Login_S2C_MoveCard;
			m_MsgDic[MsgID.MsgID_004121_Login_S2C_Win] = Msg_Login_S2C_Win;
			m_MsgDic[MsgID.MsgID_004122_Login_S2C_KickOffPlayer] = Msg_Login_S2C_KickOffPlayer;
			m_MsgDic[MsgID.MsgID_004123_Login_X2X_ChatContent] = Msg_Login_X2X_ChatContent;
			
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