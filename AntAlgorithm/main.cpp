//////////////////////////////////////////////////////////////////////////  
///     COPYRIGHT NOTICE  
///     Copyright (c) 2017, ���Ͻ�ͨ��ѧ        
///     All rights reserved.  
///   
/// @author           ����������  
/// @date              2017��5��9��  
///  
//////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <time.h>
#include <vector>
#include "lib_io.h"
#include "map.h"
#include "ShortestPath.h"
/// ԭͼ���������ļ���Ϣ���������ͼ
Graph G;
/// Ҫ�󾭹��ĵ�ĸ���
int includeNodeNum;
/// Ҫ�󾭹��ıߵĸ���
int includeEdgeNum;
/// ��ֹ�����ıߵĸ���
int banEdgeNum;
/// topoͼ����
int edge_num;
/// ��űؾ���
std::vector<int> demand_file;
/// �����㡢�յ�ڵ�
int VertexStart, VertexEnd;
/// ��űؾ���
std::vector<int> mustedge_file;
/// ��Ž�ֹ��
std::vector<int> ban_file;
/// ������Ƶ����
int LimitNum;
/// �����������
int minDistance[600][600];
int pre[600][600];

int main(int argc, char* argv[])
{
	///���ͼ��Ϣ
	char* topo[5000];
	///ָ��ͼ�ļ�
	char* topo_file;
	///����ıؾ���ĸ���
	int demand_num;
	///����ıؾ��ߵĸ���
	int mustedge_num;
	///����Ľ�ֹ�ߵĸ���
	int ban_num;

	///��ʼ���������
	time_t tm;
	time(&tm);
	srand(time(NULL));

	///��ʼ��ͼ����Ϣ
	topo_file = "topo.csv";
	///��ȡtopoͼ�������رߵ���Ŀ
	edge_num = read_file(topo, 5000, topo_file);
	///�����ļ���ʼ��ͼG��������Ϣ
	InitMap(topo);

	///����ؾ������β��
	demand_num = InputDemand(demand_file, &VertexStart, &VertexEnd);
	///��ʼ���ع���Ҫ��
	includeNodeNum = InitDemandSet(demand_file, demand_num, VertexStart, VertexEnd);
	///����ؾ���
	mustedge_num = InputMustEdge(mustedge_file);
	///��ʼ���ع���Ҫ��
	includeEdgeNum = InitMustEdge(mustedge_file, mustedge_num);
	///�����ֹ��
	ban_num = InputBanEdge(ban_file, &LimitNum);
	///��ʼ����ֹ��Ҫ��
	banEdgeNum = InitBanEdge(ban_file, ban_num, LimitNum);

	/*���Գ�������ʱ��*/
	clock_t Time_start, Time_end;
	double Duration_Time;
	Time_start = clock();

	///��Ⱥ�㷨�ĺ��ĺ���
	FloydWarshall(minDistance, pre);
	CShortestPath csp;
	csp.InitData();
	csp.Search();

	///�ͷŶ��ڴ�
	release_buff(topo, edge_num);
	DestroyMap(&G);
	includingSet.DestroySet(&includingSet);

	/*ͳ�Ƴ�������ʱ��*/
	Time_end = clock();
	Duration_Time = ((double)(Time_end - Time_start)) / CLK_TCK;
	//	std::cout <<"the used time is: "<<Duration_Time<<std::endl;

	system("pause");
	return 0;
}

