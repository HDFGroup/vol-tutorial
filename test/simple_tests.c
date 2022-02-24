/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * Purpose:     Tests basic tutorial VOL operations.
 *
 *              Uses the tutorial VOL connector which is loaded as a
 *              dynamically-loaded plugin.
 */

#include <hdf5.h>
#include <stdlib.h>

#include "tutorial_vol_connector.h"

/* herr_t values from H5private.h */
#define SUCCEED    0                                                             
#define FAIL    (-1) 

/* Testing macros from h5test.h
 *
 * The name of the test is printed by saying TESTING("something") which will     
 * result in the string `Testing something' being flushed to standard output.    
 * If a test passes, fails, or is skipped then the PASSED(), H5_FAILED(), or     
 * SKIPPED() macro should be called.  After H5_FAILED() or SKIPPED() the caller  
 * should print additional information to stdout indented by at least four       
 * spaces.  If the h5_errors() is used for automatic error handling then         
 * the H5_FAILED() macro is invoked automatically when an API function fails.    
 */                                                                              

#define AT()                printf ("   at %s:%d...\n", __FILE__, __LINE__);   
#define TESTING(WHAT)       {printf("Testing %-62s", WHAT); fflush(stdout);}       
#define PASSED()            {puts(" PASSED"); fflush(stdout);}                          
#define H5_FAILED()         {puts("*FAILED*"); fflush(stdout);}                      
#define H5_WARNING()        {puts("*WARNING*"); fflush(stdout);}                    
#define SKIPPED()           {puts(" -SKIP-"); fflush(stdout);}                         
#define PUTS_ERROR(s)       {puts(s); AT(); goto error;}                           
#define TEST_ERROR          {H5_FAILED(); AT(); goto error;}                         
#define STACK_ERROR         {H5Eprint2(H5E_DEFAULT, stdout); goto error;}            
#define FAIL_STACK_ERROR    {H5_FAILED(); AT(); H5Eprint2(H5E_DEFAULT, stdout); goto error;}
#define FAIL_PUTS_ERROR(s)  {H5_FAILED(); AT(); puts(s); goto error;} 

/* If true, deletes the generated files. Since we want you to be able to
 * inspect the tutorial HDF5 files, this is set to false by default.
 */
bool DELETE_FILES_g = false;

/*-------------------------------------------------------------------------
 * Function:    test_registration_by_value()
 *
 * Purpose:     Tests if we can load, register, and close a VOL
 *              connector by value.
 *
 * Return:      SUCCEED/FAIL
 *
 *-------------------------------------------------------------------------
 */
static herr_t
test_registration_by_value(void)
{
    htri_t  is_registered   = FAIL;
    hid_t   vol_id          = H5I_INVALID_HID;

    TESTING("VOL registration by value");

    /* The VOL connector should not be registered at the start of the test */
    if((is_registered = H5VLis_connector_registered_by_name(TUTORIAL_VOL_CONNECTOR_NAME)) < 0)
        TEST_ERROR;
    if(true == is_registered)
        FAIL_PUTS_ERROR("VOL connector is inappropriately registered");

    /* Register the connector by value */
    if((vol_id = H5VLregister_connector_by_value(TUTORIAL_VOL_CONNECTOR_VALUE, H5P_DEFAULT)) < 0)
        TEST_ERROR;

    /* The connector should be registered now */
    if((is_registered = H5VLis_connector_registered_by_name(TUTORIAL_VOL_CONNECTOR_NAME)) < 0)
        TEST_ERROR;
    if(false == is_registered)
        FAIL_PUTS_ERROR("VOL connector was not registered");

    /* Unregister the connector */
    if(H5VLunregister_connector(vol_id) < 0)
        TEST_ERROR;

    /* The connector should not be registered now */
    if((is_registered = H5VLis_connector_registered_by_name(TUTORIAL_VOL_CONNECTOR_NAME)) < 0)
        TEST_ERROR;
    if(true == is_registered)
        FAIL_PUTS_ERROR("VOL connector is inappropriately registered");

    PASSED();
    return SUCCEED;

error:
    H5E_BEGIN_TRY {
        H5VLunregister_connector(vol_id);
    } H5E_END_TRY;
    return FAIL;

} /* end test_registration_by_value() */

