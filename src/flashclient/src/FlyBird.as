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
	import flash.net.URLLoader;
	import flash.net.URLRequest;
	import flash.system.ApplicationDomain;
	import flash.system.LoaderContext;
	import flash.ui.Keyboard;
	import flash.utils.ByteArray;
	
	import game.control.LoginController;
	import game.view.LoginView;
	
	[SWF(width="800", height="600", frameRate="24", backgroundColor="0x000000")]
	public class FlyBird extends Sprite
	{
		private static var _instance:FlyBird;
		
		private var csm:ClientSocketMgr;
		
		public function FlyBird()
		{
			_instance=this;
			this.addEventListener(Event.ADDED_TO_STAGE,adderHandler);
			
		}
		private function adderHandler(e:Event):void
		{
			this.removeEventListener(Event.ADDED_TO_STAGE,adderHandler);
			
//			var game:GameView=new GameView();
//			addChild(game);
//			
//			stage.stageFocusRect=false;
//			StageProxy.registed(this.stage);
//			PopUpManager.layer=LayerManager.topLayer;
//			
//			loading=PopUpManager.createPopUp(Loading,true) as Loading;
			this.loaderInfo.addEventListener(Event.COMPLETE, completeInfoHandler);
			
//			ComponentManager.init();
//			
//			Security.allowDomain("*");
		}
		
		private function completeInfoHandler(e:Event):void
		{
			loaderInfo.removeEventListener(Event.COMPLETE, completeInfoHandler);
			//ConfigManager.parameters=LoaderInfo(e.target).parameters;
			loadConfig();
		}
		/**
		 * 加载配置文件
		 *
		 */
		private function loadConfig():void
		{
			var urlLoad:URLLoader=new URLLoader();
			urlLoad.addEventListener(Event.COMPLETE, completeConfigHandler);
			urlLoad.load(new URLRequest("assets/config/config.xml"));
		}
		/**
		 * 完成加载配置文件 
		 * @param e
		 * 
		 */	
		private function completeConfigHandler(e:Event):void
		{
			//trace(e.target.data);
			e.target.removeEventListener(Event.COMPLETE, completeConfigHandler);
//			ConfigManager.setConfigData(new XML(e.target.data));
			loadResource();
			//			var mainController:MainController=MainController.getInstance();
			//			mainController.startSocket();
		}
		
		/**
		 * 加载素材
		 *
		 */
		public function loadResource():void
		{
			var loader:Loader=new Loader();
			loader.contentLoaderInfo.addEventListener(Event.COMPLETE,onLoadResourceComplete);
			loader.load(new URLRequest("main.swf"),new LoaderContext(true,ApplicationDomain.currentDomain,null));
			
//			loader=new ResLoader();
//			
//			var a:XML=ConfigManager.configData;
//			var swfList:XMLList=ConfigManager.configData.file.swf.children();
//			var xmlList:XMLList=ConfigManager.configData.file.xml.children();
//			var i:int;
//			var info:Object;
//			//trace(swfList.length(),xmlList.length())
//			var root:String=ConfigManager.configData.file.@root.toString();
//			for (i=0; i < swfList.length(); i++)
//			{
//				info=new Object();
//				info.type="swf";
//				info.name=swfList[i].@name;
//				info.title=swfList[i].@title;
//				info.url=root+swfList[i].toString();
//				try
//				{
//					info.isLogin=swfList[i].@isLogin;
//				}
//				catch(e:Error){}
//				if(info.isLogin!=1 || GameManager.getInstance().firstLogin==true)
//				{
//					loader.addItem(new LoaderItem(info.url, info));
//				}
//				
//			}
//			for (i=0; i < xmlList.length(); i++)
//			{
//				info=new Object();
//				info.type="xml";
//				info.name=xmlList[i].@name;
//				info.title=xmlList[i].@title;
//				info.url=root+xmlList[i].toString();
//				loader.addItem(new LoaderItem(info.url, info));
//			}
//			loader.addEventListener(ResLoader.COMPLETE, queueCompleteHandler);
//			loader.addEventListener(ResLoader.ITEM_COMPLETE, itemCompleteHandler);
//			loader.addEventListener(ResLoader.ITEM_START, itemStartHandler);
//			loader.load();
		}
		/**
		 * 完成加载素材 
		 * @param e
		 * 
		 */	
//		private function queueCompleteHandler(e:ParamEvent):void
//		{
//			PopUpManager.removePopUp(loading);
//			e.target.removeEventListener(ResLoader.COMPLETE, queueCompleteHandler);
//			e.target.removeEventListener(ResLoader.ITEM_COMPLETE, itemCompleteHandler);
//			e.target.removeEventListener(ResLoader.ITEM_START, itemStartHandler);
//			//e.target.dispose();
//			
//			applicationStart();
//		}
//		/**
//		 * 開始加载一个队列的素材 
//		 * @param e
//		 * 
//		 */	
//		private function itemStartHandler(e:ParamEvent):void
//		{
//			loading.setText("正在加载" + e.param.info.title);
//		}
//		/**
//		 * 完成加载一个队列的素材  
//		 * @param e
//		 * 
//		 */	
//		private function itemCompleteHandler(e:ParamEvent):void
//		{
//			if (e.param.info == null)return;
//			if(e.param.info.title=="技能配置")
//			{
//				SkillManager.getInstance().parseSkillXML(XML(e.param.content).children());
//			}
//			if(e.param.info.title=="物品配置")
//			{
//				GoodsListManage.getInstance().createGoodsXML(XML(e.param.content).children());
//			}
//		}
		
		private function applicationStart():void
		{
			var loginView:LoginView=new LoginView();
			this.addChild(loginView);
			LoginController.getInstance(loginView);
			
			
//			var loginView:LoginView=new LoginView();
//			LayerManager.loginLayer.addChild(loginView);
//			LoginController.getInstance(loginView);
//			PopManage.getInstance();
//			RoleController.getInstance();
//			ChangeLineController.getInstance();
//			SceneController.getInstance();
//			/** 激活[背包]，[任务]，[邮件],[坐骑]的控制层 */
//			ButtomController.getInstance();
//			BagController.getInstance();
//			TaskController.getInstance();
//			//			EmailController.getInstance();
//			FigureController.getInstance();
//			InfoGoodsController.getInstance();
//			ShopController.getInstance();
//			PetController.getInstance();
//			FriendController.getInstance();
//			SkillController.getInstance();
//			BuildController.getInstance();
//			PhaseController.getInstance();
//			ShortCutController.getInstance();
//			
//			
//			MapController.getInstance();
//			AutoMFController.getInstance();
//			NoticeController.getInstance();
//			DialogController.getInstance();
//			TaskFollowController.getInstance();
//			ChatController.getInstance();
//			
//			SystemMessageController.getInstance();
//			MailController.getInstance();
//			MallController.getInstance();
//			GuildController.getInstance();
//			TeamController.getInstance();
//			RankController.getInstance();
//			DropPackController.getInstance();
//			
//			var m_fps:Fps=new Fps();
//			//m_fps.x=StageProxy.width - 30;
//			m_fps.x=80;
//			m_fps.y=180;
//			addChild(m_fps);
//			
//			//添加鼠标
//			this.stage.addChild(MouseCursor.getInstance());
//			
//			MainController.getInstance().sendLineListRequest();
		}
		
		
		
		private function onLoadResourceComplete(e:Event):void
		{
			e.target.removeEventListener(Event.COMPLETE, onLoadResourceComplete);
			applicationStart();
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