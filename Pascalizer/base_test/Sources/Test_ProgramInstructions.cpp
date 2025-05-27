#include <ProgramInstructions.h>
#include <gtest/gtest.h>

HierarchicalList< std::shared_ptr<Instruction>> code;
IO_ProcessorInterface fakeIOProcessor;
ProgramState programState(code, &fakeIOProcessor);

class FakeExpression : public Expression
{
private:
    std::shared_ptr<Value> _value;
public:
    FakeExpression(std::shared_ptr<Value> value) : Expression({}), _value(value) {}

    std::shared_ptr<Value> Calculate(ProgramState& state) override
    {
        state.log.push_back("FakeExpression used");
        return _value;
    }
};


TEST(Test_Simple_Logging, IProgram_Writes_To_The_Log)
{
    IProgram inst("Test");
    inst.Execute(programState);
    EXPECT_EQ(programState.log.back(), "Program started, name: Test");
}

TEST(Test_Simple_Logging, IConstBlock_Writes_To_The_Log)
{
    IConstBlock inst;
    inst.Execute(programState);
    EXPECT_EQ(programState.log.back(), "Const block started");
}

TEST(Test_Simple_Logging, IVarBlock_Writes_To_The_Log)
{
    IVarBlock inst;
    inst.Execute(programState);
    EXPECT_EQ(programState.log.back(), "Var block started");
}

TEST(Test_Simple_Logging, IMainBlock_Writes_To_The_Log)
{
    IMainBlock inst;
    inst.Execute(programState);
    EXPECT_EQ(programState.log.back(), "Main block started");
}

TEST(Test_IDeclareConst, Declare_INT)
{
    std::shared_ptr<Value> val;
    int ival = 1;
    val = std::make_shared<IntValue>(ival);
    IDeclareConst inst("Test", val);

    inst.Execute(programState);
    EXPECT_EQ(programState.log.back(), "Const Test = 1 declared");
}

TEST(Test_IDeclareConst, Declare_STRING)
{
    std::shared_ptr<Value> val;
    std::string sval = "1";
    val = std::make_shared<StringValue>(sval);
    IDeclareConst inst("Test", val);

    inst.Execute(programState);
    EXPECT_EQ(programState.log.back(), "Const Test = 1 declared");
}

TEST(Test_IDeclareConst, Declare_DOUBLE)
{
    std::shared_ptr<Value> val;
    double dval = 1.0f;
    val = std::make_shared<DoubleValue>(dval);
    IDeclareConst inst("Test", val);

    inst.Execute(programState);
    EXPECT_EQ(programState.log.back(), "Const Test = 1.000000 declared");
}

TEST(Test_IDeclareVar, INT_Var_Declares_Sucessfully)
{
    std::string name = "Test";
    Type type = Type::INT;
    IDeclareVar inst(type, "Test");

    inst.Execute(programState);
    EXPECT_EQ(programState.log.back(), "Var Test = 0 declared");
}

TEST(Test_IDeclareVar, STRING_Var_Declares_Sucessfully)
{
    std::string name = "Test";
    Type type = Type::STRING;
    IDeclareVar inst(type, "Test");

    inst.Execute(programState);
    EXPECT_EQ(programState.log.back(), "Var Test =  declared");
}

TEST(Test_IDeclareVar, DOUBLE_Var_Declares_Sucessfully)
{
    std::string name = "Test";
    Type type = Type::DOUBLE;
    IDeclareVar inst(type, "Test");

    inst.Execute(programState);
    EXPECT_EQ(programState.log.back(), "Var Test = 0.000000 declared");
}

TEST(Test_IAssignVar, INT_Var_Assigns_Successfully)
{
    std::string name = "Test";
    std::shared_ptr<Value> val;
    int ival = 10;
    val = std::make_shared<IntValue>(ival);
    std::shared_ptr sexp = std::make_shared<FakeExpression>(val);

    IDeclareVar decl(Type::INT, name);
    decl.Execute(programState);

    IAssignVar ass(name, sexp);
    ass.Execute(programState);

    EXPECT_EQ(programState.valuesTable[name]->PrintValue(), "10");
}

TEST(Test_IAssignVar, DOUBLE_Var_Assigns_Successfully)
{
    std::string name = "Test";
    std::shared_ptr<Value> val;
    double ival = 10.000000;
    val = std::make_shared<DoubleValue>(ival);
    std::shared_ptr sexp = std::make_shared<FakeExpression>(val);

    IDeclareVar decl(Type::DOUBLE, name);
    decl.Execute(programState);

    IAssignVar ass(name, sexp);
    ass.Execute(programState);

    EXPECT_EQ(programState.valuesTable[name]->PrintValue(), "10.000000");
}

TEST(Test_IAssignVar, STRING_Var_Assigns_Successfully)
{
    std::string name = "Test";
    std::shared_ptr<Value> val;
    std::string ival = "Hi";
    val = std::make_shared<StringValue>(ival);
    std::shared_ptr sexp = std::make_shared<FakeExpression>(val);

    IDeclareVar decl(Type::STRING, name);
    decl.Execute(programState);

    IAssignVar ass(name, sexp);
    ass.Execute(programState);

    EXPECT_EQ(programState.valuesTable[name]->PrintValue(), "Hi");
}

TEST(Test_IAssignVar, Throws_When_Assigning_STRING_To_DOUBLE)
{
    std::string name = "Test";
    std::shared_ptr<Value> val;
    std::string ival = "Hi";
    val = std::make_shared<StringValue>(ival);
    std::shared_ptr sexp = std::make_shared<FakeExpression>(val);

    IDeclareVar decl(Type::DOUBLE, name);
    decl.Execute(programState);

    IAssignVar ass(name, sexp);
    EXPECT_ANY_THROW(ass.Execute(programState));
}

