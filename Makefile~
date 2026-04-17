CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

SRC_DIR = src
TEST_DIR = tests

QUEUE_OBJ = $(SRC_DIR)/queue.o
MUTEX_SIM_OBJ = $(SRC_DIR)/mutex_sim.o
DLIST_OBJ = $(SRC_DIR)/dlist.o

TEST_GQUEUE_OBJ = $(TEST_DIR)/test_gqueue.o
TEST_LAMPORT_OBJ = $(TEST_DIR)/test_lamport.o
TEST_DLIST_OBJ = $(TEST_DIR)/test_dlist.o

TARGETS = test_gqueue test_lamport_clock test_dlist

.PHONY: all clean

all: $(TARGETS)

test_gqueue: $(QUEUE_OBJ) $(TEST_GQUEUE_OBJ)
	$(CC) $(CFLAGS) -o $@ $(QUEUE_OBJ) $(TEST_GQUEUE_OBJ)

test_lamport: $(QUEUE_OBJ) $(MUTEX_SIM_OBJ) $(TEST_LAMPORT_OBJ)
	$(CC) $(CFLAGS) -o $@ $(QUEUE_OBJ) $(MUTEX_SIM_OBJ) $(TEST_LAMPORT_OBJ)

test_dlist: $(DLIST_OBJ) $(TEST_DLIST_OBJ)
	$(CC) $(CFLAGS) -o $@ $(DLIST_OBJ) $(TEST_DLIST_OBJ)

$(SRC_DIR)/dlist.o: $(SRC_DIR)/dlist.c include/dlist.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_DIR)/test_dlist.o: $(TEST_DIR)/test_dlist.c include/dlist.h
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/queue.o: $(SRC_DIR)/queue.c include/queue.h
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/mutex_sim.o: $(SRC_DIR)/mutex_sim.c include/mutex_sim.h include/queue.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_DIR)/test_gqueue.o: $(TEST_DIR)/test_gqueue.c include/queue.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_DIR)/test_lamport.o: $(TEST_DIR)/test_lamport.c include/mutex_sim.h include/queue.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o $(TEST_DIR)/*.o $(TARGETS)
