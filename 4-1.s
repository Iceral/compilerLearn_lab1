.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"

.text

read:
    li $v0, 4
    la $a0, _prompt
    syscall
    li $v0, 5
    syscall
    jr $ra

write:
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, _ret
    syscall
    move $v0, $0
    jr $ra


.globl main
main:
    addi $sp, $sp, -40
    sw $ra, 36($sp)
    sw $fp, 32($sp)
    addi $fp, $sp, 32
    li $t8, 0
    sw $t8, -8($fp)
    li $t8, 1
    sw $t8, -12($fp)
    li $t8, 0
    sw $t8, -16($fp)
    li $t8, 0
    sw $t8, -20($fp)
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    jal read
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    sw $v0, -24($fp)
    lw $t8, -24($fp)
    sw $t8, -20($fp)
    j label1
label2:
    lw $t8, -8($fp)
    lw $t9, -12($fp)
    add $t8, $t8, $t9
    sw $t8, -28($fp)
    lw $t8, -28($fp)
    sw $t8, -32($fp)
    lw $a0, -12($fp)
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    jal write
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    lw $t8, -12($fp)
    sw $t8, -8($fp)
    lw $t8, -32($fp)
    sw $t8, -12($fp)
    lw $t8, -16($fp)
    li $t9, 1
    add $t8, $t8, $t9
    sw $t8, -36($fp)
    lw $t8, -36($fp)
    sw $t8, -16($fp)
    j label1
label1:
    lw $t8, -16($fp)
    lw $t9, -20($fp)
    blt $t8, $t9, label2
    j label3
label3:
    li $v0, 0
    lw $ra, 36($sp)
    lw $fp, 32($sp)
    addi $sp, $sp, 40
    jr $ra
