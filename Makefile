CXX = g++
CXXFLAGS = -std=c++11 -Wall
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system
SRCS = main.cpp GameObject.cpp Animation.cpp Cloud.cpp Background.cpp Player.cpp Platform.cpp Playground.cpp Enemy.cpp Pickup.cpp SpecialEffects.cpp
TARGET = jump.out
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(SFML_LIBS)
