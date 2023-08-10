#ifndef _SET_H_
#define _SET_H_

///		�ؾ��㼯�ϵĽṹ��

///		������ȫ�ֱؾ���ļ��ϣ������ڵ���Ŀ���ޡ����ڵ㡢���Һ�ɾ���Ȳ���
typedef struct _unionSet
{
	int maxn;															///< �ڵ���Ŀ����
	int* pa;																///< ��ʾx�ĸ��ڵ�										
	int (*FindSet)(int x, struct _unionSet*);					///< ����x�����ĸ����ϵĺ���ָ��	
	void (*UnionSet)(int x, int y, struct _unionSet*);		///< �ϲ�x,y���ڵļ��ϵĺ���ָ��	
	void (*clear)(struct _unionSet*);							///< ���_unionSet�ĺ���ָ��	
	void (*DestroySet)(struct _unionSet*);				///< ����_unionSet�ĺ���ָ��	
}*pUnionFindSet, UnionFindSet;

///		InitUnionSet����

///		��ʼ��UnionSet���ؾ��㼯�ϣ�
///     @param p						ָ��һ��_unionSet�ṹ���ָ��
///     @param n						������Ŀ�����ֵ
void InitUnionSet(pUnionFindSet p, int n);

#endif
