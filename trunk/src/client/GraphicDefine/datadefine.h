#ifndef _DATADEFINE_H__
#define _DATADEFINE_H__
class SColor
{
public:
	inline SColor( signed int a, signed int r, signed int g, signed int b)
		: color((( a & 0xff) << 24) | ((r& 0xff)<<16)|((g&0xff)<8)|(b&0xff)){}
	signed int color;
};
#endif