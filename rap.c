#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 4096
#define DEFAULT_WIDTH 100

char inbuf[BUFSIZE];
int in_size; //bytes read()
int in_last_copied_index = -1;

// We are required to do some amount of buffering because you need to know
// whether the current word is going to exceed the max line length *before*
// you print it, and input buffer edges don't align with word edges.
char outbuf[BUFSIZE];
int out_index = -1;

int copy_size;

int cur_line_length = 0;
int max_width = DEFAULT_WIDTH;


void buffer(int copy_size) {
	memcpy(&outbuf[out_index+1], &inbuf[in_last_copied_index+1], copy_size);
	in_last_copied_index += copy_size;
	out_index += copy_size;
}


void flush() {
	write(1, outbuf, out_index + 1);
	out_index = -1;
	copy_size = 0;
}


int main(void) {
	// disable buffering
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);

	char* rapwidth_str = getenv("RAPWIDTH");
	if (rapwidth_str != NULL) {
		max_width = atoi(rapwidth_str);
		if (max_width < 1) {
			max_width = DEFAULT_WIDTH;
			printf("Invalid RAPWIDTH! Defaulting to %d\n\n", max_width);
		}
	}

	while((in_size = read(0, inbuf, BUFSIZE)) > 0) {
		copy_size = 0;
		in_last_copied_index = -1;
		for(int i=0; i<in_size; i++) {
			copy_size++;
			if (inbuf[i] == '\n') {
				if (cur_line_length + copy_size + 1 > max_width) {
					// Writing the current buffer would exceed the line limit.
					// (e.g. long word ending with '\n'.)
					// Instead, write an additional \n before flushing.
					write(1, "\n", 1);
				}
				// copy and flush
				buffer(copy_size);
				flush();
				cur_line_length = 0;
			}
			if (inbuf[i] == ' ') {
				//consider linebreak
				if (cur_line_length + copy_size + 1 > max_width) {
					write(1, "\n", 1);
					cur_line_length = 0;
				} else {
					cur_line_length += copy_size;
				}
				// either way, copy and flush
				buffer(copy_size);
				flush();
			}
			if (i == in_size-1) {
				//end of this buffer, copy what's left
				if (inbuf[i] == 0) {
					// don't print the record-separating null.
					copy_size -= 1;
				}
				buffer(copy_size);
				cur_line_length += copy_size;
			}
		}
	}
	if (out_index > 0) {
		// there was more output
		flush();
	}
	write(1, "\n", 1);
	return 0;
}
