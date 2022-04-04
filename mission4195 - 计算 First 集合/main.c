#include "First.h"
#include <string.h>
#include <stdlib.h>

const char* VoidSymbol = "$"; // "��"

char rule_table_ci[20][256];
char ruleNameArr[20][64];

int main(int argc, char* argv[])
{
	//
	// ���� InitRules ������ʼ���ķ�
	//
#ifdef CODECODE_CI
	Rule* pHead = InitRules_CI();  	// ���д�����������ˮ������
#else
	Rule* pHead = InitRules();		// ���д����� CP Lab ������
#endif
	
	//
	// ����ķ�
	//
	PrintRule(pHead);
	
	//
	// ���� First �������ķ��� First ����
	//
	SetList FirstSet;
	FirstSet.nSetCount = 0;
	First(pHead, &FirstSet);
	
	//
	// ���First����
	// 
	PrintFirstSet(&FirstSet);

	return 0;
}

/*
���ܣ�
	���һ�� Set �� SetList��

������
	pSetList -- SetList ָ�롣
	pName -- ���������ַ���ָ�롣 
*/
void AddOneSet(SetList* pSetList, const char* pName)
{
	// �����ǰҪ��ӵ� Set �Ѿ������� SetList �У���ֱ�ӷ���
	if (GetSet(pSetList, pName) != NULL)
	{
		return;
	}
	strcpy(pSetList->Sets[pSetList->nSetCount].Name, pName);
	pSetList->Sets[pSetList->nSetCount].nTerminalCount = 0;
	pSetList->nSetCount++;
}

/*
���ܣ�
	���������� SetList �в��� Set��

������
	pSetList -- SetList ָ�롣
	pName -- ���������ַ���ָ�롣
	  
����ֵ��
	����ҵ����� Set ��ָ�룬���򷵻� NULL��
*/
Set* GetSet(SetList* pSetList, const char* pName)
{
	Set *pSet = pSetList->Sets;
	int i;
	for (i = 0; i < pSetList->nSetCount; i++)
	{
		if (strcmp(pSet->Name, pName) == 0)
		{
			return pSet;
		}
		pSet++;
	}
	return NULL;
}

/*
���ܣ�
	���������� Set �в��� Terminal��

������
	pSet -- Set ָ�롣
	pName -- �ս�������ַ���ָ�롣
	  
����ֵ��
	����ҵ����� 1�����򷵻� 0��
*/
int GetTerminal(Set* pSet, const char* pName)
{
	int i;
	for (i = 0; i < pSet->nTerminalCount; i++)
	{
		if (strcmp(pSet->Terminal[i], pName) == 0)
		{
			return 1;
		}
	}
	return 0;
}

/*
���ܣ�
	���һ���ս���� Set��

������
	pSet -- Set ָ�롣
	pTerminal -- �ս������ָ�롣
	  
����ֵ��
	��ӳɹ����� 1��
	��Ӳ��ɹ����� 0��
*/
int AddTerminalToSet(Set* pSet, const char* pTerminal)
{
	// �жϵ�ǰ Set �Ƿ��������������ֱ�ӷ��� 0
	if (pSet->nTerminalCount == 32)
	{
		return 0;
	}
	// �����ǰ Set ��û��Ҫ��ӵ� Terminal������� Terminal ��ӵ���ǰ Set ��
	if (GetTerminal(pSet, pTerminal) == 0)
	{
		strcpy(pSet->Terminal[pSet->nTerminalCount], pTerminal);
		pSet->nTerminalCount++;
		return 1;
	}
	// ���δ����ӣ����� 0
	return 0;
}

