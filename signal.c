#include"mini.h"

extern char prompt_str[30];
extern int pid,status;

extern s *head;
extern char *cmd;
int flag;


void signal_handler(int signum)
{
   
    if(signum==SIGINT)
    {
        if(pid==0)
        {

            printf("\n%s ",prompt_str);
            fflush(stdout);

        }


    }

    else if(signum == SIGTSTP)
    {
        if(pid==0)
        {
            printf("\n%s ",prompt_str);
            fflush(stdout);
        }

        //else if pid!=0

        else if(pid != 0)
        {
           // flag=1;

          insert_at_first(&head,cmd,pid);
          //  return;
            pid = 0;

        }
    }

    else if(signum==SIGCHLD)
    {
        //the resources of child

        //waitpid
        waitpid(-1,&status,WNOHANG);

        delete_first(&head);


    }
}





int insert_at_first(s **head, char *cmd, int pid)
{
    // Creating the new node
    s *new = malloc(sizeof(s));

    // Check whether new node created or not
    if (new == NULL)
    {
        return FAILURE;
    }

    // Fill the parts of the node
    new->pid = pid;
    strcpy(new->command,cmd);
    new->link = NULL;

    //If list is empty
    if (*head == NULL)
    {
        //If *head is empty then create the first node
        *head = new;
        return SUCCESS;
    }
    else
    {
        // *head is not empty then store value i.e link in the temp variable
        s *temp = *head;

        // Stroring that new link in the head inserting the element at first *
        *head = new;

        // Creating the link to next data
        new -> link = temp;
    }

    return SUCCESS;
}
