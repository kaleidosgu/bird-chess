package game.events
{

	public class EventName
	{
		public static const SHOWLOGINPANEL:String="showLoginPanel"
		public static const INTOSCENE:String="intoScene";
		public static const REQUEST_CREATE:String="create";
		public static const SHOW_ROLE_LIST:String="showRoleList";
		public static const REQUEST_JOIN_GAME:String="joinGame";
		public static const REQUEST_DELETE:String="onDelete";
		public static const CREATE_GOODS:String = "createGoods";
		public static const SHOW_UI_BAG:String="showUIBag"; //打开背包
		public static const SHOW_UI_TASK:String="showUITask"; //打开任务
		public static const SHOW_UI_MAILGM:String="show_ui_mailgm"; //打开联系GM
		public static const SHOW_UI_EMAIL:String="showUIEmail"; //打开邮件
		public static const SHOW_UI_WRITEMAIL:String="show_ui_writemail"; //打开写邮件
		public static const SHOW_UI_MAIL_ITEMNUM:String="show_ui_mail_itemnum";		//打开邮件附件数量窗口
		public static const SHOW_UI_SQUAD:String="showUISquad"; //打开组队
		public static const SHOW_UI_FIGURE:String="showUIFigure"; //打开人物信息
		public static const SHOW_UI_PET:String="showUIPet"; //打开宠物
		public static const SHOW_UI_HORSES:String="showUIHorses"; //打开坐骑
		public static const SHOW_UI_TREASURE:String="showUITreasure"; //打开法宝
		public static const SHOW_UI_HANG:String="showUIHand"; //打开挂机
		public static const SHOW_UI_FRIEND:String="showUIFriend"; //打开好友
		public static const SHOW_UI_MALL:String="showUIMall"; //打开商城
		public static const SHOW_UI_SHOP:String="showUIShop"; //打开商店（NPC）
		public static const SHOW_UI_MAP:String="showUIMap"; //打开地图
		public static const SHOW_UI_RANK:String="showrUIRank";//打开排行榜
		public static const SHOW_UI_SKILL:String="showUISkill"; //打开技能
		public static const SHOW_UI_FACTION:String="showUIFaction"; //打开帮派
		public static const SHOW_UI_BUILD:String="showUIBuild"; //打开打造
		public static const SHOW_UI_LINE:String="showUILine";//打开换线
		public static const SHOW_OTHER_PLAYER:String = "showOtherPlayer";//查看玩家信息

		public static const PACK_CHANGE:String="packChange"; //背包有变动(移动，使用，装备物品，删除)
		public static const PACK_REFRESH:String = "packRefresh";//刷新背包
		public static const SHORT_CUT_CHANGE:String = "shortCutChange"; //快捷栏变化，传给服务端保存
		public static const GOODS_PIC_LOADED:String = "goodsPicLoaded"; //物品图片分割完成
		
		public static const PET_SHOW_FEED:String = "pet_show_feed";						//打开宠物喂养		
		public static const PET_SHOW_UPLEVEL:String = "pet_show_uplevel";				//打开升级列表
		public static const PET_SHOW_CHANGE_NAME:String = "pet_show_change_name";		//打开宠物改名
		public static const PET_LOAD_LIST:String = "pet_load_list";			//加载宠物列表
		public static const PET_CHANGE_NAME:String="pet_change_name"; 		//宠物改名
		public static const PET_ABANDON:String="pet_abandon"; 				//遗弃宠物
		public static const PET_FEED:String="pet_feed";						//喂食宠物
		public static const PET_EXPAND_QUEUE:String = "pet_expand_queue";	//扩展升级队列
		public static const PET_JOIN_FIGHT:String="pet_join_fight"; 		//宠物参战
		public static const PET_REPLACE_FIGHT:String="pet_replace_fight";	//宠物替换出战	
		public static const PET_WATCH_FIGHT:String="pet_watch_fight"; 		//宠物观战
		public static const PET_CALL_BACK:String="pet_call_back"; 			//召回宠物
		public static const PET_CREATE_NEW:String="pet_create_new";			//孵化宠物
		public static const PET_GET_ONE:String="pet_get_one";				//获取单个宠物信息
		public static const PET_UPDATE_FIGHTPOS:String="pet_update_fightpos";		//更新宠物出战位置状态
		public static const PET_WASH_POINT:String = "pet_wash_point";					//宠物洗炼
		public static const PET_USE_VALUE:String = "pet_use_value";			//使用洗炼属性值
		public static const PET_UPGRADE_APTITUDE:String = "pet_upgrade_aptitude";	//宠物驯养
		public static const PET_UPGRADE_COMPLETE:String="pet_upgrade_complete";	//宠物完成升级
		public static const PET_FLASH_QUEUE:String = "Pet_flash_queue";			//刷新宠物升级列表
		public static const PET_UPGRADE_START:String="pet_upgrade_start";		//宠物开始升级
		public static const PET_UPGRADE_SPEEDUP:String = "pet_upgrade_speedup";	//宠物加速升级
		public static const PET_CANCEL_UPGRADE:String = "pet_cancel_upgrade";	//取消宠物升级
		public static const PET_UPDATE_INFORMATION:String = "pet_update_information";	//更新所有tab面板数据
		public static const PET_UPDATE_AFTER_CHANGE:String = "pet_update_after_change";	//宠物数据有变化后更新界面
		public static const PET_CLEAR:String = "pet_clear";								//请空界面
		public static const PET_UPGRADE_NUM:String = "pet_upgrade_num";					//获取可升级队列个数
		public static const PET_UPGRADE_QUALITY:String = "pet_upgrade_quality";			//宠物升阶				
		public static const PET_UPGRADE_QUALITY_RESULT:String = "pet_upgrade_quality_result";//宠物升阶结果
		public static const Pet_UPDATE_HP:String = "pet_update_hp";							//体力值同步
		
		public static const BUTTON_ENABLE:String = "button_enable";			//通知启用按钮
		
		public static const SHOW_NPC_DIALOG:String="showNPCDialog"; //显示npc对话面板
		public static const SHOW_PLAYER_PANEL:String="showPlayerPanel"; //显示被攻击的人物面板
		public static const SHOW_MONSTER_PANEL:String="showMonsterPanel"; //显示被攻击的怪物面板
		public static const HIDE_ELEMENT_PANEL:String="hideElementPanel"; //隐藏面板
		public static const SCENE_CHAT:String="sceneChat"; //场景聊天信息事件
		public static const SYSTEM_MESSAGE:String="systemMessage"; //系统消息
		public static const FIND_ELEMENT:String="findElement"; //寻找元素
		public static const GAME_START:String="gameStart";//休息开始事件
		public static const SHOW_DROPPACK:String="show_droppack";//掉落包
		
		public static const FRIEND_LOAD_FRIEND:String = "frield_load_Friend";		//请求好友列表
		public static const FRIEND_LOAD_BLACK:String = "friend_load_black";			//请求黑名单列表
		public static const FRIEND_LOAD_ENEMY:String = "friend_load_enemy";			//请求仇人列表
		public static const FRIEND_DEL_Friend:String = "friend_del_friend";			//删除好友
		public static const FRIEND_DEL_BLACK:String = "friend_del_black";			//删除黑名单	
		public static const FRIEND_DEL_STRANGER:String = "friend_del_stranger";		//删除陌生人
		public static const FRIEND_DEL_ENEMY:String = "friend_del_enemy";			//删除仇人
		public static const FRIEND_DEL_GROUP:String = "friend_del_group";			//删除分组
		public static const FRIEND_ADD_FRIEND:String = "friend_add_friend";			//加为好友
		public static const FRIEND_ADD_BLACK:String = "friend_add_black";			//加到黑名单
		public static const FRIEND_ADD_ENEMY:String = "friend_add_enemy";			//加为仇人
		public static const FRIEND_ADD_GROUP:String = "friend_add_group";			//新增分组
		public static const FRIEND_ADD_STRANGER:String = "friend_add_stranger";		//增加陌生人
		public static const FRIEDN_FIND_STRANGER:String = "friedn_find_stranger";	//请求获取陌生人信息
		public static const FRIEND_UPDATE_GROUPNAME:String = "friend_update_groupname";		//更改分组名
		public static const FRIEND_CHECK_INFO:String = "friend_check_info";			//查看资料
		public static const FRIEND_CHANGE_GROUP:String = "friend_change_group";		//改变分组
		public static const FRIEND_SHOW_SETUP:String = "friend_show_setup";			//打开好友设置面板
		public static const FRIEND_SHOW_INFO:String = "friend_show_Info";			//打开好友属性面板
		public static const FRIEND_SHOW_SEARCH:String = "friend_show_search";		//打开搜索面板
		public static const FRIEND_AGREE_ADD:String = "friend_agree_add";			//同意加为好友
		public static const FRIEND_SEARCH:String = "friend_search";					//搜索好友
		
		public static const MAIL_TO_GM:String="mail_to_gm"; 						//发信给GM
		public static const MAIL_GET_ALL:String = "mail_load_all";					//请求所有信件
		public static const MAIL_GET_SINGLE:String = "mail_get_single";				//获取单个邮件详情
		public static const MAIL_DELETE:String = "MAIL_BATCH_DELETE";				//删除信件
		public static const MAIL_NEW:String = "MAIL_NEW";							//是否有末读邮件
		public static const MAIL_GET_ITEM:String = "mail_get_item";					//获取附件
		public static const MAIL_AFTER_GET_ITEM:String = "mail_after_get_item";		//成功收取附件以后
		public static const MAIL_WRITE:String = "mail_write";						//写信
		public static const MAIL_WRITE_ADDITEM:String = "MAIL_WRITE_ADDITEM";		//写信添加附件
		public static const MAIL_SET_ITEMNUM:String="mail_set_itemnum";				//设置附件
		public static const CHAT_INPUT_FOCUS:String = "chatInputFocus";				//设置聊天获取焦点事件
		public static const TASK_REFRESH:String = "taskRefresh";				//设置聊天获取焦点事件
		public static const MESSAGE:String = "message";				//设置聊天获取焦点事件
		
		public static const USESKILL:String="useSkill";				//使用技能
		public static const SELECTSKILL:String="selectSkill";				//选择技能
		
		//对话相关
		public static const BUY:String="buy";            //购买商品
		public static const BUY_GOOD:String="buyGood";            //购买商品
		public static const SELL:String="sell";            //出售商品
		public static const ENTER_DUNGEON:String="enterDungeon";//进入副本
		public static const LEAVE_DUNGEON:String="leaveDungeon";//离开副本
		
		public static const CCMD_24002:String="join"; //加入队伍
		public static const CCMD_24006:String="inviteTeam"; //邀请加入队伍
		public static const CCMD_24016:String="getTeamInfo"; //获取队伍资料
		public static const SEND_GOOD:String="sendGood"; //发送装备到聊天框
		public static const VIEW_GOOD:String="viewGood"; //查看装备信息
		public static const CLOSE_GOODS_VIEW:String = "closegoodsview";//关闭排行中的查看装备信息弹框
	}
}