// BWT.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
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
#if defined(WIN32)
# define  TIMEB     _timeb
# define  ftime     _ftime64_s
typedef __int64 TIME_T;
#else
#define TIMEB timeb
typedef long long TIME_T;
#endif
#define setbit(x,y) x|=(1<<y) //将X的第Y位置1
#define clrbit(x,y) x&=~(1<<y) //将X的第Y位清0
#define CSize 257
#define SIZE 100
#define BSize 8
#define SBSize 24
using namespace std;

class vectorBit
{
protected:
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
	int vectorBit::pop_back(int i){
		if (i < 0 || i >= Length)
			return 0;
		int i1 = i / 8;
		int i2 = i % 8;
		return (tData[i1] >> i2) & 0x01;
	};
}
class BaisOperate
{
public:
	static int rank1(vectorBit inarray,int pos)
	{
		int count = 0;
		pos = pos < inarray.size() ? pos : inarray.size();
		for (int i = 0; i < pos; i++)
			if (inarray[i] == 1)
				count++;
		return count;
	}
};
/**
 * \brief Header vector
 */
class vectorBitHeader :public vectorBit
{
public:
	void push_back(int input)
	{
		//int i1 = Length / 8;
		//
		switch (input)
		{
		case 0: 
			vectorBit::push_back(0);
			vectorBit::push_back(0);
			vectorBit::push_back(0);
			break;
		case 1:
			vectorBit::push_back(0);
			vectorBit::push_back(0);
			vectorBit::push_back(1);
			break;
		case 2:
			vectorBit::push_back(0);
			vectorBit::push_back(1);
			vectorBit::push_back(0);
			break;
		case 3:
			vectorBit::push_back(0);
			vectorBit::push_back(1);
			vectorBit::push_back(1);
			break;
		case 4:
			vectorBit::push_back(1);
			vectorBit::push_back(0);
			vectorBit::push_back(0);
			break;
		case 5:
			vectorBit::push_back(1);
			vectorBit::push_back(0);
			vectorBit::push_back(1);
			break;
		default:
			return;
		}
	}
	int vectorBitHeader::operator[](int i)
	{
		int pos = i * 3;
		int result = vectorBit::pop_back(pos++);
		result = (result << 1) + vectorBit::pop_back(pos++);
		result = (result << 1) + vectorBit::pop_back(pos++);
		return result;
	}
};
class GAMACode
{
public:
	enum HEADERTYPE{ Plain, RLG0, RLG1, ALL0, ALL1 };
	//HEADERTYPE header;
	//vectorBit Svector;
	///根据inarray进行gama编码
	static vectorBit EncodeSingle(int Length);
	static void Encode(vectorBit inarray, HEADERTYPE& header, vectorBit& Outvector);
	///根据inarray进行gama译码
	//	static void DecodeSingle()
	static void Decode(vectorBit inarray, HEADERTYPE& header, vectorBit& Outvector, int length);
	///inarray为s
	static void Rank(vectorBit inarray, int offset, HEADERTYPE Head, int loffset)
	{

	}
};
class GamaCompressData
{
	vectorBitHeader gamaHeader;
	vectorBit		gamacode;
	vector<int>		SBrank;
	vector<int>		Brank;
	vector<int>		SB;
	vector<int>		B;
	public:
	GamaCompressData();
	void CreateDate(vectorBit inarray)
	{
		int i = 0;
		int SBrank_s = 0;
		int SB_s = 0;
		SBrank.push_back(0);
		Brank.push_back(0);
		SB.push_back(0);
		B.push_back(0);

		for (i = 0; i < inarray.size(); )
		{
			GAMACode gama;
			vectorBit bitetemp;
			GAMACode::HEADERTYPE headtemp;
			gama.Encode(inarray, headtemp, bitetemp);
			gamaHeader.push_back(static_cast<int>(headtemp));
			gamacode.push_back(bitetemp);
			B.push_back(bitetemp.size());
			int ranktemp = BaisOperate::rank1(bitetemp, bitetemp.size());
			Brank.push_back(ranktemp);
			i += i + BSize;
			SBrank_s += ranktemp;
			SB_s += ranktemp;
			if (i%SBSize == 0)
			{
				SBrank.push_back(SBrank_s);
				SB.push_back(SB_s);
			}

		}
		if (i%SBSize != 0)
		{
			SBrank.push_back(SBrank_s);
			SB.push_back(SB_s);
		}

	}
};
class waveletTreeNodeByBit_C
{
	vectorBit tData;//store S
	vector<vectorBit> headervector;//Sotre header use 3bit;
	vector<unsigned int> SBrank;
	vector<unsigned int> Brank;
	vector<unsigned int> SB;
	vector<unsigned int> B;
};
class waveletTreeByBit_C
{
	waveletTreeNodeByBit_C* root;
public:
	waveletTreeByBit_C() {
		root = new waveletTreeNodeByBit_C();
	}
	//waveletTreeByBit(int alphbetCount) :waveletTreeByBit(){
	//	//layer = (int)ceil(float(log2(alphbetCount))) + 1;
	//}
	waveletTreeNodeByBit_C* getRoot() const
	{
		return root;
	}
};
struct waveletTreeNodeByBit
{
	vectorBit tData;
	map<unsigned char, int> allist;
	char ch;
	waveletTreeNodeByBit *l, *r, *p;
	waveletTreeNodeByBit(): ch(0)
	{
		l = nullptr;
		r = nullptr;
		p = nullptr;
	}

