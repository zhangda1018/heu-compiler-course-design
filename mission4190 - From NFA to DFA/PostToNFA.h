#ifndef POSTTONFA_H_
#define POSTTONFA_H_

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

// �洢���е� NFA ״ָ̬��
static NFAState* NFAStateList[5005];
// �洢 NFA ״̬����
static int cnt = 0;

//
// �ڴ˴���������
//

NFAState* CreateNFAState();
NFAFragment MakeNFAFragment(NFAState *StartState, NFAState *AcceptState);
NFAState* post2nfa(char *postfix);

//
//  �ڴ�����ȫ�ֱ���
//

#endif /* POSTTONFA_H_ */
