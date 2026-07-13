#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "so_util.h"

namespace imports {
extern FILE* stderr_fake;
extern so::DynLibFunction dynlib_functions[];
extern size_t dynlib_numfunctions;

void update_imports(void);
}