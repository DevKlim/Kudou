#ifndef WSLAY_QUEUE_H
#define WSLAY_QUEUE_H

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */

#include <wslay/wslay.h>

struct wslay_queue_entry {
  struct wslay_queue_entry *next;
};

struct wslay_queue {
  struct wslay_queue_entry *top;
  struct wslay_queue_entry **tail;
};

void wslay_queue_init(struct wslay_queue *queue);
void wslay_queue_deinit(struct wslay_queue *queue);
void wslay_queue_push(struct wslay_queue *queue, struct wslay_queue_entry *ent);
void wslay_queue_push_front(struct wslay_queue *queue,
                            struct wslay_queue_entry *ent);
void wslay_queue_pop(struct wslay_queue *queue);
struct wslay_queue_entry *wslay_queue_top(struct wslay_queue *queue);
struct wslay_queue_entry *wslay_queue_tail(struct wslay_queue *queue);
int wslay_queue_empty(struct wslay_queue *queue);

#endif /* WSLAY_QUEUE_H */
