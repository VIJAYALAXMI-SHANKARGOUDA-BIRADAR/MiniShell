#include "mini.h"



char *external_command[160];
int pid,status;


s *head = NULL;

char *cmd=NULL;

void scan_input(char *prompt, char *input_string)

{
    system("clear");

    //register signal
    signal(SIGINT,signal_handler);
    signal(SIGTSTP,signal_handler);




    //call external command
    extract_external_commands(external_command);

    while(1)
    {   
        //1.prompt

        //print the prompt_str
        printf(ANSI_COLOR_GREEN"%s"ANSI_COLOR_RESET,prompt);
        scanf(" %[^\n]",input_string);

        //check if ps1=is passed or not
        if( (strncmp(input_string,"PS1=",4))==0)
        {
            if(input_string[4]!= ' ')
            {
                strcpy(prompt,input_string+4);
            }
        }



        else
        {
            //2nd function getcommand
            cmd = get_command(input_string);
            printf("The first word is %s\n",cmd);

            int res= check_command_type(cmd);

            if(res==BUILTIN)
            {
                printf("Built %d\n",res);
                execute_internal_commands(input_string);
            }

            /*printf("CMD: %s\n", cmd);
              exit(0);*/

            else if(res == EXTERNAL)
            {
                printf("External %d\n",res);
                //create a process
                pid=fork();

                if(pid > 0)
                {
                    waitpid(pid,&status,WUNTRACED);
                    //pid=0;
                }
                else if(pid == 0)
                {
                    signal(SIGINT,SIG_DFL);
                    signal(SIGTSTP,SIG_DFL);
                    execute_external_commands(input_string);
                    exit(0);
                }
                /*
                   switch(pid)
                   {
                   case -1:
                   printf("fork fail\n");
                   return;

                   break;


                //child
                case 0:
                //register signal
                signal(SIGINT,SIG_DFL);
                signal(SIGTSTP,SIG_DFL);

                execute_external_commands(input_string);



                break;

                //parent  
                default:
                waitpid(pid,&status,WUNTRACED);
                pid=0;
                }*/
            }
            else
            {
                printf("no command\n");
            }
        }
        memset(input_string, 0, sizeof(input_string));

    }

}

