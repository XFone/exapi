# Use official release
FROM centos:7
LABEL maintainer="Ziven<ziven@zerone.io>"

ENV container docker
ENV DEBIAN_FRONTEND noninteractive

# Replace yum repo with aliyun repo.
# RUN curl -o /etc/yum.repos.d/CentOS-Base.repo http://mirrors.aliyun.com/repo/Centos-7.repo
# RUN yum update -y && yum clean all

# RUN yum-config-manager --enable extras
# RUN yum makecache

# Install development environment
RUN yum install -y net-tools vim yum-utils
RUN yum install -y autoconf automake make libtool
RUN yum install -y libuuid-devel python-devel libpcap-devel openssl-devel

# Install default clang-3.4.x
RUN yum install -y gcc-c++ clang

# Install clang-5.x
# RUN yum install -y centos-release-scl
# RUM yum install -y llvm-toolset-7
# RUN scl enable llvm-toolset-7 bash

# Install tools for building third-party libs
RUN yum install -y cmake wget patch git

RUN yum clean all

# EXPOSE 80

CMD ["/usr/bin/tail", "-f", "/dev/null"]
