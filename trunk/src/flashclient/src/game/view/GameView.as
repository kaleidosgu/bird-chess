package game.view
{
	import com.raoway.utils.Reflection;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	
	import game.events.EventName;
	import game.model.GameModel;
	import game.events.ParamEvent;
	
	public class GameView extends Sprite
	{
		private var gameModel:GameModel;
		public var gameMc:MovieClip = null;
		public function GameView(gameModel:GameModel)
		{
			super();
			this.gameModel = gameModel;
			this.initView();
			this.addModelEventListener();
		}
		private function initView():void
		{
			gameMc = Reflection.createInstance("gameMC");
			addChild(gameMc);
		}
		private function addModelEventListener():void
		{
			this.gameModel.addEventListener(EventName.SETMAINPLAYERINFO, onSetMainPlayerInfo);
		}
		private function onSetMainPlayerInfo(e:ParamEvent):void
		{
			this.gameMc.playername.text = "lijian";
		}
	}
}