	waveletTreeNodeByBit(waveletTreeNodeByBit& parentNode) :waveletTreeNodeByBit()
	{
		p = &parentNode;
	}
	~waveletTreeNodeByBit()
	{
		if (l != nullptr){
			delete l;
			l = nullptr;
		}
		
		if (r != nullptr){
			delete r;
			r = nullptr;
		}
	}
};
class waveletTreeByBit
{
	waveletTreeNodeByBit* root;

public:
	waveletTreeByBit() {
		root = new waveletTreeNodeByBit();
	}
	waveletTreeByBit(int alphbetCount) :waveletTreeByBit(){
		//layer = (int)ceil(float(log2(alphbetCount))) + 1;
	}
	waveletTreeNodeByBit* getRoot() const
	{
		return root;
	}
	//构造小波树
	void CounstructWaveletTree(const vector<unsigned char> inarray, const vector<unsigned char> alphbetList, waveletTreeNodeByBit &wt) const
	{
		//map<unsigned char, int> allist;
		unsigned int i;
		vector<unsigned char> lCharArray, rCharArray;
		typedef pair <unsigned char, int> In_Pair;
		if (alphbetList.size()>1)
		{
			float countlevel = (float(alphbetList.size()) / 2);
			for (i = 0; i < countlevel; i++)
				wt.allist.insert(In_Pair(alphbetList[i], 0));
			vector <unsigned char> lalphbetList(alphbetList.begin(), alphbetList.begin() + i);
			vector <unsigned char> ralphbetList(alphbetList.begin() + i, alphbetList.end());
			for (; i < alphbetList.size(); i++)
				wt.allist.insert(In_Pair(alphbetList[i], 1));
			for (i = 0; i < inarray.size(); i++)
				if (wt.allist[inarray[i]] == 0)
				{
				wt.tData.push_back(0);
				lCharArray.push_back(inarray[i]);
				}
				else
				{
					wt.tData.push_back(1);
					rCharArray.push_back(inarray[i]);
				}
			wt.l = new waveletTreeNodeByBit(wt);
			wt.r = new waveletTreeNodeByBit(wt);
			CounstructWaveletTree(lCharArray, lalphbetList, *wt.l);
			CounstructWaveletTree(rCharArray, ralphbetList, *wt.r);
		}
		else
		{
			vectorBit vector(static_cast<int>(inarray.size()), '1');
			wt.tData = vector;
			wt.ch = inarray[0];
		}


	}
	//字符字典中是否含有字符c。
	static bool ContainChar(const map<unsigned char, int>& allist, unsigned char c)
	{
		if (allist.find(c) == allist.end())
			return false;
		else
			return true;
	}
	//返回值是BWT[pos]处的字符
	unsigned char Access(const int& pos) const
	{
//		unsigned char  ch;
		int ipos = pos;
		waveletTreeNodeByBit* nodetemp = root;
		while (true)
		{
			if (nodetemp->tData[ipos] == 0)
			{
				ipos = ipos - rank1(nodetemp->tData, ipos);
				if (nodetemp->l != nullptr)
					nodetemp = nodetemp->l;
				else
					break;
			}
			else
			{
				ipos = rank1(nodetemp->tData, ipos);
				if (nodetemp->r != nullptr)
					nodetemp = nodetemp->r;
				else
					break;
			}
		}

		return nodetemp->ch;
	}
	///前pos个字符串中字符c的个数
	int Rank(const unsigned char c, const int& pos/*, const vector<unsigned char> alphbetList*/) const
	{
		waveletTreeNodeByBit* nodetemp = root;
		if (!ContainChar(nodetemp->allist, c))
			return -1;
		//int Divedepos;
		//auto i = 0;
		int ipos = pos;
		typedef pair <unsigned char, int> In_Pair;
		while (nodetemp->l != nullptr)
		{
			//Divedepos = i;
			if (nodetemp->allist[c] == 0)
			{
				ipos = ipos - rank1(nodetemp->tData, ipos);
				nodetemp = nodetemp->l;
			}
			else
			{
				ipos = rank1(nodetemp->tData, ipos);
				nodetemp = nodetemp->r;
			}
		}
		return ipos;

	}



