/*********************************************************
 *                     ΠΑΟΛΑ ΒΕΛΑΣΚΟ                  
 *                   cs161020, 9ο εξάμηνο 
 * *******************************************************
 *              Πανεπιστήμιο Δυτικής Αττικής 
 *       Τμήμα Μηχανικών Πληροφορικής και Υπολογιστών
 * -------------------------------------------------------
 *            Εισαγωγή στον Παράλληλο Υπολογισμό 
 *            Εργαστήριο Τμήμα: Ε2 (Δευτέρα 1-2)
 *        Καθηγητές: κος Β. Μάμαλης, κος Μ. Ιορδανάκης
 * -------------------------------------------------------
 *        ΕΡΓΑΣΙΑ 1 - MPI in C (Point To Point)
 *********************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

void main(int argc, char **argv)
{
    int j;
    int i;
    int size;
    int *data;
    int *data_local;
    int *numbersSend;

    int numbers;
    int number;
    int mod;
    int sum = 0;
    int flag = 0;

    int index;
    int error_index;
    int errors_index;
    int size_local;

    int my_rank, numtasks;
    int source, dest;
    int tag1 = 30; // size of local data
    int tag2 = 40; // data
    int tag3 = 50; // flag
    int tag4 = 60; // previous
    int tag5 = 70; // error

    int previous = 0;

    int rc;
    MPI_Status status;

    // start the parallelism
    rc = MPI_Init(&argc, &argv);
    if (rc != 0)
    {
        printf("MPI initialization error\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }
    // function that returns the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    // function that returns the rank of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == 0)
    {
        printf("Give size of array: ");
        scanf("%d", &size);
        data = (int *)malloc(sizeof(int) * size);

        // Check if malloc was successfully completed
        if (!data)
        {
            printf("Failure");
            exit(0);
        }

        // User's input values in the array
        for (i = 0; i < size; i++)
        {
            printf("Give value for data[%d]: ", i);
            scanf("%d", (data + i));
        }

        // Create an array that keeps how many numbers should be given to each process
        numbersSend = (int *)malloc(sizeof(int) * numtasks);
        // Check if malloc was successfully completed
        if (!numbersSend)
        {
            printf("Failure");
            exit(0);
        }

        // dividing how many numbers should be given to each process
        mod = size % numtasks;
        for (i = 1; i < numtasks; i++)
        {
            dest = i;

            if (i < mod)
            {
                *(numbersSend + i) = size / numtasks + 1;
            }
            else
            {
                *(numbersSend + i) = size / numtasks;
            }

            MPI_Send((numbersSend + i), 1, MPI_INT, dest, tag1, MPI_COMM_WORLD);
        }

        // Αποστολή σε κάθε διεργασία διαφορετικά δεδομένα. (Διαμοίραση δεδομένων)
        index = *(numbersSend + 1);

        for (i = 1; i < numtasks; i++)
        {
            dest = i;
            MPI_Send(&data[index - 1], 1, MPI_INT, dest, tag4, MPI_COMM_WORLD);
            MPI_Send(&data[index], *(numbersSend + i), MPI_INT, dest, tag2, MPI_COMM_WORLD);
            index += *(numbersSend + i);
        }

        if (0 < mod)
        {
            *numbersSend = size / numtasks + 1;
        }
        else
        {
            *numbersSend = size / numtasks;
        }

        number = *numbersSend;

        // Apothikeush sto data_local[] tou processor oi prwtoi "arithmous_pou_tha_anatethoun_se_kathe_diergasia"
        data_local = (int *)malloc(sizeof(int) * number);
        // Check if malloc was successfully completed
        if (!data_local)
        {
            printf("Failure");
            exit(0);
        }
        for (i = 0; i < number; i++)
        {
            data_local[i] = data[i];
        }

        previous = *data_local;

        free(data);
    }
    else
    {
        MPI_Recv(&number, 1, MPI_INT, 0, tag1, MPI_COMM_WORLD, &status);

        data_local = (int *)malloc(sizeof(int) * number);
        if (!data_local)
        {
            printf("Failure");
            exit(0);
        }
        MPI_Recv(&previous, 1, MPI_INT, 0, tag4, MPI_COMM_WORLD, &status);
        MPI_Recv(data_local, number, MPI_INT, 0, tag2, MPI_COMM_WORLD, &status);

    }

    flag = 0;

    if (number == 1)
    {
        numbers = 2;
    }
    else
    {
        numbers = number;
    }

    for (i = 0; i < numbers - 1; i++)
    {
        if (previous <= data_local[i])
        {
            if ((sizeof(int) * number) != (sizeof(int)))
            {
                if (data_local[i] > data_local[i + 1])
                {
                    error_index = i;
                    if (my_rank != 0)
                        MPI_Send(&error_index, 1, MPI_INT, 0, tag5, MPI_COMM_WORLD);

                    flag = 1;
                    break;
                }
            }
        }

        else
        {
            if (my_rank != 0)
            {
                error_index = -1;
                MPI_Send(&error_index, 1, MPI_INT, 0, tag5, MPI_COMM_WORLD);
            }
            else
                error_index = i;

            flag = 1;
            break;
        }
    }

    if (my_rank == 0)
    {
        if (flag == 1)
        {
            printf("Array not sorted\n");
            printf("There's an error at data[%d]\n", error_index);
            free(data_local);
            free(numbersSend);
            MPI_Finalize();
            exit(0);
        }

        for (i = 1; i < numtasks; i++)
        {
            source = i;
            errors_index = 0;
            MPI_Recv(&flag, 1, MPI_INT, source, tag3, MPI_COMM_WORLD, &status);
            if (flag == 1)
            {
                MPI_Recv(&error_index, 1, MPI_INT, source, tag5, MPI_COMM_WORLD, &status);

                for (j = 0; j < source; j++)
                    errors_index = errors_index + numbersSend[i];
                    
                errors_index = errors_index + error_index;

                printf("\nArray not sorted\n");
                printf("%d There's an error at data[%d]\n", source, errors_index);
                break;
            }
        }

        if (flag == 0)
            printf("\nArray is sorted in ascending order\n");

        free(numbersSend);
        free(data_local);
    }
    else
    {
        MPI_Send(&flag, 1, MPI_INT, 0, tag3, MPI_COMM_WORLD);
        free(data_local);
    }

    MPI_Finalize();
}
