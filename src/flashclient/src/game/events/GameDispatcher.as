
package game.events
{
   
   import flash.events.Event;
   import flash.events.EventDispatcher;
   import flash.events.IEventDispatcher;

   
   public class GameDispatcher
   {
      private static var instance : GameDispatcher;  
      private var eventDispatcher : IEventDispatcher;
      
      /**
       * 返回实例
       */ 
      public static function getInstance() : GameDispatcher 
      {
         if ( instance == null )
            instance = new GameDispatcher();
          
           return instance;
      }
      public function GameDispatcher( target:IEventDispatcher = null ) 
      {
         eventDispatcher = new EventDispatcher( target );
      }
      
      /**
       * 加侦听
       */
      public function addEventListener( type:String, listener:Function, useCapture:Boolean = false, priority:int = 0, useWeakReference:Boolean = false ) : void 
      {
         eventDispatcher.addEventListener( type, listener, useCapture, priority, useWeakReference );
      }
      
      /**
       * 移除侦听
       */
      public function removeEventListener( type:String, listener:Function, useCapture:Boolean = false ) : void 
      {
         eventDispatcher.removeEventListener( type, listener, useCapture );
      }

      /**
       * 派发事件
       */
      public function dispatchEvent( event:Event ) : Boolean 
      {
         return eventDispatcher.dispatchEvent( event );
      }
      
      /**
       * 返回是否有这个侦听
       */
      public function hasEventListener( type:String ) : Boolean 
      {
         return eventDispatcher.hasEventListener( type );
      }
      
      /**
       * 
       */
      public function willTrigger(type:String) : Boolean 
      {
         return eventDispatcher.willTrigger( type );
      }
   }
}