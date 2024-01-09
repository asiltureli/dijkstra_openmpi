# Dijkstra OpenMPI
## Overview
The DijkstraOpenMPI project is a C++ application that implements the Dijkstra algorithm using the OpenMPI library for parallel computing. This project is designed to run on systems with MPI support. You can create huge random directed graphs automatically with the Dijkstra class and test these. The project is designed to run on systems with MPI support.

## Prerequisites
Before building the project, ensure you have the following installed on your system:

CMake (version 3.10 or higher)
An MPI implementation (e.g., OpenMPI, MPICH)

Install these using your system's package manager. For example, on Ubuntu or Debian:

```
sudo apt-get update
sudo apt-get install build-essential cmake make openmpi-bin libopenmpi-dev
```
## Building the project
### 1. Clone the Repository  
First, clone the project repository to your local machine. You can do this using the following command:  
```
git clone https://github.com/asiltureli/dijkstra_openmpi
```
### 2. Navigate to the Project Directory 
Change to the project directory:
```
cd DijkstraOpenMPI
```
### 3. Create a Build Directory  
```
mkdir build && cd build
```
### 4. Run CMake
```
cmake ..
```

### 5. Compile the Project
```
make
```
### 6. Run the Program
- For a standard execution:
```
./DijkstraApp
```
- For an MPI execution:
```
mpirun -np N ./my_dijkstra_program
```
replace 'N' with the number of processes you want to run
