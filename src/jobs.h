
#include <sys/types.h>

typedef struct job_node job_node;

job_node* prune_jobs(job_node* head);

job_node* job_add(job_node* head, pid_t pid, char* name);
job_node* job_next(job_node* node);
pid_t job_pid(job_node* node);
char* job_name(job_node* node);

