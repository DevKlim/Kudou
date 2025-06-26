#include "command_queue_mt.h"

CommandQueueMT::CommandQueueMT() {
	command_mem.reserve(DEFAULT_COMMAND_MEM_SIZE_KB * 1024);
}

CommandQueueMT::~CommandQueueMT() {
}
