#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> //to use system()
#include <sys/wait.h>
#include "rb-test.h"

int main(int argc, char *argv[])
{
    system("gcc mandelbrot.c -o mandelbrot"); // to compile mandelbrot set code
    int pid = 1;                              // holder for parent
    int perm_index;
    int count = 5;

    int pid_list[5];
    char *command_list[] = {"./mandelbrot -2 0.5 -1.5 1.5 1000 1024 pic1.ppm", "./mandelbrot -2 0.5 -1.5 1.5 1000 2048 pic2.ppm", "./mandelbrot -2 0.5 -1.5 1.5 2000 1024 pic3.ppm", "./mandelbrot -2 0.5 -1.5 1.5 2000 2048 pic4.ppm", "./mandelbrot -2 0.5 -1.5 1.5 1000 4096 pic5.ppm"};

    for (int index = 0; index < 5; index++)
    {
        if (pid < 0)
        {
            printf("Error while creating process \n");
            return 2;
        }
        if (pid)
        {
            pid = fork();
            if (pid)
            {
                pid_list[index] = pid; // pid_list is a list containing the pid's of child processes
                printf("Process %d: %d is created \n", index + 1, pid);
                kill(pid, SIGSTOP);
            }
            else
                perm_index = index;
        }
    }

    if (!pid)
    {
        system(command_list[perm_index]);
        // printf("process %d done \n", perm_index + 1);
        return 10;
    }
    if (pid)
    {
        int status;
        float split;
        int time_quantum = 4;
        int *vruntime = (int *)calloc(count, sizeof(int));
        RBTree *tree = newRBTree();
        RBTreeNode *smallest;
        for (int i = 0; i < count; i++)
        {
            RBInsert(tree, 0, pid_list[i]);
        }
        while (count)
        {
            inorderTraversal(tree->root, tree->nil);
            printf("\n");
            split = (float)time_quantum / count;
            smallest = popSmallest(tree);
            kill(smallest->process_id, SIGCONT);
            usleep(split * 1000000); // cuz microseconds
            printf("Running process %d\n", smallest->process_id);
            if (!waitpid(smallest->process_id, &status, WNOHANG))
            { // waitpid gives 0 for executing process
                kill(smallest->process_id, SIGSTOP);
                RBInsert(tree, smallest->key + split, smallest->process_id);
            }
            else
            {
                printf("Process %d killed in %f\n ", smallest->process_id, smallest->key);
                count -= 1;
            }
        }
        wait(NULL);
    }

    return 0;
}