FROM ubuntu

RUN apt-get -y update
RUN apt-get -y install automake g++ make gawk qemu xorriso nasm wget xz-utils grub

ADD script/build_cross_gcc /opt/build_cross_gcc
RUN /opt/build_cross_gcc /opt/build
RUN echo "export PATH=\"/opt/build/opt/cross/bin:\$PATH\"" >> /etc/profile.d/cross_gcc.sh

VOLUME /data

CMD /bin/bash --login
