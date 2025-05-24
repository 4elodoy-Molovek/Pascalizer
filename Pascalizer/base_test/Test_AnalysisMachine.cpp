#include "gtest/gtest.h"
#include "AnalysisMachine.h"
#include "ProgramInstructions.h"

#define ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation)									\
do																									\
{																									\
	AnalysisMachine* analysisMachine = new AnalysisMachine();										\

	for (auto& token : tokenStream)																	\
		analysisMachine->ProcessElement(token);														\

	HierarchicalList<std::shared_ptr<Instruction>> code = analysisMachine->GetResult();				\

	int i = 0;																						\
	std::shared_ptr<HListNode<std::shared_ptr<Instruction>>> instructionPointer = code.GetFirst();	\
	while (instructionPointer)																		\
	{																								\
		EXPECT_EQ(expectedCodeNotation[i++], instructionPointer->value->GetStringNotation());		\

		// Modifies instruction pointer, traversing the list in the top to bottom order (default)
		if (instructionPointer->pSub)																\
			instructionPointer = instructionPointer->pSub;											\
		else if (instructionPointer->pNext)															\
			instructionPointer = instructionPointer->pNext;											\
		else																						\
			instructionPointer = instructionPointer->pUp->pNext;									\
	}																								\
} while (0)




// Empty program and basics
TEST(AnalysisMachine, Can_Analyze_Empty_Program)
{
	std::vector<Token> tokenStream =
	{
		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
		{BEGIN, "begin"},
		{END, "end"},
		{PROGRAMM_END, "."}
	};

	std::vector<std::string> expectedCodeNotation =
	{
		"IProgram(test)"
	};

	// Standard check
	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
}

TEST(AnalysisMachine, Throws_When_Program_Header_Is_Incorrect)
{
	std::vector<Token> tokenStream =
	{
		{PROGRAM, "prog"}, {NAME, "test"},
		{BEGIN, "begin"},
		{END, "end"},
		{PROGRAMM_END, "."}
	};

	std::vector<std::string> expectedCodeNotation =
	{
		"IProgram(test)"
	};

	// Standard check
	EXPECT_ANY_THROW(ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation));
}


// Constant declaration

TEST(AnalysisMachine, Can_Analyze_Int_Const_Declaration)
{
	std::vector<Token> tokenStream =
	{
		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
		{CONST, "const"},
		{NAME, "Int"}, {COLON, ":"}, {NAME, "int"}, {EQUAL, "="}, {VALUE_INT, "3"}, {END_LINE, ";"},
		{BEGIN, "begin"},
		{END, "end"},
		{PROGRAMM_END, "."}
	};

	std::vector<std::string> expectedCodeNotation =
	{
		"IProgram(test)", "IConstBlock()", "IDeclareConst(Int, 3)"
	};

	// Standard check
	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
}

TEST(AnalysisMachine, Can_Analyze_Double_Const_Declaration)
{
	std::vector<Token> tokenStream =
	{
		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
		{CONST, "const"},
		{NAME, "Doub"}, {COLON, ":"}, {NAME, "double"}, {EQUAL, "="}, {VALUE_DOUBLE, "3.14"}, {END_LINE, ";"},
		{BEGIN, "begin"},
		{END, "end"},
		{PROGRAMM_END, "."}
	};

	std::vector<std::string> expectedCodeNotation =
	{
		"IProgram(test)", "IConstBlock()", "IDeclareConst(Doub, 3.14)"
	};

	// Standard check
	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
}

TEST(AnalysisMachine, Can_Analyze_String_Const_Declaration)
{
	std::vector<Token> tokenStream =
	{
		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
		{CONST, "const"},
		{NAME, "Str"}, {COLON, ":"}, {NAME, "string"}, {EQUAL, "="}, {VALUE_STRING, "some thing like a name, idk"}, {END_LINE, ";"},
		{BEGIN, "begin"},
		{END, "end"},
		{PROGRAMM_END, "."}
	};

	std::vector<std::string> expectedCodeNotation =
	{
		"IProgram(test)", "IConstBlock()", "IDeclareConst(Str, some thing like a name, idk)"
	};

	// Standard check
	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
}

TEST(AnalysisMachine, Can_Analyze_Multiple_Const_Declarations)
{
	std::vector<Token> tokenStream =
	{
		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
		{CONST, "const"},
		{NAME, "Str"}, {COLON, ":"}, {NAME, "string"}, {EQUAL, "="}, {VALUE_STRING, "some thing like a name, idk"}, {END_LINE, ";"},
		{NAME, "Doub"}, {COLON, ":"}, {NAME, "double"}, {EQUAL, "="}, {VALUE_DOUBLE, "3.14"}, {END_LINE, ";"},
		{NAME, "Int"}, {COLON, ":"}, {NAME, "int"}, {EQUAL, "="}, {VALUE_INT, "3"}, {END_LINE, ";"},
		{BEGIN, "begin"},
		{END, "end"},
		{PROGRAMM_END, "."}
	};

	std::vector<std::string> expectedCodeNotation =
	{
		"IProgram(test)", "IConstBlock()", 
		"IDeclareConst(Str, some thing like a name, idk)", "IDeclareConst(Doub, 3.14)", "IDeclareConst(Int, 3)"
	};

	// Standard check
	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
}

