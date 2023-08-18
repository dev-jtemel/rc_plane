add_executable(som_controller
  main.cpp
)

target_link_libraries(som_controller
  PUBLIC
    boost_log
    boost_log_setup
    boost_system
    boost_thread
    pthread
)
