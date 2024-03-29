// Copyright 1986-2017 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2017.4 (win64) Build 2086221 Fri Dec 15 20:55:39 MST 2017
// Date        : Sat Mar 17 15:40:59 2018
// Host        : Peter running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode synth_stub
//               C:/Users/Peter/OneDrive/ECE532_Demo/Nexys-Video-DMA-BT-FFT-Full-Precision/Nexys-Video-DMA/src/bd/design_1/ip/design_1_xlconcat_0_0/design_1_xlconcat_0_0_stub.v
// Design      : design_1_xlconcat_0_0
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7a200tsbg484-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* X_CORE_INFO = "xlconcat_v2_1_1_xlconcat,Vivado 2017.4" *)
module design_1_xlconcat_0_0(In0, In1, dout)
/* synthesis syn_black_box black_box_pad_pin="In0[0:0],In1[6:0],dout[7:0]" */;
  input [0:0]In0;
  input [6:0]In1;
  output [7:0]dout;
endmodule
