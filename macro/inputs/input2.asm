START
MACRO
INCR &A
&A = &A + 1
MEND

MACRO
DOUBLEINCR &X
; First increment
INCR &X
; Increment again
INCR &X
MEND

MACRO
NESTED &P,&Q
; Call a simple increment
INCR &P
; Now call a macro that itself calls another macro
DOUBLEINCR &Q
MEND

DATA1 5
DATA2 10
NESTED DATA1, DATA2
END
