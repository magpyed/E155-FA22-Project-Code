if {[catch {

# define run engine funtion
source [file join {C:/lscc/radiant/3.2} scripts tcl flow run_engine.tcl]
# define global variables
global para
set para(gui_mode) 1
set para(prj_dir) "C:/Users/lserafini/Desktop/E155_Project/fpga/final_proj_fpga"
# synthesize IPs
# synthesize VMs
# synthesize top design
file delete -force -- final_proj_fpga_impl_1.vm final_proj_fpga_impl_1.ldc
run_engine_newmsg synthesis -f "final_proj_fpga_impl_1_lattice.synproj"
run_postsyn [list -a iCE40UP -p iCE40UP5K -t SG48 -sp High-Performance_1.2V -oc Industrial -top -w -o final_proj_fpga_impl_1_syn.udb final_proj_fpga_impl_1.vm] "C:/Users/lserafini/Desktop/E155_Project/fpga/final_proj_fpga/impl_1/final_proj_fpga_impl_1.ldc"

} out]} {
   runtime_log $out
   exit 1
}
