#pragma once
#include "glm.h"

class ChargeObject
{

    private:
        char *name;

    public:
        ChargeObject(char *);
        ~ChargeObject();
        char*  getName();
        void display();
};
