#include "blather.h"
#include <errno.h>

client_t *server_get_client(server_t *server, int idx){
  return  &server->client[idx];         // Gets a pointer to the client_t struct at the given index.
}

void server_start(server_t *server, char *server_name, int perms){

  int ret1 = snprintf(server->server_name, MAXPATH, "%s", server_name);  //set the server with the given name
  check_fail(ret1 < 0, 1, "server_name initialization failed\n");

  int rm = remove(server->server_name);					// Remove any prior creations of files of name server_name
  if(rm < 0){
    fprintf(stderr, "can't remove server name file\n");
  }

  int ret2 = mkfifo(server->server_name,perms);
  check_fail(ret2 < 0, 1, "fifo can't be made\n");

  server->join_fd = open(server->server_name, O_RDWR);               //Opens the FIFO and stores its file descriptor in join_fd.
  check_fail(server->join_fd < 0, 1, "join fifo can't be opened\n");

  server->join_ready = 0;                                            //Initialize other parts of server struct
  server->n_clients = 0;

  // ADVANCED: create the log file "server_name.log" and write the
  // initial empty who_t contents to its beginning. Ensure that the
  // log_fd is position for appending to the end of the file. Create the
  // POSIX semaphore "/server_name.sem" and initialize it to 1 to
  // control access to the who_t portion of the log.
}


void server_shutdown(server_t *server){

  int close_join = close(server->join_fd);                          //close join FIFO
  check_fail(close_join < 0 , 1, "Join FIFO cannot be closed");

  int remove_server = remove(server->server_name);                  //remove it so that no furthur clients can join
  check_fail(remove_server < 0, 1, "Fail to remove");

  mesg_t msg;
  mesg_t* mesg = &msg;
  memset(mesg, 0, sizeof(mesg_t));
  
  mesg->kind = BL_SHUTDOWN;
  while(server->n_clients > 0){                                     //Send a BL_SHUTDOWN message to all clients
    client_t* client = server_get_client(server, server-> n_clients-1);
    int ret = write(client->to_client_fd, mesg, sizeof(mesg_t));
    check_fail(ret < 0, 1 , "Fail to write shutdown message");
    client = NULL;                                                  //remove clients
    server->n_clients-=1 ;
  }
  // ADVANCED: Close the log file. Close the log semaphore and unlink
  // it.
}

int server_add_client(server_t *server, join_t *join)
{
  client_t client_actual;
  client_t* client = &client_actual;

  int ret1 = snprintf(client->name, MAXPATH, "%s",join->name);                          //Adds a client to the server according to the parameter join which
  check_fail(ret1 < 0, 1, "client name initialization failed\n");

  int ret2 = snprintf(client->to_client_fname,MAXPATH, "%s",join->to_client_fname);    // should have fileds such as name filed in.
  check_fail(ret2 < 0, 1, "to_client_fname initialization failed\n");

  int ret3 = snprintf(client->to_server_fname,MAXPATH, "%s",join->to_server_fname);    //The client data is
  check_fail(ret3 < 0, 1, "to_server_fname initialization failed\n");

  client->to_client_fd = open(client->to_client_fname, O_WRONLY);           // copied into the client[] array and file descriptors are opened for
  check_fail(client->to_client_fd < 0, 1, "to client fifo can't be opened\n");

  client->to_server_fd = open(client->to_server_fname, O_RDONLY);           //its to-server and to-client FIFOs.
  check_fail(client->to_server_fd < 0, 1, "to server fifo can't be opened\n");

  client-> data_ready = 0;                                                  // Initializes the data_ready field for the client to 0

  if(server->n_clients == MAXCLIENTS){
    return 1;                                                         //non-zero if the server as no space for clients (n_clients == MAXCLIENTS).
  }

  server->client[server->n_clients] = *client;
  server->n_clients++;
  return 0;                                                           // Returns 0 on success
}


int server_remove_client(server_t *server, int idx){

  client_t* client = server_get_client(server,idx);

  int close1 = close(client->to_client_fd);                      //Close fifos associated with the client.
  check_fail(close1 < 0, 1, "to client fifo can't be closed\n");
  int close2 = close(client->to_server_fd);
  check_fail(close2 < 0, 1, "to server fifo can't be closed\n");
  int remove1 = remove(client->to_client_fname);                  //remove them
  check_fail(remove1 < 0, 1, "to client name file can't be removed\n");
  int remove2 = remove(client->to_server_fname);
  check_fail(remove2 < 0, 1, "to server name file can't be removed\n");

  for(int i=idx+1; i < server->n_clients;i++){        //Shift the remaining clients to lower indices of the client[]
    server->client[i-1] = *server_get_client(server,i);       // preserving their order in the array
  }

  server->n_clients -- ;                            //decreases n_clients.
  return 0;
}

