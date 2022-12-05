onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -radix binary /testbench/dut/clk
add wave -noupdate -radix binary /testbench/dut/reset
add wave -noupdate -radix binary /testbench/dut/done
add wave -noupdate -radix binary /testbench/dut/cols
add wave -noupdate -radix binary /testbench/dut/rows
add wave -noupdate -radix binary /testbench/dut/leds
add wave -noupdate -radix binary /testbench/dut/crct_pwd
add wave -noupdate -radix binary /testbench/dut/incrct_pwd
add wave -noupdate -radix binary /testbench/dut/state
add wave -noupdate -radix binary /testbench/dut/nextstate
add wave -noupdate -radix hexadecimal /testbench/dut/num1
add wave -noupdate -radix hexadecimal /testbench/dut/num2
add wave -noupdate -radix hexadecimal /testbench/dut/num3
add wave -noupdate -radix hexadecimal /testbench/dut/num4
add wave -noupdate -radix binary /testbench/dut/nextnum
add wave -noupdate -radix binary /testbench/dut/synched_cols
add wave -noupdate -radix binary /testbench/dut/synched_rows
add wave -noupdate -radix binary /testbench/dut/buttonpress
add wave -noupdate -radix hexadecimal /testbench/dut/counter
add wave -noupdate -radix decimal /testbench/dut/threshold
add wave -noupdate -radix hexadecimal /testbench/dut/pwd1
add wave -noupdate -radix hexadecimal /testbench/dut/pwd2
add wave -noupdate -radix hexadecimal /testbench/dut/pwd3
add wave -noupdate -radix hexadecimal /testbench/dut/pwd4
add wave -noupdate -radix hexadecimal /testbench/dut/numpressed
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {0 ns} 0}
quietly wave cursor active 1
configure wave -namecolwidth 207
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ns
update
WaveRestoreZoom {0 ns} {864 ns}