/*
���ܣ�
	��Դ Set �е������ս����ӵ�Ŀ�� Set �С�

������
	pDesSet -- Ŀ�� Set ָ�롣
	pSrcSet -- Դ Set ָ�롣
	  
����ֵ��
	��ӳɹ����� 1�����򷵻� 0��
*/
int AddSetToSet(Set* pDesSet, const Set* pSrcSet)
{
	// ���Դ Set ��Ŀ�� Set ָ����ͬ������һ�����޷����У�ֱ�ӷ��� 0
	if (pDesSet == pSrcSet)
	{
		return 0;
	}
	int i;
	for (i = 0; i < pSrcSet->nTerminalCount; i++)
	{
		// ���Բ��뵱ǰ�ս�����������ʧ�ܣ��򷵻� 0
		if (AddTerminalToSet(pDesSet, pSrcSet->Terminal[i]) == 0)
		{
			return 0;
		}
	}
	return 1;
}

/*
���ܣ�
	�ж� Set ���ս�����Ƿ��Цš�

������
	pSet -- Set ָ�롣
	  
����ֵ��
	���ڷ��� 1��
	�����ڷ��� 0��
*/
int SetHasVoid(const Set* pSet)
{
	int i;
	for (i = 0; i < pSet->nTerminalCount; i++)
	{
		// ���� 0 ��ʾ��ǰ���ս��Ϊ��
		if (strcmp(pSet->Terminal[i], VoidSymbol) == 0)
		{
			return 1;
		}
	}
	return 0;			
}

/*
���ܣ�
	���ķ��� First ���ϡ�

������
	pHead -- �ķ���ͷָ�롣
	pFirstSetList -- First ����ָ�롣
*/
void First(const Rule* pHead, SetList* pFirstSetList)
{
	const Rule* pRule;  // Rule ָ��
	int isChange;	    // �����Ƿ��޸ĵı�־
	RuleSymbol* pSymbol;// Symbol �α�

	// ʹ���ķ������ʼ�� First ����
	for(pRule = pHead; pRule != NULL; pRule = pRule->pNextRule)
	{
		AddOneSet(pFirstSetList, pRule->RuleName);
	}

	do
	{
		isChange = 0; // �����޸ı�־

		for(pRule = pHead; pRule != NULL; pRule = pRule->pNextRule)
		{
			// �����ķ������� pFirstSetList �в��� Set
			Set* pDesSet = GetSet(pFirstSetList, pRule->RuleName);

			int hasVoid = 1; // First �������Ƿ��Цŵı�־
			for(pSymbol = pRule->pFirstSymbol; pSymbol != NULL && hasVoid; pSymbol = pSymbol->pNextSymbol)
			{
				if(pSymbol->isToken)	// �ս��
				{
					// ���� AddTerminalToSet �������ս����ӵ� pDesSet���������޸ı�־
					if(AddTerminalToSet(pDesSet, pSymbol->SymbolName))
						isChange = 1;

					hasVoid = 0; // ���� First �������Ƿ��Цŵı�־
				}
				else	// ���ս��
				{
					// ���ݷ��ս�������� pFirstSetList �в��� Set
					Set* pSrcSet = GetSet(pFirstSetList, pSymbol->SymbolName);

					// ���� AddSetToSet ��������Դ Set �е������ս����ӵ�Ŀ�� Set �У��������޸ı�־
					if(AddSetToSet(pDesSet, pSrcSet))
						isChange = 1;

					// ���� SetHasVoid �������ж�Դ Set ���Ƿ��Ц�
					hasVoid = SetHasVoid(pSrcSet);
				}
			}
			if(hasVoid)
			{
				// ���� AddTerminalToSet �������ż��뵽Ŀ�꼯����
				AddTerminalToSet(pDesSet, VoidSymbol);
			}
		}
	} while(isChange);
	
}

typedef struct _SYMBOL
{
	int isToken;
	char SymbolName[MAX_STR_LENGTH];
}SYMBOL;

typedef struct _RULE_ENTRY
{
	char RuleName[MAX_STR_LENGTH];
	SYMBOL Symbols[64];
}RULE_ENTRY;

