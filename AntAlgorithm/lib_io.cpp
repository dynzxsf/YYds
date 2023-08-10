#include <string.h>
#include <assert.h>
#include <iostream>
#include <time.h>
#include <sys/timeb.h>
#include <signal.h>
#include "lib_io.h"

#define MAX_LINE_LEN 4000

///		read_file����

///		��һ��csv�ļ���ȡtopoͼ������������뾲̬�ڴ�
///     @param buff			ָ��洢topoͼ������ָ�룬����һ����ά����洢ͼ��Ϣ
///		@param spec		��ʾ��ά����������������ͼ�ı���
///		@param filename  ָ��Ҫ��ȡ��csv�ļ�·��
///     @return					����ȡ�ɹ������ض�ά���������������ȡʧ�ܣ�����0
int read_file(char** const buff, const unsigned int spec, const char* const filename)
{
    FILE* fp = fopen(filename, "r");
    char line[MAX_LINE_LEN + 2];
    unsigned int cnt = 0;	//ͳ������
    if (fp == NULL)
    {
        std::cout << "Fail to open file " << filename << ", " << strerror(errno) << ".\n";
        return 0;
    }
    std::cout << "Open file " << filename << " is OK.\n";
    while ((cnt < spec) && !feof(fp))
    {
        line[0] = 0;
        fgets(line, MAX_LINE_LEN + 2, fp);
        if (line[0] == 0)   continue;
        buff[cnt] = new char[MAX_LINE_LEN + 2];
        strncpy(buff[cnt], line, MAX_LINE_LEN + 2 - 1);
        buff[cnt][4001] = 0;
        cnt++;
    }
    fclose(fp);
    std::cout << "There are " << cnt << " lines in file " << filename << " .\n\n";
    return cnt;
}

///		release_buff����

///		�ͷŶ�ȡtopoͼʱ�����Ķ�̬�ڴ�ռ�
///     @param buff							ָ��洢topoͼ������ָ��
///		@param valid_item_num		��ʾ�����ͷŵ������������ͼ�ı���
void release_buff(char** const buff, const int valid_item_num)
{
    int i = 0;
    for (; i < valid_item_num; i++)
        delete[] buff[i];
}