	int Select(const unsigned char c, const int& pos) const
	{
		int count;
		int postCur = 0;
		int postPre = pos;
		waveletTreeNodeByBit* nodetemp = root;
		if (!ContainChar(nodetemp->allist, c))
			return -1;
		while (nodetemp->l != nullptr)
		{
			if (nodetemp->allist[c] == 0)
				nodetemp = nodetemp->l;
			else
				nodetemp = nodetemp->r;
		}
		postPre = nodetemp->tData.size() < postPre ? nodetemp->tData.size() : postPre;

		while (nodetemp->p != nullptr)
		{
			nodetemp = nodetemp->p;
			postCur = 0;
			count = 0;
			while (count<postPre)
			{
				if (nodetemp->tData[postCur++] == nodetemp->allist[c])
					count++;
			}
			postPre = postCur;

		}
		return postCur - 1;
	}

	static int rank1(vectorBit L, int pos)
	{
		int icount = 0;
		for (int i = 0; i < (pos<L.size() ? pos : L.size()); i++)
		{
			if (L[i] == 1)
				icount++;
		}
		return icount;
	}
	//}
	void destory(waveletTreeNodeByBit* child, waveletTreeNodeByBit* parent)
	{
		if (!child) return;
		destory(child->l, child);
		destory(child->r, child);
		if (parent == nullptr){
			root = nullptr;
			delete child;
			//child = nullptr;
		}
		else if (parent->l == child){
			parent->l = nullptr;
			delete child;
			//child = nullptr;
		}
		else{
			parent->r = nullptr;
			delete child;
			//child = nullptr;
		}
	}
	//void ~waveletTreeByBit()
	//{
	//	waveletTreeNodeByBit temp = root;

	//	if (!temp) return;
	//	delete
	//}
};
struct waveletTreeNode
{
	vector<int> tData;
	map<unsigned char, int> allist;
	char ch;
	waveletTreeNode *l,*r,*p;
	waveletTreeNode(): ch(0)
	{
		l = nullptr;
		r = nullptr;
		p = nullptr;
	}

