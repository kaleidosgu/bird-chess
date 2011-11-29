package game.view
{
	import com.raoway.utils.Reflection;
	
	import common.net.msg.login.Msg_Login_LoginResult;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	
	import game.events.EventName;
	import game.events.ParamEvent;
	import game.model.LoginModel;
	
	public class LoginView extends Sprite
	{
		private var params:Object=new Object();
		private var loginModel:LoginModel;
		
		public var bgMc:MovieClip = null;
		public var loginMc:MovieClip = null;
		
		public function LoginView(loginModel:LoginModel)
		{
			super();
			this.loginModel = loginModel;
			this.initLoginView();
			this.addModelEventListener();
		}
		private function initLoginView():void
		{
			bgMc = Reflection.createInstance("bgMc");
			addChild(bgMc);
			loginMc = Reflection.createInstance("loginMc");
			loginMc.x = 300;
			loginMc.y = 150;
			loginMc.usernameinput.text = "lijian";
			loginMc.passwordinput.text = "1111";
			addChild(loginMc);
		}
		private function addModelEventListener():void
		{
			this.loginModel.addEventListener(EventName.SHOWLOGINRESULT, onShowLoginResult);
		}
		private function onShowLoginResult(e:ParamEvent):void
		{
			if (e.param == Msg_Login_LoginResult.cResult_Success)
			{
				this.visible = false;
			}
			else if (e.param == Msg_Login_LoginResult.cResult_Relogin)
			{
				this.loginMc.usernameinput.text = "cResult_Relogin";
			}
			else if (e.param == Msg_Login_LoginResult.cResult_Failed)
			{
				this.loginMc.usernameinput.text = "cResult_Failed";
			}
			else
			{
				this.loginMc.usernameinput.text = "Unknown";
			}
		}
	}
}