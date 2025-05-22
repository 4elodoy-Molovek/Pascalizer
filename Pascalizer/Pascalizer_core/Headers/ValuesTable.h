#pragma once
#include <memory>
#include <string>
#include <exception>

// Lists all posible types of values
enum Type {INT, DOUBLE, STRING};

class Value 
{
protected:
    // Type of this value
    Type type = INT;

public:

    Value() {}
    virtual ~Value() {}

    const Type& GetType() { return type; }

    virtual std::shared_ptr<Value> Add(const Value& rhs) const { throw std::exception("Addition not supported"); }
    virtual std::shared_ptr<Value> Subtract(const Value& rhs) const { throw std::exception("Subtraction not supported"); }
    virtual std::shared_ptr<Value> Multiply(const Value& rhs) const { throw std::exception("Multiplication not supported"); }
    virtual std::shared_ptr<Value> Divide(const Value& rhs) const { throw std::exception("Division not supported"); }
    virtual std::shared_ptr<Value> Mod(const Value& rhs) const { throw std::exception("Modulo not supported"); }
    virtual std::shared_ptr<Value> Div(const Value& rhs) const { throw std::exception("Div not supported"); }
    virtual std::shared_ptr<Value> USin() const { throw std::exception("Sin not supported"); }
    virtual std::string PrintValue() { throw std::exception("Print not supported"); }
};

// Operators for syntactic sugar
std::shared_ptr<Value> operator+(const Value& lhs, const Value& rhs);
std::shared_ptr<Value> operator-(const Value& lhs, const Value& rhs);
std::shared_ptr<Value> operator*(const Value& lhs, const Value& rhs);
std::shared_ptr<Value> operator/(const Value& lhs, const Value& rhs);
std::shared_ptr<Value> operator%(const Value& lhs, const Value& rhs);
std::shared_ptr<Value> usin(const Value& lhs);



class IntValue : public Value {
public:
    int value;

    IntValue(int initialValue);

    std::shared_ptr<Value> Add(const Value& rhs) const override;
    std::shared_ptr<Value> Subtract(const Value& rhs) const override;
    std::shared_ptr<Value> Multiply(const Value& rhs) const override;
    std::shared_ptr<Value> Divide(const Value& rhs) const override;
    std::shared_ptr<Value> Mod(const Value& rhs) const override;

    std::shared_ptr<Value> USin() const override;
    std::string PrintValue() { return std::to_string(value); }

};



class DoubleValue : public Value {
public:
    double value;

    DoubleValue(double initialValue);

    std::shared_ptr<Value> Add(const Value& rhs) const override;
    std::shared_ptr<Value> Subtract(const Value& rhs) const override;
    std::shared_ptr<Value> Multiply(const Value& rhs) const override;
    std::shared_ptr<Value> Divide(const Value& rhs) const override;

    std::shared_ptr<Value> USin() const override;
    std::string PrintValue() { return std::to_string(value); }

};



class StringValue : public Value {
public:
    std::string value;

    StringValue(std::string initialValue);

    std::shared_ptr<Value> Add(const Value& rhs) const override;
    std::shared_ptr<Value> Multiply(const Value& rhs) const override;
    std::string PrintValue() { return value; }

};


