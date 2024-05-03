#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <cstddef>

//Reference: https://skypjack.github.io/2019-02-14-ecs-baf-part-1/ 
/// Family Generator
/// @brief Used to generate unique id's for component types and their positions in vectors
class Generator {
private:
    //Used to generate the index associated with the component type
    static size_t Identifier() {
        static size_t value = 0;
        return value++;
    }
public:
    //Templated so it makes a copy of this function for each different type
    //When an instance of the template is created, it takes in the current value of identifier() and stores it as a static local variable, 
    //meaning the value will carry over when the same type is used again.
    template<typename>
    static size_t TypeID() {
        static const size_t value = Identifier();
        return value;
    }
};

#endif