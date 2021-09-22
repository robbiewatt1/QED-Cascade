# QED-Cascade

Monte Carlo package for simulating strong field QED interactions.

## Standard Version

### Dependencies:
* CMake > 3.5
* HDF5
* OpenMP (optional)
* MPI (optional)

### Installing:

```bash
git clone https://github.com/robbiewatt1/QED-Cascade.git
cd QED-Cascade
mkdir Build
cd Build
cmake ..
make install -j
```

To build with OpenMP or MPI replace "cmake .." with either:
```bash
cmake .. -DBUILD_OPENMP=ON
cmake .. -DBUILD_MPI=ON
```
Once installing has finished, a directory should appear in the code root called "Install". The code executable is located in: "./Install/bin/".

### Running the code:

The code will write all output to the current working directory. To manage files, it is advised that you create a new directory inside "./Simulations" for each new simulation performed. An example input deck is given in: "./example/example.ini". To run this test simulation use the following (assuming you are in the code root directory):

```bash
mkdir Simulations/example
cp ./example/example.ini Simulations/example/
cd Simulations/example
../../Install/bin/QEDCASC example.ini
````
or if running with mpi replace the last line with:
```bash
mpiexec -n <np> ../../Install/bin/QEDCASC example.ini

```
where <np> is the number of processes.

After running the code two, files will written to the current directory. "input-check.txt" gives a summary of the input parameters and is a useful check that file parsing has been successful. "example.h5" is an hdf5 file containing all the code output.


## Python Install

The code can also be 


only dependency is Numpy 

### Linux
* Clone the 