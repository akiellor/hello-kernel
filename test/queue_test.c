#include <tap.h>

typedef struct Queue {
  int capacity;
  int size;
  int tail;
  int head;
  int items[];
} Queue;

enum Result {
  ENQUEUED,
  NOT_ENQUEUED
};

struct Queue* create_queue(int capacity) {
  struct Queue *q = malloc(sizeof(*q) + capacity * sizeof q->items[0]);
  q->capacity = capacity;
  q->size = 0;
  q->tail = 0;
  q->head = 0;
  return q;
}

enum Result enqueue(struct Queue *q, int n) {
  if (q->size == q->capacity) {
    return NOT_ENQUEUED;
  }
  q->size++;
  q->items[q->tail] = n;
  q->tail = (q->tail + 1) % q->capacity;
  return ENQUEUED;
}

int dequeue(struct Queue *q) {
  int result = q->items[q->head];
  q->size--;
  q->head = (q->head + 1) % q->capacity;
  return result;
}

#define PRINT_OPAQUE_STRUCT(p)  print_mem((p), sizeof(*(p)))

void print_mem(void const *vp, size_t n)
{
  unsigned char const *p = vp;
  for (size_t i=0; i<n; i++)
    printf("%02x\n", p[i]);
  putchar('\n');
};

void check_enqueued(Queue* sut, int n) {
  cmp_ok(enqueue(sut, n), "==", ENQUEUED, "Expected to be enqueued");
}

void check_dequeued(Queue* sut, int expectation) {
  cmp_ok(dequeue(sut), "==", expectation, "Expected to be dequeued");
}

void test_basics() {
  struct Queue* sut = create_queue(3);

  check_enqueued(sut, 1);
  check_enqueued(sut, 2);
  check_enqueued(sut, 3);

  check_dequeued(sut, 1);
  check_dequeued(sut, 2);
  check_dequeued(sut, 3);
}

void test_overflow() {
  struct Queue* sut = create_queue(1);

  check_enqueued(sut, 1);
  cmp_ok(enqueue(sut, 2), "==", NOT_ENQUEUED);
  check_dequeued(sut, 1);
}

void test_repeats() {
  struct Queue* sut = create_queue(1);

  check_enqueued(sut, 1);
  check_dequeued(sut, 1);

  check_enqueued(sut, 2);
  check_dequeued(sut, 2);

  check_enqueued(sut, 3);
  check_dequeued(sut, 3);

  check_enqueued(sut, 4);
  check_dequeued(sut, 4);
}

int main () {
  test_basics();
  test_overflow();
  test_repeats();
  done_testing();
}
