#include <cmath>
#include <fstream>

#include "NonLinearCompton.hh"
#include "Photon.hh"
#include "Numerics.hh"
#include "MCTools.hh"
#include "UnitsSystem.hh"


NonLinearCompton::NonLinearCompton(EMField* field, double dt, double eMin,
    bool importance, bool track):
m_eMin(eMin), Process(field, dt, importance, track)
{
    LoadTables();
}

NonLinearCompton::~NonLinearCompton()
{
    UnloadTables();
}

void NonLinearCompton::Interact(Particle *part, ParticleList *partList) const
{
    if (part->GetMass() == 0 || part->IsAlive() == false) return;
    // First we need to update the optical depth of the particle based on 
    // local values
    double eta = CalculateEta(part);
    // Check for very small values of eta and skip interpolation
    double logh;
    if (eta < 1.0e-12)
    {
        logh = 5.24;
    } else
    {
        logh = Numerics::Interpolate1D(m_h_etaAxis, m_h_dataTable,
            m_h_length, std::log10(eta));
    }
    double deltaOD = m_dt * std::sqrt(3) * UnitsSystem::alpha * eta
        * std::pow(10.0, logh)
        / (part->GetGamma() * 2.0 * UnitsSystem::pi);
    part->UpdateOpticalDepth(deltaOD);
    // Now check if process has occured. If so then emmit and react
    if (part->GetOpticalDepth() < 0.0)
    {

        // Update track as normal
        double chi = CalculateChi(eta);
        double gammaE = 2.0 * chi * part->GetGamma() / eta;
        ThreeVector gammaP = gammaE * part->GetDirection();
        part->UpdateTrack(part->GetPosition(), part->GetMomentum()
            - gammaP);
        part->InitOpticalDepth();

        if (m_importance)
        {
            if (eta > 1e-3) // stops extraoplation from table. Should fix soon
            {
                // Use uniform, may change later
                double chi = MCTools::RandDouble(1e-4, eta / 2.0);
                double weight = CalculateRate(eta, chi) * (eta / 2.0);
                if (weight > 1e5)
                {
                    std::cout << chi << " " << eta << " " <<weight << std::endl;
                }
                //std::cout << weight * part->GetWeight() << std::endl;
                double gammaE = 2.0 * chi * part->GetGamma() / eta;
                ThreeVector gammaP = gammaE * part->GetDirection();
                if (gammaE > m_eMin)
                {
                    Photon* photon = new Photon(gammaE, part->GetPosition(),
                        part->GetDirection(), weight * part->GetWeight(), 
                        part->GetTime(), m_track);
                    partList->AddParticle(photon);
                }
            }
        } else
        {
            if (gammaE > m_eMin)
            {
                if (eta > 1e-3 && chi > 1e-4) // stops extraoplation from table. Should fix soon
                {
                    Photon* photon = new Photon(gammaE, part->GetPosition(),
                        part->GetDirection(), 1, part->GetTime(), m_track);
                    partList->AddParticle(photon);
                }
            }
        }
    }
}

double NonLinearCompton::CalculateEta(Particle* part) const
{
    ThreeVector partDir = part->GetDirection();
    ThreeVector eField, bField;
    m_field->GetField(part->GetTime(), part->GetPosition(), eField, bField);
    ThreeVector ePara = eField.Dot(partDir) * partDir;
    ThreeVector ePerp = eField - ePara;
    double eta = std::sqrt((ePerp + part->GetBeta() * partDir.Cross(bField)).Mag2()
                           + std::pow(partDir.Dot(eField), 2.0) / (part->GetGamma() 
                            * part->GetGamma())) * part->GetGamma();
    return eta;
}

