#include "gTEST/gTEST.h"
#include "Expressionevaluationblock.h"
#include <./ProgramState.h>
#include <./Instruction.h>
#include <ProgramInstructions.h>


static HierarchicalList< std::shared_ptr<Instruction>> code;
static IO_ProcessorInterface fakeIOProcessor;
static ProgramState programState(code, &fakeIOProcessor);


// Helper function to create and evaluate simple binary expressions
template<typename Operation>
std::shared_ptr<Value> evaluateBinaryOp(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs) {
    std::vector<std::shared_ptr<ExpressionElement>> postfix;
    postfix.push_back(std::make_shared<ValueExpressionElement>(lhs));
    postfix.push_back(std::make_shared<ValueExpressionElement>(rhs));
    postfix.push_back(std::make_shared<Operation>());
    Expression exp(postfix);
    return exp.Calculate(programState);
}

// Helper function to create and evaluate unary operations
template<typename Operation>
std::shared_ptr<Value> evaluateUnaryOp(std::shared_ptr<Value> val) {
    std::vector<std::shared_ptr<ExpressionElement>> postfix;
    postfix.push_back(std::make_shared<ValueExpressionElement>(val));
    postfix.push_back(std::make_shared<Operation>());
    Expression exp(postfix);
    return exp.Calculate(programState);
}





// Arithmetic operations tests
TEST(ExpressionEvaluationTest, AddOperationIntInt) {
    auto result = evaluateBinaryOp<AddOperation>(
        std::make_shared<IntValue>(5),
        std::make_shared<IntValue>(3)
    );
    EXPECT_EQ(dynamic_cast<IntValue*>(result.get())->value, 8);
}

TEST(ExpressionEvaluationTest, AddOperationIntDouble) {
    auto result = evaluateBinaryOp<AddOperation>(
        std::make_shared<IntValue>(5),
        std::make_shared<DoubleValue>(3.5)
    );
    EXPECT_DOUBLE_EQ(dynamic_cast<DoubleValue*>(result.get())->value, 8.5);
}

TEST(ExpressionEvaluationTest, AddOperationStringString) {
    auto result = evaluateBinaryOp<AddOperation>(
        std::make_shared<StringValue>("hello"),
        std::make_shared<StringValue>(" world")
    );
    EXPECT_EQ(dynamic_cast<StringValue*>(result.get())->value, "hello world");
}


TEST(ExpressionEvaluationTest, SubtractOperationIntInt) {
    auto result = evaluateBinaryOp<SubtractOperation>(
        std::make_shared<IntValue>(10),
        std::make_shared<IntValue>(4)
    );
    EXPECT_EQ(dynamic_cast<IntValue*>(result.get())->value, 6);
}

TEST(ExpressionEvaluationTest, SubtractOperationDoubleInt) {
    auto result = evaluateBinaryOp<SubtractOperation>(
        std::make_shared<DoubleValue>(10.25),
        std::make_shared<IntValue>(4)
    );
    EXPECT_EQ(dynamic_cast<DoubleValue*>(result.get())->value, 6.25);
}


TEST(ExpressionEvaluationTest, MultiplyOperationIntInt) {
    auto result = evaluateBinaryOp<MultiplyOperation>(
        std::make_shared<IntValue>(5),
        std::make_shared<IntValue>(4)
    );
    EXPECT_EQ(dynamic_cast<IntValue*>(result.get())->value, 20);
}

TEST(ExpressionEvaluationTest, MultiplyOperationIntString) {
    auto result = evaluateBinaryOp<MultiplyOperation>(
        std::make_shared<IntValue>(3),
        std::make_shared<StringValue>("ab")
    );
    EXPECT_EQ(dynamic_cast<StringValue*>(result.get())->value, "ababab");
}

TEST(ExpressionEvaluationTest, MultiplyOperationStringInt) {
    auto result = evaluateBinaryOp<MultiplyOperation>(
        std::make_shared<StringValue>("ab"),
        std::make_shared<IntValue>(3)
    );
    EXPECT_EQ(dynamic_cast<StringValue*>(result.get())->value, "ababab");
}


