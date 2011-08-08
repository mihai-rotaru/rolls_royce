# !/bin/env/python
#APPNAME = "fish"
#VERSION = "0.1"

from subprocess import *
from datetime import date
import os
import re

top = '.'
out = 'build'
sep = os.path.sep

def options( opt ):
   # opt.load( "compiler_cxx" )
   pass

def configure( cnf ):
    print( "configuring the project in " + cnf.path.abspath())
    cnf.check_tool( "g++" )
    cnf.find_program( "git" )
    cnf.find_program( "date" )

def build( bld ):
    # determine version
    if bld.env[ 'GIT' ] !="":
        ver_short = Popen( "git describe --abbrev=0" , stdout=PIPE, stderr=PIPE ).stdout.read()
        ver_full = Popen( "git describe --abbrev=7 --dirty" , stdout=PIPE, stderr=PIPE ).stdout.read()
        exe_name = 'rr_'+str(ver_short.strip())
        print "building " + exe_name + "..."
        ver_file = open( out + sep + "VERSION", 'w' )
        ver_file.write( ver_short.strip() + '\n' )
        ver_file.write( ver_full.strip() + '\n')
        d = date.today()
        ver_file.write( d.strftime("%a, %d %b %Y, %H:%M %p") )

    a_path = bld.path.abspath()
    boost_path = "C:/pdev/boost_1_45_0"

    print( bld.path.abspath())
    bld.program(
            source       = [ 
                'main.cpp', 
                'globals.cpp',
                'utils.cpp',
                'Point.cpp',
                'Line.cpp',
                'BezierCurve.cpp', 
                'Shape.cpp',
                'Group.cpp',
                'Layer.cpp'
                ],
            
            target       = exe_name,
            features     = [ 'cxxprogram' ],
            includes     = [ 
                './freeglut/include', 
                boost_path ],
            defines      = [ 'FREEGLUT_STATIC'],
            # if not linking statically, the above define and the gdi32, winmm libs are not needed
            lib          = [ 'freeglut_static','opengl32','gdi32','glu32','winmm' ],
            libpath      = [ 
                a_path + '/freeglut/lib', 
                a_path + '/freeglut' ],
            stlib        = [ 'boost_regex-gcc-1_45'],
            stlibpath    = [ boost_path + '/libs/regex/build/gcc' ],
            linkflags    = [ '-static-libgcc','-static-libstdc++','-W1,subsystem,windows' ],
            cxxflags     = [ '-c', '-g', '-O2', '-Wall' ],
            )

# vim:filetype=python
