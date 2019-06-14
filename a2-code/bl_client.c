#include"blather.h"

simpio_t simpio_actual;
simpio_t *simpio = &simpio_actual;

client_t client_actual;
client_t *client = &client_actual;

pthread_t user_thread;
pthread_t background_thread;

struct arg_struct{
  char* arg1;
  char* arg2;
};

void *user_worker(void *arg){
  struct arg_struct *args = (struct arg_struct *)arg;

  int ts_fd = open(args->arg1, O_WRONLY);				  // Open the to-server FIFO
  check_fail(ts_fd < 0, 1, "can't open to_server fifo\n");

  char * user_name = args->arg2;					  // Retrieve the username
  mesg_t msg;
  mesg_t *mesg_to_s = &msg;
  memset(mesg_to_s, 0, sizeof(mesg_t));					  // Initialize message struct bytes to default values of 0

  int ret = snprintf(mesg_to_s->name, MAXNAME, "%s", user_name);			  // Write user name to message struct
  check_fail(ret < 0, 1, "mesg_to_s initialization failed\n");

  while(!simpio->end_of_input) {

    simpio_reset(simpio);
    iprintf(simpio, "");                			                 // print prompt
    while(!simpio->line_ready && !simpio->end_of_input){		  // Take in user input, one char at a time
      simpio_get_char(simpio);
    }

    if(simpio->line_ready){
      int ret2 = snprintf(mesg_to_s->body, MAXLINE, "%s", simpio->buf); 		  // Once input for a line is complete, print the line to the message struct
      check_fail(ret2 < 0, 1, "mesg_to_s body initialization failed\n");
      mesg_to_s->kind = BL_MESG;					  // Set message kind to appropriate value BL_MESG
      int wr = write(ts_fd, mesg_to_s, sizeof(mesg_t));				  // Send message to to-server FIFO
      check_fail(wr < 0, 1, "write to to_server fifo failed\n");
    }
  }

  int cancel = pthread_cancel(background_thread); 					           // kill the background thread
  check_fail(cancel != 0, 1, "server thread cancel failed\n");
  mesg_to_s->kind = BL_DEPARTED;					                         // Set message kind to appropriate value BL_DEPARTED

  int wr = write(ts_fd, mesg_to_s, sizeof(mesg_t));				       // Send DEPART message to to-server FIFO
  check_fail(wr < 0, 1, "write to to_server fifo depart failed\n");

  int cl = close(ts_fd);								  // Close the to-server FIFO
  check_fail(cl < 0, 1, "can't close to_server fifo\n");
  return NULL;
}

void *background_worker(void *arg){
  char* user_fname = (char*) arg;
  int to_server_fd = open(user_fname, O_RDONLY);
  check_fail(to_server_fd < 0, 1, "can't open to_client fifo\n");
  while(1){
    mesg_t msg;
    mesg_t* mesg = &msg;
    memset(mesg, 0, sizeof(mesg_t));

    int rd = read(to_server_fd,mesg,sizeof(mesg_t));           //read a mesg_t from to-client FIFO
    check_fail(rd < 0, 1, "read from to_client fifo failed\n");

    char terminal_message[MAXLINE];                   //print appropriate response to terminal with simpio

    if(mesg->kind == BL_MESG){
      int ret1 = snprintf(terminal_message, MAXLINE, "[%s] : %s \n", mesg->name, mesg->body);
      check_fail(ret1 < 0, 1, "can't initialize MESG message\n");
    }
    else if(mesg->kind == BL_JOINED){
      int ret2 = snprintf(terminal_message, MAXLINE, "-- %s JOINED --\n", mesg->name);
      check_fail(ret2 < 0, 1, "can't initialize JOINED message\n");
    }
    else if(mesg->kind == BL_DEPARTED){
      int ret3 = snprintf(terminal_message, MAXLINE, "-- %s DEPARTED --\n", mesg->name);
      check_fail(ret3 < 0, 1, "can't initialize JOINED message\n");
    }
    else if(mesg->kind == BL_SHUTDOWN){
      int ret4 = snprintf(terminal_message, MAXLINE, "!!! server is shutting down !!!\n");
      check_fail(ret4 < 0, 1, "can't initialize SHUTDOWN message\n");
      iprintf(simpio,terminal_message);
      break;                                                 //until a SHUTDOWN mesg_t is read
    }
    iprintf(simpio, terminal_message);
  }
  int close2 = close(to_server_fd);
  check_fail(close2 < 0, 1, "can't close to_client fifo\n");
  int cancel = pthread_cancel(user_thread);                               //cancel the user thread
  check_fail(cancel != 0, 1, "user thread cancel failed\n");
  return NULL;
}

