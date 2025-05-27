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

    //Проверяем const
    auto& constToken = tokenizer.GetToken(0);
    EXPECT_EQ(constToken.type, CONST);
    EXPECT_EQ(constToken.value, "const");

    //Проверяем Pi
    auto& piToken = tokenizer.GetToken(1);
    EXPECT_EQ(piToken.type, NAME);
    EXPECT_EQ(piToken.value, "Pi");

    //Проверяем двоеточие
    auto& colonToken = tokenizer.GetToken(2);
    EXPECT_EQ(colonToken.type, COLON);

    // Проверяем double
    auto& doubleToken = tokenizer.GetToken(3);
    EXPECT_EQ(doubleToken.type, NAME);
    EXPECT_EQ(doubleToken.value, "double");

    //Проверяем =
    auto& assignToken = tokenizer.GetToken(4);
    EXPECT_EQ(assignToken.type, MATH_OPERATOR);

    //Проверяем значение
    auto valueToken = tokenizer.GetToken(5);
    EXPECT_EQ(valueToken.type, TokenType::VALUE_DOUBLE);
    EXPECT_EQ(valueToken.value, "3.1415926");
}

TEST(TokenizerTest, VarSection) {
    Tokenizer tokenizer;
    std::string pascalCode = "var  \t  num1, num2: integer;";
    tokenizer.TokenizeCode(pascalCode);


    // Проверяем var
    auto& varToken = tokenizer.GetToken(0);
    EXPECT_EQ(varToken.type, VAR);
    EXPECT_EQ(varToken.value, "var");

    // Проверяем num1
    auto& num1Token = tokenizer.GetToken(1);
    EXPECT_EQ(num1Token.type, NAME);
    EXPECT_EQ(num1Token.value, "num1");

    // Проверяем запятую
    auto& commaToken = tokenizer.GetToken(2);
    EXPECT_EQ(commaToken.type, COMMA);

    // Проверяем num2
    auto& num2Token = tokenizer.GetToken(3);
    EXPECT_EQ(num2Token.type, NAME);
    EXPECT_EQ(num2Token.value, "num2");

    auto& colonToken = tokenizer.GetToken(4);
    EXPECT_EQ(colonToken.type, COLON);
    EXPECT_EQ(colonToken.value, ":");

    auto token = tokenizer.GetToken(5);
    EXPECT_EQ(token.type, NAME);
    EXPECT_EQ(token.value, "integer");

    token = tokenizer.GetToken(6);
    EXPECT_EQ(token.type, END_LINE);
    EXPECT_EQ(token.value, ";");
}

TEST(TokenizerTest, BeginEndBlock) {
    Tokenizer tokenizer;
    std::string pascalCode = "begin \n\t someVar := 42; \n\t Write(\"somaString\"); end";
    tokenizer.TokenizeCode(pascalCode);

    // Проверяем begin
    auto& beginToken = tokenizer.GetToken(0);
    EXPECT_EQ(beginToken.type, BEGIN);
    EXPECT_EQ(beginToken.value, "begin");


    size_t lastEndPos = tokenizer.GetTokenNumber() - 1;
    auto& endToken = tokenizer.GetToken(lastEndPos);
    EXPECT_EQ(endToken.type, END);
    EXPECT_EQ(endToken.value, "end");
}

TEST(TokenizerTest, IfStatement) {
    Tokenizer tokenizer;
    std::string pascalCode = "if (1 == 1) then begin \n\t someVar := 42; \n\t Write(\"somaString\"); end";
    tokenizer.TokenizeCode(pascalCode);

    // Проверяем if
    auto token = tokenizer.GetToken(0);
    EXPECT_EQ(token.type, IF);
    EXPECT_EQ(token.value, "if");

    // Проверяем открывающую скобку
    token = tokenizer.GetToken(1);
    EXPECT_EQ(token.type, BRACKET_OPEN);

    size_t i = 1;
    // Проверяем закрывающую скобку    
    while (tokenizer.GetToken(i).type != BRACKET_CLOSE && i < tokenizer.GetTokenNumber())
        i++;

    // Проверяем then
    token = tokenizer.GetToken(i);
    EXPECT_EQ(token.type, BRACKET_CLOSE);
    EXPECT_EQ(token.value, ")");

    // Проверяем then
    token = tokenizer.GetToken(i + 1);
    EXPECT_EQ(token.type, THEN);
    EXPECT_EQ(token.value, "then");

}

