/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Purpose:     A simple virtual object layer (VOL) connector that maps HDF5
 *              API calls to file system objects like directories and flat
 *              text files.
 *
 *              Used with the VOL tutorial presentation
 */

#ifndef _tutorial_vol_connector_H
#define _tutorial_vol_connector_H

/* The value must be between 256 and 65535 (inclusive) */
#define TUTORIAL_VOL_CONNECTOR_VALUE    ((H5VL_class_value_t)198)
#define TUTORIAL_VOL_CONNECTOR_NAME     "tutorial_vol_connector"

#endif /* _tutorial_vol_connector_H */

