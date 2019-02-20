ADD_EXECUTABLE(Compton ComptonTest.cpp)
ADD_EXECUTABLE(breitwheeler BreitWheelerTest.cpp)
ADD_EXECUTABLE(Pusher PusherTest.cpp)
ADD_EXECUTABLE(Laser LaserTest.cpp)
ADD_EXECUTABLE(File InputTest.cpp)
ADD_EXECUTABLE(Main MainTest.cpp)
ADD_EXECUTABLE(Geant GeantTest.cpp)


TARGET_LINK_LIBRARIES(Compton Tools IO Particles Physics)
TARGET_LINK_LIBRARIES(Pusher Tools IO Particles Physics)
TARGET_LINK_LIBRARIES(Laser Tools IO Particles Physics)
TARGET_LINK_LIBRARIES(File Tools IO)
TARGET_LINK_LIBRARIES(Main Tools IO Particles Physics)
TARGET_LINK_LIBRARIES(breitwheeler Tools IO Particles Physics)
TARGET_LINK_LIBRARIES(Geant GeantQED)

