gcc basic_lighting.cpp ../glad.c \
 ../libraries/imgui/*.cpp \
 -I ./ -I ../include -I ../libraries \
 -lstdc++ -lglfw -lGL -lm -lX11 -lpthread -lXi -lXrandr -ldl -o ../output/basic_lighting; \
../output/basic_lighting