static const RULE_ENTRY rule_table[] =
{
	/* exp -> exp addop term| term */
	{ "exp", { { 0, "exp" }, { 0, "addop"}, { 0, "term"} } },
	{ "exp", { { 0, "term" } } },

	/* addop -> + | - */
	{ "addop", { { 1, "+" } } },
	{ "addop", { { 1, "-" } } },

	/* term -> term mulop factor | factor */
	{ "term", { { 0, "term" }, { 0, "mulop"}, { 0, "factor"} } },
	{ "term", { { 0, "factor" } } },

	/* mulop -> * */
	{ "mulop", { { 1, "*" } } },

	/* factor -> (exp) | number */
	{ "factor", { { 1, "(" }, { 0, "exp"}, { 1, ")"} } },
	{ "factor", { { 1, "number" } } },
};

/*
���ܣ�
	��ʼ���ķ�����
	
����ֵ��
	�ķ���ͷָ�롣
*/
Rule* InitRules()
{
	Rule *pHead, *pRule;
	RuleSymbol **pSymbolPtr, *pNewSymbol;
	// ���� rule_table ��ռ�ÿռ��һ�� rule ��ռ�ÿռ䣬���� rule ������
	int nRuleCount = sizeof(rule_table) / sizeof(rule_table[0]);
	int i, j;

	// pRulePtr �洢��ǰ��Ҫ������ rule ��ָ��ĵ�ַ
	Rule** pRulePtr = &pHead;
	// ����ÿһ�� rule
	for (i = 0; i < nRuleCount; i++)
	{
		// �������ƴ����µ� rule
		*pRulePtr = CreateRule(rule_table[i].RuleName);
		// ��ָ��ǰ rule ����һ�� rule ��ָ��ĵ�ַ�洢�� pRulePtr �У��Ա�������һ�� rule
		pRulePtr = &(*pRulePtr)->pNextRule;
	}

	// �������е� rule
	pRule = pHead;
	for (i = 0; i < nRuleCount; i++)
	{
		pSymbolPtr = &pRule->pFirstSymbol;
		// ������ǰ rule �ж���õ����� Symbol
		for (j = 0; rule_table[i].Symbols[j].SymbolName[0] != '\0'; j++)
		{
			// �洢Ԥ�ȶ���� symbol ��Ϊ�������� symbol �ڵ�Ĳο�
			const SYMBOL* pSymbol = &rule_table[i].Symbols[j];

			pNewSymbol = CreateSymbol();
			pNewSymbol->isToken = pSymbol->isToken;
			strcpy(pNewSymbol->SymbolName, pSymbol->SymbolName);
			// �� pSymbolPtr ��ָ���ĵ�ַ�����µ� symbol �ڵ�
			*pSymbolPtr = pNewSymbol;
			
			// �� pSymbolPtr ��ֵ����Ϊ��һ�� symbol ָ��Ĵ洢��ַ
			pSymbolPtr = &pNewSymbol->pNextSymbol;
		}

		pRule = pRule->pNextRule;
	}

	return pHead;
}

