  MODULE  RefreshLedMatrix
  SECTION .text : CODE : NOROOT(2)
  THUMB

	PUBLIC	RefreshLedMatrix


;-----------------------------------------------------------------------------
; RefreshLedMatrix(unsigned long int *u32Address, unsigned long int u32Bytes)
; 
; The processor is fully awake during this time and simply kills instructions
; by running through a loop.  
; To be compatible with C code, the function input parameters must use r0 and r1 as 
; convention, that's where the compiler will put them.
; All timing is shown in [ ].  The function overhead is subtracted off the value passed
; in, then the value is reduced by the number of cycles it takes for each run of the kill_x_loop
; There will be a remainder error here, but it's at most 2 cycles.
;
; Requires:
;	- r0 is the starting address of bytes to send
; - r1 is the number of bytes to send (decremented during function)
; - r2 is the address of SODR
; - r3 counts bit shifts for each byte
; - r4 is the current byte from *r0 being sent
; - r5 is the pin number that is written on SODR and CODR (output bit)
; - r6 is for-loop counter
; - Output pin in r5 is fixed (PB_03_BLADE_AN0)
; - Interrupts can be turned off
;
; Promises:
; - r0-r5 are destroyed during this code, thus the function is not re-entrant.
; - Interrupts are disabled while this runs, but re-enabled upon exit
; - If u32Bytes > 120, this will start violating 1ms timing (480us per 60 bytes)


#define OVERHEAD            7                 
#define OUTPUT_PIN          0x00000008
#define AT91C_PIOB_SODR     0x400E0E30
#define AT91C_PIOB_CODR     0x400E0E34
#define SODR_CODR_OFFSET    (AT91C_PIOB_CODR - AT91C_PIOB_SODR)

RefreshLedMatrix				            ; Function entry
  LDR   r2, =AT91C_PIOB_SODR        ; 59: Load r2 with the address of SODR
  LDR   r5, =OUTPUT_PIN             ; r5 to hold the pin number for writes to SODR/CODR
  CPSID i                           ; 62: Disable interrupts

ByteLoopInit                         ; Get current byte ready
  CBZ   r1, RefreshDone              ; 64, 529: Check if u32Bytes is 0 [1 cycle]
  SUB   r1, r1, #1                   ; 65: Decrement byte counter [1]
  MOV   r3, #8                       ; 66: r3 will count shifts [1]
  LDRB  r4, [r0]                     ; 67: r4 = *r0 (get current byte to send) [2]
  ADD   r0, r0, #1                   ; 69, 534: Advance r0 to next byte address [1]
  LSL   r4, r4, #24                  ; 70, 535: Setup MSB to bit 32 [1]
 
ByteLoop
  LSLS  r4, r4, #1                   ; 130, 536; Shift first bit out to carry [1]
  STR   r5, [r2]                     ; 72, 130, 188, 246, 304, 362, 420, 478, 537, 595, ..., 943, 1002 [1] - write SODR regardless
  BHI   ONE_BIT                      ; 73, 131: Branch if C = 1 [1 no branch, 4 if branch]


ZERO_BIT                             ; 14 cycles high, 44 cycles low
  MOV   r6, #2                       ; 132: Load r6 loop counter to kill cycles [1]

ZeroBitShortLoop
  SUBS  r6, r6, #1                   ; : Decrement r6, update flags
  BPL   ZeroBitShortLoop             ; : Loop if not zero

  STR   r5, [r2, #SODR_CODR_OFFSET]  ; 145, 203, 377 [1]: write CODR 
  MOV   r6, #4                       ; 146 [1]: Load r6 loop counter to kill cycles

ZeroBitLongLoop
  SUBS  r6, r6, #1                   ; 147 [1]: Decrement r6, update flags
  BPL   ZeroBitLongLoop              ; Loop if not zero
  NOP                                ;
  NOP                                ;
  NOP                                ;
  B     ByteLoopEnd                  ; 169 [4] exit out to next bit or byte


ONE_BIT                              ; 44 cycles high, 14 cycles low
  MOV   r6, #7                       ; 77 [1]: Load r6 loop counter to kill cycles
OneBitLoop
  SUBS  r6, r6, #1                   ; : Decrement r6, update flags
  BPL   OneBitLoop                   ; : Loop if not zero
  NOP                                ; 
  STR   r5, [r2, #SODR_CODR_OFFSET]  ; 116, 290, 348 [1]: write CODR
                                     ; Proceed to ByteLoopEnd

ByteLoopEnd                          ; From here it is either x cyles until next bit; or y cycles for start of new byte
  SUB   r3, #1                       ; 117, 172, 523: Decrement bit counter [1]
  CMP   r3, #0                       ; 173, 524: [1]
  BEQ   ByteLoopInit                 ; 525: If bit counter is 0, time for new byte
                                     ; Kill a few more cycles to balance ByteLoopInit instruction cycles
  NOP                                ;
  NOP                                ;
  NOP                                ;
  NOP                                ;
  NOP                                ;
  NOP                                ;
  B     ByteLoop                     ; 



RefreshDone
  CPSIE i                             ; Re-enable interrupts
  MOV	  PC, r14			                  ; [1] Move the return address back to the PC
  
  END