#include "smalldb.hpp"

static volatile std::atomic<int> run(1);
static volatile std::atomic<int> end(0);
static volatile std::atomic<int> queries(0);
std::vector<int> sockets;
static database_t* db = new database_t;


void handler(int sig){
  if(sig == SIGINT){end.fetch_add(1);}
  else if(sig == SIGUSR1){
    run.fetch_sub(0);
    printf("Waiting for all queries to end ...\n");
    while(queries.load() > 0){sleep(1);}
    printf("Saving changes ...\n");
    db_save(db);
    printf("DONE!\n");
    run.fetch_add(1);
    }
}

void writer(thread_args_t* args,char*query){
  pthread_mutex_lock(args->new_access_mutex);
  pthread_mutex_lock(args->write_access_mutex);
  pthread_mutex_unlock(args->new_access_mutex);
  parse_and_execute(args->socket, args->db, query);
  pthread_mutex_unlock(args->write_access_mutex);
}

void reader(thread_args_t* args,char*query){
  pthread_mutex_lock(args->new_access_mutex);
  pthread_mutex_lock(args->reader_registration_mutex);
  if (args->socket,args->readers->load() == 0){pthread_mutex_lock(args->write_access_mutex);}
  args->socket,args->readers->fetch_add(1);
  pthread_mutex_unlock(args->new_access_mutex);
  pthread_mutex_unlock(args->reader_registration_mutex);
  parse_and_execute(args->socket, args->db, query);
  pthread_mutex_lock(args->reader_registration_mutex);
  args->readers->fetch_sub(1);
  if (args->socket,args->readers->load() == 0){pthread_mutex_unlock(args->write_access_mutex);}
  pthread_mutex_unlock(args->reader_registration_mutex);
}

void do_query(char* query, thread_args_t* args, char* c){
  queries.fetch_add(1);
  query[strlen(query)] = '\0';
  int type = parse(query);
  switch (type){
  case 0: 
    //writer ("delete", "update" and "insert")
    writer(args,query);
    break;
  case 1: 
    //reader ("select")
    reader(args,query);
    break;
  case -1:
    // query_type != {"delete","update","insert","select"}
    query_fail_bad_query_type(args->socket);
    break;
  }
  // reset variables
  memset(query,'\0',strlen(query));
  *c='A';
  queries.fetch_sub(1);
}

void close_thread(thread_args_t* args, int test){
  if(end.load() == 1){
    // SIGINT recieved by the main thread
    printf("Closing connection to client %i\n",args->socket);
  }else if (test == 0){
    // Client disconnect
    printf("Client %i disconnected (normal). Closing connection and thread\n",args->socket);
    close(args->socket);
  }else{
    // Error while reading -> lost connection to client
    printf("Lost connection to client %i\n",args->socket);
    printf("Closing connection %i\n",args->socket);
    printf("Closing thread for connection %i\n",args->socket);
    close(args->socket);
  }
  pthread_mutex_lock(args->sockets_mutex);
  auto new_end = std::remove(sockets.begin(),sockets.end(),args->socket);
  sockets.erase(new_end,sockets.end());
  pthread_mutex_unlock(args->sockets_mutex);
  delete(args);
  return;
}

void* client_thread(void * ptr){
  thread_args_t* args = (thread_args_t*) ptr;

  // Add this client socket to the list of sockets
  pthread_mutex_lock(args->sockets_mutex);
  sockets.push_back(args->socket);
  pthread_mutex_unlock(args->sockets_mutex);
  
  char query[1024];
  int test = 1;
  char c;
  int i=0;
  while(test > 0 && end.load() == 0){
    if(run.load() == 1){
      test = read(args->socket,&c,sizeof(char));
      query[i]=c;
      i++;
      if(c == '\0'){
        i=0;
        do_query(query,args,&c);
      }
    }else{
      // When SIGUSR1 is recieved by the main thread then stop recieving new queries
      sleep(1);
    }
  };
  close_thread(args,test);
  return NULL;
}

void setup_signal_handler(){
  struct sigaction action;
  action.sa_handler = handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags =0;
  sigaction(SIGUSR1,&action,NULL);
  sigaction(SIGINT,&action,NULL);
}

sigset_t create_mask(){
  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask,SIGUSR1);
  sigaddset(&mask,SIGINT);
  return mask;
}

thread_args_t* new_args(int s, pthread_mutex_t* nac,pthread_mutex_t* wam,pthread_mutex_t* rrm,pthread_mutex_t* sm, std::atomic<int>* r){
  thread_args_t *args = new thread_args_t[1];
  *args={
    .socket = s,
    .db = db,
    .new_access_mutex = nac,
    .write_access_mutex = wam,
    .reader_registration_mutex = rrm,
    .sockets_mutex = sm,
    .readers = r
  };
  return args;
}

void close_server(int server_fd, pthread_mutex_t sockets_mutex){
  close(server_fd);
  printf("Waiting for all queries to end ...\n");
  while(queries.load() > 0){sleep(1);}
  printf("Waiting for the clients to disconnect...\n");
  pthread_mutex_lock(&sockets_mutex);
  for(int thread_socket : sockets){close(thread_socket);}
  pthread_mutex_unlock(&sockets_mutex);
  bool empty=false;
  while(!empty){
    pthread_mutex_lock(&sockets_mutex);
    empty = sockets.empty();
    pthread_mutex_unlock(&sockets_mutex);
    if(!empty){sleep(1);}
  }
}

void server(database_t* db){
  // Set up signal handler
  setup_signal_handler();
  // Set up masks
  sigset_t mask = create_mask();
  // Creat the four mutex needed and readers
  pthread_mutex_t new_access_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t write_access_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t reader_registration_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t sockets_mutex = PTHREAD_MUTEX_INITIALIZER;
  std::atomic<int> readers(0);
  // Creat socket
  int server_fd = safe_socket(AF_INET, SOCK_STREAM, 0);
  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
  // Creat adress
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(28772);
  safe_bind(server_fd, (struct sockaddr *)&address, sizeof(address));
  safe_listen(server_fd, 3);
  size_t addrlen = sizeof(address);

  while(end.load() == 0){
    int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if(new_socket<0){
      // We ignore the error recieved EINTR recieved when accept is
      // stopped by a signal
      if(errno == EINTR){continue;}
      else{perror("Accept error");exit(1);}
    }
    printf("client %i connecté\n",new_socket);
    pthread_t new_thread;
    // Creat the arguments to pass to the thread
    thread_args_t* args = new_args(new_socket,&new_access_mutex,&write_access_mutex,&reader_registration_mutex,&sockets_mutex,&readers);
    sigprocmask(SIG_BLOCK,&mask,NULL);  // Block signal
    pthread_create(&new_thread,NULL,client_thread,args);  // Create thread
    sigprocmask(SIG_UNBLOCK,&mask,NULL); // Unblock signal
  }
  close_server(server_fd,sockets_mutex);
}

int main(int argc, char const* argv[]) {
  // Check usage
  if(argc != 2){printf("Wrong usage.\ntry smalldb [<path_to_the_database>]\n");exit(1);}
  const char* path = argv[1];
  // Load database
  db_load(db,path);
  server(db);
  printf("Saving changes ...\n");
  db_save(db);
  delete(db);
  printf("BYE!\n");
  return 0;
}