#ifndef __QQDATA_DEF_H__
#define __QQDATA_DEF_H__

#include <vector>
#include <mutex>
#include <pthread.h>
#include <list>
#include <stdint.h>
#include <string>

#define  QQDATA_BLOCK_NUMBER    6

class CQQData
{
public:
	typedef struct
	{
		size_t id;
		size_t packindex;
		size_t packLen;
		size_t packcopyedLen;
		std::string buffer;
	}pcmblock_t,*pcmblock_pt;
	int totalLen;
	int copyedLen;
	int localsize;
	int packindex;
	std::string songid;
	CQQData();
	~CQQData();
	void Clear();
	int CopyData(char* pData, int len);
	bool GetData(pcmblock_pt ptblock);
	void ChangePack(int index,int len);
	void resetblock( pcmblock_pt ptblock );

private:
	std::list<pcmblock_pt> m_blockList;
	std::list<pcmblock_pt> m_freeList;
	pcmblock_pt pcur_block;
	pcmblock_t m_blocks[QQDATA_BLOCK_NUMBER];
	std::recursive_mutex m_mtx;
};

enum{
	QQMusic_Type_None,
	QQMusic_Type_Song,
	QQMusic_Type_Dir,
	QQMusic_Type_Radio,
};

struct stQQMusicItem
{
	unsigned char type;
	std::string id;
	std::string name;
	std::string artist;
	std::string album;
	int duration;
};

struct stQQMediaInfo
{
	int rate;
	int channel;
	int bit;
	int length;
	std::string id;
	int errorcode;
	uint32_t querytime;
	int playpos;
	unsigned char retrycount;
	stQQMusicItem id3;
	bool Valid()
	{
		return rate != 0 && channel != 0 && bit != 0 &&  \
				length != 0 && errorcode == 0;
	}
	void Clear()
	{
		rate = 0;
		channel = 0;
		bit = 0;
		length = 0;
		errorcode = 0;
		querytime = 0;
		retrycount = 0;
		playpos = 0;
	}
	bool operator != (const stQQMediaInfo &other)
	{
		return !( this->rate == other.rate && this->channel == other.channel && this->bit == other.bit && this->id == other.id );
	}
};

class CQQMusicList
{
public:
	std::string parentid;
	std::string parentname;
	int pageindex;
	int count;
	int errorcode;
	uint32_t querytime;
	std::vector<stQQMusicItem> items;
	bool IsDownloadComplete()
	{
		if (items.size() < count || pageindex == -1)
			return false;
		return true;
	}
	void Clear()
	{
		pageindex = -1;
		querytime = 0;
		items.clear();
	}
};

#endif