TEST(TokenizerTest, MathOperations) {
    Tokenizer tokenizer;
    std::string pascalCode = "num1 - num2 * 5 div 2";
    tokenizer.TokenizeCode(pascalCode);

    auto token = tokenizer.GetToken(0);
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
    EXPECT_EQ(token.type, VALUE_INT);
    EXPECT_EQ(token.value, "5");

    token = tokenizer.GetToken(5);
    EXPECT_EQ(token.type, MATH_OPERATOR);
    EXPECT_EQ(token.value, "div");

    token = tokenizer.GetToken(6);
    EXPECT_EQ(token.type, VALUE_INT);
    EXPECT_EQ(token.value, "2");
}

TEST(TokenizerTest, ASSIGN_OPERATOR) {
    Tokenizer tokenizer;
    std::string pascalCode = "num := 2";;
    tokenizer.TokenizeCode(pascalCode);

    auto token = tokenizer.GetToken(0);
    EXPECT_EQ(token.type, NAME);
    EXPECT_EQ(token.value, "num");

    token = tokenizer.GetToken(1);
    EXPECT_EQ(token.type, ASSIGN_OPERATOR);
    EXPECT_EQ(token.value, ":=");

    token = tokenizer.GetToken(2);
    EXPECT_EQ(token.type, VALUE_INT);
    EXPECT_EQ(token.value, "2");
}

TEST(TokenizerTest, PROGRAM_END) {
    Tokenizer tokenizer;
    std::string pascalCode = "begin ; end.";;
    tokenizer.TokenizeCode(pascalCode);

    auto token = tokenizer.GetToken(0);
    EXPECT_EQ(token.type, BEGIN);
    EXPECT_EQ(token.value, "begin");


    token = tokenizer.GetToken(2);
    EXPECT_EQ(token.type, END);
    EXPECT_EQ(token.value, "end");

    token = tokenizer.GetToken(3);
    EXPECT_EQ(token.type, PROGRAMM_END);
    EXPECT_EQ(token.value, "."); //?
}

TEST(TokenizerTest, DOUBLE_TEST) {
    Tokenizer tokenizer;
    std::string pascalCode = "num := 2.22";
    tokenizer.TokenizeCode(pascalCode);

    auto token = tokenizer.GetToken(0);
    EXPECT_EQ(token.type, NAME);
    EXPECT_EQ(token.value, "num");

    token = tokenizer.GetToken(1);
    EXPECT_EQ(token.type, ASSIGN_OPERATOR);
    EXPECT_EQ(token.value, ":=");

    token = tokenizer.GetToken(2);
    EXPECT_EQ(token.type, VALUE_DOUBLE);
    EXPECT_EQ(token.value, "2.22");
}

TEST(TokenizerTest, STRING_TEST) {
    Tokenizer tokenizer;
    std::string pascalCode = "num := \"test\"";
    tokenizer.TokenizeCode(pascalCode);

    auto token = tokenizer.GetToken(0);
    EXPECT_EQ(token.type, NAME);
    EXPECT_EQ(token.value, "num");

    token = tokenizer.GetToken(1);
    EXPECT_EQ(token.type, ASSIGN_OPERATOR);
    EXPECT_EQ(token.value, ":=");

    token = tokenizer.GetToken(2);
    EXPECT_EQ(token.type, VALUE_STRING);
    EXPECT_EQ(token.value, "test");
}

TEST(TokenizerTest, WHILE_TEST) {
    Tokenizer tokenizer;
    std::string pascalCode = "while";
    tokenizer.TokenizeCode(pascalCode);

    auto token = tokenizer.GetToken(0);
    EXPECT_EQ(token.type, WHILE);
    EXPECT_EQ(token.value, "while");
}

TEST(TokenizerTest, ELSE_TEST) {
    Tokenizer tokenizer;
    std::string pascalCode = "else";
    tokenizer.TokenizeCode(pascalCode);

    auto token = tokenizer.GetToken(0);
    EXPECT_EQ(token.type, ELSE);
    EXPECT_EQ(token.value, "else");
}

