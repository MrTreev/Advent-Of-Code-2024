CXX			=	clang++
CXXLIB		=	libstdc++
RTLIB		=	compiler-rt

CXXFLAGS	+=	-rtlib=${RTLIB}
CXXFLAGS	+=	-iquote${PATH_SRC}
CXXFLAGS	+=	-Weverything
CXXFLAGS	+=	-pedantic
CXXFLAGS	+=	-Wno-padded
CXXFLAGS	+=	-Wno-c99-extensions
CXXFLAGS	+=	-Wno-c99-compat
CXXFLAGS	+=	-Wno-c++-compat
CXXFLAGS	+=	-Wno-c++20-compat
CXXFLAGS	+=	-Wno-c++98-compat
CXXFLAGS	+=	-Wno-c++98-compat-pedantic
CXXFLAGS	+=	-Wno-pre-c++20-compat-pedantic
CXXFLAGS	+=	-Wno-pre-c++17-compat-pedantic
CXXFLAGS	+=	-Wno-switch-default
