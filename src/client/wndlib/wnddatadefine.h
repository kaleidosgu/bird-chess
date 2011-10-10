#ifndef _WNDDATADEFINE_H__
#define _WNDDATADEFINE_H__
enum WND_RESULT
{
	WND_RESULT_YES,
	WND_RESULT_NO,
};
enum WND_UI_EVENT
{
	WND_CLICK,
	WND_ONCHAR,
};
struct ResStruct
{
	int x;
	int y;
	int w;
	int h;
};

enum btnState { normal = 0, focus, down, gray };

#endif