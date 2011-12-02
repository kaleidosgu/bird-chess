package game.view
{
	import com.raoway.utils.Reflection;
	import flash.display.MovieClip;
	import flash.display.Sprite;

	public class BackGroundView extends Sprite
	{
		public var bgMc:MovieClip = null;
		public function BackGroundView()
		{
			super();
			this.initView();
		}
		private function initView():void
		{
			bgMc = Reflection.createInstance("bgMC");
			addChild(bgMc);
		}
	}
}