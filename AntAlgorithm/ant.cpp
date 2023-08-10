#include <cmath>
#include <iostream>
#include "ant.h"
#include "map.h"
#include "set.h"
#include "time.h"
#include "stack"
using std::stack;

extern int minDistance[600][600];
extern list<int> initInfluenceSet;
extern list<int> initMustEdge;
extern list<int> initBanEdge;
extern std::vector<int> mustedge_file;
extern std::vector<int> ban_file;
extern int limit;
extern int Flag_EndVertex_Must;
extern int includeNodeNum;
/// �����ڵ����Ϣ�أ����ǻ�����Ϣ��
double g_Trial[N_NODE_COUNT][N_NODE_COUNT];

///		rnd����

///		����ָ����Χ�ڵ����������
///     @param dbLow		��Χ��ʼ��    
///     @param dbUpper  ��Χ��ֹ��
///     @return					���صĴ˷�Χ�ڵ������ 
double rnd(double dbLow, double dbUpper)
{
	double dbTemp = rand() / ((double)RAND_MAX + 1.0);
	return dbLow + dbTemp * (dbUpper - dbLow);
}

///		Init����

///		��ʼ��������Ϣ
///     @note		  ��ʼ�����ϵĳ�Ա������������ʼ�㡢��ֹ�㡢�ؾ��㼯�ϡ��ؾ��߼��ϡ���ֹ�㼯�ϵȵ�  
void CAnt::Init()
{
	for (int i = 0; i < N_NODE_COUNT; i++)
	{
		m_nAllowedNode[i] = 1;					   //����ȫ���ڵ�Ϊû��ȥ����������ȥ
		m_nPath[i] = -1;								   //�����ߵ�·��ȫ������Ϊ-1
	}
	///��Ѱ·�Ĺ����У��յ���������ǲ��ɼ��ġ�ֻ�е���������ȫ���ع��ڵ�󣬲Ż��ֶ���m_nAllowedNode[end]����Ϊ1
	if (Flag_EndVertex_Must == 0)  //�ؾ��߰����յ�
		m_nAllowedNode[end] = 0;
	else                                        //�ؾ��߲������յ�
		m_nAllowedNode[end] = 1;
	///�����߹���·����������Ϊ0
	m_nPathLength = 0;
	///�����ڵ�Ϊ��ʼ��
	m_nCurNodeNo = start;
	///�ѳ����ڵ㱣����·��������
	m_nPath[0] = m_nCurNodeNo;
	///�Ѿ�ȥ���Ľڵ���������Ϊ1����Ϊ���Ҳ����ȥ���Ľڵ�
	m_nMovedNodeCount = 1;
	///·�������нڵ����m_nIncluding��including set�еĽڵ�
	m_nIncluding = 0;
	///���·��������־λΪ0
	m_finish = false;
	///�ó�ʼ�����������ϳ�ʼ��influenceSet
	influenceSet = initInfluenceSet;
	///�ó�ʼ�ıؾ��߼��ϳ�ʼ��mustEdgeSet
	mustEdgeSet = initMustEdge;
	///�ó�ʼ�Ľ�ֹ�߼��ϳ�ʼ��banEdgeSet
	banEdgeSet = initBanEdge;
	///�ó�ʼ�Ľ�ֹ�߳�ʼ��limit_vertex
	limit_vertex = limit;
	///��¼�����߹��Ľ�ֹ�ߵĸ���
	arcBanNum = 0;
}

///		deleteElement����

///		���������ƶ���ɾ�����������ϵ�Ԫ��
///     @param num   ����ıؾ������    
///     @note		  ���Ͼ���һ���ؾ���󣬴�������������ɾ���˽ڵ㣬�����ñؾ��㽫����������������
void CAnt::deleteElement(int key)
{
	list<int>::iterator Itor;
	for (Itor = influenceSet.begin(); Itor != influenceSet.end();)
	{
		if (*Itor == key)
		{
			Itor = influenceSet.erase(Itor);
		}
		else
		{
			Itor++;
		}
	}
}

///		deleteEdge����

