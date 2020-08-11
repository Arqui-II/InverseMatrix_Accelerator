# ------------------------------------------------------------------------------
# Top Level Simulation Script to source msim_setup.tcl
# ------------------------------------------------------------------------------
set QSYS_SIMDIR obj/default/runtime/sim
source msim_setup.tcl
# Copy generated memory initialization hex and dat file(s) to current directory
file copy -force C:/Users/juanp/Desktop/Arqui2/Proyecto_3/Nios2_accelerator/software/Nios2_accelerator/mem_init/hdl_sim/Nios2_accelerator_RAM.dat ./ 
file copy -force C:/Users/juanp/Desktop/Arqui2/Proyecto_3/Nios2_accelerator/software/Nios2_accelerator/mem_init/Nios2_accelerator_RAM.hex ./ 