TEST(TokenizerTest, COLON_TEST) {
    Tokenizer tokenizer;
    std::string pascalCode = "num : 2";
    tokenizer.TokenizeCode(pascalCode);

    auto token = tokenizer.GetToken(0);
    EXPECT_EQ(token.type, NAME);
    EXPECT_EQ(token.value, "num");

    token = tokenizer.GetToken(1);
    EXPECT_EQ(token.type, COLON);
    EXPECT_EQ(token.value, ":");

    token = tokenizer.GetToken(2);
    EXPECT_EQ(token.type, VALUE_INT);
    EXPECT_EQ(token.value, "2");
}

TEST(TokenizerTest, COMMA_TEST) {
    Tokenizer tokenizer;
    std::string pascalCode = "num, a";
    tokenizer.TokenizeCode(pascalCode);

    auto token = tokenizer.GetToken(0);
    EXPECT_EQ(token.type, NAME);
    EXPECT_EQ(token.value, "num");

    token = tokenizer.GetToken(1);
    EXPECT_EQ(token.type, COMMA);
    EXPECT_EQ(token.value, ",");

    token = tokenizer.GetToken(2);
    EXPECT_EQ(token.type, NAME);
    EXPECT_EQ(token.value, "a");
}

TEST(TokenizerTest, ENDLINE_TEST) {
    Tokenizer tokenizer;
    std::string pascalCode = "num := 2;";
    tokenizer.TokenizeCode(pascalCode);

    auto token = tokenizer.GetToken(0);
    EXPECT_EQ(token.type, NAME);
    EXPECT_EQ(token.value, "num");

    token = tokenizer.GetToken(1);
    EXPECT_EQ(token.type, ASSIGN_OPERATOR);
    EXPECT_EQ(token.value, ":=");

    token = tokenizer.GetToken(2);
    EXPECT_EQ(token.type, VALUE_INT);
    EXPECT_EQ(token.value, "2");

    token = tokenizer.GetToken(3);
    EXPECT_EQ(token.type, END_LINE);
    EXPECT_EQ(token.value, ";");
}


// wrong behavior
TEST(TokenizerTest, wrong_point) {
    Tokenizer tokenizer;
    std::string pascalCode = "num := 3.14.15";
    tokenizer.TokenizeCode(pascalCode);

    auto token = tokenizer.GetToken(0);
    EXPECT_EQ(token.type, NAME);
    EXPECT_EQ(token.value, "num");


    token = tokenizer.GetToken(1);
    EXPECT_EQ(token.type, ASSIGN_OPERATOR);
    EXPECT_EQ(token.value, ":=");

    token = tokenizer.GetToken(2);
    EXPECT_EQ(token.type, WRONG);
    EXPECT_EQ(token.value, string(1, WRONG_CHAR_SYMBOL));

}

TEST(TokenizerTest, WRONG_SYMBOL_EXCLAMATION) {
    Tokenizer tokenizer;
    std::string pascalCode = "num # :=";
    tokenizer.TokenizeCode(pascalCode);

    auto token = tokenizer.GetToken(0);
    EXPECT_EQ(token.type, NAME);
    EXPECT_EQ(token.value, "num");

    token = tokenizer.GetToken(1);
    EXPECT_EQ(token.type, WRONG);
    EXPECT_EQ(token.value, string(1, WRONG_CHAR_SYMBOL));

    token = tokenizer.GetToken(2);
    EXPECT_EQ(token.type, ASSIGN_OPERATOR);
    EXPECT_EQ(token.value, ":=");

}

TEST(TokenizerTest, WRONG_SYMBOL) {
    Tokenizer tokenizer;
    std::string pascalCode = "num ! :=";
    tokenizer.TokenizeCode(pascalCode);

    auto token = tokenizer.GetToken(0);
    EXPECT_EQ(token.type, NAME);
    EXPECT_EQ(token.value, "num");

    token = tokenizer.GetToken(1);
    EXPECT_EQ(token.type, WRONG);
    EXPECT_EQ(token.value, string(1, WRONG_CHAR_SYMBOL));

    token = tokenizer.GetToken(2);
    EXPECT_EQ(token.type, ASSIGN_OPERATOR);
    EXPECT_EQ(token.value, ":=");

}