TEST(ExpressionEvaluationTest, DivideOperationIntInt) {
    auto result = evaluateBinaryOp<DivideOperation>(
        std::make_shared<IntValue>(10),
        std::make_shared<IntValue>(2)
    );
    EXPECT_EQ(dynamic_cast<IntValue*>(result.get())->value, 5);
}

TEST(ExpressionEvaluationTest, DivideOperationDoubleInt) {
    auto result = evaluateBinaryOp<DivideOperation>(
        std::make_shared<DoubleValue>(10.12),
        std::make_shared<IntValue>(2)
    );
    EXPECT_EQ(abs(dynamic_cast<DoubleValue*>(result.get())->value - 5.06) < 0.1, 1);
}

TEST(ExpressionEvaluationTest, DivideOperationIntDouble) {
    auto result = evaluateBinaryOp<DivideOperation>(
        std::make_shared<IntValue>(20),
        std::make_shared<DoubleValue>(0.5)
    );
    EXPECT_EQ(abs(dynamic_cast<DoubleValue*>(result.get())->value - 40) < 0.1, 1);
}


TEST(ExpressionEvaluationTest, ModOperationIntInt) {
    auto result = evaluateBinaryOp<ModOperation>(
        std::make_shared<IntValue>(10),
        std::make_shared<IntValue>(3)
    );
    EXPECT_EQ(dynamic_cast<IntValue*>(result.get())->value, 1);
}

TEST(ExpressionEvaluationTest, DivOperationIntInt) {
    auto result = evaluateBinaryOp<DivOperation>(
        std::make_shared<IntValue>(10),
        std::make_shared<IntValue>(3)
    );
    EXPECT_EQ(dynamic_cast<IntValue*>(result.get())->value, 3);
}



// Comparison operations tests
TEST(ExpressionEvaluationTest, MoreOperationIntIntTrue) {
    auto result = evaluateBinaryOp<MoreOperation>(
        std::make_shared<IntValue>(10),
        std::make_shared<IntValue>(5)
    );
    EXPECT_EQ(dynamic_cast<IntValue*>(result.get())->value, 1);
}

TEST(ExpressionEvaluationTest, MoreOperationIntIntFalse) {
    auto result = evaluateBinaryOp<MoreOperation>(
        std::make_shared<IntValue>(5),
        std::make_shared<IntValue>(10)
    );
    EXPECT_EQ(dynamic_cast<IntValue*>(result.get())->value, 0);
}

TEST(ExpressionEvaluationTest, MoreEqualOperationIntIntEqual) {
    auto result = evaluateBinaryOp<MoreEqualOperation>(
        std::make_shared<IntValue>(5),
        std::make_shared<IntValue>(5)
    );
    EXPECT_EQ(dynamic_cast<IntValue*>(result.get())->value, 1);
}

TEST(ExpressionEvaluationTest, MoreOperationStringString) {
    std::vector<std::shared_ptr<ExpressionElement>> pst2;
    pst2.push_back(std::make_shared<ValueExpressionElement>(std::make_shared<StringValue>("baa")));
    pst2.push_back(std::make_shared<ValueExpressionElement>(std::make_shared<StringValue>("bag")));
    pst2.push_back(std::make_shared<MoreOperation>());
    Expression exp2(pst2);
    auto result = exp2.Calculate(programState);
    EXPECT_EQ(dynamic_cast<IntValue*>(result.get())->value, 0);  // "baa" > "bag" is false
}

TEST(ExpressionEvaluationTest, LessOperationIntDoubleTrue) {
    auto result = evaluateBinaryOp<EqualOperation>(
        std::make_shared<IntValue>(5),
        std::make_shared<DoubleValue>(5.0)
    );
    EXPECT_EQ(dynamic_cast<IntValue*>(result.get())->value, 1);
}

