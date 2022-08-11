#include <iostream>
#include <thread>

#include "./include/BigBoilerPlate.h"

int main()
{
    //Maybe insert threading here, so that we finish actions 
    //This won;t corrupt the data
    BoilerPlate bp;
    bp.take_input();

    bp.create_system();

    return 0;
}