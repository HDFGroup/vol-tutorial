# Set the tutorial VOL connector name
set (TVC_NAME tutorial_vol_connector)

# Build the tutorial VOL connector
add_library (${TVC_NAME} SHARED
    tutorial_dataset.c
    tutorial_file.c
    tutorial_group.c
    tutorial_util.c
    tutorial_vol_connector.c
)
set_target_properties (${TVC_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties (${TVC_NAME} PROPERTIES SOVERSION 1)
set_target_properties (${TVC_NAME} PROPERTIES PUBLIC_HEADER "${TVC_NAME}.h")

