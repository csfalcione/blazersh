
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "jobs.h"

typedef struct job_node {
    pid_t pid;
    char* display_name;
    job_node* next;
    job_node* prev;
} job_node;

job_node* job_create(pid_t pid, char* name) {
    job_node* new_job = malloc(sizeof(job_node));
    new_job->pid = pid;
    new_job->display_name = name;
    new_job->prev = NULL;
    new_job->next = NULL;
    return new_job;
}

void job_free(job_node* node) {
    free(node->display_name);
    free(node);
}

job_node* prune_jobs(job_node* head) {
    job_node* cursor = head;
    int status;
    while (cursor != NULL) {
        pid_t waitres = waitpid(cursor->pid, &status, WNOHANG);
        if (waitres <= 0) { // job still running or error
            cursor = cursor->next;
            continue;
        }
        int exit_status;
        if (!WIFEXITED(status) && (exit_status = WEXITSTATUS(status)) != 0 ) {
            fprintf(stderr, "job %d (%s) didn't exit normally with status %d\n", cursor->pid, cursor->display_name, exit_status);
        }

        if (cursor->prev != NULL) {
            cursor->prev->next = cursor->next;
        }
        else {
            head = cursor->next;
        }
        if (cursor->next != NULL) {
            cursor->next->prev = cursor->prev;
        }

        job_node* old_cursor = cursor;
        cursor = cursor->next;
        job_free(old_cursor);
    }
    return head;
}

job_node* job_add(job_node* head, pid_t pid, char* name) {
    job_node* new = job_create(pid, name);
    if (head == NULL) {
        return new;
    }

    new->next = head;
    head->prev = new;
    return new;
}

job_node* job_next(job_node* node) {
    return node->next;
}

pid_t job_pid(job_node* node) {
    return node->pid;
}

char* job_name(job_node* node) {
    return node->display_name;
}

