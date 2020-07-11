#
# strong_type C++14/17/20 strong typedef library
#
# Copyright (C) Björn Fahller
#
#  Use, modification and distribution is subject to the
#  Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#
# Project home: https://github.com/rollbear/strong_type
#

FROM ubuntu:focal AS python-build


RUN apt-get update          && \
    apt-get dist-upgrade -y && \
    apt-get install -y cmake wget 'g++-10' 'g++-9' 'g++-8' 'g++-7' 'clang-10' 'clang-9' 'clang-8' 'clang-7' 'clang-6.0' 'libc++-10-dev' 'libc++abi-dev'


ENV PATH="/usr/local/bin:$PATH" LD_LIBRARY_PATH="/usr/local/lib64:/usr/local/lib:/usr/local/libexec"
RUN echo "#include <iostream>\n\
\n\
template<typename T>\n\
requires (sizeof(T) == sizeof(int))\n\
constexpr void hello(T t)\n\
{\n\
	std::cout << \"Hello, \" << t << '\\\\n';\n\
}\n\
\n\
int main()\n\
{\n\
	hello(42);\n\
}\n\
" >/tmp/hello.cpp
RUN g++-10 -std=c++20 /tmp/hello.cpp -o /tmp/a.out -fuse-ld=gold
RUN clang++-10 -std=c++20 /tmp/hello.cpp -o /tmp/a.out
RUN clang++-10 -std=c++20 /tmp/hello.cpp -o /tmp/a.out -stdlib=libc++ -fuse-ld=gold -lc++abi
RUN cmake --version
WORKDIR /tmp

WORKDIR /
RUN rm -rf /tmp/*
