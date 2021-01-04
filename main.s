# To get instruction's bytes, execute the following commands.
#
# $ gcc main.s -g -o main
# $ objdump -D main | grep -a10 main
#

.section .text
    .global main
main:
    # case 1
    #cmp    $5,%ecx
    #ja     10
    #mov    %ecx,%eax
    #lea    1337(%rip),%rbx
    #movslq (%rbx,%rax,4),%rax
    #add    %rbx,%rax
    #jmpq   *%rax

    # case 1
    #cmp    $5,%eax
    #ja     10
    #mov    %eax,%esi
    #lea    1337(%rip),%rdx
    #movslq (%rdx,%rsi,4),%rsi
    #add    %rdx,%rsi
    #jmpq   *%rsi

    # case 2
    #cmp    $5,%ecx
    #ja     10
    #lea    1000(%rip),%rbx
    #mov    %ecx,%ecx
    #movslq 420(%rbx,%rcx,4),%rax
    #add    %rbx,%rax
    #jmpq   *%rax

    # case 3
    #cmpl   $6,20(%rcx)
    #ja     10
    #mov    20(%rcx),%eax
    #lea    1000(%rip),%rbx
    #movsxd 100(%rbx,%rax,4),%rax
    #add    %rbx,%rax
    #jmpq   *%rax

    # case 4
    #cmp    $8,%rcx
    #ja     10
    #lea    1000(%rip),%rbx
    #movslq 100(%rbx,%rcx,4),%rax
    #add    %rbx,%rax
    #jmpq   *%rax

    # case 5
    #cmp    $7,%edx
    #ja     10
    #lea    1000(%rip),%rbx
    #mov    %edx,%ecx
    #movslq 100(%rbx,%rcx,4),%rax
    #add    %rbx,%rax
    #jmpq   *%rax
    
    # case 6
    #lea    1000(%rip),%rbx
    #cmpl   $6,50(%rax)
    #ja     10
    #mov    50(%rax),%eax
    #movslq 100(%rbx,%rax,4),%rax
    #add    %rbx,%rax
    #jmpq   *%rax

    # case 7
    #lea    1000(%rip),%rbx
    #cmp    $6,%al
    #ja     10
    #movzbl %al,%eax
    #movslq 100(%rbx,%rax,4),%rax
    #add    %rbx,%rax
    #jmpq   *%rax

    # case 8
    #cmpl   $6,22(%rip)
    #ja     10
    #mov    10(%rip),%eax
    #lea    1000(%rip),%rbx
    #movslq 100(%rbx,%rax,4),%rax
    #add    %rbx,%rax
    #jmpq   *%rax

    # case 9
    #lea    1000(%rip),%rbx
    #cmpl   $8,20(%rcx)
    #ja     10
    #mov    20(%rcx),%eax
    #movslq 100(%rbx,%rax,4),%rax
    #add    %rbx,%rax
    #jmpq   *%rax

    # case 10
    #cmp    $6,%eax
    #ja     10
    #lea    1000(%rip),%rbx
    #movslq 100(%rbx,%rax,4),%rax
    #add    %rbx,%rax
    #jmpq   *%rax

    # case 11
    cmp    $9,%al
    ja     10
    lea    1000(%rip),%rbx
    movzbl %al,%eax
    movslq 100(%rbx,%rax,4),%rax
    add    %rbx,%rax
    jmpq   *%rax
