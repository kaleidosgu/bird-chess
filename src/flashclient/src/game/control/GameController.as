package game.control
{
	import common.net.ClientSocketMgr;
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	import common.net.msg.cardgame.Msg_Login_C2S_CreateRoom;
	import common.net.msg.cardgame.Msg_Login_C2S_EnterRoom;
	import common.net.msg.cardgame.Msg_Login_C2S_NextRoomPage;
	import common.net.msg.cardgame.Msg_Login_RoomInfo;
	import common.net.msg.cardgame.Msg_Login_S2C_PlayerInfo;
	import common.net.msg.cardgame.Msg_Login_X2X_ChatContent;
	import common.net.msg.cardgame.RoomInfo;
	
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;
	import flash.ui.Keyboard;
	
	import game.events.EventName;
	import game.events.ParamEvent;
	import game.manager.LayerManager;
	import game.model.GameModel;
	import game.view.GameView;

	public class GameController extends Controller
	{
		private var m_csm:ClientSocketMgr;
		private var gameView:GameView = null;
		private var gameModel:GameModel = null;
		private static var _instance:GameController=null;
		public function GameController()
		{
			super();
			m_csm = ClientSocketMgr.getInstance();
			init();
			this.addViewEventListener();
			this.addSocketEventListener();
		}
		private function init():void
		{
			gameModel = new GameModel();
			gameView = new GameView(gameModel);
		}
		private function addViewEventListener():void
		{
			this.gameView.gameMc.playerBTN.addEventListener(MouseEvent.CLICK, onStartGame);
			this.gameView.gameMc.chatcontent.addEventListener(KeyboardEvent.KEY_DOWN, onChatContentKeyDown);
		}
		private function addSocketEventListener():void
		{
			m_csm.addMsgListener(MsgID.MsgID_004099_Login_S2C_PlayerInfo, onPlayerInfoResult);
			m_csm.addMsgListener(MsgID.MsgID_004100_Login_S2C_RoomInfo, onRoomListResult);
			
		}
		private function onStartGame(e:MouseEvent):void
		{
			// 
		}
		private function onChatContentKeyDown(e:KeyboardEvent):void
		{
			if (e.keyCode == Keyboard.ENTER)
			{	
				var ccMsg:Msg_Login_X2X_ChatContent = new Msg_Login_X2X_ChatContent();
				ccMsg.content = this.gameView.gameMc.chatcontent.text;
				this.gameView.gameMc.chatcontent.text = "";
				m_csm.sendMsg(ccMsg);
				ccMsg = null;
			}
		}
		private function onPlayerInfoResult(msg:MsgBase):void
		{
			var pi:Msg_Login_S2C_PlayerInfo = msg as Msg_Login_S2C_PlayerInfo;
			this.gameModel.setMainPlayerInfo();
		}
		private function onRoomListResult(msg:MsgBase):void
		{
			var riMsg:Msg_Login_RoomInfo = msg as Msg_Login_RoomInfo;
			if (riMsg.roomNumber == 0)
			{
				var crMsg:Msg_Login_C2S_CreateRoom = new Msg_Login_C2S_CreateRoom();
				m_csm.sendMsg(crMsg);
				crMsg = null;
			}
			else
			{
				var bEnterRoom:Boolean = false;
				for (var i:int=0; i<riMsg.roomArray.length; i++)
				{
					var ri:RoomInfo = riMsg.roomArray[i];
					if (ri.playerCount == 1)
					{
						var erMsg:Msg_Login_C2S_EnterRoom = new Msg_Login_C2S_EnterRoom();
						erMsg.roomID = ri.roomID;
						m_csm.sendMsg(erMsg);
						erMsg = null;
						bEnterRoom = true;
						break;
					}
				}
				if (!bEnterRoom)
				{
					var nrpMsg:Msg_Login_C2S_NextRoomPage = new Msg_Login_C2S_NextRoomPage();
					m_csm.sendMsg(nrpMsg);
					nrpMsg = null;
				}
			}
		}
		public function loginSuccess():void
		{
			LayerManager.sceneLayer.addChild(gameView);
		}
		public static function getInstance():GameController
		{
			if (_instance == null)
				_instance=new GameController();
			return _instance;
		}
	}
}