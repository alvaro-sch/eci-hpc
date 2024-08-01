CC = mpicc
CFLAGS = -Wall -Wextra -fopenmp
LDFLAGS =

SOURCE_DIR = src
SOURCE = $(wildcard $(SOURCE_DIR)/*.c)
OBJECT = $(SOURCE:.c=.o)

TARGETS = hello_world hello_world_sync pi global_sum \
		  intra_hello_world intra_pi

all: $(TARGETS)

%: $(SOURCE_DIR)/%.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	-rm -f $(TARGETS) $(OBJECT)

.PHONY: clean run

