#ifndef KEYBOARD_H_
#define KEYBOARD_H_

struct keyboard_buffer{
  unsigned char key[128];
  uint32_t head;
  uint32_t tail;
};

struct keyboard_buffer k_buff;

void keyboard_handler(struct keyboard_buffer *k_buff);
void print_c(struct keyboard_buffer *k_buff);

#endif
