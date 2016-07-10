TARGETS=http-client
SRCS=http-client.c
SRCS_A=arduino.c Makefile_A
OBJS=$(SRCS:.c=.o)

all: $(TARGETS)

http-client: $(OBJS) $(SRCS_A)
	gcc -o $@ $(OBJS)
	scp $(SRCS_A) vps:cgi/
	ssh vps "cd cgi; mv Makefile_A Makefile; make; touch log/arduino.log; rm log/arduino.log"
	@echo ""
	@echo "+++++++++++++++TEST START++++++++++++++++++++"
	@echo ""
	./$@
	@echo ""
	@echo "+++++++++++++++TEST END++++++++++++++++++++"
	@echo ""
	scp vps:cgi/log/arduino.log .
	cat arduino.log

vpslog:
	scp vps:cgi/log/arduino.log .
	cat arduino.log

.c.o:
	gcc -c $<

.PHONY: clean
clean:
	$(RM) $(OBJS)
