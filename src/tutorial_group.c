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

/* Purpose:     Group functionality for a simple tutorial virtual object
 *              layer (VOL) connector
 */

#include <dirent.h>
#include <hdf5.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "tutorial_internal.h"
#include "tutorial_util.h"

struct tutorial_object *
init_group(struct tutorial_object *parent, const char *name, hbool_t create_on_disk)
{
    struct tutorial_object *obj = NULL;

    /* NOTE: When the parent object is NULL, we're creating a new root group */

    /* Create the new group structure */
    if (NULL == parent) {
        /* Special case for root group */
        obj = make_object(H5I_GROUP, ".", name);
    }
    else {
        obj = make_object(H5I_GROUP, parent->path, name);
    }

    /* Create the group, if desired */
    if (create_on_disk)
        mkdir(obj->path, 0700);

    struct tutorial_group *group = &(obj->data.group);

    /* Open and store the directory state */
    group->dir = opendir(obj->path);

    return obj;
}

void *
tutorial_group_create(void *obj, const H5VL_loc_params_t *loc_params, const char *name, hid_t lcpl_id,
                      hid_t gcpl_id, hid_t gapl_id, hid_t dxpl_id, void **req)
{
    struct tutorial_object *new_obj = NULL;
    struct tutorial_object *parent  = NULL;

    /* Get the parent group */
    if (H5I_FILE == loc_params->obj_type) {
        struct tutorial_file *f = (struct tutorial_file *)obj;
        parent                  = f->root;
    }
    else {
        parent = (struct tutorial_object *)obj;
    }

    /* Create the group */
    new_obj = init_group(parent, name, true);

    return (void *)new_obj;
}

void *
tutorial_group_open(void *_parent, const H5VL_loc_params_t *loc_params, const char *name, hid_t gapl_id,
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

    /* Open the group */
    new_obj = init_group(parent, name, false);

    return (void *)new_obj;
}

herr_t
tutorial_group_close(void *_obj, hid_t dxpl_id, void **req)
{
    struct tutorial_object *obj   = (struct tutorial_object *)_obj;
    struct tutorial_group * group = &(obj->data.group);

    /* Close the directory */
    closedir(group->dir);

    /* Destroy the object */
    destroy_object(&obj);

    return 0;
}
