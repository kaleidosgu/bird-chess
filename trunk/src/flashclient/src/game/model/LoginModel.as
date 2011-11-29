package game.model
{
	import common.net.msg.login.Msg_Login_LoginResult;
	
	import flash.events.EventDispatcher;
	import flash.events.IEventDispatcher;
	
	import game.events.EventName;
	import game.events.ParamEvent;
	
	public class LoginModel extends EventDispatcher
	{
		
		public function LoginModel(target:IEventDispatcher=null)
		{
			super(target);
		}
		public function setLoginResult(loginResult:int):void
		{
			this.dispatchEvent(new ParamEvent(EventName.SHOWLOGINRESULT, loginResult));
		}
	}
}