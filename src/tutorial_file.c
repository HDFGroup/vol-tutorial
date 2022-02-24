/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Purpose:     File functionality for a simple tutorial virtual object
 *              layer (VOL) connector
 */

#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <hdf5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "tutorial_internal.h"
#include "tutorial_util.h"

#define MARKER_FILE_NAME "TUTORIAL_VOL_CONNECTOR_FILE"

static void
add_hdf5_marker(const char *filename)
{
    char *path = NULL;
    FILE *f    = NULL;

    path = make_path(filename, MARKER_FILE_NAME, NULL);

    f = fopen(path, "w");
    fclose(f);

    free(path);
}

static hbool_t
check_hdf5_marker(const char *filename)
{
    char *      path = NULL;
    struct stat st;
    hbool_t     ret = true;

    path = make_path(filename, MARKER_FILE_NAME, NULL);

    if (stat(path, &st) == -1)
        ret = false;

    free(path);

    return ret;
}

static int
remove_callback(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb)
{
    remove(pathname);
    return 0;
}

static void
recursive_removal(const char *root)
{
    nftw(root, remove_callback, 10, FTW_DEPTH | FTW_MOUNT | FTW_PHYS);
}

void *
tutorial_file_create(const char *name, unsigned flags, hid_t fcpl_id, hid_t fapl_id, hid_t dxpl_id,
                     void **req)
{
    struct tutorial_file *f = NULL;

    /* Check if this is an HDF5 tutorial file and fail if it already exists */
    if (check_hdf5_marker(name))
        return NULL;

    f = calloc(1, sizeof(struct tutorial_file));

    /* Save this for later */
    f->filename = strdup(name);

    /* Create the root group */
    f->root = init_group(NULL, name, true);

    /* Add a marker that this is an HDF5 file */
    add_hdf5_marker(name);

    return f;
}

void *
tutorial_file_open(const char *name, unsigned flags, hid_t fapl_id, hid_t dxpl_id, void **req)
{
    struct tutorial_file *f = NULL;

    /* Check if this is an HDF5 tutorial file */
    if (!check_hdf5_marker(name))
        return NULL;

    f = calloc(1, sizeof(struct tutorial_file));

    /* Save this for later */
    f->filename = strdup(name);

    /* Open the root group */
    f->root = init_group(NULL, name, false);

    return f;
}

herr_t
tutorial_file_specific(void *obj, H5VL_file_specific_args_t *args, hid_t dxpl_id, void **req)
{
    switch (args->op_type) {
        case H5VL_FILE_DELETE: {
            recursive_removal(args->args.del.filename);
            break;
        }
        case H5VL_FILE_IS_ACCESSIBLE: {
            hbool_t exists;

            exists = check_hdf5_marker(args->args.is_accessible.filename);

            *args->args.is_accessible.accessible = exists;

            break;
        }
        case H5VL_FILE_REOPEN:
        case H5VL_FILE_IS_EQUAL:
        default:
            return -1;
    }

    return 0;
}

herr_t
tutorial_file_close(void *file, hid_t dxpl_id, void **req)
{
    struct tutorial_file *f = (struct tutorial_file *)file;

    /* The root group doesn't have an ID, so we manually close it */
    tutorial_group_close(f->root, dxpl_id, NULL);

    free(f->filename);
    free(f);

    return 0;
}
