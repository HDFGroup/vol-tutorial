/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://www.hdfgroup.org/licenses.               *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
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
