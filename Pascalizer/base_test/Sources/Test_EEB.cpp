//#include "gtest/gtest.h"
//#include "ExpressionEvaluationBlock.h"
//
//TEST(EEB, eeb_string_equal)
//{
//	std::vector<std::shared_ptr<ExpressionElement>> pst2;
//	
//	pst2.push_back(std::make_shared<ValueExpressionElement>(ValueExpressionElement(std::make_shared<StringValue>(StringValue("baa")))));
//	pst2.push_back(std::make_shared<ValueExpressionElement>(ValueExpressionElement(std::make_shared<StringValue>(StringValue("bag")))));
//	pst2.push_back(std::make_shared<MoreOperation>(MoreOperation()));
//	Expression exp2(pst2);
//	
//	EXPECT_EQ(exp2.Caculate()->PrintValue(), "0");
//}
//
//TEST(EEB, string_not_equal)
//{
//	std::vector<std::shared_ptr<ExpressionElement>> pst2;
//	
//	pst2.push_back(std::make_shared<ValueExpressionElement>(ValueExpressionElement(std::make_shared<StringValue>(StringValue("baa")))));
//	pst2.push_back(std::make_shared<ValueExpressionElement>(ValueExpressionElement(std::make_shared<StringValue>(StringValue("bag")))));
//	pst2.push_back(std::make_shared<LessOperation>(LessOperation()));
//	Expression exp2(pst2);
//	
//	EXPECT_EQ(exp2.Caculate()->PrintValue(), "1");
//}
//
//
//TEST(EEB, string_more_equal)
//{
//	std::vector<std::shared_ptr<ExpressionElement>> pst2;
//	
//	pst2.push_back(std::make_shared<ValueExpressionElement>(ValueExpressionElement(std::make_shared<StringValue>(StringValue("bag")))));
//	pst2.push_back(std::make_shared<ValueExpressionElement>(ValueExpressionElement(std::make_shared<StringValue>(StringValue("bag")))));
//	pst2.push_back(std::make_shared<MoreEqualOperation>(MoreEqualOperation()));
//	Expression exp2(pst2);
//	
//	EXPECT_EQ(exp2.Caculate()->PrintValue(), "1");
//	
//	pst2.clear();
//	
//	std::vector<std::shared_ptr<ExpressionElement>> pst2;
//	
//	pst2.push_back(std::make_shared<ValueExpressionElement>(ValueExpressionElement(std::make_shared<StringValue>(StringValue("bag")))));
//	pst2.push_back(std::make_shared<ValueExpressionElement>(ValueExpressionElement(std::make_shared<StringValue>(StringValue("bay")))));
//	pst2.push_back(std::make_shared<MoreEqualOperation>(MoreEqualOperation()));
//	Expression exp2(pst2);
//	
//	EXPECT_EQ(exp2.Caculate()->PrintValue(), "1");
//}
//
//TEST(EEB, string_less_equal)
//{
//	std::vector<std::shared_ptr<ExpressionElement>> pst2;
//	
//	pst2.push_back(std::make_shared<ValueExpressionElement>(ValueExpressionElement(std::make_shared<StringValue>(StringValue("bag")))));
//	pst2.push_back(std::make_shared<ValueExpressionElement>(ValueExpressionElement(std::make_shared<StringValue>(StringValue("bag")))));
//	pst2.push_back(std::make_shared<LessEqualOperation>(LessEqualOperation()));
//	Expression exp2(pst2);
//	
//	EXPECT_EQ(exp2.Caculate()->PrintValue(), "1");
//	
//	pst2.clear();
//	
//	std::vector<std::shared_ptr<ExpressionElement>> pst2;
//	
//	pst2.push_back(std::make_shared<ValueExpressionElement>(ValueExpressionElement(std::make_shared<StringValue>(StringValue("bag")))));
//	pst2.push_back(std::make_shared<ValueExpressionElement>(ValueExpressionElement(std::make_shared<StringValue>(StringValue("bay")))));
//	pst2.push_back(std::make_shared<LessEqualOperation>(LessEqualOperation));
//	Expression exp2(pst2);
//	
//	EXPECT_EQ(exp2.Caculate()->PrintValue(), "0");
//}