#ifndef CONNECTION_H
#define CONNECTION_H

class Neuron;


class Connection
{
public:
    Connection();

    double weight() const;
    void setWeight(double newWeight);
    double momentum() const;
    void setMomentum(double newMomentum);

private:

    double mWeight;
    double mMomentum;
};

#endif // CONNECTION_H
