#include "scaner.h"

TScaner::TScaner() : uk (0), ukLine (1), ukInLine (1)
{
	
	ifstream f;
	f.open("input.txt");
	if (!f.is_open())
	{
		cout << "�� ������� ������� ���� \"input.txt\"" << endl;
		//cout << "������� �����-������ ������ ��� ���������� ���������" << endl;
		//_getche();
		exit(1);
		return;
	}

	int i = 0;

	while (!f.eof())
	{
		f.get(t[i++]);
	}
	f.close();
	t[i - 1] = '\0'; // ����������� ���� �\0� � ����� ������
	KW["return"] = RETURN;
	KW["main"] = MAIN;
	KW["while"] = WHILE;
	KW["int"] = INT;
	KW["short"] = SHORT;
	KW["float"] = FLOAT;
	ErrorMessages[ERROR_OPEN_COMMENT_IN_THE_END] = "�������� ����������� � ����� ����";
	ErrorMessages[ERROR_UNKNOWN_LEKSEMA] = "����������� �������";
	ErrorMessages[ERROR_OPEN_CHAR_CONST] = "���������� ���������� �������";
	ErrorMessages[ERROR_WRONG_CHAR_CONST] = "�������� ���������� �������";
	ErrorMessages[ERROR_LEXEMA_IS_TOO_LONG] = "������� ������� �������";
};

void TScaner::SetUK(int newUK, int newUKLine, int newUKInLine)
{
	uk = newUK;
	ukLine = newUKLine;
	ukInLine = newUKInLine;
}
int TScaner::GetUK() const
{
	return uk;
}
int TScaner::GetUKLine() const
{
	return ukLine;
}
int TScaner::GetUKInLine() const
{
	return ukInLine;
}

void TScaner::PrintError(int er_type, int i, LEX l)  const// ������ ��������� �� ������
{
	cout << "������: ";

	if (er_type != ERROR_OPEN_COMMENT_IN_THE_END)
	{
		cout << "\"";
		for (int dop_i = 0; dop_i < i; dop_i++)
			cout << l[dop_i];
		cout << "\" - ";
	}
	cout << " ����� ������: " << er_type << endl;
};

void TScaner::PrintError(int oldukLine, int oldUkInLine, int er_type, LEXs l) // ������ ��������� �� ������
{
	cout << "������: " << endl;
	if (er_type != ERROR_OPEN_COMMENT_IN_THE_END)
	{
		cout << "\"";
			cout << l;
		cout << "\" - " << endl;
	} 
	cout << " ������: " << oldukLine << " �������: " << oldUkInLine << endl;
	map <int, string>::iterator it;
	it = ErrorMessages.find(er_type);
	cout << it->second << endl;
	//cout << "������� �����-������ ������ ��� ���������� ���������" << endl;
	//_getche();
	exit(1);
};

