#include "Token_Stream.h"
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

//TODO: add support for literals as string, int, or double
//TODO: change multi-word combinations (i.e. VALUES FROM) to be a single token 


//Each character input is turned into a token, using Token_stream::get
Token Token_stream::get() {
	Token curr_token(_null);
	char ch;
	string s;

	if (!unget_tokens.empty()) {
		curr_token = unget_tokens.top();
		unget_tokens.pop();

		return curr_token;
	}

	if (*input_stream >> ch) {
		switch (ch) {
		case '(': curr_token = Token(_lpar); break;
		case ')': curr_token = Token(_rpar); break;
		case '+': curr_token = Token(_plus); break;
		case '-': curr_token = Token(_minus); break;
		case '*': curr_token = Token(_multiply); break;
		case '/': curr_token = Token(_divide); break;
		case '%': curr_token = Token(_mod); break;
		case ';': curr_token = Token(_semicolon); break;
		case ',': curr_token = Token(_comma); break;
		case '"': curr_token = Token(_quotation); break;
		case '<':
			input_stream->get(ch);
			if (ch == '-') {
				curr_token = Token(_assign, "<-");
			}
			else if (ch == '=') {
				curr_token = Token(_less_eq, "<=");
			}
			else {
				curr_token = Token(_less, "<");
			}
			break;
		case '>':
			input_stream->get(ch);
			if (ch == '=') {
				curr_token = Token(_greater_eq, ">=");
			}
			else {
				curr_token = Token(_greater, ">");
			}
			break;
		case '=':
			input_stream->get(ch);
			if (ch == '=') {
				curr_token = Token(_equals, "==");
			}
			else {
				curr_token = Token(_assign_eq, "=");
			}
			break;
		case '!':
			input_stream->get(ch);
			if (ch == '=') {
				curr_token = Token(_not_eq, "!=");
			}
			break;
		case '&':
			input_stream->get(ch);
			if (ch == '&') {
				curr_token = Token(_and, "&&");
			}
			break;
		case '|':
			input_stream->get(ch);
			if (ch == '|') {
				curr_token = Token(_or, "||");
			}
			break;
		case '.':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': { //we are reading some kind of number
			s = "";
			s += ch;
			while (input_stream->get(ch) && (isdigit(ch) || ch == '.')) {
				s += ch;
			}
					  
			input_stream->unget();
			stringstream ss(s);
			//determine if the number is a float or integer
			if (s.find('.') == string::npos) {
				int int_val;
				ss >> int_val;
				curr_token = Token(_int_num, int_val);
			}
			else {
				float float_val;
				ss >> float_val;
				curr_token = Token(_float_num, float_val);
			}
			break;
		}
		default:
			if (isalpha(ch)) {
				s = "";
				s += ch;
				while (input_stream->get(ch) && (isalpha(ch) || ch == '_')) {
					s += ch;
				}
				input_stream->unget();
				transform(s.begin(), s.end(), s.begin(), ::tolower); //convert string to lowercase

				//reserved words
				if (s == "create")		curr_token = Token(_create);
				else if (s == "insert")		curr_token = Token(_insert);
				else if (s == "select")		curr_token = Token(_select);
				else if (s == "project")	curr_token = Token(_project);
				else if (s == "show")		curr_token = Token(_show);
				else if (s == "rename")		curr_token = Token(_rename);

				else if (s == "into")		curr_token = Token(_into);
				else if (s == "table")		curr_token = Token(_table);
				else if (s == "values")		curr_token = Token(_values);
				else if (s == "from")		curr_token = Token(_from);
				else if (s == "primary")	curr_token = Token(_primary);
				else if (s == "key")		curr_token = Token(_key);

				else if (s == "relation")	curr_token = Token(_relation);
				else if (s == "write")		curr_token = Token(_write);
				else if (s == "open")		curr_token = Token(_open);
				else if (s == "close")		curr_token = Token(_close);
				else if (s == "delete")		curr_token = Token(_delete);
				else if (s == "update")		curr_token = Token(_update);
				else if (s == "set")		curr_token = Token(_set);
				else if (s == "where")		curr_token = Token(_where);

				else if (s == "integer")	curr_token = Token(_int_type);
				else if (s == "varchar") {
					char lpar, rpar;
					int size;
					*input_stream >> lpar;
					if (lpar == '(') {
						*input_stream >> size >> rpar;
						if (rpar == ')') {
							curr_token = Token(_varchar, size);
						}
					}
				}
				else if (s == "char")		curr_token = Token(_char);
				else if (s == "float")		curr_token = Token(_float_type);

				else if (s == "exit")		curr_token = Token(_exit_program);
				else						curr_token = Token(_identifier, s);
				break;
			}
		}
		if (curr_token.get_type() != _null) {
			tokens.push_back(curr_token);
			return curr_token;
		}
		else {
			BadTokenException ex("Bad Token: " + s);
			throw ex;
		}
	}
	else {
		BadTokenException ex("Attempt to read from empty stream");
		throw ex;
	}
}

vector<Token> Token_stream::tokenize() {
	Token t = get();

	while (true) {
		if (t.get_type() == _exit_program) {
			break;
		}
		t = get();
	}

	return tokens;
}

//Allows for a bad token to be ignored and not interfere with execution
void Token_stream::ignore(Token_Type c) {
	//if ( == unget_tokens.top().get_type()) {
		//return;
	//}

	return;
}