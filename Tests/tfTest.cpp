#include "TensorflowGraph.hh"
#include <iostream>
#include "G4CascadeTF.hh"
#include <omp.h>

int main(int argc, char* argv[])
{

    G4CascadeTF* test = new G4CascadeTF();
    std::string in_path = "/home/robbie/Documents/QED-Cascade/Tables/TensorflowGraphs";
    std::string ele_path = "/home/robbie/Documents/QED-Cascade/Tables/TensorflowGraphs/ElectronFiles";
    std::string pho_path = "/home/robbie/Documents/QED-Cascade/Tables/TensorflowGraphs/GammaFiles";

    test->LoadGraphs(in_path, ele_path, pho_path);

    double startTime = omp_get_wtime();
    
  //  #pragma omp parallel for
    for(int i = 0; i < 10000; i++)
    {
    	test->test();
    }
    std::cout << omp_get_wtime() - startTime << std::endl;
    return 0;
}