/*
���ܣ�
	��ʼ���ķ�����(��ִ����ˮ��ʱ����)��
	
����ֵ��
	�ķ���ͷָ�롣
*/
Rule* InitRules_CI()
{
	int nRuleCount = 0;
	// ��Ҫ��ȡ�����ı�
	for (int i = 0; i < 20; i++)
	{
		gets(rule_table_ci[i]);	
		int length = strlen(rule_table_ci[i]);
		if (length == 0)
		{
			break;
		}
		
		for (int j = 0; j < length; j++)
		{
			if (rule_table_ci[i][j] == ' ')
			{
				ruleNameArr[i][j] = '\0';
				break;
			}
			ruleNameArr[i][j]= rule_table_ci[i][j];
		}	  
		nRuleCount++;
	}
	
	Rule *pHead, *pRule;
	RuleSymbol **pSymbolPtr, *pNewSymbol;

	Rule** pRulePtr = &pHead;
	for (int i=0; i<nRuleCount; i++)
	{
		*pRulePtr = CreateRule(ruleNameArr[i]);
		pRulePtr = &(*pRulePtr)->pNextRule;
	}

	pRule = pHead;
	for (int i=0; i<nRuleCount; i++)
	{
		pSymbolPtr = &pRule->pFirstSymbol;
		
		int start = 0;
		for (int j=0; rule_table_ci[i][j] != '\0'; j++)
		{
			if (rule_table_ci[i][j] == ' '
			 && rule_table_ci[i][j + 1] == '-'
			&& rule_table_ci[i][j + 2] == '>' 
			&& rule_table_ci[i][j + 3] == ' ')
			{
				start = j + 4;
				break;
			}
		}
			
		for (int k=start; rule_table_ci[i][k] != '\0'; k++)
		{
			if (rule_table_ci[i][k] == ' ')
			{
				continue;
			}
				
			pNewSymbol = CreateSymbol();
			char tokenName[MAX_STR_LENGTH] = {};
			
			for (int m = 0; ;m++)
			{
				if (rule_table_ci[i][k] ==  ' ' || rule_table_ci[i][k] == '\0' || rule_table_ci[i][k] == '\n')
				{
					tokenName[m] = '\0';
					break;
				}
				tokenName[m] = rule_table_ci[i][k++];
				
			}
			
			
			strcpy(pNewSymbol->SymbolName, tokenName);
			
			pNewSymbol->isToken = 1;
			for (int n = 0; n < nRuleCount; n++)
			{
				if (strcmp(pNewSymbol->SymbolName, ruleNameArr[n]) == 0)
				{
					pNewSymbol->isToken = 0;
					break;
				}
			}		
			
			*pSymbolPtr = pNewSymbol;

			pSymbolPtr = &pNewSymbol->pNextSymbol;
			
		}
			
		pRule = pRule->pNextRule;
	}

	return pHead;
}

/*
���ܣ�
	����һ���µ��ķ���
	
������
	pRuleName -- �ķ������֡�	
	
����ֵ��
	�ķ���ָ�롣
*/
Rule* CreateRule(const char* pRuleName)
{
	Rule* pRule = (Rule*)malloc(sizeof(Rule));

	strcpy(pRule->RuleName, pRuleName);
	pRule->pFirstSymbol = NULL;
	pRule->pNextRule = NULL;

	return pRule;
}

/*
���ܣ�
	����һ���µķ��š�
	
����ֵ��
	���ŵ�ָ�롣
*/
RuleSymbol* CreateSymbol()
{
	RuleSymbol* pSymbol = (RuleSymbol*)malloc(sizeof(RuleSymbol));

	pSymbol->pNextSymbol = NULL;
	pSymbol->isToken = -1;
	pSymbol->SymbolName[0] = '\0';

	return pSymbol;
}

/*
���ܣ�
	����ķ���
	
������
	pHead -- �ķ���ͷָ�롣
*/
void PrintRule(Rule* pHead)
{
	Rule *pRule = pHead;
	// �������е� Rule
	while (pRule != NULL)
	{
		// ��� Rule ������
		printf("%s ->", pRule->RuleName);
		RuleSymbol *pSymbol = pRule->pFirstSymbol;
		// ������ǰ Rule �����е� Symbol
		while (pSymbol != NULL)
		{
			printf(" %s", pSymbol->SymbolName);
			pSymbol = pSymbol->pNextSymbol;
		}
		puts("");
		pRule = pRule->pNextRule;
	}
}

/*
���ܣ�
	��� First ���ϡ�

������
	pFirstSetList -- First ����ָ�롣
*/
void PrintFirstSet(SetList* pFirstSetList)
{
	printf("\nThe First Set:\n");
	for (int i = 0; i < pFirstSetList->nSetCount; i++)
	{
		printf("First(%s) = { ", pFirstSetList->Sets[i].Name);
		for (int j = 0; j < pFirstSetList->Sets[i].nTerminalCount; j++)
		{
			if (j == pFirstSetList->Sets[i].nTerminalCount - 1)
			{
				printf("%s ", pFirstSetList->Sets[i].Terminal[j]);
			}
			else
			{
				printf("%s, ", pFirstSetList->Sets[i].Terminal[j]);
			}
			
		}
		printf("}\n");
	}
}

