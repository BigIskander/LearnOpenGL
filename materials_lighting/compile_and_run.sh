gcc materials_lighting.cpp ../glad.c \
 ../libraries/imgui/*.cpp \
 -I ./ -I ../include -I ../libraries \
 -lstdc++ -lglfw -lGL -lm -lX11 -lpthread -lXi -lXrandr -ldl -o ../output/materials_lighting; \
../output/materials_lighting