#include <fstream>
#include <cmath>

#include "G4CascadeTF.hh"
#include "G4Gamma.hh"
#include "G4Positron.hh"
#include "G4Electron.hh"
#include "MCTools.hh"



G4CascadeTF::G4CascadeTF(const G4String& name, G4ProcessType type):
G4VDiscreteProcess(name, type), m_timeLag(0)
{
    m_units = new UnitsSystem("SI");
    SetProcessSubType(1);
}

G4CascadeTF::~G4CascadeTF()
{
    delete m_elecGraph;
    delete m_posiGraph;
    delete m_photGraph;
    delete m_photCountGraph;
    delete m_g4_part;
}

void G4CascadeTF::LoadGraphs(const std::string& seedPath,
        const std::string& elecPath, const std::string& photPath)
{
    // Set output vecotrs
    std::vector<std::string> elecOut = {"frac", "mean", "sigma", "skew"};
    std::vector<std::string> photOut = {"frac", "mean", "sigma", "skew"};
    std::vector<std::string> photCountOut = {"mean"};
    // Load the tensorflow graphs
    m_elecGraph = new TensorflowGraph(elecPath + "/param_graph.pb", 6,
                                      elecOut);
    m_photGraph = new TensorflowGraph(photPath + "/param_graph.pb", 6, 
                                      photOut);
    m_photCountGraph = new TensorflowGraph(photPath + "/count_graph.pb", 6,
                                          photCountOut);
    // Load the meta data
    std::ifstream inMetaFile(seedPath + "/in_meta.dat");
    std::ifstream elecMetaFile(elecPath + "/meta.dat");
    std::ifstream photMetaFile(photPath + "/meta.dat");

    for (unsigned int i = 0; i < 6; i++)
    {
        inMetaFile >> m_inParams[i];
    }
    for (unsigned int i = 0; i < 3; i++)
    {
        elecMetaFile >> m_elecParmas[i];
        photMetaFile >> m_photParams[i];
    }
}

G4bool G4CascadeTF::IsApplicable(const G4ParticleDefinition& particle)
{
    return (&particle == G4Electron::Electron());
}

G4double G4CascadeTF::GetMeanFreePath(const G4Track& track, G4double,
                                             G4ForceCondition*)
{
    // check if inside laser field
    G4Material* material = track.GetMaterial();
    double inField = material->GetMaterialPropertiesTable()->GetConstProperty("Laser Field");
    if (inField < 1.0)
    {
        return 1e99;
    }
    if (m_pSwitch == false)
    {
        m_pSwitch = true;
        return 1e-99;
    } else
    {
        m_pSwitch = false;
        return 1e99;
    }
}

