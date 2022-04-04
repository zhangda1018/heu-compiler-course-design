#ifndef NFASTATESTACK_H_
#define NFASTATESTACK_H_

//
// �ڴ˴����� C ��׼��ͷ�ļ�
//

//
// �ڴ˴���������ͷ�ļ�
//

//
// �ڴ˴��������ݽṹ
//

#define MAX_STACK_LENGTH 1024 	// ջ����󳤶�

// ջ
typedef struct _NFAStateStack
{
	NFAState* buffer[MAX_STACK_LENGTH];	// ջ�Ļ�����
	int top;		// ָʾջ����λ�ã�������ջ��Ԫ�صĸ���
}NFAStateStack;

//
// �ڴ˴���������
//

void InitNFAStateStack(NFAStateStack* pS);
void PushNFAState(NFAStateStack* pS, NFAState* Elem);
NFAState* PopNFAState(NFAStateStack* pS);
int NFAStateStackEmpty(NFAStateStack* pS);

//
// �ڴ˴�����ȫ�ֱ���
//

#endif /* NFASTATESTACK_H_ */

