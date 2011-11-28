package game.events
{
	import flash.events.Event;
	
	public class ParamEvent extends Event
	{
		public var param:Object;
		
		public function ParamEvent(type:String,param:Object=null,bubbles:Boolean=false,cancelable:Boolean=false):void
		{
			super(type,bubbles,cancelable);
			this.param=param;
		}

	}
}