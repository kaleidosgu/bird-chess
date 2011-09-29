#pragma once
#include <vector>


template <typename T_Info>
class CInfoManagerTemplate//: //public CInfoManger
{
public:
	typedef std::vector<T_Info*> InfoVector;
	typedef T_Info** ArrarOfPoint;
	typedef typename InfoVector::iterator InfoVectorIterator;
	CInfoManagerTemplate(bool bAutoDelete = true)
		:m_matrixData(0)
		,m_nX(0)
		,m_nSize(0)
		,m_nCount(1)
		,m_nOffset(0)
		,m_bAutoDelete(bAutoDelete)
	{

	}

	CInfoManagerTemplate(int nX,bool bAutoDelete= true,int nOffset = 0,int nSize = 0)
		:m_bAutoDelete(bAutoDelete)
	{
		m_nOffset = nOffset;
		m_matrixData = 0;
		Initialize(nX,nOffset,nSize);
	}
	~CInfoManagerTemplate()
	{
		Clear();
	};
	int Count() const {return m_Vector.size();};
	int GetMemorySize() { return m_nX;};
	T_Info* GetByIndex(int nIndex)
	{
		if(nIndex >= 0 && nIndex < m_Vector.size())
		{
			return m_Vector[nIndex];
		}
		return 0;
	};
	T_Info* GetByID(int x)
	{
		if(m_nSize == 0)
		{
			return 0;
		}
		x -= m_nOffset;
		if(x<0 || x>= m_nX)
		{
			return 0;
		}

		int nPrieceIndex = x/m_nSize;
		int pos = x% m_nSize;
		if(m_matrixData[nPrieceIndex])
		{
			return m_matrixData[nPrieceIndex][pos];
		}
		return 0;
	};

	bool Add(T_Info* pInfo ,int x)
	{
		if (m_nSize == 0)
		{
			return false;
		}
		x -= m_nOffset;
		if(!(x>=0&& x<m_nX))
		{
			if(m_bAutoDelete)
			{
				delete pInfo;
			}
			return false;
		};
		int nPieceIndex(0);//快的index
		int pos = x; //快中的位置
		if(m_nCount != 1)
		{
			nPieceIndex = x/m_nSize;
			pos = x%m_nSize;
		}
		if(m_matrixData[nPieceIndex] == NULL)
		{
			m_matrixData[nPieceIndex] = new T_Info*[m_nSize];
			for( int i = 0;i<m_nSize;i++)
			{
				m_matrixData[nPieceIndex][i] = NULL;
			}
		}

		if(m_matrixData[nPieceIndex][pos]!=0)
		{
			if(m_bAutoDelete)
			{
				delete pInfo;
			}
		}
		m_matrixData[nPieceIndex][pos] = pInfo;
		m_Vector.push_back(pInfo);
		return true;
	};

	bool Initialize(int nX,int nOffset = 0,int nSize = 0)
	{
		Clear();
		m_nOffset = nOffset;
		m_nSize = (nSize>0)?nSize:nX;
		if(m_nSize == 0)
		{
			m_nCount =1;
		}
		else
		{
			m_nCount = (nX%m_nSize == 0)?(nX/m_nSize):(nX/m_nSize +1);
		}
		m_matrixData = new ArrarOfPoint[m_nCount];
		if(m_matrixData == 0)
		{
			return false;
		}
		for(int i= 0;i<m_nCount ;++i)
		{
			m_matrixData[i] = 0;
		}
		m_nX = nX;
		m_Vector.clear();
		m_Vector.reserve(nX/10);
		return true;
	}
	bool ReInitialize()
	{
		return Initialize(m_nX,m_nOffset,m_nSize);
	}
	virtual void AdjustAfterRead()
	{
		if(m_Vector.capacity()<= m_Vector.size())
			return;
		else
		{
			std::vector<T_Info*> temp(m_Vector);
			temp.swap(m_Vector);
		}
	}
protected:
	void Clear()
	{
		if(m_bAutoDelete)
		{
			for(InfoVectorIterator it = m_Vector.begin();it!=m_Vector.end();++it)
			{
				//delete(*it);
			}
		}
		m_Vector.clear();
		std::vector<T_Info*> ().swap(m_Vector);
		if(m_matrixData!=0)
		{
			for(int i =0;i<m_nCount;++i)
			{
				if(m_matrixData[i])
				{
					delete[]m_matrixData[i];
				}
			}
			delete[] m_matrixData;
			m_matrixData = 0;
		}
		m_nX = 0;
	};

private:
	InfoVector m_Vector;
	ArrarOfPoint* m_matrixData;
	int m_nX;
	int m_nSize;
	int m_nCount;
	int m_nOffset;
	bool m_bAutoDelete;
};