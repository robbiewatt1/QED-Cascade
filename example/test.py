from QEDCascPy import RunManager, ThreeVector
import time as timeee
import numpy as np
import matplotlib.pyplot as plt

### Time variables
time = 100e-15
d_t = 0.05e-15

### Set Field Variables
field_type = "focusing"
peak_field = 2.00e+14
wavelength = 0.8e-6
duration_laser = 30.0e-15
waist = 10e-6
polarisation = 10e-6
start = ThreeVector(0,0, 10.0e-6)
focus = ThreeVector(0,0, 0e-6)

### Set particle source
n_particles = 1000
particle_type = "Electron"
energy_dist = "gaussian"
energy_mean = 8.176e-11
eneray_sig = 8.176e-12
spot_size = 0.
duration_part = 0.
divergence = 0
position = ThreeVector(0,0,-10e-6)
diraction  = ThreeVector(0,0,1)

### Set particle pusher
pusher = "Lorentz"

### Select physics
nonlinear_Compton = True
nonlinear_BreitWheeler = False

### Set inputs
run_manager = RunManager()
run_manager.setTime(d_t, time)
run_manager.setField(field_type, peak_field, wavelength,  duration_laser, waist, 
    polarisation, start, focus)
run_manager.setPusher(pusher);
run_manager.setPhysics(nonlinear_Compton, nonlinear_BreitWheeler)
run_manager.setGenerator(n_particles, particle_type, energy_dist, energy_mean, 
    eneray_sig, spot_size, duration_part, divergence, position, diraction);

### Run simulation
run_manager.beamOn(2)

### Get inputs / outputs
init_electrons = run_manager.getInput()
final_electrons = run_manager.getOutput("Electron")
final_photon = run_manager.getOutput("Photon")

init_p_z = init_electrons[:,2]
final_p_z = final_electrons[:,2]
final_photon = final_photon[:,2]


### Plots
fig, ax = plt.subplots()
bins = np.linspace(0, 1500, 100)
ax.hist(init_p_z, bins, histtype="step", lw=2)

fig, ax = plt.subplots()
ax.hist(final_p_z, 100, histtype="step", lw=2)
ax.set_xlabel(r"$P_z \,\,(m_e)$")

fig, ax = plt.subplots()
ax.hist(final_photon, 100, histtype="step", lw=2)
ax.set_xlabel(r"$P_z \,\,(m_e)$")

plt.show()
