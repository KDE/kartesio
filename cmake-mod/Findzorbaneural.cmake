#/***************************************************************************
# *   Kartesio is a program for calculating best fit curves with            * 
# *   experimental points using regression algorithms or neural networks.   *
# *                                                                         *
# *                   Kartesio has been created by                          *
# *                Luca Tringali, TRINGALINVENT@libero.it                   *
# *                                                                         *
# *                    Copyright 2011-2013 Luca Tringali                    *
# *                                                                         *
# *   This program is free software; you can redistribute it and/or modify  *
# *   it under the terms of the GNU General Public License as published by  *
# *   the Free Software Foundation; either version 2 of the License, or     *
# *   (at your option) any later version.                                   *
# *                                                                         *
# *   This program is distributed in the hope that it will be useful,       *
# *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
# *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
# *   GNU General Public License for more details.                          *
# *                                                                         *
# *   You should have received a copy of the GNU General Public License     *
# *   along with this program; if not, write to the                         *
# *   Free Software Foundation, Inc.,                                       *
# *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
# ***************************************************************************/


#locate libzorbaneural library

# This module defines
#  LIBZORBANEURAL_LIBRARY
#  LIBZORBANEURAL_FOUND
#  LIBZORBANEURAL_INCLUDE_DIR
#  LIBZORBANEURAL_CFLAGS
#  LIBZORBANEURAL_LINKFLAGS


IF(LIBZORBANEURAL_LIBRARY AND LIBZORBANEURAL_INCLUDE_DIR)
	SET(LIBZORBANEURAL_FOUND TRUE)
ELSE(LIBZORBANEURAL_LIBRARY AND LIBZORBANEURAL_INCLUDE_DIR)
	INCLUDE(UsePkgConfig)
	PKGCONFIG("libzorbaneural" _libzorbaneuralIncDir _libzorbaneuralLibDir _libzorbaneuralLinkFlags _libzorbaneuralCflags)

	# set additional flags needed to compile/link against libzorbaneural
	SET(LIBZORBANEURAL_CFLAGS ${_libzorbaneuralCFlags} CACHE STRING "CFLAGS required for libzorbaneural")
	SET(LIBZORBANEURAL_LINKFLAGS ${_libzorbaneuralLinkFlags} CACHE STRING "Flags used for linking against libzorbaneural")

	# search for include and library path
	FIND_PATH(LIBZORBANEURAL_INCLUDE_DIR libzorbaneural/neuralnet.h PATHS ${_libzorbaneuralIncDir} ${_libzorbaneuralIncDir}/libzorbaneural)
	FIND_LIBRARY(LIBZORBANEURAL_LIBRARY zorbaneural PATHS ${_libzorbaneuralLibDir})

	IF(LIBZORBANEURAL_INCLUDE_DIR AND LIBZORBANEURAL_LIBRARY)
		SET(LIBZORBANEURAL_FOUND TRUE)
		MESSAGE(STATUS "Found libzorbaneural: ${LIBZORBANEURAL_LIBRARY}")

	ELSE(LIBZORBANEURAL_INCLUDE_DIR AND LIBZORBANEURAL_LIBRARY)
		SET(LIBZORBANEURAL_FOUND FALSE)
		MESSAGE(SEND_ERROR "Could NOT find libzorbaneural")
	ENDIF(LIBZORBANEURAL_INCLUDE_DIR AND LIBZORBANEURAL_LIBRARY)
ENDIF(LIBZORBANEURAL_LIBRARY AND LIBZORBANEURAL_INCLUDE_DIR)