TEST(AnalysisMachine, Throws_When_Incorrect_Const_Type)
{
	std::vector<Token> tokenStream =
	{
		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
		{CONST, "const"},
		{NAME, "Str"}, {COLON, ":"}, {NAME, "not_a_type"}, {EQUAL, "="}, {VALUE_STRING, "some thing like a name, idk"}, {END_LINE, ";"},
		{BEGIN, "begin"},
		{END, "end"},
		{PROGRAMM_END, "."}
	};

	std::vector<std::string> expectedCodeNotation =
	{
		"IProgram(test)", "IConstBlock()", "IDeclareConst(Str, some thing like a name, idk)"
	};

	// Standard check
	EXPECT_ANY_THROW(ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation));
}

TEST(AnalysisMachine, Throws_When_Incorrect_Int_Const_Initialization)
{
	std::vector<Token> tokenStream =
	{
		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
		{CONST, "const"},
		{NAME, "Int"}, {COLON, ":"}, {NAME, "int"}, {EQUAL, "="}, {VALUE_STRING, "aboba"}, {END_LINE, ";"},
		{BEGIN, "begin"},
		{END, "end"},
		{PROGRAMM_END, "."}
	};

	std::vector<std::string> expectedCodeNotation =
	{
		"IProgram(test)", "IConstBlock()", "IDeclareConst(Int, 3)"
	};

	// Standard check
	EXPECT_ANY_THROW(ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation));
}

TEST(AnalysisMachine, Throws_When_Incorrect_Double_Const_Initialization)
{
	std::vector<Token> tokenStream =
	{
		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
		{CONST, "const"},
		{NAME, "Doub"}, {COLON, ":"}, {NAME, "double"}, {EQUAL, "="}, {VALUE_STRING, "aboba"}, {END_LINE, ";"},
		{BEGIN, "begin"},
		{END, "end"},
		{PROGRAMM_END, "."}
	};

	std::vector<std::string> expectedCodeNotation =
	{
		"IProgram(test)", "IConstBlock()", "IDeclareConst(Doub, 3.14)"
	};

	// Standard check
	EXPECT_ANY_THROW(ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation));
}

TEST(AnalysisMachine, Throws_When_Unexpected_Token_In_Const_Declaration)
{
	std::vector<Token> tokenStream =
	{
		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
		{CONST, "const"},
		{COLON, ":"}, {NAME, "double"}, {EQUAL, "="}, {VALUE_STRING, "aboba"}, {END_LINE, ";"},
		{BEGIN, "begin"},
		{END, "end"},
		{PROGRAMM_END, "."}
	};

	std::vector<std::string> expectedCodeNotation =
	{
		"IProgram(test)", "IConstBlock()", "IDeclareConst(Doub, 3.14)"
	};

	// Standard check
	EXPECT_ANY_THROW(ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation));


	std::vector<Token> tokenStream =
	{
		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
		{CONST, "const"},
		{NAME, "Doub"}, {NAME, "double"}, {EQUAL, "="}, {VALUE_STRING, "aboba"}, {END_LINE, ";"},
		{BEGIN, "begin"},
		{END, "end"},
		{PROGRAMM_END, "."}
	};

	std::vector<std::string> expectedCodeNotation =
	{
		"IProgram(test)", "IConstBlock()", "IDeclareConst(Doub, 3.14)"
	};

	// Standard check
	EXPECT_ANY_THROW(ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation));


	std::vector<Token> tokenStream =
	{
		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
		{CONST, "const"},
		{NAME, "Doub"}, {COLON, ":"}, {NAME, "double"}, {VALUE_STRING, "aboba"}, {END_LINE, ";"},
		{BEGIN, "begin"},
		{END, "end"},
		{PROGRAMM_END, "."}
	};

	std::vector<std::string> expectedCodeNotation =
	{
		"IProgram(test)", "IConstBlock()", "IDeclareConst(Doub, 3.14)"
	};

	// Standard check
	EXPECT_ANY_THROW(ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation));


	std::vector<Token> tokenStream =
	{
		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
		{CONST, "const"},
		{NAME, "Doub"}, {COLON, ":"}, {NAME, "double"}, {EQUAL, "="}, {END_LINE, ";"},
		{BEGIN, "begin"},
		{END, "end"},
		{PROGRAMM_END, "."}
	};

	std::vector<std::string> expectedCodeNotation =
	{
		"IProgram(test)", "IConstBlock()", "IDeclareConst(Doub, 3.14)"
	};

	// Standard check
	EXPECT_ANY_THROW(ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation));


	std::vector<Token> tokenStream =
	{
		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
		{CONST, "const"},
		{NAME, "Doub"}, {COLON, ":"}, {NAME, "double"}, {EQUAL, "="}, {VALUE_STRING, "aboba"},
		{BEGIN, "begin"},
		{END, "end"},
		{PROGRAMM_END, "."}
	};

	std::vector<std::string> expectedCodeNotation =
	{
		"IProgram(test)", "IConstBlock()", "IDeclareConst(Doub, 3.14)"
	};

	// Standard check
	EXPECT_ANY_THROW(ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation));
}


// Variable declaration

TEST(AnalysisMachine, Can_Analyze_Int_Variable_Declaration)
{
	std::vector<Token> tokenStream =
	{
		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
		{CONST, "const"},
		{NAME, "Int"}, {COLON, ":"}, {NAME, "int"}, {END_LINE, ";"},
		{BEGIN, "begin"},
		{END, "end"},
		{PROGRAMM_END, "."}
	};

	std::vector<std::string> expectedCodeNotation =
	{
		"IProgram(test)", "IConstBlock()", "IDeclareVar(Int)"
	};

	// Standard check
	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
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

