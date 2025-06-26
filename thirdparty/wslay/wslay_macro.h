#ifndef WSLAY_MACRO_H
#define WSLAY_MACRO_H

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */

#include <wslay/wslay.h>

#include <stddef.h>

#define wslay_struct_of(ptr, type, member)                                     \
  ((type *)(void *)((char *)(ptr)-offsetof(type, member)))

#endif /* WSLAY_MACRO_H */