/*-------------------------------------------------------------------------
 * Function:    test_registration_by_name()
 *
 * Purpose:     Tests if we can load, register, and close a VOL
 *              connector by name.
 *
 * Return:      SUCCEED/FAIL
 *
 *-------------------------------------------------------------------------
 */
static herr_t
test_registration_by_name(void)
{
    htri_t  is_registered   = FAIL;
    hid_t   vol_id          = H5I_INVALID_HID;

    TESTING("VOL registration by name");

    /* The VOL connector should not be registered at the start of the test */
    if((is_registered = H5VLis_connector_registered_by_name(TUTORIAL_VOL_CONNECTOR_NAME)) < 0)
        TEST_ERROR;
    if(true == is_registered)
        FAIL_PUTS_ERROR("VOL connector is inappropriately registered");

    /* Register the connector by name */
    if((vol_id = H5VLregister_connector_by_name(TUTORIAL_VOL_CONNECTOR_NAME, H5P_DEFAULT)) < 0)
        TEST_ERROR;

    /* The connector should be registered now */
    if((is_registered = H5VLis_connector_registered_by_name(TUTORIAL_VOL_CONNECTOR_NAME)) < 0)
        TEST_ERROR;
    if(false == is_registered)
        FAIL_PUTS_ERROR("VOL connector was not registered");

    /* Unregister the connector */
    if(H5VLunregister_connector(vol_id) < 0)
        TEST_ERROR;

    /* The connector should not be registered now */
    if((is_registered = H5VLis_connector_registered_by_name(TUTORIAL_VOL_CONNECTOR_NAME)) < 0)
        TEST_ERROR;
    if(true == is_registered)
        FAIL_PUTS_ERROR("VOL connector is inappropriately registered");

    PASSED();
    return SUCCEED;

error:
    H5E_BEGIN_TRY {
        H5VLunregister_connector(vol_id);
    } H5E_END_TRY;
    return FAIL;

} /* end test_registration_by_name() */

/*-------------------------------------------------------------------------
 * Function:    test_multiple_registration()
 *
 * Purpose:     Tests if we can register a VOL connector multiple times.
 *
 * Return:      SUCCEED/FAIL
 *
 *-------------------------------------------------------------------------
 */
#define N_REGISTRATIONS 10
static herr_t
test_multiple_registration(void)
{
    htri_t  is_registered   = FAIL;
    hid_t   vol_ids[N_REGISTRATIONS];
    int     i;

    TESTING("registering a VOL connector multiple times");

    /* The VOL connector should not be registered at the start of the test */
    if((is_registered = H5VLis_connector_registered_by_name(TUTORIAL_VOL_CONNECTOR_NAME)) < 0)
        TEST_ERROR;
    if(true == is_registered)
        FAIL_PUTS_ERROR("VOL connector is inappropriately registered");

    /* Register the connector multiple times */
    for(i = 0; i < N_REGISTRATIONS; i++) {
        if((vol_ids[i] = H5VLregister_connector_by_name(TUTORIAL_VOL_CONNECTOR_NAME, H5P_DEFAULT)) < 0)
            TEST_ERROR;
    }

    /* The connector should be registered now */
    if((is_registered = H5VLis_connector_registered_by_name(TUTORIAL_VOL_CONNECTOR_NAME)) < 0)
        TEST_ERROR;
    if(false == is_registered)
        FAIL_PUTS_ERROR("VOL connector was not registered");

    /* Unregister the connector */
    for(i = 0; i < N_REGISTRATIONS; i++) {
        if(H5VLunregister_connector(vol_ids[i]) < 0)
            TEST_ERROR;
        /* Also test close on some of the IDs. This call currently works
         * identically to unregister.
         */
        i++;
        if(H5VLclose(vol_ids[i]) < 0)
            TEST_ERROR;
    }

    /* The connector should not be registered now */
    if((is_registered = H5VLis_connector_registered_by_name(TUTORIAL_VOL_CONNECTOR_NAME)) < 0)
        TEST_ERROR;
    if(true == is_registered)
        FAIL_PUTS_ERROR("VOL connector is inappropriately registered");

    PASSED();
    return SUCCEED;

error:
    H5E_BEGIN_TRY {
        for(i = 0; i < N_REGISTRATIONS; i++)
            H5VLunregister_connector(vol_ids[i]);
    } H5E_END_TRY;
    return FAIL;

} /* end test_multiple_registration() */

