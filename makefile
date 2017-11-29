GXX = g++
GXXFLAGS = -g
OBJS = MazeMaster.o
OUTDIR = data
LIBS = -lncurses

INCLUDES = $(addprefix -I, $(sort $(dir $(wildcard include/* include/**/*))))
EXCLUDESRCS = $(wildcard *test.cpp include/*test.cpp include/**/*test.cpp *test.cc include/*test.cc include/**/*test.cc)
SRCS = main.cpp $(wildcard include/*.cc include/**/*.cc include/*.cpp include/**/*.cpp)
SRCS := $(filter-out $(EXCLUDESRCS), $(SRCS))
HDRS = $(wildcard include/*.h include/**/*.h)

$(OUTDIR)/$(OBJS): $(SRCS) $(HDRS)
	$(GXX) $(GXXFLAGS) $(LIBS) $(INCLUDES) -o $@ $(SRCS)

run: $(OUTDIR)/$(OBJS)
	./$<

clean:
	\rm -f $(OUTDIR)/$(OBJS) saves/lastsession.dat
