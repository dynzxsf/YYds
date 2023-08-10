#include "map.h"
#include <string.h>
#include "set.h"
#include <list>
#include <string>
#include <sstream>
using std::list;
extern int includeNodeNum;
extern int edge_num;

/// ȫ�ֱؾ��㼯��
list<int> initInfluenceSet;
/// ȫ�ֱؾ��߼���
list<int> initMustEdge;
/// ȫ�ֽ�ֹ�߼���
list<int> initBanEdge;
/// ȫ�����ƽڵ�
int limit;
/// ���ڶԽڵ���м���,��ʼ��Ϊ0
int bitFlagArray_1[1 + MAX_VERTEX_NUM / BITSPERWORD] = { 0 };
/// ���ڶ�Ҫ�󾭹��ĵ���б��
int bitFlagArray_2[1 + MAX_DEMAND_NUM / BITSPERWORD] = { 0 };
/// ���ڴ��Ҫ��
int demandSet[60];
/// Ѱ·��ʼ��
int start;
/// Ѱ·�յ�
int end;
/// �жϱؾ����Ƿ����յ�ı�־λ
int Flag_EndVertex_Must = 0;
/// ���ڴ洢ͼ��·����Ϣ
int g_Distance[600][600];
/// ��ͼ��һ�����޸ĵ�ʱ�򣬻��õ�
int g_Distance_temp[600][600];
/// including set�еĵ�ļ���
UnionFindSet includingSet;
/// infָ����������
const int inf = 100000000;

void set(int i, int a[])
{
	a[i >> SHIFT] |= (1 << (i & MASK));
}
void clr(int i, int a[])
{
	a[i >> SHIFT] &= ~(1 << (i & MASK));
}
int test(int i, int a[])
{
	return a[i >> SHIFT] & (1 << (i & MASK));
}

///		MakeNode����

///		���ݴ��ļ���ȡ�����ݹ���һ���ڽ�����Ľڵ㣨������һ���ߣ�
///     @param linkid				����ıߵı��
///		@param destinationid		����ıߵ�ָ��ڵ�
///		@param cost					����ıߵ�Ȩ��
///     @return							���ع���ıߵ�ָ��
ArcNode* MakeNode(unsigned int linkid, unsigned int destinationid, unsigned int cost)
{
	ArcNode* pArcNode;
	if ((pArcNode = (ArcNode*) new ArcNode) == NULL)
		return NULL;
	pArcNode->nextarc = NULL;
	pArcNode->adjvex = destinationid;		//�ñ�ָ��Ķ���
	(pArcNode->arcinfo).arcnum = linkid;	//�ñߵı��
	(pArcNode->arcinfo).weight = cost;		//�ñߵ�Ȩ��
	return pArcNode;
}

///��ʼ��ͼ����Ϣ
void InitInfo()
{
	int i = 0;
	G.arcnum = edge_num;
	G.vexnum = 0;
	for (; i < MAX_VERTEX_NUM; i++)
	{
		G.vertices[i].adjlist.head = NULL;
		G.vertices[i].adjlist.tail = NULL;
		G.vertices[i].adjlist.len = 0;
		G.vertices[i].nodeinfo.inDegree = 0;
		G.vertices[i].nodeinfo.outDegree = 0;
		G.vertices[i].nodeinfo.distance = INT_MAX;
		G.vertices[i].nodeinfo.num = i;			//��¼�Լ��Ľڵ����Ϣ
	}
}

///��buffer�л�ȡ��Ӧ��linkid��sourceid��destinationid��cost
void GetValue(int value[], char buffer[])
{
	unsigned int i = 0, j = 0, k = 0;
	for (; j < 4; j++)
	{
		k = i;
		while (!(buffer[i] == ',' || buffer[i] == '|' || buffer[i] == '\n' || buffer[i] == '\0'))
			i++;
		buffer[i++] = '\0';
		value[j] = atoi(buffer + k);
	}
}

///		AddNode����

