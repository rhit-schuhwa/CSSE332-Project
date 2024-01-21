#ifndef __LOG_H
#define __LOG_H

// colored output
#define RED   "\033[0;31m"
#define BLUE  "\033[0;34m"
#define GREEN "\033[0;32m"
#define RESET "\033[0m]"

#define print_red(fmt, ...) do { \
  printf(RED);                   \
  printf(fmt, ##__VA_ARGS__);    \
}while(0)

#define print_green(fmt, ...) do { \
  printf(GREEN);                   \
  printf(fmt, ##__VA_ARGS__);      \
}while(0)

#define print_blue(fmt, ...) do { \
  printf(BLUE);                   \
  printf(fmt, ##__VA_ARGS__);     \
}while(0)


#endif /* log.h */
