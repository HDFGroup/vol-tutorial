/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Purpose:     Internal functionality for a simple tutorial virtual object
 *              layer (VOL) connector
 */

#ifndef TUTORIAL_INTERNAL_H
#define TUTORIAL_INTERNAL_H

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-result"
//#pragma clang diagnostic ignored "-Wunused-parameter"

#include <dirent.h>
#include <hdf5.h>
#include <stdio.h>

struct tutorial_file;
struct tutorial_object;

struct tutorial_file {
    /* The file's name */
    char *filename;
};

struct tutorial_object {
    /* Dataset or group (and eventually datatype) */
    H5I_type_t type;

    /* The name of this object */
    char *name;

    /* The full path to the object, including the name, as opened by the user */
    char *path;

    /* Data union goes here */
};

/* File callbacks */
void * tutorial_file_create(const char *name, unsigned flags, hid_t fcpl_id, hid_t fapl_id, hid_t dxpl_id,
                            void **req);
void * tutorial_file_open(const char *name, unsigned flags, hid_t fapl_id, hid_t dxpl_id, void **req);
herr_t tutorial_file_specific(void *obj, H5VL_file_specific_args_t *args, hid_t dxpl_id, void **req);
herr_t tutorial_file_close(void *file, hid_t dxpl_id, void **req);

/* Introspect callbacks */
herr_t tutorial_introspect_opt_query(void *obj, H5VL_subclass_t subcls, int opt_type, uint64_t *flags);

#endif /* TUTORIAL_INTERNAL_H */