/*-------------------------------------------------------------------------
 * Function:    test_getters()
 *
 * Purpose:     Tests H5VL getters
 *
 * Return:      SUCCEED/FAIL
 *
 *-------------------------------------------------------------------------
 */
static herr_t
test_getters(void)
{
    htri_t  is_registered   = FAIL;
    hid_t   vol_id          = H5I_INVALID_HID;
    hid_t   vol_id_out      = H5I_INVALID_HID;

    TESTING("VOL getters");

    /* The VOL connector should not be registered at the start of the test */
    if((is_registered = H5VLis_connector_registered_by_name(TUTORIAL_VOL_CONNECTOR_NAME)) < 0)
        TEST_ERROR;
    if(true == is_registered)
        FAIL_PUTS_ERROR("VOL connector is inappropriately registered");

    /* Register the connector by name */
    if((vol_id = H5VLregister_connector_by_name(TUTORIAL_VOL_CONNECTOR_NAME, H5P_DEFAULT)) < 0)
        TEST_ERROR;

    /* Get the connector's ID */
    if((vol_id_out = H5VLget_connector_id_by_name(TUTORIAL_VOL_CONNECTOR_NAME)) < 0)
        TEST_ERROR;
    if(vol_id != vol_id_out)
        FAIL_PUTS_ERROR("VOL connector IDs don't match");

    /* Unregister the connector */
    if(H5VLunregister_connector(vol_id) < 0)
        TEST_ERROR;

    PASSED();
    return SUCCEED;

error:
    H5E_BEGIN_TRY {
        H5VLunregister_connector(vol_id);
    } H5E_END_TRY;
    return FAIL;

} /* end test_getters() */

/*-------------------------------------------------------------------------
 * Function:    test_file_ops()
 *
 * Purpose:     Tests VOL file operations
 *
 * Return:      SUCCEED/FAIL
 *
 *-------------------------------------------------------------------------
 */
static herr_t
test_file_ops(hid_t fapl_id)
{
    const char *filename = "file_ops.h5tut";
    hid_t       fid      = H5I_INVALID_HID;

    TESTING("VOL file operations");

    /* Create an HDF5 file using the tutorial VOL connector */
    if ((fid = H5Fcreate(filename, H5F_ACC_EXCL, H5P_DEFAULT, fapl_id)) < 0)
        TEST_ERROR;

    /* Close the file */
    if (H5Fclose(fid) < 0)
        TEST_ERROR;

    /* Open the HDF5 file using the tutorial VOL connector */
    if ((fid = H5Fopen(filename, H5F_ACC_RDWR, fapl_id)) < 0)
        TEST_ERROR;

    /* Close the file */
    if (H5Fclose(fid) < 0)
        TEST_ERROR;

    /* Delete the file */
    if (DELETE_FILES_g)
        if (H5Fdelete(filename, fapl_id) < 0)
            TEST_ERROR;

    PASSED();
    return SUCCEED;

error:
    H5E_BEGIN_TRY
    {
        H5Fclose(fid);
    }
    H5E_END_TRY;
    return FAIL;

} /* end test_file_ops() */

