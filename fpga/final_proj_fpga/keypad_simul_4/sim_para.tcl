lappend auto_path "C:/lscc/radiant/3.2/scripts/tcl/simulation"
package require simulation_generation
set ::bali::simulation::Para(DEVICEPM) {ice40tp}
set ::bali::simulation::Para(DEVICEFAMILYNAME) {iCE40UP}
set ::bali::simulation::Para(PROJECT) {keypad_simul_4}
set ::bali::simulation::Para(PROJECTPATH) {C:/Users/lserafini/Desktop/E155_Project/fpga/final_proj_fpga}
set ::bali::simulation::Para(FILELIST) {"C:/Users/lserafini/Desktop/E155_Project/fpga/final_proj_fpga/source/impl_1/final_proj.sv" "C:/Users/lserafini/Desktop/E155_Project/fpga/final_proj_fpga/source/impl_1/testbench.sv" }
set ::bali::simulation::Para(GLBINCLIST) {}
set ::bali::simulation::Para(INCLIST) {"none" "none"}
set ::bali::simulation::Para(WORKLIBLIST) {"work" "" }
set ::bali::simulation::Para(COMPLIST) {"VERILOG" "VERILOG" }
set ::bali::simulation::Para(LANGSTDLIST) {"System Verilog" "" }
set ::bali::simulation::Para(SIMLIBLIST) {pmi_work ovi_ice40up}
set ::bali::simulation::Para(MACROLIST) {}
set ::bali::simulation::Para(SIMULATIONTOPMODULE) {testbench}
set ::bali::simulation::Para(SIMULATIONINSTANCE) {}
set ::bali::simulation::Para(LANGUAGE) {VERILOG}
set ::bali::simulation::Para(SDFPATH)  {}
set ::bali::simulation::Para(INSTALLATIONPATH) {C:/lscc/radiant/3.2}
set ::bali::simulation::Para(MEMPATH) {}
set ::bali::simulation::Para(UDOLIST) {}
set ::bali::simulation::Para(ADDTOPLEVELSIGNALSTOWAVEFORM)  {1}
set ::bali::simulation::Para(RUNSIMULATION)  {1}
set ::bali::simulation::Para(SIMULATIONTIME)  {100}
set ::bali::simulation::Para(SIMULATIONTIMEUNIT)  {ns}
set ::bali::simulation::Para(ISRTL)  {1}
set ::bali::simulation::Para(HDLPARAMETERS) {}
::bali::simulation::ModelSim_Run
