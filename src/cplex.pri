# CPLEX
QMAKE_CXXFLAGS += -DIL_STD
INCLUDEPATH += /ibr/local/cplex/include/
INCLUDEPATH += /ibr/local/concert29/include/
LIBS += -L/ibr/local/cplex/lib/x86-64_debian4.0_4.1/static_pic/ -lilocplex -lcplex
LIBS += -L/ibr/local/concert29/lib/x86-64_debian4.0_4.1/static_pic/ -lconcert
