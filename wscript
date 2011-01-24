# !/bin/env/python
#APPNAME = "fish"
#VERSION = "0.1"

top = '.'
out = 'waf-build'

def options( opt ):
   # opt.load( "compiler_cxx" )
   pass

def configure( cnf ):
    print( "configuring the project in " + cnf.path.abspath())
    cnf.check_tool( "g++" )

def build( bld ):
    print( "building..." )
    print( bld.path.abspath())
    bld.program(
            source       = 'src/main.cpp',
            target       = 'fish',
            features     = [ 'cxxprogram' ],
            includes     = [ './freeglut/include' ],
            lib          = [ 'freeglut','opengl32', 'glu32' ],
            libpath      = [ bld.path.abspath() + '/freeglut/lib' ],
            linkflags    = [ '-W1,subsystem,windows' ],
            cxxflags     = [ '-c', '-g', '-O2', '-Wall' ]
            )
