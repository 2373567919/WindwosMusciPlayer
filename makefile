TARGET:=build/MusicPlayer

#依赖文件 .o
OBJS:=main.cpp fun.cpp 
LIBS:=-lbass -lbassflac
LDFLAGS:=-L ./lib
$(TARGET):$(OBJS)
	$(CXX) $^ -O3 -o $@ $(LDFLAGS)  $(LIBS)  -mwindows