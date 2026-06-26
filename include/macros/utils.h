#pragma once

#include <assert.h>
#include <stdbool.h>

#define UNREACHABLE(x) assert(false && x);
#define UNUSED(x) (void)(x);
