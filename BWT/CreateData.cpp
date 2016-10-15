
#include "math.h"
#include <vector>
#include "math.h"
#include <vector>
#include <map>
#include <iostream>
//#include <sys/timeb.h>
#include<stdio.h>
//#include<string.h>
#include <string>
#include <stdlib.h>

#include <atltrace.h>

#define setbit(x,y) x|=(1<<y) //将X的第Y位置1
#define clrbit(x,y) x&=~(1<<y) //将X的第Y位清0

//typedef map<unsigned char, int> AlphbetMapping;
// PropertyBuilderByName 用于生成类的成员变量
// 并生成set和get方法
// type 为变量类型
// access_permission 为变量的访问权限(public, priavte, protected)

#define PropertyBuilderByName(type, name, access_permission)\
    access_permission:\
        type m_##name;\
    public:\
    inline void set##name(type v) {\
        m_##name = v;\
	    }\
    inline type get##name() {\
        return m_##name;\
	    }\

#define PointerPropertyBuilderByName(type, name, access_permission)\
    access_permission:\
        type* m_##name;\
    public:\
        inline void set##name(type* v){\
            m_##name = v;\
		        }\
        inline type* get##name(){\
            return m_##name;\
		        }\

using namespace std;
class vectorBit
{
private:
	vector<byte> tData;
	int Length;
public:
	vectorBit()
	{
		Length = 0;
	}
	vectorBit(int length, byte val) :vectorBit()
	{
		byte val1 = val == 0 ? 0x00 : 0xFF;
		int i1 = ceil(float(length) / 8);
		//int i2 = length % 8;
		Length = length;
		vector<byte> temp(i1, val1);
		tData = temp;
	}
	void push_back(int input)
	{

		int i1 = Length / 8;
		int i2 = Length % 8;
		if (i2 == 0)
		{
			tData.push_back(0);
		}
		Length++;
		tData[i1] = input ? setbit(tData[i1], i2) : clrbit(tData[i1], i2);
	}
	void push_back(int count, bool value)
	{
		for (int i = 0; i < count; i++)
			this->push_back(value);
	}
	void push_back(vectorBit input)
	{
		int i1 = Length / 8;
		int i2 = Length % 8;
		for (int i = 0; i < input.Length; i++)
		{
			if (i2 == 0)
			{
				tData.push_back(0);
			}
			Length++;
			tData[i1] = input[i] ? setbit(tData[i1], i2) : clrbit(tData[i1], i2);
			if (i2++ == 7)
			{
				i2 = 0;
				i1++;
			}
		}
	}
	int size() const
	{
		return Length;
	}
	int vectorBit::operator[](int i){
		if (i < 0 || i >= Length)
			return 0;
		int i1 = i / 8;
		int i2 = i % 8;
		return (tData[i1] >> i2) & 0x01;
	}

};
template<class Indata, class Tdata>
class CreateData
{
public:
	CreateData() = default;

	virtual void Create(Indata& in, Tdata& out, Indata& lout, Indata& rout);
	~CreateData() = default;
};
typedef map<unsigned char, int> AlphbetMapping;
class WaveLetTdata
{
	//Use Macros to create member and its get and set function;
	PropertyBuilderByName(vectorBit, tData, private);
	PropertyBuilderByName(AlphbetMapping, allist, private);
	PropertyBuilderByName(char, ch, private);
	WaveLetTdata()
	{
		//ch = 0;
	}
};
class WaveLetIndata
{
	//Use Macros to create member and its get and set function;
	PropertyBuilderByName(bool, IsLeaf, private);
	PropertyBuilderByName(vector<unsigned char>, inarray, private);
	PropertyBuilderByName(vector<unsigned char>, alphbetList, private);
};
template<class Indata, class Tdata>
class CreateCWaveLetDate :public CreateData<Indata, Tdata>
{
public:
	//void Create(Indata& in, Tdata& out) override;
	void Create(Indata& in, Tdata& out, Indata& lout, Indata& rout) override;
};
template<class Indata, class Tdata>
class CreateWaveLetDate :public CreateData<Indata, Tdata>
{
public:
	//void Create(Indata& in, Tdata& out) override;
	CreateWaveLetDate(){}
	void Create(Indata& in, Tdata& out, Indata& lout, Indata& rout)override;
};
template <class Indata, class Tdata>
void CreateCWaveLetDate<Indata, Tdata>::Create(Indata& in, Tdata& out, Indata& lout, Indata& rout)
{
}

template <class Indata, class Tdata>
void CreateData<Indata, Tdata>::Create(Indata& in, Tdata& out, Indata& lout, Indata& rout)
{
}

template <class Indata, class Tdata>
void CreateWaveLetDate<Indata, Tdata>::Create(Indata& in, Tdata& out, Indata& lout, Indata& rout)
{
	//vector<unsigned char> in.
	unsigned int i;
	vector<unsigned char>& inarray = in.Getvectorchar();
	vector<unsigned char>& alphbetList = in.GetalphbetList();
	vector<unsigned char> lCharArray;
	vector<unsigned char> rCharArray;
	float countlevel = (float(alphbetList.size()) / 2);
	for (i = 0; i < countlevel; i++)
		out.allist.insert(In_Pair(alphbetList[i], 0));
	vector <unsigned char> lalphbetList(alphbetList.begin(), alphbetList.begin() + i);
	vector <unsigned char> ralphbetList(alphbetList.begin() + i, alphbetList.end());
	for (; i < alphbetList.size(); i++)
		out.allist.insert(In_Pair(alphbetList[i], 1));
	for (i = 0; i < inarray.size(); i++)
		if (wt.allist[inarray[i]] == 0)
		{
		out.bitvector.push_back(0);
		lCharArray.push_back(inarray[i]);
		}
		else
		{
			wt.tData.push_back(1);
			rCharArray.push_back(inarray[i]);
		}
	lout.Set(lCharArray, lalphbetList);
	rout.Set(rCharArray, ralphbetList);
}