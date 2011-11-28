package game.control
{
	import flash.display.DisplayObject;
	
	import game.events.GameDispatcher;
	
	public class Controller
	{
		public var dispatcher:GameDispatcher;
		
		public function Controller()
		{
			dispatcher=GameDispatcher.getInstance();
		}
		
	}
}