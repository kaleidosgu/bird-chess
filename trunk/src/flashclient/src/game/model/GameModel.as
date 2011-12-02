package game.model
{
	import flash.events.EventDispatcher;
	import flash.events.IEventDispatcher;
		
	import game.events.EventName;
	import game.events.ParamEvent;
	public class GameModel extends EventDispatcher
	{
		public function GameModel(target:IEventDispatcher=null)
		{
			super(target);
		}
		public function setMainPlayerInfo():void
		{
			this.dispatchEvent(new ParamEvent(EventName.SETMAINPLAYERINFO));
		}
	}
}