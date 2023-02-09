#include <stdio.h>

typedef enum ErrorCode {
  E_NOT_SUPPORT = -1,
  E_OK = 0, // 成功
} ErrorCode_E;

struct ErrorDescriptionTable {
  ErrorCode_E error_num;
  const char *description;
};

static const struct ErrorDescriptionTable kErrorDescriptionMap[] = {
    {E_OK, "Success"}, {E_NOT_SUPPORT, "Not Support"}};

const char *get_error_description(ErrorCode_E e) {
  int i = 0;
  for (i = 0;
       i < sizeof(kErrorDescriptionMap) / sizeof(kErrorDescriptionMap[0]);
       i++) {
    if (kErrorDescriptionMap[i].error_num == e) {
      return kErrorDescriptionMap[i].description;
    }
  }
  return "Error code unknown";
}

int main(int argc, char const *argv[]) {
  printf("Code:%s\n", get_error_description(E_NOT_SUPPORT));
  return 0;
}
