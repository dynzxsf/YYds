
#ifndef __LIB_IO_H__
#define __LIB_IO_H__

///		read_file����

///		��һ��csv�ļ���ȡtopoͼ������������뾲̬�ڴ�
///     @param buff			ָ��洢topoͼ������ָ�룬����һ����ά����洢ͼ��Ϣ
///		@param spec		��ʾ��ά����������������ͼ�ı���
///		@param filename  ָ��Ҫ��ȡ��csv�ļ�·��
///     @return					����ȡ�ɹ������ض�ά���������������ȡʧ�ܣ�����0
int read_file(char** const buff, const unsigned int spec, const char* const filename);


///		release_buff����

///		�ͷŶ�ȡtopoͼʱ�����Ķ�̬�ڴ�ռ�
///     @param buff							ָ��洢topoͼ������ָ��
///		@param valid_item_num		��ʾ�����ͷŵ������������ͼ�ı���
void release_buff(char** const buff, const int valid_item_num);

#endif

