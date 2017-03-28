FROM creatordev/docker-nat64-client

RUN curl -sL http://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add /dev/stdin
ADD docker/lldb.list /etc/apt/sources.list.d/
RUN apt-get update && \
	apt-get install -yq openssh-server gdbserver pwgen lldb-4.0 && \
	ln -s /usr/bin/lldb-mi-4.0 /usr/bin/lldb-mi
ADD docker/run.sh /
RUN chmod +x /run.sh

CMD ["/run.sh","bash"]