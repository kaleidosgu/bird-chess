#include "UIGround.h"
#include "UICloudEntity.h"
#include "UIBirdEntity.h"

const int nMaxBirdCount = 5;
UIGround::UIGround()
{
	for(int i = 0 ;i < nMaxBirdCount ; ++i)
	{
		m_vecBird = new CBirdEntity()
	}
	
}

UIGround::~UIGround()
{
}

void UIGround::Render()
{

}

void UIGround::Update()
{

}

void UIGround::Response()
{

}