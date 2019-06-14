#include"blather.h"

int signalled = 0;                  //indicates if the server should shut doawn at next opportunity

void handle_signals(int sig_num){
  signalled = 1;
}

int main(int argc, char *argv[]){

  struct sigaction my_sig = {
    .sa_handler = handle_signals,
    .sa_flags = SA_RESTART,
  };
  sigaction(SIGTERM, &my_sig, NULL);  //handle SIGTERM and
  sigaction(SIGINT, &my_sig, NULL);   //SIGINT by shutting down

  server_t serve;
  server_t* server = &serve;

  server_start(server, argv[1], DEFAULT_PERMS);

  while(!signalled){

    server_check_sources(server);              //check all sources

    if(server_join_ready(server)){            //handle a join request if on is ready
      server_handle_join(server);
    }
    for(int i=0; i < server->n_clients;i++){  //for each client{

      if(server_client_ready(server,i)){      //if the client is ready handle data from it
        server_handle_client(server,i);
      }
    }
  }

  server_shutdown(server);
  return 0 ;
}
