package game.view
{
	import flash.display.Sprite;
	
	import game.manager.LayerManager;
	
	public class MainView extends Sprite
	{
		public function MainView()
		{
			super();
			init();
		}
		public function init():void
		{
			this.mouseEnabled=false;
			
			LayerManager.sceneLayer=new Sprite;
			LayerManager.sceneLayer.mouseEnabled=false;
			addChild(LayerManager.sceneLayer);
			
			LayerManager.sceneUILayer=new Sprite;
			LayerManager.sceneUILayer.mouseEnabled=false;
			addChild(LayerManager.sceneUILayer);
			
			LayerManager.dialogLayer=new Sprite;
			LayerManager.dialogLayer.mouseEnabled=false;
			addChild(LayerManager.dialogLayer);
			
			
			
			LayerManager.uiLayer=new Sprite();
			LayerManager.uiLayer.mouseEnabled=false;
			addChild(LayerManager.uiLayer);
			
			//弹出窗
			LayerManager.windowLayer=new Sprite;
			LayerManager.windowLayer.mouseEnabled=false;
			addChild(LayerManager.windowLayer);
			//查看物品信息
			LayerManager.goodsInfoLayer=new Sprite;
			LayerManager.goodsInfoLayer.mouseEnabled=false;
			addChild(LayerManager.goodsInfoLayer);
			
			
			
			LayerManager.noticeLayer=new Sprite;
			LayerManager.noticeLayer.mouseEnabled=false;
			LayerManager.noticeLayer.mouseChildren=false;
			addChild(LayerManager.noticeLayer);
			
//			LayerManager.tipsLayer=new Sprite();
//			LayerManager.tipsLayer.mouseEnabled=false;
//			LayerManager.tipsLayer.mouseChildren=false;
//			addChild(LayerManager.tipsLayer);
//			LayerManager.tipsLayer.addChild(TipsCanvas.getInstance());
			
			LayerManager.loginLayer=new Sprite();
			addChild(LayerManager.loginLayer);
			
			LayerManager.loadingLayer=new Sprite();
			addChild(LayerManager.loadingLayer);
			
			LayerManager.topLayer=new Sprite();
			addChild(LayerManager.topLayer);
			
			LayerManager.msgLayer = new Sprite();
			addChild(LayerManager.msgLayer);
		}
	}
}