#include "connection.h"

Connection::Connection()
{

}

double Connection::momentum() const
{
    return mMomentum;
}

void Connection::setMomentum(double newMomentum)
{
    mMomentum = newMomentum;
}

double Connection::weight() const
{
    return mWeight;
}

void Connection::setWeight(double newWeight)
{
    mWeight = newWeight;
}

