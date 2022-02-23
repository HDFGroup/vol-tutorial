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

struct tutorial_dataset;
struct tutorial_file;
struct tutorial_group;
struct tutorial_link;
struct tutorial_object;

enum tutorial_data_type {
    TUTORIAL_DATA_TYPE_FLOAT,
    TUTORIAL_DATA_TYPE_INT,
};

#define TUTORIAL_DATA_TYPE_FLOAT_STRING "TUTORIAL_DATA_TYPE_FLOAT"
#define TUTORIAL_DATA_TYPE_INT_STRING   "TUTORIAL_DATA_TYPE_INT"

struct tutorial_dataset {
    /* The dataset's data file */
    FILE *data_file;

    /* The dataset's dataspace file */
    FILE *space_file;

    /* Dataspace info */
    hsize_t dims;

    /* Datatype info */
    /* NOT USED AT THIS TIME */
    enum tutorial_data_type type;

    /* The fill value */
    int fillval;
};

struct tutorial_file {
    /* The root group */
    struct tutorial_object *root;

    /* The file's name */
    char *filename;
};

struct tutorial_group {
    /* The directory representing a group */
    DIR *dir;
};

struct tutorial_object {
    /* Dataset or group (and eventually datatype) */
    H5I_type_t type;

    /* The name of this object */
    char *name;

    /* The full path to the object, including the name, as opened by the user */
    char *path;

    /* The object's data */
    union {
        struct tutorial_dataset dataset;
        struct tutorial_group   group;
    } data;
};

/* Dataset callbacks */
void *tutorial_dataset_create(void *obj, const H5VL_loc_params_t *loc_params, const char *name, hid_t lcpl_id,
                              hid_t type_id, hid_t space_id, hid_t dcpl_id, hid_t dapl_id, hid_t dxpl_id,
                              void **req);
void *tutorial_dataset_open(void *obj, const H5VL_loc_params_t *loc_params, const char *name, hid_t dapl_id,
                            hid_t dxpl_id, void **req);
herr_t tutorial_dataset_read(void *obj, hid_t mem_type_id, hid_t mem_space_id, hid_t file_space_id,
                             hid_t dxpl_id, void *buf, void **req);
herr_t tutorial_dataset_write(void *obj, hid_t mem_type_id, hid_t mem_space_id, hid_t file_space_id,
                              hid_t dxpl_id, const void *buf, void **req);
herr_t tutorial_dataset_close(void *dset, hid_t dxpl_id, void **req);

/* File callbacks */
void * tutorial_file_create(const char *name, unsigned flags, hid_t fcpl_id, hid_t fapl_id, hid_t dxpl_id,
                            void **req);
void * tutorial_file_open(const char *name, unsigned flags, hid_t fapl_id, hid_t dxpl_id, void **req);
herr_t tutorial_file_specific(void *obj, H5VL_file_specific_args_t *args, hid_t dxpl_id, void **req);
herr_t tutorial_file_close(void *file, hid_t dxpl_id, void **req);

/* Group callbacks */
void * tutorial_group_create(void *obj, const H5VL_loc_params_t *loc_params, const char *name, hid_t lcpl_id,
                             hid_t gcpl_id, hid_t gapl_id, hid_t dxpl_id, void **req);
void * tutorial_group_open(void *obj, const H5VL_loc_params_t *loc_params, const char *name, hid_t gapl_id,
                           hid_t dxpl_id, void **req);
herr_t tutorial_group_close(void *grp, hid_t dxpl_id, void **req);
/* Group utility function (needed to create root group in file code) */
struct tutorial_object *init_group(struct tutorial_object *parent, const char *name, hbool_t create_on_disk);

/* Introspect callbacks */
herr_t tutorial_introspect_opt_query(void *obj, H5VL_subclass_t subcls, int opt_type, uint64_t *flags);

#endif /* TUTORIAL_INTERNAL_H */
