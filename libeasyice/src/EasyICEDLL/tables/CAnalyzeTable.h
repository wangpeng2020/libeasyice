/*
MIT License

Copyright  (c) 2009-2019 easyice

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* Generated by Together */

#ifndef CANALYZETABLE_H
#define CANALYZETABLE_H
#include "CBuildUpSection.h"
#include "TsPacket.h"
#include <vector>
#include <algorithm>

namespace tables{
	using namespace std;
/**
 * 解析PSI/SI 
 */
class CAnalyzeTable {
public:

    CAnalyzeTable();

    ~CAnalyzeTable();

	 /**
     * 解析之前调用此函数做预处理，用于：
     * 1.解析出的PMT pid加入过滤列表
	 * 2.没有找到PAT，返回 -1
     */
    //int ParsePATAndPMT(BYTE* pData, int nLength);
	int ParseFindPAT(BYTE* pData, int nLength);

	//设置network_pmt_pid列表，用于取代ParsePATAndPMT函数
	void SetNetworkPmtPidList(const vector<int>& vecPidList);

	  /**
     * 每当个TS包都经由此函数过滤
     * 根据PID列表过滤psi/si表的ts包，不对应直接返回
     * 首先要由PAT解析出PMT的PID，添加到PID过滤列表，此前不进行其他表的过滤
     */
    void PushBackTsPacket(CTsPacket* tsPacket);

	//不需要提前解析PAT。收到第一个PAT前其他表将得不到解析
	void PushBackTsPacket2(BYTE* pPacket);

    /**
     * 初始化。
     * 1在此初始化Pid过滤列表
     * 2.初始化m_mapSectionData
     */
    void Init();

	void SetTsPacketLength(int nLength);

	//获取解析好的表
	TABLES* GetTables();

public:
	vector<int> m_vecPmtPidList;

private:
    /**
     * 初始化PID列表。只在构造函数调用,额或者单独放到Init函数？待定。。
     */
    void InitPidFilterList();

	inline void ParsePATForPMTPID(const PAT& pat);

private:

    /**
     * 表示PSI/SI信息的PID列表
     * 根据此列表过滤出包含表的TS包
     * 对于PMT等需要动态获取的PID，需要在解析过程中由PAT分析后动态添加到此列表 
     */
    vector<int> m_vecPidFilterList;

	

    /**
     * pid，SECTION_DATA
     * 组section的缓冲 
     */
    SECTION_BUFFER m_mapSectionData;


    /**
     * 组zection 的类对象 
     */
    CBuildUpSection m_buildSection;

    /**
     * 解析完毕的表 
     */
    TABLES m_tables;

	//TS包长
	int	m_nTsLength;


	//是否已解析PAT
	bool m_bPatParsed;

	//解析PAT用
	CBuildUpSection m_buildSectionPat;

};

}

#endif //CANALYZETABLE_H
