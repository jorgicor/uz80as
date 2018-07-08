	.org 0

	.db 2*-3, -2*-3
	.db -+1
 	; .db 1/0

        ;              0              1
	.db $7FFFFFFF>>32, $7FFFFFFF>>33

	;              2               0                   0
	.db $7FFFFFFF>>-30, $7FFFFFFF>>-31, $7FFFFFFF>>-32

	.db ***
	.db 2**

	.dw "\377"

label12;comment

#define ALLOW(a,ab,_a,_7,_.)	a+ab+_a+_7+_.

	.db ALLOW(1,2,3,4,5)

	.end

