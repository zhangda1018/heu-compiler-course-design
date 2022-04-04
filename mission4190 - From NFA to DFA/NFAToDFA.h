#ifndef NFATODFA_H_
#define NFATODFA_H_

//
// �ڴ˴����� C ��׼��ͷ�ļ�
//

#include <stdio.h>

//
// �ڴ˴���������ͷ�ļ�
//

//
// �ڴ˴��������ݽṹ
//

#define MAX_STATE_NUM 64	// ״̬�����������

typedef struct _NFAState
{
	char Transform;       	 // ״̬��װ���ı�ʶ���� '$' ��ʾ '��-ת��'��
	struct _NFAState *Next1; // ָ����һ��״̬��
	struct _NFAState *Next2; // ָ����һ��״̬��
	int Name;			     // ״̬���ơ�
	int AcceptFlag;		     // �Ƿ�Ϊ����״̬�ı�־��1 ��ʾ�ǽ���״̬ 0 ��ʾ�ǽ���״̬�� 
}NFAState;

// Fragment �ṹ��һ�� NFA ��Ƭ��
typedef struct _NFAFragment
{
	NFAState *StartState; 	// ��ʼ״̬��
	NFAState *AcceptState; 	// ����״̬��
}NFAFragment;

// ת��
typedef struct _Transform
{
	char TransformChar;				 // ״̬֮���ת����ʶ����
	int DFAStateIndex;				 // DFA ״̬�������е��±ꡣ
	NFAState* NFAlist[MAX_STATE_NUM];// NFA ״̬���ϡ�
	int NFAStateCount; 				 // ״̬���ϼ�����
	struct _Transform* NextTrans;	 // ָ����һ��ת����
}Transform;

// DFA ״̬
typedef struct _DFAState
{
	NFAState* NFAlist[MAX_STATE_NUM];// NFA ״̬���ϡ�
	int NFAStateCount;				 // ״̬���ϼ�����
	Transform* firstTran;			 // ָ���һ��ת����
}DFAState;

// DFA
typedef struct DFA
{
	DFAState* DFAlist[MAX_STATE_NUM]; // DFA ״̬���ϡ�
	int length;						  // ״̬���ϼ�����
}DFA;

//
// �ڴ˴���������
//

void Closure(NFAState* State, NFAState** StateArray, int* Count);
DFAState* CreateDFAState(Transform* pTransform);
Transform* CreateDFATransform(char TransformChar, NFAState** NFAStateArray, int Count);
int NFAStateIsSubset(DFA* pDFA, Transform* pTransform);
Transform* IsTransformExist(DFAState* pDFAState, char TransformChar);
void AddNFAStateArrayToTransform(NFAState** NFAStateArray, int Count, Transform* pTransform);
DFA* post2dfa(DFA* pDFA, char *postfix);

//
// �ڴ�����ȫ�ֱ���
//

extern const char VoidTrans;
extern NFAState* Start;

#endif /* NFATODFA_H_ */


