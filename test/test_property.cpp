#include <iostream>
#include "property.h"


int main(int argc, char const *argv[])
{
    PropertyDict dict;
    dict["name"] = make_value<std::string>("Hello");
    dict["height"] = make_value<int>(1);
    dict["weight"] = make_value<double>(4.5);
    dict["boolean"] = make_value<bool>(false);

    // Retrieve data:
    std::cout << get_value<std::string>(dict["name"]) << std::endl;
    std::cout << get_value<int>(dict["height"]) << std::endl;
    std::cout << get_value<double>(dict["weight"]) << std::endl;
    std::cout << get_value<bool>(dict["boolean"]) << std::endl;

    // Simpler way to retrieve data:
    double d;
    dict["weight"] >> d;
    std::cout << d << std::endl;

    std::string name;
    dict["name"] >> name;
    std::cout << name << std::endl;
}
