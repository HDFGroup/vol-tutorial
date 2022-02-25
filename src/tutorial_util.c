/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <hdf5.h>
#include <stdlib.h>
#include <string.h>

#include "tutorial_internal.h"
#include "tutorial_util.h"

char *
make_path(const char *component1, const char *component2, const char *ext)
{
    char * out = NULL;
    char * ptr = NULL;
    size_t len = 0;

    /* length = component 1 + "/" + component 2 + "\0" */
    len += strlen(component1) + strlen(component2) + 2;
    if (ext)
        len += strlen(ext) + 1; /* +1 for "." */

    out = calloc(len, sizeof(char));

    ptr = out;

    ptr = stpcpy(ptr, component1);
    ptr = stpcpy(ptr, "/");
    ptr = stpcpy(ptr, component2);

    if (ext) {
        ptr = stpcpy(ptr, ".");
        ptr = stpcpy(ptr, ext);
    }

    return out;
}

struct tutorial_object *
make_object(H5I_type_t type, const char *parent_path, const char *name)
{
    struct tutorial_object *obj = NULL;

    /* Create the new object */
    obj = calloc(1, sizeof(struct tutorial_object));

    /* Set the object type */
    obj->type = type;

    /* Set the name and path */
    obj->name = strdup(name);
    obj->path = make_path(parent_path, name, NULL);

    return obj;
}

void
destroy_object(struct tutorial_object **obj)
{
    free((*obj)->name);
    free((*obj)->path);
    free(*obj);
}
