#include "ChargeObject.h"

ChargeObject::ChargeObject(char *name){ this->name = name; }

ChargeObject::~ChargeObject(){}

char* ChargeObject::getName(){ return name; }

void ChargeObject::display()
{
    GLMmodel *model = glmReadOBJ(name);
   //glColor3f(1.0, 0.0, 1.0);
    glmDraw(model, GLM_SMOOTH);
}