double NonLinearCompton::CalculateChiMin(Particle* part, double energyMin) const
{
    ThreeVector partDir = part->GetDirection();
    ThreeVector eField, bField;
    m_field->GetField(part->GetTime(), part->GetPosition(), eField, bField);
    ThreeVector ePara = eField.Dot(partDir) * partDir;
    ThreeVector ePerp = eField - ePara;
    return 0.5 * energyMin * (ePerp + partDir.Cross(bField)).Mag();
}

double NonLinearCompton::CalculateRate(double eta, double chi) const
{
    int lowIndex;
    double frac;
    Numerics::ClosestPoints(m_log_eta_rate, m_rate_size_x, std::log10(eta),
        lowIndex, frac);
    double lowValue = Numerics::Interpolate1D(m_log_chi_rate[lowIndex], 
        m_log_rate[lowIndex],m_rate_size_y, std::log10(chi));
    double highValue = Numerics::Interpolate1D(m_log_chi_rate[lowIndex+1], 
        m_log_rate[lowIndex+1], m_rate_size_y, std::log10(chi));
    return std::pow(10.0, (1.0 - frac) * lowValue + frac * highValue);
}

double NonLinearCompton::CalculateChi(double eta) const
{
    double rand = MCTools::RandDouble(0, 1);
    int lowIndex;
    double frac;
    Numerics::ClosestPoints(m_phEn_etaAxis, m_phEn_etaLength, std::log10(eta),
        lowIndex, frac);
    double lowValue = Numerics::Interpolate1D(m_phEn_dataTable[lowIndex],
        m_phEn_chiAxis[lowIndex], m_phEn_chiLength, rand);
    double highValue = Numerics::Interpolate1D(m_phEn_dataTable[lowIndex+1],
        m_phEn_chiAxis[lowIndex+1], m_phEn_chiLength, rand);
    return std::pow(10.0, (1.0 - frac) * lowValue + frac * highValue);
}