	waveletTreeNode(waveletTreeNode& parentNode) :waveletTreeNode()
	{

		p = &parentNode;
	}
};
class waveletTree
{
	waveletTreeNode* root;
public:
	waveletTree() {
		root = new waveletTreeNode();
	}
	waveletTree(int alphbetCount) :waveletTree(){
	//layer = (int)ceil(float(log2(alphbetCount))) + 1;
	}
	waveletTreeNode* getRoot() const
	{
		return root;
	}
	//构造小波树
	void CounstructWaveletTree(const vector<unsigned char> inarray,const vector<unsigned char> alphbetList, waveletTreeNode &wt) const
	{
		//map<unsigned char, int> allist;
		unsigned int i;
		vector<unsigned char> lCharArray, rCharArray;
		typedef pair <unsigned char, int> In_Pair;
		if (alphbetList.size()>1)
		{
			float countlevel = (float(alphbetList.size()) / 2);
			for (i = 0; i < countlevel; i++)
				wt.allist.insert(In_Pair(alphbetList[i], 0));
			vector <unsigned char> lalphbetList(alphbetList.begin(), alphbetList.begin() + i);
			vector <unsigned char> ralphbetList(alphbetList.begin() + i, alphbetList.end());
			for (; i < alphbetList.size(); i++)
				wt.allist.insert(In_Pair(alphbetList[i], 1));
			for (i = 0; i < inarray.size(); i++)
				if (wt.allist[inarray[i]] == 0)
				{
				wt.tData.push_back(0);
				lCharArray.push_back(inarray[i]);
				}
				else
				{
					wt.tData.push_back(1);
					rCharArray.push_back(inarray[i]);
				}
				wt.l= new waveletTreeNode(wt);
				wt.r = new waveletTreeNode(wt);
			CounstructWaveletTree(lCharArray, lalphbetList, *wt.l);
			CounstructWaveletTree(rCharArray, ralphbetList, *wt.r);
		}
		else
		{
			vector<int> vector(inarray.size(), 1);
			wt.tData = vector;
			wt.ch = inarray[0];
		}


	}
	//字符字典中是否含有字符c。
	static bool ContainChar(const map<unsigned char, int>& allist, unsigned char c)
	{
		if (allist.find(c) == allist.end())
			return false;
		else 
			return true;
	}
	//返回值是BWT[pos]处的字符
	unsigned char Access(const int& pos) const
	{
	//	unsigned char  ch;
		int ipos = pos;
		waveletTreeNode* nodetemp = root;
		while (true)
		{
			if (nodetemp->tData[ipos] == 0)
			{
				ipos = ipos - rank1(nodetemp->tData, ipos);
				if (nodetemp->l!= nullptr)
					nodetemp = nodetemp->l;
				else
					break;
			}
			else
			{
				ipos = rank1(nodetemp->tData, ipos);
				if (nodetemp->r != nullptr)
					nodetemp = nodetemp->r;
				else
					break;				
			}
		}

		return nodetemp->ch;
	}
	///前pos个字符串中字符c的个数
	int Rank(const unsigned char c, const int& pos/*, const vector<unsigned char> alphbetList*/) const
	{
		waveletTreeNode* nodetemp = root;
		if (!ContainChar(nodetemp->allist, c))
			return -1;
	
		int ipos = pos;
		typedef pair <unsigned char, int> In_Pair;
		while (nodetemp->l != nullptr)
		{
			if (nodetemp->allist[c] == 0)
			{
				ipos = ipos - rank1(nodetemp->tData, ipos);
				nodetemp = nodetemp->l;
			}
			else
			{
				ipos = rank1(nodetemp->tData, ipos);
				nodetemp = nodetemp->r;
			}
		}
		return ipos;
		
	}

	
	
	int Select(const unsigned char c, const int& pos) const
	{
		int count;
		int postCur = 0;
		int postPre = pos;
		waveletTreeNode* nodetemp = root;
		if (!ContainChar(nodetemp->allist, c))
			return -1;
		while (nodetemp->l!= nullptr)
		{
			if (nodetemp->allist[c] == 0)
				nodetemp = nodetemp->l;
			else
				nodetemp = nodetemp->r;
		}
		postPre = nodetemp->tData.size() < postPre ? nodetemp->tData.size() : postPre;
		
		while (nodetemp->p != nullptr)
		{
			nodetemp = nodetemp->p;
			postCur = 0;
			count = 0;
			while (count<postPre)
			{
				if (nodetemp->tData[postCur++] == nodetemp->allist[c])
					count++;
			}
			postPre = postCur;

		}
		return postCur-1;
	}