TEST(ExpressionEvaluationTest, LessOperationStringStringFalse) {
    auto result = evaluateBinaryOp<LessOperation>(
        std::make_shared<DoubleValue>(1.2),
        std::make_shared<IntValue>(1)
    );
    EXPECT_EQ(dynamic_cast<IntValue*>(result.get())->value, 0);
}

TEST(ExpressionEvaluationTest, NotEqualOperationStringStringFalse) {
    auto result = evaluateBinaryOp<NotEqualOperation>(
        std::make_shared<StringValue>("test"),
        std::make_shared<StringValue>("test")
    );
    EXPECT_EQ(dynamic_cast<IntValue*>(result.get())->value, 0);
}

TEST(ExpressionEvaluationTest, MoreOperationStringStringFalse)
{
    std::vector<std::shared_ptr<ExpressionElement>> pst2;

    pst2.push_back(std::make_shared<ValueExpressionElement>(ValueExpressionElement(std::make_shared<StringValue>(StringValue("baa")))));
    pst2.push_back(std::make_shared<ValueExpressionElement>(ValueExpressionElement(std::make_shared<StringValue>(StringValue("bag")))));
    pst2.push_back(std::make_shared<MoreOperation>(MoreOperation()));
    Expression exp2(pst2);

    EXPECT_EQ(std::string(exp2.Calculate(programState)->PrintValue()), std::string("0"));
}

TEST(ExpressionEvaluationTest, LessOperationStringStringTrue)
{
    std::vector<std::shared_ptr<ExpressionElement>> pst2;

    pst2.push_back(std::make_shared<ValueExpressionElement>(ValueExpressionElement(std::make_shared<StringValue>(StringValue("baa")))));
    pst2.push_back(std::make_shared<ValueExpressionElement>(ValueExpressionElement(std::make_shared<StringValue>(StringValue("bag")))));
    pst2.push_back(std::make_shared<LessOperation>(LessOperation()));
    Expression exp2(pst2);

    EXPECT_EQ(exp2.Calculate(programState)->PrintValue(), "1");
}

TEST(ExpressionEvaluationTest, MoreEqualOperationStringStringTrue)
{
    std::vector<std::shared_ptr<ExpressionElement>> pst2;

    pst2.push_back(std::make_shared<ValueExpressionElement>(ValueExpressionElement(std::make_shared<StringValue>(StringValue("bag")))));
    pst2.push_back(std::make_shared<ValueExpressionElement>(ValueExpressionElement(std::make_shared<StringValue>(StringValue("bag")))));
    pst2.push_back(std::make_shared<MoreEqualOperation>(MoreEqualOperation()));
    Expression exp2(pst2);

    EXPECT_EQ(exp2.Calculate(programState)->PrintValue(), "1");

    pst2.clear();


    pst2.push_back(std::make_shared<ValueExpressionElement>(ValueExpressionElement(std::make_shared<StringValue>(StringValue("bag")))));
    pst2.push_back(std::make_shared<ValueExpressionElement>(ValueExpressionElement(std::make_shared<StringValue>(StringValue("bay")))));
    pst2.push_back(std::make_shared<MoreEqualOperation>(MoreEqualOperation()));

    EXPECT_EQ(exp2.Calculate(programState)->PrintValue(), "1");
}

TEST(ExpressionEvaluationTest, LessEqualOperationStringStringTrue)
{
    std::vector<std::shared_ptr<ExpressionElement>> pst2;

    pst2.push_back(std::make_shared<ValueExpressionElement>(ValueExpressionElement(std::make_shared<StringValue>(StringValue("bag")))));
    pst2.push_back(std::make_shared<ValueExpressionElement>(ValueExpressionElement(std::make_shared<StringValue>(StringValue("bag")))));
    pst2.push_back(std::make_shared<LessEqualOperation>(LessEqualOperation()));
    Expression exp2(pst2);

    EXPECT_EQ(exp2.Calculate(programState)->PrintValue(), "1");

    pst2.clear();


    pst2.push_back(std::make_shared<ValueExpressionElement>(ValueExpressionElement(std::make_shared<StringValue>(StringValue("bag")))));
    pst2.push_back(std::make_shared<ValueExpressionElement>(ValueExpressionElement(std::make_shared<StringValue>(StringValue("bay")))));
    pst2.push_back(std::make_shared<LessEqualOperation>(LessEqualOperation()));

    EXPECT_EQ(exp2.Calculate(programState)->PrintValue(), "1");
}


