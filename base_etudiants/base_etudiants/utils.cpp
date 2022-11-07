#include <unistd.h>
#include <sys/mman.h>

#include "utils.h"
#include "query.h"
#include "student.h"

size_t safe_read(int fd, void* buffer, size_t nbytes) {
  ssize_t nbytes_read = read(fd, buffer, nbytes);
  if (nbytes_read < 0) {
    perror("Read pipe error");
    exit(1);
  }
  return (size_t)nbytes_read;
}

size_t safe_write(int fd, const void* buffer, size_t nbytes) {
  ssize_t bytes_written = write(fd, buffer, nbytes);
  if (bytes_written < 0) {
    perror("Write pipe error");
    exit(1);
  }
  return (size_t)bytes_written;
}

void log_query(query_result_t* result) {
  char buffer[512];
  if (result->status == QUERY_SUCCESS) {
    char filename[512];
    char type[256];
    strcpy(type, result->query);
    type[6] = '\0';
    sprintf(filename, "logs/%ld-%s.txt", result->start_ns, type);
    printf("%s\n", filename);
    FILE* f = fopen(filename, "w");
    float duration = (float)(result->end_ns - result->start_ns) / 1.0e6;
    sprintf(buffer, "Query \"%s\" completed in %fms with %d results.\n", result->query, duration, result->count);
    fwrite(buffer, sizeof(char), strlen(buffer), f);
    if (result->count > 0) {
      for (int i = 0; i < result->count; i++) {
        student_to_str(buffer, &result->students[i]);
        fwrite(buffer, sizeof(char), strlen(buffer), f);
        fwrite("\n", sizeof(char), 1, f);
      }
    }
    fclose(f);
  } else if (result->status == UNRECOGNISED_FIELD) {
    fprintf(stderr, "Unrecognized field in query %s\n", result->query);
  }
}

void* create_shared_memory(size_t size) {
  const int protection = PROT_READ | PROT_WRITE;
  const int visibility = MAP_SHARED | MAP_ANONYMOUS;
  return mmap(NULL, size, protection, visibility, -1, 0);
}
