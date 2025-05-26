//#define ASSERT_LOG_CONTAINS(state, substring)                                   \
//    ASSERT_FALSE((state).log.empty()) << "Log is empty!";                       \
//    ASSERT_NE((state).log.back().find(substring), std::string::npos)            \
//        << "Log does not contain expected substring: " << substring
//
//
//#include <gtest/gtest.h>
//#include "ProgramInstructions.h"
//#include "ValuesTable.h"
//#include "ExpressionEvaluationBlock.h"
//#include "IO_ProcessorInterface.h"
//#include "HierarchicalList.h"
//
//
//// Мокаем Expression, чтобы обойти Caculate
//class FakeExpression : public Expression 
//{
//public:
//    FakeExpression(std::shared_ptr<Value> value)
//        : Expression({}), _value(value) {}
//
//    // Точно перегружаем виртуальный метод родителя!
//    std::shared_ptr<Value> Caculate(ProgramState& state) 
//    {
//        state.log.push_back("FakeExpression used");
//        return _value;
//    }
//
//private:
//    std::shared_ptr<Value> _value;
//};
//
//
//// Мокаем IO-процессор
//class MockIO : public IO_ProcessorInterface 
//{
//public:
//    std::string capturedOutput;
//    std::function<void(IO_InstructionInterface*)> inputCallback;
//    std::function<void(const std::string&)> inputHandler;
//
//    void CallReceiveUserInput(IO_InstructionInterface* instruction) override 
//    {
//        if (inputCallback) inputCallback(instruction);
//    }
//
//    void OnUserInputReceived(const std::string& userInput) override 
//    {
//        if (inputHandler) inputHandler(userInput);
//    }
//
//    void CallOutputString(const std::string& output) override 
//    {
//        capturedOutput = output;
//    }
//};
//
//
//
//// Хелпер для создания состояния
//ProgramState MakeState() 
//{
//    HierarchicalList<std::shared_ptr<Instruction>> dummyCode;
//    static MockIO io;
//    return ProgramState(dummyCode, &io);
//}
//
//// ---- Тесты ----
//
//TEST(IProgramTest, ExecutesSuccessfully) 
//{
//    auto state = MakeState();
//    IProgram program("TestProgram");
//    program.Execute(state);
//    ASSERT_EQ(state.log.back(), "Program started, name: TestProgram");
//}
//
//TEST(IConstBlockTest, LogsStart) 
//{
//    auto state = MakeState();
//    IConstBlock block;
//    block.Execute(state);
//    ASSERT_EQ(state.log.back(), "Const block started");
//}
//
//TEST(IVarBlockTest, LogsStart) 
//{
//    auto state = MakeState();
//    IVarBlock block;
//    block.Execute(state);
//    ASSERT_EQ(state.log.back(), "Var block started");
//}
//
//TEST(IMainBlockTest, LogsStart) 
//{
//    auto state = MakeState();
//    IMainBlock block;
//    block.Execute(state);
//    ASSERT_EQ(state.log.back(), "Main block started");
//}
//
//TEST(IDeclareConstTest, DeclaresConstSuccessfully) 
//{
//    auto state = MakeState();
//    auto value = std::make_shared<IntValue>(5);
//    IDeclareConst inst("c", value);
//    inst.Execute(state);
//    ASSERT_EQ(dynamic_cast<IntValue*>(state.valuesTable["c"].get())->value, 5);
//}
//
//TEST(IDeclareVarTest, DeclaresInt) 
//{
//    auto state = MakeState();
//    IDeclareVar inst(INT, "x");
//    inst.Execute(state);
//    ASSERT_EQ(dynamic_cast<IntValue*>(state.valuesTable["x"].get())->value, 0);
//}
//
//TEST(IAssignVarTest, AssignsCorrectly) 
//{
//    auto state = MakeState();
//    state.valuesTable["x"] = std::make_shared<IntValue>(0);
//
//    auto expr = std::make_shared<FakeExpression>(std::make_shared<IntValue>(123));
//    IAssignVar assign("x", expr);
//
//    ASSERT_NO_THROW(assign.Execute(state));
//    ASSERT_EQ(dynamic_cast<IntValue*>(state.valuesTable["x"].get())->value, 123);
//
//    ASSERT_LOG_CONTAINS(state, "FakeExpression used");
//}
//
//
//TEST(IAssignVarTest, TypeMismatchThrows) 
//{
//    auto state = MakeState();
//    state.valuesTable["x"] = std::make_shared<IntValue>(0);
//    auto expr = std::make_shared<FakeExpression>(std::make_shared<StringValue>("oops"));
//    IAssignVar assign("x", expr);
//    EXPECT_THROW(assign.Execute(state), std::runtime_error);
//}
//
//TEST(IIfTest, ExecutesTrueBranch) 
//{
//    auto state = MakeState();
//    auto expr = std::make_shared<FakeExpression>(std::make_shared<IntValue>(1));
//    IIf instr(expr);
//
//    auto node = std::make_shared<HListNode<std::shared_ptr<Instruction>>>();
//    node->value = std::make_shared<IIf>(expr);
//    state.instructionPointer = node;
//
//    instr.Execute(state);
//    ASSERT_TRUE(state.log.back().find("Entering IF body") != std::string::npos);
//}
//
//TEST(IIfTest, SkipsFalseBranch) 
//{
//    auto state = MakeState();
//    auto expr = std::make_shared<FakeExpression>(std::make_shared<IntValue>(0));
//    IIf instr(expr);
//
//    auto node = std::make_shared<HListNode<std::shared_ptr<Instruction>>>();
//    auto elseNode = std::make_shared<HListNode<std::shared_ptr<Instruction>>>();
//    node->value = std::make_shared<IIf>(expr);
//    node->pNext = elseNode;
//    elseNode->value = std::make_shared<IElse>();
//
//    state.instructionPointer = node;
//
//    instr.Execute(state);
//    ASSERT_EQ(state.instructionPointer, elseNode);
//}
//
//TEST(IElseTest, SkipsIfTrueCondition) 
//{
//    auto state = MakeState();
//    state.branchingStack.push(true);
//    auto nextNode = std::make_shared<HListNode<std::shared_ptr<Instruction>>>();
//    state.instructionPointer = std::make_shared<HListNode<std::shared_ptr<Instruction>>>();
//    state.instructionPointer->pNext = nextNode;
//
//    IElse instr;
//    instr.Execute(state);
//    ASSERT_EQ(state.instructionPointer, nextNode);
//}
//
//TEST(IWhileTest, SkipsWhenFalse)
//{
//    auto state = MakeState();
//    auto expr = std::make_shared<FakeExpression>(std::make_shared<IntValue>(0));
//    IWhile loop(expr);
//
//    auto node = std::make_shared<HListNode<std::shared_ptr<Instruction>>>();
//    node->value = std::make_shared<IWhile>(expr);
//    auto afterNode = std::make_shared<HListNode<std::shared_ptr<Instruction>>>();
//    node->pNext = afterNode;
//    state.instructionPointer = node;
//
//    loop.Execute(state);
//    ASSERT_EQ(state.instructionPointer, afterNode);
//}
//
//TEST(IWriteTest, OutputsExpressionValue) 
//{
//    auto state = MakeState();
//    auto expr = std::make_shared<FakeExpression>(std::make_shared<StringValue>("Hello"));
//    IWrite write({ expr });
//
//    write.Execute(state);
//    auto io = static_cast<MockIO*>(state.ioProcessor);
//    ASSERT_EQ(io->capturedOutput, "Hello");
//}
//
//TEST(IReadTest, AssignsParsedInputCorrectly) 
//{
//    auto& io = *new MockIO();
//    HierarchicalList<std::shared_ptr<Instruction>> dummyCode;
//    ProgramState state(dummyCode, &io);
//    state.valuesTable["x"] = std::make_shared<IntValue>(0);
//
//    auto expr = std::make_shared<FakeExpression>(std::make_shared<StringValue>("x"));
//    auto read = std::make_shared<IRead>(expr);
//
//    io.inputCallback = [&](IO_InstructionInterface* instr) {
//        instr->OnUserInputReceived("42"); // имитируем получение ввода
//    };
//
//    read->Execute(state);
//    ASSERT_EQ(dynamic_cast<IntValue*>(state.valuesTable["x"].get())->value, 42);
//}
//
//TEST(IElseTest, ThrowsWithoutMatchingIf) {
//    auto state = MakeState();
//    IElse instr;
//    EXPECT_THROW(instr.Execute(state), std::runtime_error);
//}
//
//TEST(IIfElseTest, ExecutesElseBranchWhenFalse) 
//{
//    auto state = MakeState();
//
//    auto ifNode = std::make_shared<HListNode<std::shared_ptr<Instruction>>>();
//    auto elseNode = std::make_shared<HListNode<std::shared_ptr<Instruction>>>();
//
//    ifNode->value = std::make_shared<IIf>(std::make_shared<FakeExpression>(std::make_shared<IntValue>(0)));
//    elseNode->value = std::make_shared<IElse>();
//
//    ifNode->pNext = elseNode;
//    state.instructionPointer = ifNode;
//
//    ifNode->value->Execute(state);
//    ASSERT_EQ(state.instructionPointer, elseNode);
//
//    state.branchingStack.push(false);
//    elseNode->pNext = nullptr;
//    elseNode->value->Execute(state);
//    // Проверим, что не скипнуло тело else
//    ASSERT_TRUE(state.log.back().find("Entering ELSE body") != std::string::npos);
//}
//
//TEST(IWhileTest, ExecutesBodyOnce) 
//{
//    auto state = MakeState();
//
//    auto condition = std::make_shared<FakeExpression>(std::make_shared<IntValue>(1));
//    auto loop = std::make_shared<IWhile>(condition);
//
//    auto node = std::make_shared<HListNode<std::shared_ptr<Instruction>>>();
//    node->value = loop;
//    state.instructionPointer = node;
//
//    node->pSub = std::make_shared<HListNode<std::shared_ptr<Instruction>>>();
//    node->pSub->value = std::make_shared<IMainBlock>();
//
//    loop->Execute(state);
//    // Цикл один раз заходит внутрь
//    ASSERT_TRUE(state.log.back().find("Entering WHILE body") != std::string::npos);
//}
//
//TEST(IReadTest, ThrowsIfVarMissing) 
//{
//    auto state = MakeState();
//    auto expr = std::make_shared<FakeExpression>(std::make_shared<StringValue>("missingVar"));
//    auto read = std::make_shared<IRead>(expr);
//
//    EXPECT_THROW(read->Execute(state), std::runtime_error);
//}
//
//TEST(IReadTest, ThrowsOnWrongTypeAssign) 
//{
//    auto& io = *new MockIO();
//    HierarchicalList<std::shared_ptr<Instruction>> dummyCode;
//    ProgramState state(dummyCode, &io);
//    state.valuesTable["x"] = std::make_shared<IntValue>(0);
//
//    auto expr = std::make_shared<FakeExpression>(std::make_shared<StringValue>("x"));
//    auto read = std::make_shared<IRead>(expr);
//
//    io.inputCallback = [&](IO_InstructionInterface* instr) {
//        instr->OnUserInputReceived("hello world"); // строка вместо числа
//    };
//
//    EXPECT_THROW(read->Execute(state), std::runtime_error);
//}
//
//TEST(IWriteTest, OutputsMultipleExpressions) 
//{
//    auto state = MakeState();
//    auto expr1 = std::make_shared<FakeExpression>(std::make_shared<StringValue>("Hi, "));
//    auto expr2 = std::make_shared<FakeExpression>(std::make_shared<StringValue>("World!"));
//    IWrite write({ expr1, expr2 });
//
//    write.Execute(state);
//    auto io = static_cast<MockIO*>(state.ioProcessor);
//    ASSERT_EQ(io->capturedOutput, "Hi, World!");
//}
//
//TEST(IAssignVarTest, ThrowsIfVariableMissing) 
//{
//    auto state = MakeState();
//    auto expr = std::make_shared<FakeExpression>(std::make_shared<IntValue>(5));
//    IAssignVar assign("missing", expr);
//
//    // в отличие от других, тут выбрасывается при попытке сравнить типы
//    EXPECT_THROW(assign.Execute(state), std::runtime_error);
//}
//
//TEST(ExpressionEvaluationBlockTest, ThrowsOnUnrecognizedElement) 
//{
//    class DummyExpr : public ExpressionElement {};
//    Expression expr({ std::make_shared<DummyExpr>() });
//
//    auto state = MakeState();
//    EXPECT_THROW(expr.Caculate(state), std::exception);
//}
//
//TEST(IWriteTest, HandlesEmptyList) 
//{
//    auto state = MakeState();
//    IWrite write({});
//    write.Execute(state);
//
//    auto io = static_cast<MockIO*>(state.ioProcessor);
//    ASSERT_EQ(io->capturedOutput, "");
//}
