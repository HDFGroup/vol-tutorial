/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Purpose:     Dataset functionality for a simple tutorial virtual object
 *              layer (VOL) connector
 */

#include <dirent.h>
#include <hdf5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "tutorial_internal.h"
#include "tutorial_util.h"

/* File extensions for dataset components */
#define SPACE_EXT   "dataspace"
#define DATA_EXT    "data"
#define TYPE_EXT    "datatype"
#define FILLVAL_EXT "fillval"

/*************/
/* DATASPACE */
/*************/

static void
read_dataspace_file(struct tutorial_object *obj)
{
    struct tutorial_dataset *dset = &(obj->data.dataset);

    /* Seek to the beginning */
    rewind(dset->space_file);

    /* Extract the value */
    fscanf(dset->space_file, "%" PRIuHSIZE "\n", &(dset->dims));
}

static void
write_dataspace_file(struct tutorial_object *obj, hsize_t dims)
{
    int                      fd;
    struct tutorial_dataset *dset = &(obj->data.dataset);

    /* Seek to the beginning */
    rewind(dset->space_file);

    /* Truncate the file */
    fd = fileno(dset->space_file);
    ftruncate(fd, 0);

    /* Write out the new size */
    fprintf(dset->space_file, "%" PRIuHSIZE "\n", dims);
}

static void
open_dataspace_file(struct tutorial_object *obj, hbool_t create)
{
    char *                   path = NULL;
    const char *             mode = "r+";
    struct tutorial_dataset *dset = &(obj->data.dataset);

    if (create)
        mode = "w";

    path = make_path(obj->path, obj->name, SPACE_EXT);

    dset->space_file = fopen(path, mode);

    if (create)
        write_dataspace_file(obj, 0);
    else
        read_dataspace_file(obj);

    free(path);
}

/************/
/* DATATYPE */
/************/

static void
read_datatype_file(struct tutorial_object *obj)
{
    char *                   path      = NULL;
    FILE *                   type_file = NULL;
    char *                   line      = NULL;
    size_t                   len       = 0;
    ssize_t                  size      = 0;
    struct tutorial_dataset *dset      = &(obj->data.dataset);

    path = make_path(obj->path, obj->name, TYPE_EXT);

    type_file = fopen(path, "r");

    size = getline(&line, &len, type_file);

    if (strncmp(line, TUTORIAL_DATA_TYPE_INT_STRING, (size_t)size - 1) == 0)
        dset->type = TUTORIAL_DATA_TYPE_INT;
    else
        dset->type = TUTORIAL_DATA_TYPE_FLOAT;

    fclose(type_file);

    free(line);
    free(path);
}

static void
write_datatype_file(struct tutorial_object *obj, enum tutorial_data_type type)
{
    char *path      = NULL;
    FILE *type_file = NULL;

    path = make_path(obj->path, obj->name, TYPE_EXT);

    type_file = fopen(path, "w");

    if (TUTORIAL_DATA_TYPE_INT == type)
        fprintf(type_file, "%s\n", TUTORIAL_DATA_TYPE_INT_STRING);
    else
        fprintf(type_file, "%s\n", TUTORIAL_DATA_TYPE_FLOAT_STRING);

    fclose(type_file);

    free(path);
}

/**************/
/* FILL VALUE */
/**************/

static void
read_fillval_file(struct tutorial_object *obj)
{
    char *                   path         = NULL;
    FILE *                   fillval_file = NULL;
    struct tutorial_dataset *dset         = &(obj->data.dataset);

    path = make_path(obj->path, obj->name, FILLVAL_EXT);

    fillval_file = fopen(path, "r");

    /* Extract the value */
    fscanf(fillval_file, "%d\n", &(dset->fillval));

    fclose(fillval_file);
    free(path);
}

static void
write_fillval_file(struct tutorial_object *obj, int fillval)
{
    char *                   path         = NULL;
    FILE *                   fillval_file = NULL;
    struct tutorial_dataset *dset         = &(obj->data.dataset);

    path = make_path(obj->path, obj->name, FILLVAL_EXT);

    fillval_file = fopen(path, "w");

    fprintf(fillval_file, "%d\n", dset->fillval);

    fclose(fillval_file);
    free(path);
}

/******************/
/* DATASET / DATA */
/******************/

static void
write_data(struct tutorial_object *obj, hsize_t n, const int *data)
{
    int                      fd;
    struct tutorial_dataset *dset = &(obj->data.dataset);

    /* Seek to the beginning */
    rewind(dset->data_file);

    /* Truncate the file */
    fd = fileno(dset->data_file);
    ftruncate(fd, 0);

    if (data) {
        for (hsize_t i = 0; i < n; i++)
            fprintf(dset->data_file, "%d\n", data[i]);
    }
    else {
        /* Special initial dataset fill value case */
        for (hsize_t i = 0; i < n; i++)
            fprintf(dset->data_file, "%d\n", dset->fillval);
    }
}

static void
read_data(struct tutorial_object *obj, hsize_t n, int *data)
{
    struct tutorial_dataset *dset = &(obj->data.dataset);

    /* Seek to the beginning */
    rewind(dset->data_file);

    for (hsize_t i = 0; i < n; i++)
        (void)fscanf(dset->data_file, "%d\n", &(data[i]));
}

