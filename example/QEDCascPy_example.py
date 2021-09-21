from QEDCascPy import RunManager, ThreeVector
import numpy as np
import matplotlib.pyplot as plt
import time


### Time variables
end_time = 100e-15
d_t = 0.05e-15

### Set Field Variables
field_type = "gaussian"
peak_field = 2.00e+14
wavelength = 0.8e-6
duration_laser = 30.0e-15
waist = 10e-6
polarisation = 10e-6
start = ThreeVector(0, 0, 10.0e-6)
focus = ThreeVector(0, 0, 0e-6)

### Set particle source
particle_type = "Electron"
energy_dist = "gaussian"
energy_mean = 8.176e-11
eneray_sig = 8.176e-12
spot_size = 0.
duration_part = 0.
divergence = 0
position = ThreeVector(0e-6, 0, -10e-6)
diraction  = ThreeVector(0, 0, 1)

### Set particle pusher
pusher = "Lorentz"

### Select physics (if nonlinear_Compton = True, must use pusher = "Lorentz")
nonlinear_Compton = True
nonlinear_BreitWheeler = False

### Set inputs
run_manager = RunManager()
run_manager.setTime(d_t, end_time)
run_manager.setField(field_type, peak_field, wavelength,  duration_laser, waist, 
    polarisation, start, focus)
run_manager.setPusher(pusher);
run_manager.setPhysics(nonlinear_Compton, nonlinear_BreitWheeler)
run_manager.setGenerator(particle_type, energy_dist, energy_mean, 
    eneray_sig, spot_size, duration_part, divergence, position, diraction);

### Run simulation
run_manager.beamOn(events = 1000, threads = 2)

### Get inputs / outputs
init_electrons = run_manager.getInput()
final_electrons = run_manager.getElectrons()
final_photon = run_manager.getPhotons()

init_p_z = init_electrons[:,2]
final_p_z = final_electrons[:,2]
final_photon = final_photon[:,2]

### Plots
mev_c = 5.344286e-22
bins = np.linspace(0, 800, 100)

fig, ax = plt.subplots()
ax.hist(init_p_z / mev_c, bins, histtype="step", lw=2)
ax.set_xlabel(r"$P_z \,\,(MeV/c)$")

fig, ax = plt.subplots()
ax.hist(final_p_z / mev_c, bins, histtype="step", lw=2)
ax.set_xlabel(r"$P_z \,\,(MeV/c)$")

fig, ax = plt.subplots()
ax.hist(final_photon / mev_c, bins/2, histtype="step", lw=2)
ax.set_xlabel(r"$P_z \,\,(MeV/c)$")
ax.set_yscale('log')
plt.show()
