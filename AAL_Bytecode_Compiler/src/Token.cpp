#include "Token.h"
#include "commonFuncs.h"

Token::Token(TokenTypes genTok, const std::string& value)
{
	type=genTok;
	tokenValue=value;
}

void Token::setToken(TokenTypes tokType,const std::string& value)
{
	type=tokType;
	tokenValue=value;
}

void Token::setToken(const std::string& value)
{
	updateGeneralTokenType(value);
}


/** \brief Function which tries to set the tokens based on the characters of token
 * This function only tries to update token, there may be some wrong recognized tokens
 * But this preprocessing is needed for Parser
 *
 * \param
 * \param
 *
 */
void Token::updateGeneralTokenType(const std::string& value)
{
	tokenValue=value;
	int lValue=value.length();
	if(lValue==0)
	{
        type=TOK_UNKNOWN;
        return;
	}

	char firstChar=value[0];
    char secondChar=0;
    if(lValue>1)
    {
        secondChar=value[1];
    }

	switch (firstChar)
	{
		case '/':
            type=TOK_OP_DIV;
			break;
		case '*':
            type=TOK_OP_MUL;
			break;
		case '+':
            type=TOK_OP_ADD;
			break;
		case '-':
            type=TOK_OP_SUB;
			break;
		case '^':
            type=TOK_OP_POW;
			break;
		case '&':
            type=TOK_OP_JOIN;
			break;
		case '%':
            type=TOK_OP_MOD;
			break;
		case '=':
            type=TOK_ASSIGN;
            break;
		case '<':
            type=TOK_BRACKET_ANGLE_OPEN;
			break;
		case '>':
			type=TOK_BRACKET_ANGLE_CLOSE;
			break;
		case '[':
            type=TOK_BRACKET_ARRAY_OPEN;
			break;
		case ']':
            type=TOK_BRACKET_ARRAY_CLOSE;
			break;
		case '(':
            type=TOK_BRACKET_ROUND_OPEN;
			break;
		case ')':
            type=TOK_BRACKET_ROUND_CLOSE;
			break;
		case '{':
            type=TOK_BRACKET_OPEN;
			break;
		case '}':
			type=TOK_BRACKET_CLOSE;
			break;
		case '$':
			type=TOK_VAR;
			break;
		case '@':
			type=TOK_PREDEFINEDVAR;
			break;
		case '#':
			type=TOK_PREPROCESS;
			break;
		case ',':
            type=TOK_PUNCT_COMMA;
			break;
		case '.':
            type=TOK_PUNCT_DOT;
			break;
		case ':':
            type=TOK_PUNCT_DOUBLEDOT;
			break;
		case '?':
			type=TOK_PUNCT_QST;
			break;
        case '!':
			type=TOK_PUNCT_NOT;
			break;
		case '"':
		case '\'':
			type=TOK_LITERAL;
			tokenValue.pop_back();
            tokenValue=tokenValue.substr(1);
			break;
        case '\n':
			type=TOK_NEWLINE;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if(isValidNumber(tokenValue))
			{
				if(firstChar=='0' && (secondChar=='x' || secondChar=='X'))
					type=TOK_NUMBER_HEX;
				else if(firstChar=='0' && (secondChar=='b' || secondChar=='B'))
					type=TOK_NUMBER_BIN;
				else
					type=TOK_NUMBER_DEC;
			}
			else
			{
				type=getKeyword(value);
			}
			break;
		default:
			type=getKeyword(value);
	}
}

/** \brief Function which returns the token keyword type for a given value or TOK_UNKNOWN if token could not be recognized.
 *
 * \param
 * \param
 *
 */
