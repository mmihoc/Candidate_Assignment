FROM ubuntu:latest
RUN apt-get update && apt-get install -y curl
RUN apt install -y gcc-avr binutils-avr avr-libc gdb-avr avrdude
RUN apt install -y build-essential
RUN apt install -y git
WORKDIR /Home
RUN git clone https://github.com/mmihoc/Candidate_Assignment
WORKDIR /Home/Candidate_Assignment/Debug
RUN make clean
RUN make all
WORKDIR /Home/Candidate_Assignment/Release
RUN make clean
RUN make all
CMD ["/bin/bash"]
