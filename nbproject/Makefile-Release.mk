#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW-Windows
CND_DLIB_EXT=dll
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/HaarCascadeObjectDetector.o \
	${OBJECTDIR}/NNController.o \
	${OBJECTDIR}/NeuralNet.o \
	${OBJECTDIR}/Neuron.o \
	${OBJECTDIR}/NeuronLayer.o \
	${OBJECTDIR}/eye_detect.o \
	${OBJECTDIR}/find_word.o \
	${OBJECTDIR}/imageProcess.o \
	${OBJECTDIR}/initializeNN.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/poiDetection.o \
	${OBJECTDIR}/tracking.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../../../../../opencv-2.4.3/release/lib ../../../../../opencv-2.4.3/release/lib/libopencv_calib3d243.dll.a ../../../../../opencv-2.4.3/release/lib/libopencv_contrib243.dll.a ../../../../../opencv-2.4.3/release/lib/libopencv_core243.dll.a ../../../../../opencv-2.4.3/release/lib/libopencv_features2d243.dll.a ../../../../../opencv-2.4.3/release/lib/libopencv_flann243.dll.a ../../../../../opencv-2.4.3/release/lib/libopencv_gpu243.dll.a ../../../../../opencv-2.4.3/release/lib/libopencv_haartraining_engine.a ../../../../../opencv-2.4.3/release/lib/libopencv_highgui243.dll.a ../../../../../opencv-2.4.3/release/lib/libopencv_imgproc243.dll.a ../../../../../opencv-2.4.3/release/lib/libopencv_legacy243.dll.a ../../../../../opencv-2.4.3/release/lib/libopencv_ml243.dll.a ../../../../../opencv-2.4.3/release/lib/libopencv_nonfree243.dll.a ../../../../../opencv-2.4.3/release/lib/libopencv_objdetect243.dll.a ../../../../../opencv-2.4.3/release/lib/libopencv_photo243.dll.a ../../../../../opencv-2.4.3/release/lib/libopencv_stitching243.dll.a ../../../../../opencv-2.4.3/release/lib/libopencv_ts243.a ../../../../../opencv-2.4.3/release/lib/libopencv_video243.dll.a ../../../../../opencv-2.4.3/release/lib/libopencv_videostab243.dll.a ../../../../../MinGW/lib/libwinmm.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/classification4.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/classification4.exe: ../../../../../opencv-2.4.3/release/lib/libopencv_calib3d243.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/classification4.exe: ../../../../../opencv-2.4.3/release/lib/libopencv_contrib243.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/classification4.exe: ../../../../../opencv-2.4.3/release/lib/libopencv_core243.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/classification4.exe: ../../../../../opencv-2.4.3/release/lib/libopencv_features2d243.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/classification4.exe: ../../../../../opencv-2.4.3/release/lib/libopencv_flann243.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/classification4.exe: ../../../../../opencv-2.4.3/release/lib/libopencv_gpu243.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/classification4.exe: ../../../../../opencv-2.4.3/release/lib/libopencv_haartraining_engine.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/classification4.exe: ../../../../../opencv-2.4.3/release/lib/libopencv_highgui243.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/classification4.exe: ../../../../../opencv-2.4.3/release/lib/libopencv_imgproc243.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/classification4.exe: ../../../../../opencv-2.4.3/release/lib/libopencv_legacy243.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/classification4.exe: ../../../../../opencv-2.4.3/release/lib/libopencv_ml243.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/classification4.exe: ../../../../../opencv-2.4.3/release/lib/libopencv_nonfree243.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/classification4.exe: ../../../../../opencv-2.4.3/release/lib/libopencv_objdetect243.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/classification4.exe: ../../../../../opencv-2.4.3/release/lib/libopencv_photo243.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/classification4.exe: ../../../../../opencv-2.4.3/release/lib/libopencv_stitching243.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/classification4.exe: ../../../../../opencv-2.4.3/release/lib/libopencv_ts243.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/classification4.exe: ../../../../../opencv-2.4.3/release/lib/libopencv_video243.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/classification4.exe: ../../../../../opencv-2.4.3/release/lib/libopencv_videostab243.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/classification4.exe: ../../../../../MinGW/lib/libwinmm.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/classification4.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/classification4 ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/HaarCascadeObjectDetector.o: HaarCascadeObjectDetector.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../../../opencv-2.4.3/include -I../../../../../MinGW/lib -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/HaarCascadeObjectDetector.o HaarCascadeObjectDetector.cpp

${OBJECTDIR}/NNController.o: NNController.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../../../opencv-2.4.3/include -I../../../../../MinGW/lib -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/NNController.o NNController.cpp

${OBJECTDIR}/NeuralNet.o: NeuralNet.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../../../opencv-2.4.3/include -I../../../../../MinGW/lib -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/NeuralNet.o NeuralNet.cpp

${OBJECTDIR}/Neuron.o: Neuron.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../../../opencv-2.4.3/include -I../../../../../MinGW/lib -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Neuron.o Neuron.cpp

${OBJECTDIR}/NeuronLayer.o: NeuronLayer.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../../../opencv-2.4.3/include -I../../../../../MinGW/lib -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/NeuronLayer.o NeuronLayer.cpp

${OBJECTDIR}/eye_detect.o: eye_detect.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../../../opencv-2.4.3/include -I../../../../../MinGW/lib -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/eye_detect.o eye_detect.cpp

${OBJECTDIR}/find_word.o: find_word.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../../../opencv-2.4.3/include -I../../../../../MinGW/lib -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/find_word.o find_word.cpp

${OBJECTDIR}/imageProcess.o: imageProcess.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../../../opencv-2.4.3/include -I../../../../../MinGW/lib -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/imageProcess.o imageProcess.cpp

${OBJECTDIR}/initializeNN.o: initializeNN.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../../../opencv-2.4.3/include -I../../../../../MinGW/lib -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/initializeNN.o initializeNN.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../../../opencv-2.4.3/include -I../../../../../MinGW/lib -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/poiDetection.o: poiDetection.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../../../opencv-2.4.3/include -I../../../../../MinGW/lib -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/poiDetection.o poiDetection.cpp

${OBJECTDIR}/tracking.o: tracking.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../../../opencv-2.4.3/include -I../../../../../MinGW/lib -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tracking.o tracking.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
