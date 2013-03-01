#pragma once
#ifndef MMLT_CPLEX_CONCERT_H
#define MMLT_CPLEX_CONCERT_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <ilcplex/ilocplex.h>
#pragma GCC diagnostic pop

class CPLEX :
        public IloCplex
{
public:
    CPLEX() :
        IloCplex(IloEnv())
    {
        this->setOut(this->getEnv().getNullStream());
        this->setWarning(this->getEnv().getNullStream());
        //this->setError(this->getEnv().getNullStream());

        this->setParam(IloCplex::RootAlg, IloCplex::AutoAlg);
        this->setParam(IloCplex::SimDisplay, 2);
    }
};

#endif // MMLT_CPLEX_CONCERT_H