///		�������Ͼ����ؾ��ߺ�ɾ���ؾ���
///     @param linkid   ����ıؾ��ߵ�LinkID���    
///     @note		  ���Ͼ���һ���ؾ��ߺ󣬴ӱؾ��߼�����ɾ���˱ߵı�ţ������ñؾ��߽�����������������
void CAnt::deleteEdge(int linkid)
{
	list<int>::iterator Itor;
	for (Itor = mustEdgeSet.begin(); Itor != mustEdgeSet.end();)
	{
		if (*Itor == linkid)
		{
			Itor = mustEdgeSet.erase(Itor);
		}
		else
		{
			Itor++;
		}
	}
}

///		getInfluence����

///		ȡ��ָ����ŵĽڵ㵽�����������ڵ����С����
///     @param num ����Ľڵ���    
///     @return      �������������С·��ֵ 
int CAnt::getInfluence(int num)
{
	list<int>::iterator Itor;
	int val = 100000000;
	for (Itor = influenceSet.begin(); Itor != influenceSet.end(); Itor++)
	{
		if (val > minDistance[num][*Itor])
		{
			val = minDistance[num][*Itor];
		}
	}
	return val;
}

///		ChooseNextNode����

///		����ѡ����һ���ڵ�
///     @return      ����ѡ�����һ��·���ڵ�ı��
///     @note		  ���ϸ��ݵ����������������С·�������ת�Ƹ��ʣ��������������ķ���ѡ����һ��Ҫ�ߵ�·���ڵ� 
int CAnt::ChooseNextNode()
{
	//�˿����ϻ�û�������ض���
	int nSelectedNode = -1; //���ؽ��������ʱ��������Ϊ-1
	double dbTotal = 0.0;
	double prob[8] = { -1,-1,-1,-1,-1,-1,-1,-1 };		///�������ϴӵ�ǰ�ڵ㵽�����ڽӵ�ȥ�ĸ��ʣ�ת�Ƹ��ʣ�
	int pos[8] = { -1,-1,-1,-1,-1,-1,-1,-1 };		    ///��prob����ĸ��ʶ�Ӧ����ʾ��Ӧ���ʶ�Ӧ�Ľڵ�number
	int i = 0;
	pArcNode parc = G.vertices[m_nCurNodeNo].adjlist.head;//��ǰ�ڵ���ڽ������ͷָ��
	//���㵱ǰ�ڵ�������ڵ��ת�Ƹ���
	for (; parc != NULL; parc = parc->nextarc)
	{
		if (m_nAllowedNode[parc->adjvex] == 1)  ///ûȥ�� ( �����������ϲ�ֱ��ȥ�յ㣬���Ǿ������бؾ�����ȥ )
		{
			double flu = getInfluence(parc->adjvex) + parc->arcinfo.weight;	///�õ������ߵ�������
			prob[i] = pow(g_Trial[m_nCurNodeNo][parc->adjvex], ALPHA) * pow(1.0 / flu, BETA);
			//���������ǲ��Ǳؾ���
			if (!mustEdgeSet.empty())
			{
				for (auto itor = mustEdgeSet.begin(); itor != mustEdgeSet.end(); itor++)
				{
					if (*itor == parc->arcinfo.arcnum)
					{
						prob[i] = 10;
						break;
					}
				}
			}
			//���������ǲ��ǽ�ֹ��
			if (!banEdgeSet.empty())
			{
				for (auto itor = banEdgeSet.begin(); itor != banEdgeSet.end(); itor++)
				{
					if (*itor == parc->arcinfo.arcnum)
					{
						prob[i] = 0;
					}
				}
			}
			pos[i] = parc->adjvex;
			dbTotal = dbTotal + prob[i];
			i++;
		}
	}	//���iΪ��ǰ�ڵ���һ�������ߵ�·������Ŀ
	if (i == 0)
	{
		return -1;//��ʾ��ǰ�ڵ���һ����·���ߣ�����-1.��i=0������ǰ�ڵ�����ڽڵ㶼�Ѿ�ȥ����
	}
	int nAvailable = i;//��ʾ��ǰ�ڵ���һ�������ߵ�·������Ŀ

	double dbTemp = 0.0;
	if (dbTotal > 0.0)
	{
		dbTemp = rnd(0.0, dbTotal);
		for (int i = 0; i < nAvailable; i++)
		{
			dbTemp = dbTemp - prob[i];
			if (dbTemp < 0.0)
			{
				nSelectedNode = pos[i];
				break;
			}
		}
	}
	if (nSelectedNode == -1)
	{
		nSelectedNode = pos[0];
	}

	return nSelectedNode;	//���ؽ�������ǽڵ�ı��
}
///		Move����

///		�����ڽڵ���ƶ�
///     @return      �����Ͽ����ߵ���һ���ڵ��򷵻�1����֮������·�����򷵻�2
///     @note		  ��·����˵�����ع���û������(��Ϊ�����˾Ͳ���������Move����)��ͬʱ��û����һ�����ߵ�·
int CAnt::Move()
{
	int nNodeNo = ChooseNextNode();		//ѡ����һ���ڵ�
	if (nNodeNo != -1)							    //˵�����нڵ������
	{
		m_nPath[m_nMovedNodeCount] = nNodeNo;				//���������ߵ�·��
		int CurLinkID = GetLinkID(m_nCurNodeNo, nNodeNo);  //ȡ����ǰ�ڵ����һ��Ҫ���Ľڵ�֮���LinkID��
		m_nCurNodeNo = nNodeNo;										//�ı䵱ǰ���ڽڵ�Ϊѡ��Ľڵ�
		m_nMovedNodeCount++;											//�Ѿ�ȥ���Ľڵ�������1

		/*�������Ƿ�Ҫ�� ��û���ߵıؾ���*/
		if (includingSet.FindSet(nNodeNo, &includingSet) == demandSet[0])
		{
			m_nIncluding++;
			deleteElement(nNodeNo);	//���ѡ��Ľڵ���influence set����ĵ㣬�����influence set��ɾ��
			if (m_nIncluding == includeNodeNum)
				m_nAllowedNode[end] = 1;
		}
		/*�������Ƿ�Ҫ�� ��û���ߵıؾ���*/
		if (!mustEdgeSet.empty())
		{
			for (auto itor = mustedge_file.begin(); itor != mustedge_file.end(); itor++)
			{
				if (*itor == CurLinkID)
				{
					deleteEdge(CurLinkID);			//���ѡ��ı���mustEdgeSet����ĵ㣬�����mustEdgeSet��ɾ��
				}
			}
		}
		/*�������Ƿ����� ��ֹ��*/
		for (auto itor = ban_file.begin(); itor != ban_file.end(); itor++)
		{
			if (*itor == CurLinkID)
			{
				arcBanNum++;
			}
		}
		return 1;
	}
	else											 //˵����·�����ˣ���ֻ�����ߵ�ͷ��
		return 2;
}

///		Search����

///		��������·��
///     @note		  ���Ͻ���·��������������������Ҫ�����˳�����������·�����ȼ��㣬��֮��һֱ����MoveѰ·״̬
void CAnt::Search()
{
	///���ϳ�ʼ��
	Init();
	///ֻҪ�����߹���·���а������ض��㻹û�дﵽҪ�����Ŀ����������һֱmove
	while (!(influenceSet.empty() && mustEdgeSet.empty()) && 1 == Move());
	///�������������·������
	if (m_nIncluding == includeNodeNum)
		CalPathLength();
}

///		CalPathLength����

///		���������߹���·������
///     @note		  ��Dijkstra�㷨��������ֹ�����̾������������Ҽ����ܵ�·������
void CAnt::CalPathLength()
{
	int m = 0;
	int n = 0;
	int i = 1;
	int vnum = G.vexnum;
	m_nPathLength = 0;
	int dist[600], pres[600];
	Dijkstra(vnum, m_nPath[m_nMovedNodeCount - 1], dist, pres, g_Distance);
	if (dist[end] < 100000000)
	{
		m_nPathLength += dist[end];
		m_finish = true;//˵�������һ��·������
	}
	else
	{
		//std::cout<<"Error\n";
		return;
	}
	while (m_nPath[i] != -1)
	{
		m = m_nPath[i];
		n = m_nPath[i - 1];
		m_nPathLength = m_nPathLength + g_Distance[n][m];
		i++;
	}

	stack<int> sta;
	int temp = end;
	while (pres[temp] != m_nPath[m_nMovedNodeCount - 1])
	{
		sta.push(temp);
		temp = pres[temp];
	}
	sta.push(temp);
	while (!sta.empty())
	{
		int temp = sta.top();
		m_nPath[m_nMovedNodeCount] = temp;
		sta.pop();
		m_nMovedNodeCount++;
	}
}
