CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

SRC_DIR = src
TEST_DIR = tests

QUEUE_OBJ = $(SRC_DIR)/queue.o
MUTEX_SIM_OBJ = $(SRC_DIR)/mutex_sim.o

TEST_GQUEUE_OBJ = $(TEST_DIR)/test_gqueue.o
TEST_LAMPORT_OBJ = $(TEST_DIR)/test_lamport_clock.o

TARGETS = test_gqueue test_lamport_clock

.PHONY: all clean

all: $(TARGETS)

test_gqueue: $(QUEUE_OBJ) $(TEST_GQUEUE_OBJ)
	$(CC) $(CFLAGS) -o $@ $(QUEUE_OBJ) $(TEST_GQUEUE_OBJ)

test_lamport_clock: $(QUEUE_OBJ) $(MUTEX_SIM_OBJ) $(TEST_LAMPORT_OBJ)
	$(CC) $(CFLAGS) -o $@ $(QUEUE_OBJ) $(MUTEX_SIM_OBJ) $(TEST_LAMPORT_OBJ)

$(SRC_DIR)/queue.o: $(SRC_DIR)/queue.c include/queue.h
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/mutex_sim.o: $(SRC_DIR)/mutex_sim.c include/mutex_sim.h include/queue.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_DIR)/test_gqueue.o: $(TEST_DIR)/test_gqueue.c include/queue.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_DIR)/test_lamport_clock.o: $(TEST_DIR)/test_lamport_clock.c include/mutex_sim.h include/queue.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o $(TEST_DIR)/*.o $(TARGETS)
