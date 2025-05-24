
#include <gtest/gtest.h>
#include "Tokenizer.h"
#include "Tokens.h"
#include <string>


TEST(TokenizerTest, ProgramDeclaration) {
	Tokenizer tokenizer;
	std::string pascalCode = "program Example;";
	tokenizer.TokenizeCode(pascalCode);
	
    auto& tokens = tokenizer.GetToken(0);
    EXPECT_EQ(tokens.type, PROGRAM);
    EXPECT_EQ(tokens.value, "program");
    
    auto& nameToken = tokenizer.GetToken(1);
    EXPECT_EQ(nameToken.type, NAME);
    EXPECT_EQ(nameToken.value, "Example");
    
    auto& endLineToken = tokenizer.GetToken(2);
    EXPECT_EQ(endLineToken.type, END_LINE);
}

TEST(TokenizerTest, ConstSection) {
	Tokenizer tokenizer;
	std::string pascalCode = "const \n\tPi: double = 3.1415926;";
	tokenizer.TokenizeCode(pascalCode);
	
    // ��������� const
    auto& constToken = tokenizer.GetToken(0);
    EXPECT_EQ(constToken.type, CONST);
    EXPECT_EQ(constToken.value, "const");
    
    // ��������� Pi
    auto& piToken = tokenizer.GetToken(1);
    EXPECT_EQ(piToken.type, NAME);
    EXPECT_EQ(piToken.value, "Pi");
    
    // ��������� ���������
    auto& colonToken = tokenizer.GetToken(2);
    EXPECT_EQ(colonToken.type, COLON);
    
    // ��������� double
    auto& doubleToken = tokenizer.GetToken(3);
    EXPECT_EQ(doubleToken.type, NAME);
    EXPECT_EQ(doubleToken.value, "double");
    
    // ��������� =
    auto& assignToken = tokenizer.GetToken(4);
    EXPECT_EQ(assignToken.type, ASSIGN_OPERATOR);
    
    // ��������� ��������
    auto& valueToken = tokenizer.GetToken(5);
    EXPECT_EQ(valueToken.type, VALUE);
    EXPECT_EQ(valueToken.value, "3.1415926");
}

TEST(TokenizerTest, VarSection) {
	Tokenizer tokenizer;
	std::string pascalCode = "var  \t  num1, num2: integer;";
	tokenizer.TokenizeCode(pascalCode);
	
	
    // ��������� var
    auto& varToken = tokenizer.GetToken(0);
    EXPECT_EQ(varToken.type, VAR);
    EXPECT_EQ(varToken.value, "var");
    
    // ��������� num1
    auto& num1Token = tokenizer.GetToken(1);
    EXPECT_EQ(num1Token.type, NAME);
    EXPECT_EQ(num1Token.value, "num1");
    
    // ��������� �������
    auto& commaToken = tokenizer.GetToken(2);
    EXPECT_EQ(commaToken.type, COMMA);
    
    // ��������� num2
    auto& num2Token = tokenizer.GetToken(3);
    EXPECT_EQ(num2Token.type, NAME);
    EXPECT_EQ(num2Token.value, "num2");
	
	auto& colonToken = tokenizer.GetToken(4);
    EXPECT_EQ(colonToken.type, COLON);
    EXPECT_EQ(colonToken.value, ":");
	
	auto& token = tokenizer.GetToken(5);
    EXPECT_EQ(token.type, NAME);
    EXPECT_EQ(token.value, "integer");
	
	token = tokenizer.GetToken(6);
    EXPECT_EQ(token.type, END_LINE);
    EXPECT_EQ(token.value, ";");
}

TEST(TokenizerTest, BeginEndBlock) {
	Tokenizer tokenizer;
	std::string pascalCode = "begin \n\t someVar := 42; \n\t Write("somaString"); end";
	tokenizer.TokenizeCode(pascalCode);
	
    // ��������� begin
    auto& beginToken = tokenizer.GetToken(19);
    EXPECT_EQ(beginToken.type, BEGIN);
    EXPECT_EQ(beginToken.value, "begin");
    
	
    size_t lastEndPos = tokenizer.GetTokenNumber() - 1;
    auto& endToken = tokenizer.GetToken(lastEndPos);
    EXPECT_EQ(endToken.type, END);
    EXPECT_EQ(endToken.value, "end");   
}

TEST(TokenizerTest, IfStatement) {
	Tokenizer tokenizer;
	std::string pascalCode = "if (1 == 1) then begin \n\t someVar := 42; \n\t Write("somaString"); end";
	tokenizer.TokenizeCode(pascalCode);
	
    // ��������� if
    auto& token = tokenizer.GetToken(0);
    EXPECT_EQ(token.type, IF);
    EXPECT_EQ(token.value, "if");
    
    // ��������� ����������� ������
    token = tokenizer.GetToken(1);
    EXPECT_EQ(token.type, BRACKET_OPEN);
	
	size_t i = 1;
	// ��������� ����������� ������    
	while (tokenizer.GetToken(2) != BRACKET_CLOSE && i < tokenizer.GetTokenNumber()) 
		i++;
	
	 // ��������� then
	token = tokenizer.GetToken(i);
    EXPECT_EQ(token.type, BRACKET_CLOSE);
    EXPECT_EQ(token.value, ")");
	
    // ��������� then
	token = tokenizer.GetToken(i+1);
    EXPECT_EQ(token.type, THEN);
    EXPECT_EQ(token.value, "then");
	
}

