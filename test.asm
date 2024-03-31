LXI H, 0040H
MOV A,M
INX H        ; This is comment
MOV B,M
L1: CMP B 
JZ OUT        
JC L2
SUB B
JMP L1
L2: MOV C,B
MOV B,A
MOV A,C
JMP L1
OUT: STA 0050H
HLT