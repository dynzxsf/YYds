#include "ShortestPath.h"
#include "string.h"
#include "map.h"
#include <iostream>
#include <queue>
#include <random>
#include <time.h>
using std::queue;
extern int includeNodeNum;
extern list<int> initMustEdge;
/// ·���ڵ����Ƿ������ƽڵ���֮��		1*/
bool if_nodeNum;
/// �ؾ���==Ҫ����		2*/
bool if_nodeNeed;
///	�ؾ���==Ҫ����        3*/
bool if_arcNeed;
///	�Ƿ����˽�ֹ��		    4*/
bool  if_arcBan;
///	�Ƿ���л����ٻ��߽ڵ����ٵ�ѡ��
bool if_56 = false;
/// ���汻ȥ���ı�ID
list<int > arcObsolete;
/// �����������
std::default_random_engine eng((unsigned)time(NULL));

///		deleteArc����

///		ɾ��������ɾ���ıؾ���
///     @param MustEdge		ָ��ؾ���ļ��ϵ�����
///     @param arcObsolete		ָ�򱣴��ֹ�ߵļ��ϵ�����
///     @return							�����Ѿ�ɾ���Ľ�ֹ�ߵļ���
list<int> deleteArc(list<int >& MustEdge, list<int >& arcObsolete)
{
	std::uniform_int_distribution<int> randInt(1, MustEdge.size());
	int deleteArcAt = randInt(eng);
	int j = 1;
	if (MustEdge.size() != 0)
		for (auto i = MustEdge.begin(); i != MustEdge.end(); i++, j++)
		{
			if (j == deleteArcAt)
			{
				arcObsolete.push_back(*i);
				MustEdge.erase(i);
				break;
			}
		}
	return arcObsolete;
}

///		InitData����

///		��ʼ�����ϵ���Ϣ�طֲ�
///     @note		  ��ʼ��topoͼ�����������ߵ���Ϣ�ش�СΪ10000
void CShortestPath::InitData()
{
	const int inf = 100000000;
	m_cGlobalBestAnt.m_nPathLength = inf;//��ʼ��ȫ����������·���������
	pArcNode parc = NULL;
	/*��ʼ�����бߵ���Ϣ��Ϊ10000.0,��������������ڱߵĻ�����Ϣ��Ϊ0*/
	for (int i = 0; i < G.vexnum; i++)    //��ʼ��ÿ���ߵ���Ϣ��Ϊ10000
	{
		parc = G.vertices[i].adjlist.head;
		while (parc != NULL)
		{
			g_Trial[i][parc->adjvex] = 10000.0;//��ʼ��ͼ�е�ÿ��·������Ϣ��Ϊ10000.0
			parc = parc->nextarc;
		}
	}
	/*��һ�ε���ʱ����û��ȫ�����Ž⣬���м��㲻�����ֵ����Сֵ��������Ϊ0.0*/
	Qmax = 0.0;
	Qmin = 0.0;
	m_dbRate = 15;//�����Ϣ�غ���С��Ϣ�صı�ֵΪ15��
}

///		UpdateTrial����

///		����·������Ϣ�طֲ�
///     @param flag  ��ʾ�Ƿ�ʹ��ȫ�����Ž⣬1��ʾʹ��ȫ�����Ž⣬0��ʾʹ�õ������Ž� 
///     @note			 ʹ��ȫ�����ź͵������Ž�����µĲ��ԣ�ÿ5�ε�������һ����Ϣ��
void CShortestPath::UpdateTrial(int nFlag)
{
	if (nFlag == 1)			//ʹ��ȫ�����Ž�
	{
		if (m_cGlobalBestAnt.m_nPathLength == 100000000)
		{
			return;		//����ε�����ȫ���������ϻ�������
		}
		m_cTempAnt = m_cGlobalBestAnt;
	}
	else							//ʹ�õ������Ž�
	{
		if (m_cIterationBestAnt.m_nPathLength == 100000000)
		{
			return;		//��ε���û�в���һ����
		}
		m_cTempAnt = m_cIterationBestAnt;
	}
	double dbTempAry[N_NODE_COUNT][N_NODE_COUNT];
	memset(dbTempAry, 0, sizeof(dbTempAry));
	int m = 0;
	int n = 0;

	/*ֻ��ȫ�����Ż���ĳ�ε�������������ȥ������Ϣ��*/
	double dbTemp = 1.0 / m_cTempAnt.m_nPathLength;
	for (int j = 1; j <= m_cTempAnt.m_nMovedNodeCount; j++)
	{
		m = m_cTempAnt.m_nPath[j];
		n = m_cTempAnt.m_nPath[j - 1];
		dbTempAry[n][m] = dbTempAry[n][m] + dbTemp;
	}

	/*���»�����Ϣ�أ���Ҫ�ο������Ϣ�غ���С��Ϣ��*/
	Qmax = 1.0 / (m_cGlobalBestAnt.m_nPathLength * (1.0 - ROU));
	Qmin = Qmax / m_dbRate;
	/*------------------------------------------------------------*/
	pArcNode parc = NULL;
	for (int i = 0; i < G.vexnum; i++)
	{
		parc = G.vertices[i].adjlist.head;
		while (parc != NULL)
		{
			int x = parc->adjvex;
			g_Trial[i][x] = g_Trial[i][x] * ROU + dbTempAry[i][x];
			if (g_Trial[i][x] < Qmin)
			{
				g_Trial[i][x] = Qmin;
			}
			if (g_Trial[i][x] > Qmax)
			{
				g_Trial[i][x] = Qmax;
			}
			parc = parc->nextarc;
		}//while
	}//for
}


///		Search����

///		Ѱ��ȫ������������ȡ������·����ͬʱ��Լ��ģ���жϴ˴�Ѱ·�Ƿ��н�
///     @note		  �������������������н��򷵻����Ž⣻���޽������Լ��ģ���ٴν���Ѱ·ֱ�������������һ�����Ž�
void CShortestPath::Search()
{
	const int inf = 100000000;
	char costOrNode = 'a';
	m_cGlobalBestAnt.m_nPathLength = inf;//��ʼ��ȫ���������ϵ�·������ΪINT_MAX

	while (true)
	{
		for (int i = 0; i < N_IT_COUNT; i++) //����
		{
			m_cIterationBestAnt.m_nPathLength = inf;//��ʼ�����ε��������Ž�ΪINT_MAX
			//ÿֻ��������һ��
			for (int j = 0; j < N_ANT_COUNT; j++) {//һ�ε����ų��������ϳ�ȥ����
				m_cAntAry[j].Search();
			}//for
			/*-----------------------------------------------*/
			//������ѽ��
			switch (costOrNode)//�ж��û�ѡ��cost���Ȼ�Node������
			{
			case 'a':
				for (int j = 0; j < N_ANT_COUNT; j++)
				{
					if (m_cAntAry[j].m_finish && m_cAntAry[j].m_nPathLength < m_cGlobalBestAnt.m_nPathLength) {
						m_cGlobalBestAnt = m_cAntAry[j];//����·�����ȣ�cost������ȫ�ֵ���������
					}
					if (m_cAntAry[j].m_finish && m_cAntAry[j].m_nPathLength < m_cIterationBestAnt.m_nPathLength) {
						m_cIterationBestAnt = m_cAntAry[j];//���±��ε�������������
					}
				}//for
				break;
			case 'b':
				for (int j = 0; j < N_ANT_COUNT; j++)
				{
					if (m_cAntAry[j].m_finish && m_cAntAry[j].m_nMovedNodeCount < m_cGlobalBestAnt.m_nMovedNodeCount) {
						m_cGlobalBestAnt = m_cAntAry[j];//���ݽڵ�������ȫ�ֵ���������
					}
					if (m_cAntAry[j].m_finish && m_cAntAry[j].m_nMovedNodeCount < m_cIterationBestAnt.m_nMovedNodeCount) {
						m_cIterationBestAnt = m_cAntAry[j];//���±��ε�������������
					}
				}//for    
				break;
			default:
				std::cout << "�������,Ĭ��a��cost��С��\n"; costOrNode = 'a';
				break;
			}
			//ʹ��ȫ�����ź͵������Ž�����µĲ���
			//ÿ5�ε���ʹ��һ��ȫ���������ϸ�����Ϣ��
			if ((i + 1) % 5 == 0)
			{
				UpdateTrial(1);//ʹ��ȫ�����Ÿ�����Ϣ��
			}
			else
			{
				UpdateTrial(0);//ʹ�þֲ����Ÿ�����Ϣ��
			}
		}//for

		if_nodeNum = (m_cGlobalBestAnt.m_nMovedNodeCount <= m_cGlobalBestAnt.limit_vertex);  /* ���Ƶ��������       1 */
		if_nodeNeed = m_cGlobalBestAnt.influenceSet.empty();														 /* �ؾ���==Ҫ����       2 */
		if_arcNeed = m_cGlobalBestAnt.mustEdgeSet.empty();														 /* �ؾ���==Ҫ����       3 */
		if_arcBan = (m_cGlobalBestAnt.arcBanNum == 0);																 /* ���˶��ٽ�ֹ��        4 */

		//    std::cout<<"global: "<<m_cGlobalBestAnt.m_nMovedNodeCount <<" "<<m_cGlobalBestAnt.influenceSet.size()<<" "<<m_cGlobalBestAnt.mustEdgeSet.size()<< " "<<m_cGlobalBestAnt.arcBanNum<<std::endl;
		if (if_nodeNum && if_nodeNeed && if_arcNeed && if_arcBan) /*1,2,3,4*/
		{
			printPath();
			break;
		}
		else if (if_56 == false)//�û�ѡ��5,6
		{
			if_56 = true;
			std::cout << "\n����ȫ����Ҫ���·������ѡ���������ִ���·����\n";
			std::cout << "a����������\tb�������ڵ�������\n";
			std::cin >> costOrNode;
			if (costOrNode == 'b')
			{
				continue;
			}
		}

		if (if_nodeNeed && if_arcNeed && if_arcBan)/*2,3,4 */
		{
			printPath();
			break;
		}
		else if (initMustEdge.size() > 0) /*3 ��ʼ���ؾ�����>2(�������ܵ���ʼ����ֹ�����ڵı�)*/
		{
			arcObsolete = deleteArc(initMustEdge, arcObsolete); //����ɾ���ıؾ��� 
			continue;
		}
		else if (if_nodeNum && if_nodeNeed && if_arcBan) /*1,2,4*/  /*���˴�·���к��еıؾ�����==0*/
		{
			printPath();
			break;
		}
		else if (if_nodeNeed && if_arcBan)/*2,4 */
		{
			printPath();
			break;
		}
		else
		{
			std::cout << "��ֹ��=" << m_cGlobalBestAnt.arcBanNum << std::endl;
			std::cout << "�޽�\n";
			break;
		}
	}//while

}// Search

///		printPath����

///		������ŵ�·��
///     @note      ��ȫ�������������ߵ�·�������õĻ��ѣ����д�ӡ��� 
void CShortestPath::printPath()
{
	std::cout << "\n���·��Ϊ��    ";
	std::cout << m_cGlobalBestAnt.m_nPath[0];
	int i = 1;
	for (; i < m_cGlobalBestAnt.m_nMovedNodeCount; i++)
		if (m_cGlobalBestAnt.m_nPath[i] != end)
			std::cout << "-->" << m_cGlobalBestAnt.m_nPath[i];
	std::cout << "-->" << m_cGlobalBestAnt.m_nPath[i - 1] << std::endl;
	std::cout << "���� = " << m_cGlobalBestAnt.m_nPathLength << std::endl;
}