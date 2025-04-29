		SECTION	.data
		EXTERN	inbound_queue	; (defined in main.C)
data		DB	0		; put rcvd byte here

		SECTION	.text
		ALIGN	16
		BITS	32

BASE_PORT	EQU	3F8h		; we have this in our lab

LSR_PORT	EQU	BASE_PORT+5
RBR_PORT	EQU	BASE_PORT
THR_PORT	EQU	BASE_PORT

; ---------------------------------------------------------------------
; void SerialPut(char ch)
; ---------------------------------------------------------------------
; This function uses programmed waiting loop I/O
; to output the ASCII character 'ch' to the UART.

		GLOBAL	SerialPut

SerialPut:	

		push EBP 
		mov EBP, ESP
	
CHECK_THRE:
		mov DX, LSR_PORT
		in AL, DX ; Read LSR port
		shr AL, 6
		
		jnc CHECK_THRE ; Jump to start if THRE not set

		mov AL, [EBP + 8] ; load ch into AL
		mov DX, THR_PORT
		out DX, AL

		pop EBP
		ret

; ---------------------------------------------------------------------
; void interrupt SerialISR(void)
; ---------------------------------------------------------------------
; This is an Interrupt Service Routine (ISR) for
; serial receive interrupts.  Characters received
; are placed in a queue by calling Enqueue(char).

		GLOBAL	SerialISR
		EXTERN	QueueInsert	; (provided by LIBPC)

SerialISR:	STI             	; Enable (higher-priority) IRQs 

		pusha

		mov DX, LSR_PORT
		in AL, DX
		shr AL, 1
		jnc END_OF_INTERRUPT

		mov DX, RBR_PORT
		in AL, DX
		mov [data], AL
		push data
		push dword [inbound_queue]


		CALL	QueueInsert
		ADD	ESP,8

END_OF_INTERRUPT:
		mov AL, 0x20
		mov DX, 0x20
		out DX, AL
		sti
		
		popa
		iret


		; <your code here>	; (4) Enable lower priority interrupts
		; <your code here>	;       (Send Non-Specific EOI to PIC)
		; <your code here>	; (5) Restore all registers
		; <your code here>	; (6) Return to interrupted code
