#pragma once
#include <string>
#include <memory>


class Value
{

public:

	Value() {}
	virtual ~Value() {}


	// Operator methods
	virtual std::shared_ptr<Value> Add(const Value& rhs)		const	{ throw(std::exception("ERROR: the type(s) does not support addition!")); }
	virtual std::shared_ptr<Value> Subtract(const Value& rhs)	const	{ throw(std::exception("ERROR: the type(s) does not support subtraction!")); }
	virtual std::shared_ptr<Value> Multiply(const Value& rhs)	const	{ throw(std::exception("ERROR: the type(s) does not support multiplication!")); }
	virtual std::shared_ptr<Value> Divide(const Value& rhs)		const	{ throw(std::exception("ERROR: the type(s) does not support division!")); }
	virtual std::shared_ptr<Value> Mod(const Value& rhs)		const	{ throw(std::exception("ERROR: the type(s) does not support division!")); }
	virtual std::shared_ptr<Value> Div(const Value& rhs)		const	{ throw(std::exception("ERROR: the type(s) does not support division!")); }

	// Add more if needed
};

// Operators for easier use
std::shared_ptr<Value> operator+(const Value& lhs, const Value& rhs) { return lhs.Add(rhs); }
std::shared_ptr<Value> operator-(const Value& lhs, const Value& rhs) { return lhs.Subtract(rhs); }
std::shared_ptr<Value> operator*(const Value& lhs, const Value& rhs) { return lhs.Multiply(rhs); }
std::shared_ptr<Value> operator/(const Value& lhs, const Value& rhs) { return lhs.Divide(rhs); }




// DATA TYPES


class IntValue : public Value
{
public:

	int value;

	IntValue(int initialValue): value(initialValue) {}

	virtual std::shared_ptr<Value> Add(const Value& rhs) const override
	{
		// Int case
		if (auto rhs_int = dynamic_cast<const IntValue*>(&rhs))
			return std::make_shared<IntValue>( IntValue(value + rhs_int->value) );

		// Double case
		if (auto rhs_double = dynamic_cast<const DoubleValue*>(&rhs))
			return std::make_shared<DoubleValue>( DoubleValue(value + rhs_double->value) );
	}

	virtual std::shared_ptr<Value> Subtract(const Value& rhs) const override
	{
		// Int case
		if (auto rhs_int = dynamic_cast<const IntValue*>(&rhs))
			return std::make_shared<IntValue>(IntValue(value - rhs_int->value));

		// Double case
		if (auto rhs_double = dynamic_cast<const DoubleValue*>(&rhs))
			return std::make_shared<DoubleValue>(DoubleValue(value - rhs_double->value));
	}

	virtual std::shared_ptr<Value> Multiply(const Value& rhs) const override
	{
		// Int case
		if (auto rhs_int = dynamic_cast<const IntValue*>(&rhs))
			return std::make_shared<IntValue>( IntValue(value * rhs_int->value) );

		// Double case
		if (auto rhs_double = dynamic_cast<const DoubleValue*>(&rhs))
			return std::make_shared<DoubleValue>( DoubleValue(value * rhs_double->value) );
		
		// String case
		if (auto rhs_string = dynamic_cast<const StringValue*>(&rhs))
		{
			StringValue newValue("");

			for (int i = 0; i < value; i++)
				newValue.value += rhs_string->value;

			return std::make_shared<StringValue>(newValue);
		}
	}

	virtual std::shared_ptr<Value> Divide(const Value& rhs) const override
	{
		// Int case
		if (auto rhs_int = dynamic_cast<const IntValue*>(&rhs))
			return std::make_shared<IntValue>(IntValue(value / rhs_int->value));

		// Double case
		if (auto rhs_double = dynamic_cast<const DoubleValue*>(&rhs))
			return std::make_shared<DoubleValue>(DoubleValue(value / rhs_double->value));
	}
};


class DoubleValue : public Value
{
public:

	double value;

	DoubleValue(double initialValue): value(initialValue) {}

	virtual std::shared_ptr<Value> Add(const Value& rhs) const override
	{
		// Int case
		if (auto rhs_int = dynamic_cast<const IntValue*>(&rhs))
			return std::make_shared<DoubleValue>(DoubleValue(value + rhs_int->value));

		// Double case
		if (auto rhs_double = dynamic_cast<const DoubleValue*>(&rhs))
			return std::make_shared<DoubleValue>(DoubleValue(value + rhs_double->value));
	}

	virtual std::shared_ptr<Value> Subtract(const Value& rhs) const override
	{
		// Int case
		if (auto rhs_int = dynamic_cast<const IntValue*>(&rhs))
			return std::make_shared<DoubleValue>(DoubleValue(value - rhs_int->value));

		// Double case
		if (auto rhs_double = dynamic_cast<const DoubleValue*>(&rhs))
			return std::make_shared<DoubleValue>(DoubleValue(value - rhs_double->value));
	}

	virtual std::shared_ptr<Value> Multiply(const Value& rhs) const override
	{
		// Int case
		if (auto rhs_int = dynamic_cast<const IntValue*>(&rhs))
			return std::make_shared<DoubleValue>(DoubleValue(value * rhs_int->value));

		// Double case
		if (auto rhs_double = dynamic_cast<const DoubleValue*>(&rhs))
			return std::make_shared<DoubleValue>(DoubleValue(value * rhs_double->value));
	}

	virtual std::shared_ptr<Value> Divide(const Value& rhs) const override
	{
		// Int case
		if (auto rhs_int = dynamic_cast<const IntValue*>(&rhs))
			return std::make_shared<DoubleValue>(DoubleValue(value / rhs_int->value));

		// Double case
		if (auto rhs_double = dynamic_cast<const DoubleValue*>(&rhs))
			return std::make_shared<DoubleValue>(DoubleValue(value / rhs_double->value));
	}
};


class StringValue : public Value
{
public:

	std::string value;

	StringValue(std::string initialValue) : value(initialValue) {}

	virtual std::shared_ptr<Value> Add(const Value& rhs) const override
	{
		// String case
		if (auto rhs_string = dynamic_cast<const StringValue*>(&rhs))
		{
			StringValue newValue("");

			newValue = value + rhs_string->value;

			return std::make_shared<StringValue>(newValue);
		}
	}

	virtual std::shared_ptr<Value> Multiply(const Value& rhs) const override
	{
		// Int case
		if (auto rhs_int = dynamic_cast<const IntValue*>(&rhs))
		{
			StringValue newValue("");

			for (int i = 0; i < rhs_int->value; i++)
				newValue.value += value;

			return std::make_shared<StringValue>(newValue);
		}
	}
};
