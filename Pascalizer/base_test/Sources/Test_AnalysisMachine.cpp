//#include "gtest/gtest.h"
//#include "AnalysisMachine.h"
//#include "ProgramInstructions.h"
//
//static void ANALYSIS_MACHINE_CHECK(const std::vector<Token>& tokenStream, const std::vector<std::string>& expectedCodeNotation)
//{
//	AnalysisMachine* analysisMachine = new AnalysisMachine();
//
//	for (auto& token : tokenStream)
//		analysisMachine->ProcessElement(token);
//
//	HierarchicalList<std::shared_ptr<Instruction>> code = analysisMachine->GetResult();
//
//	int i = 0;
//	int level = 0;
//	std::shared_ptr<HListNode<std::shared_ptr<Instruction>>> instructionPointer = code.GetFirst();
//	while (instructionPointer)
//	{
//		std::string tabStr = "";
//		for (int j = 0; j < level; j++) tabStr += "-";
//
//		EXPECT_EQ(expectedCodeNotation[i++], tabStr + instructionPointer->value->GetStringNotation());
//
//		if (instructionPointer->pSub)
//		{
//			instructionPointer = instructionPointer->pSub;
//			level++;
//		}
//		else if (instructionPointer->pNext)
//		{
//			instructionPointer = instructionPointer->pNext;
//		}
//		else
//		{
//			instructionPointer = instructionPointer->pUp->pNext;
//			level--;
//		}
//	}
//	delete analysisMachine;
//}
//
//
//// Used for catching throws
//static void ANALYSIS_MACHINE_ERROR_CHECK(const std::vector<Token>& tokenStream)
//{
//	AnalysisMachine* analysisMachine = new AnalysisMachine();
//
//	for (auto& token : tokenStream)
//		analysisMachine->ProcessElement(token);
//
//	EXPECT_EQ(analysisMachine->GetStatus(), ERROR);
//}
//
//
//
//
//// Empty program and basics
//TEST(AnalysisMachine, Can_Analyze_Empty_Program)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	std::vector<std::string> expectedCodeNotation =
//	{
//		"IProgram(test)"
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
//}
//
//TEST(AnalysisMachine, Throws_When_Program_Header_Is_Incorrect)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "prog"}, {NAME, "test"},
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_ERROR_CHECK(tokenStream);
//}
//
//
//// Constant declaration
//
//TEST(AnalysisMachine, Can_Analyze_Int_Const_Declaration)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "const"},
//		{NAME, "Int"}, {COLON, ":"}, {NAME, "int"}, {MATH_OPERATOR, "="}, {VALUE_INT, "3"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	std::vector<std::string> expectedCodeNotation =
//	{
//		"IProgram(test)", "-IConstBlock()", "--IDeclareConst(Int, 3)"
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
//}
//
//TEST(AnalysisMachine, Can_Analyze_Double_Const_Declaration)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "const"},
//		{NAME, "Doub"}, {COLON, ":"}, {NAME, "double"}, {MATH_OPERATOR, "="}, {VALUE_DOUBLE, "3.14"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	std::vector<std::string> expectedCodeNotation =
//	{
//		"IProgram(test)", "-IConstBlock()", "--IDeclareConst(Doub, 3.14)"
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
//}
//
//TEST(AnalysisMachine, Can_Analyze_String_Const_Declaration)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "const"},
//		{NAME, "Str"}, {COLON, ":"}, {NAME, "string"}, {MATH_OPERATOR, "="}, {VALUE_STRING, "some thing like a name, idk"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	std::vector<std::string> expectedCodeNotation =
//	{
//		"IProgram(test)", "-IConstBlock()", "--IDeclareConst(Str, some thing like a name, idk)"
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
//}
//
//TEST(AnalysisMachine, Can_Analyze_Multiple_Const_Declarations)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "const"},
//		{NAME, "Str"}, {COLON, ":"}, {NAME, "string"}, {MATH_OPERATOR, "="}, {VALUE_STRING, "some thing like a name, idk"}, {END_LINE, ";"},
//		{NAME, "Doub"}, {COLON, ":"}, {NAME, "double"}, {MATH_OPERATOR, "="}, {VALUE_DOUBLE, "3.14"}, {END_LINE, ";"},
//		{NAME, "Int"}, {COLON, ":"}, {NAME, "int"}, {MATH_OPERATOR, "="}, {VALUE_INT, "3"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	std::vector<std::string> expectedCodeNotation =
//	{
//		"IProgram(test)", "-IConstBlock()",
//		"--IDeclareConst(Str, some thing like a name, idk)", "--IDeclareConst(Doub, 3.14)", "--IDeclareConst(Int, 3)"
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
//}
//
//TEST(AnalysisMachine, Throws_When_Incorrect_Const_Type)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "const"},
//		{NAME, "Str"}, {COLON, ":"}, {NAME, "not_a_type"}, {MATH_OPERATOR, "="}, {VALUE_STRING, "some thing like a name, idk"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_ERROR_CHECK(tokenStream);
//}
//
//TEST(AnalysisMachine, Throws_When_Incorrect_Int_Const_Initialization)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "const"},
//		{NAME, "Int"}, {COLON, ":"}, {NAME, "int"}, {MATH_OPERATOR, "="}, {VALUE_STRING, "aboba"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_ERROR_CHECK(tokenStream);
//}
//
//TEST(AnalysisMachine, Throws_When_Incorrect_Double_Const_Initialization)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "const"},
//		{NAME, "Doub"}, {COLON, ":"}, {NAME, "double"}, {MATH_OPERATOR, "="}, {VALUE_STRING, "aboba"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_ERROR_CHECK(tokenStream);
//}
//
//TEST(AnalysisMachine, Throws_When_Unexpected_Token_In_Const_Declaration)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "const"},
//		{COLON, ":"}, {NAME, "double"}, {MATH_OPERATOR, "="}, {VALUE_STRING, "aboba"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_ERROR_CHECK(tokenStream);
//
//
//	tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "const"},
//		{NAME, "Doub"}, {NAME, "double"}, {MATH_OPERATOR, "="}, {VALUE_STRING, "aboba"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_ERROR_CHECK(tokenStream);
//
//
//	tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "const"},
//		{NAME, "Doub"}, {COLON, ":"}, {NAME, "double"}, {VALUE_STRING, "aboba"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_ERROR_CHECK(tokenStream);
//
//
//	tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "const"},
//		{NAME, "Doub"}, {COLON, ":"}, {NAME, "double"}, {MATH_OPERATOR, "="}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_ERROR_CHECK(tokenStream);
//
//
//	tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "const"},
//		{NAME, "Doub"}, {COLON, ":"}, {NAME, "double"}, {MATH_OPERATOR, "="}, {VALUE_STRING, "aboba"},
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//
//	// Standard check
//	ANALYSIS_MACHINE_ERROR_CHECK(tokenStream);
//}
//
//
//// Variable declaration
//
//TEST(AnalysisMachine, Can_Analyze_Int_Variable_Declaration)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "var"},
//		{NAME, "Int"}, {COLON, ":"}, {NAME, "int"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	std::vector<std::string> expectedCodeNotation =
//	{
//		"IProgram(test)", "-IVarBlock()", "--IDeclareVar(INT, Int)"
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
//}
//
//TEST(AnalysisMachine, Can_Analyze_Double_Variable_Declaration)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "var"},
//		{NAME, "Doub"}, {COLON, ":"}, {NAME, "double"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	std::vector<std::string> expectedCodeNotation =
//	{
//		"IProgram(test)", "-IVarBlock()", "--IDeclareVar(DOUBLE, Doub)"
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
//}
//
//TEST(AnalysisMachine, Can_Analyze_String_Variable_Declaration)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "var"},
//		{NAME, "Str"}, {COLON, ":"}, {NAME, "string"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	std::vector<std::string> expectedCodeNotation =
//	{
//		"IProgram(test)", "-IVarBlock()", "--IDeclareVar(STRING, Str)"
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
//}
//
//TEST(AnalysisMachine, Can_Analyze_Multiple_Variable_Declarations)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "var"},
//		{NAME, "Str"}, {COLON, ":"}, {NAME, "string"}, {END_LINE, ";"},
//		{NAME, "Doub"}, {COLON, ":"}, {NAME, "double"}, {END_LINE, ";"},
//		{NAME, "Int"}, {COLON, ":"}, {NAME, "int"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	std::vector<std::string> expectedCodeNotation =
//	{
//		"IProgram(test)", "-IVarBlock()", "--IDeclareVar(STRING, Str)", "--IDeclareVar(DOUBLE, Doub)", "--IDeclareVar(INT, Int)"
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
//}
//
//TEST(AnalysisMachine, Can_Analyze_Single_Line_Multi_Int_Variable_Declaration)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "var"},
//		{NAME, "Int_1"}, {COMMA, ","}, {NAME, "Int_2"}, {COLON, ":"}, {NAME, "int"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	std::vector<std::string> expectedCodeNotation =
//	{
//		"IProgram(test)", "-IVarBlock()", "--IDeclareVar(INT, Int_1)", "--IDeclareVar(INT, Int_2)"
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
//}
//
//TEST(AnalysisMachine, Can_Analyze_Single_Line_Multi_Double_Variable_Declaration)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "var"},
//		{NAME, "Doub_1"}, {COMMA, ","}, {NAME, "Doub_2"}, {COLON, ":"}, {NAME, "double"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	std::vector<std::string> expectedCodeNotation =
//	{
//		"IProgram(test)", "-IVarBlock()", "--IDeclareVar(DOUBLE, Doub_1)", "--IDeclareVar(DOUBLE, Doub_2)"
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
//}
//
//TEST(AnalysisMachine, Can_Analyze_Single_Line_Multi_String_Variable_Declaration)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "var"},
//		{NAME, "Str_1"}, {COMMA, ","}, {NAME, "Str_2"}, {COLON, ":"}, {NAME, "string"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	std::vector<std::string> expectedCodeNotation =
//	{
//		"IProgram(test)", "-IVarBlock()", "--IDeclareVar(STRING, Str_1)", "--IDeclareVar(STRING, Str_2)"
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
//}
//
//TEST(AnalysisMachine, Can_Analyze_Multiple_Single_Line_Multi_Variable_Declarations)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "var"},
//		{NAME, "Int_1"}, {COMMA, ","}, {NAME, "Int_2"}, {COLON, ":"}, {NAME, "int"}, {END_LINE, ";"},
//		{NAME, "Doub_1"}, {COMMA, ","}, {NAME, "Doub_2"}, {COLON, ":"}, {NAME, "double"}, {END_LINE, ";"},
//		{NAME, "Str_1"}, {COMMA, ","}, {NAME, "Str_2"}, {COLON, ":"}, {NAME, "string"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	std::vector<std::string> expectedCodeNotation =
//	{
//		"IProgram(test)", "-IVarBlock()",
//		"--IDeclareVar(INT, Int_1)", "--IDeclareVar(INT, Int_2)",
//		"--IDeclareVar(DOUBLE, Doub_1)", "--IDeclareVar(DOUBLE, Doub_2)",
//		"--IDeclareVar(STRING, Str_1)", "--IDeclareVar(STRING, Str_2)"
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
//}
//
//TEST(AnalysisMachine, Throws_When_Incorrect_Variable_Type)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "var"},
//		{NAME, "Int"}, {COLON, ":"}, {NAME, "not_a_type"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_ERROR_CHECK(tokenStream);
//}
//
//TEST(AnalysisMachine, Throws_When_Trying_To_Initialize_Variable)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "var"},
//		{NAME, "Int"}, {COLON, ":"}, {NAME, "int"}, {MATH_OPERATOR, "="}, {VALUE_INT, "3"}, { END_LINE, ";" },
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_ERROR_CHECK(tokenStream);
//}
//
//TEST(AnalysisMachine, Throws_When_Unexpected_Token_In_Variable_Declaration)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "var"},
//		{NAME, "Int"}, {COLON, ":"}, {NAME, "int"},
//		{BEGIN, "begin"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//
//	// Standard check
//	ANALYSIS_MACHINE_ERROR_CHECK(tokenStream);
//}
//
//
//
//// Main code block
//
//// Error caching
//TEST(AnalysisMachine, Throws_When_No_Program_Begin_Found)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "prog"}, {NAME, "test"}, {END_LINE, ";"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_ERROR_CHECK(tokenStream);
//}
//
//TEST(AnalysisMachine, Throws_When_No_Program_End_Found)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "prog"}, {NAME, "test"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{PROGRAMM_END, "."}
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_ERROR_CHECK(tokenStream);
//
//	tokenStream =
//	{
//		{PROGRAM, "prog"}, {NAME, "test"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{END, "end"}
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_ERROR_CHECK(tokenStream);
//}
//
//
//TEST(AnalysisMachine, Throws_When_Unexpected_Token_In_Assigning_Variable)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "prog"}, {NAME, "test"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		//{NAME, "var"}, {ASSIGN_OPERATOR, ":="}, {VALUE_INT, "3"}, {END_LINE, ";"},
//		{ASSIGN_OPERATOR, ":="}, {VALUE_INT, "3"}, {END_LINE, ";"},
//
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_ERROR_CHECK(tokenStream);
//
//
//	tokenStream =
//	{
//		{PROGRAM, "prog"}, {NAME, "test"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		//{NAME, "var"}, {ASSIGN_OPERATOR, ":="}, {VALUE_INT, "3"}, {END_LINE, ";"},
//		{NAME, "var"}, {VALUE_INT, "3"}, {END_LINE, ";"},
//
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_ERROR_CHECK(tokenStream);
//
//
//	tokenStream =
//	{
//		{PROGRAM, "prog"}, {NAME, "test"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		//{NAME, "var"}, {ASSIGN_OPERATOR, ":="}, {VALUE_INT, "3"}, {END_LINE, ";"},
//		{NAME, "var"}, {ASSIGN_OPERATOR, ":="}, {END_LINE, ";"},
//
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_ERROR_CHECK(tokenStream);
//
//
//	tokenStream =
//	{
//		{PROGRAM, "prog"}, {NAME, "test"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		//{NAME, "var"}, {ASSIGN_OPERATOR, ":="}, {VALUE_INT, "3"}, {END_LINE, ";"},
//		{NAME, "var"}, {ASSIGN_OPERATOR, ":="}, {VALUE_INT, "3"},
//
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_ERROR_CHECK(tokenStream);
//}
//
//TEST(AnalysisMachine, Throws_When_Unexpected_Token_In_Function_Call)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "prog"}, {NAME, "test"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		//{NAME, "Write"}, {BRACKET_OPEN, "("}, {VALUE_INT, "3"}, {BRACKET_CLOSE, ")"}, {END_LINE, ";"},
//		{NAME, "Write"}, {BRACKET_OPEN, "("}, {VALUE_INT, "3"}, {END_LINE, ";"},
//
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_ERROR_CHECK(tokenStream);
//}
//
//TEST(AnalysisMachine, Throws_When_Unexpected_Token_In_If_Statement)
//{
//	ADD_FAILURE();
//}
//
//// Branching and code blocks
//TEST(AnalysisMachine, Throws_When_No_Code_Block_End_Found)
//{
//	ADD_FAILURE();
//}
//
//TEST(AnalysisMachine, Throws_When_Else_With_No_If_Prior_To_It)
//{
//	ADD_FAILURE();
//}
//
//
//// Unexpected tokens In expressions
//TEST(AnalysisMachine, Throws_When_Unexpected_Token_In_Expression_Block)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "prog"}, {NAME, "test"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		//{NAME, "var"}, {ASSIGN_OPERATOR, ":="}, {VALUE_INT, "3"}, {END_LINE, ";"},
//		{ASSIGN_OPERATOR, ":="}, {VALUE_INT, "3"}, {END_LINE, ";"},
//
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_ERROR_CHECK(tokenStream);
//}
//
//
//// Main Block Correctness
//
//// Variables
//TEST(AnalysisMachine, Can_Handle_Int_Variable_Assignment)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "var"},
//		{NAME, "Int"}, {COLON, ":"}, {NAME, "int"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{NAME, "Int"}, {ASSIGN_OPERATOR, ":="}, {VALUE_INT, "3"}, {END_LINE, ";"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	std::vector<std::string> expectedCodeNotation =
//	{
//		"IProgram(test)", "-IVarBlock()", "--IDeclareVar(INT, Int)", "-IMainBlock()", "--IAssignVar(Int)"
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
//}
//
//TEST(AnalysisMachine, Can_Handle_Double_Variable_Assignment)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "var"},
//		{NAME, "Doub"}, {COLON, ":"}, {NAME, "double"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{NAME, "Int"}, {ASSIGN_OPERATOR, ":="}, {VALUE_INT, "3"}, {END_LINE, ";"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	std::vector<std::string> expectedCodeNotation =
//	{
//		"IProgram(test)", "-IVarBlock()", "--IDeclareVar(DOUBLE, Doub)", "-IMainBlock()", "--IAssignVar(Doub)"
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
//}
//
//TEST(AnalysisMachine, Can_Handle_String_Variable_Assignment)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "var"},
//		{NAME, "Str"}, {COLON, ":"}, {NAME, "string"}, {END_LINE, ";"},
//		{BEGIN, "begin"},
//		{NAME, "Int"}, {ASSIGN_OPERATOR, ":="}, {VALUE_INT, "3"}, {END_LINE, ";"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	std::vector<std::string> expectedCodeNotation =
//	{
//		"IProgram(test)", "-IVarBlock()", "--IDeclareVar(STRING, Str)", "-IMainBlock()", "--IAssignVar(Str)"
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
//}
//
//// Functions
//TEST(AnalysisMachine, Can_Handle_Function_Calls)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "var"},
//		{BEGIN, "begin"},
//		{NAME, "Write"}, {BRACKET_OPEN, "("}, {VALUE_STRING, "Hello World!"}, {BRACKET_CLOSE, ")"}, {END_LINE, ";"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	std::vector<std::string> expectedCodeNotation =
//	{
//		"IProgram(test)", "-IVarBlock()", "--IDeclareVar(INT, Int)", "-IMainBlock()", "--IWrite()"
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
//}
//
//// Branching
//TEST(AnalysisMachine, Can_Handle_If_Statements)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "var"},
//		{BEGIN, "begin"},
//		{IF, "if"}, {BRACKET_OPEN, "("}, {VALUE_INT, "1"}, {BRACKET_CLOSE, ")"}, {THEN, "then"}, {BEGIN, "begin"},
//		{NAME, "Write"}, {BRACKET_OPEN, "("}, {VALUE_STRING, "Hello World!"}, {BRACKET_CLOSE, ")"}, {END_LINE, ";"},
//		{END, "end"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	std::vector<std::string> expectedCodeNotation =
//	{
//		"IProgram(test)", "-IVarBlock()", "--IDeclareVar(INT, Int)", "-IMainBlock()", "--IIF()", "---IWrite()"
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
//}
//
//TEST(AnalysisMachine, Can_Handle_If_Else_Statements)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "var"},
//		{BEGIN, "begin"},
//		{IF, "if"}, {BRACKET_OPEN, "("}, {VALUE_INT, "1"}, {BRACKET_CLOSE, ")"}, {THEN, "then"}, {BEGIN, "begin"},
//		{NAME, "Write"}, {BRACKET_OPEN, "("}, {VALUE_STRING, "Hello World!"}, {BRACKET_CLOSE, ")"}, {END_LINE, ";"},
//		{END, "end"},
//		{ELSE, "else"}, {BEGIN, "begin"},
//		{VALUE_STRING, "Not really Hello World!"},
//		{END, "end"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	std::vector<std::string> expectedCodeNotation =
//	{
//		"IProgram(test)", "-IVarBlock()", "--IDeclareVar(INT, Int)", "-IMainBlock()",
//		"--IIF()", "---IWrite()", "--IElse()", "---IWrite()"
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
//}
//
//TEST(AnalysisMachine, Can_Handle_Nested_Code_Blocks)
//{
//	std::vector<Token> tokenStream =
//	{
//		{PROGRAM, "program"}, {NAME, "test"}, {END_LINE, ";"},
//		{CONST, "var"},
//		{BEGIN, "begin"},
//		{IF, "if"}, {BRACKET_OPEN, "("}, {VALUE_INT, "1"}, {BRACKET_CLOSE, ")"}, {THEN, "then"}, {BEGIN, "begin"},
//		{IF, "if"}, {BRACKET_OPEN, "("}, {VALUE_INT, "1"}, {BRACKET_CLOSE, ")"}, {THEN, "then"}, {BEGIN, "begin"},
//		{NAME, "Write"}, {BRACKET_OPEN, "("}, {VALUE_STRING, "Hello World!"}, {BRACKET_CLOSE, ")"}, {END_LINE, ";"},
//		{END, "end"},
//		{ELSE, "else" }, { BEGIN, "begin" },
//		{VALUE_STRING, "Not really Hello World!" },
//		{END, "end" },
//		{END, "end"},
//		{ELSE, "else"}, {BEGIN, "begin"},
//		{VALUE_STRING, "Not really Hello World!"},
//		{END, "end"},
//		{END, "end"},
//		{PROGRAMM_END, "."}
//	};
//
//	std::vector<std::string> expectedCodeNotation =
//	{
//		"IProgram(test)", "-IVarBlock()", "--IDeclareVar(INT, Int)", "-IMainBlock()",
//		"--IIF()", "---IIF()","----IWrite()", "---IElse()", "----IWrite()", "--IElse()", "----IWrite()"
//	};
//
//	// Standard check
//	ANALYSIS_MACHINE_CHECK(tokenStream, expectedCodeNotation);
//}
//
//// Expressions
//TEST(AnalysisMachine, Can_Handle_Expressions_In_Variable_Assignment)
//{
//	ADD_FAILURE();
//}
//
//TEST(AnalysisMachine, Can_Handle_Expressions_In_Function_Calls)
//{
//	ADD_FAILURE();
//}
//
//TEST(AnalysisMachine, Can_Handle_Expressions_In_If_Conditions)
//{
//	ADD_FAILURE();
//}
//
