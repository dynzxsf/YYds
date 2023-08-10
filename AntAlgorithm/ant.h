#pragma once
#include "map.h"
#include "set.h"
#include <list>
using std::list;

/// �������ӣ���Ϣ�ص���Ҫ�̶�
#define ALPHA 1.0 
/// �������ӣ��ڵ��������Ҫ�̶�
#define BETA 2   
/// ��Ϣ�ز�������
#define ROU  0.8 
/// ��������
#define N_ANT_COUNT 50
/// ��������
#define N_IT_COUNT 200 
/// ���ڵ�����
#define N_NODE_COUNT 600 
/// �ܵ���Ϣ��
#define DBQ 100.0		 
/// һ����־����10��9�η�
#define DB_MAX 1000000000 
/// �����ڵ����Ϣ�أ����ǻ�����Ϣ��
extern double g_Trial[N_NODE_COUNT][N_NODE_COUNT];

///		rnd����

///		����ָ����Χ�ڵ����������
///     @param dbLow		��Χ��ʼ��    
///     @param dbUpper  ��Χ��ֹ��
///     @return					���صĴ˷�Χ�ڵ������ 
double rnd(double dbLow, double dbUpper);

/// ������CAnt		
/// 
/// �๦�ܣ����嵥��������Ѱ·��صĳ�Ա�����ͳ�Ա����
class CAnt
{
public:
	CAnt(void) { };
	~CAnt(void) { };
public:
	/// �����ߵ�·��
	int m_nPath[N_NODE_COUNT];
	/// �����߹���·������(ʵ�ʾ���ָ����ֵ)
	int m_nPathLength;
	/// ����ûȥ���Ľڵ�
	int m_nAllowedNode[N_NODE_COUNT];
	/// ��ǰ���ڽڵ���
	int m_nCurNodeNo;
	/// �Ѿ�ȥ���Ľڵ�����
	int m_nMovedNodeCount;
	/// ��ʾ���������Ƿ񵽴��յ�,ͬʱͨ�����бع���
	bool m_finish;
	/// ��ʾ�ܶԵ�ǰ���ϲ�����������including set���ؾ��㣩�ĵ�ļ��ϣ�һֻ�������������ĳ��set�ĵ㣬��ô�����Ͳ����ٲ���Ӱ��������Ҫ�����޳�����
	list<int> influenceSet;
	/// ��ʾ��ǰ���ϵıؾ��ߵļ��ϣ�һ�����Ͼ���������ؾ��ߣ�����Ҫ�����ͷŵ���
	list<int> mustEdgeSet;
	/// ��ʾ��ǰ���ϵĽ�ֹ�ߵļ��ϡ�
	list<int> banEdgeSet;
	/// ��ʾ�����ߵ�·�������нڵ��У��ж��ٸ���including set���ؾ��㣩�����
	int m_nIncluding;
	/// ��ʾ�����ߵ�·���Ľڵ�������Ƶ����ֵ
	int limit_vertex;
	/// ��¼�����߹��Ľ�ֹ�ߵĸ���
	int arcBanNum;
public:
	///		getInfluence����

	///		ȡ��ָ����ŵĽڵ㵽�����������ڵ����С����
	///     @param num ����Ľڵ���    
	///     @return      �������������С·��ֵ 
	int getInfluence(int num);
	///		ChooseNextNode����

	///		����ѡ����һ���ڵ�
	///     @return      ����ѡ�����һ��·���ڵ�ı��
	///     @note		  ���ϸ��ݵ����������������С·�������ת�Ƹ��ʣ��������������ķ���ѡ����һ��Ҫ�ߵ�·���ڵ�  
	int ChooseNextNode();
	///		Init����

	///		��ʼ��������Ϣ
	///     @note		  ��ʼ�����ϵĳ�Ա������������ʼ�㡢��ֹ�㡢�ؾ��㼯�ϡ��ؾ��߼��ϡ���ֹ�㼯�ϵȵ�  
	void Init();
	///		Move����

	///		�����ڽڵ���ƶ�
	///     @return      �����Ͽ����ߵ���һ���ڵ��򷵻�1����֮������·�����򷵻�2
	///     @note		  ��·����˵�����ع���û������(��Ϊ�����˾Ͳ���������Move����)��ͬʱ��û����һ�����ߵ�·
	int Move();
	///		Search����

	///		��������·��
	///     @note		  ���Ͻ���·��������������������Ҫ�����˳�����������·�����ȼ��㣬��֮��һֱ����MoveѰ·״̬
	void Search();
	///		CalPathLength����

	///		���������߹���·������
	///     @note		  ��Dijkstra�㷨��������ֹ�����̾������������Ҽ����ܵ�·������
	void CalPathLength();
	///		deleteElement����

	///		���������ƶ���ɾ�����������ϵ�Ԫ��
	///     @param num   ����ıؾ������    
	///     @note		  ���Ͼ���һ���ؾ���󣬴�������������ɾ���˽ڵ㣬�����ñؾ��㽫����������������
	void deleteElement(int num);
	///		deleteEdge����

	///		�������Ͼ����ؾ��ߺ�ɾ���ؾ���
	///     @param linkid   ����ıؾ��ߵ�LinkID���    
	///     @note		  ���Ͼ���һ���ؾ��ߺ󣬴ӱؾ��߼�����ɾ���˱ߵı�ţ������ñؾ��߽�����������������
	void deleteEdge(int linkid);
};