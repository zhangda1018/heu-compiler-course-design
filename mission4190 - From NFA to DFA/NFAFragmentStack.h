#ifndef NFAFRAGMENTSTACK_H_
#define NFAFRAGMENTSTACK_H_

//
// �ڴ˴����� C ��׼��ͷ�ļ�
//

//
// �ڴ˴���������ͷ�ļ�
//

#include "NFAToDFA.h"

//
// �ڴ˴��������ݽṹ
//

#define MAX_STACK_LENGTH 1024 	// ջ����󳤶�

// ջ
typedef struct _NFAFragmentStack
{
	NFAFragment buffer[MAX_STACK_LENGTH];	// ջ�Ļ�����
	int top;		// ָʾջ����λ�ã�������ջ��Ԫ�صĸ���
}NFAFragmentStack;

//
// �ڴ˴���������
//

void InitNFAFragmentStack(NFAFragmentStack* pS);
void PushNFAFragment(NFAFragmentStack* pS, NFAFragment Elem);
NFAFragment PopNFAFragment(NFAFragmentStack* pS);
int NFAFragmentStackEmpty(NFAFragmentStack* pS);

//
// �ڴ˴�����ȫ�ֱ���
//

#endif /* NFAFRAGMENTSTACK_H_ */