G4VParticleChange* G4CascadeTF::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep)
{
    // Get the particle properties and normilise
    aParticleChange.Initialize(aTrack);
    G4ThreeVector momentum = aTrack.GetMomentum() * m_units->G4Energy();
    G4ThreeVector position = aTrack.GetPosition();
    double time = aTrack.GetProperTime();

    double test();

    std::vector<double> input(6);
    input[0] = momentum[0] / (m_inParams[0] + 1);
    input[1] = momentum[1] / (m_inParams[1] + 1);
    input[2] = momentum[2] / m_inParams[2];
    input[3] = position[0] / m_inParams[3];
    input[4] = position[1] / m_inParams[4];
    input[5] = (time - m_timeLag)  / m_inParams[5];

    // Run the graph
    std::vector<tensorflow::Tensor> elecTensor;
    std::vector<tensorflow::Tensor> photTensor;
    std::vector<tensorflow::Tensor> photCountTensor;
    m_elecGraph->runGraph(input, elecTensor);
    m_photGraph->runGraph(input, photTensor);

    m_photCountGraph->runGraph(input, photCountTensor);

    // Select the density component
    auto elecFrac = elecTensor[0].SubSlice(0);
    int nElecFracts = elecFrac.shape().dim_size(0);
    std::vector<double> electFracData(elecFrac.flat<double>().data(),
            elecFrac.flat<double>().data() + nElecFracts);

    int elecFracIndex = MCTools::RandDiscrete(electFracData);

    // Do a load of annoying data minipulation
    auto elecMean = elecTensor[1].SubSlice(0).matrix<double>().chip(elecFracIndex, 0);
    Eigen::Tensor<double, 1, Eigen::RowMajor> elecMeanTensor = elecMean.eval();
    auto elecCovar = elecTensor[2].SubSlice(0).matrix<double>().chip(elecFracIndex, 0);
    Eigen::Tensor<double, 1, Eigen::RowMajor> elecCovarTensor = elecCovar.eval();
    auto elecSkew = elecTensor[3].SubSlice(0).matrix<double>().chip(elecFracIndex, 0);
    Eigen::Tensor<double, 1, Eigen::RowMajor> elecSkewTensor = elecSkew.eval();
    Eigen::Map<Eigen::VectorXd> elecMeanVec(elecMeanTensor.data(), 3);
    Eigen::Map<Eigen::VectorXd> elecCovarVec(elecCovarTensor.data(), 3);
    Eigen::Map<Eigen::VectorXd> elecSkewVec(elecSkewTensor.data(), 3);
    Eigen::VectorXd tf_elecP = MCTools::RandSinhArcsinhNd(elecMeanVec,
                                                          elecCovarVec,
                                                          elecSkewVec);

    // Sample the number of photons
    double meanPhotons = *photCountTensor[0].flat<double>().data();
    unsigned long int nPhotons = MCTools::RandPoisson(meanPhotons);
    
    aParticleChange.SetNumberOfSecondaries(nPhotons);
    // Do the same all over again for photons
    for (unsigned long int i = 0; i < nPhotons; i++)
    {
        auto photFrac = photTensor[0].SubSlice(0);
        int nPhotFracts = photFrac.shape().dim_size(0);
        std::vector<double> photFracData(photFrac.flat<double>().data(),
                photFrac.flat<double>().data() + nPhotFracts);

        int photFracIndex = MCTools::RandDiscrete(electFracData);

        // Do a load of annoying data minipulation
        auto photMean = photTensor[1].SubSlice(0).matrix<double>().chip(photFracIndex, 0);
        Eigen::Tensor<double, 1, Eigen::RowMajor> photMeanTensor = photMean.eval();
        auto photCovar = photTensor[2].SubSlice(0).matrix<double>().chip(photFracIndex, 0);
        Eigen::Tensor<double, 1, Eigen::RowMajor> photCovarTensor = photCovar.eval();
        auto photSkew = photTensor[3].SubSlice(0).matrix<double>().chip(photFracIndex, 0);
        Eigen::Tensor<double, 1, Eigen::RowMajor> photSkewTensor = photSkew.eval();
        Eigen::Map<Eigen::VectorXd> photMeanVec(photMeanTensor.data(), 3);
        Eigen::Map<Eigen::VectorXd> photCovarVec(photCovarTensor.data(), 3);
        Eigen::Map<Eigen::VectorXd> photSkewVec(photSkewTensor.data(), 3);
        Eigen::VectorXd tf_photMomentum = MCTools::RandSinhArcsinhNd(photMeanVec, 
                photCovarVec, photSkewVec);
        G4ThreeVector g_photMomentum = G4ThreeVector(tf_elecP[0] * m_photParams[0],
                                                     tf_elecP[1] * m_photParams[1], 
                                                     tf_elecP[2] * m_photParams[2]);
        m_g4_part = new G4DynamicParticle(G4Gamma::Gamma(), g_photMomentum.unit(),
                                          g_photMomentum.mag2() / m_units->G4Energy());
        aParticleChange.AddSecondary(m_g4_part);
    }

    G4ThreeVector g_elecMomentum = G4ThreeVector(tf_elecP[0] * m_elecParmas[0],
                                                 tf_elecP[1] * m_elecParmas[1], 
                                                 tf_elecP[2] * m_elecParmas[2]);
    G4double elecEnergy = std::sqrt(g_elecMomentum.mag2() + 1.0);
    aParticleChange.ProposeMomentumDirection(g_elecMomentum.unit());
    aParticleChange.ProposeEnergy(elecEnergy / m_units->G4Energy());

    return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
}

