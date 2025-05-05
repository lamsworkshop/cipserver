#define MAX 130

#define LENGTH(X) ((unsigned char*)X)[-1]

struct __attribute__((packed)) Message {  // no padding between length and data
  unsigned char length;
  char data[MAX];
  int flush(void);
  void add(char);
};
