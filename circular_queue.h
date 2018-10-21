#ifndef __circular_queue_h__
#define __circular_queue_h__

int enqueue(char *arr_ptr, char data, uint32_t *curr_pos, uint32_t *curr_size, uint32_t max_size) {
	if (*curr_size == max_size)
		return -1;

	if (*curr_pos == 0xFFFFFFFF) {
		*curr_pos = 0;
	}/* else {
        curr_pos = (curr_pos+1) % max_size;
    }*/
	(*curr_size)++;

	arr_ptr[(*curr_pos + *curr_size - 1) % max_size] = data;

	return 0;
}

int enqueue_overwrite_last(char *arr_ptr, char data, uint32_t *curr_pos, uint32_t *curr_size, uint32_t max_size) {
	if (*curr_size != max_size)
		(*curr_size)++;

	if (*curr_pos == 0xFFFFFFFF) {
		*curr_pos = 0;
	}/* else {
        curr_pos = (curr_pos+1) % max_size;
    }*/

	arr_ptr[(*curr_pos + *curr_size - 1) % max_size] = data;

	return 0;
}

int enqueue_overwrite_shift(char *arr_ptr, char data, uint32_t *curr_pos, uint32_t *curr_size, uint32_t max_size) {
	if (*curr_size != max_size)
		(*curr_size)++;
	else
		*curr_pos = (*curr_pos + 1) % max_size;

	if (*curr_pos == 0xFFFFFFFF) {
		*curr_pos = 0;
	}/* else {
        curr_pos = (curr_pos+1) % max_size;
    }*/

	

	arr_ptr[(*curr_pos + *curr_size - 1) % max_size] = data;

	return 0;
}

int dequeue(char *arr_ptr, char *data, uint32_t *curr_pos, uint32_t *curr_size, uint32_t max_size) {
	if (*curr_size == 0) {
		return -1;
	}

	*data = arr_ptr[*curr_pos];

	*curr_pos = (*curr_pos + 1) % max_size;
	(*curr_size)--;

	return 0;
}

void reset_queue(uint32_t *curr_pos, uint32_t *curr_size) {
	(*curr_pos) = 0xFFFFFFFF;
	(*curr_size) = 0;
}

#endif
