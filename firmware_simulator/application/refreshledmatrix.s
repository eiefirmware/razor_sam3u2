;-----------------------------------------------------------------------------
; RefreshLedMatrix(unsigned long int x)
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
;	- R0 is an unsigned value holding the number of bytes to send
; - Output pin is fixed (
;
; Promises:
; - r0 is destroyed during this code, thus the function is not re-entrant.
; - r1 is destroyed during this code

#define OVERHEAD  7
#define OUTPUT_PIN 0x03

RefreshLedMatrix				; [1] Function entry
		
kill_x_loop				; [3 cycle loop]
	SUBS	r0, r0, #3		; [1] Subtract the loop cycle cost from the counter
	BPL	kill_x_loop		; [2] Check if positive or zero and repeat if so

kill_x_cycles_end			;
	MOV	PC, r14			; [1] Move the return address back to the PC
  
	END
