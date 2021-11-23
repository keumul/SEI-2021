#include "Header.h"
#include <iomanip>
#define W(x, y)\
		<< std::setw(x) << (y) <<
#define STR(n, line, type, id)\
		"|" W(4,n) " |  " W(5,line) "    |" W(17,type) " |  " W(SCOPED_ID_MAXSIZE, id) " |"


namespace IT
{
	IdTable Create(int size)
	{
		if (size > MAXSIZE_TI)
			throw ERROR_THROW(203);
		IdTable idtable;
		idtable.table = new Entry[idtable.maxsize = size];
		idtable.size = NULL;
		return idtable;
	}

	void Add(IdTable& idtable, Entry entry)
	{
		if (idtable.size >= idtable.maxsize)
			throw ERROR_THROW(203);
		idtable.table[idtable.size++] = entry;
	}

	// �������: ����� ������(���� ����), TI_NULLIDX(���� ���)
	int isId(IdTable& idtable, char id[SCOPED_ID_MAXSIZE])
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (strcmp(idtable.table[i].id, id) == 0)
				return i;
		}
		return NULLIDX_TI;
	}

	bool SetValue(IT::IdTable& idtable, int index, char* value)
	{
		return SetValue(&(idtable.table[index]), value);
	}

	bool SetValue(IT::Entry* entry, char* value) // ��������� �������� ����������
	{
		bool rc = true;
		if (entry->iddatatype == ENT)
		{
			int temp = atoi(value);
			if (temp > VAL_MAXSIZE || temp < VAL_MINSIZE)
			{
				if (temp > VAL_MAXSIZE)
					temp = VAL_MAXSIZE;
				if (temp < VAL_MINSIZE)
					temp = VAL_MINSIZE;
				rc = false;
			}
			entry->value.vint = temp;
		}
		else
		{
			for (unsigned i = 1; i < strlen(value) - 1; i++)	// ��� �������
				entry->value.vstr.str[i - 1] = value[i];
			entry->value.vstr.str[strlen(value) - 2] = '\0';
			entry->value.vstr.len = strlen(value) - 2;
		}
		return rc;
	}
	void writeIdTable(std::ostream* stream, IT::IdTable& idtable)
	{
		*stream << "\n\n[ ������� ��������������� ]\n" << std::endl;
		*stream << "|  N  |������ � ��| ��� ��������������  |        ���        | �������� (���������)" << std::endl;
		*stream << "_______________________________________________________________________________________" << std::endl;
		for (int i = 0; i < idtable.size; i++)
		{
			IT::Entry* e = &idtable.table[i];
			char type[50] = "";

			switch (e->iddatatype)
			{
			case IT::IDDATATYPE::ENT:
				strcat(type, "   entier");
				break;
			case IT::IDDATATYPE::STR:
				strcat(type, "   valeur");
				break;
			case IT::IDDATATYPE::LECT:
				strcat(type, "   lect  ");
				break;
			case IT::IDDATATYPE::UNDEF:
				strcat(type, "UNDEFINED");
				break;
			}
			switch (e->idtype)
			{
			case IT::IDTYPE::V:
				strcat(type, "  variable ");
				break;
			case IT::IDTYPE::F:
				strcat(type, "  function ");
				break;
			case IT::IDTYPE::P:
				strcat(type, "  parameter");
				break;
			case IT::IDTYPE::L:
				strcat(type, "   literal ");
				break;
			case IT::IDTYPE::S: strcat(type, "  LIB FUNC "); break;
			default:
				strcat(type, "UNDEFINED ");
				break;
			}

			*stream << STR(i, e->idxfirstLE, type, e->id);
			if (e->idtype == IT::IDTYPE::L || e->idtype == IT::IDTYPE::V && e->iddatatype != IT::IDDATATYPE::UNDEF)
			{
				if (e->iddatatype == IT::IDDATATYPE::ENT)
					*stream << e->value.vint;
				else
					*stream << "[" << (int)e->value.vstr.len << "]" << e->value.vstr.str;
			}
			if (e->idtype == IT::IDTYPE::F || e->idtype == IT::IDTYPE::S)
			{
				for (int i = 0; i < e->value.params.count; i++)
				{
					*stream << " P" << i << ":";
					switch (e->value.params.types[i])
					{
					case IT::IDDATATYPE::ENT:
						*stream << "ENTIER |";
						break;
					case IT::IDDATATYPE::STR:
						*stream << "VALEUR |";
						break;
					case IT::IDDATATYPE::LECT:
					case IT::IDDATATYPE::UNDEF:
						*stream << "UNDEFINED";
						break;
					}
				}
			}
			*stream << std::endl;
		}
	}
};