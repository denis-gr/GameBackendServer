project(GameBackendServer CXX)
add_executable(${PROJECT_NAME} "mongo_service.cpp")
target_link_libraries(${PROJECT_NAME} userver-core userver-mongo)