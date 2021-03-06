// BWT.cpp : 定义控制台应用程序的入口点。
//
//#include "afx.h"
//#include <afxwin.h>
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
#include <time.h>
#include <bitset>
//#include <atltrace.h>
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
#define SIZE 1024*1024

#define BSize 64
#define SBSize 256


const int BRunlength = ceil(log2(BSize));
const int Bmask = (1 << BRunlength) - 1;
const int SBRunlength = ceil(log2(SBSize));
const int SBmask = (1 << SBRunlength) - 1;

using namespace std;


class vectorBitSingle
{
protected:
	vector<byte> tData;
	int CurrentPos;
public:
	//static vectorBitSingle ConstructABit(int value, int length)
	//{
	////	vectorBitSingle temp;

	//}
	friend class vectorBit;
	vectorBitSingle()
	{
		CurrentPos = 0;
	}
	vectorBitSingle(int sizeInBit) :vectorBitSingle()
	{
		tData.resize((sizeInBit >> 3)+1, 0);
	}
	vectorBitSingle(int length, byte val) :vectorBitSingle()
	{
		byte val1 = val == 0 ? 0x00 : 0xFF;
		//int i1 = ceil(float(length) / 8);
		int i1 = (length >> 3) + ((length&0x7)==0?0:1);
		CurrentPos = length;
		vector<byte> temp(i1, val1);
		tData = temp;
		tData.resize((length >> 3) + 1, val1);
	}

	void reserve(int input);
	void push_back(const unsigned char* input);
	void push_back(string input);

	int getcapacity() const
	{
		return tData.capacity();
	}
	/**
	 * \brief push one bit into vectorBitSingle
	 * \param input 0 or 1,the value of this bit
	 */
	void vectorBitSingle::push_back(bitset<16> input);
	void push_back(int input);
	void push_back(int count, bool value);
	void push_back(vectorBitSingle input);
	vectorBitSingle copy(int startpos, int count)
	{
		vectorBitSingle temp(count);
		count = CurrentPos> startpos + count ? count : CurrentPos - startpos;
		for (int i = 0; i < count; i++)
			temp.push_back(at(i + startpos));
		return temp;
	}
	vectorBitSingle copy()
	{
		vectorBitSingle temp(CurrentPos);
		for (int i = 0; i < CurrentPos; i++)
			temp.push_back(at(i));
		return temp;
	}
	int size() const
	{
		return CurrentPos;
	}
	void resize(unsigned int newSizeInbit)
	{
		tData.resize((newSizeInbit >> 3) + 1);
		//return CurrentPos;
	}
	void resizeToRealSize()
	{
		tData.resize((CurrentPos >> 3) + 1);
		//return CurrentPos;
	}
	void SetPos(int size) 
	{
		CurrentPos = size;
	}
	int vectorBitSingle::operator[](int i){
		return at(i);
	}

	int vectorBitSingle::at(int i){
		if (i < 0 || i >= CurrentPos)
			return 0;
		int currentPosInVectorByte = i >> 3;//
		int currentPosInByte = i & 0x7;//求8的模
		return (tData[currentPosInVectorByte] >> currentPosInByte) & 0x01;
	}
	vectorBitSingle& vectorBitSingle::operator=(const vectorBitSingle &temp){
		tData = temp.tData;
		CurrentPos = temp.CurrentPos;
		return *this;;
	}

};
class vectorBit 
{
private:
	vector<byte> tData;
	int bitcapacity = 1;
	int CurrentPos;//denote this is the CurrentPos,th int the vector
	int TheLogicAndNumberArray[64];

public:

	int getbitcapacity()
	{
		return  bitcapacity;
	}
	vectorBit() :CurrentPos(0)
	{

		for (int i = 0; i < 64; i++)
			TheLogicAndNumberArray[i] = (1 << i) - 1;
	}
	vectorBit(int bitlength) :vectorBit()
	{
		//bitcapacity = 1;
		bitcapacity = bitlength;
	}
	void init(int bitlength)
	{
		bitcapacity = bitlength;
	}
	void init(int bitlength, int vectorsize)
	{
		init(bitlength);
		tData.resize(((getLengthInBit()) >> 3) + 1, 0);
	}
	void reserve(int length)
	{
		tData.reserve(((getLengthInBit()) >> 3) + 1);
	}
	vectorBit(int bitlength, int vectorsize) :vectorBit(bitlength)
	{
		tData.resize(((getLengthInBit()) >> 3) + 1, 0);
	}
	///
	int getcapacity() const
	{
		//(tData.capacity() << 3) / bitcapacity)
		return ((tData.capacity() << 3) / bitcapacity) + ((tData.capacity() << 3) % bitcapacity == 0 ? 0 : 1);
	}
public:
	inline int  getCorrectLengthInput(int input)
	{
		return  input&TheLogicAndNumberArray[bitcapacity];
	}
	void  push_back(UINT64 input)
	{
		input = getCorrectLengthInput(input);
		int currentPosInVectorByte = getLengthInBit() >> 3;
		int currentPosInByte = getLengthInBit() & 0x7;
		byte lowByte = input&TheLogicAndNumberArray[8 - currentPosInByte];
		int size = tData.size();
		for (; getLengthInBit() + bitcapacity >= size << 3; size++)
		{
			tData.push_back(0);
		}
		tData[currentPosInVectorByte] = tData[currentPosInVectorByte] | (lowByte << currentPosInByte);
		currentPosInVectorByte++;
		input = input >> (8 - currentPosInByte);
		int i = 8 - currentPosInByte;//i denote the mount of bit pushed into the vector;
		for (; i < bitcapacity; i += 8)
		{
			tData[currentPosInVectorByte++] = input&TheLogicAndNumberArray[8];
			input = input >> 8;
		}
		//currentPosInByte = ((bitcapacity & 0x07) + currentPosInByte) & 0x07;//new 
		CurrentPos++;
	}
	inline int getLengthInBit() const
	{
		return  CurrentPos*bitcapacity;
	}
	vectorBit copy()
	{
		vectorBit temp(CurrentPos);
		for (int i = 0; i < CurrentPos; i++)
			temp.push_back(at(i));
		return temp;
	}
	int size() const
	{
		return CurrentPos;
	}
	int sizeOfOccupyMemory()
	{
		return tData.size();
	}
	void resize(unsigned int newSize)
	{
		tData.resize(((newSize*bitcapacity) >> 3) + 1);
	}
	void resizeToRealSize()
	{
		tData.resize((getLengthInBit() >> 3) + 1);
	}
	void SetPos(int size)
	{
		CurrentPos = size;
	}
	UINT64 vectorBit::operator[](int i){
		return at(i);
	}
	UINT64 vectorBit::at(int i)
	{
		if (i < 0 || i >= CurrentPos)
			return 0;

		UINT64 result;
		int highPosInbit = i*bitcapacity + bitcapacity;
		int currentPosInVectorByte = (highPosInbit) >> 3;
		int currentPosInByte = (highPosInbit & 0x7);
		int Length = currentPosInByte;
		int movebitPace = Length > bitcapacity ? Length - bitcapacity : 0;
		result = (tData[currentPosInVectorByte--]) & TheLogicAndNumberArray[Length];
		result = result >> movebitPace;
		int mountsOfPushIntoResult = Length;//mountsOfPushIntoResult denote the mount of bit pushed into the vector;
		for (; mountsOfPushIntoResult < bitcapacity;)
		{
			movebitPace = mountsOfPushIntoResult <bitcapacity - 8 ? 8 : bitcapacity - mountsOfPushIntoResult;
			result = result << movebitPace;
			result += (tData[currentPosInVectorByte] >> (8 - movebitPace)) & TheLogicAndNumberArray[movebitPace];
			--currentPosInVectorByte;
			mountsOfPushIntoResult += movebitPace;
		}
		return 	result;
	}
	void copy(vectorBitSingle& vbsingle,int BitRunlength)
	{
		tData = vbsingle.tData;
		CurrentPos = (vbsingle.size()+BitRunlength-1) / BitRunlength;
		bitcapacity = BitRunlength;
	}
};
//the number of bitcapacity must small than 64;



int ranksize = 0;
class BaisOperate
{
	//todo rank need to change to popcount
public:
	static int assembly_popcnt(unsigned int n)
	{
		#ifdef _MSC_VER /* Intel style assembly */
				__asm popcnt eax, n;
		#elif __GNUC__ /* AT&T style assembly */
				__asm__("popcnt %0,%%eax"::"r"(n));
		#else
		# error "which assembly style does your compiler support, Intel or AT&T?"
		#endif
	}
	static int StupidRank(unsigned char* inarray, int length,unsigned char c,int pos)
	{
		pos = length > pos ? pos: length;
		int icount = 0;
		for (int i = 0; i < pos;i++)
			if (inarray[i] == c)
				icount++;
		return icount;
	}
	static int rank1(vectorBitSingle inarray,int pos)
	{

		int count = 0;
		pos = pos < inarray.size() ? pos : inarray.size();
		for (int i = 0; i < pos; i++)
			if (inarray[i] == 1)
				count++;
		return count;
	}

