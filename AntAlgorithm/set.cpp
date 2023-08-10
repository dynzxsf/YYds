#include "set.h"
#include "string.h"

///		_findSet����

///		����x�ĸ�ĸ
///     @param x						�ڵ���
///     @param pset				ָ��һ��_unionSet�ṹ���ָ��
int _findSet(int x, pUnionFindSet pset)
{
	return (pset->pa)[x];
}

///		_unionset����

///		��x�ĸ�ĸ����Ϊy
///     @param x						�ڵ���1
///     @param y						�ڵ���2
///     @param pset				ָ��һ��_unionSet�ṹ���ָ��
void _unionset(int x, int y, pUnionFindSet pset)
{
	pset->pa[x] = y;
}

///		_destroySet����

///		�ͷ��ڴ�
///     @param pset				ָ��һ��_unionSet�ṹ���ָ��
void _destroySet(pUnionFindSet pset)
{
	delete[](pset->pa);
}

///		_clear����

///		�������Ϊ -1
///     @param p						ָ��һ��_unionSet�ṹ���ָ��
void _clear(pUnionFindSet pset)
{
	memset(pset->pa, 0xff, (pset->maxn) * sizeof(int));
}

///		InitUnionSet����

///		��ʼ��UnionSet���ؾ��㼯�ϣ�
///     @param p						ָ��һ��_unionSet�ṹ���ָ��
///     @param n						������Ŀ�����ֵ
void InitUnionSet(pUnionFindSet pset, int n)
{
	pset->pa = new int[n];
	pset->maxn = n;
	memset(pset->pa, 0xff, n * sizeof(int));
	pset->FindSet = _findSet;
	pset->UnionSet = _unionset;
	pset->DestroySet = _destroySet;
	pset->clear = _clear;
}