	static int rank1(vector<int> L, int pos)
	{
		int icount = 0;
		for (int i = 0; i < (pos<L.size() ? pos : L.size()); i++)
		{
			if (L[i] ==1)
				icount++;
		}
		return icount;
	}
	//}
	~waveletTree()
	{

	}
};
//class GAMACode
//{
//public:
//	enum HEADERTYPE{ Plain, RLG0, RLG1, ALL0, ALL1 };
//	//HEADERTYPE header;
//	//vectorBit Svector;
//	///根据inarray进行gama编码
//	static vectorBit EncodeSingle(int Length);
//	static void Encode(vectorBit inarray, HEADERTYPE& header, vectorBit& Outvector);
//	///根据inarray进行gama译码
//	//	static void DecodeSingle()
//	static void Decode(vectorBit inarray, HEADERTYPE& header, vectorBit& Outvector, int length);
//	///inarray为s
//	static void Rank(vectorBit inarray, int offset, HEADERTYPE Head,int loffset)
//	{
//		
//	}
//};

vectorBit GAMACode::EncodeSingle(int Length)
{
	int i;
	vectorBit outarray;
	int size = log2(Length) + 1;
	for (i = 0; i < size - 1; i++)
	{
		outarray.push_back(0);
	}
	for (i = 0; i < size; i++)
	{
		int t = (Length >> (size - i - 1)) & 0x01;
		outarray.push_back(t);
	}
	return outarray;
}

void GAMACode::Encode(vectorBit inarray, HEADERTYPE& header, vectorBit& Outvector)
{
	if (inarray.size() < 1)
		return;
	int compareNum = inarray[0];
	//vector<int> mount;
	if (compareNum == 1)
		header = RLG1;
	else
		header = RLG0;
	int count = 1;
	for (int i = 1; i < inarray.size(); i++)
	{
		if (inarray[i] == compareNum)
			count++;
		else
		{
			Outvector.push_back(EncodeSingle(count));
			//count = 1;
			//	mount.push_back(count);
			count = 1;
			compareNum = inarray[i];
		}
		if (inarray.size() < Outvector.size())
		{
			header = Plain;
			break;
		}
	}
	if (count == inarray.size())
	{
		header = header == RLG1 ? ALL1 : ALL0;
		return;
	}
	Outvector.push_back(EncodeSingle(count));
	//mount.push_back(count);
	if (inarray.size() < Outvector.size())
		header = Plain;
}

/**
 * \brief 
 * \param inarray 
 * \param header 
 * \param Outvector 
 * \param length 
 */
void GAMACode::Decode(vectorBit inarray, HEADERTYPE& header, vectorBit& Outvector, int length)
{
	bool bval;
	//const int compareNum =0;
	int count0 = 0;
	switch (header)
	{
	case HEADERTYPE::Plain:
		Outvector = inarray;
		break;
	case HEADERTYPE::RLG0:
	case HEADERTYPE::RLG1:
		//int count0 = 0;
		bval = HEADERTYPE::RLG0 == header ? false : true;
		for (int i = 0; i < inarray.size(); )
		{
			if (inarray[i++] == 0)
				count0++;
			else
			{
				//todo wodo
				count0++;
				int mount = 1;
				AtlTrace("count0 = %d,", count0);
				for (int j = 0; j < count0-1 ; j++)
				{
					AtlTrace("%d,",inarray[i]);
					mount = (mount << 1) + inarray[i++];
				}
				AtlTrace("mount = %d\n", mount);
				Outvector.push_back(mount, bval);
				bval = bval ? false: true;
				count0 = 0;
			}
		}
		break;
	case HEADERTYPE::ALL0:
		Outvector = *(new vectorBit(length, 0));
		break;
	case HEADERTYPE::ALL1:
		Outvector = *(new vectorBit(length, 1));
		break;
	default: break;
	}
}