	static int rank1(vectorBitSingle& inarray,int startpos, int mount)
	{
		
		ranksize = ranksize < inarray.size() ? inarray.size() : ranksize;
		int count = 0;
		int endpos = startpos + mount;
		endpos = endpos < inarray.size() ? endpos : inarray.size();
		for (int i = startpos; i < endpos; i++)
			if (inarray[i] == 1)
				count++;
		return count;

	}
	static int rank1(unsigned char* L, int size)
	{
		int icount = 0;
		for (int i = 0; i < size; i++)
		{
			if (L[i] == L[size])
				icount++;
		}
		return icount;
	}
	static int GetRvalue(int i,int itemID)
	{
		int result = 0;
		if (itemID == 1)
			result = (i >> 23) & 0x01;
		else if (itemID == 2)
			result = (i >> 16) & 0x10;
		else  if (itemID == 1)
			result = (i >> 8 )& 0xFF;
		else  if (itemID == 0)
			result = (i) & 0xFF;

		return result;
	}

};
class GAMACode
{
	vector<vectorBitSingle> GamaCodeToBitVector;
	vector<int> lookupTableOfR;
public:
	enum HEADERTYPE{ Plain, RLG0, RLG1, ALL0, ALL1 };
	//HEADERTYPE header;
	//vectorBitSingle Svector;
	///根据inarray进行gama编码
//	static vectorBitSingle EncodeSingle(int CurrentPos);
	void EncodeSingle(vectorBitSingle& outarray, int Length);
	vectorBitSingle EncodeSingle(const int& Length);
	void Encode(vectorBitSingle inarray, HEADERTYPE& header, vectorBitSingle& Outvector);
	tuple<int, int, int, int> Decode(vectorBitSingle& inarray, const HEADERTYPE& header, vectorBitSingle& Outvector, int length);
//	tuple<int, int, int, int> Decode(vectorBitSingle& inarray, HEADERTYPE& header, vectorBitSingle& Outvector, int length);
	///根据inarray进行gama译码
	//	static void DecodeSingle()

	void GenerateGamaCodeToBitVector()
	{
		if (GamaCodeToBitVector.size()>0)
			return;
		GamaCodeToBitVector.push_back(EncodeSingle(1));
		for (int i = 1; i <= BSize; i++)
			GamaCodeToBitVector.push_back(EncodeSingle(i));
	}
	///           31-----24 23-----16 15--8 7------0
	///            00000000 r1 000 r3   r2      r1
	void GeneratelookupTableOfR()
	{
		if (lookupTableOfR.size()>0)
			return;
		int looptimes = 1 << 16;
		lookupTableOfR.reserve(looptimes);
		for (int i = 0; i <= looptimes; i++)
		{
			vectorBitSingle vbSingle;
			vbSingle.push_back(bitset<16>(i));
			vectorBitSingle out;
			tuple<int, int, int, int> RvalueTuple = Decode(vbSingle, GAMACode::RLG1, out, BSize);
			int rvale = ((get <0>(RvalueTuple) & 0x01) << 23) |((get <2>(RvalueTuple) & 0xff) << 16) | ((get <1>(RvalueTuple) & 0xff )<< 8) | (get <3>(RvalueTuple) & 0xff);
			lookupTableOfR.push_back(rvale);
			//GamaCodeToBitVector.push_back(Decode());
		}
	}

	void Init()
	{	
		GeneratelookupTableOfR();
		GenerateGamaCodeToBitVector(); 
	}
};
class GamaCompressData
{
	typedef  vectorBit vectorBitHeader;

public:
	vectorBitHeader gamaHeader;
	vectorBitSingle	gamacode;
	vectorBit		SBrank;
	vectorBit		Brank;
	vectorBit		SB;
	vectorBit		B;
public:
	GamaCompressData()
	{
		//gamaHeader.init(3);
		gamaHeader = vectorBitHeader(3);
	};
	void Init(int bitLength)
	{
		int	SbitrunLength = log2(bitLength);
		int	BbitrunLength = BRunlength;
		SBrank = vectorBit(SbitrunLength);
		SB = vectorBit(SbitrunLength);
		B = vectorBit(SbitrunLength);
		Brank = vectorBit(SbitrunLength);
		SBrank.push_back(0);
		Brank.push_back(0);
		SB.push_back(0);
		B.push_back(0);
	}

