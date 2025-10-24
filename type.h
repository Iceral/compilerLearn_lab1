// src/parser/type.h ����ʱ�����ã�
#ifndef TYPE_H
#define TYPE_H

// ��ʱ���� Type Ϊһ���򵥽ṹ�������ڱ���ͨ���ͻ������ԣ�
typedef struct Type_ {
    int kind;  // 0=int, 1=float, 2=array, 3=struct...
    // �����ֶ���ʱ����Ҫ������ʱֻ���ġ��ܴ����͡�
} Type;

// �ṩһ�����㴴�� Type �ĺ�������ѡ��
Type make_type(int kind);

#endif