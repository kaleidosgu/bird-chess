#ifndef __net_socketutil_h_
#define __net_socketutil_h_

namespace net
{
	inline unsigned int CheckSum(const unsigned char * pData, unsigned long nLen)
	{
		unsigned int nCheckSum = 0;
		for (unsigned long i=0; i<nLen; ++i)
		{
			nCheckSum += *pData++;
		}
		return nCheckSum;
	}

	inline void Encrypt(unsigned char * pData, unsigned int nLen)
	{
	}

	inline void Decrypt(unsigned char * pData, unsigned int nLen)
	{
	}
}

#endif // __net_socketutil_h_
