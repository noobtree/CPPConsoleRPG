#include "UObject.h"
#include <boost/uuid/generators.hpp>    // random_ganerator

Unreal::UObject::UObject()
{
    static boost::uuids::random_generator generator;
    this->uid = generator();
}

bool Unreal::UObject::operator==(const UObject& other)
{
    return this->uid == other.uid;
}

bool Unreal::UObject::operator!=(const UObject& other)
{
    return this->uid != other.uid;
}
