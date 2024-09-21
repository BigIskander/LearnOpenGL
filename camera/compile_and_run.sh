gcc camera.cpp ../glad.c -I ./ -I ../include -I ../libraries \
 -lstdc++ -lglfw -lGL -lm -lX11 -lpthread -lXi -lXrandr -ldl -o ../output/camera; \
../output/camera