///		�ڽڵ� i ���ڽ�����β�����һ����
///     @param i						�ڵ�
///		@param pArcNode		ָ��Ҫ��ӵıߵ�ָ��
void AddNode(unsigned int i, ArcNode* pArcNode)
{

	if (G.vertices[i].adjlist.len == 0)		//�ж������Ҫ��ӵĶ����Ƿ���ĳ��������ڽ�����ĵ�һ���ڵ�
		G.vertices[i].adjlist.head = pArcNode;
	else
		(G.vertices[i].adjlist.tail)->nextarc = pArcNode;
	G.vertices[i].adjlist.tail = pArcNode;
	G.vertices[i].adjlist.len++;
	//�������� if ͳ�ƽڵ����
	if (!test(i, bitFlagArray_1))
	{
		G.vexnum++;
		set(i, bitFlagArray_1);
	}
	if (!test(pArcNode->adjvex, bitFlagArray_1))
	{
		G.vexnum++;
		set(pArcNode->adjvex, bitFlagArray_1);
	}
	G.vertices[i].nodeinfo.outDegree += 1;  //�ڵ� i ����+1
	G.vertices[pArcNode->adjvex].nodeinfo.inDegree += 1;  //�¼���Ľڵ����+1
}

///		InitMap����

///		�����ļ���ʼ��ͼ����Ϣ
///     @param topo[]				����ͼ��Ϣ������ָ��
void InitMap(char* topo[])
{
	ArcNode* pArcNode;
	int value[4];		//�ֱ���linkid,sourceid,destinationid,cost
	InitInfo();
	for (int i = 0; i < edge_num; i++)
	{
		GetValue(value, topo[i]);
		g_Distance[value[1]][value[2]] = value[3];		//�������ʽ�洢ͼ����Ϣ
		g_Distance_temp[value[1]][value[2]] = value[3];
		if ((pArcNode = MakeNode(value[0], value[2], value[3])) == NULL)
		{
			printf("�����ڽ�����ڵ�ʧ�ܣ�\n");
			return;
		}
		//���������������ӵ��ڵ��Ϊsourceid���ڽ�������ȥ
		AddNode(value[1], pArcNode);
	}
	/*��ͼ�в����ڵıߵ�Ȩֵ��Ϊinf */
	for (int i = 0; i < G.vexnum; i++)
	{
		for (int j = 0; j < G.vexnum; j++)
		{
			if (g_Distance[i][j] == 0)
			{
				g_Distance[i][j] = inf;
				g_Distance_temp[i][j] = inf;
			}
		}
	}
	return;
}

///		InitDemandSet����

///		��ʼ���ؾ��������������
///     @param buf				�洢ȫ�ֱؾ���ļ���
///		@param bufsize			�ؾ���ĸ���
///		@param StartVertex	Ѱ·�����
///     @param EndVertex	Ѱ·���յ�
///     @return						�ؾ���ĸ���
int InitDemandSet(std::vector<int> buf, int bufsize, int StartVertex, int EndVertex)
{
	for (int i = 0; i < bufsize; i++)
	{
		demandSet[i] = buf[i];
	}
	start = StartVertex;
	end = EndVertex;
	/*��ʼ��including set����*/
	InitUnionSet(&includingSet, G.vexnum);
	/*����including set�ļ���*/
	for (int i = 0; i < bufsize; i++)
	{
		includingSet.UnionSet(demandSet[i], demandSet[0], &includingSet);        //demandSet[0]��Ϊ���ڵ㡣
		initInfluenceSet.push_back(demandSet[i]);			//��ʼ���ܶ����ϲ����������Ľڵ㼯�ϡ�
	}
	return bufsize;
}

///		InitMustEdge����

///		/��ʼ���ؾ���ļ���
///     @param buf				�洢ȫ�ֱؾ��ߵļ���
///		@param bufsize			�ؾ��ߵĸ���
///     @return						�ؾ��ߵĸ���
int InitMustEdge(std::vector<int> buf, int bufsize)
{
	for (int i = 0; i < bufsize; i++)
	{
		initMustEdge.push_back(buf[i]);
	}
	return initMustEdge.size();
}

///		InitBanEdge����

///		��ʼ����ֹ�ߵļ��ϡ����ƽڵ�����ֵ
///     @param buf				�洢ȫ�ֽ�ֹ�ߵļ���
///		@param bufsize			��ֹ�ߵĸ���
///     @param num				���ƽڵ�����ֵ
///     @return						��ֹ�ߵĸ���
int InitBanEdge(std::vector<int> buf, int bufsize, int num)
{
	for (int i = 0; i < bufsize; i++)
	{
		initBanEdge.push_back(buf[i]);
	}
	limit = num;
	return initBanEdge.size();
}

///		InputDemand����

///		������β�ڵ㡢�ؾ������Ϣ
///     @param demand				ָ��ȫ�ֱؾ��������
///		@param VertexStart			ָ�������ŵ�ָ��
///     @param VertexEnd			ָ���յ���ŵ�ָ��
///     @return								�ؾ���ĸ���
int InputDemand(std::vector<int>& demand, int* VertexStart, int* VertexEnd)
{
	/*���������յ�*/
	int i = 0;
	std::string line;
	std::string word;
	std::cout << "���������յ� (�ո��ϣ��س�����)��  \n ";
	getline(std::cin, line);
	std::istringstream record(line);
	std::string tmp;
	while (record >> word)
	{
		tmp = word;
		if (0 == i)
			*VertexStart = atoi(tmp.c_str());
		else if (1 == i)
			*VertexEnd = atoi(tmp.c_str());
		else
		{
			std::cerr << "��������\n";
			exit(1);
		}
		++i;
	}
	std::cin.clear();

	/*����ؾ���*/
	std::string line_1;
	std::string word_1;
	std::cout << "����ؾ��� (�ո��ϣ��س�����)�� \n ";
	getline(std::cin, line_1);
	std::istringstream rtc(line_1);
	while (rtc >> word_1)
	{
		demand.push_back((atoi(word_1.c_str())));
	}
	std::cin.clear();

	return demand.size();
}

///		InputMustEdge����

///		����ؾ��ߵ���Ϣ
///     @param Mustedge				ָ��ȫ�ֱؾ��ߵ�����
///     @return									�ؾ��ߵĸ���
int InputMustEdge(std::vector<int>& Mustedge)
{
	std::string buf;
	std::string vertex;
	int first = 0;
	int v[2];
	int tmp;
	std::cout << "����ؾ��ߣ����磺����2 4 11 12����ʾ2��4Ϊһ���ؾ��ߣ�11��12Ϊ��һ���ؾ��ߣ���\n ";
	getline(std::cin, buf);
	std::istringstream rtc(buf); //�ļ�����
	while (rtc >> vertex)		   //���������Զ�ȡ����Ϣ���Կո�Ϊ���
	{
		tmp = atoi(vertex.c_str());
		v[first] = tmp;
		if (++first > 1)
		{
			if (v[0] == end || v[1] == end)
				Flag_EndVertex_Must = 1;
			int linkID = GetLinkID(v[0], v[1]);
			//		 std::cout<<"linkId:"<<linkID<<std::endl;
			Mustedge.push_back(linkID);
			v[0] = v[1] = 0;
			first = 0;
		}
	}
	std::cin.clear();
	return Mustedge.size();
}

///		InputBanEdge����

///		�����ֹ�ߡ����Ƶ����Ϣ
///     @param ban_file				ָ��ȫ�ֽ�ֹ�ߵ�����
///     @param num						ָ�����ƽڵ����ֵ��ָ��
///     @return								��ֹ�ߵĸ���
int InputBanEdge(std::vector<int>& ban_file, int* num)
{
	/*�����ֹ��*/
	std::string buf;
	std::string vertex;
	int first = 0;
	int v[2];
	int tmp;
	std::cout << "�����ֹ�ߣ����磺����2 4 11 12����ʾ2��4Ϊһ����ֹ�ߣ�11��12Ϊ��һ����ֹ�ߣ���\n ";
	getline(std::cin, buf);
	std::istringstream rtc(buf); //�ļ�����
	while (rtc >> vertex)		//���������Զ�ȡ����Ϣ���Կո�Ϊ���
	{
		tmp = atoi(vertex.c_str());
		v[first] = tmp;
		if (++first > 1)
		{
			int linkID = GetLinkID(v[0], v[1]);
			ban_file.push_back(linkID);
			v[0] = v[1] = 0;
			first = 0;
		}
	}
	std::cin.clear();

	/*�������ƽڵ����*/
	std::string numstring;
	std::string temp;
	std::cout << "�������ƽڵ���������ֵ�� \n ";
	getline(std::cin, numstring);
	std::istringstream record(numstring);
	record >> temp;
	*num = atoi(temp.c_str());
	std::cin.clear();

	return ban_file.size();
}

///		DestroyMap����

///		�ͷŴ洢ͼ�Ķ��ڴ�
///     @param pgraph					ָ��洢ͼ�Ľṹ��ָ��
void DestroyMap(Graph* pgraph)
{
	int i = 0;
	pArcNode parc = NULL;
	pArcNode preParc = NULL;
	for (; i < pgraph->vexnum; i++)
	{
		parc = pgraph->vertices[i].adjlist.head;
		while (parc != NULL)
		{
			preParc = parc;
			parc = parc->nextarc;
			delete preParc;
		}
	}
}