int TScaner::scan(LEX l)
{
beginning:
	int i; // ������� ����� �������
	for (i = 0; i < MaxLex; i++)
	{
		l[i] = 0; //�������� ���� �������
	}
	i = 0; // ������� ����������� � ������� i


	while (Ignor(t[uk]))
	{
		uk++;
	}// ������� ���������� ���������


	if (t[uk] == '\0')
	{
		l[0] = '\0';
		return TYPE_END;
	}
	else
	{
		if (t[uk] == '/')
		{
			l[i++] = t[uk++];
			if (t[uk] == '/')
			{
				uk++;
				while (t[uk] != '\n' && t[uk] != '\0')
				{
					uk++;
				}
				goto beginning;
			}
			else if (t[uk] == '*')
			{
				uk++;
			long_comment:
				while (t[uk] != '*' && t[uk] != '\0')
				{
					uk++;
				}
				if (t[uk] == '*')
				{
					uk++;
					if (t[uk] == '/')
					{
						uk++;
						goto beginning;
					}
					goto long_comment;
				}
				else if (t[uk] == '\0')
				{
					PrintError(ERROR_OPEN_COMMENT_IN_THE_END, 0, l);
					return TYPE_ERROR;
				}
			}
			return TYPE_DIV;
		}//������� ������������ ��� ��������� DIV
		else if (Number(t[uk])) 
		{
			l[i++] = t[uk++];
			while (Number(t[uk]))
			{
				if (i == MaxLex - 1)
				{
					PrintError(ERROR_LEXEMA_IS_TOO_LONG, i, l);
					return TYPE_ERROR;
				}
				l[i++] = t[uk++];
			}
			return TYPE_CONST_10;
		}
		else if (LetterSmall(t[uk]) || LetterBig(t[uk]))
		{
			l[i++] = t[uk++];
			while (Number(t[uk]) || LetterSmall(t[uk]) || LetterBig(t[uk]))
			{
				if (i == MaxLex-1)
				{
					PrintError(ERROR_LEXEMA_IS_TOO_LONG, i, l);
					return TYPE_ERROR;
				}
				l[i++] = t[uk++];
			}

			string check_for_key_words = "";
			for (int dop_i = 0; dop_i < i; dop_i++)
				check_for_key_words += l[dop_i];

			map <string, int>::iterator it;
			it = KW.find(check_for_key_words);
			if (it == KW.end())
				return TYPE_IDENT;
			else
				return it->second;
		}
		else if (t[uk] == '+')
		{
			l[i++] = t[uk++]; return TYPE_PLUS;
		}
		else if (t[uk] == '-')
		{
			l[i++] = t[uk++]; return TYPE_MINUS;
		}
		else if (t[uk] == '<')
		{
			l[i++] = t[uk++];
			if (t[uk] == '=')
			{
				l[i++] = t[uk++];
				return TYPE_LE;
			}
			else if (t[uk] == '<')
			{
				l[i++] = t[uk++]; return TYPE_LS;
			}
			return TYPE_LT;
		}
		else if (t[uk] == '>')
		{
			l[i++] = t[uk++];
			if (t[uk] == '=')
			{
				l[i++] = t[uk++]; return TYPE_GE;
			}
			else if (t[uk] == '>')
			{
				l[i++] = t[uk++]; return TYPE_GS;
			}
			else return TYPE_GT;
		}
		else if (t[uk] == '=')
		{
			l[i++] = t[uk++];
			if (t[uk] == '=')
			{
				l[i++] = t[uk++]; return TYPE_EQ;
			}
			return TYPE_ASSIGN;
		}
		else if (t[uk] == '!')
		{
			l[i++] = t[uk++];
			if (t[uk] == '=')
			{
				l[i++] = t[uk++];
				return TYPE_NEQ;
			}
			PrintError(ERROR_UNKNOWN_LEKSEMA, i, l);
			return TYPE_ERROR;
		}
		else if (t[uk] == '\'')
		{
			l[i++] = t[uk++];
			if (Special(t[uk]) || Number(t[uk]) || LetterSmall(t[uk]) || LetterBig(t[uk]))
			{
				l[i++] = t[uk++];
				if (t[uk] == '\'')
				{
					l[i++] = t[uk++];
					return TYPE_CONST_CHAR;
				}
				PrintError(ERROR_OPEN_CHAR_CONST, i, l);
				return TYPE_ERROR;
			}
			else if (t[uk] == '\\')
			{
				l[i++] = t[uk++];
				if (Special2(t[uk]))
				{
					l[i++] = t[uk++];
					if (t[uk] == '\'')
					{
						l[i++] = t[uk++];
						return TYPE_CONST_CHAR;
					}
					l[i++] = t[uk++];
					PrintError(ERROR_OPEN_CHAR_CONST, i, l);
					return TYPE_ERROR;
				}
				l[i++] = t[uk++];
				if (t[uk] == '\'')
				{
					l[i++] = t[uk++];
					PrintError(ERROR_WRONG_CHAR_CONST, i, l);
				}
				else
				{
					PrintError(ERROR_OPEN_CHAR_CONST, i, l);
					PrintError(ERROR_WRONG_CHAR_CONST, i, l);
				}
				return TYPE_ERROR;
			}
			l[i++] = t[uk++];
			if (t[uk] == '\'')
			{
				l[i++] = t[uk++];
				PrintError(ERROR_WRONG_CHAR_CONST, i, l);
			}
			else
			{
				PrintError(ERROR_OPEN_CHAR_CONST, i, l);
				PrintError(ERROR_WRONG_CHAR_CONST, i, l);
			}
			return TYPE_ERROR;
		}
		else if (t[uk] == '*')
		{
			l[i++] = t[uk++];
			return TYPE_MUL;
		}
		else if (t[uk] == '(')
		{
			l[i++] = t[uk++];
			return TYPE_BRACE_OPEN;
		}
		else if (t[uk] == ')')
		{
			l[i++] = t[uk++];
			return TYPE_BRACE_CLOSED;
		}
		else if (t[uk] == '{')
		{
			l[i++] = t[uk++];
			return TYPE_CUR_BR_OPEN;
		}
		else if (t[uk] == '}')
		{
			l[i++] = t[uk++];
			return TYPE_CUR_BR_CLOSED;
		}
		else if (t[uk] == '.')
		{
			l[i++] = t[uk++];
			return TYPE_DOT;
		}
		else if (t[uk] == ',')
		{
			l[i++] = t[uk++];
			return TYPE_COMMA;
		}
		else if (t[uk] == ';')
		{
			l[i++] = t[uk++];
			return TYPE_SEMICOLON;
		}
		else
		{
			l[i++] = t[uk++];
			PrintError(ERROR_UNKNOWN_LEKSEMA, i, l); // ������
			return TYPE_ERROR;
		}

	} // ����� Scaner

};

