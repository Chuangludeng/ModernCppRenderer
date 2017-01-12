#pragma once
#include "math.h"
#include <vector>

struct Model {
    std::vector<uint16_t> indices;
    std::vector<vector_t> vertices;
};


//TODO: implement ModelLoader
/*
this is a basic obj loader. 
you can use modern c++'s move semantics to make it more efficient.
*/

class ModelLoader {
public:
    void load(std::string file, Model &model);//signature may changed to make use of modern c++ syntax
    //TODO: implement the rest...
};