int main(int argc, char *argv[]){

  if(argc != 3){				//Check if correct number of arguments has been entered,
    printf("usage: %s <program> <int>\n",argv[0]); // if not, notify user and exit
    exit(0);
  }

  char *serv_fname = argv[1];			//Retrieve name info from input
  char *user_name = argv[2];

  char to_server_fname[MAXPATH];			//Char arrays that will contain the full fifo names
  char to_client_fname[MAXPATH];


  strcpy(to_client_fname,"");			//Properly set up names of FIFOs in relation to entered username

  strcpy(to_server_fname,"");
  strcat(to_client_fname,user_name);
  strcat(to_client_fname,"_to_cl \0");
  strcat(to_server_fname,user_name);
  strcat(to_server_fname,"_to_srv \0");
  strcat(user_name,"\0");

  mkfifo(to_client_fname, S_IRUSR | S_IWUSR);	//Make to-client FIFO
  mkfifo(to_server_fname, S_IRUSR | S_IWUSR);     //Make to-server FIFO


  join_t join_actual;
  join_t *join = &join_actual;					//Create join request struct

  memset(join, 0, sizeof(join_t));

  int ret = snprintf(join->name, MAXPATH, "%s", user_name); //Send user name info to join struct
  check_fail(ret < 0, 1, "can't initialize join request name\n");

  int ret2 = snprintf(join->to_client_fname, MAXPATH, "%s", to_client_fname); //Send to-client name info to join struct
  check_fail(ret2 < 0, 1, "can't initialize to_client name\n");

  int ret3 = snprintf(join->to_server_fname, MAXPATH, "%s", to_server_fname); //Send to-server name info to join struct
  check_fail(ret3 < 0, 1, "can't initialize to_server name\n");

  int serv_fd = open(serv_fname, O_RDWR); 		// Open the server's join FIFO, set as READ and WRITE
  check_fail(serv_fd < 0, 1, "can't open join fifo\n");

  struct arg_struct args;			// Initialize struct to contain the two pieces of information needed by the user thread
  args.arg1 = to_server_fname;
  args.arg2 = user_name;


  int wr = write(serv_fd, join, sizeof(join_t));		//Write join request to server's FIFO
  check_fail(wr < 0, 1, "can't write to join fifo\n");

  char prompt[MAXNAME];
  int prmpt = snprintf(prompt, MAXNAME, "%s>> ",user_name); // create a prompt string
  check_fail(prmpt < 0, 1, "can't initialize prompt\n");

  simpio_set_prompt(simpio, prompt);         // set the prompt
  simpio_reset(simpio);                      // initialize io
  simpio_noncanonical_terminal_mode();       // set the terminal into a compatible mode

  int usr = pthread_create(&user_thread, NULL, user_worker, (void *) &args);		// Create thread for user input
  check_fail(usr != 0, 1, "can't create user thread\n");

  int srv = pthread_create(&background_thread, NULL, background_worker, (void *) to_client_fname);    // Create thread for server output
  check_fail(srv != 0, 1, "can't create server thread\n");

  int usr_join = pthread_join(user_thread, NULL);						// Regroup user thread following cancellation
  check_fail(usr_join != 0, 1, "can't join user thread\n");

  int srv_join = pthread_join(background_thread, NULL);						// Regroup server thread following cancellation
  check_fail(srv_join != 0, 1, "can't join server thread\n");

  int cl = close(serv_fd);								// Close the server's join FIFO
  check_fail(cl < 0, 1, "can't close join fifo\n");
  simpio_reset_terminal_mode();
  printf("\n");                 // newline just to make returning to the terminal prettier
}
/*create to-server and to-client FIFOs
write a join_t request to the server FIFO
start a user thread to read inpu
start a server thread to listen to the server
wait for threads to return
restore standard terminal output*/
