#include "gtest.h"
#include "Tokeniser.h"


// Creating
TEST(Tokenizer, Tokenizer_Could_be_created)
{
    ASSERT_NO_THROW();
}


// 	Token& GetTokens(size_t index);
TEST(Tokenizer, Tokenizer_Throw_Error_When_Trying_To_Get_Out_Of_Range_Index)
{
    ASSERT_NO_THROW();
}

TEST(Tokenizer, Tokenizer_Can_Get_Token_By_Index)
{
    ASSERT_NO_THROW();
}


// size_t GetTokenNumber();
TEST(Tokenizer, Tokenizer_Can_Get_Token_Vector_Size_Correctly)
{
    ASSERT_NO_THROW();
}

TEST(Tokenizer, Tokenizer_Can_Get_Token_Vector_Size)
{
    ASSERT_NO_THROW();
}

TEST(Tokenizer, Empty_Tokenizer_Returns_Zero_Token_Count)
{
    Tokenizer tk;
    ASSERT_TRUE(tk.GetTokenNumber() == 0);
}

// const std::vector<Token>& GetCachedTokens();
TEST(Tokenizer, Tokenizer_Can_Get_Cached_Tokens)
{
    ASSERT_NO_THROW();
}

TEST(Tokenizer, Empty_Tokenizer_Returns_Zero_Token_Count)
{
    Tokenizer tk;
    ASSERT_TRUE(tk.GetTokenNumber() == 0);
}

TEST(Tokenizer, Empty_Tokenizer_Returns_Empty_Cached_Token_Vector)
{
    Tokenizer tk;
    ASSERT_TRUE(tk.GetCachedToken().szie() == 0);
}


// vector<Token>& TokenizeCode(string& sourceCode);
// Normal behavior
TEST(Tokenizer, Tokenizer_Can_Tokenize_Code)
{
    ASSERT_NO_THROW();
}

TEST(Tokenizer, Tokenizer_Can_Tokenize_Empty_Code)
{
    ASSERT_NO_THROW();
}

TEST(Tokenizer, Tokenizer_Can_Tokenize_Empty_Code_Pattern)
{
    // When there are only empty sections. Maybe error?
    ASSERT_NO_THROW();
}


// Tokens types test
TEST(Tokenizer, Tokenizer_Can_Identify_BeginEnd_Section)
{
    ASSERT_NO_THROW();
}

TEST(Tokenizer, Tokenizer_Can_Identify_Variable_defeniton)
{
    ASSERT_NO_THROW();
}

TEST(Tokenizer, Tokenizer_Can_Identify_Single_If)
{
    ASSERT_NO_THROW();
}

TEST(Tokenizer, Tokenizer_Can_Identify_If_Else_Section)
{
    ASSERT_NO_THROW();
}

TEST(Tokenizer, Tokenizer_Can_Identify_BRACKET_OPEN_Token)
{
    ASSERT_NO_THROW();
}

TEST(Tokenizer, Tokenizer_Can_Identify_BRACKET_CLOSE_Token)
{
    ASSERT_NO_THROW();
}

TEST(Tokenizer, Tokenizer_Can_Identify_Many_Arguements_Functions)
{
    ASSERT_NO_THROW();
}

// Errors detecting
TEST(Tokenizer, Tokenizer_Throws_Error_When_Meets_Undefined_Synbols)
{
    ASSERT_NO_THROW();
}
