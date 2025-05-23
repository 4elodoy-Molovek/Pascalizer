#include "gtest/gtest.h"
#include "AnalysisMachine.h"


// Empty program and basics
TEST(AnalysisMachine, Can_Analyze_Empty_Program)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Throws_When_Program_Header_Is_Incorrect)
{
	ADD_FAILURE();
}


// Constant declaration

TEST(AnalysisMachine, Can_Analyze_Int_Const_Declaration)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Can_Analyze_Double_Const_Declaration)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Can_Analyze_String_Const_Declaration)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Can_Analyze_Multiple_Const_Declarations)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Throws_When_Incorrect_Const_Type)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Throws_When_Incorrect_Int_Const_Initialization)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Throws_When_Incorrect_Double_Const_Initialization)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Throws_When_Incorrect_String_Const_Initialization)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Throws_When_Unexpected_Token_In_Const_Declaration)
{
	ADD_FAILURE();
}


// Variable declaration

TEST(AnalysisMachine, Can_Analyze_Int_Variable_Declaration)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Can_Analyze_Double_Variable_Declaration)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Can_Analyze_String_Variable_Declaration)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Can_Analyze_Multiple_Variable_Declarations)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Can_Analyze_Single_Line_Multi_Int_Variable_Declaration)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Can_Analyze_Single_Line_Multi_Double_Variable_Declaration)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Can_Analyze_Single_Line_Multi_String_Variable_Declaration)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Can_Analyze_Multiple_Single_Line_Multi_Variable_Declarations)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Throws_When_Incorrect_Variable_Type)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Throws_When_Trying_To_Initialize_Variable)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Throws_When_Unexpected_Token_In_Variable_Declaration)
{
	ADD_FAILURE();
}



// Main code block

// Error caching
TEST(AnalysisMachine, Throws_When_No_Program_Begin_Found)
{
	ADD_FAILURE();

}

TEST(AnalysisMachine, Throws_When_No_Program_End_Found)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Throws_When_Unexpected_Token_In_Variable_Declaration)
{
	ADD_FAILURE();
}

// Unexpected tokens
TEST(AnalysisMachine, Throws_When_Unexpected_Token_In_Expression_Block)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Throws_When_Unexpected_Token_In_Assigning_Variable)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Throws_When_Unexpected_Token_In_Function_Call)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Throws_When_Unexpected_Token_In_If_Statement)
{
	ADD_FAILURE();
}

// Branching and code blocks
TEST(AnalysisMachine, Throws_When_No_Code_Block_End_Found)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Throws_When_Else_With_No_If_Prior_To_It)
{
	ADD_FAILURE();
}


// Main Block Correctness

// Variables
TEST(AnalysisMachine, Can_Handle_Int_Variable_Assignment)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Can_Handle_Double_Variable_Assignment)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Can_Handle_String_Variable_Assignment)
{
	ADD_FAILURE();
}

// Functions
TEST(AnalysisMachine, Can_Handle_Function_Calls)
{
	ADD_FAILURE();
}

// Branching
TEST(AnalysisMachine, Can_Handle_If_Statements)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Can_Handle_If_Else_Statements)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Can_Handle_Nested_Code_Blocks)
{
	ADD_FAILURE();
}

// Expressions
TEST(AnalysisMachine, Can_Handle_Expressions_In_Variable_Assignment)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Can_Handle_Expressions_In_Function_Calls)
{
	ADD_FAILURE();
}

TEST(AnalysisMachine, Can_Handle_Expressions_In_If_Conditions)
{
	ADD_FAILURE();
}