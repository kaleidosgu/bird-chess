//////////////////////////////////////////////////////////////////////////
//
//		通用内存池,在有频繁的new和delete,并且数量会维持一定大小的时候
//
//////////////////////////////////////////////////////////////////////////

#include <vector>
#include <deque>

using namespace std;

template<typename T>
class CComMemPool
{
public:
	CComMemPool():m_nMaxCount(10)
	{
		
	};
	virtual ~CComMemPool()
	{
		typename vector<T*>::iterator itBeg = m_VectAll.begin();
		typename vector<T*>::iterator itEnd = m_VectAll.end();
		for(;itBeg!=itEnd;itBeg++)
		{
			delete [] *itBeg;
		}
		m_VectAll.clear();
	};

	T* NewObject()
	{
		T* newObject = NULL;
		if(!m_CanUse.empty())
		{
			newObject = m_CanUse.front();
			m_CanUse.pop_front();
			return newObject;
		}
		if(newObject == NULL)
		{
			T* p = NULL;
			try
			{
				p = new T[m_nMaxCount];
				for(int i = 0;i<m_nMaxCount;i++)
				{
					m_CanUse.push_back(&p[i]);
				}
			}
			catch (std::bad_alloc&)
			{
			//	printf("NoMem to Use");
				return NULL;
			}
			m_VectAll.push_back(p);
			m_nMaxCount *=2;
			if(!m_CanUse.empty())
			{
				newObject = m_CanUse.front();
				m_CanUse.pop_front();
				return newObject;
			}
		}
		return NULL;
	};
	void ReleaseOject(T* CurrentObject)
	{
		m_CanUse.push_front(CurrentObject);
	}
private:
	int m_nMaxCount;
	vector<T*> m_VectAll;
	deque<T*> m_CanUse;
	//set<T*> m_Set;
};