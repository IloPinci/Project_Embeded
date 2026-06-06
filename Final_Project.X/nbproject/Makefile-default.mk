#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Final_Project.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Final_Project.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=project_main.c ../libraries/adc.c ../libraries/pwm.c ../libraries/spi.c ../libraries/uart.c ../libraries/timer.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/project_main.o ${OBJECTDIR}/_ext/754011402/adc.o ${OBJECTDIR}/_ext/754011402/pwm.o ${OBJECTDIR}/_ext/754011402/spi.o ${OBJECTDIR}/_ext/754011402/uart.o ${OBJECTDIR}/_ext/754011402/timer.o
POSSIBLE_DEPFILES=${OBJECTDIR}/project_main.o.d ${OBJECTDIR}/_ext/754011402/adc.o.d ${OBJECTDIR}/_ext/754011402/pwm.o.d ${OBJECTDIR}/_ext/754011402/spi.o.d ${OBJECTDIR}/_ext/754011402/uart.o.d ${OBJECTDIR}/_ext/754011402/timer.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/project_main.o ${OBJECTDIR}/_ext/754011402/adc.o ${OBJECTDIR}/_ext/754011402/pwm.o ${OBJECTDIR}/_ext/754011402/spi.o ${OBJECTDIR}/_ext/754011402/uart.o ${OBJECTDIR}/_ext/754011402/timer.o

# Source Files
SOURCEFILES=project_main.c ../libraries/adc.c ../libraries/pwm.c ../libraries/spi.c ../libraries/uart.c ../libraries/timer.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/Final_Project.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP512MU810
MP_LINKER_FILE_OPTION=,--script=p33EP512MU810.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/project_main.o: project_main.c  .generated_files/flags/default/8c23776af36abc26d972a53e398aa58e003731cd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/project_main.o.d 
	@${RM} ${OBJECTDIR}/project_main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  project_main.c  -o ${OBJECTDIR}/project_main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/project_main.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/adc.o: ../libraries/adc.c  .generated_files/flags/default/75f2bb59a3ca766b208eac648e0a9c41baa71b2c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/adc.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/adc.c  -o ${OBJECTDIR}/_ext/754011402/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/adc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/pwm.o: ../libraries/pwm.c  .generated_files/flags/default/2e7a23c9bf449f92f77fd9d27abc80f0ab2eae8b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/pwm.c  -o ${OBJECTDIR}/_ext/754011402/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/pwm.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/spi.o: ../libraries/spi.c  .generated_files/flags/default/813dd01790f3d4ba2892be956a3aad76823c558a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/spi.c  -o ${OBJECTDIR}/_ext/754011402/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/spi.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/uart.o: ../libraries/uart.c  .generated_files/flags/default/94e32e37a6c645577ca4628df9414b8076801583 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/uart.c  -o ${OBJECTDIR}/_ext/754011402/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/uart.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/timer.o: ../libraries/timer.c  .generated_files/flags/default/c1a322acf6c27e4ae9579fba3c0f8c754074326b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/timer.c  -o ${OBJECTDIR}/_ext/754011402/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/timer.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/project_main.o: project_main.c  .generated_files/flags/default/77a598e2d88473f8f701668cb9216e279e6c5065 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/project_main.o.d 
	@${RM} ${OBJECTDIR}/project_main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  project_main.c  -o ${OBJECTDIR}/project_main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/project_main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/adc.o: ../libraries/adc.c  .generated_files/flags/default/e9d66418116b89e0f828e30389f6a2d0f47ad695 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/adc.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/adc.c  -o ${OBJECTDIR}/_ext/754011402/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/adc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/pwm.o: ../libraries/pwm.c  .generated_files/flags/default/754f12b80858fc0619a4880402a048a90825828c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/pwm.c  -o ${OBJECTDIR}/_ext/754011402/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/pwm.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/spi.o: ../libraries/spi.c  .generated_files/flags/default/b7a7a2dc06e9410c64a8c8fe76dcba3279f1ccee .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/spi.c  -o ${OBJECTDIR}/_ext/754011402/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/spi.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/uart.o: ../libraries/uart.c  .generated_files/flags/default/48c6edde7746a64bf2acdeaad35911b21a25ca44 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/uart.c  -o ${OBJECTDIR}/_ext/754011402/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/uart.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/timer.o: ../libraries/timer.c  .generated_files/flags/default/725427e34be1db769fcae4a4865532ccee82f63c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/timer.c  -o ${OBJECTDIR}/_ext/754011402/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/timer.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Final_Project.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Final_Project.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries"     -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/Final_Project.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Final_Project.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/Final_Project.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
