#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/Error.h"

void throwError(int code, char *message)
{

  Error Error = {
      .code = code,
      .message = message};

  printf("<Parser> Error: %d with message: %s\n", Error.code, Error.message);
  exit(Error.code);
}
