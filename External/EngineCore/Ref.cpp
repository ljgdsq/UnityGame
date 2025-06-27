#include "Ref.h"

void Ref::retain()
{
    ++referenceCount;
}

void Ref::release()
{
    --referenceCount;
    if (referenceCount == 0)
    {
        delete this;
    }
}

Ref *Ref::autoRelease()
{
    return this;
}

unsigned int Ref::getReferenceCount() const
{
    return referenceCount;
}

Ref::Ref()
    : referenceCount(1)
{
}
Ref::~Ref(){
    
}