///		GetLinkID����

///		ȡ�������������ڵ��linkIDֵ
///     @param v1						�ڵ�1
///     @param v2						�ڵ�2
///     @return								���ڵ������ߵ�LinkIDֵ�����ߵı��
int GetLinkID(int v1, int v2)
{
	int i = 0;
	int LinkValue = -1;
	auto head = G.vertices[v1].adjlist.head;
	while (head != NULL)
	{
		if (head->adjvex == v2)
		{
			LinkValue = head->arcinfo.arcnum;
			//	std::cout<<"link:"<<LinkValue<<std::endl;
			break;
		}
		head = head->nextarc;
	}
	return LinkValue;
}

///		FloydWarshall����

///		���������㷨��Ѱ�Ҹ�����ͼ�ж�Դ��֮�����·��
///     @param mindist	t[][600]						mindist[][600] ��ʾ���� i �� j �����·��
///     @param pre[][600]							    pre[][600] ��¼ i �� j �����·���ĸ��ڵ�
void FloydWarshall(int mindist[][600], int pre[][600])
{

	int vnum = G.vexnum;
	/*ȥ��start��end�ڵ���صı���Ϣ������ΪInf��*/
	for (int i = 0; i < vnum; i++)
	{
		g_Distance_temp[start][i] = inf;
		g_Distance_temp[end][i] = inf;
		g_Distance_temp[i][start] = inf;
		g_Distance_temp[i][end] = inf;
	}
	int i, j, k;
	/*����ѭ����ʼ��*/
	for (i = 0; i < vnum; i++)
	{
		for (j = 0; j < vnum; j++)
		{
			mindist[i][j] = g_Distance_temp[i][j];
			pre[i][j] = (i == j) ? -1 : i;
		}
	}
	/*����ѭ��Ȩֵ����*/
	for (k = 0; k < vnum; k++)
	{
		for (i = 0; i < vnum; i++)
		{
			for (j = 0; j < vnum; j++)
			{
				if (mindist[i][k] + mindist[k][j] < mindist[i][j])
					mindist[i][j] = mindist[i][k] + mindist[k][j];
				pre[i][j] = pre[k][j];
			}
		}
	}
	/*��include set����ĵ㵽�Լ�����̾�������Ϊ0,����Ϊ�˼�����������ʱ�򷽱�*/
	for (i = 0; i < includeNodeNum; i++)
	{
		mindist[demandSet[i]][demandSet[i]] = 0;
	}
}

///		Dijkstra����

///		�Ͻ�˹�����㷨��Ѱ����������Ҫ���Ľڵ㵽�յ�����·��
///     @param n						ͼ�Ľڵ���
///     @param v						����ӵ�v��ʼ�ĵ�Դ���·��
///     @param dist					���ص����·��
///     @param prev				���ص�·����Ϣ
///     @param c						ͼ����Ϣ
void Dijkstra(int n, int v, int* dist, int* prev, int c[][600])
{
	bool s[600];    // �ж��Ƿ��Ѵ���õ㵽S������
	for (int i = 0; i < n; ++i)
	{
		dist[i] = c[v][i];
		s[i] = 0;     // ��ʼ��δ�ù��õ�
		if (dist[i] == INT_MAX)
			prev[i] = 0;
		else
			prev[i] = v;
	}
	dist[v] = 0;
	s[v] = 1;
	for (int i = 1; i < n; ++i)
	{
		int tmp = INT_MAX;
		int u = v;
		/* �ҳ���ǰδʹ�õĵ�j��dist[j]��Сֵ*/
		for (int j = 0; j < n; ++j)
		{
			if ((!s[j]) && dist[j] < tmp)
			{
				u = j;              // u���浱ǰ�ڽӵ��о�����С�ĵ�ĺ���
				tmp = dist[j];
			}
		}
		s[u] = 1;    // ��ʾu���Ѵ���S������
		/* ����dist*/
		for (int j = 0; j < n; ++j)
		{
			if ((!s[j]) && c[u][j] < INT_MAX)
			{
				int newdist = dist[u] + c[u][j];
				if (newdist < dist[j])
				{
					dist[j] = newdist;
					prev[j] = u;  //u��ǰ������
				}
			}
		}
	}
}

