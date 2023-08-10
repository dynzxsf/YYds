#pragma once
#include "ant.h"

/// ������CShortestPath		
/// 
/// �๦�ܣ����������ڹ涨Լ���½�������·��Ѱ�ң�������ʼ����������Ϣ�ء�Ѱ·������ 
class CShortestPath
{
public:
	CShortestPath() {};
	~CShortestPath() {};
public:
	/// ��������
	CAnt	 m_cAntAry[N_ANT_COUNT];
	/// ȫ����������
	CAnt	 m_cGlobalBestAnt;
	/// ���ε�����������
	CAnt	 m_cIterationBestAnt;
	/// ��ʱ����
	CAnt	 m_cTempAnt;
	/// ��Ϣ�����ֵ
	double	Qmax;
	/// ��Ϣ����Сֵ
	double	Qmin;
	/// ���ֵ����Сֵ�ı���
	double	m_dbRate;
	///		InitData����

	///		��ʼ�����ϵ���Ϣ�طֲ�
	///     @note		  ��ʼ��topoͼ�����������ߵ���Ϣ�ش�СΪ10000
	void InitData();

	///		Search����

	///		Ѱ��ȫ������������ȡ������·����ͬʱ��Լ��ģ���жϴ˴�Ѱ·�Ƿ��н�
	///     @note		  �������������������н��򷵻����Ž⣻���޽������Լ��ģ���ٴν���Ѱ·ֱ�������������һ�����Ž�
	void Search();

	///		UpdateTrial����

	///		����·������Ϣ�طֲ�
	///     @param flag  ��ʾ�Ƿ�ʹ��ȫ�����Ž⣬1��ʾʹ��ȫ�����Ž⣬0��ʾʹ�õ������Ž� 
	///     @note   ʹ��ȫ�����ź͵������Ž�����µĲ��ԣ�ÿ5�ε�������һ����Ϣ��
	void UpdateTrial(int flag);

	///		printPath����

	///		������ŵ�·��
	///     @note      ��ȫ�������������ߵ�·�������õĻ��ѣ����д�ӡ��� 
	void printPath();
};