TEST(TokenizerTest, MathOperations) {
	Tokenizer tokenizer;
	std::string pascalCode = "(num1 - num2 * 5 div 2)";
	tokenizer.TokenizeCode(pascalCode);
	
    auto& token = tokenizer.GetToken(0);
    EXPECT_EQ(token.type, NAME);
    EXPECT_EQ(token.value, "num1");
	
	token = tokenizer.GetToken(1);
    EXPECT_EQ(token.type, MATH_OPERATOR);
    EXPECT_EQ(token.value, "-");
    
    token = tokenizer.GetToken(2);
    EXPECT_EQ(token.type, NAME);
    EXPECT_EQ(token.value, "num2");
    
    token = tokenizer.GetToken(3);
    EXPECT_EQ(token.type, MATH_OPERATOR);
    EXPECT_EQ(token.value, "*");
	
	token = tokenizer.GetToken(4);
    EXPECT_EQ(token.type, INT_VALUE);
    EXPECT_EQ(token.value, "5");   
	
	token = tokenizer.GetToken(5);
    EXPECT_EQ(token.type, MATH_OPERATOR);
    EXPECT_EQ(token.value, "div");
	
	token = tokenizer.GetToken(6);
    EXPECT_EQ(token.type, INT_VALUE);
    EXPECT_EQ(token.value, "5");
}

TEST(TokenizerTest, ASSIGN_OPERATOR) {
	Tokenizer tokenizer;
	std::string pascalCode = "num := 2";;
	tokenizer.TokenizeCode(pascalCode);
	
    auto& token = tokenizer.GetToken(0);
    EXPECT_EQ(token.type, NAME);
    EXPECT_EQ(token.value, "num1");
	
	 token = tokenizer.GetToken(1);
    EXPECT_EQ(token.type, ASSIGN_OPERATOR);
    EXPECT_EQ(token.value, "*");
	
	token = tokenizer.GetToken(2);
    EXPECT_EQ(token.type, INT_VALUE);
    EXPECT_EQ(token.value, "5");   
	
	token = tokenizer.GetToken(3);
    EXPECT_EQ(token.type, END_LINE);
    EXPECT_EQ(token.value, ";"); 
	
}

TEST(TokenizerTest, PROGRAM_END) {
	Tokenizer tokenizer;
	std::string pascalCode = "begin ; end.";;
	tokenizer.TokenizeCode(pascalCode);
	
	auto& token = tokenizer.GetToken(0);
    EXPECT_EQ(token.type, BEGIN);
    EXPECT_EQ(token.value, "begin");
    
	
    token = tokenizer.GetToken(2);
    EXPECT_EQ(token.type, END);
    EXPECT_EQ(token.value, "end");  

    token = tokenizer.GetToken(3);
    EXPECT_EQ(token.type, PROGRAM_END);
    EXPECT_EQ(token.value, "."); //?
}

//wrong behavior

/*
DOUBLE_VALUE,
STRING_VALUE,

QUOTES,
COMMA,
COLON,

// Condition
ELSE,
WHILE,


// Specific
WRONG,
NULL_TOKEN, // this is not for you

// comparison
MORE_EQUAL,
LESS,
MORE,
LESS_EQUAL,
EQUAL,
NOT_EQUAL
*/

/*
program Example;
const
    Pi: double = 3.1415926;
var
    num1, num2: integer;
    Res, d: double;
begin
    num1 := 5;
    Write("������� ������ ����� �����: ");
    Read(num2);
    Write("������� ������������ �����: ");
    Read(d);
    if (num2 mod 2 = 0) then
    begin
        Res := (num1 - num2 * 5 div 2) / (d * 2);
        Write("��������� = ", Res);
    end
    else
        Write("�������� ����");
end.
)";
        tokenizer.TokenizeCode(pascalCode);
    }

    Tokenizer tokenizer;
    std::string pascalCode;
};
*/
/*
program Example;
const
    Pi: double = 3.1415926;
var
    num1, num2: integer;
    Res, d: double;
begin
    num1 := 5;
    Write("������� ������ ����� �����: ");
    Read(num2);
    Write("������� ������������ �����: ");
    Read(d);
    if (num2 mod 2 = 0) then
    begin
        Res := (num1 - num2 * 5 div 2) / (d * 2);
        Write("��������� = ", Res);
    end
    else
        Write("�������� ����");
end.
)";
        tokenizer.TokenizeCode(pascalCode);
    }

    Tokenizer tokenizer;
    std::string pascalCode;
};
*/