TEST(Test_IAssignVar, Throws_When_Assigning_DOUBLE_To_STRING)
{
    std::string name = "Test";
    std::shared_ptr<Value> val;
    double ival = 1.0f;
    val = std::make_shared<DoubleValue>(ival);
    std::shared_ptr sexp = std::make_shared<FakeExpression>(val);

    IDeclareVar decl(Type::STRING, name);
    decl.Execute(programState);

    IAssignVar ass(name, sexp);
    EXPECT_ANY_THROW(ass.Execute(programState));
}

TEST(Test_IWrite, Writes_INT_To_Log)
{
    std::shared_ptr<Value> val = std::make_shared<IntValue>(42);
    std::shared_ptr<FakeExpression> expr = std::make_shared<FakeExpression>(val);
    std::vector<std::shared_ptr<Expression>> vexpr;
    vexpr.push_back(expr);

    IWrite inst(vexpr);
    inst.Execute(programState);

    EXPECT_EQ(programState.log.back(), "OUTPUT: 42");
}

TEST(Test_IWrite, Writes_STRING_To_Log)
{
    auto val = std::make_shared<StringValue>("Hello");
    auto expr = std::make_shared<FakeExpression>(val);
    std::vector<std::shared_ptr<Expression>> vexpr;
    vexpr.push_back(expr);

    IWrite inst(vexpr);
    inst.Execute(programState);

    EXPECT_EQ(programState.log.back(), "OUTPUT: Hello");
}

TEST(Test_IWrite, Writes_DOUBLE_To_Log)
{
    auto val = std::make_shared<DoubleValue>(3.14);
    auto expr = std::make_shared<FakeExpression>(val);
    std::vector<std::shared_ptr<Expression>> vexpr;
    vexpr.push_back(expr);

    IWrite inst(vexpr);
    inst.Execute(programState);

    EXPECT_EQ(programState.log.back(), "OUTPUT: 3.140000");
}

TEST(Test_IIf, If_True_Executes_IfBody)
{
    HierarchicalList<std::shared_ptr<Instruction>> code;

    auto ifInstr = std::make_shared<IIf>(std::make_shared<FakeExpression>(std::make_shared<IntValue>(1)));
    code.AddNextElement(ifInstr);

    auto writeInstr = std::make_shared<IWrite>(
        std::vector<std::shared_ptr<Expression>>{
        std::make_shared<FakeExpression>(std::make_shared<IntValue>(42))
    });
    code.AddSubElement(writeInstr);

    ProgramState state(code, &fakeIOProcessor);
    state.code = code;
    state.currentInstruction = code.GetFirst();
    state.instructionPointer = code.GetFirst();
    state.codeBlockExitInstructionPointerStack.push(nullptr); // фиктивный выход

    state.instructionPointer->value->Execute(state);

    EXPECT_EQ(state.log[0], "IF block started");
    EXPECT_EQ(state.log.back(), "Entering IF body");
}

TEST(Test_IIf, If_False_Skips_IfBody)
{
    HierarchicalList<std::shared_ptr<Instruction>> code;

    // if (false)
    auto ifInstr = std::make_shared<IIf>(std::make_shared<FakeExpression>(std::make_shared<IntValue>(0)));
    code.AddNextElement(ifInstr);

    // тело if
    auto writeInstr = std::make_shared<IWrite>(
        std::vector<std::shared_ptr<Expression>>{
        std::make_shared<FakeExpression>(std::make_shared<IntValue>(42))
    });
    code.AddSubElement(writeInstr);

    ProgramState state(code, &fakeIOProcessor);
    state.code = code;
    state.currentInstruction = code.GetFirst();
    state.instructionPointer = code.GetFirst();
    state.codeBlockExitInstructionPointerStack.push(nullptr); // фиктивный выход

    state.instructionPointer->value->Execute(state);

    EXPECT_EQ(state.log.back(), "Skipping IF body");
    EXPECT_EQ(state.instructionPointer, code.GetFirst()->pNext); // должно перейти на следующую
}

TEST(Test_IElse, Executes_When_If_Condition_False)
{
    HierarchicalList<std::shared_ptr<Instruction>> code;

    auto ifInstr = std::make_shared<IIf>(std::make_shared<FakeExpression>(std::make_shared<IntValue>(0)));
    code.AddNextElement(ifInstr);

    auto elseInstr = std::make_shared<IElse>();
    code.AddNextElement(elseInstr);

    auto writeInstr = std::make_shared<IWrite>(std::vector<std::shared_ptr<Expression>>{
        std::make_shared<FakeExpression>(std::make_shared<IntValue>(999))
    });
    code.AddSubElement(writeInstr);

    ProgramState state(code, &fakeIOProcessor);
    state.currentInstruction = code.GetFirst();
    state.instructionPointer = code.GetFirst();
    state.codeBlockExitInstructionPointerStack.push(nullptr);

    state.currentInstruction->value->Execute(state);

    EXPECT_EQ(state.log.end()[-1], "Skipping IF body");

    // Перейдём к else и выполним
    state.currentInstruction = state.instructionPointer;
    state.instructionPointer = state.instructionPointer->pSub;
    state.currentInstruction->value->Execute(state);

    EXPECT_EQ(state.log.back(), "Entering ELSE body");
}
