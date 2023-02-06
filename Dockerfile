FROM ubuntu

RUN	apt update && apt upgrade
RUN apt	install -y inspircd
RUN apt install -y tcpflow
RUN apt install -y irssi
RUN apt install -y lsof
RUN apt install -y curl
RUN apt install -y sudo
RUN apt install -y vim

RUN mkdir /var/run/inspircd
RUN echo "alias myserver='inspircd --runasroot --nofork'" >> ~/.bashrc
RUN echo "alias myclient='irssi -c localhost -p 6667'" >> ~/.bashrc
RUN echo "alias mytcp='tcpflow -i lo -c'" >> ~/.bashrc

EXPOSE 6667

CMD ["inspircd", "--runasroot", "--nofork"]
