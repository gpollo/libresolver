# To get instruction's bytes, execute the following commands.
#
# $ gcc main.s -g -o main
# $ objdump -D main | grep -a10 main
#

.global main

.text
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
    cmp    $7,%edx
    ja     10
    lea    1000(%rip),%rbx
    mov    %edx,%ecx
    movslq 100(%rbx,%rcx,4),%rax
    add    %rbx,%rax
    jmpq   *%rax
    