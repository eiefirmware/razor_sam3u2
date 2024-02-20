;-----------------------------------------------------------------------------
; RefreshLedMatrix(unsigned long int u32Address, unsigned long int u32Bytes)
; 
; The processor is fully awake during this time and simply kills instructions
; by running through a loop.  
; To be compatible with C code, the function parameter must use r0 as, by 
; convention, that's where the compiler would put it.
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


#define OVERHEAD  7
#define OUTPUT_PIN 0x00000008
#define SODR_CODR_OFFSET  (AT91C_PIOB_CODR - AT91C_PIOB_SODR)

RefreshLedMatrix				            ; Function entry
  LDR   r2, =AT91C_PIOB_SODR        ; Load r2 with the address of SODR
  LDR   r5, #OUTPUT_PIN             ; r5 to hold the pin number for writes to SODR/CODR
  CPSID i                           ; Disable interrupts

ByteLoopInit                         ; Get current byte ready
  CBZ   r1, RefreshDone              ; Check if u32Bytes is 0
  SUB   r1, r1, #1                   ; Decrement byte counter
  MOV   r3, #8                       ; r3 will count shifts
  LDRB  r4, [r0]                     ; r4 = *r0 (get current byte to send)
  ADD   r0, r0, #1                   ; Advance r0 to next byte address
  LSL   r4, r4, #24                  ; Setup MSB to bit 32
 
ByteLoop
  LSLS  r4, r4, #1                   ; Shift first bit out to carry
  STR   r5, [r2]                     ; x cycles to here; write SODR regardless
  BLO   ZERO_BIT                     ; Branch if C = 0

ONE_BIT                              ; 44 cycles high, 14 cycles low
  MOV   r6, #10                      ; Load r6 loop counter to kill cycles
OneBitLoop
  SUBS  r6, r6, #1                   ; Decrement r6, update flags
  BPL   OneBitLoop                   ; Loop if not zero
  STR   r5, [r2, #SODR_CODR_OFFSET]  ; x cycles to here; write CODR 
                                     ; Proceed to ByteLoopEnd

ByteLoopEnd                          ; From here it is either x cyles until next bit; or y cycles for start of new byte
  SUB   r3, #1                       ; Decrement byte counter
  CBZ   r3, ByteLoopInit             ; If bit counter is 0, time for new byte
                                     ; Kill a few more cycles
  B     ByteLoop                     ;

ZERO_BIT                             ; 14 cycles high, 44 cycles low
  MOV   r6, #3                       ; Load r6 loop counter to kill cycles

ZeroBitShortLoop
  SUBS  r6, r6, #1                   ; Decrement r6, update flags
  BPL   ZeroBitShortLoop             ; Loop if not zero

  STR   r5, [r2, #SODR_CODR_OFFSET]  ; x cycles to here; write CODR 
  MOV   r6, #10                      ; Load r6 loop counter to kill cycles

ZeroBitLongLoop
  SUBS  r6, r6, #1                   ; Decrement r6, update flags
  BPL   ZeroBitLongLoop              ; Loop if not zero
  B     ByteLoopEnd                  ; x cycles to here, exit out to next bit or byte


RefreshDone
  CPSIE i                             ; Re-enable interrupts
  MOV	  PC, r14			                  ; [1] Move the return address back to the PC
  
END



kill_x_loop				; [3 cycle loop]
SUBS	r0, r0, #3		; [1] Subtract the loop cycle cost from the counter
BPL	kill_x_loop		; [2] Check if positive or zero and repeat if so

kill_x_cycles_end			;


; Loop of u32Bytes
; Point to current byte
; Load byte to working register shifting by 24 to get 8 bits in MSB
; loop 8 times:
;  shift once
;  respond to carry by branch to 0 loop or 1 loop
;  set SODR
;  kill x cycles
;  set CODR
;  kill y cycles
;  update counter; if 0, check if done; if not, load new byte