package game.view
{
	import com.raoway.utils.Reflection;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.KeyboardEvent;
	import flash.ui.Keyboard;
	
	import game.events.EventName;
	import game.events.ParamEvent;
	import common.net.msg.MsgBase;
	import common.net.msg.login.Msg_Login_LoginResult;
	
	public class LoginView extends Sprite
	{
		private var bgMc:MovieClip = null;
		private var loginMc:MovieClip = null;
		private var params:Object=new Object();
		public function LoginView()
		{
			super();
			initLoginView();
		}
		public function initLoginView():void
		{
			bgMc = Reflection.createInstance("bgMc");
			addChild(bgMc);
			loginMc = Reflection.createInstance("loginMc");
			loginMc.x = 300;
			loginMc.y = 150;
			loginMc.usernameinput.text = "lijian";
			loginMc.passwordinput.text = "1111";
			loginMc.addEventListener(KeyboardEvent.KEY_DOWN, onLoginEnter);
			addChild(loginMc);
		}
		private function onLoginEnter(e:KeyboardEvent):void
		{
			if (e.keyCode == Keyboard.ENTER)
			{
				loginMc.removeEventListener(KeyboardEvent.KEY_DOWN, onLoginEnter);
				trace(loginMc.usernameinput.text);
				trace(loginMc.passwordinput.text);
				
				params.username=loginMc.usernameinput.text;
				params.password=loginMc.passwordinput.text;
				
				this.dispatchEvent(new ParamEvent(EventName.REQUEST_JOIN_GAME, params));
			}
		}
		public function onLoginResult(msg:MsgBase):void
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
	}
}