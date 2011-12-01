package common.loader
{
	import flash.display.Loader;
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.net.URLLoader;
	import flash.net.URLRequest;
	import flash.system.ApplicationDomain;
	import flash.system.LoaderContext;
	import flash.utils.Dictionary;
	
	import game.events.ParamEvent;
	
	public class ResLoader extends EventDispatcher
	{
		public static const COMPLETE:String="complete";
		public static const ITEM_COMPLETE:String="itemComplete";
		public static const ITEM_START:String="itemStart";
		
		private var _list:Array;
		private var _currentItem:LoaderItem;
		
		function ResLoader()
		{
			init()
		}
		private function init():void
		{
			_list=new Array();
		}
		public function load():void
		{
			if(_list.length==0)
			{
				this.dispatchEvent(new ParamEvent(COMPLETE,_currentItem));
				return;
			}
			_currentItem=(_list.shift());
			this.dispatchEvent(new ParamEvent(ITEM_START,_currentItem));
			if(_currentItem.type==LoaderItem.FILETYPE_SWF)
			{
				var loader:Loader=new Loader();
				loader.contentLoaderInfo.addEventListener(Event.COMPLETE,completeHandler);
				loader.load(new URLRequest(_currentItem.url),new LoaderContext(true,ApplicationDomain.currentDomain,null));
			}
			else if(_currentItem.type==LoaderItem.FILETYPE_XML)
			{
				var urlLoader:URLLoader=new URLLoader();
				urlLoader.addEventListener(Event.COMPLETE,completeDataHandler);
				urlLoader.load(new URLRequest(_currentItem.url));
			}
		}
		public function addItem(item:LoaderItem):void
		{
			_list.push(item);
		}
		private function completeHandler(e:Event):void
		{
			_currentItem.content=e.target.content;
			this.dispatchEvent(new ParamEvent(ITEM_COMPLETE,_currentItem));
			load();
		}
		private function completeDataHandler(e:Event):void
		{
			_currentItem.content=e.target.data;
			this.dispatchEvent(new ParamEvent(ITEM_COMPLETE,_currentItem));
			load();
		}
	}
}