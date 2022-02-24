/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Purpose:     A simple tutorial virtual object layer (VOL) connector
 */

#include <stddef.h>
#include <stdint.h>

/* For HDF5 plugin functionality */
#include "H5PLextern.h"

/* This connector's header */
#include "tutorial_vol_connector.h"
#include "tutorial_internal.h"

/* The VOL class struct */
static const H5VL_class_t tutorial_vol_g = {
    H5VL_VERSION,                 /* VOL class struct version */
    TUTORIAL_VOL_CONNECTOR_VALUE, /* value            */
    TUTORIAL_VOL_CONNECTOR_NAME,  /* name             */
    0,                            /* connector version */
    0,                            /* capability flags */
    NULL,                         /* initialize       */
    NULL,                         /* terminate        */
    {
        /* info_cls */
        (size_t)0, /* size             */
        NULL,      /* copy             */
        NULL,      /* compare          */
        NULL,      /* free             */
        NULL,      /* to_str           */
        NULL,      /* from_str         */
    },
    {
        /* wrap_cls */
        NULL, /* get_object       */
        NULL, /* get_wrap_ctx     */
        NULL, /* wrap_object      */
        NULL, /* unwrap_object    */
        NULL, /* free_wrap_ctx    */
    },
    {
        /* attribute_cls */
        NULL, /* create           */
        NULL, /* open             */
        NULL, /* read             */
        NULL, /* write            */
        NULL, /* get              */
        NULL, /* specific         */
        NULL, /* optional         */
        NULL  /* close            */
    },
    {
        /* dataset_cls */
        NULL, /* create           */
        NULL, /* open             */
        NULL, /* read             */
        NULL, /* write            */
        NULL, /* get              */
        NULL, /* specific         */
        NULL, /* optional         */
        NULL  /* close            */
    },
    {
        /* datatype_cls */
        NULL, /* commit           */
        NULL, /* open             */
        NULL, /* get_size         */
        NULL, /* specific         */
        NULL, /* optional         */
        NULL  /* close            */
    },
    {
        /* file_cls */
        tutorial_file_create,   /* create           */
        tutorial_file_open,     /* open             */
        NULL,                   /* get              */
        tutorial_file_specific, /* specific         */
        NULL,                   /* optional         */
        tutorial_file_close     /* close            */
    },
    {
        /* group_cls */
        tutorial_group_create, /* create           */
        tutorial_group_open,   /* open             */
        NULL,                  /* get              */
        NULL,                  /* specific         */
        NULL,                  /* optional         */
        tutorial_group_close   /* close            */
    },
    {
        /* link_cls */
        NULL, /* create           */
        NULL, /* copy             */
        NULL, /* move             */
        NULL, /* get              */
        NULL, /* specific         */
        NULL  /* optional         */
    },
    {
        /* object_cls */
        NULL, /* open             */
        NULL, /* copy             */
        NULL, /* get              */
        NULL, /* specific         */
        NULL  /* optional         */
    },
    {
        /* introspect_cls */
        NULL,                          /* get_conn_cls     */
        NULL,                          /* get_cap_flags    */
        tutorial_introspect_opt_query, /* opt_query        */
    },
    {
        /* request_cls */
        NULL, /* wait             */
        NULL, /* notify           */
        NULL, /* cancel           */
        NULL, /* specific         */
        NULL, /* optional         */
        NULL  /* free             */
    },
    {
        /* blob_cls */
        NULL, /* put              */
        NULL, /* get              */
        NULL, /* specific         */
        NULL  /* optional         */
    },
    {
        /* token_cls */
        NULL, /* cmp              */
        NULL, /* to_str           */
        NULL  /* from_str         */
    },
    NULL /* optional         */
};

/* These two functions are necessary to load this plugin using
 * the HDF5 library.
 */

H5PL_type_t
H5PLget_plugin_type(void)
{
    return H5PL_TYPE_VOL;
}
const void *
H5PLget_plugin_info(void)
{
    return &tutorial_vol_g;
}

herr_t
tutorial_introspect_opt_query(void *obj, H5VL_subclass_t subcls, int opt_type, uint64_t *flags)
{
    /* For now, we'll ignore the feature flags */
    *flags = 0;
    return 0;
}