// logical operations tests
TEST(ExpressionEvaluationTest, AndOperationTrue_BinaryArguments) {
    auto result = evaluateBinaryOp<AndOperation>(
        std::make_shared<IntValue>(1),
        std::make_shared<IntValue>(1)
    );
    EXPECT_EQ(dynamic_cast<IntValue*>(result.get())->value, 1);
}

TEST(ExpressionEvaluationTest, AndOperationTrue_IntArguments) {
    auto result = evaluateBinaryOp<AndOperation>(
        std::make_shared<IntValue>(1),
        std::make_shared<IntValue>(567)
    );
    EXPECT_EQ(dynamic_cast<IntValue*>(result.get())->value, 1);
}

TEST(ExpressionEvaluationTest, OrOperationTrue_BinaryArguments) {
    auto result = evaluateBinaryOp<OrOperation>(
        std::make_shared<IntValue>(0),
        std::make_shared<IntValue>(1)
    );
    EXPECT_EQ(dynamic_cast<IntValue*>(result.get())->value, 1);
}

TEST(ExpressionEvaluationTest, OrOperationTrue_IntArguments) {
    auto result = evaluateBinaryOp<OrOperation>(
        std::make_shared<IntValue>(1),
        std::make_shared<IntValue>(567)
    );
    EXPECT_EQ(dynamic_cast<IntValue*>(result.get())->value, 1);
}

TEST(ExpressionEvaluationTest, NotOperationFalse_BinaryArguments) {
    auto result = evaluateUnaryOp<NotOperation>(
        std::make_shared<IntValue>(0)
    );
    EXPECT_EQ(dynamic_cast<IntValue*>(result.get())->value, 1);
}

TEST(ExpressionEvaluationTest, NotOperationTrue_BinaryArguments) {
    auto result = evaluateUnaryOp<NotOperation>(
        std::make_shared<IntValue>(1)
    );
    EXPECT_EQ(dynamic_cast<IntValue*>(result.get())->value, 0);
}

TEST(ExpressionEvaluationTest, NotOperationTrue_IntArguments) {
    auto result = evaluateUnaryOp<NotOperation>(
        std::make_shared<IntValue>(21345)
    );
    EXPECT_EQ(dynamic_cast<IntValue*>(result.get())->value, 0);
}

TEST(ExpressionEvaluationTest, OrOperationFalse_BinaryArguments) {
    auto result = evaluateBinaryOp<OrOperation>(
        std::make_shared<IntValue>(0),
        std::make_shared<IntValue>(0)
    );
    EXPECT_EQ(dynamic_cast<IntValue*>(result.get())->value, 0);
}


// Unary operations tests
TEST(ExpressionEvaluationTest, SinOperationInt) {
    auto result = evaluateUnaryOp<SinOperation>(
        std::make_shared<IntValue>(0)
    );
    EXPECT_DOUBLE_EQ(dynamic_cast<DoubleValue*>(result.get())->value, 0.0);
}

TEST(ExpressionEvaluationTest, SinOperationDouble) {
    auto result = evaluateUnaryOp<SinOperation>(
        std::make_shared<DoubleValue>(3.14159265358979323846 / 2)  // ?/2
    );
    EXPECT_NEAR(dynamic_cast<DoubleValue*>(result.get())->value, 1.0, 1e-9);
}