int server_broadcast(server_t *server, mesg_t *mesg){

  for(int i=0; i < server->n_clients;i++){            // Send the given message to all clients connected to the server by
    client_t* client = server_get_client(server, i);  // writing it to the file descriptors associated with them.
    int ret = write(client->to_client_fd, mesg, sizeof(mesg_t));
    check_fail(ret < 0, 1, "write to client %d failed\n", i);
  }

  // ADVANCED: Log the broadcast message unless it is a PING which
  // should not be written to the log.
  return 0;
}

void server_check_sources(server_t *server){

  fd_set fd_set;								          // Create an fd set containing the join struct
  FD_ZERO(&fd_set);
  FD_SET(server->join_fd, &fd_set);					// Add join struct to fd set
  int max_fd = server->join_fd;						// Retrieve the max fd for the select call

  for(int i = 0; i < server->n_clients; i++){
    client_t *client = server_get_client(server, i);			// Get client information
    FD_SET(client->to_server_fd, &fd_set);					          // Add client's to-server FIFO to fd set
    if(client->to_server_fd > max_fd){					          // If the most recent FIFO has the highest fd #, set it as the max fd
      max_fd = client->to_server_fd;
    }
  }

  int ret = select(max_fd+1, &fd_set, NULL, NULL, NULL);			// Checks if any data is ready on the entered FIFOs

  if(errno == EINTR){
    fprintf(stderr, "the select for server_check_sources received a signal and exited\n");
    return;
  }
  else{
    check_fail(ret < 0, 1, "select call failed\n");			// Check if select() system call returned properly
    if(FD_ISSET(server->join_fd, &fd_set)){				        // If the join_fd has ready information, set join_ready flag to 1
      server->join_ready = 1;
    }
    for(int i = 0; i < server->n_clients; i++){			    // If any client has ready information, set its data_ready flag to 1
      client_t *client = server_get_client(server, i);

      if(FD_ISSET(client->to_server_fd, &fd_set)){
        client->data_ready = 1;
      }
    }
  }
}

int server_join_ready(server_t *server){
  return server->join_ready;
  // Return the join_ready flag from the server which indicates whether
  // a call to server_handle_join() is safe.
}


int server_handle_join(server_t *server){

  join_t join;
  int nbytes = read(server->join_fd, &join, sizeof(join_t));    // read a join request
  check_fail(nbytes < 0, 1, "read for join fifo failed\n");
  server_add_client(server, &join);                             //add the new client to the server

  client_t* client = server_get_client(server, server->n_clients-1);
  mesg_t msg;
  mesg_t* mesg = &msg;
  memset(mesg, 0, sizeof(mesg_t));

  int ret = snprintf(mesg->name, MAXNAME, "%s", client->name);  //set the client name as the sending client
  check_fail(ret < 0, 1, "message name initialization failed\n");
  mesg->kind = BL_JOINED;
  server_broadcast(server, mesg);                              //broadcasts join
  server -> join_ready = 0;                                    //set join_ready flag to 0

  return 0;
}


int server_client_ready(server_t *server, int idx){
    client_t *client = server_get_client(server,idx);
    return client-> data_ready;
    // Return the data_ready field of the given client which indicates
    // whether the client has data ready to be read from it.
}


int server_handle_client(server_t *server, int idx){

  mesg_t msg;
  mesg_t* mesg = &msg;
  client_t* client = server_get_client(server,idx);

  int nbytes = read(client->to_server_fd,mesg,sizeof(mesg_t));
  check_fail(nbytes < 0, 1,"Read for server failed.");

  if (mesg->kind == BL_MESG){                  //normal kind message
    server_broadcast(server,mesg);
    client->data_ready = 0;                         //set flag to 0
  }
  else if(mesg->kind == BL_DEPARTED){                    //departure kind message
    server_remove_client(server,idx);
    server_broadcast(server,mesg);
  }
  // ADVANCED: Update the last_contact_time of the client to the current
  // server time_sec.
  return 0;
}
void server_tick(server_t *server);
// ADVANCED: Increment the time for the server

void server_ping_clients(server_t *server);
// ADVANCED: Ping all clients in the server by broadcasting a ping.

void server_remove_disconnected(server_t *server, int disconnect_secs);
// ADVANCED: Check all clients to see if they have contacted the
// server recently. Any client with a last_contact_time field equal to
// or greater than the parameter disconnect_secs should be
// removed. Broadcast that the client was disconnected to remaining
// clients.  Process clients from lowest to highest and take care of
// loop indexing as clients may be removed during the loop
// necessitating index adjustments.

void server_write_who(server_t *server);
// ADVANCED: Write the current set of clients logged into the server
// to the BEGINNING the log_fd. Ensure that the write is protected by
// locking the semaphore associated with the log file. Since it may
// take some time to complete this operation (acquire semaphore then
// write) it should likely be done in its own thread to preven the
// main server operations from stalling.  For threaded I/O, consider
// using the pwrite() function to write to a specific location in an
// open file descriptor which will not alter the position of log_fd so
// that appends continue to write to the end of the file.

void server_log_message(server_t *server, mesg_t *mesg);
// ADVANCED: Write the given message to the end of log file associated
// with the server.
