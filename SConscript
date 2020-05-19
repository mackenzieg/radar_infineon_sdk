# First draft as a good start. Let's do it in a decent way.

import os, sys
Import('PROG_NAME')

if sys.platform.startswith('win32'):
    compiler_tool_chain = 'mingw'
else:
    compiler_tool_chain = 'default'
env = Environment(tools=[compiler_tool_chain, 'textfile'], ENV=os.environ)

SRCS = Glob('src/*.c')
SRCS = SRCS + Glob('src/*.cpp')

env.MergeFlags('-pthread -lpthread')

libs=['radar_sdk']
if not sys.platform.startswith('linux'):
    env.Append(LINKFLAGS='-static')
    libs.append('winmm')
else:
    libs.append('wiringPi')

env.Program(os.path.join('build', PROG_NAME + '.app'),
             SRCS,  
             CPPPATH=['externals/radar_sdk/inc', 'inc'], 
             LIBPATH=['externals/radar_sdk/lib'], 
             LIBS=libs)