static struct tutorial_object *
create_dataset(struct tutorial_object *parent, const char *name, hid_t sid, hid_t tid, hid_t dcpl_id)
{
    struct tutorial_object *obj  = NULL;
    const char *            mode = "w+";
    char *                  path = NULL;
    enum tutorial_data_type type;
    hsize_t                 maxdims;

    /* Create a new dataset object */
    obj = make_object(H5I_DATASET, parent->path, name);

    struct tutorial_dataset *dset = &(obj->data.dataset);

    /* We put all the dataset stuff in a directory.
     * This would be helpful if we implemented links.
     */
    mkdir(obj->path, 0700);

    /* Get the number of elements in the memory space */
    H5Sget_simple_extent_dims(sid, &(dset->dims), &maxdims);

    /* Create the dataspace file and write the size to it */
    open_dataspace_file(obj, true);
    write_dataspace_file(obj, dset->dims);

    /* Get the fill value */
    H5Pget_fill_value(dcpl_id, H5T_NATIVE_INT, &(dset->fillval));
    write_fillval_file(obj, dset->fillval);

    /* Create the type file */
    if (tid == H5T_NATIVE_INT)
        type = TUTORIAL_DATA_TYPE_INT;
    else
        type = TUTORIAL_DATA_TYPE_FLOAT;
    write_datatype_file(obj, type);

    /* Create the data file */
    path            = make_path(obj->path, obj->name, DATA_EXT);
    dset->data_file = fopen(path, mode);
    free(path);

    /* Write fill value data */
    write_data(obj, dset->dims, NULL);

    return obj;
}

static struct tutorial_object *
open_dataset(struct tutorial_object *parent, const char *name)
{
    struct tutorial_object *obj  = NULL;
    const char *            mode = "r+";
    char *                  path = NULL;

    /* Create a new dataset object */
    obj = make_object(H5I_DATASET, parent->path, name);

    struct tutorial_dataset *dset = &(obj->data.dataset);

    /* Read the dataspace file */
    open_dataspace_file(obj, false);

    /* Get the fill value from the file */
    read_fillval_file(obj);

    /* Read the datatype file */
    read_datatype_file(obj);

    /* Create or open the data file */
    path            = make_path(obj->path, obj->name, DATA_EXT);
    dset->data_file = fopen(path, mode);
    free(path);

    return obj;
}

/*************/
/* CALLBACKS */
/*************/

void *
tutorial_dataset_create(void *_parent, const H5VL_loc_params_t *loc_params, const char *name, hid_t lcpl_id,
                        hid_t type_id, hid_t space_id, hid_t dcpl_id, hid_t dapl_id, hid_t dxpl_id,
                        void **req)
{
    struct tutorial_object *new_obj = NULL;
    struct tutorial_object *parent  = NULL;

    /* Get the parent group */
    if (H5I_FILE == loc_params->obj_type) {
        struct tutorial_file *f = (struct tutorial_file *)_parent;
        parent                  = f->root;
    }
    else {
        parent = (struct tutorial_object *)_parent;
    }

    /* Create the dataset */
    new_obj = create_dataset(parent, name, space_id, type_id, dcpl_id);

    return new_obj;
}

void *
tutorial_dataset_open(void *_parent, const H5VL_loc_params_t *loc_params, const char *name, hid_t dapl_id,
                      hid_t dxpl_id, void **req)
{
    struct tutorial_object *new_obj = NULL;
    struct tutorial_object *parent  = NULL;

    /* Get the parent group */
    if (H5I_FILE == loc_params->obj_type) {
        struct tutorial_file *f = (struct tutorial_file *)_parent;
        parent                  = f->root;
    }
    else {
        parent = (struct tutorial_object *)_parent;
    }

    /* Open the dataset */
    new_obj = open_dataset(parent, name);

    return (void *)new_obj;
}

herr_t
tutorial_dataset_read(void *_obj, hid_t mem_type_id, hid_t mem_space_id, hid_t file_space_id, hid_t dxpl_id,
                      void *buf, void **req)
{
    struct tutorial_object * obj  = (struct tutorial_object *)_obj;
    struct tutorial_dataset *dset = &(obj->data.dataset);

    /* Assuming H5S_ALL for now */

    read_data(obj, dset->dims, (int *)buf);

    return 0;
}

herr_t
tutorial_dataset_write(void *_obj, hid_t mem_type_id, hid_t mem_space_id, hid_t file_space_id, hid_t dxpl_id,
                       const void *buf, void **req)
{
    struct tutorial_object *obj = (struct tutorial_object *)_obj;
    hsize_t                 dims;
    hsize_t                 maxdims;

    /* Get the number of elements in the memory space */
    H5Sget_simple_extent_dims(mem_space_id, &dims, &maxdims);

    /* Write out the data */
    write_data(obj, dims, (const int *)buf);

    /* Write out the new dataspace */
    write_dataspace_file(obj, dims);

    return 0;
}

herr_t
tutorial_dataset_close(void *_obj, hid_t dxpl_id, void **req)
{
    struct tutorial_object * obj  = (struct tutorial_object *)_obj;
    struct tutorial_dataset *dset = &(obj->data.dataset);

    /* Close the dataset's files */
    fclose(dset->data_file);
    fclose(dset->space_file);

    /* Destroy the object */
    destroy_object(&obj);

    return 0;
}
