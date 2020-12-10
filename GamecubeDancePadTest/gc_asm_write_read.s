; no attempt to reserve registers!
; void gc_asm_write_read(uint32_t *buff, uint8_t len, uint32_t *read_buff, uint8_t read_buff_len)
; R0-R3 inputs
; R1 and R3 are decremented as the buffers are looped through
; R4 #0x00000001 bit mask for p10
; R5 my personal "temp" register for quick calculations, if not used in succession it should be considered clobbered
; R6 0x2009C020 GPIO port 2 base address
; R7 address offset to write and read from buff and read_buff_len, it increments whenever R0 or R3 decrements


    AREA asm_func, CODE, READONLY
    EXPORT gc_asm_write_read
gc_asm_write_read
    PUSH    {R4-R7, LR}
    SUB     R7, R7, R7
    
WRITE_LOOP
    
; Load GPIO Port 1 base address in register R1 
    LDR     R6, =0x2009C000 ; 0x2009C020 = GPIO port 1 base address
    MOV.W   R4, #0x00000001   ; 0x040000 = 1<<18 all "0"s with a "1" in bit 18

    LDR     R5, [R0, R7]
    
    CMP     R5, #0          ; value == 0 ?
    BNE     SEND_HIGH
    STR     R4, [R6,#0x1c]  ; set low
    BL      WAIT_ONE_US_MINUS_OVERHEAD
    BL      WAIT_ONE_US
    BL      WAIT_ONE_US
    B       _END_SEND_HIGH
SEND_HIGH
    STR     R4, [R6,#0x1c]  ; set low
    BL      WAIT_ONE_US
    STR     R4, [R6,#0x18]  ; set high
    BL      WAIT_ONE_US_MINUS_OVERHEAD
    BL      WAIT_ONE_US
    
_END_SEND_HIGH
    STR     R4, [R6,#0x18]
    BL       WAIT_ONE_US
    
    SUB     R1, R1, #0x01 ; going through the array backwards (it was reversed in the c layer)
    ADD     R7, R7, #0x04 ; increment address that we access using R7
    
    CMP     R1, #0
    BLT     FINISH_WRITE
    B       WRITE_LOOP
    
FINISH_WRITE
    ; send one high bit to terminate
    STR     R4, [R6,#0x1C]  ; set low
    BL       WAIT_ONE_US
    STR     R4, [R6,#0x18]  ; set high
    BL       WAIT_ONE_US_MINUS_OVERHEAD
    ;end of send one high bit to terminate

; begin read
    
    SUB     R7, R7, R7          ; offset to write into the array given
    SUB     R3, R3, #1
    B       FIRST_READ
    
READ_LOOP
    SUB     R3, R3, #1  ;R3 is the number of bits to read
    CMP     R3, #0
    BLT     FINISH
    
    LDR     R5, =0x999999
WAIT_FOR_HIGH
    SUB     R5, R5, #1          ; only check 0x100 times, 0x100 is arbitrarily chosen right now
    CMP     R5, #0
    BEQ     FINISH              ; TODO: add in fail case here!
    
    LDR     R5, [R6, #0x14]     ; load pin states
    AND     R5, R5, R4          ; get only p10
    CMP     R5, #0
    BEQ     WAIT_FOR_HIGH
    
    ; wait for 2us to get the bit we want
    BL      WAIT_TWO_US_MINUS_OVERHEAD

FIRST_READ
    LDR     R5, [R6, #0x14]     ; load pin states
    AND     R5, R5, R4          ; get only p10
    CMP     R5, #0
    BEQ     STORE_ZERO
    LDR     R5, =0x01
    STR     R5, [R2, R7]
    BL      WAIT_ONE_US_MINUS_OVERHEAD    
    B       END_STORE_ZERO
STORE_ZERO
    LDR     R5, =0x00
    STR     R5, [R2, R7]
END_STORE_ZERO
    ADD     R7, R7, #0x04       ; increment address stored
    B READ_LOOP


FINISH   
    POP     {R4-R7, LR}
    BX      LR
    
WAIT_ONE_US_MINUS_OVERHEAD
    LDR     R5, =0x8
_WAIT_ONE_US_MINUS_OVERHEAD
    SUB     R5, R5, #1
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    CMP     R5, #0
    BNE     _WAIT_ONE_US_MINUS_OVERHEAD
    BX      LR
    
WAIT_ONE_US
    LDR     R5, =0x9
_WAIT_ONE_US
    SUB     R5, R5, #1
    NOP
    NOP
    NOP
    NOP
    NOP
    CMP     R5, #0
    BNE     _WAIT_ONE_US
    BX      LR
    
WAIT_TWO_US_MINUS_OVERHEAD
    LDR     R5, =0x17
_WAIT_TWO_US_MINUS_OVERHEAD
    SUB     R5, R5, #1
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    CMP     R5, #0
    BNE     _WAIT_TWO_US_MINUS_OVERHEAD
    BX      LR
    
    
    END