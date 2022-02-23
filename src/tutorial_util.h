/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Purpose:     Utility functions for a simple tutorial virtual object layer
 *              (VOL) connector
 */

#ifndef TUTORIAL_UTIL_H
#define TUTORIAL_UTIL_H

#include <hdf5.h>

#include "tutorial_internal.h"

char *                  make_path(const char *component1, const char *component2, const char *ext);
struct tutorial_object *make_object(H5I_type_t type, const char *parent_path, const char *name);
void                    destroy_object(struct tutorial_object **obj);

#endif /* TUTORIAL_UTIL_H */
