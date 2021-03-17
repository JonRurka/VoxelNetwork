#pragma once

#define NOMINMAX
//#define BOOST_ASIO_NO_TS_EXECUTORS

#include <boost/asio.hpp>
#include <cstdio>
#include <map>
#include <string>
#include <limits>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <list>
#include <cstdarg>
#include <thread>


#ifdef WINDOWS_BUILD

#include <Windows.h>
#include <conio.h> 
#define WIN32_LEAN_AND_MEAN

#else 

#define LINUX_BUILD
#include "Linux_conio.hpp"


#endif

