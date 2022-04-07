# Traffic Simulation

Wriiten in C, this is a simulation of a random two-way road with traffic lights.

In order to compile and link the code, navigate to the correct directory containing the files and write ./compileSim on the command line. 
This will create the runSimulations file, so write ./runSimulations, along with four arguments in order left traffic rotation, right traffic rotation, 
left arrival rate and right arrival rate. If you put incorrect arguments, otherwise it should be outputted to the stderr stream, the results (averaged 
over 100 runs) will be outputted to the stdout stream.
