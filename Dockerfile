FROM ubuntu

RUN	apt update -y && apt upgrade -y
RUN apt	install -y inspircd
RUN apt install -y tcpflow
RUN apt install -y irssi

RUN mkdir /var/run/inspircd
RUN echo "alias myserver='inspircd --runasroot --nofork'" >> ~/.bashrc
RUN echo "alias myclient='irssi -c localhost -p 6667'" >> ~/.bashrc
RUN echo "alias mytcp='tcpflow -i lo -c'" >> ~/.bashrc

EXPOSE 6667

CMD ["inspircd", "--runasroot", "--nofork"]
