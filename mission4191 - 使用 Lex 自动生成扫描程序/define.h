#ifndef _DEFINE_H_
#define _DEFINE_H_

typedef enum 
{
	// �ļ�����
	ENDFILE,
	
	// ����
	ERROR,
	
    // �ؼ���
    IF,			// if
    THEN,		// then
    ELSE,		// else
    END,		// end
    REPEAT,		// repeat
    UNTIL,		// until
    READ,		// read
    WRITE,		// write
    
    // ��ʶ��
    ID,
    
    // �޷�������
    NUM,
    
    // �������
    ASSIGN,		// :=
    EQ,			// =
    LT,			// <
    PLUS,		// +
    MINUS,		// -
    TIMES,		// *
    OVER,		// /
    LPAREN,		// (
    RPAREN,		// )
    SEMI,		// ;
    
    // ע��
    COMMENT		// {...}
    
}TokenType;

#endif // _DEFINE_H_