	bool IsSuperBlock(int pos)
{
		int temp;
		//if(assembly_popcnt(pos)==1)
		//	temp = pos&SBmask;
		//else 
		//	temp = pos%SBSize; 
		int i = BaisOperate::assembly_popcnt(pos);
		temp = pos & SBmask;
		if (i==1)
			return temp;
		else
			temp = (pos%SBSize);
		return temp;
}
	void allocatMemeryForVectors(int i)
	{
		int SBlength = i / SBSize;
		int Blength = i / BSize;
		SBrank.resize(SBlength);
		SB.resize(SBlength);
		Brank.resize(Blength);
		B.resize(Blength);
		gamacode.resize(i);
		gamaHeader.resize(i);
	}
	void CreateDate(vectorBitSingle& inarray)
	{	
		Init(inarray.size());
		allocatMemeryForVectors(inarray.size());
		GAMACode gama;
		//gamaHeader;
		//gamacode;
		gama.Init();

		int i;
		int ranktemp;
		int SBrank_s = 0, SB_s = 0, Brank_s = 0, B_s = 0, SBrank_s_pre = 0, SB_s_pre =0 ;
		//int sbLength = inarray.size() >> SBRunlength;
		int sbLength = inarray.size() / SBSize;
		SBrank.reserve(sbLength);
		SB.reserve(sbLength);
		//int bLength = inarray.size() >> BRunlength;
		//int bLength = inarray.size() / BSize;
		//Brank.reserve(bLength);
		//B.reserve(bLength);
		for (i = 0; i < inarray.size(); )
		{
			//AtlTrace("%d,", i);
			vectorBitSingle bitetemp(BSize);
			//bitetemp.resize(BSize);
			GAMACode::HEADERTYPE headtemp;
			vectorBitSingle intemp = inarray.copy( i, BSize);
			gama.Encode(intemp, headtemp, bitetemp);	
			//AtlTrace("beforeCompress-length:%d,afterCompress-length:%d\n", intemp.size(), bitetemp.size());
			//////////////////////////////////////////////////
			gamaHeader.push_back(static_cast<int>(headtemp));
			gamacode.push_back(bitetemp);
			//////////////////////////////////////////////////
			B_s = bitetemp.size() + B_s;
			if (headtemp == GAMACode::ALL1)
				ranktemp = BSize;
			else if (headtemp == GAMACode::ALL0)
				ranktemp = 0;
			else
				//todo need to change
				ranktemp = BaisOperate::rank1(inarray, i, BSize);
			Brank_s = ranktemp + Brank_s;

//			AtlTrace("%d\n", Brank_s);
			i += BSize;
			//(pos + 1) & Bmask
			
			if (IsSuperBlock(i) != 0)
			{
				Brank.push_back(Brank_s - SBrank_s_pre);
				B.push_back(B_s - SB_s_pre);
			}
			else
			{
				SBrank_s_pre = Brank_s;
				SB_s_pre = B_s;
				SBrank.push_back(Brank_s);
				Brank.push_back(Brank_s - SBrank_s_pre);
				SB.push_back(B_s);
				B.push_back(B_s - SB_s_pre);
			}
			SBrank_s += ranktemp;
			SB_s += ranktemp;	
		}
		ResizeToRealsize();
		AtlTrace("beforeCompress-length:%d,afterCompress-length:%d\n", inarray.size()>>3, GetSize());
	}
	void ResizeToRealsize()
	{
		gamacode.resizeToRealSize();
		gamaHeader.resizeToRealSize();
		SBrank.resizeToRealSize();
		Brank.resizeToRealSize();
		SB.resizeToRealSize();
		B.resizeToRealSize();
	}
	int GetSize() 
	{
		return (gamaHeader.sizeOfOccupyMemory()) + (gamacode.size() >> 3) + SBrank.sizeOfOccupyMemory() + Brank.sizeOfOccupyMemory() + SB.sizeOfOccupyMemory() + B.sizeOfOccupyMemory();
	}
};
//class waveletTreeNodeByBit_C
//{
//	vectorBitSingle tData;//store S
//	vector<vectorBitSingle> headervector;//Sotre header use 3bit;
//	vector<unsigned int> SBrank;
//	vector<unsigned int> Brank;
//	vector<unsigned int> SB;
//	vector<unsigned int> B;
//};
//class waveletTreeByBit_C
//{
//	waveletTreeNodeByBit_C* root;
//public:
//	waveletTreeByBit_C() {
//		root = new waveletTreeNodeByBit_C();
//	}
//	//waveletTreeByBit(int alphbetCount) :waveletTreeByBit(){
//	//	//layer = (int)ceil(float(log2(alphbetCount))) + 1;
//	//}
//	waveletTreeNodeByBit_C* getRoot() const
//	{
//		return root;
//	}
//};
struct waveletTreeNodeByBit
{
	vectorBitSingle tData;
	map<unsigned char, int> allist;
	char ch;
	GamaCompressData GamaData;
	waveletTreeNodeByBit *l, *r, *p;
	waveletTreeNodeByBit(): ch(0)
	{
		//tData(0);
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
		int icount = 0;
		wt.tData.resize(inarray.size());
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
			wt.GamaData.CreateDate(wt.tData);
			/*if (wt.GamaData.gamacode.size() == 477)
				AtlTrace("here");*/
			wt.GamaData.ResizeToRealsize();
			wt.l = new waveletTreeNodeByBit(wt);
			wt.r = new waveletTreeNodeByBit(wt);
			icount++;
			CounstructWaveletTree(lCharArray, lalphbetList, *wt.l);
			CounstructWaveletTree(rCharArray, ralphbetList, *wt.r);
		}
		else
		{
			vectorBitSingle vector(static_cast<int>(inarray.size()), '1');
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
				ipos = ipos - BaisOperate::rank1(nodetemp->tData, ipos);
				if (nodetemp->l != nullptr)
					nodetemp = nodetemp->l;
				else
					break;
			}
			else
			{
				ipos = BaisOperate::rank1(nodetemp->tData, ipos);
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
		AtlTrace("Start Rank: now pos =%d\n", pos);
		waveletTreeNodeByBit* nodetemp = root;
		if (!ContainChar(nodetemp->allist, c))
			return -1;
		int ipos = pos;
		//typedef pair <unsigned char, int> In_Pair;
		while (nodetemp->l != nullptr)
		{
			//Divedepos = i;

			if (nodetemp->allist[c] == 0)
			{				
				ipos = ipos - BaisOperate::rank1(nodetemp->tData, ipos);
				AtlTrace("lnode,:now pos =%d\n", ipos);
				nodetemp = nodetemp->l;
			}
			else
			{
				
				ipos = BaisOperate::rank1(nodetemp->tData, ipos);
				AtlTrace("rnode,:now pos =%d\n", ipos);
				nodetemp = nodetemp->r;
			}
		}
		AtlTrace("End Rank\n");
		return ipos;

	}
	int RankOFGama(const unsigned char c, const int& pos/*, const vector<unsigned char> alphbetList*/) const
	{
		AtlTrace("Start RankOFGama:now pos =%d\n", pos);
		waveletTreeNodeByBit* nodetemp = root;
		if (!ContainChar(nodetemp->allist, c))
			return -1;
		int ipos = pos;
		//typedef pair <unsigned char, int> In_Pair;
		while (nodetemp->l != nullptr)
		{
			//Divedepos = i;
			if (nodetemp->allist[c] == 0) //c is 0
			{
				
				ipos = ipos - rankOfCurrentGama(nodetemp->GamaData, ipos-1);
				AtlTrace("lnode,:now pos =%d\n", ipos);
				nodetemp = nodetemp->l;
			}
			else//c is 1
			{
				
				ipos = rankOfCurrentGama(nodetemp->GamaData, ipos-1);
				AtlTrace("rnode,:now pos =%d\n", ipos);
				nodetemp = nodetemp->r;
			}
		}
		AtlTrace("End RankOFGama\n");
		return ipos;

	}
	static int rankOfCurrentGama(GamaCompressData& GamaData,const int& pos /*, const vector<unsigned char> alphbetList*/)
	{
		//waveletTreeNodeByBit* nodetemp = root;
		                       
		int offset =GamaData.SB[(pos + 1) / SBSize] + GamaData.B[(pos + 1) / BSize];
		int header = GamaData.gamaHeader[(pos + 1) / BSize];
		int rankresult = GamaData.SBrank[(pos + 1) / SBSize] + GamaData.Brank[(pos + 1) / BSize] + lrank(GamaData.gamacode, offset, header, (pos + 1) & Bmask );
		return  rankresult;
	}
	static int lrank(vectorBitSingle s, int offset, int headertype, int mount) 
	{
		//todo finish lrank with lookuptable
		int rankval = 0;

		return rankval;
	/*	bool bval;
		//const int compareNum =0;
		int count0 = 0;
		int Bpos = 0;
		int rankval=0; 
		switch (headertype)
		{
		case GAMACode::Plain:
			return BaisOperate::rank1(s, offset, offset + mount);
		case GAMACode::RLG0:
		case GAMACode::RLG1:
			bval = GAMACode::RLG0 == headertype ? false : true;
			int i = offset;
			//while (i+16)
			for (i = offset; Bpos <mount ;)
			{
				
				if (s[i++] == 0)
					count0++;
				else
				{
					//todo wodo
					count0++;
					int m1 = 1;//count of runlength;
					for (int j = 0; j < count0 - 1; j++)
					{
						m1 = (m1 << 1) + s[i++];
					}
					if (bval)
					{
						if (Bpos + m1>mount)
							rankval += m1 - (Bpos + m1 - mount);
						else
							rankval += m1;
						
					}
					bval = bval ? false : true;
					
					count0 = 0;
					Bpos += m1;
				}
			}

			break;
		case GAMACode::ALL0:
			return 0;
		case GAMACode::ALL1:
			return mount;
		default: break;
		}*/
		
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
	static int calculateGamaCodeSize(waveletTreeNodeByBit& wt)
	{
		int count = 0;
		if (wt.l != nullptr)	
			count = calculateGamaCodeSize(*wt.l) + calculateGamaCodeSize(*wt.r);

		
		count += wt.GamaData.GetSize();

		return count;
	}

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

};


void vectorBitSingle::reserve(int input)
{
	tData.reserve(input >> 3);
}
void vectorBitSingle::push_back(const unsigned char* input)
{
	int i = 0;
	while (input[i] != '\0')
	{
		if (input[i] == '1')
			this->push_back(1);
		else if (input[i] == '0')
			this->push_back(0);
		i++;
	}
}
void vectorBitSingle::push_back(bitset<16> input)
{
	//input.reve
	for (int i = 0; i < input.size();i++)
	{
		push_back(input[input.size()-1-i]);
	}
}
void vectorBitSingle::push_back(int input)
{
	int i1 = CurrentPos >> 3;//
	int i2 = CurrentPos & 0x7;//求8的模
	if (i1 + 1 > tData.size())
	{
		tData.push_back(0);
	}
	CurrentPos++;
	tData[i1] = input ? setbit(tData[i1], i2) : clrbit(tData[i1], i2);
}/**
 * \brief push the number of count bits into vectorBitSingle
 * \param count 0 or 1,the value pushed is value
 * \param value the number of bit pushed in
 */
void vectorBitSingle::push_back(int count, bool value)
{
	for (int i = 0; i < count; i++)
		this->push_back(value);

}

void vectorBitSingle::push_back(vectorBitSingle input)
{
	int i1 = CurrentPos >> 3;//
	int i2 = CurrentPos & 0x7;//求8的模
	for (int i = 0; i < input.CurrentPos; i++)
	{
		//if (i2 == 0)
		//{
		//	tData.push_back(0);
		//}
		CurrentPos++;
		tData[i1] = input[i] ? setbit(tData[i1], i2) : clrbit(tData[i1], i2);
		if (i2++ == 7)
		{
			i2 = 0;
			i1++;
		}
	}
}

void GAMACode::EncodeSingle(vectorBitSingle& outarray, int Length)
{
	//int i;
	//int size = log2(Length) + 1;
	outarray.push_back(this->GamaCodeToBitVector[Length]);
}

