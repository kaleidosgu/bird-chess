package game.control
{
	import game.events.ParamEvent;

	public class MainController extends Controller
	{
		private static var _instance:MainController=null;
		public function MainController()
		{
			super();
		}
		public static function getInstance():MainController
		{
			if (_instance == null)
				_instance=new MainController();
			return _instance;
		}
		public function init():void
		{
			this.dispatcher.addEventListener(LoginController.LOGINSUCCESS, onLoginSuccess);
		}
		private function onLoginSuccess(e:ParamEvent):void
		{
			GameController.getInstance().loginSuccess();
		}
	}
}