void quick_sort(unsigned char* s, int l, int r)
{
	if (l < r)
	{
		//Swap(s[l], s[(l + r) / 2]); //将中间的这个数和第一个数交换 参见注1  
		int i = l, j = r, x = s[l];
		while (i < j)
		{
			while (i < j && s[j] >= x) // 从右向左找第一个小于x的数  
				j--;
			if (i < j)
				s[i++] = s[j];

			while (i < j && s[i] < x) // 从左向右找第一个大于等于x的数  
				i++;
			if (i < j)
				s[j--] = s[i];
		}
		s[i] = x;
		quick_sort(s, l, i - 1); // 递归调用   
		quick_sort(s, i + 1, r);
	}
}
void quick_sort(vector<unsigned char> *s, int l, int r)
{
	if (l < r)
	{
		//Swap(s[l], s[(l + r) / 2]); //将中间的这个数和第一个数交换 参见注1  
		int i = l, j = r, x = s->at(l);
		while (i < j)
		{
			while (i < j && s->at(j) >= x) // 从右向左找第一个小于x的数  
				j--;
			if (i < j)
				//s[i++] = s[j];
				s->at(i++) = s->at(j);

			while (i < j &&s->at(i) < x) // 从左向右找第一个大于等于x的数  
				i++;
			if (i < j)
				s->at(j--) = s->at(i);
		}
		s->at(i) = x;
		quick_sort(s, l, i - 1); // 递归调用   
		quick_sort(s, i + 1, r);
	}
}
void Calcdelt(unsigned char* list, vector<unsigned char> *result,  int size)
{
	//int result = 0;
	vector<bool> b(256);

	for (int i = 0; i < size; i++)
	{
		if (!b[list[i]])
			result->push_back(list[i]);
		b[list[i]] = true;
	}
	

}
int Calcdelt(unsigned char* list, unsigned char *resultlist, int size)
{
	int result = 0;
	vector<bool> b(256);
	for (int i = 0; i < size; i++)
	{
		if (!b[list[i]])
			resultlist[result++]=(list[i]);
		
		b[list[i]] = true;
	}
	return result;
}
int cmp(int *r, int a, int b, int l)
{
	return r[a] == r[b] && r[a + l] == r[b + l];
}
void da(unsigned char  *r, int *sa, int n, int m)
{
	//n = n > m ? n : m;
	int* x = new int[n];
	int* y = new int[n];
	int* wv = new int[n > m ? n : m];
	int* ws = new int[n > m ? n : m];
	int i, j, p, *t;
	for (i = 0; i<m; i++) ws[i] = 0;
	for (i = 0; i<n; i++) ws[x[i] = r[i]]++;
	for (i = 1; i<m; i++) ws[i] += ws[i - 1];
	for (i = n - 1; i >= 0; i--)
		sa[--ws[x[i]]] = i;
	for (j = 1, p = 1; p<n; j *= 2, m = p)
	{
		for (p = 0, i = n - j; i<n; i++) y[p++] = i;
		for (i = 0; i<n; i++) if (sa[i] >= j) y[p++] = sa[i] - j;
		for (i = 0; i<n; i++) wv[i] = x[y[i]];
		for (i = 0; i<m; i++) ws[i] = 0;
		for (i = 0; i<n; i++) ws[wv[i]]++;
		for (i = 1; i<m; i++) ws[i] += ws[i - 1];
		for (i = n - 1; i >= 0; i--) sa[--ws[wv[i]]] = y[i];
		for (t = x, x = y, y = t, p = 1, x[sa[0]] = 0, i = 1; i < n; i++)
		{
			x[sa[i]] = cmp(y, sa[i - 1], sa[i], j) ? p - 1 : p++;
		}
	}
	delete[] wv;
	delete[] ws;
	delete[] x;
	delete[] y;
	return;
}
int rank1(unsigned char* L,int pos)
{
	int icount = 0;
	for (int i = 0; i < pos;i++)
	{
		if (L[i] == L[pos])
			icount++;
	}
	return icount;
}
void computerLF(int* C,unsigned char* L,int* LF,int length)
{
	int i;
	LF[0] = C[L[0]];
	for (i = 0; i < length;i++)
	{
		LF[i] = (C[L[i]] + rank1(L, i))%length;
	}
}
int constructBWT(unsigned char*T, vector<unsigned char> *L, int length)
{
//	int pos = 0;
	int posofend = 0;
	int* sa = new int[length];
	da(T, sa, length, 256);
	for (int i = 0; i < length; i++)
	{
		if (sa[i] == 0)
		{
			posofend = i;
			L->at(i) = T[(sa[i] - 1 + length) % length];
		}
		else
			L->at(i) = T[(sa[i] - 1) % length];
	}
	return  posofend;
}
int constructBWT(unsigned char*T, unsigned char*L,int length)
{
//	int pos = 0;
	int posofend = 0;
	int* sa = new int[length];
	da(T, sa, length, 256);
	for (int i = 0; i < length;i++)
	{
		//L[i] = T[(sa[i] - 1<0 ? sa[i] - 1 + length : sa[i] - 1) % length];
		if (sa[i] == 0)
		{
			posofend = i;
			L[i] = T[( sa[i] - 1 + length ) % length];
		}
		else 
			L[i] = T[ (sa[i] - 1) % length];
	}
	return  posofend;
}
void constructC(unsigned char*T, int* CTable, int length)
{
	int i;
	memset(CTable, 0, CSize * sizeof(int));
	for ( i = 0; i < length; i++)
	{
		CTable[T[i]+1]++;
	}
	for (i = 1; i < CSize; i++)
	{
		CTable[i] += CTable[i-1];
	}
}
void ReconstructT(unsigned char*L, int* LF, int I, unsigned char* T , int length)
{
	int i = I;
	for (int k = length - 1; k >= 0;k--)
	{
		T[k] = L[i];
		i = LF[i];
	}
}
unsigned char access(waveletTree& tree, int pos, vector<unsigned char> alphbetVector)
{

	return ' ';
}
int main()
{
	//todo mainpos
	vectorBit v,v1,v2;
	v.push_back(0);
	v.push_back(1);
	v.push_back(1);
	v.push_back(1);
	v.push_back(1);
	v.push_back(1);
	v.push_back(1);
	v.push_back(1);
	v.push_back(1);
	v.push_back(1);
	v.push_back(0);
	v.push_back(0); 
	GAMACode::HEADERTYPE head;
	GAMACode::Encode(v, head, v1);
	GAMACode::Decode(v1, head, v2,12);
}
//int _tmain(int argc, _TCHAR* argv[])
//{
//	FILE* fp;
//	//vector<unsigned char> alphbetVector,BWTvector;
//	unsigned char* alphbetList = new unsigned char[256];
//	int* CTable = new int[CSize];
//	int posOfend = 0;
//	//unsigned char* T = new unsigned char[21]{'a','s','a','k','a','k','r','h','a','k','a','k','r','h','a','k','a','s','a','k','#'};
//	const char* strpath = "G:\\测试数据\\normal\\dna";
//	if (fopen_s(&fp, strpath, "r"))
//	{
//		printf("The file %s is not exist.", strpath);
//		return 0;
//	}
//	int err = fseek(fp, 0L, SEEK_END);
//	long size = ftell(fp);
//	fseek(fp, 0, SEEK_SET);
//	size = size < SIZE ? size : SIZE;
//	unsigned char* list = new unsigned char[size];
//	long count = fread(list, sizeof(char), size, fp);
//	//unsigned char* inarray = new int[size];
//
//	//unsigned char* list = new unsigned char[size];
//	vector<unsigned char> Tvector;
//	unsigned char* T2 = new unsigned char[size];
//	const int length = size;
//	unsigned char* BWT = new unsigned char[length];
//	int* LF = new int[length];
//	posOfend = constructBWT(list, BWT, length);
//	constructC(list, CTable, length);
//	computerLF(CTable, BWT, LF, length);
//	ReconstructT(BWT, LF, posOfend, T2,length);
//	int alCount = Calcdelt(list, alphbetList, length);
//	quick_sort(alphbetList, 0, alCount-1);
////	waveletTree* tree = new waveletTree(alCount);
//	waveletTreeByBit* tree = new waveletTreeByBit(alCount);
//	vector<unsigned char> alphbetVector(alphbetList, alphbetList + alCount);
//	vector<unsigned char> BWTvector(BWT, BWT + length);
//	tree->CounstructWaveletTree(BWTvector, alphbetVector, *tree->getRoot());	
//	for (int i = 0; i < length; i++)
//	{
//		std::cout << BWT[i] << ",";
//	}
//	std::cout << endl; std::cout << endl;
//
//	for (int i = 0; i < length; i++)
//	{
//		std::cout << tree->Access(i) << ",";
//	}
//	std::cout << endl;
//	std::cout << tree->Rank('A', 13);
//	std::cout << endl;
//	std::cout << tree->Select('C', 10);
//	std::cout << endl;
//	tree->destory(tree->getRoot(), NULL);
//	return 0;
//}

