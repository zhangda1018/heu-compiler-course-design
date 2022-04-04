#ifndef REGEXPTONFA_H_
#define REGEXPTONFA_H_

//
// �ڴ˴����� C ��׼��ͷ�ļ�
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

//
// �ڴ˴���������ͷ�ļ�
//

//
// �ڴ˴��������ݽṹ
//

typedef struct _NFAState
{
	char Transform;       	 // ״̬��ת���ı�ʶ���� '$' ��ʾ '��-ת��'
	struct _NFAState *Next1; // ָ����һ��״̬ 
	struct _NFAState *Next2; // ָ����һ��״̬
	int Name;			     // ״̬����
	int AcceptFlag;		     // �Ƿ�Ϊ����״̬�ı�־��1 ��ʾ�ǽ���״̬ 0 ��ʾ���ǽ���״̬ 
}NFAState;

// Fragment �ṹ��һ�� NFA ��Ƭ��
typedef struct _NFAFragment
{
	NFAState *StartState; 	// ��ʼ״̬
	NFAState *AcceptState; 	// ����״̬
}NFAFragment;

//
// �ڴ˴���������
//

NFAState* CreateNFAState();
NFAState* post2nfa(char *postfix);
NFAFragment MakeNFAFragment(NFAState *StartState, NFAState *AcceptState);

//
// �ڴ�����ȫ�ֱ���
//

extern int nstate;
extern const char VoidTrans; 
extern char* regexp;

#endif /* REGEXPTONFA_H_ */