TokenTypes Token::getKeyword(const std::string& value)
{
	char firstChar=value[0];
	switch(firstChar)
	{
		case 'a': case 'A':
			if(iequals(value,"and"))
				return TOK_LOGIC_AND;
		break;
		case 'c': case 'C':
			if(iequals(value,"const"))
				return TOK_CONST;
			if(iequals(value,"case"))
				return TOK_CASE;
            if(iequals(value,"class"))
				return TOK_CLASS;
			if(iequals(value,"continueloop") || iequals(value,"continuecase") || iequals(value,"continue"))
				return TOK_CONTINUE;
			break;
		case 'd': case 'D':
			if(iequals(value,"do"))
				return TOK_DO;
			if(iequals(value,"dim"))
				return TOK_VARDECL;
			if(iequals(value,"default"))
				return TOK_DEFAULT;
			break;
		case 'e': case 'E':
			if(iequals(value,"endfunc"))
				return TOK_ENDFUNC;
			if(iequals(value,"endswitch"))
				return TOK_ENDSWITCH;
			if(iequals(value,"endselect"))
				return TOK_ENDSELECT;
			if(iequals(value,"endif"))
				return TOK_ENDIF;
            if(iequals(value,"endclass"))
				return TOK_ENDCLASS;
            if(iequals(value,"endfor"))
                return TOK_NEXT;
            if(iequals(value,"endwhile"))
				return TOK_WEND;
            if(iequals(value,"enum"))
                return TOK_ENUM;
			if(iequals(value,"else"))
				return TOK_ELSE;
			if(iequals(value,"elseif"))
				return TOK_ELSEIF;
			if(iequals(value,"exit"))
				return TOK_GEN_EXIT;
			if(iequals(value,"exitloop"))
				return TOK_EXIT_LOOP;
			break;
		case 'f': case 'F':
			if(iequals(value,"func"))
				return TOK_FUNCDECL;
			if(iequals(value,"for"))
				return TOK_FOR;
			if(iequals(value,"false"))
				return TOK_FALSE;
			break;
		case 'i': case 'I':
			if(iequals(value,"if"))
				return TOK_IF;
			break;
		case 'n': case 'N':
			if(iequals(value,"null"))
				return TOK_NULL;
			if(iequals(value,"next"))
				return TOK_NEXT;
            if(iequals(value,"new"))
				return TOK_NEW;
			if(iequals(value,"not"))
				return TOK_OP_NOT;
			break;
		case 'r': case 'R':
			if(iequals(value,"return"))
				return TOK_RETURN;
			if(iequals(value,"redim"))
				return TOK_VARDECL;
			break;
		case 's': case 'S':
			if(iequals(value,"select"))
				return TOK_SELECT;
			if(iequals(value,"switch"))
				return TOK_SWITCH;
			if(iequals(value,"step"))
				return TOK_STEP;
			if(iequals(value,"static"))
				return TOK_STATIC;
            if(iequals(value,"synchronized"))
                return TOK_SYNCHRONIZED;
			break;
		case 't': case 'T':
			if(iequals(value,"true"))
				return TOK_TRUE;
			if(iequals(value,"to"))
				return TOK_TO;
			if(iequals(value,"then"))
				return TOK_THEN;
			break;
		case 'u': case 'U':
			if(iequals(value,"until"))
				return TOK_UNTIL;
			break;
		case 'v': case 'V':
			if(iequals(value,"volatile"))
				return TOK_VOLATILE;
			break;
		case 'w': case 'W':
			if(iequals(value,"with"))
				return TOK_WITH;
			if(iequals(value,"wend"))
				return TOK_WEND;
			if(iequals(value,"while"))
				return TOK_WHILE;
			break;
		default:

			break;
	}
	if(iequals(value,"or"))
		return TOK_LOGIC_OR;
	if(iequals(value,"global") || iequals(value,"local") )
		return TOK_VARDECL;
	if(iequals(value,"break"))
		return TOK_EXIT_LOOP;
	if(iequals(value,"byref"))
		return TOK_BYREF;

	if(isValidWordString(value))
		return TOK_FUNC;

	return TOK_UNKNOWN;
}
