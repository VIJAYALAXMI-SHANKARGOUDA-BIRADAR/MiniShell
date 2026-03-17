#include"mini.h"


/*builtin commands/*/
        char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
                                                "set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
                                                "exit", "jobs","fg","bg","exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};

extern int status;
extern s *head;
extern int pid;
extern int cmd,flag;
//char *external_command[160];
char *get_command(char *input_string)
{
    //*fetch the first word  from input string
        static char cmd[30];
        int i=0;
        while( input_string[i]!='\0')
        {
            if( input_string[i] != ' ')
            {
                cmd[i]=input_string[i];
            }
            else
            {
                break;
            }
            i++;
        }
        cmd[i]='\0';

    //2.and return it

    return cmd;
}

 void extract_external_commands(char **external_commands)
 {

     char ch;
     char buf[30]={'\0'};

    //i)open the file ,read the content till \n

     int fd=open("external.txt",O_RDONLY,0666);

     if(fd==-1)
     {
         perror(" ");
         return ;
     }
     int i=0,j=0;

     while(read(fd,&ch,1) >0)
     {
         if(ch != '\n')
         {

             buf[i++]=ch;
         }
         else
         {
             //ii)take the length of the command
             int len= strlen(buf);
             //iii)allocate the memory for  col[len] column dynamic
                buf[i]='\0';
             external_commands[j]=calloc(len+1,sizeof(char));

             //iv)store the cmd into external command
             strcpy(external_commands[j++],buf);
             //buf[0]='\0';

             i=0;
         }
     }

     /*i = 0;
     while(external_commands[i] != NULL)
     {
         printf("%s\n", external_commands[i++]);
     }*/
 }

extern char *external_command[160];

  int check_command_type(char *command)
  {
        //1)comapre command with builtin

            for(int i=0;builtins[i] !=NULL;i++)
            {

            //if it is matching
            if( strcmp(command,builtins[i])==0)
            {

            return 1;//BUILTIN

            }

           }

          //2.compare command with external commands
            for(int i=0;external_command[i]!=NULL;i++)
            {
                //if it is matching return EXTERNAL
                if( strcmp(command,external_command[i]) ==0)
                {
                    return EXTERNAL;
                }

            }
          return NO_COMMAND;
  }

void execute_internal_commands(char *input_string)

{
    //1.check the input string is exit or not

    if( strcmp(input_string,"exit")==0)
    {
        exit(0);
    }

    //2.compare input_str with pwd
    else if( strcmp(input_string,"pwd")==0)
    {
    //if it is pwd  using getcwd library

    char buff[50];
    getcwd(buff,50);
    printf("%s\n",buff);

    }

    //3.compare input_string with cd[strncmp]
    else if( strncmp(input_string,"cd",2)==0)
    {
        chdir(input_string+3);
    }

    //4.echo $$
    //compare input string with echo $$ and getpid()
    else if( strcmp(input_string,"echo $$")==0)
    {
        printf("echo %d\n",getpid());

    }
    //5.echo $?
    else if( strcmp(input_string,"echo $?")==0)
    {
        if(WIFEXITED(status))
        {
            printf("%d\n",WEXITSTATUS(status));
        }
        else
        {
            printf("%d\n",WEXITSTATUS(status));
        }
    }
    //6.echo $SHELL
    else if( strcmp(input_string,"echo $SHELL")==0)
    {
        printf("%s \n",getenv("SHELL"));
    }

    //7.jobs
    else if( strcmp(input_string,"jobs")==0)
    {
        print_list(head);

    }
    //fg

    else if( strcmp(input_string,"fg")==0)
    {
        if(head != NULL)
        {
            printf("%s\n", head->command);
            kill(head->pid, SIGCONT);
            waitpid(head->pid, &status, WUNTRACED);
            wait(NULL);
            delete_first(&head);

         }
    }

//bg
    else if(strcmp(input_string,"bg")==0)
    {
        if(head==NULL)
        {
            printf("bg : no jobs\n");

        }
        else
        {
        printf("%s\n",head->command);
     //   signal(SIGCHLD,signal_handler);
        kill(head->pid,SIGCONT);

        }
    }

}

void execute_external_commands(char *input_string)
{

   //1.convert 1d array 2d array (input_str)->argv[][]
    char *argv[30];
    int len=strlen(input_string);

    char *temp=input_string;

    char *token;int i=0;
    token=strtok(temp," ");
    while(token)
    {
        argv[i++]=token;
        token=strtok(NULL," ");
    }
    argv[i]=NULL;
    int j=0;


    //2,check pipe is passesd or not
    int pipe_flag=0;
    for(int i=0;argv[i]!=NULL;i++)
    {
        if(strcmp(argv[i],"|")==0)
        {
             pipe_flag=1;
            break;
        }
    }



    if(pipe_flag==0)
    {
        execvp(argv[0],argv);
        exit(1);


    }

//n_pipe

    if(pipe_flag==1)
    {

        int argc=i;



    int cmd_pos[argc],cmd_count=0;
   // if(argc<3)
    //{
      //  printf("Error: No sufficient arguments\n");
        //return;

    //}
    cmd_pos[cmd_count++]=0;
    for(int i=0;i<argc;i++)
    {
       if( strcmp(argv[i],"|")==0)
       {
        argv[i]=NULL;

        cmd_pos[cmd_count] = i+1;
        cmd_count++;
       }
    }

    for(int i=0;i<cmd_count;i++)
    {
        int fd[2];

        if(i<cmd_count-1)
      {
        int status=pipe(fd);

        if(status==-1)
        {
            perror(" pipe \n");
            return ;

        }
      }

         pid=fork();

        switch(pid)
        {
            case -1:
            printf("fork fail %d\n",i);
            return;
            break;

            case 0://child
            if(i<cmd_count-1)
            {
            close(fd[0]);
            dup2(fd[1],1);
            close(fd[1]);
            }
            execvp(argv[cmd_pos[i]],argv+ cmd_pos[i]);
            return;
            break;

            default://parent
            if(i<cmd_count-1)
            {

            close(fd[1]);
            dup2(fd[0],0);
            close(fd[0]);
            }
            wait(NULL);
        }
    }

    for(int i=0;i<cmd_count;i++)
    {
        wait(NULL);
    }
    }
}



void print_list(s *head)
{
    if(head==NULL)
    {
        printf("INFO : no processes\n");
    }
    else
    {
        while(head)
        {
            printf("%d\t",head->pid);
            printf("%s\n",head->command);
            head=head->link;
        }
    }

}

int delete_first(s **head)
{
    if(*head==NULL)
    {
        return 1;
    }

    s *temp=*head;
    *head=temp->link;

    free(temp);

    return 0;
}
