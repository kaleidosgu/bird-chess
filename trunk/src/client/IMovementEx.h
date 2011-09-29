//////////////////////////////////////////////////////////////////////////
//					移动系统的测试接口
//  
//	驱动模型是有事件,坐标,ID
//
//
//
//////////////////////////////////////////////////////////////////////////


class IMovementEX
{
public:
	IMovementEX(){};
	~IMovementEX(){};
	virtual void SetDafulatSpeedX(int nOrigin,int nFast) = 0;
	virtual void SetDafulatSpeedY(int nOrigin,int nFast) = 0;
	virtual void SetDafulatSpeedZ(int nOrigin,int nFast) = 0;
	virtual void SetSpeedUp(int nX,in nY,in nZ)= 0 ;
	virtual void AffectByGravity(bool bYes) = 0; //重力影响
	virtual void Fly(bool bFly)= 0 ;
	virtual bool IsFlying() = 0 ; //是否飞行道具
    virtual void PopOnWall(bool bYes) = 0;
	virtual void PopOnLand(bool bYes) = 0;

	virtual void EnableSrcPos() = 0;
	virtual void DisableSrcPos() = 0;

	virtual void SetThrough(bool bYes) = 0;//穿越
	virtual bool IsThrough() = 0;

	virtual void SetStopDistance(int nDistanceX,int nDistanceY,int nDistanceZ) = 0;
	virtual void SetStopMode(int nMode ,int nParam) = 0;
	virtual void SetStopTime(int nTime);
	virtual void SetDestination(int nX,int nY,int nZ) = 0;
	virtual void SetRunto(bool bRun) = 0;
	
	virtual void ChaseEntity(IMoveable& rDstEntity,int nMode,int nParam1,int nParam2,int nParam3,int nParam4) = 0;

public:
	virtual bool IsStartPosEnable() = 0;
	virtual void SetStartPos(int nPosX,int nPosY,int nPosZ) =0;
	virtual void ResetStartPos() = 0;

	virtual bool BeginRoutSampleMove(CMoveOnRouteSampleOnEntityParam& param) =0;
	virtual bool ActiveBeginToRun(int SpeedX,int SpeedY,int SpeedZ) = 0;
	virtual bool PassiveBeginTorun(int SpeedX,int SpeedY,int SpeedZ) = 0;

	virtual bool DefaultMove(int nXDir,int nZDir) = 0;
	virtual bool defaultMoveX(bool bLeft) = 0;
	virtual bool DefaultMoveZ(bool bUp) = 0;

	virtual bool Jump() = 0;
	virtual void OnEntJump() = 0;
	virtual bool JumpFast() = 0;
	virtual bool IsRunJump() = 0;

	virtual bool Stop() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual 3DTYPE GetPosition() = 0;
	virtual bool IssStopped() = 0;
	virtual void SetPosition(int x,int y,int z) = 0;
	virtual void GetPosition(int& x,int& y,int& z) = 0;
	virtual void OnJumpUp() = 0;
	virtual void OnJumpComplete() = 0 ;
	virtual void GetSurWalkingSpeed(int& x,int&y) = 0;
	virtual void GetPositionByTime(float time,int&x,int&y,int&z);
	virtual void AttachEnvironment(CAStar* pAStar) = 0;
	virtual void DetachEnvironment() = 0;

	virtual void Move(CMoveParam& param) =0;
	virtual bool MoveOnRouteSamples(CMoveRoutSamplesParam& param) = 0;
	virtual void ShotWithoutGravity() = 0;
	virtual bool ResponseEx(CInput& input) = 0;
	virtual bool ResponseEx(int nEvent) = 0;
};