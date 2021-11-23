#pragma once
#include <iostream>

#define MAXSIZE_ID	8						//���� ����� �������� ��������������
#define SCOPED_ID_MAXSIZE MAXSIZE_ID*2		//���� ����� �������� ������������� + ������� ���������
#define MAXSIZE_TI	4096					//���� ����� ���������� ����� � ������� ���������������
#define LICH_DEFAULT 0x00000000				//�������� �� ��������� ��� valeur
#define STR_DEFAULT	0x00					//�������� �� ��������� ��� entier
#define NULLIDX_TI	0xffffffff				//��� �������� ������� ���������������
#define STR_MAXSIZE	255						//������������ ����� ���������� ��������
#define VAL_MAXSIZE 32767					//������������ �������� ��� ���� valeur
#define VAL_MINSIZE -32768					//����������� �������� ��� ���� valeur
#define MAX_PARAMS_COUNT 3					//������������ ���������� ���������� � �������
#define CONCAT_PARAMS_CNT 2					//���-�� ���������� � ������� concat
#define LENGHT_PARAMS_CNT 1					//���-�� ���������� � ������� lenght
#define ATOII_PARAMS_CNT 1					//���-�� ���������� � ������� atoii
#define CONCAT_TYPE IT::IDDATATYPE::STR
#define LENGHT_TYPE IT::IDDATATYPE::ENT
#define ATOII_TYPE IT::IDDATATYPE::ENT



namespace IT
{
	enum IDDATATYPE { ENT = 1, STR = 2, LECT = 3, UNDEF };//���� ������ ���������������: ��������, ���������, ��� ����(��� ��������), ��������������
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, S = 5 };	//���� ���������������: ����������, �������, ��������, �������, ����������� �������
	enum STDFNC { F_CONCAT, F_LENGHT, F_ATOII, F_NOT_STD };	//����������� �������
	static const IDDATATYPE CONCAT_PARAMS[] = { IT::IDDATATYPE::STR, IT::IDDATATYPE::STR };//��������� �������  concatstr
	static const IDDATATYPE LENGHT_PARAMS[] = { IT::IDDATATYPE::STR };//��������� ������� strlen
	static const IDDATATYPE ATOII_PARAMS[] = { IT::IDDATATYPE::STR };//��������� �-��� atoi
	struct Entry
	{
		union
		{
			int	vint;            			//�������� integer
			struct
			{
				int len;					//���������� ��������
				char str[STR_MAXSIZE - 1];  //�������
			} vstr;							//�������� ������
			struct
			{
				int count;					// ���������� ���������� �������
				IDDATATYPE* types;			//���� ���������� �������
			} params;
		} value;						    //�������� ��������������
		int			idxfirstLE;				//������ � ������� ������		
		char		id[SCOPED_ID_MAXSIZE];	//�������������
		IDDATATYPE	iddatatype;				//��� ������
		IDTYPE		idtype;					//��� ��������������

		Entry()							    //����������� ��� ����������
		{
			this->value.vint = LICH_DEFAULT;
			this->value.vstr.len = NULL;
			this->value.params.count = NULL;
		};
		Entry(char* id, int idxLT, IDDATATYPE datatype, IDTYPE idtype) //����������� � �����������
		{
			strncpy_s(this->id, id, SCOPED_ID_MAXSIZE - 1);
			this->idxfirstLE = idxLT;
			this->iddatatype = datatype;
			this->idtype = idtype;
		};
	};
	struct IdTable		//��������� ������� ���������������
	{
		int maxsize;	//������� ������� ��������������� < TI_MAXSIZE
		int size;		//������� ������ ������� ��������������� < maxsize
		Entry* table;	//������ ����� ������� ���������������
	};
	IdTable Create(int size = NULL);	//������� ������� ��������������� < TI_MAXSIZE
	void Add(					//�������� ������ � ������� ���������������
		IdTable& idtable,		//��������� ������� ���������������
		Entry entry);			//������ ������� ��������������� 
	int isId(					//�������: ����� ������(���� ����), TI_NULLIDX(���� ����)
		IdTable& idtable,		//��������� ������� ���������������
		char id[SCOPED_ID_MAXSIZE]);	//�������������
	bool SetValue(IT::Entry* entry, char* value);	//������ �������� ��������������
	bool SetValue(IT::IdTable& idtable, int index, char* value);
	void writeIdTable(std::ostream* stream, IT::IdTable& idtable); //������� ������� ���������������
};
