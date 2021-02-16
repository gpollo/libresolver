cmp k5,%r1
ja k4
lea k3(%rip),%r2
movslq k1(%r2,%r1,k2),%r1
add %r2,%r1
jmp %r1