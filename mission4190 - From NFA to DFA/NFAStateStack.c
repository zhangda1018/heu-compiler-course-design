#include "NFAToDFA.h"
#include "NFAStateStack.h"


/*
���ܣ�
	��ʼ��ջ��

������
	pS -- ջ��ָ��
*/
void InitNFAStateStack(NFAStateStack* pS)
{
	pS->top = -1;
}

/*
���ܣ�
	��Ԫ����ջ��

������
	pS -- ջ��ָ��
	Elem -- ��ջ��Ԫ��
  
����ֵ��
	��
*/
void PushNFAState(NFAStateStack* pS, NFAState* Elem)
{
	//
	// ջ������ջʧ�ܡ�
	//
	if (MAX_STACK_LENGTH - 1 <= pS->top)
	{
		return;
	}
		
	pS->top++;
	pS->buffer[pS->top] = Elem; // ��Ԫ�ز���ջ��
	
	return;
}

/*
���ܣ�
	��ջ��Ԫ�س�ջ

������
	pS -- ջ��ָ��
  
����ֵ��
	�����ջ�ɹ����س�ջԪ�ص�ֵ��
	�����ջʧ�ܷ��� -1��
*/
NFAState* PopNFAState(NFAStateStack* pS)
{
	int pos;
	NFAState* State = NULL;
	
	//
	// ջΪ�գ���ջʧ��
	//	
	if (NFAStateStackEmpty(pS))
	{
		return State;
	}
	
	pos = pS->top;
	pS->top--;
	
	return pS->buffer[pos];
}

/*
���ܣ�
	�ж�ջ�Ƿ�Ϊ�ա�

������
	pQ -- ջ��ָ��
  
����ֵ��
	���ջ�շ��� 1���棩
	���ջ�ǿշ��� 0���٣�
*/
int NFAStateStackEmpty(NFAStateStack* pS)
{
	return -1 == pS->top ? 1 : 0;
}