static herr_t
test_group_ops(hid_t fapl_id)
{
    const char *filename = "group_ops.h5tut";
    hid_t       fid      = H5I_INVALID_HID;
    hid_t       gid1_1   = H5I_INVALID_HID;
    hid_t       gid2_1   = H5I_INVALID_HID;
    hid_t       gid2_2   = H5I_INVALID_HID;

    TESTING("VOL group operations");

    /* Create an HDF5 file using the tutorial VOL connector */
    if ((fid = H5Fcreate(filename, H5F_ACC_EXCL, H5P_DEFAULT, fapl_id)) < 0)
        TEST_ERROR;

    /* Create a group off the root group */
    if ((gid1_1 = H5Gcreate2(fid, "level_1_group_1", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT)) < 0)
        TEST_ERROR;

    /* Create two sub-groups off that group */
    if ((gid2_1 = H5Gcreate2(gid1_1, "level_2_group_1", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT)) < 0)
        TEST_ERROR;
    if ((gid2_2 = H5Gcreate2(gid1_1, "level_2_group_2", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT)) < 0)
        TEST_ERROR;

    /* Close everything */
    if (H5Gclose(gid1_1) < 0)
        TEST_ERROR;
    if (H5Gclose(gid2_1) < 0)
        TEST_ERROR;
    if (H5Gclose(gid2_2) < 0)
        TEST_ERROR;
    if (H5Fclose(fid) < 0)
        TEST_ERROR;

    /* Open the HDF5 file using the tutorial VOL connector */
    if ((fid = H5Fopen(filename, H5F_ACC_RDWR, fapl_id)) < 0)
        TEST_ERROR;

    /* Open the group off the root group */
    if ((gid1_1 = H5Gopen2(fid, "level_1_group_1", H5P_DEFAULT)) < 0)
        TEST_ERROR;

    /* Open the sub-groups */
    if ((gid2_1 = H5Gopen2(gid1_1, "level_2_group_1", H5P_DEFAULT)) < 0)
        TEST_ERROR;
    if ((gid2_2 = H5Gopen2(gid1_1, "level_2_group_2", H5P_DEFAULT)) < 0)
        TEST_ERROR;

    /* Close everything */
    if (H5Gclose(gid1_1) < 0)
        TEST_ERROR;
    if (H5Gclose(gid2_1) < 0)
        TEST_ERROR;
    if (H5Gclose(gid2_2) < 0)
        TEST_ERROR;
    if (H5Fclose(fid) < 0)
        TEST_ERROR;

    /* Delete the file */
    if (DELETE_FILES_g)
        if (H5Fdelete(filename, fapl_id) < 0)
            TEST_ERROR;

    PASSED();
    return SUCCEED;

error:
    H5E_BEGIN_TRY
    {
        H5Fclose(fid);
        H5Gclose(gid1_1);
        H5Gclose(gid2_1);
        H5Gclose(gid2_2);
    }
    H5E_END_TRY;
    return FAIL;

} /* end test_group_ops() */

/*-------------------------------------------------------------------------
 * Function:    main
 *
 * Purpose:     Tests tutorial VOL connector operations
 *
 * Return:      EXIT_SUCCESS/EXIT_FAILURE
 *
 *-------------------------------------------------------------------------
 */
int
main(void)
{
    hid_t fapl_id = H5I_INVALID_HID;
    hid_t vol_id  = H5I_INVALID_HID;
    int   nerrors = 0;

    puts("Testing tutorial VOL connector functionality.");

    nerrors += test_registration_by_name() < 0 ? 1 : 0;
    nerrors += test_registration_by_value() < 0 ? 1 : 0;
    nerrors += test_multiple_registration() < 0 ? 1 : 0;
    nerrors += test_getters() < 0 ? 1 : 0;

    /* Create a fapl that uses the tutorial VOL connector */
    if ((fapl_id = H5Pcreate(H5P_FILE_ACCESS)) < 0) {
        printf("File access property list creation FAILED\n");
        nerrors++;
        goto error;
    }
    if ((vol_id = H5VLregister_connector_by_name(TUTORIAL_VOL_CONNECTOR_NAME, H5P_DEFAULT)) < 0) {
        printf("Tutorial VOL registration FAILED\n");
        nerrors++;
        goto error;
    }
    if (H5Pset_vol(fapl_id, vol_id, NULL) < 0) {
        printf("Setting VOL in the fapl FAILED\n");
        nerrors++;
        goto error;
    }

    nerrors += test_file_ops(fapl_id) < 0 ? 1 : 0;
    nerrors += test_group_ops(fapl_id) < 0 ? 1 : 0;

    /* Close fapl and VOL connector */
    if (H5Pclose(fapl_id) < 0) {
        printf("Closing the fapl FAILED\n");
        nerrors++;
        goto error;
    }
    if (H5VLunregister_connector(vol_id) < 0) {
        printf("Closing the VOL connector FAILED\n");
        nerrors++;
        goto error;
    }

error:
    if (nerrors) {
        printf("***** %d VOL connector plugin TEST%s FAILED! *****\n", nerrors, nerrors > 1 ? "S" : "");
        H5E_BEGIN_TRY
        {
            H5Pclose(fapl_id);
            H5VLunregister_connector(vol_id);
        }
        H5E_END_TRY;
        exit(EXIT_FAILURE);
    }

    puts("All VOL connector plugin tests passed.");

    exit(EXIT_SUCCESS);

} /* end main() */
