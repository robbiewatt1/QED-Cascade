ADD_EXECUTABLE(ComptonTest ComptonTest.cpp)
ADD_EXECUTABLE(Pusher PusherTest.cpp)
ADD_EXECUTABLE(Laser LaserTest.cpp)


TARGET_LINK_LIBRARIES(ComptonTest Tools IO Particles Physics)
TARGET_LINK_LIBRARIES(Pusher Tools IO Particles Physics)
TARGET_LINK_LIBRARIES(Laser Tools IO Particles Physics)