// Complex expressions tests
TEST(ExpressionEvaluationTest, ComplexExpression1) {
    std::vector<std::shared_ptr<ExpressionElement>> postfix;
    postfix.push_back(std::make_shared<ValueExpressionElement>(std::make_shared<IntValue>(5)));
    postfix.push_back(std::make_shared<ValueExpressionElement>(std::make_shared<IntValue>(3)));
    postfix.push_back(std::make_shared<AddOperation>());
    postfix.push_back(std::make_shared<ValueExpressionElement>(std::make_shared<IntValue>(2)));
    postfix.push_back(std::make_shared<MultiplyOperation>());

    Expression exp(postfix);
    auto result = exp.Calculate(programState);
    EXPECT_EQ(dynamic_cast<IntValue*>(result.get())->value, 16);
}

TEST(ExpressionEvaluationTest, ComplexExpression2) {
    std::vector<std::shared_ptr<ExpressionElement>> postfix;
    postfix.push_back(std::make_shared<ValueExpressionElement>(std::make_shared<IntValue>(10)));
    postfix.push_back(std::make_shared<ValueExpressionElement>(std::make_shared<IntValue>(2)));
    postfix.push_back(std::make_shared<DivideOperation>());
    postfix.push_back(std::make_shared<ValueExpressionElement>(std::make_shared<IntValue>(3)));
    postfix.push_back(std::make_shared<ModOperation>());

    Expression exp(postfix);
    auto result = exp.Calculate(programState);
    EXPECT_EQ(dynamic_cast<IntValue*>(result.get())->value, 2);
}

TEST(ExpressionEvaluationTest, ComplexExpressionWithStrings) {
    std::vector<std::shared_ptr<ExpressionElement>> postfix;
    postfix.push_back(std::make_shared<ValueExpressionElement>(std::make_shared<StringValue>("a")));
    postfix.push_back(std::make_shared<ValueExpressionElement>(std::make_shared<IntValue>(3)));
    postfix.push_back(std::make_shared<MultiplyOperation>());
    postfix.push_back(std::make_shared<ValueExpressionElement>(std::make_shared<StringValue>("b")));
    postfix.push_back(std::make_shared<AddOperation>());

    Expression exp(postfix);
    auto result = exp.Calculate(programState);
    EXPECT_EQ(dynamic_cast<StringValue*>(result.get())->value, "aaab");
}


// Error cases (should throw exceptions)
TEST(ExpressionEvaluationTest, InvalidOperationThrows) {
    std::vector<std::shared_ptr<ExpressionElement>> postfix;
    postfix.push_back(std::make_shared<ValueExpressionElement>(std::make_shared<StringValue>("a")));
    postfix.push_back(std::make_shared<ValueExpressionElement>(std::make_shared<IntValue>(3)));
    postfix.push_back(std::make_shared<SubtractOperation>());  // String - Int is invalid

    Expression exp(postfix);
    EXPECT_THROW(exp.Calculate(programState), std::exception);
}

TEST(ExpressionEvaluationTest, EmptyExpressionThrows) {
    std::vector<std::shared_ptr<ExpressionElement>> postfix;
    Expression exp(postfix);
    ASSERT_ANY_THROW(exp.Calculate(programState));
}

//TEST(ExpressionEvaluationTest, OneElemnetForBinaryOperationThrows) {
//    std::vector<std::shared_ptr<ExpressionElement>> postfix;
//    postfix.push_back(std::make_shared<ValueExpressionElement>(std::make_shared<StringValue>("a")));
//    postfix.push_back(std::make_shared<SubtractOperation>());
//
//    Expression exp(postfix);
//    EXPECT_THROW(exp.Calculate(programState), std::runtime_error);
//}
//
//TEST(ExpressionEvaluationTest, ZeroElemnetsForUnaryOperationThrows) {
//    std::vector<std::shared_ptr<ExpressionElement>> postfix;
//    postfix.push_back(std::make_shared<SinOperation>());
//
//    Expression exp(postfix);
//    EXPECT_THROW(exp.Calculate(programState), std::runtime_error);
//}
