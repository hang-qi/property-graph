#include <iostream>
#include "property.h"


int main(int argc, char const *argv[])
{
    std::cout << "Construct a dict: " << std::endl;
    PropertyDict dict;
    dict["name"] = make_value<std::string>("Hello");
    dict["height"] = make_value<int>(1);
    dict["weight"] = make_value<double>(4.5);
    dict["boolean"] = make_value<bool>(false);

    // Retrieve data:
    std::cout << "Retrieve Data using helper functions: " << std::endl;
    std::cout << get_value<std::string>(dict["name"]) << std::endl;
    std::cout << get_value<int>(dict["height"]) << std::endl;
    std::cout << get_value<double>(dict["weight"]) << std::endl;
    std::cout << get_value<bool>(dict["boolean"]) << std::endl;

    // Simpler way to retrieve data:
    std::cout << "Retrieve Data using >> operator: " << std::endl;
    double d;
    dict["weight"] >> d;
    std::cout << d << std::endl;

    std::string name;
    dict["name"] >> name;
    std::cout << name << std::endl;

    // Print out property
    std::cout << "Print property values: " << std::endl;
    std::cout << dict["name"] << std::endl;
    std::cout << dict["height"] << std::endl;
}
