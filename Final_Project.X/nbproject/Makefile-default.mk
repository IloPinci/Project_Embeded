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
${OBJECTDIR}/project_main.o: project_main.c  .generated_files/flags/default/f77dc261fc6018322ec63218be1732a6279a3f00 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/project_main.o.d 
	@${RM} ${OBJECTDIR}/project_main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  project_main.c  -o ${OBJECTDIR}/project_main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/project_main.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/adc.o: ../libraries/adc.c  .generated_files/flags/default/af6b1dda01d948c0ef34d2d14cba5d0ab81cd8d7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/adc.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/adc.c  -o ${OBJECTDIR}/_ext/754011402/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/adc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/pwm.o: ../libraries/pwm.c  .generated_files/flags/default/a19d87d957efa9183e8accffadc26ffa08d5c92c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/pwm.c  -o ${OBJECTDIR}/_ext/754011402/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/pwm.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/spi.o: ../libraries/spi.c  .generated_files/flags/default/5b9bf92aae59b5c90124a8c41e538cc9958012ec .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/spi.c  -o ${OBJECTDIR}/_ext/754011402/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/spi.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/uart.o: ../libraries/uart.c  .generated_files/flags/default/f704db1cbca466f01fbd42056fe76924474bcdf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/uart.c  -o ${OBJECTDIR}/_ext/754011402/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/uart.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/timer.o: ../libraries/timer.c  .generated_files/flags/default/5e4dc25fbfd772cb901e0383e0e42ab0bf498f12 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/timer.c  -o ${OBJECTDIR}/_ext/754011402/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/timer.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/project_main.o: project_main.c  .generated_files/flags/default/3c24869908d6c3982659e0ebda60ea48ad91521 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/project_main.o.d 
	@${RM} ${OBJECTDIR}/project_main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  project_main.c  -o ${OBJECTDIR}/project_main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/project_main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/adc.o: ../libraries/adc.c  .generated_files/flags/default/ee1caa67f53564bdc8cacda589c4fad96bfd92f8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/adc.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/adc.c  -o ${OBJECTDIR}/_ext/754011402/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/adc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/pwm.o: ../libraries/pwm.c  .generated_files/flags/default/b0ed035de26e202a697c7590aaee8b2c44258a5c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/pwm.c  -o ${OBJECTDIR}/_ext/754011402/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/pwm.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/spi.o: ../libraries/spi.c  .generated_files/flags/default/5ba72dd03310aa7689d76cbe09c57aebcc78da76 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/spi.c  -o ${OBJECTDIR}/_ext/754011402/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/spi.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/uart.o: ../libraries/uart.c  .generated_files/flags/default/d4536dbebfd09313a58de1fdbf21e126a18fed1b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/uart.c  -o ${OBJECTDIR}/_ext/754011402/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/uart.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/timer.o: ../libraries/timer.c  .generated_files/flags/default/d66b0cb463b0378d454c88c83ab503f6e0f0c543 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
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
