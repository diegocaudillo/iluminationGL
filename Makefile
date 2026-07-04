###############################################################################
# Product:     Pharos' Geometry
# File:        Makefile
# Description: Top level makefile for Linux systems (should work on most
#              Un*x-like systems with gcc, e.g. OpenSolaris).
###############################################################################
#       Copyright 2010 Diego de Jesus Caudillo Amador <dcaudillo@cimat.mx>
#       
#       This program is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 2 of the License, or
#       (at your option) any later version.
#       
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#       
#       You should have received a copy of the GNU General Public License
#       along with this program; if not, write to the Free Software
#       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#       MA 02110-1301, USA.
###############################################################################

CPPFLAGS = -Wall -l GLEW -l glut -l SOIL -l openctm 
CPP = g++ 
ECHO = echo 
CLN = rm 
STAT = stat 
CD = cd
MOVE = mv

.phony: default help clean

default:
	$(CPP) -o scene src/interfaz.cpp  $(CPPFLAGS)
help:
	$(ECHO) "Para poder compilar es necesario "
	$(ECHO) "OPEN GL 2.1 (GLEW C LIBRARY)"
	$(ECHO) "FREE GLUT C LIBRARY DEV"
	$(ECHO) "SOIL C LIBRARY DEV"
	$(ECHO) "OPENCTM LIBRARY DEV"

clean:
	$(STAT) scene && $(CLN) scene


