#ifndef _WORK_QUEUE_H_
#define _WORK_QUEUE_H_

#include "global.h"
#include "helper.h"

class base_query;

struct wq_entry {
  base_query * qry;
  uint64_t starttime;
  struct wq_entry * next;
  struct wq_entry * prev;
};

typedef wq_entry * wq_entry_t;

struct id_entry {
  uint64_t id;
  struct id_entry * next;
};

typedef id_entry * id_entry_t;

class QHash {
public:
  void init();
  void lock();
  void unlock();
  bool in_hash(uint64_t id);
  void add_hash(uint64_t id);
  void update_hash(uint64_t id);
  void remove_hash(uint64_t id);
private:
  uint64_t id_hash_size;
  id_entry_t * id_hash;
  pthread_mutex_t mtx;
};

// Really a linked list
class QWorkQueueHelper {
public:
  void init(QHash * hash);
  bool poll_next_query();
  void finish(uint64_t time); 
  void abort_finish(uint64_t time); 
  void add_query(base_query * qry);
  void add_abort_query(base_query * qry);
  base_query * get_next_query();
  void remove_query(base_query * qry);
  void done(uint64_t id);
  bool poll_abort(); 
  base_query * get_next_abort_query();


private:
  pthread_mutex_t mtx;
  wq_entry_t head;
  wq_entry_t tail;
  uint64_t cnt;
  uint64_t last_add_time;
  QHash * hash;

};


class QWorkQueue {
public:
  void init();
  int inline get_idx(int input);
  bool poll_next_query(int tid);
  void finish(uint64_t time); 
  void abort_finish(uint64_t time); 
  void add_query(int tid, base_query * qry);
  void add_abort_query(int tid, base_query * qry);
  base_query * get_next_query(int tid);
  void remove_query(int tid, base_query * qry);
  void update_hash(int tid, uint64_t id);
  void done(int tid, uint64_t id);
  bool poll_abort(int tid); 
  base_query * get_next_abort_query(int tid);


private:
  QWorkQueueHelper * queue;
  QHash * hash;
  int q_len;

};


#endif
