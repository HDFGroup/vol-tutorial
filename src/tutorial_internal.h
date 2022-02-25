/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Purpose:     Internal functionality for a simple tutorial virtual object
 *              layer (VOL) connector
 */

#ifndef TUTORIAL_INTERNAL_H
#define TUTORIAL_INTERNAL_H

#include <hdf5.h>

struct tutorial_object;

struct tutorial_object {
    /* Dataset or group (and eventually datatype) */
    H5I_type_t type;

    /* The name of this object */
    char *name;

    /* The full path to the object, including the name, as opened by the user */
    char *path;

    /* Data union goes here */
};

#endif /* TUTORIAL_INTERNAL_H */
