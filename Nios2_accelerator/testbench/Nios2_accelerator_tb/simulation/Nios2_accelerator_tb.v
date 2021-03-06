// Nios2_accelerator_tb.v

// Generated using ACDS version 18.0 614

`timescale 1 ps / 1 ps
module Nios2_accelerator_tb (
	);

	wire    nios2_accelerator_inst_clk_bfm_clk_clk;       // Nios2_accelerator_inst_clk_bfm:clk -> [Nios2_accelerator_inst:clk_clk, Nios2_accelerator_inst_reset_bfm:clk]
	wire    nios2_accelerator_inst_reset_bfm_reset_reset; // Nios2_accelerator_inst_reset_bfm:reset -> Nios2_accelerator_inst:reset_reset_n

	Nios2_accelerator nios2_accelerator_inst (
		.clk_clk       (nios2_accelerator_inst_clk_bfm_clk_clk),       //   clk.clk
		.reset_reset_n (nios2_accelerator_inst_reset_bfm_reset_reset)  // reset.reset_n
	);

	altera_avalon_clock_source #(
		.CLOCK_RATE (50000000),
		.CLOCK_UNIT (1)
	) nios2_accelerator_inst_clk_bfm (
		.clk (nios2_accelerator_inst_clk_bfm_clk_clk)  // clk.clk
	);

	altera_avalon_reset_source #(
		.ASSERT_HIGH_RESET    (0),
		.INITIAL_RESET_CYCLES (50)
	) nios2_accelerator_inst_reset_bfm (
		.reset (nios2_accelerator_inst_reset_bfm_reset_reset), // reset.reset_n
		.clk   (nios2_accelerator_inst_clk_bfm_clk_clk)        //   clk.clk
	);

endmodule
