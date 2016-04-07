CXX = gcc
CXXFLAGS += -g -Wall -Wextra

NGX_ROOT = ../nginx-1.0.14_comment

TARGETS = ngx_pool_t_test
TARGETS_C_FILE = $(TARGETS).c

CLEANUP = rm -f $(TARGETS) *.o

all: $(TARGETS)

clean:
	$(CLEANUP)

CORE_INCS = -I. \
	-I$(NGX_ROOT)/src/core \
	-I$(NGX_ROOT)/src/event \
	-I$(NGX_ROOT)/src/event/modules \
	-I$(NGX_ROOT)/src/os/unix \
	-I$(NGX_ROOT)/objs \

NGX_PALLOC = $(NGX_ROOT)/src/core/ngx_palloc.c
NGX_STRING = $(NGX_ROOT)/src/core/ngx_string.c
NGX_ALLOC = $(NGX_ROOT)/src/os/unix/ngx_alloc.c

$(TARGETS): $(TARGETS_C_FILE)
	$(CXX) $(CXXFLAGS) -o $@ $(NGX_PALLOC) $(NGX_STRING) $(NGX_ALLOC) $^ $(CORE_INCS)
	#$(CXX) $(CXXFLAGS) $(CORE_INCS) $(NGX_PALLOC) $(NGX_STRING) $(NGX_ALLOC) $^ -o $@
