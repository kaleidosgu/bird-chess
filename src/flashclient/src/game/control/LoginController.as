package game.control
{
	import common.net.ClientSocketMgr;
	import common.net.msg.MsgID;
	import common.net.msg.login.Msg_Login_LoginRequest;
	
	import flash.events.Event;
	
	import game.events.EventName;
	import game.events.ParamEvent;
	import game.view.LoginView;
	import common.net.msg.login.Msg_Login_LoginResult;
	import common.net.msg.MsgBase;
	import common.net.msg.login.Msg_Login_RoomInfo;

	public class LoginController extends Controller
	{
		private static var _instance:LoginController=null;
		private var m_csm:ClientSocketMgr;
		private var loginView:LoginView=null;
		
		public function LoginController(loginView:LoginView)
		{
			super();
			m_csm = ClientSocketMgr.getInstance();
			
			
			//socket
			m_csm = ClientSocketMgr.getInstance();
			//csm.start("114.80.196.52", 7753);
			//csm.start("172.18.39.117", 8888);
			m_csm.start("172.18.39.116", 8888);
			
			
			
			this.addSocketEvent();
			
			this.loginView=loginView;
			this.addEvent();
		}
		private function addEvent():void
		{
			this.loginView.addEventListener(EventName.REQUEST_JOIN_GAME, sendRequestJoinGame);
			
			
		}
		private function addSocketEvent():void
		{
			m_csm.addMsgListener(MsgID.MsgID_004098_Login_LoginResult, onLoginResult);
			m_csm.addMsgListener(MsgID.MsgID_004100_Login_S2C_RoomInfo, onRoomListResult);
		}
		
		public function sendRequestJoinGame(e:ParamEvent):void
		{
			var lrMsg:Msg_Login_LoginRequest = new Msg_Login_LoginRequest();
			lrMsg.username.value = e.param.username;
			lrMsg.password.value = e.param.password;
			m_csm.sendMsg(lrMsg);
			lrMsg = null;
		}
		private function onLoginResult(msg:MsgBase):void
		{
			this.loginView.onLoginResult(msg);
		}
		private function onRoomListResult(msg:MsgBase):void
		{
			var ri:Msg_Login_RoomInfo = msg as Msg_Login_RoomInfo;
			if (ri.roomNumber == 0)
			{
				// create room
			}
			else
			{
			}
		}
		public static function getInstance(loginView:LoginView=null):LoginController
		{
			if (_instance == null)
				_instance=new LoginController(loginView);
			return _instance;
		}
	}
}