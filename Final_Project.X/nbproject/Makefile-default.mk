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
SOURCEFILES_QUOTED_IF_SPACED=project_main.c ../libraries/adc.c ../libraries/pwm.c ../libraries/spi.c ../libraries/timer_config.c ../libraries/uart.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/project_main.o ${OBJECTDIR}/_ext/754011402/adc.o ${OBJECTDIR}/_ext/754011402/pwm.o ${OBJECTDIR}/_ext/754011402/spi.o ${OBJECTDIR}/_ext/754011402/timer_config.o ${OBJECTDIR}/_ext/754011402/uart.o
POSSIBLE_DEPFILES=${OBJECTDIR}/project_main.o.d ${OBJECTDIR}/_ext/754011402/adc.o.d ${OBJECTDIR}/_ext/754011402/pwm.o.d ${OBJECTDIR}/_ext/754011402/spi.o.d ${OBJECTDIR}/_ext/754011402/timer_config.o.d ${OBJECTDIR}/_ext/754011402/uart.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/project_main.o ${OBJECTDIR}/_ext/754011402/adc.o ${OBJECTDIR}/_ext/754011402/pwm.o ${OBJECTDIR}/_ext/754011402/spi.o ${OBJECTDIR}/_ext/754011402/timer_config.o ${OBJECTDIR}/_ext/754011402/uart.o

# Source Files
SOURCEFILES=project_main.c ../libraries/adc.c ../libraries/pwm.c ../libraries/spi.c ../libraries/timer_config.c ../libraries/uart.c



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
${OBJECTDIR}/project_main.o: project_main.c  .generated_files/flags/default/7ff4a8ed5dcb48de9f3133f2d6f11662b2b9434e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/project_main.o.d 
	@${RM} ${OBJECTDIR}/project_main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  project_main.c  -o ${OBJECTDIR}/project_main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/project_main.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/adc.o: ../libraries/adc.c  .generated_files/flags/default/2fe0860283601eb83802ef1211ba82dd0db90c13 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/adc.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/adc.c  -o ${OBJECTDIR}/_ext/754011402/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/adc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/pwm.o: ../libraries/pwm.c  .generated_files/flags/default/1e78a2e9191a0f22772a795dcc62a828113b46ea .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/pwm.c  -o ${OBJECTDIR}/_ext/754011402/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/pwm.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/spi.o: ../libraries/spi.c  .generated_files/flags/default/d06480dd3a2d6e65cbe578664ed55700ea2d22a2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/spi.c  -o ${OBJECTDIR}/_ext/754011402/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/spi.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/timer_config.o: ../libraries/timer_config.c  .generated_files/flags/default/5a1825deb48b679592121279866e09cce3b8495d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/timer_config.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/timer_config.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/timer_config.c  -o ${OBJECTDIR}/_ext/754011402/timer_config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/timer_config.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/uart.o: ../libraries/uart.c  .generated_files/flags/default/8dd202064f956f9fabc354c9d118342dbcbe63d2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/uart.c  -o ${OBJECTDIR}/_ext/754011402/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/uart.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/project_main.o: project_main.c  .generated_files/flags/default/20aa9d4792d3bdd5e61d0f83daeec6c683ad099c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/project_main.o.d 
	@${RM} ${OBJECTDIR}/project_main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  project_main.c  -o ${OBJECTDIR}/project_main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/project_main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/adc.o: ../libraries/adc.c  .generated_files/flags/default/44e0a550c3ba1bfe2c74aeae9e1d5609f650f5a1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/adc.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/adc.c  -o ${OBJECTDIR}/_ext/754011402/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/adc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/pwm.o: ../libraries/pwm.c  .generated_files/flags/default/ad5992082ce5cd887d97df0e3a6e0993306fe272 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/pwm.c  -o ${OBJECTDIR}/_ext/754011402/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/pwm.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/spi.o: ../libraries/spi.c  .generated_files/flags/default/ab661df1c6f81e0054609e415a0639ebdb26d7e0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/spi.c  -o ${OBJECTDIR}/_ext/754011402/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/spi.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/timer_config.o: ../libraries/timer_config.c  .generated_files/flags/default/29fcde644624bcc9e6ee58f9ed1fcebb3f9a5525 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/timer_config.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/timer_config.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/timer_config.c  -o ${OBJECTDIR}/_ext/754011402/timer_config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/timer_config.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/uart.o: ../libraries/uart.c  .generated_files/flags/default/46888faaab858ce9558d04c1057158e202bbfd19 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/uart.c  -o ${OBJECTDIR}/_ext/754011402/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/uart.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
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
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Final_Project.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)      -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/Final_Project.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Final_Project.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
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
