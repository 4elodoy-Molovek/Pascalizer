#include "./ValuesTable.h"
#include <stdexcept>


std::shared_ptr<Value> operator+(const Value& lhs, const Value& rhs) { return lhs.Add(rhs); }
std::shared_ptr<Value> operator-(const Value& lhs, const Value& rhs) { return lhs.Subtract(rhs); }
std::shared_ptr<Value> operator*(const Value& lhs, const Value& rhs) { return lhs.Multiply(rhs); }
std::shared_ptr<Value> operator/(const Value& lhs, const Value& rhs) { return lhs.Divide(rhs); }
std::shared_ptr<Value> operator%(const Value& lhs, const Value& rhs) { return lhs.Mod(rhs); }

std::shared_ptr<Value> div(const Value& lhs, const Value& rhs) { return lhs.Div(rhs); }



std::shared_ptr<Value> usin(const Value& lhs) { return lhs.USin(); }



IntValue::IntValue(int initialValue) : value(initialValue) {}

std::shared_ptr<Value> IntValue::Add(const Value& rhs) const {
    if (auto rhs_int = dynamic_cast<const IntValue*>(&rhs))
        return std::make_shared<IntValue>(value + rhs_int->value);

    if (auto rhs_double = dynamic_cast<const DoubleValue*>(&rhs))
        return std::make_shared<DoubleValue>(value + rhs_double->value);

    throw std::runtime_error("Unsupported Add operands");
}

std::shared_ptr<Value> IntValue::Subtract(const Value& rhs) const {
    if (auto rhs_int = dynamic_cast<const IntValue*>(&rhs))
        return std::make_shared<IntValue>(value - rhs_int->value);

    if (auto rhs_double = dynamic_cast<const DoubleValue*>(&rhs))
        return std::make_shared<DoubleValue>(value - rhs_double->value);

    throw std::runtime_error("Unsupported Subtract operands");
}

std::shared_ptr<Value> IntValue::Multiply(const Value& rhs) const {
    if (auto rhs_int = dynamic_cast<const IntValue*>(&rhs))
        return std::make_shared<IntValue>(value * rhs_int->value);

    if (auto rhs_double = dynamic_cast<const DoubleValue*>(&rhs))
        return std::make_shared<DoubleValue>(value * rhs_double->value);

    if (auto rhs_str = dynamic_cast<const StringValue*>(&rhs)) {
        std::string result;
        for (int i = 0; i < value; ++i)
            result += rhs_str->value;
        return std::make_shared<StringValue>(result);
    }

    throw std::runtime_error("Unsupported Multiply operands");
}

std::shared_ptr<Value> IntValue::Divide(const Value& rhs) const {
    if (auto rhs_int = dynamic_cast<const IntValue*>(&rhs))
        return std::make_shared<IntValue>(value / rhs_int->value);

    if (auto rhs_double = dynamic_cast<const DoubleValue*>(&rhs))
        return std::make_shared<DoubleValue>(value / rhs_double->value);

    throw std::runtime_error("Unsupported Divide operands");
}

std::shared_ptr<Value> IntValue::Mod(const Value& rhs) const {
    if (auto rhs_int = dynamic_cast<const IntValue*>(&rhs))
        return std::make_shared<IntValue>(value % rhs_int->value);

    if (auto rhs_double = dynamic_cast<const DoubleValue*>(&rhs))
        return std::make_shared<DoubleValue>(fmod(double(value), rhs_double->value));

    throw std::runtime_error("Unsupported Divide operands");
}

//t
std::shared_ptr<Value> IntValue::Div(const Value& rhs) const {
    if (auto rhs_int = dynamic_cast<const IntValue*>(&rhs))
        return std::make_shared<IntValue>(int(value / rhs_int->value));

    if (auto rhs_double = dynamic_cast<const DoubleValue*>(&rhs))
        return std::make_shared<IntValue>(int(value / rhs_double->value));

    throw std::runtime_error("Unsupported Div operands");
}



std::shared_ptr<Value> IntValue::USin() const {
    return std::make_shared<DoubleValue>(sin(value));
}


DoubleValue::DoubleValue(double initialValue) : value(initialValue) {}

std::shared_ptr<Value> DoubleValue::Add(const Value& rhs) const {
    if (auto rhs_int = dynamic_cast<const IntValue*>(&rhs))
        return std::make_shared<DoubleValue>(value + rhs_int->value);

    if (auto rhs_double = dynamic_cast<const DoubleValue*>(&rhs))
        return std::make_shared<DoubleValue>(value + rhs_double->value);

    throw std::runtime_error("Unsupported Add operands");
}

std::shared_ptr<Value> DoubleValue::Subtract(const Value& rhs) const {
    if (auto rhs_int = dynamic_cast<const IntValue*>(&rhs))
        return std::make_shared<DoubleValue>(value - rhs_int->value);

    if (auto rhs_double = dynamic_cast<const DoubleValue*>(&rhs))
        return std::make_shared<DoubleValue>(value - rhs_double->value);

    throw std::runtime_error("Unsupported Subtract operands");
}

std::shared_ptr<Value> DoubleValue::Multiply(const Value& rhs) const {
    if (auto rhs_int = dynamic_cast<const IntValue*>(&rhs))
        return std::make_shared<DoubleValue>(value * rhs_int->value);

    if (auto rhs_double = dynamic_cast<const DoubleValue*>(&rhs))
        return std::make_shared<DoubleValue>(value * rhs_double->value);

    throw std::runtime_error("Unsupported Multiply operands");
}

std::shared_ptr<Value> DoubleValue::Divide(const Value& rhs) const {
    if (auto rhs_int = dynamic_cast<const IntValue*>(&rhs))
        return std::make_shared<DoubleValue>(value / rhs_int->value);

    if (auto rhs_double = dynamic_cast<const DoubleValue*>(&rhs))
        return std::make_shared<DoubleValue>(value / rhs_double->value);

    throw std::runtime_error("Unsupported Divide operands");
}

std::shared_ptr<Value> DoubleValue::USin() const {
    return std::make_shared<DoubleValue>(sin(value));
}


StringValue::StringValue(std::string initialValue) : value(std::move(initialValue)) {}

std::shared_ptr<Value> StringValue::Add(const Value& rhs) const {
    if (auto rhs_str = dynamic_cast<const StringValue*>(&rhs))
        return std::make_shared<StringValue>(value + rhs_str->value);

    throw std::runtime_error("Unsupported Add for StringValue");
}

std::shared_ptr<Value> StringValue::Multiply(const Value& rhs) const {
    if (auto rhs_int = dynamic_cast<const IntValue*>(&rhs)) {
        std::string result;
        for (int i = 0; i < rhs_int->value; ++i)
            result += value;
        return std::make_shared<StringValue>(result);
    }

    throw std::runtime_error("Unsupported Multiply for StringValue");
}
