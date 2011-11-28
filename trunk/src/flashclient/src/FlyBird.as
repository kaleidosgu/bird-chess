package
{
	import com.raoway.utils.Reflection;
	
	import common.net.ClientSocketMgr;
	import common.net.msg.MsgBase;
	import common.net.msg.MsgID;
	import common.net.msg.login.*;
	
	import flash.display.Loader;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.*;
	import flash.net.URLRequest;
	import flash.system.ApplicationDomain;
	import flash.system.LoaderContext;
	import flash.ui.Keyboard;
	import flash.utils.ByteArray;
	
	[SWF(width="800", height="600", frameRate="24", backgroundColor="0x000000")]
	public class FlyBird extends Sprite
	{
		private var bgMc:MovieClip = null;
		private var loginMc:MovieClip = null;
		private var csm:ClientSocketMgr;
		
		public function FlyBird()
		{
			//socket
			csm = ClientSocketMgr.getInstance();
			//csm.start("114.80.196.52", 7753);
			//csm.start("172.18.39.117", 8888);
			csm.start("172.18.39.116", 8888);
			csm.addMsgListener(MsgID.MsgID_004098_Login_LoginResult, onLoginResult);
			csm.addMsgListener(MsgID.MsgID_004100_Login_S2C_RoomInfo, onRoomListResult);
			//			
			////			csm.addMsgListener(4098, handle4098);
			////			csm.addMsgListener(4098, handle4098);
			////			csm.addMsgListener(4098, handle4098x);
			//			
			var loader:Loader=new Loader();
			loader.contentLoaderInfo.addEventListener(Event.COMPLETE,loginLoaderComplete);
			
			//loader.load(new URLRequest("login.swf"),new LoaderContext(true,ApplicationDomain.currentDomain,null));
			loader.load(new URLRequest("main.swf"),new LoaderContext(true,ApplicationDomain.currentDomain,null));
			
		}
		private function onLoginResult(msg:MsgBase):void
		{
			var ri:Msg_Login_LoginResult = msg as Msg_Login_LoginResult;
			if (ri.nResult == Msg_Login_LoginResult.cResult_Success)
			{
				this.removeChild(loginMc);
			}
			else if (ri.nResult == Msg_Login_LoginResult.cResult_Relogin)
			{
				loginMc.addEventListener(KeyboardEvent.KEY_DOWN, onLoginEnter);
				loginMc.usernameinput.text += " relogin";
			}
			else if (ri.nResult == Msg_Login_LoginResult.cResult_Failed)
			{
				loginMc.addEventListener(KeyboardEvent.KEY_DOWN, onLoginEnter);
				loginMc.usernameinput.text = " login failed";
			}
			else
			{
				loginMc.addEventListener(KeyboardEvent.KEY_DOWN, onLoginEnter);
				loginMc.usernameinput.text = "Unkown error";
			}
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
		private function loginLoaderComplete(e:Event):void
		{
			bgMc = Reflection.createInstance("bgMc");
//			loginMc.x = 350;
//			loginMc.y = 350;
			addChild(bgMc);
//			loginMc.usernameinput.text = "lijian";
//			loginMc.passwordinput.text = "1111";
			loginMc = Reflection.createInstance("loginMc");
			loginMc.x = 300;
			loginMc.y = 150;
			loginMc.usernameinput.text = "lijian";
			loginMc.passwordinput.text = "1111";
			loginMc.addEventListener(KeyboardEvent.KEY_DOWN, onLoginEnter);
			addChild(loginMc);
			
//			loginMc.loginbtn.addEventListener(MouseEvent.CLICK, onLoginBTNClick);
//			loginMc = Reflection.createInstance("loginMC2");
//			loginMc.x = 350;
//			loginMc.y = 350;
//			addChild(loginMc);
//			loginMc.usernameinput.text = "lijian";
//			loginMc.passwordinput.text = "1111";
//			
//			loginMc.loginbtn.addEventListener(MouseEvent.CLICK, onLoginBTNClick);
		}
		private function onLoginEnter(e:KeyboardEvent):void
		{
			if (e.keyCode == Keyboard.ENTER)
			{
				loginMc.removeEventListener(KeyboardEvent.KEY_DOWN, onLoginEnter);
				trace(loginMc.usernameinput.text);
				trace(loginMc.passwordinput.text);
				var lrMsg:Msg_Login_LoginRequest = new Msg_Login_LoginRequest();
				lrMsg.username.value = loginMc.usernameinput.text;
				lrMsg.password.value = loginMc.passwordinput.text;
				csm.sendMsg(lrMsg);
				lrMsg = null;
			}
		}
//		private function onLoginBTNClick(e:MouseEvent):void
//		{
//			trace(loginMc.usernameinput.text);
//			trace(loginMc.passwordinput.text);
//			var lrMsg:Msg_Login_LoginRequest = new Msg_Login_LoginRequest();
//			lrMsg.username.value = loginMc.usernameinput.text;
//			lrMsg.password.value = loginMc.passwordinput.text;
//			csm.sendMsg(lrMsg);
//			lrMsg = null;
//		}
	}
}