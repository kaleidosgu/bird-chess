package game.control
{
	import common.net.ClientSocketMgr;
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	import common.net.msg.login.Msg_Login_LoginRequest;
	import common.net.msg.login.Msg_Login_LoginResult;
	import common.net.msg.login.Msg_Login_RoomInfo;
	
	import flash.events.Event;
	import flash.events.KeyboardEvent;
	import flash.ui.Keyboard;
	
	import game.events.EventName;
	import game.events.ParamEvent;
	import game.manager.LayerManager;
	import game.model.LoginModel;
	import game.view.LoginView;

	public class LoginController extends Controller
	{
		private static var _instance:LoginController=null;
		private var m_csm:ClientSocketMgr;
		private var loginModel:LoginModel=null;
		private var loginView:LoginView=null;
		
		public function LoginController(loginModel:LoginModel, loginView:LoginView)
		{
			super();
			this.loginModel=loginModel;
			this.loginView=loginView;
			//socket
			m_csm = ClientSocketMgr.getInstance();
			m_csm.start("172.18.39.116", 8888);
			
			this.addViewEventListener();
			this.addSocketEventListener();
		}
		private function addViewEventListener():void
		{
			this.loginView.loginMc.addEventListener(KeyboardEvent.KEY_DOWN, onLoginMCKeyDown);
		}
		private function addSocketEventListener():void
		{
			m_csm.addMsgListener(MsgID.MsgID_004098_Login_LoginResult, onLoginResult);
			m_csm.addMsgListener(MsgID.MsgID_004100_Login_S2C_RoomInfo, onRoomListResult);
		}
		
		private function onLoginMCKeyDown(e:KeyboardEvent):void
		{
			if (e.keyCode == Keyboard.ENTER)
			{
				this.loginView.loginMc.removeEventListener(KeyboardEvent.KEY_DOWN, onLoginMCKeyDown);

				var lrMsg:Msg_Login_LoginRequest = new Msg_Login_LoginRequest();
				lrMsg.username.value = this.loginView.loginMc.usernameinput.text;
				lrMsg.password.value = this.loginView.loginMc.passwordinput.text;
				m_csm.sendMsg(lrMsg);
				lrMsg = null;
			}
		}
		private function onLoginResult(msg:MsgBase):void
		{
			var ri:Msg_Login_LoginResult = msg as Msg_Login_LoginResult;
			if (ri.nResult == Msg_Login_LoginResult.cResult_Success)
			{
				//this.removeChild(loginMc);
			}
			else
			{
				this.loginView.loginMc.addEventListener(KeyboardEvent.KEY_DOWN, onLoginMCKeyDown);
			}
			this.loginModel.setLoginResult(ri.nResult);
//			else if (ri.nResult == Msg_Login_LoginResult.cResult_Relogin)
//			{
//				this.loginView.loginMc.addEventListener(KeyboardEvent.KEY_DOWN, onLoginMCKeyDown);
//				loginMc.usernameinput.text += " relogin";
//			}
//			else if (ri.nResult == Msg_Login_LoginResult.cResult_Failed)
//			{
//				this.loginView.loginMc.addEventListener(KeyboardEvent.KEY_DOWN, onLoginMCKeyDown);
//				loginMc.usernameinput.text = " login failed";
//			}
//			else
//			{
//				this.loginView.loginMc.addEventListener(KeyboardEvent.KEY_DOWN, onLoginMCKeyDown);
//				loginMc.usernameinput.text = "Unkown error";
//			}
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
		public static function getInstance(loginModel:LoginModel, loginView:LoginView):LoginController
		{
			if (_instance == null)
				_instance=new LoginController(loginModel, loginView);
			return _instance;
		}
	}
}