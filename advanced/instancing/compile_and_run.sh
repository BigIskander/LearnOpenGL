gcc instancing.cpp ../../glad.c \
 ../../libraries/imgui/*.cpp \
 -I ./ -I ../../include -I ../../libraries \
 -lstdc++ -lglfw -lGL -lm -lX11 -lpthread -lXi -lXrandr -ldl -lassimp -o ../../output/instancing; \
../../output/instancing