#include "QEDCascPy.hh"
#include "ThreeVector.hh"

int main()
{
    QEDCascPy* manager = new QEDCascPy();
    manager->setTime(0.000001, 1);
    manager->setField("Gaussian", 1., 1., 
        1., 1., 0, ThreeVector(0,0,-1), ThreeVector(0,0,0));
    manager->setPusher("Lorentz");
    manager->setGenerator(100, "Electron", "Gaussian", 1e28, 0.1, 0.1, 0.1, 0, 
        ThreeVector(0,0,1), ThreeVector(0,0,-1));
    manager->setPhysics(true, false);
    manager->beamOn(16);

    std::vector<double> output = manager->getOutput("Electron");

    std::cout << output.size() << std::endl;

    return 0;
}