//int _tmain(int argc, _TCHAR* argv[])
//{
//	//TIME_T t1, t2, ti;
//	FILE* fp;
//	struct _timeb  ts1, ts2;
//	//vector<unsigned char> alphbetVector, BWTvector;
//	unsigned char* alphbetList = new unsigned char[256];
//	int* CTable = new int[CSize];
//	const char* strpath = "G:\\测试数据\\normal\\dna";
//	if (fopen_s(&fp, strpath, "r"))
//	{
//		printf("The file %s is not exist.", strpath);
//		return 0;
//	}
//	int err = fseek(fp, 0L, SEEK_END);
//	long size = ftell(fp);
//	size = size < SIZE ? size : SIZE;
//	//unsigned char* inarray = new int[size];
//	int* sa3 = new int[size];
//	int* sa4 = new int[size];
//	fseek(fp, 0, SEEK_SET);
//	unsigned char* list = new unsigned char[size];
//	long count = fread(list, sizeof(char), size, fp);
//	ftime(&ts1);//开始计时
//	da(list, sa3, size, 256);
//	ftime(&ts2);//停止计时
//	//t1 = (TIME_T)ts1.time * 1000 + ts1.millitm;
//	//printf("t1=%lld\n", t1);
//	//t2 = (TIME_T)ts2.time * 1000 + ts2.millitm;
//	//ti = t2 - t1;//获取时间间隔，ms为单位的 10m 34.602ms;
//	//printf("ti of  da =%lld\n", ti);
//	ftime(&ts1);//开始计时
//	//sufffixArrayCreate(list, sa4, size, 256);
//	//int alCount = Calcdelt(T, alphbetList, size);
//	int alCount = Calcdelt(list, alphbetList, size);
//	//waveletTree* tree = new waveletTree(alCount);
//	waveletTreeByBit* tree = new waveletTreeByBit(alCount);
//	vector<unsigned char> alphbetVector(alphbetList, alphbetList + alCount);
//	vector<unsigned char> BWTvector(list, list + size);
//	tree->CounstructWaveletTree(BWTvector, alphbetVector, *tree->getRoot());
//	ftime(&ts2);//停止计时
//	//t1 = (TIME_T)ts1.time * 1000 + ts1.millitm;
//	//printf("t1 of myda=%lld\n", t1);
//	//t2 = (TIME_T)ts2.time * 1000 + ts2.millitm;
//	//ti = t2 - t1;//获取时间间隔，ms为单位的 10m 34.602ms;
////	printf("t1 of myda=%lld\n", ti);
//	for (int i = 0; i < size; i++)
//	{
//		if (sa4[i] != sa3[i])
//			printf("Wrong\n");
//	}
//	return 0;
//}