void G4CascadeTF::test()
{
     // Set up the tf graphs
    std::vector<double> input(6);
    input[0] = 0.0;
    input[1] = 0.0;
    input[2] = 0.8;
    input[3] = 0.5;
    input[4] = 0.5;
    input[5] = 1.0;

    // Run the graph
    std::vector<tensorflow::Tensor> elecTensor;
    std::vector<tensorflow::Tensor> photTensor;
    std::vector<tensorflow::Tensor> photCountTensor;
    m_elecGraph->runGraph(input, elecTensor);
    m_photGraph->runGraph(input, photTensor);
    m_photCountGraph->runGraph(input, photCountTensor);


    // Select the density component
    auto elecFrac = elecTensor[0].SubSlice(0);
    int nElecFracts = elecFrac.shape().dim_size(0);
    std::vector<double> electFracData(elecFrac.flat<double>().data(),
            elecFrac.flat<double>().data() + nElecFracts);

    int elecFracIndex = MCTools::RandDiscrete(electFracData);

    // Do a load of annoying data minipulation
    auto elecMean = elecTensor[1].SubSlice(0).matrix<double>().chip(elecFracIndex, 0);
    Eigen::Tensor<double, 1, Eigen::RowMajor> elecMeanTensor = elecMean.eval();
    auto elecCovar = elecTensor[2].SubSlice(0).matrix<double>().chip(elecFracIndex, 0);
    Eigen::Tensor<double, 1, Eigen::RowMajor> elecCovarTensor = elecCovar.eval();
    auto elecSkew = elecTensor[3].SubSlice(0).matrix<double>().chip(elecFracIndex, 0);
    Eigen::Tensor<double, 1, Eigen::RowMajor> elecSkewTensor = elecSkew.eval();
    Eigen::Map<Eigen::VectorXd> elecMeanVec(elecMeanTensor.data(), 3);
    Eigen::Map<Eigen::VectorXd> elecCovarVec(elecCovarTensor.data(), 3);
    Eigen::Map<Eigen::VectorXd> elecSkewVec(elecSkewTensor.data(), 3);
    MCTools::RandSinhArcsinhNd(elecMeanVec, elecCovarVec, elecSkewVec);

    // Sample the number of photons
    double meanPhotons = *photCountTensor[0].flat<double>().data();
    unsigned long int nPhotons = MCTools::RandPoisson(meanPhotons);
    
    // Do the same all over again for photons
    for (unsigned long int i = 0; i < nPhotons; i++)
    {
        auto photFrac = photTensor[0].SubSlice(0);
        int nPhotFracts = photFrac.shape().dim_size(0);
        std::vector<double> photFracData(photFrac.flat<double>().data(),
                photFrac.flat<double>().data() + nPhotFracts);

        int photFracIndex = MCTools::RandDiscrete(electFracData);

        // Do a load of annoying data minipulation
        auto photMean = photTensor[1].SubSlice(0).matrix<double>().chip(photFracIndex, 0);
        Eigen::Tensor<double, 1, Eigen::RowMajor> photMeanTensor = photMean.eval();
        auto photCovar = photTensor[2].SubSlice(0).matrix<double>().chip(photFracIndex, 0);
        Eigen::Tensor<double, 1, Eigen::RowMajor> photCovarTensor = photCovar.eval();
        auto photSkew = photTensor[3].SubSlice(0).matrix<double>().chip(photFracIndex, 0);
        Eigen::Tensor<double, 1, Eigen::RowMajor> photSkewTensor = photSkew.eval();
        Eigen::Map<Eigen::VectorXd> photMeanVec(photMeanTensor.data(), 3);
        Eigen::Map<Eigen::VectorXd> photCovarVec(photCovarTensor.data(), 3);
        Eigen::Map<Eigen::VectorXd> photSkewVec(photSkewTensor.data(), 3);
        MCTools::RandSinhArcsinhNd(photMeanVec, photCovarVec, photSkewVec);
    }
}