 vectorBitSingle GAMACode::EncodeSingle(const int& RunLength)
{

	 int i;
	 
	 int size = log2(RunLength) + 1;
	 vectorBitSingle outarray(size<<1);
	 for (i = 0; i < size - 1; i++)
	 {
		 outarray.push_back(0);
	 }
	 for (i = 0; i < size; i++)
	 {
		 int t = (RunLength >> (size - i - 1)) & 0x01;
		 outarray.push_back(t);
	 }
	 //AtlTrace("beforeCompress-length:%d,afterCompress-length:%d\n", RunLength, size * 2 -1);
	return outarray;
 }
void GAMACode::Encode(vectorBitSingle inarray, HEADERTYPE& header, vectorBitSingle& Outvector)
{
	//AtlTrace("-------------------\n");
	if (inarray.size() < 1)
		return;
	int compareNum = inarray[0];
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
			EncodeSingle(Outvector, count);
			count = 1;
			compareNum = inarray[i];
			
		}
		if (inarray.size() <= Outvector.size())
		{
			header = Plain;
			Outvector.SetPos(0);
			Outvector = inarray.copy();
			return;
		}
	}
	if (count == inarray.size())//ALL 0/1
	{
		header = header == RLG1 ? ALL1 : ALL0;
		Outvector.SetPos(0);
		return;
	}
	EncodeSingle(Outvector, count);
	//mount.push_back(count);
	if (inarray.size() < Outvector.size())//plain
	{
		header = Plain;
		Outvector.SetPos(inarray.size());
		Outvector = inarray.copy();
	}
	
}
//tuple<int, int, int, int> GetLookupTableElement(vectorBitSingle& inarray, HEADERTYPE& header, int length)
//{
//	int r1, r2, r3, r4;
//	switch (header)
//	{
//		case HEADERTYPE::Plain:
//			r1 = 0; r2 = 0; r3 = 16; r4 = BaisOperate::rank1(inarray, inarray.size());
//			break;
//
//	}
//	tuple<int, int, int, int> result(r1, r2, r3, r4);
//	return result;
//}
tuple<int,int,int,int> GAMACode::Decode(vectorBitSingle& inarray, const HEADERTYPE& header, vectorBitSingle& Outvector, int length)
{
	int r1=0, r2=0, r3=0, r4=0;
	int PrePos = 0;
	bool bval;
	//const int compareNum =0;
	int count0 = 0;
	switch (header)
	{
	case HEADERTYPE::Plain:
		r1 = 0; r2 = 0; r3 = 16; r4 = BaisOperate::rank1(inarray, inarray.size());
		Outvector = inarray;
		break;
	case HEADERTYPE::RLG0:
	case HEADERTYPE::RLG1:
		bval = HEADERTYPE::RLG0 == header ? false : true;
		for (int i = 0; i + count0 < inarray.size();)
		{
			if (inarray[i++] == 0)
				count0++;
			else
			{
				//todo wodo
				count0++;
				int mount = 1;
				for (int j = 0; j < count0 - 1 & i< inarray.size(); j++)
				{
					mount = (mount << 1) + inarray[i++];
				}
				if (bval)
				{
					r4 += mount;
				}
				r2 += mount;
				r3 = i;
				r1++;
			//	AtlTrace("mount = %d\n", mount);
				Outvector.push_back(mount, bval);
				bval = bval ? false: true;
				PrePos = i;
				count0 = 0;
				
			}
		}
		break;
	case HEADERTYPE::ALL0:
		r1 = 0; r2 = 0; r3 = 0; r4 = 0;
		Outvector = *(new vectorBitSingle(length, 0));
		break;
	case HEADERTYPE::ALL1:
		r1 = 0; r2 = 16; r3 = 16; r4 = 8;
		Outvector = *(new vectorBitSingle(length, 1));
		break;
	default: break;
	}
	tuple<int, int, int, int> result(r1, r2, r3, r4);
	return result;
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

void computerLF(int* C,unsigned char* L,int* LF,int length)
{
	int i;
	LF[0] = C[L[0]];
	for (i = 0; i < length;i++)
	{
		LF[i] = (C[L[i]] +BaisOperate::rank1(L, i))%length;
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
unsigned char access(waveletTreeByBit& tree, int pos, vector<unsigned char> alphbetVector)
{

	return ' ';
}
int main()
{
	//
	vectorBitSingle v,v1,v2;
	//const unsigned char ch[] = "1001111001010101";
	//const unsigned char ch[] =   "0000000111111111";
	const unsigned char ch[] = "1000000011111111";
	v.push_back(ch);
	/*GAMACode::HEADERTYPE head;
	GAMACode::Encode(v, head, v1);
	GAMACode::Decode(v1, head, v2,12);*/
	//int result = BaisOperate::rank1(v);
	//AtlTrace("%d", result);
	GamaCompressData gamacode;
	GAMACode gc;

	tuple<int, int, int, int> p = gc.Decode(v, GAMACode::RLG1, v1, v.size());
	AtlTrace("%d,%d,%d,%d\n", get<0>(p), get<1>(p), get<2>(p), get<3>(p));
	gamacode.CreateDate(v);
	AtlTrace("new size  =  %d \n", gamacode.GetSize());
	for (int i = 0; i < 84; i++)
	{
	//	int r = waveletTreeByBit::rankOfCurrentGama(i, gamacode);
	//	AtlTrace("Rank(%d) =  %d \n", i,r);
	}
}
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	
//	vectorBit bit(15);
//	//bit.push_back(10);
//	//bit.push_back(10);
//	for (int t = 0; t < 1024;t++)
//	{
//		bit.push_back(t);
//	}
//	for (int t =0; t < 1024; t++)
//	{
//		if (bit.at(t) != (t))
//			printf("bit[%d] is  wrong! bit.at(t)  = %d\n", t, bit.at(t));
//	}
//	clock_t start, end;
//	clock_t startall, endall;
//	double duration;
//	FILE* fp;
//	//vector<unsigned char> alphbetVector,BWTvector;
//	unsigned char* alphbetList = new unsigned char[256];
//	int* CTable = new int[CSize];
//	int posOfend;
//	//unsigned char* T = new unsigned char[21]{'a','s','a','k','a','k','r','h','a','k','a','k','r','h','a','k','a','s','a','k','#'};
//	const char* strpath = "E:\\测试数据\\测试数据\\normal\\\para";
//	//TCHAR* strpath = _T("E:\\测试数据\\测试数据\\english");
//	errno_t err;
//	//err = fopen_s(&fp, strpath, "r");
//	if (fopen_s(&fp, strpath, "r"))
//	{
//		printf("The file %s is not exist.", strpath);
//		return 0;
//	}
//	// ReSharper disable once CppEntityNeverUsed
//	err = fseek(fp, 0L, SEEK_END);
//	
//	long size = ftell(fp);
//	fseek(fp, 0, SEEK_SET);
//	size = size < SIZE ? size : SIZE;
//	unsigned char* list = new unsigned char[size];
//	// ReSharper disable once CppEntityNeverUsed
//	long count = fread(list,sizeof(char), size,fp);
//	//unsigned char* inarray = new int[size];
//	printf("File Name ;%s,\nFile Size :%d M\n", strpath, count / 1024 / 1024);
//	//unsigned char* list = new unsigned char[size];
//	vector<unsigned char> Tvector;
//	unsigned char* T2 = new unsigned char[size];
//	const int length = size;
//	unsigned char* BWT = new unsigned char[length];
//	int* LF = new int[length];
//	start = clock();
//	startall = clock();
//	posOfend = constructBWT(list, BWT, length);
//	end = clock();
//	duration = static_cast<double>(end - start) / CLOCKS_PER_SEC;
//	printf("durationOF constructBWT %f\n", duration); 
//	start = clock();
//	constructC(list, CTable, length);
//	end = clock();
//	duration = static_cast<double>(end - start) / CLOCKS_PER_SEC;
//	//printf("durationOF constructC %f\n", duration);
//	start = clock();
//	//computerLF(CTable, BWT, LF, length);
//	end = clock();
//	duration = static_cast<double>(end - start) / CLOCKS_PER_SEC;
////	printf("durationOF computerLF %f\n", duration);
//	start = clock();
//	//ReconstructT(BWT, LF, posOfend, T2,length);
//	end = clock();
//	duration = static_cast<double>(end - start) / CLOCKS_PER_SEC;
//	//printf("durationOF ReconstructT %f\n", duration);
//	int alCount = Calcdelt(list, alphbetList, length);
//	quick_sort(alphbetList, 0, alCount-1);
////	waveletTree* tree = new waveletTree(alCount);
//	waveletTreeByBit* tree = new waveletTreeByBit();
//	vector<unsigned char> alphbetVector(alphbetList, alphbetList + alCount);
//	vector<unsigned char> BWTvector(BWT, BWT + length);
//	start = clock();
//	tree->CounstructWaveletTree(BWTvector, alphbetVector, *tree->getRoot());
//	end = clock();
//	endall = clock();
//	duration = static_cast<double>(end - start) / CLOCKS_PER_SEC;
//	printf("durationOF CounstructWaveletTree %f\n", duration);
//	duration = static_cast<double>(endall - startall) / CLOCKS_PER_SEC;
//	printf("durationOF ALlProcess %f\n", duration);
//	auto node = tree->getRoot();
//	
//	int size1 = waveletTreeByBit::calculateGamaCodeSize(*node);
//	duration = static_cast<double>(size1 / length);
//	printf("durationOF ALlProcess %f\n", duration);
//	double per = static_cast<double>(size1) / static_cast<double>(length);
//	printf("new size is  %d\n,old is %d\n,%f\n", size1, length, per);
//	printf(" ranksize =%d", ranksize);
// //	for (int i = 219; i < size;i++)
//	//{
//	//	int stupid = BaisOperate::StupidRank(BWT, length, 'a', i);
//	//	int gama = tree->RankOFGama('a', i);
//	//	int wavelet = tree->Rank('a', i);
//	//	if (stupid != gama)
//	//		AtlTrace("pos:%d\n,stupid:%d,gamarank:%d", i, stupid, gama);
//	//}
//	
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