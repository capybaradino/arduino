TARGETS=http-client
SRCS=http-client.c
SRCS_A=arduino.c secret.h Makefile_A
OBJS=$(SRCS:.c=.o)

all: $(TARGETS)

http-client: $(OBJS) $(SRCS_A)
	gcc -o $@ $(OBJS)
	scp $(SRCS_A) vps:cgi/
	ssh vps "cd cgi; mv Makefile_A Makefile; make cleancgi; make; touch log/arduino.log; rm log/arduino.log"
	@echo ""
	@echo "+++++++++++++++TEST START++++++++++++++++++++"
	@echo ""
	./$@
	@echo ""
	@echo "+++++++++++++++TEST END++++++++++++++++++++"
	@echo ""
	scp vps:cgi/log/arduino.log .
	cat arduino.log
	scp vps:cgi/data/arduino.dat .
	cat arduino.dat

vpslog:
	scp vps:cgi/log/arduino.log .
	cat arduino.log
	scp vps:cgi/data/arduino.dat .
	cat arduino.dat

.c.o:
	gcc -c $<

.PHONY: clean
clean:
	$(RM) $(OBJS)
