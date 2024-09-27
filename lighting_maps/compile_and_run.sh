gcc lighting_maps.cpp ../glad.c \
 ../libraries/imgui/*.cpp \
 -I ./ -I ../include -I ../libraries \
 -lstdc++ -lglfw -lGL -lm -lX11 -lpthread -lXi -lXrandr -ldl -o ../output/lighting_maps; \
../output/lighting_maps