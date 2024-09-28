gcc multiple_lights.cpp ../glad.c \
 ../libraries/imgui/*.cpp \
 -I ./ -I ../include -I ../libraries \
 -lstdc++ -lglfw -lGL -lm -lX11 -lpthread -lXi -lXrandr -ldl -o ../output/multiple_lights; \
../output/multiple_lights