void NonLinearCompton::LoadTables()
{
    char* tablePath(getenv("QED_TABLES_PATH"));
    if (tablePath == NULL)
    {
        std::cout << "Error: Enviromental variable \"QED_TABLES_PATH\" "; 
        std::cout << "is not set!" << std::endl;
        std::cout <<  "Please set QED_TABLES_PATH to point to tables directory." << std::endl;
    }

    std::string path(tablePath);
    // Load the table for h
    std::ifstream hFile(path + "/hsokolov.table");
    if (!hFile)
    {
        std::cerr << "ERROR: Data table for h not found!" << std::endl;
        std::cerr << "No file at: " << path + "/hsokolov.table" << std::endl;
        exit(1);
    }

    std::ifstream phEnFile(path + "/ksi_sokolov.table");
    if (!phEnFile)
    {
        std::cerr << "ERROR: Data table for photon energy sampling not found!" << std::endl;
        std::cerr << "No file at: " << path + "/ksi_sokolov.table" << std::endl;
        exit(1); 
    }
    std::ifstream chiMinFile(path + "/chimin.table");
    if (!chiMinFile)
    {
        std::cerr << "ERROR: Data table for chimin energy sampling not found!" << std::endl;
        std::cerr << "No file at: " << path + "/chimin.table" << std::endl;
        exit(1); 
    }

    std::ifstream rateChiEtaFile(path + "/rateChiEta.table");
    std::ifstream rateFile(path + "/rate.table");
    if (m_importance)
    {
        if (!rateChiEtaFile)
        {
            std::cerr << "ERROR: Data table for eta-chi rate not found!" << std::endl;
            std::cerr << "No file at: " << path + "/rateChiEta.table" << std::endl;
            exit(1); 
        }
        if (!rateFile)
        {
            std::cerr << "ERROR: Data table for eta-chi rate not found!" << std::endl;
            std::cerr << "No file at: " << path + "/rate.table" << std::endl;
            exit(1); 
        }
    }


    // h table
    hFile >> m_h_length;
    m_h_dataTable = new double [m_h_length];
    m_h_etaAxis   = new double [m_h_length];
    double  logEta, logH;
    for (unsigned int i = 0; i < m_h_length; i++)
    {
        hFile >> logEta >> logH;
        m_h_etaAxis[i] = logEta;
        m_h_dataTable[i] = logH;

    }

    // photon energy and chi min tables
    double logMaxEta, logMinEta;
    phEnFile >> m_phEn_etaLength >> m_phEn_chiLength >> logMinEta >> logMaxEta;
    m_phEn_etaAxis = new double [m_phEn_etaLength];
    m_phEn_chiMinAxis = new double [m_phEn_etaLength];
    m_phEn_chiAxis = new double* [m_phEn_etaLength];
    m_phEn_dataTable = new double* [m_phEn_etaLength];
    for (unsigned int i = 0; i < m_phEn_etaLength; i++)
    {
        m_phEn_chiAxis[i] = new double [m_phEn_chiLength];
        m_phEn_dataTable[i] = new double [m_phEn_chiLength];
    }

    // Load or calculate each table
    for (unsigned int i = 0; i < m_phEn_etaLength; i++)
    {
        chiMinFile >> m_phEn_chiMinAxis[i];
    }

    double deltaEta = (logMaxEta - logMinEta) / (m_phEn_etaLength - 1.0);
    for (unsigned int i = 0; i < m_phEn_etaLength; i++)
    {
        double logMinChi = std::log10(m_phEn_chiMinAxis[i]);
        m_phEn_etaAxis[i] = logMinEta + i * deltaEta;
        double deltaChi = (m_phEn_etaAxis[i] - std::log10(2.0)
            - logMinChi)  / (m_phEn_etaLength - 1.0);
        for (unsigned int j = 0; j < m_phEn_chiLength; j++)
        {
            m_phEn_chiAxis[i][j] = logMinChi + j * deltaChi;
        }
    }

    for (unsigned int i = 0; i < m_phEn_etaLength; i++)
    {
        for (unsigned int j = 0; j < m_phEn_chiLength; j++)
        {
            phEnFile >> m_phEn_dataTable[i][j];
        }
    }

    // Load rate table
    if (m_importance)
    {
        rateChiEtaFile >> m_rate_size_x >> m_rate_size_y >> m_chi_min_rate;
        m_log_eta_rate = new double [m_rate_size_x];
        m_log_chi_rate = new double* [m_rate_size_x];
        m_log_rate = new double* [m_rate_size_x];
        for (unsigned int i = 0; i < m_rate_size_x; i++)
        {
            m_log_chi_rate[i] = new double[m_rate_size_y];
            m_log_rate[i] = new double[m_rate_size_y];
        }

        double  logEta, logChi;
        for (unsigned int i = 0; i < m_rate_size_x; i++)
        {
            rateChiEtaFile >> logEta;
            m_log_eta_rate[i] = logEta;

            double deltaChi = (logEta - std::log10(2.0) - 0.01 - m_chi_min_rate) 
                / (m_rate_size_y - 1);
            for (unsigned int j = 0; j < m_rate_size_y; j++)
            {
                rateFile >> m_log_rate[i][j];
                m_log_chi_rate[i][j] = m_chi_min_rate + j * deltaChi;
            }
        }
    }
}

void NonLinearCompton::UnloadTables()
{
    delete [] m_h_dataTable;
    delete [] m_h_etaAxis;
    for (unsigned int i = 0; i < m_phEn_etaLength; i++)
    {
        delete [] m_phEn_chiAxis[i];
        delete [] m_phEn_dataTable[i];
    }
    delete [] m_phEn_chiMinAxis;
    delete [] m_phEn_etaAxis;
    delete [] m_phEn_chiAxis;
    delete [] m_phEn_dataTable;

    if (m_importance)
    {
        delete m_log_eta_rate;
        for (unsigned int i = 0; i < m_rate_size_x; i++)
        {
            delete[] m_log_rate[i];
        }
        delete[] m_log_rate;
    }
}