int TScaner::scan(LEXs *l)
{
beginning:
	int i; // ������� ����� �������

	*l = ""; //�������� ���� �������
	//i = 0; // ������� ����������� � ������� i
	

	while (Ignor(t[uk]))
	{
		if (t[uk] == '\n')
		{
			ukLine++;
			ukInLine = 1;
		}
		else
			ukInLine++;
		uk++;
	}// ������� ���������� ���������
	int oldUkLine = ukLine;
	int oldUkInLine = ukInLine;

	if (t[uk] == '\0')
	{
		*l = '\0';
		return TYPE_END;
	}
	else
	{
		if (t[uk] == '/')
		{
			*l+= t[uk++]; ukInLine++;
			if (t[uk] == '/')
			{
				uk++; ukInLine++;
				while (t[uk] != '\n' && t[uk] != '\0')
				{
					uk++; ukInLine++;
				}
				goto beginning;
			}
			else if (t[uk] == '*')
			{
				uk++; ukInLine++;
			long_comment:
				while (t[uk] != '*' && t[uk] != '\0')
				{
					uk++;
					if (t[uk] == '\n')
					{
						ukLine++;
						ukInLine = 1;
					}
					else
						ukInLine++;
				}
				if (t[uk] == '*')
				{
					uk++; ukInLine++;
					if (t[uk] == '/')
					{
						uk++; ukInLine++;
						goto beginning;
					}
					goto long_comment;
				}
				else if (t[uk] == '\0')
				{
					PrintError(oldUkLine, oldUkInLine, ERROR_OPEN_COMMENT_IN_THE_END, *l);
					return TYPE_ERROR;
				}
			}
			return TYPE_DIV;
		}//������� ������������ ��� ��������� DIV
		else if (Number(t[uk]))
		{
			*l+= t[uk++]; ukInLine++;
			while (Number(t[uk]))
			{
				*l+= t[uk++]; ukInLine++;
			}
			return TYPE_CONST_10;
		}
		else if (LetterSmall(t[uk]) || LetterBig(t[uk]))
		{
			*l+= t[uk++]; ukInLine++;
			while (Number(t[uk]) || LetterSmall(t[uk]) || LetterBig(t[uk]))
			{
				*l+= t[uk++]; ukInLine++;
			}

			map <string, int>::iterator it;
			it = KW.find(*l);
			if (it == KW.end())
				return TYPE_IDENT;
			else
				return it->second;			
		}
		else if (t[uk] == '+')
		{
			*l+= t[uk++]; ukInLine++; return TYPE_PLUS;
		}
		else if (t[uk] == '-')
		{
			*l+= t[uk++]; ukInLine++; return TYPE_MINUS;
		}
		else if (t[uk] == '<')
		{
			*l+= t[uk++]; ukInLine++;
			if (t[uk] == '=')
			{
				*l+= t[uk++]; ukInLine++;
				return TYPE_LE;
			}
			else if (t[uk] == '<')
			{
				*l+= t[uk++]; ukInLine++; return TYPE_LS;
			}
			return TYPE_LT;
		}
		else if (t[uk] == '>')
		{
			*l+= t[uk++]; ukInLine++;
			if (t[uk] == '=')
			{
				*l+= t[uk++]; ukInLine++; return TYPE_GE;
			}
			else if (t[uk] == '>')
			{
				*l+= t[uk++]; ukInLine++; return TYPE_GS;
			}
			else return TYPE_GT;
		}
		else if (t[uk] == '=')
		{
			*l+= t[uk++]; ukInLine++;
			if (t[uk] == '=')
			{
				*l+= t[uk++]; ukInLine++; return TYPE_EQ;
			}
			return TYPE_ASSIGN;
		}
		else if (t[uk] == '!')
		{
			*l+= t[uk++]; ukInLine++;
			if (t[uk] == '=')
			{
				*l+= t[uk++]; ukInLine++;
				return TYPE_NEQ;
			}
			PrintError(oldUkLine, oldUkInLine, ERROR_UNKNOWN_LEKSEMA, *l);
			return TYPE_ERROR;
		}
		else if (t[uk] == '\'')
		{
			*l+= t[uk++]; ukInLine++;
			if (Special(t[uk]) || Number(t[uk]) || LetterSmall(t[uk]) || LetterBig(t[uk])) 
			{
				*l+= t[uk++]; ukInLine++;
				if (t[uk] == '\'')
				{
					*l+= t[uk++]; ukInLine++;
					return TYPE_CONST_CHAR;
				}
				PrintError(oldUkLine, oldUkInLine, ERROR_OPEN_CHAR_CONST, *l);
				return TYPE_ERROR;
			}
			else if (t[uk] == '\\')
			{
				*l+= t[uk++]; ukInLine++;
				if (Special2(t[uk]))
				{
					*l+= t[uk++]; ukInLine++;
					if (t[uk] == '\'')
					{
						*l+= t[uk++]; ukInLine++;
						return TYPE_CONST_CHAR;
					}
					*l+= t[uk++]; ukInLine++;
					PrintError(oldUkLine, oldUkInLine, ERROR_OPEN_CHAR_CONST, *l);
					return TYPE_ERROR;
				}
				*l+= t[uk++]; ukInLine++;
				if (t[uk] == '\'')
				{
					*l+= t[uk++]; ukInLine++;
					PrintError(oldUkLine, oldUkInLine, ERROR_WRONG_CHAR_CONST, *l);
				}
				else
				{
					PrintError(oldUkLine, oldUkInLine, ERROR_OPEN_CHAR_CONST, *l);
					PrintError(oldUkLine, oldUkInLine, ERROR_WRONG_CHAR_CONST, *l);
				}
				return TYPE_ERROR;
			}
			*l+= t[uk++]; ukInLine++;
			if (t[uk] == '\'')
			{
				l+= t[uk++]; ukInLine++;
				PrintError(oldUkLine, oldUkInLine, ERROR_WRONG_CHAR_CONST, *l);
			}
			else
			{
				PrintError(oldUkLine, oldUkInLine, ERROR_OPEN_CHAR_CONST,*l);
				PrintError(oldUkLine, oldUkInLine, ERROR_WRONG_CHAR_CONST,*l);
			}
			return TYPE_ERROR;
		}
		else if (t[uk] == '*')
		{
			*l+= t[uk++]; ukInLine++;
			return TYPE_MUL;
		}
		else if (t[uk] == '(')
		{
			*l+= t[uk++]; ukInLine++;
			return TYPE_BRACE_OPEN;
		}
		else if (t[uk] == ')')
		{
			*l+= t[uk++]; ukInLine++;
			return TYPE_BRACE_CLOSED;
		}
		else if (t[uk] == '{')
		{
			*l+= t[uk++]; ukInLine++;
			return TYPE_CUR_BR_OPEN;
		}
		else if (t[uk] == '}')
		{
			*l+= t[uk++]; ukInLine++;
			return TYPE_CUR_BR_CLOSED;
		}
		else if (t[uk] == '.')
		{
			*l+= t[uk++]; ukInLine++;
			return TYPE_DOT;
		}
		else if (t[uk] == ',')
		{
			*l+= t[uk++]; ukInLine++;
			return TYPE_COMMA;
		}
		else if (t[uk] == ';')
		{
			*l+= t[uk++]; ukInLine++;
			return TYPE_SEMICOLON;
		}
		else
		{
			*l+= t[uk++]; ukInLine++;
			PrintError(oldUkLine, oldUkInLine, ERROR_UNKNOWN_LEKSEMA, *l); // ������
			return TYPE_ERROR;
		}

	} // ����� Scaner

};

int TScaner::GetTypeOfNextLex(LEXs *lex)
{
	while (Ignor(t[uk]))
	{
		if (t[uk] == '\n')
		{
			ukLine++;
			ukInLine = 1;
		}
		else
			ukInLine++;
		uk++;
	}// ������� ���������� ���������
	int uk, ukLine, ukInLine;
	uk = GetUK(); ukLine = GetUKLine(); ukInLine = GetUKInLine();
	int type = scan(lex);
	SetUK(uk, ukLine, ukInLine);
	return type;
}
