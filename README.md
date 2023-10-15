# MPI_ParallelComputing_PointToPoint
MPI using point to point communication (MPI_Send, MPI_Recv). A simple uni project to understand the basics of parallel computing

# MPI Point-to-Point Communication Example

- [Introduction](#introduction)
- [Prerequisites](#prerequisites)
- [Usage](#usage)
- [Logic](#logic)
- [Contributing](#contributing)
- [License](#license)

## Introduction

This repository contains a simple example of MPI (Message Passing Interface) point-to-point communication in C. The code demonstrates how to use MPI to send and receive messages between two processes. This is a basic illustration of how to set up and run point-to-point communication in a parallel computing environment.

## Prerequisites

Before you can run this example, you need to have MPI installed on your system.
## Usage

1. **Clone this repository** to your local machine:

    ```shell
    git clone https://github.com/your-username/MPI_ParallelComputing_PointToPoint.git
    ```

2. **Navigate to the project directory**:

    ```shell
    cd MPI_ParallelComputing_PointToPoint
    ```

3. **Compile the source code** using an MPI compiler. For example, if you're using `mpicc`:

    ```shell
    mpicc mpi_pointToPoint_final.c -o mpi_pointToPoint_final
    ```

4. **Run the program** using the `mpirun` command. Ensure you specify the number of processes (e.g., 2 for this example):

    ```shell
    mpiexecn -np 2 ./mpi_pointToPoint_final
    ```

## Logic

Process 0 takes on the role of handling user input, which involves specifying the sequence's length and inputting numbers into an array. We've employed dynamic arrays for the storage of these numbers. In cases where the number of processors matches the sequence's length, each process performs an equal number of comparisons. However, if there are more processors than needed, and they don't evenly divide the sequence, the excess comparisons are distributed using the modulus operator.

## Contribution and Modification

If you have any improvements or suggestions for this example, please feel free to open an issue or create a pull request. Contributions are welcome!
 
This project was created by:
- Velasco Paola 

## Extras
* Report paper in greek: [Report paper](https://github.com/PaolaVlsc/MPI_ParallelComputing_PointToPoint/blob/main/cs161020_espy.pdf)
