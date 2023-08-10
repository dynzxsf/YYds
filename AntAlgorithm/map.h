
#ifndef _MAP_H_
#define _MAP_H_
#include "set.h"
#include <vector>
#include <iostream>
#define  BITSPERWORD 32
#define  SHIFT 5
#define  MASK 0x1F
/// ���ڵ���
#define MAX_VERTEX_NUM 600  
/// Demands����������
#define  MAX_DEMAND_NUM 50 


///		�ߵ���Ϣ��

///		�����˱ߵ�Ȩ�غͱ����Ϣ
typedef struct
{
	unsigned char weight;	///< �ߵ�Ȩ��
	unsigned int arcnum;	///< �ߵı��
}ArcInfoType;

///		�ߵĶ���

///		�����˱ߵĶ���λ�á�ָ����һ���ߵ�ָ���Լ��ñߵ���Ϣ��
typedef struct ArcNode
{
	int adjvex;							///< �û�ָ��Ķ����λ��				
	struct ArcNode* nextarc;		///< ��һ������ָ��	
	ArcInfoType arcinfo;			///< �û��������Ϣ	
}*pArcNode, ArcNode;

///		ÿ���ڵ���ڽ�����Ķ���

///		���������������βָ�롢�����ĳ���
typedef struct
{
	ArcNode* head;///< ������ͷָ��
	ArcNode* tail;	///< ������βָ��
	int len;				///< �����ĳ���
}LinkList;

///		�ڵ����Ϣ��

///		�����˽ڵ����ȳ��ȡ�ͷ���ľ��롢���
typedef struct
{
	int distance;						///< ��Դ�ڵ�ľ���					
	unsigned char inDegree;		///< �ڵ�����	
	unsigned char outDegree;	///< �ڵ�ĳ���
	unsigned int num;				///< �ڵ�����
}NodeInfoType;

///		ͼ�Ľڵ�Ķ���

///		������ͼ�ڵ�������Ϣ���ڽ�����
typedef struct VNode
{

	NodeInfoType nodeinfo;	///< �ڵ������Ϣ��
	LinkList adjlist;			///< �ڵ���ڽ�����		
}VNode, AdjList[MAX_VERTEX_NUM], * pVNode;

///		ͼ�Ķ���

///		������ͼ�Ķ�����Ŀ������Ŀ���洢���ڽ���������
typedef struct
{
	int vexnum;	///< ͼ�Ķ�����Ŀ
	int arcnum;	///< ͼ�ıߵ���Ŀ
	AdjList vertices;///< �洢ͼ�ı�����
}Graph;

///		MakeNode����

///		���ݴ��ļ���ȡ�����ݹ���һ���ڽ�����Ľڵ㣨������һ���ߣ�
///     @param linkid				����ıߵı��
///		@param destinationid		����ıߵ�ָ��ڵ�
///		@param cost					����ıߵ�Ȩ��
///     @return							���ع���ıߵ�ָ��
ArcNode* MakeNode(unsigned int linkid, unsigned int destinationid, unsigned int cost);

///		AddNode����

///		�ڽڵ� i ���ڽ�����β�����һ����
///     @param i						�ڵ�
///		@param pArcNode		ָ��Ҫ��ӵıߵ�ָ��
void AddNode(unsigned int i, ArcNode* pArcNode);

///		InitMap����

///		�����ļ���ʼ��ͼ����Ϣ
///     @param topo[]				����ͼ��Ϣ������ָ��
void InitMap(char* topo[]);

///		InitDemandSet����

///		��ʼ���ؾ��������������
///     @param buf				�洢ȫ�ֱؾ���ļ���
///		@param bufsize			�ؾ���ĸ���
///		@param StartVertex	Ѱ·�����
///     @param EndVertex	Ѱ·���յ�
///     @return						�ؾ���ĸ���
int InitDemandSet(std::vector<int> buf, int bufsize, int StartVertex, int EndVertex);

///		InitMustEdge����

///		/��ʼ���ؾ���ļ���
///     @param buf				�洢ȫ�ֱؾ��ߵļ���
///		@param bufsize			�ؾ��ߵĸ���
///     @return						�ؾ��ߵĸ���
int InitMustEdge(std::vector<int> buf, int bufsize);

///		InitBanEdge����

///		��ʼ����ֹ�ߵļ��ϡ����ƽڵ�����ֵ
///     @param buf				�洢ȫ�ֽ�ֹ�ߵļ���
///		@param bufsize			��ֹ�ߵĸ���
///     @param num				���ƽڵ�����ֵ
///     @return						��ֹ�ߵĸ���
int InitBanEdge(std::vector<int> buf, int bufsize, int num);

///		InputDemand����

///		������β�ڵ㡢�ؾ������Ϣ
///     @param demand				ָ��ȫ�ֱؾ��������
///		@param VertexStart			ָ�������ŵ�ָ��
///     @param VertexEnd			ָ���յ���ŵ�ָ��
///     @return								�ؾ���ĸ���
int InputDemand(std::vector<int>& demand, int* VertexStart, int* VertexEnd);

///		InputMustEdge����

///		����ؾ��ߵ���Ϣ
///     @param Mustedge				ָ��ȫ�ֱؾ��ߵ�����
///     @return									�ؾ��ߵĸ���
int InputMustEdge(std::vector<int>& Mustedge);

///		InputBanEdge����

///		�����ֹ�ߡ����Ƶ����Ϣ
///     @param ban_file				ָ��ȫ�ֽ�ֹ�ߵ�����
///     @param num						ָ�����ƽڵ����ֵ��ָ��
///     @return								��ֹ�ߵĸ���
int InputBanEdge(std::vector<int>& ban_file, int* num);

///		GetLinkID����

///		ȡ�������������ڵ��linkIDֵ
///     @param v1						�ڵ�1
///     @param v2						�ڵ�2
///     @return								���ڵ������ߵ�LinkIDֵ�����ߵı��
int GetLinkID(int v1, int v2);

///		DestroyMap����

///		�ͷŴ洢ͼ�Ķ��ڴ�
///     @param pgraph					ָ��洢ͼ�Ľṹ��ָ��
void DestroyMap(Graph* pgraph);

///		FloydWarshall����

///		���������㷨��Ѱ�Ҹ�����ͼ�ж�Դ��֮�����·��
///     @param mindist	t[][600]						mindist[][600] ��ʾ���� i �� j �����·��
///     @param pre[][600]							pre[][600] ��¼ i �� j �����·���ĸ��ڵ�
void FloydWarshall(int mindist[][600], int pre[][600]);

///		Dijkstra����

///		�Ͻ�˹�����㷨��Ѱ����������Ҫ���Ľڵ㵽�յ�����·��
///     @param n						ͼ�Ľڵ���
///     @param v						����ӵ�v��ʼ�ĵ�Դ���·��
///     @param dist					���ص����·��
///     @param prev				���ص�·����Ϣ
///     @param c						ͼ����Ϣ
void Dijkstra(int n, int v, int* dist, int* prev, int c[][600]);

//topoͼ
extern Graph G;
//��ʾ���ϵ���ʼ������
extern int start;
//��ʾ����Ҫ�����յ�
extern int end;
//��ʾ����Ȩ�صĶ�ά����
extern int g_Distance[600][600];
//��ͼ��һ�����޸ĵ�ʱ�򣬻��õ�
extern int g_Distance_temp[600][600];
//��ʾ���ϵıؾ���
extern int demandSet[60];
//��ʾ���ϵıؾ��㼯��
extern UnionFindSet includingSet;

#endif // !_MAP_H_
